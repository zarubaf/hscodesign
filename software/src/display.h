
#include <alt_types.h>

#ifndef _DISPLAY_H_
#define _DISPLAY_H_



void display_init(alt_u32 framebuffer0, alt_u32 framebuffer1);
//selects the buffer which is used by the drawing functions
void select_framebuffer(alt_u8 fb_num);
//selects the buffer which is read by the framereader
void show_framebuffer(alt_u8 fb_num);

void set_color(alt_u8 r, alt_u8 g, alt_u8 b);
void set_pixel(alt_u32 x, alt_u32 y);
void draw_line(alt_32 x0, alt_32 y0, alt_32 x1, alt_32 y1);
void fill_rect(alt_32 x, alt_32 y, alt_32 w, alt_32 h);
void clear();


#endif



