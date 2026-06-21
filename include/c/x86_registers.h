/*
 * x86_registers.h - x86/x86_64 register manipulation utilities
 * Provides macros and functions for reading and writing CPU registers,
 * control registers, model-specific registers (MSRs), etc.
 */

#ifndef X86_REGISTERS_H
#define X86_REGISTERS_H

#include <stdint.h>

/* RFLAGS register bits */
#define RFLAGS_CF   0x0001          /* Carry Flag */
#define RFLAGS_PF   0x0004          /* Parity Flag */
#define RFLAGS_AF   0x0010          /* Auxiliary Carry Flag */
#define RFLAGS_ZF   0x0040          /* Zero Flag */
#define RFLAGS_SF   0x0080          /* Sign Flag */
#define RFLAGS_TF   0x0100          /* Trap Flag */
#define RFLAGS_IF   0x0200          /* Interrupt Flag */
#define RFLAGS_DF   0x0400          /* Direction Flag */
#define RFLAGS_OF   0x0800          /* Overflow Flag */
#define RFLAGS_IOPL 0x3000          /* I/O Privilege Level */
#define RFLAGS_NT   0x4000          /* Nested Task */
#define RFLAGS_RF   0x10000         /* Resume Flag */
#define RFLAGS_VM   0x20000         /* Virtual 8086 Mode */
#define RFLAGS_AC   0x40000         /* Alignment Check */
#define RFLAGS_VIF  0x80000         /* Virtual Interrupt Flag */
#define RFLAGS_VIP  0x100000        /* Virtual Interrupt Pending */
#define RFLAGS_ID   0x200000        /* ID Flag */

/* CR0 control register bits */
#define CR0_PE      0x00000001      /* Protected Mode Enable */
#define CR0_MP      0x00000002      /* Monitor Coprocessor */
#define CR0_EM      0x00000004      /* Emulation */
#define CR0_TS      0x00000008      /* Task Switched */
#define CR0_ET      0x00000010      /* Extension Type */
#define CR0_NE      0x00000020      /* Numeric Error */
#define CR0_WP      0x00010000      /* Write Protect */
#define CR0_AM      0x00040000      /* Alignment Mask */
#define CR0_NW      0x20000000      /* Not Writethrough */
#define CR0_CD      0x40000000      /* Cache Disable */
#define CR0_PG      0x80000000      /* Paging Enable */

/* CR4 control register bits */
#define CR4_VME     0x00000001      /* Virtual 8086 Mode Extensions */
#define CR4_PVI     0x00000002      /* Protected-mode Virtual Interrupts */
#define CR4_TSD     0x00000004      /* Time Stamp Disable */
#define CR4_DE      0x00000008      /* Debugging Extensions */
#define CR4_PSE     0x00000010      /* Page Size Extensions */
#define CR4_PAE     0x00000020      /* Physical Address Extension */
#define CR4_MCE     0x00000040      /* Machine Check Enable */
#define CR4_PGE     0x00000080      /* Page Global Enable */
#define CR4_PCE     0x00000100      /* Performance Monitoring Counter Enable */
#define CR4_OSFXSR  0x00000200      /* OS FXSave/FXRestore Support */
#define CR4_OSXMMEXCPT 0x00000400   /* OS Unmasked Exception Support */
#define CR4_VMXE    0x00002000      /* VMX Enable */
#define CR4_SMXE    0x00004000      /* SMX Enable */
#define CR4_PCIDE   0x00020000      /* PCID Enable */
#define CR4_OSXSAVE 0x00040000      /* XSAVE/XRESTORE Support */
#define CR4_SMEP    0x00100000      /* Supervisor Mode Execution Prevention */
#define CR4_SMAP    0x00200000      /* Supervisor Mode Access Prevention */

/* Model-Specific Registers (MSRs) */
#define MSR_IA32_TSC            0x10        /* Time Stamp Counter */
#define MSR_IA32_APICBASE       0x1B        /* APIC Base */
#define MSR_IA32_FEATURE_CTL    0x3A        /* Feature Control */
#define MSR_IA32_BIOS_SIGN_ID   0x8B        /* BIOS Update Signature */
#define MSR_IA32_MTRRCAP        0xFE        /* MTRR Capabilities */
#define MSR_IA32_PERF_STATUS    0x198       /* Performance Status */
#define MSR_IA32_PERF_CTL       0x199       /* Performance Control */
#define MSR_IA32_CLOCK_MODULATION 0x19A     /* Clock Modulation */
#define MSR_IA32_THERM_STATUS   0x19C       /* Thermal Status */
#define MSR_IA32_MISC_ENABLE    0x1A0       /* Misc Enables */
#define MSR_IA32_ENERGY_PERF_BIAS 0x1B0     /* Energy Performance Bias */
#define MSR_IA32_POWER_CTL      0x1FC       /* Power Control */

typedef struct {
    uint64_t cr0;
    uint64_t cr2;
    uint64_t cr3;
    uint64_t cr4;
    uint64_t rflags;
} x86_control_regs_t;

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
    uint64_t rflags;
} x86_general_regs_t;

/* Function prototypes */
uint64_t x86_read_cr0(void);
uint64_t x86_read_cr2(void);
uint64_t x86_read_cr3(void);
uint64_t x86_read_cr4(void);
uint64_t x86_read_rflags(void);
uint64_t x86_read_msr(uint32_t msr);

void x86_write_cr0(uint64_t val);
void x86_write_cr3(uint64_t val);
void x86_write_cr4(uint64_t val);
void x86_write_msr(uint32_t msr, uint64_t val);

void x86_get_control_regs(x86_control_regs_t *regs);
void x86_print_control_regs(const x86_control_regs_t *regs);

#endif /* X86_REGISTERS_H */
