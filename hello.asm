; SUPER SIMPLE HELLO WORLD

section .data
    ; Just write the whole string at once
    msg db "Hello World!", 10  ; 10 = newline
    len equ $ - msg

section .text
    global _start

_start:
    ; Ask OS to print our message
    mov rax, 1      ; System call for write
    mov rdi, 1      ; File handle 1 = screen
    mov rsi, msg    ; Address of message
    mov rdx, len    ; Length of message
    syscall         ; Call the OS
    
    ; Ask OS to exit
    mov rax, 60     ; System call for exit
    xor rdi, rdi    ; Exit code 0 = success
    syscall         ; Call the OS
