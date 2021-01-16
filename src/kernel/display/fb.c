#include <stdint.h>
#include <stddef.h>

#include "../include/stivale2.h"
#include "../kernel.h"
#include "fonts/defualt_font.h"

struct stivale2_struct_tag_framebuffer *fb_hdr_tag;
uint8_t *fb_addr;
int x, y = 0;

struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0
        },
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0
};

struct Color
{
    uint8_t red, green, blue;
};

struct Color BLACK = {0, 0, 0};
struct Color WHITE = {255, 255, 255};

void fb_init(struct stivale2_struct *stivale2_struct)
{
    fb_hdr_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    fb_addr = (uint8_t *)fb_hdr_tag->framebuffer_addr;
}

void put_pixel(struct Color color, int x, int y)
{
    *(struct Color *)&((uint8_t *)fb_addr)[x * (fb_hdr_tag->framebuffer_bpp / 8) + y * fb_hdr_tag->framebuffer_pitch] = color;
}

void delete_char()
{
    for (int cy = 0; cy < 16; cy++)
    {
        for (int cx = 0; cx < 8; cx++)
        {
            put_pixel(BLACK, x * 8 + cx, y * 16 + cy);
        }
    }
}

void print_char(char letter)
{
    switch (letter)
    {
    case '\b':
         x--;
         delete_char();
         return;
    case '\n':
         y++;
         x = 0;
         return;
    }

    int glyph = (letter - 32) * 16;

    for (int cy = 0; cy < 16; cy++)
    {
        for (int cx = 0; cx < 8; cx++)
        {
            if ((fontBitmap[glyph + cy] >> (8 - cx) & 1) != 0)
                put_pixel(WHITE, x * 8 + cx, y*16+cy);
        }
    }
    x++;
}

void print_word(const char *string)
{
    for (size_t i = 0; string[i]; i++)
    {
        print_char(string[i]);
    }
}
