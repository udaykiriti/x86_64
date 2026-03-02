; hello.asm - Print "Hello, World!" using x86_64 syscall

%include "macros.inc"

section .data
    hello_message db "Hello, World!", 0xa
    message_length equ $ - hello_message

section .text
    global _hello

; void _hello(void)
; Uses Linux syscall sys_write to print message to stdout
_hello:
    mov rax, SYS_WRITE                  ; syscall number: write
    mov rdi, STDOUT                     ; file descriptor: stdout
    lea rsi, [rel hello_message]        ; buffer address
    mov rdx, message_length             ; number of bytes
    syscall
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
