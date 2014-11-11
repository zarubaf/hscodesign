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

#endif // UTILS_H
