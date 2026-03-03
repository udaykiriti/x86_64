; Assembly impl of linked list len func
; Calculates the length of a linked list
; 
; in : rdi = pointer to first Node
; out : rax = length (# nodes)

section .text
global asm_len

asm_len:
    ; rdi = head ptr
    ; rax = cnt (return val)
    xor rax, rax        ; cnt = 0
    test rdi, rdi       ; check if head is NULL
    jz .done            ; if NULL, return 0
    
.loop:
    inc rax             ; inc cnt
    mov rdi, [rdi + 8]  ; rdi = curr->next (next ptr at offset 8)
    test rdi, rdi       ; check if next is NULL
    jnz .loop           ; if not NULL, continue loop
    
.done:
    ret

section .note.GNU-stack noalloc noexec nowrite
