#include <Windows.h>
#include <iostream>
using namespace std;

int i = 0;
HANDLE hMapObject, hFile;
LPVOID lpBase;
PIMAGE_DOS_HEADER dosHeader;
PIMAGE_NT_HEADERS ntHeader;
IMAGE_FILE_HEADER header;
IMAGE_OPTIONAL_HEADER opHeader;
PIMAGE_OPTIONAL_HEADER pOptionalHeader;
PIMAGE_SECTION_HEADER pSecHeader;
PIMAGE_IMPORT_DESCRIPTOR pImport;
PIMAGE_EXPORT_DIRECTORY pExport;
OPENFILENAME ofn;
char szFile[100];
DWORD RvaToOffset(DWORD rva, PIMAGE_SECTION_HEADER psh, PIMAGE_NT_HEADERS pnt)
{
	size_t i = 0;
	PIMAGE_SECTION_HEADER pSeh;
	if (rva == 0)
	{
		return (rva);
	}
	pSeh = psh;
	for (i = 0; i < pnt->FileHeader.NumberOfSections; i++)
	{
		if (rva >= pSeh->VirtualAddress && rva < pSeh->VirtualAddress +
			pSeh->Misc.VirtualSize)
		{
			break;
		}
		pSeh++;
	}
	return (rva - pSeh->VirtualAddress + pSeh->PointerToRawData);
}
int main()
{
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPSTR)szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = ".exe";

	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_EXPLORER | OFN_HIDEREADONLY;
	if (GetOpenFileName(&ofn))
	{
		hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) { cout << endl << "ERROR : Could not open the file" << endl; };

		//Mapping EXE file to Memory
		hMapObject = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		lpBase = MapViewOfFile(hMapObject, FILE_MAP_READ, 0, 0, 0);
		DWORD byteread, size = GetFileSize(hFile, NULL);
		PVOID virtualpointer = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		ReadFile(hFile, virtualpointer, size, &byteread, NULL);
		CloseHandle(hFile);
		PIMAGE_NT_HEADERS           ntheaders = (PIMAGE_NT_HEADERS)(PCHAR(virtualpointer) + PIMAGE_DOS_HEADER(virtualpointer)->e_lfanew);
		PIMAGE_SECTION_HEADER       pSech = IMAGE_FIRST_SECTION(ntheaders);
		PIMAGE_IMPORT_DESCRIPTOR    pImportDescriptor;
		__try
		{
			if (ntheaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size != 0)
			{
				pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)virtualpointer + \
					RvaToOffset(ntheaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, pSech, ntheaders));
				LPSTR libname[256];
				size_t i = 0;
				cout << endl << "Listed All Functions File Imported: " << endl << endl;
				cout << "==============================================" << endl << endl;
				cout << "Library Name   :" << endl;
				while (pImportDescriptor->Name != NULL)
				{
					libname[i] = (PCHAR)((DWORD_PTR)virtualpointer + RvaToOffset(pImportDescriptor->Name, pSech, ntheaders));
					cout << "\t" << libname[i] << endl;
					pImportDescriptor++;
					i++;

				}
				system("pause");

			}
			else
			{
				cout << "No Import Table!\n" << endl;
				return 1;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			if (EXCEPTION_ACCESS_VIOLATION == GetExceptionCode())
			{
				cout << "Exception: EXCEPTION_ACCESS_VIOLATION" << endl;
				return 1;
			}

		}
		if (virtualpointer)
			VirtualFree(virtualpointer, size, MEM_DECOMMIT);

		return 0;
		system("pause");
	}
	else goto end;

end:
	UnmapViewOfFile(lpBase);
	CloseHandle(hMapObject);

}