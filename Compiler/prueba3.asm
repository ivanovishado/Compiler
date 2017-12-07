.386
.model flat, stdcall
option casemap:none 

include \masm32\macros\macros.asm
include \masm32\include\masm32.inc
include \masm32\include\kernel32.inc

includelib \masm32\lib\masm32.lib
includelib \masm32\lib\kernel32.lib

.data




.code
inicio:
	call main
	push 0
	call ExitProcess

fact proc a : DWORD
local x: DWORD 
local y: DWORD 
push y

	mov ecx,1
	mov edx,a
	cmp edx,ecx
	jne E0
	mov eax,1
	ret
	jmp E1
E0:
	push 1
push a

	pop eax
	pop ebx
	sub eax,ebx
	push eax

	pop eax
	mov x,eax
	push a
	push x
	call fact
push eax

	pop eax
	pop ebx
	imul ebx
	push eax

	pop eax
	mov y,eax
	mov eax,y
	ret
E1:
ret
fact endp

main proc
local a: DWORD 
	push 3
	call fact
push eax

	mov a,eax
	print str$(a)

	mov eax,a
	ret
ret
main endp

end inicio