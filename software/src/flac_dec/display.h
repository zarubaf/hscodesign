#include "system.h"
#include <io.h>
#include <ctype.h>

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define DISPLAY_HEIGHT 480
#define DISPLAY_WIDTH 800

static void inline display_init(uint32_t framebuffer0, uint32_t framebuffer1)
{
      IOWR(FRAMEREADER_BASE, 3, 0x0);
      IOWR(FRAMEREADER_BASE, 4, framebuffer0); //frame 0 base address
      IOWR(FRAMEREADER_BASE, 5, DISPLAY_WIDTH*DISPLAY_HEIGHT);
      IOWR(FRAMEREADER_BASE, 6, DISPLAY_WIDTH*DISPLAY_HEIGHT);
      IOWR(FRAMEREADER_BASE, 8, DISPLAY_WIDTH);    //frame 0 width
      IOWR(FRAMEREADER_BASE, 9, DISPLAY_HEIGHT);    //frame 0 height

      IOWR(FRAMEREADER_BASE, 11, framebuffer1); //frame 0 base address
      IOWR(FRAMEREADER_BASE, 12, DISPLAY_WIDTH*DISPLAY_HEIGHT);
      IOWR(FRAMEREADER_BASE, 13, DISPLAY_WIDTH*DISPLAY_HEIGHT);
      IOWR(FRAMEREADER_BASE, 15, DISPLAY_WIDTH);    //frame 0 width
      IOWR(FRAMEREADER_BASE, 16, DISPLAY_HEIGHT);    //frame 0 height

      //enable frame reader
      IOWR(FRAMEREADER_BASE, 0, 0x1);    //control register
}
/*
void select_framebuffer(uint8_t fb_num)
{
      cur_base = frame_buffers[(0x01 & fb_num)];
}
*/

static void inline show_framebuffer(uint8_t fb_num)
{
      IOWR(FRAMEREADER_BASE, 3, 0x01 & fb_num);
}

static uint32_t inline rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (((uint8_t)r)<<16 | ((uint8_t)g)<<8 | ((uint8_t)b));
}

static void inline set_pixel(uint32_t cur_base, uint32_t x, uint32_t y, uint32_t color)
{
    IOWR(cur_base + (y*DISPLAY_WIDTH+x)*4, 0, color);
}


static int abs_1(int v)
{
    if (v > 0)
        return v;
    else
        return -v;
}

static void inline draw_line(uint32_t cur_base, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
{
  int dx =  abs_1(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs_1(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = dx+dy, e2; /* error value e_xy */

  for(;;){  /* loop */
    set_pixel(cur_base, x0,y0, color);
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

static void inline draw_fft(uint32_t cur_base, int *data) {
    int i;
    for(i = 0; i < 480; i++) {
        set_pixel(cur_base, 797, i, rgb((uint8_t) data[i], (uint8_t) data[i], (uint8_t) data[i]));
    }
}

static uint32_t inline shift_display_left(uint32_t cur_base) {
    cur_base += 4;
    IOWR(FRAMEREADER_BASE, 4, cur_base);
    return cur_base;
}


static void inline clear(uint32_t cur_base, uint32_t cur_color)
{
    uint32_t i;
    for(i=0; i<DISPLAY_WIDTH*DISPLAY_HEIGHT; i++)
    {
        IOWR(cur_base + i*4, 0, cur_color);
    }
}
#endif



