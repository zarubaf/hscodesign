#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>

const uint8_t fast_log2_tab[256];

static inline int fast_log2(unsigned int v)
{
    int n = 0;

    if (v & 0xffff0000) {
        v >>= 16;
        n += 16;
    }

    if (v & 0xff00) {
        v >>= 8;
        n += 8;
    }

    return n + fast_log2_tab[v];
}

/*
static inline int fast_sqrt(unsigned int v)
{
    int n = 1;

    if (v & 0xffff0000) {
        v >>= 16;
        n = 256;
    }

    if (v & 0xff000) {
        v >>= 8;
        n *= 16;
    }
    
    if (v & 0xf00) {
        v >>= 4;
        n *= 4;
    }

    return n * fast_sqrt_tab[v];
}
*/

#endif // UTILS_H
