

#include "display.h"
#include "system.h"
#include <io.h>

#define DISPLAY_HEIGHT 480
#define DISPLAY_WIDTH 800

alt_u32 frame_buffers[2];

alt_u32 cur_color;
alt_u32 cur_base;

void display_init(alt_u32 framebuffer0, alt_u32 framebuffer1)
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

	  cur_color = 0;
	  cur_base = framebuffer0;
	  frame_buffers[0] = framebuffer0;
	  frame_buffers[1] = framebuffer1;
}

void select_framebuffer(alt_u8 fb_num)
{
	  cur_base = frame_buffers[(0x01 & fb_num)];
}

void show_framebuffer(alt_u8 fb_num)
{
	  IOWR(FRAMEREADER_BASE, 3, 0x01 & fb_num);
}

void set_color(alt_u8 r, alt_u8 g, alt_u8 b)
{
	cur_color = (((alt_u32)r)<<16 | ((alt_u32)g)<<8 | ((alt_u32)b));
}

void set_pixel(alt_u32 x, alt_u32 y)
{
	IOWR(cur_base + (y*DISPLAY_WIDTH+x)*4, 0, cur_color);
}


static alt_32 abs(alt_32 v)
{
	if (v > 0)
		return v;
	else
		return -v;
}

void draw_line(alt_32 x0, alt_32 y0, alt_32 x1, alt_32 y1)
{
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = dx+dy, e2; /* error value e_xy */

  for(;;){  /* loop */
    set_pixel(x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

void fill_rect(alt_32 x, alt_32 y, alt_32 w, alt_32 h)
{
	alt_u32 i,j, b;
	for(i=y; i<y+h; i++)
	{
		b = i*DISPLAY_WIDTH;
		for(j=x; j<x+w; j++)
		{
			IOWR(cur_base + (b+j)*4, 0, cur_color);
		}
	}
}


void clear()
{
	alt_u32 i;
	for(i=0; i<DISPLAY_WIDTH*DISPLAY_HEIGHT; i++)
	{
		IOWR(cur_base + i*4, 0, cur_color);
	}
}

