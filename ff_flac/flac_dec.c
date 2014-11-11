#include "flac_dec.h"

#include <limits.h>

#include "flac_dsp.h"

static const int8_t sample_size_table[] = { 0, 8, 12, 0, 16, 20, 24, 0 };

static const int ff_flac_sample_rate_table[16] = {
    0, 88200, 176400, 192000, 8000, 16000, 22050, 24000, 32000, 44100, 48000, 96000, 0, 0, 0, 0
};

static const int32_t ff_flac_blocksize_table[16] = {
    0, 192, 576<<0, 576<<1, 576<<2, 576<<3, 0, 0,
    256<<0, 256<<1, 256<<2, 256<<3, 256<<4, 256<<5, 256<<6, 256<<7
};

int ff_flac_decode_frame_header(GetBitContext *gb, FLACFrameInfo *fi)
{
    int bs_code, sr_code, bps_code;

    /* frame sync code */
    if ((get_bits(gb, 15) & 0x7FFF) != 0x7FFC) {
        fprintf(stderr, "invalid sync code\n");
        return -1;
    }

    /* variable block size stream code */
    fi->is_var_size = get_bits1(gb);

    /* block size and sample rate codes */
    bs_code = get_bits(gb, 4);
    sr_code = get_bits(gb, 4);

    /* channels and decorrelation */
    fi->ch_mode = get_bits(gb, 4);
    if (fi->ch_mode < FLAC_MAX_CHANNELS) {
        fi->channels = fi->ch_mode + 1;
        fi->ch_mode = FLAC_CHMODE_INDEPENDENT;
    } else if (fi->ch_mode < FLAC_MAX_CHANNELS + FLAC_CHMODE_MID_SIDE) {
        fi->channels = 2;
        fi->ch_mode -= FLAC_MAX_CHANNELS - 1;
    } else {
        fprintf(stderr, "invalid channel mode: %d\n", fi->ch_mode);
        return -1;
    }

    /* bits per sample */
    bps_code = get_bits(gb, 3);
    if (bps_code == 3 || bps_code == 7) {
        fprintf(stderr, "invalid sample size code (%d)\n", bps_code);
        return -1;
    }
    fi->bps = sample_size_table[bps_code];

    /* reserved bit */
    if (get_bits1(gb)) {
        fprintf(stderr, "broken stream, invalid padding\n");
        return -1;
    }

    /* sample or frame count */
    fi->frame_or_sample_num = get_utf8(gb);
    if (fi->frame_or_sample_num < 0) {
        fprintf(stderr, "sample/frame number invalid; utf8 fscked\n");
        return -1;
    }

    /* blocksize */
    if (bs_code == 0) {
        fprintf(stderr, "reserved blocksize code: 0\n");
        return -1;
    } else if (bs_code == 6) {
        fi->blocksize = get_bits(gb, 8) + 1;
    } else if (bs_code == 7) {
        fi->blocksize = get_bits(gb, 16) + 1;
    } else {
        fi->blocksize = ff_flac_blocksize_table[bs_code];
    }

    /* sample rate */
    if (sr_code < 12) {
        fi->samplerate = ff_flac_sample_rate_table[sr_code];
    } else if (sr_code == 12) {
        fi->samplerate = get_bits(gb, 8) * 1000;
    } else if (sr_code == 13) {
        fi->samplerate = get_bits(gb, 16);
    } else if (sr_code == 14) {
        fi->samplerate = get_bits(gb, 16) * 10;
    } else {
        fprintf(stderr, "illegal sample rate code %d\n", sr_code);
        return -1;
    }

    /* header CRC-8 check */
    skip_bits(gb, 8);
    return 0;
}

static int decode_residuals(GetBitContext *gb, int blocksize, int32_t *decoded, int pred_order)
{
    int i, tmp, partition, method_type, rice_order;
    int rice_bits, rice_esc;
    int samples;

    method_type = get_bits(gb, 2);
    if (method_type > 1) {
        fprintf(stderr,"illegal residual coding method %d\n",
               method_type);
        return -1;
    }

    rice_order = get_bits(gb, 4);

    samples= blocksize >> rice_order;
    if (samples << rice_order != blocksize) {
        fprintf(stderr,"invalid rice order: %i blocksize %i\n",
               rice_order, blocksize);
        return -1;
    }

    if (pred_order > samples) {
        fprintf(stderr,"invalid predictor order: %i > %i\n",
               pred_order, samples);
        return -1;
    }

    rice_bits = 4 + method_type;
    rice_esc  = (1 << rice_bits) - 1;

    decoded += pred_order;
    i= pred_order;
    for (partition = 0; partition < (1 << rice_order); partition++) {
        tmp = get_bits(gb, rice_bits);
        if (tmp == rice_esc) {
            tmp = get_bits(gb, 5);
            for (; i < samples; i++)
                *decoded++ = get_sbits(gb, tmp);
        } else {
            for (; i < samples; i++) {
                *decoded++ = get_sr_golomb_flac(gb, tmp, INT_MAX, 0);
            }
        }
        i= 0;
    }

    return 0;
}

static int decode_subframe_fixed(GetBitContext *gb, int blocksize, int32_t *decoded, int pred_order, int bps)
{
    int a, b, c, d, i;
    int ret;

    /* warm up samples */
    for (i = 0; i < pred_order; i++) {
        decoded[i] = get_sbits(gb, bps);
    }

    if ((ret = decode_residuals(gb, blocksize, decoded, pred_order)) < 0)
        return ret;

    if (pred_order > 0)
        a = decoded[pred_order-1];
    if (pred_order > 1)
        b = a - decoded[pred_order-2];
    if (pred_order > 2)
        c = b - decoded[pred_order-2] + decoded[pred_order-3];
    if (pred_order > 3)
        d = c - decoded[pred_order-2] + 2*decoded[pred_order-3] - decoded[pred_order-4];

    switch (pred_order) {
    case 0:
        break;
    case 1:
        for (i = pred_order; i < blocksize; i++)
            decoded[i] = a += decoded[i];
        break;
    case 2:
        for (i = pred_order; i < blocksize; i++)
            decoded[i] = a += b += decoded[i];
        break;
    case 3:
        for (i = pred_order; i < blocksize; i++)
            decoded[i] = a += b += c += decoded[i];
        break;
    case 4:
        for (i = pred_order; i < blocksize; i++)
            decoded[i] = a += b += c += d += decoded[i];
        break;
    default:
        fprintf(stderr,"illegal pred order %d\n", pred_order);
        return -1;
    }

    return 0;
}

static int decode_subframe_lpc(GetBitContext *gb, int blocksize, int32_t *decoded, int pred_order, int bps)
{
    int i, ret;
    int coeff_prec, qlevel;
    int coeffs[32];

    /* warm up samples */
    for (i = 0; i < pred_order; i++) {
        decoded[i] = get_sbits(gb, bps);
    }

    coeff_prec = get_bits(gb, 4) + 1;
    if (coeff_prec == 16) {
        fprintf(stderr,"invalid coeff precision\n");
        return -1;
    }
    qlevel = get_sbits(gb, 5);
    if (qlevel < 0) {
        fprintf(stderr,"qlevel %d not supported, maybe buggy stream\n",
               qlevel);
        return -1;
    }

    for (i = 0; i < pred_order; i++) {
        coeffs[pred_order - i - 1] = get_sbits(gb, coeff_prec);
    }

    if ((ret = decode_residuals(gb, blocksize, decoded, pred_order)) < 0)
        return ret;

    flac_lpc_16_c(decoded, coeffs, pred_order, qlevel, blocksize);

    return 0;
}

int decode_subframe(GetBitContext *gb, FLACFrameInfo *fi, int32_t *decoded, int channel)
{
    int type, wasted = 0;
    int bps = fi->bps;
    int i, tmp, ret;

    if (channel == 0) {
        if (fi->ch_mode == FLAC_CHMODE_RIGHT_SIDE)
            bps++;
    } else {
        if (fi->ch_mode == FLAC_CHMODE_LEFT_SIDE || fi->ch_mode == FLAC_CHMODE_MID_SIDE)
            bps++;
    }

    if (get_bits1(gb)) {
        fprintf(stderr,"invalid subframe padding\n");
        return -1;
    }
    type = get_bits(gb, 6);

    if (get_bits1(gb)) {
        wasted = 1 + get_unary1(gb, bps);
        bps -= wasted;
    }
    if (bps > 32) {
        fprintf(stderr, "error: bps > 32 not supported\n");
        return -1;
    }

//FIXME use av_log2 for types
    if (type == 0) {
        tmp = get_sbits(gb, bps);
        for (i = 0; i < fi->blocksize; i++)
            decoded[i] = tmp;
    } else if (type == 1) {
        for (i = 0; i < fi->blocksize; i++)
            decoded[i] = get_sbits(gb, bps);
    } else if ((type >= 8) && (type <= 12)) {
        if ((ret = decode_subframe_fixed(gb, fi->blocksize, decoded, type & ~0x8, bps)) < 0)
            return ret;
    } else if (type >= 32) {
        if ((ret = decode_subframe_lpc(gb, fi->blocksize, decoded, (type & ~0x20)+1, bps)) < 0)
            return ret;
    } else {
        fprintf(stderr,"invalid coding type\n");
        return -1;
    }

    if (wasted) {
        int i;
        for (i = 0; i < fi->blocksize; i++)
            decoded[i] <<= wasted;
    }

    return 0;
}
