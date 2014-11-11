#ifndef GET_BITS_H
#define GET_BITS_H

#include <stdio.h>
#include <assert.h>

#include "utils.h"

typedef struct {
    FILE *file;
    uint32_t *buf;
    int pos;
} GetBitContext;

static inline void read_block(GetBitContext *gb)
{
    int i;

    if (fread(gb->buf, 1, 512, gb->file) != 512)
        fprintf(stderr, "warning: reading file block failed\n");

    // swap bytes
    for (i = 0; i < 128; i++)
        gb->buf[i] = (gb->buf[i]<<24) | (gb->buf[i]<<8&0x00FF0000) | (gb->buf[i]>>8&0x0000FF00) | (gb->buf[i]>>24);

    gb->pos -= 4096;
}

static inline unsigned int get_bits(GetBitContext *gb, int n)
{
    uint64_t ret, cache;

    assert(n <= 32);

    while (gb->pos >= 4096)
        read_block(gb);

    cache = (uint64_t)gb->buf[gb->pos >> 5] << 32;

    if ((gb->pos & 0x1f) + n > 32) {
        if (gb->pos + 32 >= 4096)
            read_block(gb);

        cache |= (uint64_t)gb->buf[(gb->pos >> 5) + 1];
    }

    ret = (cache << (gb->pos & 0x1f)) >> (64 - n);
    gb->pos += n;

    return ret;
}

static inline int get_unary1(GetBitContext *gb, int len)
{
    uint32_t cache;
    int n, i;

    n = 0;

    while (1) {
        while (gb->pos >= 4096)
            read_block(gb);

        cache = gb->buf[gb->pos >> 5] << (gb->pos & 0x1f);

        if (cache) {
            i = 31 - fast_log2(cache);

            if (n + i > len)
                i = len - n;

            gb->pos += i + 1;
            return n + i;
        }

        i = 32 - (gb->pos & 0x1f);

        if (n + i > len) {
            gb->pos += len - n;
            return len;
        }

        n += i;
        gb->pos += i;
    }
}

static inline int sign_extend(int val, unsigned bits)
{
    unsigned shift = 8 * sizeof(int) - bits;
    union { unsigned u; int s; } v = { (unsigned) val << shift };
    return v.s >> shift;
}

static inline int get_sbits(GetBitContext *gb, int n)
{
    return sign_extend(get_bits(gb, n), n);
}

static inline unsigned int get_bits1(GetBitContext *gb)
{
    return get_bits(gb, 1);
}

static inline unsigned int align_get_bits(GetBitContext *gb)
{
    if (gb->pos & 7)
        return get_bits(gb, 8 - (gb->pos & 7));

    return 0;
}

static inline void skip_bits(GetBitContext *gb, int n)
{
    gb->pos += n;
}

static inline int64_t get_utf8(GetBitContext *gb)
{
    int64_t val = get_bits(gb, 8);

    int ones = 7 - fast_log2(val ^ 255);

    if (ones == 1)
        return -1;

    val &= 127 >> ones;

    while(--ones > 0){
        int tmp = get_bits(gb, 8) - 128;

        if (tmp >> 6)
            return -1;

        val = (val << 6) + tmp;
    }

    return val;
}

#endif // GET_BITS_H
