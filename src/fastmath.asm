; fastmath.asm - Optimized math routines in assembly

global asm_add
global asm_sub

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
