/*
 * Main entry point for the application.
 * Calls into assembly routines defined in src/hello.asm.
 */

#include <stdio.h>
#include <stdint.h>
#include "math.h"
#include "fastmath.h"
#include "io.h"
#include "simd.h"
#include "stats.h"
#include "string.h"

extern void _hello(void);

int main(void) {
    _hello();

    int a = 10 ,  b = 5;
    
    printf("C Math: %d + %d = %d\n", a, b, add(a, b));
    printf("C Math: %d - %d = %d\n", a, b, subtract(a, b));
    printf("C Math: %d * %d = %d\n", a, b, multiply(a, b));

    // Factorial demo
    printf("C Math: factorial(5) = %d\n", factorial(5));

    // Math Assembly
    printf("ASM Math: %d + %d = %d\n", a, b, asm_add(a, b));
    printf("ASM Math: %d - %d = %d\n", a, b, asm_sub(a, b));
    printf("ASM Math: factorial(5) = %d\n", asm_factorial(5));

    // String demo
    char txt[] = "time";
    printf("String: '%s' (len: %d)\n", txt, str_len(txt));
    
    str_rev(txt);
    printf("Reversed: '%s'\n", txt);

    printf("Type a line for ASM echo: ");
    fflush(stdout);
    int nrd = asm_echo();

    printf("ASM IO: echoed %d byte(s)\n", nrd);

    int v1[4] = {1, 2, 3, 4};
    int v2[4] = {10, 20, 30, 40};
    int vo[4] = {0, 0, 0, 0};

    asm_vec4_add_i32(v1, v2, vo);
    printf("SIMD add: [%d, %d, %d, %d]\n", vo[0], vo[1], vo[2], vo[3]);

    uint64_t t0 = asm_rdtsc_serialized();
    for (int i = 0; i < 1000000; i++) {
        asm_vec4_add_i32(v1, v2, vo);
    }
    uint64_t t1 = asm_rdtsc_serialized();
    printf("SIMD loop cycles (1,000,000 iters): %llu\n",
           (unsigned long long)(t1 - t0));

    int vals[] = {12, -3, 44, 18, 27, 5};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    IntSummary stats;
    int lim = 20;

    if (calculate_int_summary(vals, n, &stats)) {
        printf("C Stats: min=%d max=%d sum=%lld avg=%.2f\n",
               stats.min,
               stats.max,
               stats.sum,
               stats.avg);
    }

    int idx = find_first_above_threshold(vals, n, lim);
    printf("C Stats: first value > %d at index %d\n", lim, idx);

    return 0;
}
