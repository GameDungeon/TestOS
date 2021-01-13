#include <stdint.h>
#include <stddef.h>
#include <other/stivale2.h>

#include "../other/kernel.h"

struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    // All tags need to begin with an identifier and a pointer to the next tag.
    .tag = {
        // Identification constant defined in stivale2.h and the specification.
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        // If next is 0, then this marks the end of the linked list of tags.
        .next = 0},
    // We set all the framebuffer specifics to 0 as we want the bootloader
    // to pick the best it can.
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0
};

struct Pixel
{
    uint8_t red, green, blue;
};

struct stivale2_struct_tag_framebuffer *fb_hdr_tag;
uint8_t *fb_addr;

void fb_init(struct stivale2_struct *stivale2_struct)
{
    fb_hdr_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    fb_addr = (uint8_t *)fb_hdr_tag->framebuffer_addr;
}

void put_pixel(int x, int y)
{
    struct Pixel color = {255,255,255};
    *(struct Pixel *)&((uint8_t *)fb_addr)[x * (fb_hdr_tag->framebuffer_bpp / 8) + y * fb_hdr_tag->framebuffer_pitch] = color;
}

void print_word(char string[])
{
    for (int i = 0; i == 100; i++)
    {
        put_pixel(i, i);
    }
}
