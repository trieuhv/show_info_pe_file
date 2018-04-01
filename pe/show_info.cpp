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
PIMAGE_SECTION_HEADER pSecHeader;	
OPENFILENAME ofn;
char szFile[100];


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

		//Open the Exe File 
			hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) { cout << endl <<"ERROR : Could not open the file" << endl; };

		//Mapping EXE file to Memory
			hMapObject = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
			lpBase = MapViewOfFile(hMapObject, FILE_MAP_READ, 0, 0, 0);
			
		//Info about Optional Header
			cout << endl << "INFORMATION OF FILE" << endl;
			cout << endl << "===============================================================================" << endl;
			dosHeader = (PIMAGE_DOS_HEADER)lpBase;
			ntHeader = (PIMAGE_NT_HEADERS)((DWORD)(dosHeader)+(dosHeader->e_lfanew));
			cout << endl << endl << "PE Optional Header Info............\n-----------------------------------" << endl;

			opHeader = ntHeader->OptionalHeader;

			cout << "Pointer to Entry Point : " << opHeader.AddressOfEntryPoint << endl;
			cout << "Check Sum: " << opHeader.CheckSum << endl;
			cout << "Image Base : " << opHeader.ImageBase << endl;
			cout << "File Alignment: " << opHeader.FileAlignment << endl;
			cout << "Size of Image: " << opHeader.SizeOfImage << endl;

			cout << endl << "===============================================================================" << endl << endl;
			cout << "Sections Header Info......" << endl << "--------------------------" << endl;

			for (pSecHeader = IMAGE_FIRST_SECTION(ntHeader), i = 0; i < ntHeader->FileHeader.NumberOfSections; i++, pSecHeader++) {
				cout << endl << endl;
				cout << "Section Header name : " <<  pSecHeader->Name << endl;
				
				cout <<  "Characteristics : Contain Code ";
				if ((pSecHeader->Characteristics & 0x20) == 0x20)cout << "|| Executable Code ";
				if ((pSecHeader->Characteristics & 0x40) == 0x40)cout << "|| Initialized Data ";
				if ((pSecHeader->Characteristics & 0x80) == 0x80)cout << "|| Uninitialized Data ";
				if ((pSecHeader->Characteristics & 0x80) == 0x80)cout << "|| Uninitialized Data ";
				if ((pSecHeader->Characteristics & 0x200) == 0x200)cout << "|| Comments and Linker Commands ";
				if ((pSecHeader->Characteristics & 0x10000000) == 0x10000000)cout << "|| Shareable Data(via DLLs ";
				if ((pSecHeader->Characteristics & 0x40000000) == 0x40000000)cout << "|| Readable ";
				if ((pSecHeader->Characteristics & 0x80000000) == 0x80000000)cout << "|| Writable ";
				cout << endl;

				cout << "Raw Address : " << pSecHeader->PointerToRawData << endl;

				cout << "Raw Size : " << pSecHeader->SizeOfRawData << endl;
				
				cout << "Virtual Address(RVA) :" << pSecHeader->VirtualAddress << endl;
				
				cout << "Virtual Size : " << pSecHeader->Misc.VirtualSize << endl;

			}


			cout << endl << "===============================================================================" << endl;
			system("pause");
		}
		else goto end;

	end:
		//UnMaping 
		UnmapViewOfFile(lpBase);
		CloseHandle(hMapObject);

	}