; utils.asm — Utility / helper routines template

section .data

section .rodata

section .bss

section .text

; TODO: define helper functions here
;
; Example skeleton:
;
; my_func:
;     ; preserve callee-saved registers (rbx, rbp, r12–r15)
;     push    rbp
;     mov     rbp, rsp
;
;     ; --- body ---
;
;     pop     rbp
;     ret

section .note.GNU-stack noalloc noexec nowrite progbits
