; io.asm - x86_64 syscall-based input/output helpers

%include "macros.inc"

section .bss
    buf resb 256

section .text
    global asm_echo

; int asm_echo(void)
; Reads up to 255 bytes from stdin, echoes bytes to stdout, returns bytes read.
; Returns <= 0 on EOF or read error.
asm_echo:
    mov rax, SYS_READ
    mov rdi, STDIN
    lea rsi, [rel buf]
    mov rdx, 255
    syscall

    cmp rax, 0
    jle .done

    mov r8, rax

    mov rdx, rax
    mov rax, SYS_WRITE
    mov rdi, STDOUT
    lea rsi, [rel buf]
    syscall

    mov rax, r8

.done:
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
