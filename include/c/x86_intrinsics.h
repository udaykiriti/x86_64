/*
 * x86_intrinsics.h - Low-level x86_64 intrinsic operations
 * Provides access to CPU instructions like CLI, STI, HLT, etc.
 */

#ifndef X86_INTRINSICS_H
#define X86_INTRINSICS_H

#include <stdint.h>

/* CPU instruction intrinsics */
void x86_disable_interrupts(void);
void x86_enable_interrupts(void);
void x86_halt(void);
void x86_nop(void);
void x86_pause(void);
void x86_clflush(const void *addr);
void x86_prefetch0(const void *addr);

/* Atomic operations */
uint64_t x86_atomic_inc(volatile uint64_t *ptr);
uint64_t x86_atomic_dec(volatile uint64_t *ptr);
uint64_t x86_atomic_add(volatile uint64_t *ptr, uint64_t val);

/* Inline GCC builtins */
static inline void x86_cpuid_barrier(void) {
    asm volatile("cpuid" : : : "%eax", "%ebx", "%ecx", "%edx");
}

static inline uint64_t x86_read_tsc(void) {
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

static inline void x86_lfence(void) {
    asm volatile("lfence" : : : "memory");
}

static inline void x86_mfence(void) {
    asm volatile("mfence" : : : "memory");
}

static inline void x86_sfence(void) {
    asm volatile("sfence" : : : "memory");
}

/* Bit manipulation */
static inline int x86_bsf(uint64_t val) {
    int pos;
    asm("bsfq %1,%0" : "=r" (pos) : "r" (val));
    return pos;
}

static inline int x86_bsr(uint64_t val) {
    int pos;
    asm("bsrq %1,%0" : "=r" (pos) : "r" (val));
    return pos;
}

#endif /* X86_INTRINSICS_H */
