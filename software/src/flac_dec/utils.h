#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define ASSERT(cond) if (!(cond)) { printf("error: " __FILE__ ": assertion (" #cond ") in line failed\n"); exit(1); }
#else
#define ASSERT(cond)
#endif

extern const uint8_t fast_log2_tab[256];

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

static inline uint32_t fast_sqrt(uint32_t v)
{
    register uint32_t root, remain, place;

    root = 0;
    remain = v;
    place = 1 << 30;

    while (place > remain)
        place >>= 2;

    while (place) {
        if (remain >= root + place) {
            remain -= root + place;
            root += place << 1;
        }

        root >>= 1;
        place >>= 2;
    }

    return root;
}

#endif // UTILS_H
