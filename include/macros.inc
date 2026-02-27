; macros.inc — Reusable NASM macros / constants template
; Usage: %include "macros.inc"  at the top of any .asm file

; System-call numbers (Linux x86_64) 
%define SYS_READ    0
%define SYS_WRITE   1
%define SYS_EXIT    60

; File descriptors 
%define STDIN       0
%define STDOUT      1
%define STDERR      2

; Macros 
; TODO: add your macros here
;
; Example:
; %macro print 2          ; print  label, len
;     mov  rax, SYS_WRITE
;     mov  rdi, STDOUT
;     lea  rsi, [rel %1]
;     mov  rdx, %2
;     syscall
; %endmacro
