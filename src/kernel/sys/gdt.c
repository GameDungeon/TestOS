#include <stdint.h>
#include "gdt.h"

extern void gdt_flush(uint64_t);

static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

static void gdt_set_gate(int32_t number, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt[number].base_low = (base & 0xFFFF);
    gdt[number].base_mid = (base >> 16) & 0xFF;
    gdt[number].base_high = (base >> 24) & 0xFF;
    gdt[number].limit_low = (limit & 0xFFFF);
    gdt[number].granularity = (limit >> 16) & 0x0F;
    gdt[number].granularity |= granularity & 0xF0;
    gdt[number].access = access;
}

void gdt_init()
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base = (uint64_t)&gdt;

    gdt_set_gate(0, 0, 0x00000000, 0x00, 0x00);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_flush((uint64_t)&gp);
}