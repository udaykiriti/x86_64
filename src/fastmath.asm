; fastmath.asm - x86_64 Assembly math routines
; All functions follow System V AMD64 ABI calling convention:
; - First parameter in edi (for 32-bit integers)
; - Return value in eax

global asm_add
global asm_sub
global asm_factorial

section .text

; Add two integers: asm_add(int a, int b)
; Parameters: edi = a, esi = b
; Returns: eax = a + b
asm_add:
    mov eax, edi
    add eax, esi
    ret

; Subtract two integers: asm_sub(int a, int b)
; Parameters: edi = a, esi = b
; Returns: eax = a - b
asm_sub:
    mov eax, edi
    sub eax, esi
    ret

; Factorial (iterative): asm_factorial(int n)
; Parameter: edi = n
; Returns: eax = n!
asm_factorial:
    mov eax, 1
    cmp edi, 1          ; if (n <= 1), return 1
    jle .done

.loop:
    imul eax, edi       ; eax *= n
    dec edi             ; n--
    cmp edi, 1
    jg .loop

.done:
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
