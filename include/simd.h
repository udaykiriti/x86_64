#ifndef SIMD_H
#define SIMD_H

#include <stdint.h>

/* Adds 4 packed int32 values using SSE2. */
void asm_vec4_add_i32(const int *a, const int *b, int *out);

/* Reads serialized timestamp counter (TSC). */
uint64_t asm_rdtsc_serialized(void);

#endif
