#ifndef FLAC_DEC_H
#define FLAC_DEC_H

#include "utils.h"
#include "get_bits.h"

#define FLAC_MAX_CHANNELS 8

enum {
    FLAC_CHMODE_INDEPENDENT = 0,
    FLAC_CHMODE_LEFT_SIDE = 1,
    FLAC_CHMODE_RIGHT_SIDE = 2,
    FLAC_CHMODE_MID_SIDE = 3
};

int decode_frame(GetBitContext *gb, int32_t *out_l, int32_t *out_r, int *ch_mode);

#endif // FLAC_DEC_H
