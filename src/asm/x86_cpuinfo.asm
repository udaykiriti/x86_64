/* x86_cpuinfo.asm - x86_64 CPUID assembly optimizations */

.intel_syntax noprefix
.section .text
.globl x86_cpuid_asm
.globl x86_read_tsc
.globl x86_serialize

/*
 * x86_cpuid_asm: Execute CPUID instruction (assembly version)
 * rdi = leaf, rsi = subleaf, rdx = pointer to cpuid_regs_t
 */
x86_cpuid_asm:
    push rbx
    mov eax, edi           /* leaf into EAX */
    mov ecx, esi           /* subleaf into ECX */
    cpuid                  /* Execute CPUID */
    
    /* Store results */
    mov dword ptr [rdx],      eax    /* regs->eax */
    mov dword ptr [rdx + 4],  ebx    /* regs->ebx */
    mov dword ptr [rdx + 8],  ecx    /* regs->ecx */
    mov dword ptr [rdx + 12], edx    /* regs->edx */
    
    pop rbx
    ret

/*
 * x86_read_tsc: Read Time Stamp Counter
 * Returns TSC value in RAX
 */
x86_read_tsc:
    rdtsc                  /* TSC into EDX:EAX */
    shl rdx, 32
    or rax, rdx            /* Combine into 64-bit RAX */
    ret

/*
 * x86_serialize: Serialize CPU (fence for RDTSC)
 * Use CPUID as a serializing instruction
 */
x86_serialize:
    push rbx
    mov eax, 0
    cpuid                  /* CPUID is serializing */
    pop rbx
    ret
