#ifndef GOLOMB_H
#define GOLOMB_H

#include "utils.h"

/**
 * read signed golomb rice code (flac).
 */
int get_sr_golomb_flac(GetBitContext *gb, int k, int limit, int esc_len);

#endif // GOLOMB_H
