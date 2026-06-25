/*
 * x86_cpuinfo.c - x86/x86_64 CPU feature detection implementation
 * Implements CPUID instruction interface and CPU feature detection
 */

#include "x86_cpuinfo.h"
#include <stdio.h>
#include <string.h>

/*
 * x86_cpuid - Execute CPUID instruction
 * @leaf: CPUID leaf value
 * @subleaf: CPUID subleaf value (ECX on input)
 * @regs: Output registers (EAX, EBX, ECX, EDX)
 */
void x86_cpuid(uint32_t leaf, uint32_t subleaf, cpuid_regs_t *regs) {
    uint32_t eax = leaf;
    uint32_t ecx = subleaf;
    
    asm volatile(
        "cpuid"
        : "=a" (regs->eax), "=b" (regs->ebx), 
          "=c" (regs->ecx), "=d" (regs->edx)
        : "a" (eax), "c" (ecx)
    );
}

/*
 * x86_get_cpuinfo - Gather CPU information
 * @info: Structure to fill with CPU information
 */
void x86_get_cpuinfo(x86_cpuinfo_t *info) {
    cpuid_regs_t regs;
    char *brand_ptr;
    
    memset(info, 0, sizeof(*info));
    
    /* Get vendor string */
    x86_cpuid(CPUID_LEAF_BASIC, 0, &regs);
    info->max_cpuid = regs.eax;
    
    memcpy(info->vendor, &regs.ebx, 4);
    memcpy(info->vendor + 4, &regs.edx, 4);
    memcpy(info->vendor + 8, &regs.ecx, 4);
    info->vendor[12] = '\0';
    
    /* Get signature and feature flags */
    x86_cpuid(0x01, 0, &regs);
    info->signature = regs.eax;
    info->logical_cpus = ((regs.ebx >> 16) & 0xFF);
    info->cache_line_size = ((regs.ebx & 0xFF) * 8);
    
    /* Check hypervisor bit */
    info->hypervisor = !!(regs.ecx & (1 << 31));
    
    /* Check feature flags */
    info->has_sse = !!(regs.edx & CPUID_EDX_SSE);
    info->has_sse2 = !!(regs.edx & CPUID_EDX_SSE2);
    info->has_sse3 = !!(regs.ecx & CPUID_ECX_SSE3);
    info->has_ssse3 = !!(regs.ecx & CPUID_ECX_SSSE3);
    info->has_sse41 = !!(regs.ecx & CPUID_ECX_SSE4_1);
    info->has_sse42 = !!(regs.ecx & CPUID_ECX_SSE4_2);
    info->has_avx = !!(regs.ecx & CPUID_ECX_AVX);
    info->has_aes = !!(regs.ecx & CPUID_ECX_AES);
    info->has_vmx = !!(regs.ecx & CPUID_ECX_VMX);
    
    /* Get brand string */
    x86_cpuid(0x80000000, 0, &regs);
    if (regs.eax >= 0x80000004) {
        brand_ptr = info->brand;
        
        for (uint32_t leaf = 0x80000002; leaf <= 0x80000004; leaf++) {
            x86_cpuid(leaf, 0, &regs);
            memcpy(brand_ptr, &regs.eax, 4);
            brand_ptr += 4;
            memcpy(brand_ptr, &regs.ebx, 4);
            brand_ptr += 4;
            memcpy(brand_ptr, &regs.ecx, 4);
            brand_ptr += 4;
            memcpy(brand_ptr, &regs.edx, 4);
            brand_ptr += 4;
        }
        info->brand[48] = '\0';
    }
    
    /* Get physical CPU count from extended CPUID if available */
    x86_cpuid(0x80000000, 0, &regs);
    if (regs.eax >= 0x80000008) {
        x86_cpuid(0x80000008, 0, &regs);
        info->physical_cpus = (regs.ecx & 0xFF) + 1;
    } else {
        info->physical_cpus = info->logical_cpus;
    }
}

/*
 * x86_print_cpuinfo - Print CPU information
 * @info: CPU information structure
 */
void x86_print_cpuinfo(const x86_cpuinfo_t *info) {
    printf("\n=== x86_64 CPU Information ===\n");
    printf("Vendor:          %s\n", info->vendor);
    printf("Brand:           %s\n", info->brand);
    printf("Signature:       0x%08x\n", info->signature);
    printf("Family:          0x%02x\n", (info->signature >> 8) & 0x0F);
    printf("Model:           0x%02x\n", (info->signature >> 4) & 0x0F);
    printf("Stepping:        0x%02x\n", info->signature & 0x0F);
    printf("Logical CPUs:    %u\n", info->logical_cpus);
    printf("Physical CPUs:   %u\n", info->physical_cpus);
    printf("Cache Line:      %u bytes\n", info->cache_line_size);
    printf("Hypervisor:      %s\n", info->hypervisor ? "Yes" : "No");
    printf("\nFeature Support:\n");
    printf("  SSE:           %s\n", info->has_sse ? "Yes" : "No");
    printf("  SSE2:          %s\n", info->has_sse2 ? "Yes" : "No");
    printf("  SSE3:          %s\n", info->has_sse3 ? "Yes" : "No");
    printf("  SSSE3:         %s\n", info->has_ssse3 ? "Yes" : "No");
    printf("  SSE4.1:        %s\n", info->has_sse41 ? "Yes" : "No");
    printf("  SSE4.2:        %s\n", info->has_sse42 ? "Yes" : "No");
    printf("  AVX:           %s\n", info->has_avx ? "Yes" : "No");
    printf("  AES-NI:        %s\n", info->has_aes ? "Yes" : "No");
    printf("  VMX:           %s\n", info->has_vmx ? "Yes" : "No");
    printf("\n");
}

/*
 * x86_has_feature - Check if CPU has specific feature
 * @info: CPU information structure
 * @feature: Feature to check
 *
 * Returns: true if feature is supported
 */
bool x86_has_feature(const x86_cpuinfo_t *info, uint32_t feature) {
    switch(feature) {
        case CPUID_EDX_SSE:     return info->has_sse;
        case CPUID_EDX_SSE2:    return info->has_sse2;
        case CPUID_ECX_SSE3:    return info->has_sse3;
        case CPUID_ECX_SSSE3:   return info->has_ssse3;
        case CPUID_ECX_SSE4_1:  return info->has_sse41;
        case CPUID_ECX_SSE4_2:  return info->has_sse42;
        case CPUID_ECX_AVX:     return info->has_avx;
        case CPUID_ECX_AES:     return info->has_aes;
        case CPUID_ECX_VMX:     return info->has_vmx;
        default:                return false;
    }
}
