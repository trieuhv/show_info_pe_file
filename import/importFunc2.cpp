#include <iostream>
#include <windows.h>
using namespace std;
HANDLE hFile, mapObject;
OPENFILENAME ofn;
char szFile[100];
LPVOID lpBase;
DWORD fileSize;
PIMAGE_NT_HEADERS ntHeaders;
PIMAGE_DOS_HEADER dosHeader;
IMAGE_OPTIONAL_HEADER opHeader;
PIMAGE_SECTION_HEADER secHeader;
LPSTR libraryName[256];
int i;
void main() {
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "*.*";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn)) {
		hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE) { cout << "Couldnt open file" << szFile; };
		fileSize = GetFileSize(hFile, NULL);
		lpBase = VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_READWRITE);
		CloseHandle(hFile);
		dosHeader = (PIMAGE_DOS_HEADER)(lpBase);
		ntHeaders = (PIMAGE_NT_HEADERS)((DWORD)lpBase + dosHeader->e_lfanew);
		opHeader = ntHeaders->OptionalHeader;
		secHeader = IMAGE_FIRST_SECTION(ntHeaders);
		PIMAGE_IMPORT_DESCRIPTOR pimportdes;
		if (ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size != 0) {
			pimportdes = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)lpBase + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
			while (pimportdes->Name != NULL) {
				libraryName[i] = (PCHAR)((DWORD)lpBase + pimportdes->Name);
				cout << libraryName[i] << endl;
				i++;
			}
		}

	}
}