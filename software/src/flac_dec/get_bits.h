#ifndef GET_BITS_H
#define GET_BITS_H

#include <system.h>
#include "utils.h"

typedef struct GetBitContext_s {
    void (*read_block)(struct GetBitContext_s *gb);
    int blk_cnt;
    uint32_t *buf;
    int pos;
} GetBitContext;

static inline unsigned int get_bits(GetBitContext *gb, int n)
{
    uint64_t ret, cache;
    //uint32_t ret2, ret3;

    ASSERT(n <= 32);

    while (gb->pos >= 4096)
        gb->read_block(gb);

    cache = (uint64_t)gb->buf[ALT_CI_SHIFT_R(gb->pos,5)] << 32;
    //cache = (uint64_t)gb->buf[gb->pos >> 5] << 32;

    if ((gb->pos & 0x1f) + n > 32) {
        if (gb->pos + 32 >= 4096)
            gb->read_block(gb);

        cache |= (uint64_t)gb->buf[(ALT_CI_SHIFT_R(gb->pos,5)) + 1];
        //cache |= (uint64_t)gb->buf[(gb->pos >> 5) + 1];
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
            gb->read_block(gb);

        cache = ALT_CI_SHIFT_L(gb->buf[ALT_CI_SHIFT_R(gb->pos,5)], (gb->pos & 0x1f));
        //cache = gb->buf[gb->pos >> 5] << (gb->pos & 0x1f);

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
    union { unsigned u; int s; } v = { (unsigned) ALT_CI_SHIFT_L(val, shift) };
    //union { unsigned u; int s; } v = { (unsigned) val << shift };
    //int ret = ALT_CI_SHIFT_R(v.s, shift);
    //return ret;
    return v.s >> shift;    /* custom instruction mogned */
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

    val &= ALT_CI_SHIFT_R(127, ones);
    //val &= 127 >> ones;

    while(--ones > 0){
        int tmp = get_bits(gb, 8) - 128;

        if (ALT_CI_SHIFT_R(tmp, 6))
        //if (tmp >> 6)
            return -1;

        val = (ALT_CI_SHIFT_L(val, 6)) + tmp;
        //val = (val << 6) + tmp;
    }

    return val;
}

#endif // GET_BITS_H
