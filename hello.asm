section .data
    msg db "Hello World!", 0xa
    len equ $ - msg

section .text
    global _hello

_hello:
    ; sys_write system call
    mov rax, 1      ; syscall number for sys_write
    mov rdi, 1      ; file descriptor 1 is stdout
    mov rsi, msg    ; address of the message to write
    mov rdx, len    ; length of the message
    syscall         ; invoke the kernel

    ret             ; return to the C caller

section .note.GNU-stack noalloc noexec nowrite progbits
