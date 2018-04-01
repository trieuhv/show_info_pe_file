#include <iostream>
#include <Windows.h>
using namespace std;
HANDLE hFile, hMapping;
LPVOID lpBase;
PIMAGE_DOS_HEADER pDosHeader;
PIMAGE_NT_HEADERS ntHeader;
IMAGE_OPTIONAL_HEADER opHeader;
PIMAGE_SECTION_HEADER pSecHeader;
PIMAGE_IMPORT_DESCRIPTOR pImportDes;
int i;
DWORD VirtualSize, byteRead;
LPSTR Name[256];
void pe() {
	LPCTSTR filename = "D:\\pe.exe";
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	lpBase = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	cout << endl << "Information of File" << endl;
	cout << endl << "===============================" << endl;
	pDosHeader = (PIMAGE_DOS_HEADER)lpBase;
	ntHeader = (PIMAGE_NT_HEADERS)((DWORD)lpBase + (pDosHeader->e_lfanew));
	cout << endl << endl << "PE Optional Header Infor: " << endl;
	opHeader = ntHeader->OptionalHeader;
	cout << "PointerToEntryPoint: " << opHeader.AddressOfEntryPoint << endl;
	cout << "Check sum: " << opHeader.CheckSum << endl;
	cout << "File Alignment: " << opHeader.FileAlignment << endl;
	cout << "Size Of Image: " << opHeader.SizeOfImage << endl;
	cout << endl << endl << "Section infor: " << endl;
	for (pSecHeader = IMAGE_FIRST_SECTION(ntHeader), i = 0; i < ntHeader->FileHeader.NumberOfSections; i++, pSecHeader++) {
		cout << "Section Header Name: " << pSecHeader->Name << endl;
		cout << "Characteristics: Contain code";
		if ((pSecHeader->Characteristics & 0x20) == 0x20) {
			cout << "|| Executable Code";
		}
		if ((pSecHeader->Characteristics & 0x40) == 0x40) {
			cout << "|| Initialized Data";
		}
		if ((pSecHeader->Characteristics & 0x80) == 0x80) {
			cout << "|| Unitialized Data";
		}
		if ((pSecHeader->Characteristics & 0x200) == 0x200) {
			cout << "|| Comment and Linker command";
		}
		if ((pSecHeader->Characteristics & 0x10000000) == 0x10000000) {
			cout << "|| Shareable Data(via DLLs)";
		}
		if ((pSecHeader->Characteristics & 0x40000000) == 0x40000000) {
			cout << "|| Readable";
		}
		if ((pSecHeader->Characteristics & 0x80000000) == 0x80000000) {
			cout << "|| Writable";
		}
		cout << endl;
		cout << "Raw Address: " << pSecHeader->PointerToRawData << endl;
		cout << "Raw Size: " << pSecHeader->SizeOfRawData << endl;
		cout << "Virtual Address: " << pSecHeader->VirtualAddress << endl;
		cout << "Virtual Size: " << pSecHeader->Misc.VirtualSize << endl;
		cout << "=========================" << endl << endl;
	}
	system("pause");
}
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
void import() {
	LPCTSTR filename = "D:\\pe.exe";
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	VirtualSize = GetFileSize(hFile, NULL);
	PVOID pVirtual = VirtualAlloc(NULL, VirtualSize, MEM_COMMIT, PAGE_READWRITE);
	ReadFile(hFile, pVirtual, VirtualSize, &byteRead, NULL);
	CloseHandle(hFile);
	ntHeader = (PIMAGE_NT_HEADERS)(PCHAR(pVirtual) + PIMAGE_DOS_HEADER(pVirtual)->e_lfanew);
	pSecHeader = IMAGE_FIRST_SECTION(ntHeader);
	pImportDes = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)pVirtual + RvaToOffset(ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress, pSecHeader, ntHeader));
	size_t i = 0;
	cout << endl << "Functions File Imported: " << endl;
	cout << endl << "==================================" << endl;
	cout << "Function name: " << endl;
	while (pImportDes->Name!=NULL) {
		Name[i] = (PCHAR)((DWORD_PTR)pVirtual + RvaToOffset(pImportDes->Name, pSecHeader, ntHeader));
		cout << "\t" << Name[i] << endl;
		pImportDes++;
		i++;
	}
	system("pause");
}
void export() {

}
void main() {
	pe();
	import();
	export();
}