#include "system.h"
#include <io.h>
#include <ctype.h>

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define DISPLAY_HEIGHT 480
#define DISPLAY_WIDTH 800

static inline int32_t *display_init()
{
    uint32_t *framebuffer0, *framebuffer1;

    framebuffer0 = malloc(DISPLAY_HEIGHT * DISPLAY_WIDTH * 4 * 2);
    framebuffer1 = framebuffer0 + DISPLAY_HEIGHT * DISPLAY_WIDTH;

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
    //IOWR(FRAMEREADER_BASE, 0, 0x1);    //control register
    IOWR(FRAMEREADER_BASE, 0, 1);

    // show framebuffer
    IOWR(FRAMEREADER_BASE, 3, 0);

    return framebuffer0;
}

/*
static uint32_t inline rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (((uint8_t)r)<<16 | ((uint8_t)g)<<8 | ((uint8_t)b));
}
*/

/*
static void inline set_pixel(uint32_t *cur_base, uint32_t x, uint32_t y, uint32_t color)
{
    IOWR(cur_base + (y*DISPLAY_WIDTH+x), 0, color);
}
*/

static void inline draw_fft(uint32_t *cur_base, uint32_t *data)
{
    int i;
    cur_base += 797;

    for (i = 479; i >= 0; i--) {
        IOWR(cur_base, 0, data[i]);
        cur_base += DISPLAY_WIDTH;
    }
        //set_pixel(cur_base, 797, DISPLAY_HEIGHT - 1 - i, data);
}

static void inline shift_display_left(uint32_t **cur_base)
{
    (*cur_base)++;
    IOWR(FRAMEREADER_BASE, 4, *cur_base);
}


static void inline clear(uint32_t *cur_base, uint32_t cur_color)
{
    uint32_t i;
    for(i=0; i<DISPLAY_WIDTH*DISPLAY_HEIGHT; i++)
    {
        IOWR(cur_base + i, 0, cur_color);
    }
}
#endif



