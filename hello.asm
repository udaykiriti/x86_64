
section .data
    ; Just write the whole string at once
    msg db "Hello World!", 0xa  ; 10 = newline[we can use that tooo]
    len equ $ - msg

section .text
    global _start

_start:
    mov rdx , len  ;Length of msg
    mov rsi , msg  ;msg to write
    mov rdi , 1    ;File descrip [stdout]
    mov rax , 1    ;sys call num [sys_write]
    syscall       ;calls the kernel


    mov rax , 60    ;syscall num [sys_exit]
    mov rdi , 0
    syscall       ;call kernel
