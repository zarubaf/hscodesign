#include "utils.h"

#include <stdio.h>

unsigned int get_bits(GetBitContext *gb, int n)
{
    int pos, cnt, lost;
    unsigned int tmp, ret;
    pos = 0;
    ret = 0;

    if (n > 31)
        fprintf(stderr, "warning: reading more than 31 bit at once\n");

    //printf("reading %d bits from pos %d of ", n, gb->pos & 7);

    while (pos < n) {
        while (gb->pos >= 4096) {
            if (fread(gb->buf, 1, 512, gb->file) != 512)
                fprintf(stderr, "warning: reading file block failed\n");

            gb->pos -= 4096;
        }

        tmp = (unsigned int)gb->buf[gb->pos >> 3] & (0xff >> (gb->pos & 7));
        cnt = 8 - (lost = (gb->pos & 7));

        if (pos + cnt > n) {
            cnt = n - pos;
            tmp >>= 8 - cnt - lost;
        }

        ret <<= cnt;
        ret |= tmp;
        //printf("%02x", gb->buf[gb->pos >> 3]);

        pos += cnt;
        gb->pos += cnt;
    }

    //printf(", = %x\n", ret);

    return ret;
}

static inline int sign_extend(int val, unsigned bits)
{
    unsigned shift = 8 * sizeof(int) - bits;
    union { unsigned u; int s; } v = { (unsigned) val << shift };
    return v.s >> shift;
}

int get_sbits(GetBitContext *gb, int n)
{
    return sign_extend(get_bits(gb, n), n);
}

unsigned int get_bits1(GetBitContext *gb)
{
    return get_bits(gb, 1);
}

int get_sbits_long(GetBitContext *gb, int n)
{
    return sign_extend(get_bits(gb, n), n);
}

int64_t get_utf8(GetBitContext *gb)
{
    int64_t val;
    GET_UTF8(val, get_bits(gb, 8), return -1;)
    return val;
}

unsigned int align_get_bits(GetBitContext *gb)
{
    if (gb->pos & 7)
        return get_bits(gb, 8 - (gb->pos & 7));

    return 0;
}

void skip_bits(GetBitContext *gb, int n)
{
    gb->pos += n;
}

int get_unary(GetBitContext *gb, int stop, int len)
{
    int i;

    for(i = 0; i < len && get_bits1(gb) != stop; i++);
    return i;
}

static const uint8_t ff_log2_tab[256]={
    0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};

int av_log2(unsigned int v)
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
    n += ff_log2_tab[v];

    return n;
}
