#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
 
HANDLE hFile;
 
void Error()
{
    char * szError;
    static char szErrorBuf[128];
 
    DWORD dwError = GetLastError();
 
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&szError,
        0, NULL );
 
    _snprintf(szErrorBuf, sizeof(szErrorBuf), "\nExits with error: %s\n", szError);
    printf("%s", szErrorBuf);
 
    LocalFree(szError);
    CloseHandle(hFile);
 
    _getch();
    exit(1);
}
 
DWORD RvaToOffset(IMAGE_NT_HEADERS32 * pNtHdr, DWORD dwRVA)
{
     int i;
     WORD wSections;
     PIMAGE_SECTION_HEADER pSectionHdr;
 
     pSectionHdr = IMAGE_FIRST_SECTION(pNtHdr);
     wSections = pNtHdr->FileHeader.NumberOfSections;
 
     for (i = 0; i < wSections; i++)
     {
         if (pSectionHdr->VirtualAddress <= dwRVA)
             if ((pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize) > dwRVA)
             {
                 dwRVA -= pSectionHdr->VirtualAddress;
                  dwRVA += pSectionHdr->PointerToRawData;
 
                 return (dwRVA);
             }
 
         pSectionHdr++;
     }
 
     return 0;
}
 
int main()
{
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_NT_HEADERS pImageHeader;
 
    PIMAGE_DATA_DIRECTORY pDataDirectory;
    PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor;
 
    PIMAGE_THUNK_DATA32 pFirstThunk;
    PIMAGE_THUNK_DATA32 pOriginalFirstThunk;
 
    PIMAGE_IMPORT_BY_NAME pNameImg;
    PIMAGE_SECTION_HEADER pSecHeader;  
 
    //TCHAR szFileName[MAX_PATH];
	LPCWSTR szFileName = L"C:\\Users\\trieu\\Desktop\\crack.exe";
    DWORD dwName, dwTest;
 
    LPDWORD lpwdAddress;
 
    HANDLE hMap,
           hMapView;
 
    BOOL bFound = FALSE;
    LPVOID lpMap;
 
    //printf("File: ");
	//_wscanf_l("%s", szFileName);
	
    hFile = CreateFile(szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
        Error();
 
    hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if(!hMap)
        Error();
 
    hMapView = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    if(!hMapView)
        Error();
 
    pDosHeader = (PIMAGE_DOS_HEADER)hMapView;
    if(pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        Error();
    else
        printf("\n%x (MZ) found, valid PE\nPE Header offset: 0x%x\n", pDosHeader->e_magic, pDosHeader->e_lfanew);
 
    pImageHeader = (PIMAGE_NT_HEADERS)((char*)pDosHeader + pDosHeader->e_lfanew);
    if(pImageHeader->Signature != IMAGE_NT_SIGNATURE)
        Error();
    else {
        printf("\n%x (PE00) signature found\nImageBase: 0x%x\n\n", pImageHeader->Signature, pImageHeader->OptionalHeader.ImageBase);
         
        if(pImageHeader->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI)
            printf("\"%s\" is GUI based", szFileName);
        else if(pImageHeader->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI)
            printf("\"%s\" is CLI based", szFileName);
        else
            printf("\"%s\" is something else", szFileName);
    }
 
    printf("\nAddress of Entry Point: 0x%x", pImageHeader->OptionalHeader.AddressOfEntryPoint);
    printf("\n\nLocating IAT\n");
 
    pDataDirectory = &pImageHeader->OptionalHeader.DataDirectory[1];
    pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((char*)pDosHeader + RvaToOffset(pImageHeader, pDataDirectory->VirtualAddress));
     
    pOriginalFirstThunk = (PIMAGE_THUNK_DATA32)((char*)pDosHeader + RvaToOffset(pImageHeader, pImportDescriptor->OriginalFirstThunk));
 
    pSecHeader = IMAGE_FIRST_SECTION(pImageHeader);
    printf("IAT Entrypoint: 0x%x\nDumping IAT...\n", (pDataDirectory - pSecHeader->VirtualAddress) + pSecHeader->PointerToRawData);
 
    while(pImportDescriptor->OriginalFirstThunk != 0 && !bFound)
    {
        dwName = (DWORD)((char*)lpMap + RvaToOffset(pImageHeader, pImportDescriptor->Name));
        pOriginalFirstThunk = (PIMAGE_THUNK_DATA32)((char*)pDosHeader + RvaToOffset(pImageHeader, pImportDescriptor->OriginalFirstThunk));
 
        pFirstThunk = (PIMAGE_THUNK_DATA32)((char*)pDosHeader + RvaToOffset(pImageHeader, pImportDescriptor->FirstThunk));
         
        while(pOriginalFirstThunk->u1.AddressOfData != 0 && !bFound)
        {
            pNameImg = (PIMAGE_IMPORT_BY_NAME)((char*)pDosHeader + RvaToOffset(pImageHeader, pOriginalFirstThunk->u1.AddressOfData));
            dwTest = (DWORD)pOriginalFirstThunk->u1.Function & (DWORD)IMAGE_ORDINAL_FLAG32;
 
            printf("\nAddr: 0x%x (0x%x) - Name: %s", pOriginalFirstThunk->u1.Function, pFirstThunk->u1.AddressOfData, (const char *)pNameImg->Name);
             
            if(dwTest == 0)
                if(strcmp("printf", (const char *)pNameImg->Name) == 0)
                {
                    lpwdAddress = (LPDWORD)pFirstThunk->u1.Function;
 
                    bFound = TRUE;
                }
 
            pOriginalFirstThunk++;
            pFirstThunk++;
        }
        pImportDescriptor++;
    }
     
    printf("\n...Done");
 
    CloseHandle(hFile);
    _getch();
    return 0;
}