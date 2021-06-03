#ifndef GDT_H
#define GDT_H

struct gdt_ptr
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

void gdt_init();

#endif