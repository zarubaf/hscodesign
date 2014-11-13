#ifndef FLAC_DSP_H
#define FLAC_DSP_H

#include <stdint.h>
#include <assert.h>

#include "utils.h"
#include "get_bits.h"

/**
 * read unsigned golomb rice code
 */
static inline int get_sr_golomb_flac(GetBitContext *gb, int k, int limit, int esc_len)
{
    int v, i = get_unary1(gb, limit);

    assert(i < limit - 1);

    if (k)
        v = get_bits(gb, k);
    else
        v = 0;

    v += (i << k);
    return (v >> 1) ^ -(v & 1);
}

static inline void flac_lpc_16_c(int32_t *decoded, const int coeffs[32], int pred_order, int qlevel, int len)
{
    int i, j;
    
    for (i = pred_order; i < len; i++, decoded++) {
        int sum = 0;
        for (j = 0; j < pred_order; j++)
            sum += coeffs[j] * decoded[j];
        decoded[j] += sum >> qlevel;
    }
}

static inline void flac_decorrelate_indep_c(int16_t **out, int32_t **in, int channels, int len)
{
    int i, j;

    for (j = 0; j < len; j++)
        for (i = 0; i < channels; i++)
            out[i][j] = in[i][j];
}

static inline void flac_decorrelate_ls_c(int16_t **out, int32_t **in, int channels, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in[0][i];
        int b = in[1][i];
        out[0][i] =  a;
        out[1][i] = (a - b);
    }
}

static inline void flac_decorrelate_rs_c(int16_t **out, int32_t **in, int channels, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in[0][i];
        int b = in[1][i];
        out[0][i] = (a + b);
        out[1][i] =  b;
    }
}

static inline void flac_decorrelate_ms_c(int16_t **out, int32_t **in, int channels, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in[0][i];
        int b = in[1][i];
        a -= b >> 1;
        out[0][i] = (a + b);
        out[1][i] =  a;
    }
}

#endif // FLAC_DSP_H
