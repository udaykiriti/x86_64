; bitwise.asm - Bitwise operations (AND, OR, XOR) on x86_64
; Demonstrates basic bitwise logic operations

	section .text
	
	global bitwise_and
	global bitwise_or
	global bitwise_xor

	; bitwise_and(int a, int b) -> int
	; rdi = a, rsi = b
	; returns result in rax
	bitwise_and:
		mov eax, edi
		and eax, esi
		ret

	; bitwise_or(int a, int b) -> int
	; rdi = a, rsi = b
	; returns result in rax
	bitwise_or:
		mov eax, edi
		or eax, esi
		ret

	; bitwise_xor(int a, int b) -> int
	; rdi = a, rsi = b
	; returns result in rax
	bitwise_xor:
		mov eax, edi
		xor eax, esi
		ret

	section .note.GNU-stack noalloc noexec nowrite
