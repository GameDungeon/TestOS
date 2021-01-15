#include <stdint.h>
#include <stddef.h>
#include <src/kernel/other/stivale2.h>

#include "display/fb.h"


static uint8_t stack[4096];

__attribute__((section(".stivale2hdr"), used))
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

void _start(struct stivale2_struct *stivale2_struct) {

    fb_init(stivale2_struct);

    print_word("Hello World");

    for (;;) {
        asm ("hlt");
    }
}
