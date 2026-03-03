; hello.asm - Print "Hello, World!" using x86_64 syscall

%include "macros.inc"

section .data
    msg db "Hello, World!", 0xa
    len equ $ - msg

section .text
    global _hello

; void _hello(void)
; Uses Linux syscall sys_write to print message to stdout
_hello:
    mov rax, SYS_WRITE                  ; syscall number: write
    mov rdi, STDOUT                     ; file descriptor: stdout
    lea rsi, [rel msg]                  ; buffer address
    mov rdx, len                        ; number of bytes
    syscall
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
