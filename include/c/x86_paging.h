/*
 * x86_paging.h - x86_64 Virtual Memory Paging Structures
 * Definitions for page table entries, TLB operations, and memory attributes
 */

#ifndef X86_PAGING_H
#define X86_PAGING_H

#include <stdint.h>

/* Page size */
#define PAGE_SIZE           4096
#define PAGE_SHIFT          12
#define PAGE_MASK           0xFFF
#define PAGE_ALIGN(x)       ((x + PAGE_SIZE - 1) & ~PAGE_MASK)

/* Page table entry (PTE) flags */
#define PTE_PRESENT         (1UL << 0)      /* Page present in memory */
#define PTE_WRITE           (1UL << 1)      /* Writeable page */
#define PTE_USER            (1UL << 2)      /* User-accessible */
#define PTE_PWT             (1UL << 3)      /* Page write-through */
#define PTE_PCD             (1UL << 4)      /* Page cache disabled */
#define PTE_ACCESSED        (1UL << 5)      /* Page accessed */
#define PTE_DIRTY           (1UL << 6)      /* Page written to */
#define PTE_PAT             (1UL << 7)      /* Page attribute table */
#define PTE_GLOBAL          (1UL << 8)      /* Global TLB entry */
#define PTE_RESERVED        (1UL << 9)      /* Reserved for software */
#define PTE_COPYONWRITE     (1UL << 10)     /* Copy-on-write */
#define PTE_MPROTECT        (1UL << 11)     /* Memory protect bit */
#define PTE_NX              (1UL << 63)     /* No-execute bit (IA32_EFER.NXE) */

/* Physical address mask (removes flags) */
#define PTE_ADDR_MASK       0x000FFFFFFFFFF000UL

/* Large page flags (2MB and 1GB pages) */
#define PTE_PSE             (1UL << 7)      /* Page Size Extension */

/* Memory type (PAT index) */
#define PAT_UC              0               /* Uncacheable */
#define PAT_WC              1               /* Write-combining */
#define PAT_WT              4               /* Write-through */
#define PAT_WP              5               /* Write-protected */
#define PAT_WB              6               /* Write-back */
#define PAT_UC_MINUS        7               /* Uncacheable (minus) */

typedef struct {
    uint64_t pml4[512];                     /* Page Map Level 4 table */
} pml4_t;

typedef struct {
    uint64_t pdp[512];                      /* Page Directory Pointer table */
} pdp_t;

typedef struct {
    uint64_t pd[512];                       /* Page Directory */
} pd_t;

typedef struct {
    uint64_t pt[512];                       /* Page Table */
} pt_t;

typedef struct {
    uint64_t pml4_addr;                     /* PML4 physical address */
    uint64_t virtual_addr;
    uint64_t physical_addr;
    uint64_t flags;
} virt_page_info_t;

/* TLB operation functions */
void x86_invalidate_tlb_single(uint64_t addr);
void x86_invalidate_tlb_all(void);
void x86_load_cr3(uint64_t cr3);
uint64_t x86_read_cr3(void);

/* Page table functions */
uint64_t x86_get_phys_addr(uint64_t virt_addr);
int x86_set_page_attr(uint64_t addr, uint64_t size, uint64_t attr);
void x86_print_page_info(uint64_t addr);

#endif /* X86_PAGING_H */
