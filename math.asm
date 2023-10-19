.486
.model flat, stdcall

includelib msvcrt.lib
extern exit: proc
extern printf: proc

public start

.data
sir dd 5, 7, 10, 20, 3
len equ ($ - sir) / 4
fmt db "%d", 0dh, 0ah, 0

.code


is_prime proc
	mov ebx, [esp + 4]
	cmp ebx, 0
	je false
	cmp ebx, 1
	je false
	
	mov ecx, 2
check:
	mov edx, 0
	mov eax, ebx
	div ecx
    cmp edx, 0
	je false
	
	inc ecx
	cmp ecx, ebx
	jne check

	mov eax, 1
	ret

false:
    mov eax, 0
    ret
is_prime endp


ex_a proc
	mov esi, [esp + 4] ; sir
	mov ecx, [esp + 8] ; length
check:
	mov ebx, [esi]
	pushad
	push ebx
	call is_prime
	add esp, 4
	cmp eax, 1
	popad
	je print
	jmp print_end
	
print:
    mov ebx, [esi] ; ebx was modified during is_prime
	
	pushad

	push ebx
	push offset fmt
	call printf
	add esp, 4 * 2
	
	popad
print_end:
	
	add esi, 4
	loop check
	
	ret
ex_a endp


ex_b proc
	mov esi, [esp + 4]
	mov ecx, [esp + 8]
	mov ebx, 0
check:
    mov edx, 0
	mov eax, [esi]
	mov edi, 5
	div edi
	cmp edx, 0
	je multiple
	jmp multiple_end
	
multiple:
    add ebx, [esi]
multiple_end:
	
	add esi, 4
	loop check
	
	push ebx
	push offset fmt
	call printf
	add esp, 4 * 2
	
	ret 4 * 2
ex_b endp


ex_c proc
; ecx - sir
; edx - length
    mov esi, ecx
	mov ecx, edx
	
	mov eax, [esi] ; 1st greatest number
	mov ebx, [esi + 4] ; 2nd greatest number
	cmp eax, ebx
	jl change
	jmp change_end
change:
    xchg eax, ebx
change_end:
	
	add esi, 4 * 2
	sub ecx, 2
check:
    cmp [esi], eax
	jg swap_1
	jmp swap_1_end
swap_1:
    mov ebx, eax
	mov eax, [esi]
swap_1_end:

	cmp [esi], ebx
	jl swap_2_end
	cmp [esi], eax
	je swap_2_end
	jmp swap_2

swap_2:
	mov ebx, [esi]
swap_2_end:
	
	add esi, 4
	loop check
	
	push eax
	push offset fmt
	call printf
	add esp, 4 * 2
	
	push ebx
	push offset fmt
	call printf
	add esp, 4 * 2
	
	ret 4 * 2
ex_c endp

start:

	push dword ptr len
	push offset sir
	call ex_a
	add esp, 4 * 2
	
	push dword ptr len
	push offset sir
	call ex_b
	
	mov ecx, offset sir
	mov edx, dword ptr len
	call ex_c

	push 0
	call exit
end start