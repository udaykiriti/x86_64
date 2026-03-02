; hello.asm — Implementation of _hello function

%include "macros.inc"

section .data
    msg db "Hello, World!", 0xa
    len equ $ - msg

section .rodata

section .bss

section .text
    global _hello

; void _hello(void) — writes "Hello, World!\n" via sys_write
_hello:
    mov     rax, SYS_WRITE  ; syscall: write
    mov     rdi, STDOUT     ; fd: stdout
    lea     rsi, [rel msg]  ; buffer
    mov     rdx, len        ; length
    syscall
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
