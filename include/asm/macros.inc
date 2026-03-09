; File   : macros.inc
; Model  : Linux x86_64 constants
; Use    : %include "macros.inc"

; System calls
%define SYS_READ    0
%define SYS_WRITE   1
%define SYS_MMAP    9
%define SYS_MUNMAP  11
%define SYS_EXIT    60

; File descriptors
%define STDIN       0
%define STDOUT      1
%define STDERR      2

; mmap/munmap flags
%define PROT_READ       0x1
%define PROT_WRITE      0x2
%define MAP_PRIVATE     0x2
%define MAP_ANONYMOUS   0x20

; Macros: reserved for future examples
