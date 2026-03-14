; Program: math
; Model  : Linux x86_64, SysV AMD64 ABI

section .text
    global _add

; long _add(long a, long b)
; a -> rdi, b -> rsi, return -> rax
_add:
    mov rax, rdi
    add rax, rsi
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
