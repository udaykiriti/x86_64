/*
 * x86_cpuinfo.h - CPU information and feature detection for x86/x86_64
 * Provides CPUID interface for detecting processor capabilities,
 * cache info, and architectural features.
 */

#ifndef X86_CPUINFO_H
#define X86_CPUINFO_H

#include <stdint.h>
#include <stdbool.h>

/* CPUID leaf values */
#define CPUID_LEAF_BASIC            0x00
#define CPUID_LEAF_BASIC_EXTENDED   0x80000000

/* Features - ECX register (leaf 0x01) */
#define CPUID_ECX_SSE3              (1 << 0)
#define CPUID_ECX_PCLMUL            (1 << 1)
#define CPUID_ECX_DTES64            (1 << 2)
#define CPUID_ECX_MONITOR           (1 << 3)
#define CPUID_ECX_DSC_CPL           (1 << 4)
#define CPUID_ECX_VMX               (1 << 5)
#define CPUID_ECX_SMX               (1 << 6)
#define CPUID_ECX_EST               (1 << 7)
#define CPUID_ECX_TM2               (1 << 8)
#define CPUID_ECX_SSSE3             (1 << 9)
#define CPUID_ECX_CID               (1 << 10)
#define CPUID_ECX_CX16              (1 << 13)
#define CPUID_ECX_XTPR              (1 << 14)
#define CPUID_ECX_PDCM              (1 << 15)
#define CPUID_ECX_PCID              (1 << 17)
#define CPUID_ECX_DCA               (1 << 18)
#define CPUID_ECX_SSE4_1            (1 << 19)
#define CPUID_ECX_SSE4_2            (1 << 20)
#define CPUID_ECX_AES               (1 << 25)
#define CPUID_ECX_AVX               (1 << 28)

/* Features - EDX register (leaf 0x01) */
#define CPUID_EDX_FPU               (1 << 0)
#define CPUID_EDX_VME               (1 << 1)
#define CPUID_EDX_DE                (1 << 2)
#define CPUID_EDX_PSE               (1 << 3)
#define CPUID_EDX_TSC               (1 << 4)
#define CPUID_EDX_MSR               (1 << 5)
#define CPUID_EDX_PAE               (1 << 6)
#define CPUID_EDX_MCE               (1 << 7)
#define CPUID_EDX_CX8               (1 << 8)
#define CPUID_EDX_APIC              (1 << 9)
#define CPUID_EDX_SEP               (1 << 11)
#define CPUID_EDX_MTRR              (1 << 12)
#define CPUID_EDX_PGE               (1 << 13)
#define CPUID_EDX_MCA               (1 << 14)
#define CPUID_EDX_CMOV              (1 << 15)
#define CPUID_EDX_PAT               (1 << 16)
#define CPUID_EDX_PSE36             (1 << 17)
#define CPUID_EDX_PSN               (1 << 18)
#define CPUID_EDX_CLFSH             (1 << 19)
#define CPUID_EDX_MMX               (1 << 23)
#define CPUID_EDX_FXSR              (1 << 24)
#define CPUID_EDX_SSE               (1 << 25)
#define CPUID_EDX_SSE2              (1 << 26)
#define CPUID_EDX_SS                (1 << 27)
#define CPUID_EDX_HTT               (1 << 28)
#define CPUID_EDX_TM                (1 << 29)
#define CPUID_EDX_IA64              (1 << 30)
#define CPUID_EDX_PBE               (1 << 31)

typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} cpuid_regs_t;

typedef struct {
    char vendor[13];              /* CPU vendor string */
    char brand[49];               /* CPU brand string */
    uint32_t signature;           /* Family/Model/Stepping */
    uint32_t max_cpuid;           /* Max CPUID leaf */
    uint32_t logical_cpus;        /* Logical CPU count */
    uint32_t physical_cpus;       /* Physical CPU count */
    uint32_t cache_line_size;     /* Cache line size in bytes */
    bool hypervisor;              /* Running under hypervisor */
    bool has_sse;
    bool has_sse2;
    bool has_sse3;
    bool has_ssse3;
    bool has_sse41;
    bool has_sse42;
    bool has_avx;
    bool has_aes;
    bool has_vmx;
} x86_cpuinfo_t;

/* Function prototypes */
void x86_cpuid(uint32_t leaf, uint32_t subleaf, cpuid_regs_t *regs);
void x86_get_cpuinfo(x86_cpuinfo_t *info);
void x86_print_cpuinfo(const x86_cpuinfo_t *info);
bool x86_has_feature(const x86_cpuinfo_t *info, uint32_t feature);

#endif /* X86_CPUINFO_H */
