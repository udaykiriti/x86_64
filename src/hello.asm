; utils.asm — Utility / helper routines template

section .data
    msg db "Hello, World!", 0xa
    len equ $ - msg

section .rodata

section .bss

section .text
    global _hello

; void _hello(void) — writes "Hello, World!\n" via sys_write
_hello:
    mov     rax, 1          ; syscall: write
    mov     rdi, 1          ; fd: stdout
    lea     rsi, [rel msg]  ; buffer
    mov     rdx, len        ; length
    syscall
    ret

; TODO: define helper functions here
;
; Example skeleton:
;
; my_func:
;     ; preserve callee-saved registers (rbx, rbp, r12–r15)
;     push    rbp
;     mov     rbp, rsp
;
;     ; --- body ---
;
;     pop     rbp
;     ret

section .note.GNU-stack noalloc noexec nowrite progbits
