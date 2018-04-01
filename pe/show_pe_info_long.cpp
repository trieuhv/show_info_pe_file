#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h> 
#include<errno.h> 
int main()
{
	DWORD FileSize, PeHeaderAddress, Signature, exports, Characteristics_export;
	errno_t err;
	FILE*fp = NULL;
	fp = fopen("C:\\Windows\\SysWOW64\\kernel32.dll", "rb");

	IMAGE_DOS_HEADER DosHeader = { 0 };
	PIMAGE_DOS_HEADER dosHeader = { 0 };
	PIMAGE_NT_HEADERS PeHeader = { 0 };
	IMAGE_FILE_HEADER FileHeader = { 0 };
	IMAGE_OPTIONAL_HEADER OpHeader = { 0 };
	IMAGE_SECTION_HEADER SectionHeader = { 0 };
	IMAGE_EXPORT_DIRECTORY Export = { 0 };
	IMAGE_IMPORT_DESCRIPTOR Import = { 0 };
	IMAGE_DATA_DIRECTORY Data = { 0 };
	IMAGE_THUNK_DATA32 thunk = { 0 };
	fseek(fp, 0, SEEK_END); 									//dua con tro ve cuoi file
	FileSize = ftell(fp); 										//lay vi tri cua byte cuoi cung cua file
	if (FileSize < sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS)) {
		printf("This is not file PE!\n");
	}
	else {
		fseek(fp, 0, SEEK_SET);
		fread(&DosHeader, sizeof(DosHeader), 1, fp);
		if (DosHeader.e_magic != 0x5a4d) {
			printf("This is not file PE\n");
		}
		else {
			PeHeaderAddress = DosHeader.e_lfanew;
			if (FileSize < PeHeaderAddress + sizeof(IMAGE_NT_HEADERS)) {
				printf("This is not file PE");
			}
			else {
				printf("\t\t--------PE Header--------\n");
				fseek(fp, PeHeaderAddress, SEEK_SET);                  //Dua ve dau PeHeader
				fread(&Signature, sizeof(Signature), 1, fp);
				fread(&FileHeader, 20, 1, fp);         //Doc Gia Tri FileHeader
				fread(&OpHeader, sizeof(IMAGE_OPTIONAL_HEADER), 1, fp);
				fseek(fp, PeHeaderAddress + 24 + 28, SEEK_SET);
				int imagebase;
				fread(&imagebase, 4, 1, fp);
				printf("Optional Header: \n");
				printf("\tAddress of Entry Point: %#x\n", OpHeader.AddressOfEntryPoint);
				printf("\tCheckSum: %#x\n", OpHeader.CheckSum);
				printf("\tImage Base: %#x\n", imagebase);
				printf("\tFileAlignment: : %#x\n", OpHeader.FileAlignment);
				printf("\tSizeOfImage: %#x\n", OpHeader.SizeOfImage);
				printf("------------------------------------\n");
				int laybien_import = 100;
				int laybien_export = 100;
				for (int i = 0; i < FileHeader.NumberOfSections; i++) {
					int l;
					char name_sec[100];
					fseek(fp, PeHeaderAddress + sizeof(DWORD) + sizeof(FileHeader) + FileHeader.SizeOfOptionalHeader + sizeof(SectionHeader)*i, SEEK_SET);
					for (int j = 0; j < 8; j++) {
						fread(&l, 1, 1, fp);
						if ((char)l == ' ') {
							break;
						}
						else {
							name_sec[j] = (char)l;
						}
					}
					if (strcmp(name_sec, ".idata") == 0 || strcmp(name_sec, ".rdata") == 0) {
						laybien_import = i;
					}
					if (strcmp(name_sec, ".rdata") == 0) {
						laybien_export = i;
					}
				}
				int now = PeHeaderAddress + FileHeader.SizeOfOptionalHeader;
				int tmp = 0;
				int point = 0;
				int virtual_add = 0;
				int off_import = 0;
				int point_import = 0;
				int virtual_import = 0;
				int rva_import = OpHeader.DataDirectory[-1].VirtualAddress;
				fseek(fp, PeHeaderAddress + sizeof(DWORD) + sizeof(FileHeader) + FileHeader.SizeOfOptionalHeader, SEEK_SET);
				for (int i = 0; i < FileHeader.NumberOfSections; i++) {
					fread(&SectionHeader, sizeof(IMAGE_SECTION_HEADER), 1, fp);
					printf("Section Header %d : \n", i + 1);
					printf("\tSection name: %s\n", SectionHeader.Name);
					printf("\tSection Charecteristics: %#x\n", SectionHeader.Characteristics);
					printf("\tSection RawAddress: %#x\n", SectionHeader.PointerToRawData);
					printf("\tSection RawSize: %#x\n", SectionHeader.SizeOfRawData);
					printf("\tSection VirtualAddress %#x\n", SectionHeader.VirtualAddress);
					printf("\tSection Virtual Size %#x\n", SectionHeader.Misc.VirtualSize);
					if (i == laybien_export) {
						tmp = SectionHeader.PointerToRawData - SectionHeader.VirtualAddress + OpHeader.DataDirectory[-2].VirtualAddress;   //offset export director
						point = SectionHeader.PointerToRawData;
						virtual_add = SectionHeader.VirtualAddress;
					}
					if (i == laybien_import) {
						off_import = SectionHeader.PointerToRawData - SectionHeader.VirtualAddress + rva_import;
						point_import = SectionHeader.PointerToRawData;
						virtual_import = SectionHeader.VirtualAddress;
					}
				}
				fseek(fp, tmp, SEEK_SET);															//Tro den export director
				fread(&Export, sizeof(Export), 1, fp);										//Lay gia tri IMAGE_EXPORT_DIRECTOR
				printf("-------------CAC HAM EXPORT------------------\n ");
				int addfun = Export.AddressOfFunctions;
				int addname = Export.AddressOfNames;
				int addori = Export.AddressOfNameOrdinals;
				int diachiten = Export.Name;
				fseek(fp, point - virtual_add + diachiten, SEEK_SET);									//Tro den offset Addressofname

				int e;
				int ten_export = 1;
				int exit_export = 0;
				int checkend = 1;
				while (exit_export == 0) {
					if (ten_export == 1) {
						while (1) {
							fread(&e, 1, 1, fp);
							if ((char)e == NULL) {
								ten_export = 0;
								break;
							}
						}
					}
					else {
						while (1) {
							fread(&e, 1, 1, fp);
							if ((char)e == NULL) {
								if (checkend == 1) {
									exit_export = 1;
									break;
								}
								else {
									printf("\n");
									checkend = 1;
								}
							}
							else {
								printf("%c", (char)e);
								checkend = 0;
							}
						}
					}
				}

				printf("\n\n");
				printf("----------------------CAC HAM IMPORT-----------------------\n");
				int z;
				int stt_dll_import = 0;
				int now_import = off_import;
				while (1) {
					fseek(fp, off_import + sizeof(Import)*stt_dll_import, SEEK_SET);
					stt_dll_import++;
					fread(&Import, sizeof(Import), 1, fp);
					if (Import.Name == 0) {
						break;
					}
					fseek(fp, point_import - virtual_import + Import.Name, SEEK_SET);
					int name_import;
					while (1) {
						fread(&name_import, 1, 1, fp);
						if (name_import == 0) {
							break;
						}
						else {
							printf("%c", (char)name_import);
						}
					}
					printf("\n");
					int rva_thunk = Import.FirstThunk;
					int stt_thunk = 0;
					while (1) {
						fseek(fp, point_import - virtual_import + Import.FirstThunk + sizeof(thunk)*stt_thunk, SEEK_SET);
						fread(&thunk, sizeof(thunk), 1, fp);
						if (thunk.u1.AddressOfData == 0) {
							int tmp_1;
							break;
						}
						stt_thunk++;
						fseek(fp, point_import - virtual_import + thunk.u1.AddressOfData, SEEK_SET);
						fread(&z, sizeof(WORD), 1, fp);
						printf("\t");
						while (1) {
							fread(&z, 1, 1, fp);
							if ((char)z == ' ' || (char)z == NULL) {
								break;
							}
							else {
								printf("%c", (char)z);
							}
						}
						printf("\n");
					}
				}
			}
		}
	}
	fclose(fp);
	_getch();
}
