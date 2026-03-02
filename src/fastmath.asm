; fastmath.asm - Optimized math routines in assembly

global asm_add
global asm_sub
global asm_factorial

section .text

; int asm_add(int a, int b)
; a in edi, b in esi
; returns a + b in eax
asm_add:
    mov eax, edi
    add eax, esi
    ret

; int asm_sub(int a, int b)
; a in edi, b in esi
; returns a - b in eax
asm_sub:
    mov eax, edi
    sub eax, esi
    ret

; int asm_factorial(int n)
; n in edi
; returns n! in eax
asm_factorial:
    cmp edi, 1
    jle .base_case
    push rbp
    mov rbp, rsp
    push rbx
    mov rbx, rdi
    dec rdi
    call asm_factorial
    imul eax, ebx
    pop rbx
    pop rbp
    ret
.base_case:
    mov eax, 1
    ret
