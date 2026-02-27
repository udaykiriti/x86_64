; main.asm — x86_64 Assembly Entry Point Template
; Arch:    x86_64 (64-bit)
; Format:  ELF64 (Linux)
; Assemble: nasm -f elf64 main.asm -o ../obj/main.o

; Initialized data (read/write) 
section .data
    msg db "Hello, World!", 0xa   ; message + newline
    len equ $ - msg

; Read-only data 
section .rodata

; Uninitialized data 
section .bss

; Code 
section .text
    global _start       ; entry point for linker

_start:
    ; sys_write(stdout, msg, len)
    mov     rax, 1          ; syscall: write
    mov     rdi, 1          ; fd: stdout
    lea     rsi, [rel msg]  ; buffer
    mov     rdx, len        ; length
    syscall

    ; exit(0) — syscall 60
    mov     rax, 60
    xor     rdi, rdi
    syscall

; Suppress executable stack warning
section .note.GNU-stack noalloc noexec nowrite progbits
