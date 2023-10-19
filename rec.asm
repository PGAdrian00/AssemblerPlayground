.486
.model flat, stdcall

includelib msvcrt.lib

extern exit: proc
extern printf: proc

public start

.data
mesaj db "Salut %d %d %d", 10, 0
nr1 dd 1
nr2 dd 2
nr3 dd 3

.code
start:

	mov ecx, 5
bucla:
	push ecx
	
	push nr3
	push nr2
	push nr1

	lea ebx, mesaj
	push ebx
	call printf
	
	add esp, 16
	pop ecx
	
	dec ecx
	jnz bucla
	
	push 0
	call exit
end start