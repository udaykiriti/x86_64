; Program: hello
; Model  : Linux x86_64, direct syscall write
; Goal   : print one line to stdout

%include "macros.inc"

section .data
    m db "Hello, World!", 0xa
    l equ $ - m

section .text
    global _hello

; Procedure: _hello
; Action   : write(STDOUT, m, l)
_hello:
    mov rax, SYS_WRITE
    mov rdi, STDOUT
    lea rsi, [rel m]
    mov rdx, l
    syscall
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
