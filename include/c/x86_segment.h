/*
 * x86_segment.h - x86_64 Segmentation and GDT/IDT structures
 * Definitions for Global Descriptor Table, Interrupt Descriptor Table,
 * and segment selectors
 */

#ifndef X86_SEGMENT_H
#define X86_SEGMENT_H

#include <stdint.h>

/* GDT Descriptor flags */
#define GDT_PRESENT         (1 << 7)        /* Segment present */
#define GDT_DPL_KERNEL      (0 << 5)        /* Kernel privilege level */
#define GDT_DPL_USER        (3 << 5)        /* User privilege level */
#define GDT_CODE            (1 << 3)        /* Code segment */
#define GDT_DATA            (0 << 3)        /* Data segment */
#define GDT_EXEC            (1 << 3)        /* Executable */
#define GDT_RW              (1 << 1)        /* Readable/Writable */
#define GDT_ACCESSED        (1 << 0)        /* Accessed */
#define GDT_LONG            (1 << 5)        /* 64-bit (long mode) */
#define GDT_SIZE            (1 << 6)        /* 32-bit (default operand size) */
#define GDT_GRAN            (1 << 7)        /* Granularity (4KB) */

/* Segment types */
#define SEG_TYPE_DATA_RO    0x0             /* Data, read-only */
#define SEG_TYPE_DATA_RW    0x2             /* Data, read/write */
#define SEG_TYPE_CODE_EO    0x8             /* Code, execute-only */
#define SEG_TYPE_CODE_ER    0xA             /* Code, execute/read */

/* Segment selector */
typedef struct {
    uint16_t offset : 13;                   /* Descriptor index */
    uint16_t ti : 1;                        /* Table Indicator (0=GDT, 1=LDT) */
    uint16_t rpl : 2;                       /* Requested Privilege Level */
} segment_selector_t;

/* GDT/LDT Descriptor (8 bytes) */
typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_high : 4;
    uint8_t flags : 4;
    uint8_t base_high;
} gdt_entry_t;

/* Task State Segment (TSS) - simplified */
typedef struct {
    uint32_t reserved0;
    uint64_t rsp0;                          /* Ring 0 stack pointer */
    uint64_t rsp1;                          /* Ring 1 stack pointer */
    uint64_t rsp2;                          /* Ring 2 stack pointer */
    uint32_t reserved1;
    uint32_t reserved2;
    uint64_t ist[7];                        /* Interrupt Stack Table */
    uint32_t reserved3;
    uint32_t reserved4;
    uint16_t iopb_offset;                   /* I/O Permission Bitmap offset */
} tss_t;

/* IDT Gate Descriptor (16 bytes) */
typedef struct {
    uint16_t offset_low;
    uint16_t segment;
    uint8_t ist : 3;                        /* Interrupt Stack Table index */
    uint8_t reserved : 5;
    uint8_t gate_type : 4;
    uint8_t reserved2 : 1;
    uint8_t dpl : 2;
    uint8_t present : 1;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved3;
} idt_entry_t;

/* Gate types */
#define IDT_GATE_INTERRUPT  0xE             /* 64-bit interrupt gate */
#define IDT_GATE_TRAP       0xF             /* 64-bit trap gate */
#define IDT_GATE_TASK       0x5             /* Task gate */

/* GDTR and IDTR structure (for LGDT/LIDT) */
typedef struct {
    uint16_t limit;                         /* Table limit - 1 */
    uint64_t base;                          /* Table base address */
} __attribute__((packed)) descriptor_table_ptr_t;

/* Function prototypes */
void x86_load_gdt(const descriptor_table_ptr_t *gdtr);
void x86_load_idt(const descriptor_table_ptr_t *idtr);
void x86_load_tss(uint16_t tss_selector);
void x86_set_segment(uint16_t selector);
void x86_set_data_segment(uint16_t selector);

descriptor_table_ptr_t x86_read_gdtr(void);
descriptor_table_ptr_t x86_read_idtr(void);

#endif /* X86_SEGMENT_H */
