; stack_asm.asm
; asm_size(Stack* s) - count nodes by traversing the list
; rdi = Stack* s
; return eax = count

section .text
global asm_size

asm_size:
    xor eax, eax        ; count = 0
    mov rsi, [rdi]      ; rsi = s->top
    
.loop:
    cmp rsi, 0          ; while (node != NULL)
    je .done
    inc eax             ; count++
    mov rsi, [rsi + 8]  ; rsi = node->next
    jmp .loop
    
.done:
    ret

section .note.GNU-stack noalloc noexec

