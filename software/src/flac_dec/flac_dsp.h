#ifndef FLAC_DSP_H
#define FLAC_DSP_H

#include <stdint.h>

#include "utils.h"
#include "get_bits.h"

/**
 * read unsigned golomb rice code
 */
static inline int get_sr_golomb_flac(GetBitContext *gb, int k, int limit, int esc_len)
{
    int v, i = get_unary1(gb, limit);

    ASSERT(i < limit - 1);

    if (k)
        v = get_bits(gb, k);
    else
        v = 0;

    v += (i << k);
    return (v >> 1) ^ -(v & 1);
}

static inline void flac_lpc_16_c(int32_t *decoded, const int coeffs[32], const int pred_order, const int qlevel, const int len)
{
    int i;

    ASSERT(0 < pred_order && pred_order <= 32);

    for (i = pred_order; i < len - 3; i += 4, decoded += 4) {
        int c, d0 = 0, d1 = 0, d2 = 0, d3 = 0, s0 = 0, s1 = 0, s2 = 0, s3 = 0;

        switch (pred_order) {
            case 32: s3 += (c = coeffs[31]) * d2;
                     s2 += c * d1;
                     s1 += c * d0;
                     s0 += c * (d3 = decoded[31]);
            case 31: s3 += (c = coeffs[30]) * d1;
                     s2 += c * d0;
                     s1 += c * d3;
                     s0 += c * (d2 = decoded[30]);
            case 30: s3 += (c = coeffs[29]) * d0;
                     s2 += c * d3;
                     s1 += c * d2;
                     s0 += c * (d1 = decoded[29]);
            case 29: s3 += (c = coeffs[28]) * d3;
                     s2 += c * d2;
                     s1 += c * d1;
                     s0 += c * (d0 = decoded[28]);
            case 28: s3 += (c = coeffs[27]) * d2;
                     s2 += c * d1;
                     s1 += c * d0;
                     s0 += c * (d3 = decoded[27]);
            case 27: s3 += (c = coeffs[26]) * d1;
                     s2 += c * d0;
                     s1 += c * d3;
                     s0 += c * (d2 = decoded[26]);
            case 26: s3 += (c = coeffs[25]) * d0;
                     s2 += c * d3;
                     s1 += c * d2;
                     s0 += c * (d1 = decoded[25]);
            case 25: s3 += (c = coeffs[24]) * d3;
                     s2 += c * d2;
                     s1 += c * d1;
                     s0 += c * (d0 = decoded[24]);
            case 24: s3 += (c = coeffs[23]) * d2;
                     s2 += c * d1;
                     s1 += c * d0;
                     s0 += c * (d3 = decoded[23]);
            case 23: s3 += (c = coeffs[22]) * d1;
                     s2 += c * d0;
                     s1 += c * d3;
                     s0 += c * (d2 = decoded[22]);
            case 22: s3 += (c = coeffs[21]) * d0;
                     s2 += c * d3;
                     s1 += c * d2;
                     s0 += c * (d1 = decoded[21]);
            case 21: s3 += (c = coeffs[20]) * d3;
                     s2 += c * d2;
                     s1 += c * d1;
                     s0 += c * (d0 = decoded[20]);
            case 20: s3 += (c = coeffs[19]) * d2;
                     s2 += c * d1;
                     s1 += c * d0;
                     s0 += c * (d3 = decoded[19]);
            case 19: s3 += (c = coeffs[18]) * d1;
                     s2 += c * d0;
                     s1 += c * d3;
                     s0 += c * (d2 = decoded[18]);
            case 18: s3 += (c = coeffs[17]) * d0;
                     s2 += c * d3;
                     s1 += c * d2;
                     s0 += c * (d1 = decoded[17]);
            case 17: s3 += (c = coeffs[16]) * d3;
                     s2 += c * d2;
                     s1 += c * d1;
                     s0 += c * (d0 = decoded[16]);
            case 16: s3 += (c = coeffs[15]) * d2;
                     s2 += c * d1;
                     s1 += c * d0;
                     s0 += c * (d3 = decoded[15]);
            case 15: s3 += (c = coeffs[14]) * d1;
                     s2 += c * d0;
                     s1 += c * d3;
                     s0 += c * (d2 = decoded[14]);
            case 14: s3 += (c = coeffs[13]) * d0;
                     s2 += c * d3;
                     s1 += c * d2;
                     s0 += c * (d1 = decoded[13]);
            case 13: s3 += (c = coeffs[12]) * d3;
                     s2 += c * d2;
                     s1 += c * d1;
                     s0 += c * (d0 = decoded[12]);
            case 12: s3 += (c = coeffs[11]) * d2;
                     s2 += c * d1;
                     s1 += c * d0;
                     s0 += c * (d3 = decoded[11]);
            case 11: s3 += (c = coeffs[10]) * d1;
                     s2 += c * d0;
                     s1 += c * d3;
                     s0 += c * (d2 = decoded[10]);
            case 10: s3 += (c = coeffs[9]) * d0;
                     s2 += c * d3;
                     s1 += c * d2;
                     s0 += c * (d1 = decoded[9]);
            case 9:  s3 += (c = coeffs[8]) * d3;
                     s2 += c * d2;
                     s1 += c * d1;
                     s0 += c * (d0 = decoded[8]);
            case 8:  s3 += (c = coeffs[7]) * d2;
                     s2 += c * d1;
                     s1 += c * d0;
                     s0 += c * (d3 = decoded[7]);
            case 7:  s3 += (c = coeffs[6]) * d1;
                     s2 += c * d0;
                     s1 += c * d3;
                     s0 += c * (d2 = decoded[6]);
            case 6:  s3 += (c = coeffs[5]) * d0;
                     s2 += c * d3;
                     s1 += c * d2;
                     s0 += c * (d1 = decoded[5]);
            case 5:  s3 += (c = coeffs[4]) * d3;
                     s2 += c * d2;
                     s1 += c * d1;
                     s0 += c * (d0 = decoded[4]);
            case 4:  s3 += (c = coeffs[3]) * d2;
                     s2 += c * d1;
                     s1 += c * d0;
                     s0 += c * (d3 = decoded[3]);
            case 3:  s3 += (c = coeffs[2]) * d1;
                     s2 += c * d0;
                     s1 += c * d3;
                     s0 += c * (d2 = decoded[2]);
            case 2:  s3 += (c = coeffs[1]) * d0;
                     s2 += c * d3;
                     s1 += c * d2;
                     s0 += c * (d1 = decoded[1]);
            case 1:  s3 += (c = coeffs[0]) * d3;
                     s2 += c * d2;
                     s1 += c * d1;
                     s0 += c * (d0 = decoded[0]);
        }
        d0 = decoded[pred_order] += s0 >> qlevel;

        s1 += d0 * coeffs[pred_order - 1];
        d1 = decoded[pred_order + 1] += s1 >> qlevel;

        if (pred_order > 1) {
            s2 += d0 * coeffs[pred_order - 2];
            s3 += d1 * coeffs[pred_order - 2];
            if (pred_order > 2) s3 += d0 * coeffs[pred_order - 3];
        }
        s2 += d1 * coeffs[pred_order - 1];
        d2 = decoded[pred_order + 2] += s2 >> qlevel;

        s3 += d2 * coeffs[pred_order - 1];
        decoded[pred_order + 3] += s3 >> qlevel;
    }
    for (; i < len; i++, decoded++) {
        int j, sum = 0;
        for (j = 0; j < pred_order; j++)
            sum += coeffs[j] * decoded[j];
        decoded[pred_order] = sum >> qlevel;
    }
}

static inline void flac_decorrelate_indep_c(int32_t *in_l, int32_t *in_r, int32_t *out_l, int32_t *out_r, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        out_l[i] = in_l[i];
        out_r[i] = in_r[i];
    }
}

static inline void flac_decorrelate_ls_c(int32_t *in_l, int32_t *in_r, int32_t *out_l, int32_t *out_r, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in_l[i];
        int b = in_r[i];
        out_l[i] =  a;
        out_r[i] = (a - b);
    }
}

static inline void flac_decorrelate_rs_c(int32_t *in_l, int32_t *in_r, int32_t *out_l, int32_t *out_r, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in_l[i];
        int b = in_r[i];
        out_l[i] = (a + b);
        out_r[i] =  b;
    }
}

static inline void flac_decorrelate_ms_c(int32_t *in_l, int32_t *in_r, int32_t *out_l, int32_t *out_r, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        int a = in_l[i];
        int b = in_r[i];
        a -= b >> 1;
        out_l[i] = (a + b);
        out_r[i] =  a;
    }
}

#endif // FLAC_DSP_H
