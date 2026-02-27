
section .data
    ; Just write the whole string at once
    msg db "Hello World!", 0xa  ; 10 = newline[we can use that tooo]
    len equ $ - msg

section .text
    global _start

_start:
    mov edx , len  ;Length of msg
    mov ecx , msg  ;msg to write
    mov ebx , 1    ;File descrip [stdout]
    mov eax , 4    ;sys call num [sys_write]
    int 0x80       ;calls the kernel


    mov eax , 1    ;syscall num [sys_exit]
    mov rdi , 0
    int 0x80       ;call kernel
