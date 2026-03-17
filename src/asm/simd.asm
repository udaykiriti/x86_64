; Program: simd
; Model  : Linux x86_64, SysV AMD64 ABI, SSE2 integer ops

section .text
    global vecadd4

; void vecadd4(const int *a, const int *b, int *c)
; a -> rdi
; b -> rsi
; c -> rdx
vecadd4:
    movdqu xmm0, [rdi]
    movdqu xmm1, [rsi]
    paddd xmm0, xmm1
    movdqu [rdx], xmm0
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
