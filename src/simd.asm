; simd.asm - x86_64 SIMD and timing primitives

section .text
    global asm_vec4_add_i32
    global asm_rdtsc_serialized

; void asm_vec4_add_i32(const int *a, const int *b, int *out)
; SysV ABI: rdi=a, rsi=b, rdx=out
; Computes out[i] = a[i] + b[i] for i in [0,3]
asm_vec4_add_i32:
    movdqu xmm0, [rdi]
    movdqu xmm1, [rsi]
    paddd xmm0, xmm1
    movdqu [rdx], xmm0
    ret

; uint64_t asm_rdtsc_serialized(void)
; Uses LFENCE before/after RDTSC to reduce reordering noise.
asm_rdtsc_serialized:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    lfence
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
