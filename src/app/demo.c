/*
 * demo_x86_processor.c - Demonstration of x86_64 processor features
 * Shows CPU information, registers, and low-level operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/c/x86_cpuinfo.h"
#include "../include/c/x86_registers.h"
#include "../include/c/x86_intrinsics.h"

void demo_cpuinfo(void) {
    printf("\n CPU Information \n");
    
    x86_cpuinfo_t cpu_info;
    x86_get_cpuinfo(&cpu_info);
    x86_print_cpuinfo(&cpu_info);
}

void demo_control_registers(void) {
    printf("\n Control Registers \n");
    
    x86_control_regs_t ctrl_regs;
    x86_get_control_regs(&ctrl_regs);
    x86_print_control_regs(&ctrl_regs);
}

void demo_timing(void) {
    printf("\n TSC Timing Demo \n");
    
    uint64_t start, end;
    volatile uint64_t sum = 0;
    
    x86_cpuid_barrier();
    start = x86_read_tsc();
    
    for (int i = 0; i < 1000000; i++) {
        sum += i;
    }
    
    end = x86_read_tsc();
    x86_cpuid_barrier();
    
    uint64_t cycles = end - start;
    printf("Loop iterations: 1,000,000\n");
    printf("Total cycles: %lu\n", cycles);
    printf("Cycles per iteration: %.3f\n", (double)cycles / 1000000);
}

void demo_memory_barriers(void) {
    printf("\n Memory Barriers \n");
    
    printf("Testing LFENCE, MFENCE, SFENCE...\n");
    x86_lfence();
    printf("LFENCE executed\n");
    x86_sfence();
    printf("SFENCE executed\n");
    x86_mfence();
    printf("MFENCE executed\n");
}

void demo_cache_ops(void) {
    printf("\n Cache Operations \n");
    
    char buffer[256] __attribute__((aligned(64)));
    memset(buffer, 0xAA, sizeof(buffer));
    
    printf("Buffer at: %p\n", (void *)buffer);
    printf("Buffer size: %zu bytes\n", sizeof(buffer));
    
    x86_prefetch0(buffer);
    printf("Prefetched into L1 cache\n");
    
    for (int i = 0; i < 256; i += 64) {
        buffer[i] = 0xFF;
    }
    printf("Cache line filled\n");
    
    x86_clflush(buffer);
    printf("Cache line flushed\n");
}

void demo_bit_operations(void) {
    printf("\n Bit Manipulation \n");
    
    uint64_t value = 0x80000000;
    printf("Value: 0x%016lx\n", value);
    printf("BSF: %d (bit position of LSB)\n", x86_bsf(value));
    printf("BSR: %d (bit position of MSB)\n", x86_bsr(value));
    
    value = 0x0100000000000000;
    printf("\nValue: 0x%016lx\n", value);
    printf("BSF: %d\n", x86_bsf(value));
    printf("BSR: %d\n", x86_bsr(value));
}

void demo_atomic_ops(void) {
    printf("\n=== Atomic Operations ===\n");
    
    volatile uint64_t counter = 100;
    printf("Initial value: %lu\n", counter);
    
    uint64_t new_val = x86_atomic_add(&counter, 50);
    printf("After atomic add 50: %lu\n", new_val);
    
    new_val = x86_atomic_inc(&counter);
    printf("After atomic inc: %lu\n", new_val);
    
    new_val = x86_atomic_dec(&counter);
    printf("After atomic dec: %lu\n", new_val);
}

void demo_pause_instruction(void) {
    printf("\n PAUSE Instruction \n");
    
    printf("PAUSE reduces power in spin loops\n");
    printf("Executing PAUSE 5 times...\n");
    for (int i = 0; i < 5; i++) {
        x86_pause();
    }
    printf("PAUSE instruction demo complete\n");
}

int main(void) {
    printf("\nX86_64 PROCESSOR FEATURES DEMO\n");
    printf("Systems Programming on x86_64\n");
    
    demo_cpuinfo();
    demo_control_registers();
    demo_timing();
    demo_memory_barriers();
    demo_cache_ops();
    demo_bit_operations();
    demo_atomic_ops();
    demo_pause_instruction();
    
    printf("\nAll demos completed successfully\n\n");
    
    return EXIT_SUCCESS;
}
