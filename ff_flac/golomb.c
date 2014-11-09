#include "golomb.h"

/**
 * read unsigned golomb rice code (jpegls).
 */
static inline int get_ur_golomb_jpegls(GetBitContext *gb, int k, int limit, int esc_len)
{
    unsigned int buf;
    //int log;

    /*
    OPEN_READER(re, gb);
    UPDATE_CACHE(re, gb);
    buf = GET_CACHE(re, gb);

    log = av_log2(buf);

    if (log - k >= 32 - MIN_CACHE_BITS + (MIN_CACHE_BITS == 32) && 32 - log < limit) {
        buf >>= log - k;
        buf += (30 - log) << k;
        LAST_SKIP_BITS(re, gb, 32 + k - log);
        CLOSE_READER(re, gb);

        return buf;
    } else {
    */
        int i;
        /*
        for (i = 0; i < limit && SHOW_UBITS(re, gb, 1) == 0; i++) {
            if (gb->size_in_bits <= re_index)
                return -1;

            LAST_SKIP_BITS(re, gb, 1);
            UPDATE_CACHE(re, gb);
        }
        SKIP_BITS(re, gb, 1);
        */
        for (i = 0; i < limit && get_bits(gb, 1) == 0; i++);

        if (i < limit - 1) {
            if (k) {
                //buf = SHOW_UBITS(re, gb, k);
                //LAST_SKIP_BITS(re, gb, k);
                buf = get_bits(gb, k);
            } else {
                buf = 0;
            }

            //CLOSE_READER(re, gb);
            return buf + (i << k);
        } else if (i == limit - 1) {
            //buf = SHOW_UBITS(re, gb, esc_len);
            //LAST_SKIP_BITS(re, gb, esc_len);
            //CLOSE_READER(re, gb);
            buf = get_bits(gb, esc_len);

            return buf + 1;
        } else
            return -1;
    //}
}

int get_sr_golomb_flac(GetBitContext *gb, int k, int limit, int esc_len)
{
    int v = get_ur_golomb_jpegls(gb, k, limit, esc_len);
    return (v >> 1) ^ -(v & 1);
}
