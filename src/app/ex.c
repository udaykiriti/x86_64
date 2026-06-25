/*
 * x86_practical_examples.c - Practical usage examples for x86_64 features
 * Demonstrates real-world applications of processor-level operations
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/c/x86_cpuinfo.h"
#include "../include/c/x86_registers.h"
#include "../include/c/x86_intrinsics.h"

/*
 * Conditional compilation based on CPU features
 */
void example_conditional_cpu_features(void) {
    printf("\nConditional CPU Feature Usage\n");
    
    x86_cpuinfo_t info;
    x86_get_cpuinfo(&info);
    
    if (info.has_avx) {
        printf("  Using AVX optimized code path\n");
        /* Call AVX-optimized function */
    } else if (info.has_sse2) {
        printf("  Using SSE2 optimized code path\n");
        /* Call SSE2-optimized function */
    } else {
        printf("  Using scalar code path\n");
        /* Call scalar implementation */
    }
}

/*
 * Measure function performance with TSC
 */
uint64_t measure_function_cycles(void (*func)(void)) {
    uint64_t start, end;
    
    x86_cpuid_barrier();
    start = x86_read_tsc();
    
    func();
    
    end = x86_read_tsc();
    x86_cpuid_barrier();
    
    return end - start;
}

void dummy_work(void) {
    volatile int sum = 0;
    for (int i = 0; i < 100; i++) {
        sum += i;
    }
}

void example_performance_measurement(void) {
    printf("\nPerformance Measurement\n");
    
    uint64_t cycles = measure_function_cycles(dummy_work);
    printf("  Function took %lu CPU cycles\n", cycles);
}

/*
 * Lock-free counter with atomic operations
 */
typedef struct {
    volatile uint64_t value;
} lockfree_counter_t;

void lockfree_increment(lockfree_counter_t *counter) {
    x86_atomic_inc(&counter->value);
}

uint64_t lockfree_read(const lockfree_counter_t *counter) {
    return counter->value;
}

void example_lockfree_operations(void) {
    printf("\nLock-Free Counter\n");
    
    lockfree_counter_t counter = {0};
    
    for (int i = 0; i < 10; i++) {
        lockfree_increment(&counter);
    }
    
    printf("  Counter value: %lu\n", lockfree_read(&counter));
}

/*
 * Spin lock with PAUSE optimization
 */
typedef struct {
    volatile uint32_t locked;
} spinlock_t;

void spin_acquire(spinlock_t *lock) {
    while (__atomic_test_and_set(&lock->locked, __ATOMIC_ACQ_REL)) {
        x86_pause();  /* Reduce power consumption */
    }
}

void spin_release(spinlock_t *lock) {
    __atomic_clear(&lock->locked, __ATOMIC_ACQ_REL);
}

void example_spin_lock(void) {
    printf("\nSpin Lock with PAUSE\n");
    
    spinlock_t lock = {0};
    
    spin_acquire(&lock);
    printf("  Lock acquired\n");
    
    /* Critical section */
    volatile int critical_data = 42;
    
    spin_release(&lock);
    printf("  Lock released\n");
}

/*
 * Cache-aware data alignment
 */
typedef struct {
    uint64_t counter1;
    char padding[56];  /* Pad to 64 bytes (cache line) */
    uint64_t counter2;
    char padding2[56];
} aligned_counters_t __attribute__((aligned(64)));

void example_cache_alignment(void) {
    printf("\nCache-Aligned Data\n");
    
    aligned_counters_t counters;
    
    printf("  Counter1 address: %p\n", (void *)&counters.counter1);
    printf("  Counter2 address: %p\n", (void *)&counters.counter2);
    printf("  Distance: %lu bytes (one cache line)\n", 
           (uintptr_t)&counters.counter2 - (uintptr_t)&counters.counter1);
}

/*
 * Memory barriers for synchronization
 */
typedef struct {
    uint64_t flag;
    uint64_t data;
} shared_data_t;

void producer_writes_data(shared_data_t *shared) {
    shared->data = 42;
    x86_mfence();      /* Ensure data write is visible */
    shared->flag = 1;
}

uint64_t consumer_reads_data(shared_data_t *shared) {
    if (shared->flag) {
        x86_mfence();  /* Ensure visibility */
        return shared->data;
    }
    return 0;
}

void example_memory_barriers(void) {
    printf("\nMemory Barriers for Synchronization\n");
    
    shared_data_t shared = {0, 0};
    
    producer_writes_data(&shared);
    printf("  Data produced: %lu\n", shared.data);
    
    uint64_t value = consumer_reads_data(&shared);
    printf("  Data consumed: %lu\n", value);
}

/*
 * Bit manipulation for efficient operations
 */
int count_set_bits(uint64_t value) {
    int count = 0;
    while (value) {
        int pos = x86_bsf(value);  /* Find lowest set bit */
        count++;
        value &= value - 1;         /* Clear the bit */
    }
    return count;
}

void example_bit_operations(void) {
    printf("\nBit Manipulation\n");
    
    uint64_t value = 0xF0F0F0F0F0F0F0F0;
    int bits = count_set_bits(value);
    printf("  Value: 0x%lx\n", value);
    printf("  Set bits: %d\n", bits);
}

/*
 * Check for hypervisor
 */
void example_hypervisor_detection(void) {
    printf("\nHypervisor Detection\n");
    
    x86_cpuinfo_t info;
    x86_get_cpuinfo(&info);
    
    if (info.hypervisor) {
        printf("  Running under hypervisor\n");
        printf("  Vendor: %s\n", info.vendor);
    } else {
        printf("  Running on bare metal\n");
    }
}

int main(void) {
    printf("\nX86_64 Processor Features - Practical Examples\n");
    
    example_conditional_cpu_features();
    example_performance_measurement();
    example_lockfree_operations();
    example_spin_lock();
    example_cache_alignment();
    example_memory_barriers();
    example_bit_operations();
    example_hypervisor_detection();
    
    return 0;
}
