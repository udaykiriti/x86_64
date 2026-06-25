/*
 * x86_registers.c - x86/x86_64 register manipulation implementation
 * Provides inline assembly wrappers for reading/writing CPU registers
 */

#include "x86_registers.h"
#include <stdio.h>

/*
 * x86_read_cr0 - Read CR0 (Control Register 0)
 * Returns: CR0 value
 */
uint64_t x86_read_cr0(void) {
    uint64_t val;
    asm volatile("mov %%cr0, %0" : "=r" (val));
    return val;
}

/*
 * x86_read_cr2 - Read CR2 (Page Fault Linear Address)
 * Returns: CR2 value
 */
uint64_t x86_read_cr2(void) {
    uint64_t val;
    asm volatile("mov %%cr2, %0" : "=r" (val));
    return val;
}

/*
 * x86_read_cr3 - Read CR3 (Page Directory Base)
 * Returns: CR3 value
 */
uint64_t x86_read_cr3(void) {
    uint64_t val;
    asm volatile("mov %%cr3, %0" : "=r" (val));
    return val;
}

/*
 * x86_read_cr4 - Read CR4 (Control Register 4)
 * Returns: CR4 value
 */
uint64_t x86_read_cr4(void) {
    uint64_t val;
    asm volatile("mov %%cr4, %0" : "=r" (val));
    return val;
}

/*
 * x86_read_rflags - Read RFLAGS (Flags Register)
 * Returns: RFLAGS value
 */
uint64_t x86_read_rflags(void) {
    uint64_t val;
    asm volatile("pushfq; pop %0" : "=r" (val));
    return val;
}

/*
 * x86_read_msr - Read Model-Specific Register
 * @msr: MSR index
 * Returns: MSR value
 */
uint64_t x86_read_msr(uint32_t msr) {
    uint32_t lo, hi;
    asm volatile("rdmsr" : "=a" (lo), "=d" (hi) : "c" (msr));
    return ((uint64_t)hi << 32) | lo;
}

/*
 * x86_write_cr0 - Write CR0 (Control Register 0)
 * @val: Value to write
 */
void x86_write_cr0(uint64_t val) {
    asm volatile("mov %0, %%cr0" : : "r" (val));
}

/*
 * x86_write_cr3 - Write CR3 (Page Directory Base)
 * @val: Value to write
 */
void x86_write_cr3(uint64_t val) {
    asm volatile("mov %0, %%cr3" : : "r" (val));
}

/*
 * x86_write_cr4 - Write CR4 (Control Register 4)
 * @val: Value to write
 */
void x86_write_cr4(uint64_t val) {
    asm volatile("mov %0, %%cr4" : : "r" (val));
}

/*
 * x86_write_msr - Write Model-Specific Register
 * @msr: MSR index
 * @val: Value to write
 */
void x86_write_msr(uint32_t msr, uint64_t val) {
    uint32_t lo = val & 0xFFFFFFFF;
    uint32_t hi = (val >> 32) & 0xFFFFFFFF;
    asm volatile("wrmsr" : : "a" (lo), "d" (hi), "c" (msr));
}

/*
 * x86_get_control_regs - Read all control registers
 * @regs: Structure to fill with register values
 */
void x86_get_control_regs(x86_control_regs_t *regs) {
    regs->cr0 = x86_read_cr0();
    regs->cr2 = x86_read_cr2();
    regs->cr3 = x86_read_cr3();
    regs->cr4 = x86_read_cr4();
    regs->rflags = x86_read_rflags();
}

/*
 * x86_print_control_regs - Print control register values
 * @regs: Control registers structure
 */
void x86_print_control_regs(const x86_control_regs_t *regs) {
    printf("\n=== x86_64 Control Registers ===\n");
    printf("CR0:     0x%016lx\n", regs->cr0);
    printf("  PE    (Protected Mode Enable): %s\n", (regs->cr0 & CR0_PE) ? "Yes" : "No");
    printf("  PG    (Paging Enable):         %s\n", (regs->cr0 & CR0_PG) ? "Yes" : "No");
    printf("  WP    (Write Protect):         %s\n", (regs->cr0 & CR0_WP) ? "Yes" : "No");
    printf("  NE    (Numeric Error):         %s\n", (regs->cr0 & CR0_NE) ? "Yes" : "No");
    printf("\nCR2:     0x%016lx (Page Fault Address)\n", regs->cr2);
    printf("\nCR3:     0x%016lx (Page Directory Base)\n", regs->cr3);
    printf("\nCR4:     0x%016lx\n", regs->cr4);
    printf("  PAE   (Physical Address Ext): %s\n", (regs->cr4 & CR4_PAE) ? "Yes" : "No");
    printf("  PSE   (Page Size Ext):        %s\n", (regs->cr4 & CR4_PSE) ? "Yes" : "No");
    printf("  PGE   (Page Global Enable):   %s\n", (regs->cr4 & CR4_PGE) ? "Yes" : "No");
    printf("  PCIDE (PCID Enable):          %s\n", (regs->cr4 & CR4_PCIDE) ? "Yes" : "No");
    printf("  SMEP  (SMEP Enable):          %s\n", (regs->cr4 & CR4_SMEP) ? "Yes" : "No");
    printf("  SMAP  (SMAP Enable):          %s\n", (regs->cr4 & CR4_SMAP) ? "Yes" : "No");
    printf("\nRFLAGS:  0x%016lx\n", regs->rflags);
    printf("  CF    (Carry):                %s\n", (regs->rflags & RFLAGS_CF) ? "Yes" : "No");
    printf("  ZF    (Zero):                 %s\n", (regs->rflags & RFLAGS_ZF) ? "Yes" : "No");
    printf("  SF    (Sign):                 %s\n", (regs->rflags & RFLAGS_SF) ? "Yes" : "No");
    printf("  OF    (Overflow):             %s\n", (regs->rflags & RFLAGS_OF) ? "Yes" : "No");
    printf("  IF    (Interrupt):            %s\n", (regs->rflags & RFLAGS_IF) ? "Yes" : "No");
    printf("\n");
}
