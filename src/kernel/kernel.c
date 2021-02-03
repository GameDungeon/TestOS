#include <stdint.h>
#include <stddef.h>

#include "include/stivale2.h"
#include "display/fb.h"
#include "sys/gdt.h"


static uint8_t stack[4096];

__attribute__((used, section(".stivale2hdr"),))
struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = 0,
    .tags = (uintptr_t)&framebuffer_hdr_tag
};


void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        if (current_tag == NULL) {
            return NULL;
        }

        if (current_tag->identifier == id) {
            return current_tag;
        }
 
        current_tag = (void *)current_tag->next;
    }
}

void init(struct stivale2_struct *stivale2_struct)
{
    fb_init(stivale2_struct);
    gdt_init();
}

void _start(struct stivale2_struct *stivale2_struct)
{    
    init(stivale2_struct);

    print_word("Hello World\nHi FU\b\b");

    for (;;) {
        asm ("hlt");
    }
}
