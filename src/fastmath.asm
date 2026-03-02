; fastmath.asm - x86_64 Assembly math routines
; All functions follow System V AMD64 ABI calling convention:
; - First parameter in edi (for 32-bit integers)
; - Return value in eax

global asm_add
global asm_sub
global asm_factorial

section .text

; Add two integers: asm_add(int a, int b)
; Parameters: edi = a, esi = b
; Returns: eax = a + b
asm_add:
    mov eax, edi
    add eax, esi
    ret

; Subtract two integers: asm_sub(int a, int b)
; Parameters: edi = a, esi = b
; Returns: eax = a - b
asm_sub:
    mov eax, edi
    sub eax, esi
    ret

; Factorial (recursive): asm_factorial(int n)
; Parameter: edi = n
; Returns: eax = n!
asm_factorial:
    cmp edi, 1          ; if (n <= 1)
    jle .return_one     ; return 1

    ; n > 1: recursive case
    push rbp
    mov rbp, rsp
    push rbx            ; save n for later use
    
    mov rbx, rdi        ; rbx = n
    dec rdi              ; rdi = n - 1
    call asm_factorial  ; eax = factorial(n-1)
    imul eax, ebx       ; eax = eax * n
    
    pop rbx
    pop rbp
    ret

.return_one:
    mov eax, 1
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
