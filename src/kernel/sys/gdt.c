#include <stdint.h>
#include "gdt.h"

extern void gdt_flush(uint64_t);

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

static struct gdt_entry gdt[6];
static struct gdt_ptr gp;

static void gdt_set_entry(int32_t number, uint32_t base_low, uint32_t base_mid, uint32_t base_high, uint32_t limit_low, uint8_t access, uint8_t granularity)
{
    gdt[number].base_low = base_low;
    gdt[number].base_mid = base_mid;
    gdt[number].base_high = base_high;
    gdt[number].limit_low = limit_low;
    gdt[number].granularity = granularity;
    gdt[number].access = access;
}

void gdt_init()
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base = (uint64_t)&gdt;

    //            X  Base low    Base Mid    Base High   Limit Low   access gran
    gdt_set_entry(0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00, 0x00); //null
    gdt_set_entry(1, 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0x98, 0x20); //kernel code64
    gdt_set_entry(2, 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0x92, 0x40); //kernel data64
    gdt_set_entry(3, 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0xF2, 0x40); //user data64
    gdt_set_entry(4, 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0xF8, 0x20); //user code64
    gdt_set_entry(5, 0x00000000, 0x00000000, 0x00000000, 0xffffffff, 0x18, 0x00); //tss

    gdt_flush((uint64_t)&gp);
}
