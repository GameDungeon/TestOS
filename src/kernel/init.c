#include "sys/gdt.h"
#include "sys/idt.h"
#include "display/fb.h"

void init(struct stivale2_struct *stivale2_struct)
{
    gdt_init();
    idt_init();
    fb_init(stivale2_struct);
}