/* x86_intrinsics.asm - x86_64 Low-level intrinsic operations */

.intel_syntax noprefix
.section .text
.globl x86_disable_interrupts
.globl x86_enable_interrupts
.globl x86_halt
.globl x86_nop
.globl x86_pause
.globl x86_clflush
.globl x86_prefetch0
.globl x86_atomic_inc
.globl x86_atomic_dec
.globl x86_atomic_add

/*
 * x86_disable_interrupts: Clear interrupt flag
 */
x86_disable_interrupts:
    cli
    ret

/*
 * x86_enable_interrupts: Set interrupt flag
 */
x86_enable_interrupts:
    sti
    ret

/*
 * x86_halt: Halt CPU (HLT instruction)
 */
x86_halt:
    hlt
    ret

/*
 * x86_nop: One-byte NOP (for timing)
 */
x86_nop:
    nop
    ret

/*
 * x86_pause: PAUSE instruction (for spin loops)
 */
x86_pause:
    pause
    ret

/*
 * x86_clflush: Flush cache line
 * rdi = address to flush
 */
x86_clflush:
    clflush [rdi]
    ret

/*
 * x86_prefetch0: Prefetch cache line (T0 - L1 cache)
 * rdi = address to prefetch
 */
x86_prefetch0:
    prefetcht0 [rdi]
    ret

/*
 * x86_atomic_inc: Atomic increment
 * rdi = pointer to 64-bit value
 * Returns: new value in RAX
 */
x86_atomic_inc:
    mov rax, 1
    lock xadd [rdi], rax
    add rax, 1              /* rax now contains new value */
    ret

/*
 * x86_atomic_dec: Atomic decrement
 * rdi = pointer to 64-bit value
 * Returns: new value in RAX
 */
x86_atomic_dec:
    mov rax, -1
    lock xadd [rdi], rax
    add rax, -1             /* rax now contains new value */
    ret

/*
 * x86_atomic_add: Atomic add
 * rdi = pointer to 64-bit value
 * rsi = value to add
 * Returns: new value in RAX
 */
x86_atomic_add:
    mov rax, rsi
    lock xadd [rdi], rax
    add rax, rsi            /* rax now contains new value */
    ret
