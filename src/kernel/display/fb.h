#ifndef FB_H
#define FB_H

struct stivale2_header_tag_framebuffer framebuffer_hdr_tag;

void fb_init(struct stivale2_struct *stivale2_struct);
void print_word(char string[]);
void put_pixel(int x, int y);
void print_char(char letter);

#endif
