#include <stdint.h>

struct gdt_pointer
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct gdt_descriptor
{
    uint16_t limit_low16; // The low 16 bits of the limit of the segment
    uint16_t base_low16;  // The low 16 bits of the base of the segment
    uint8_t base_mid8;    // The middle 8 bits of the base of the segment
    uint16_t flags;       // Flags, more info below
    uint8_t base_high8;   // the high 8 bits of the base of the segment
} __attribute__((packed));

struct gdt_pointer gdt_pointer;

void gdt_init()
{
    asm volatile ("lgdt %0" ::"m"(gdt_pointer):"memory");
}

