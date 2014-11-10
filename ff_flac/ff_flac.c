#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "flac_dec.h"
#include "flac_dsp.h"

static int decode_frame(GetBitContext *gb, FILE *out);

int main(int argc, char *argv[])
{
    uint8_t buf[4];
    FILE *file, *out;
    void *ptr;
    int len;

    if (argc < 3) {
        fprintf(stderr, "usage: %s <input> <output>\n", argv[0]);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "error: failed to open `%s': %s", argv[1], strerror(errno));
        exit(1);
    }

    if (fread(buf, 4, 1, file) != 1) {
        fprintf(stderr, "error: reading stream header failed\n");
        exit(1);
    }

    if (strncmp((char *)buf, "fLaC", 4)) {
        fprintf(stderr, "error: wrong file type\n");
        exit(1);
    }

    ptr = NULL;

    do {
        if (fread(buf, 4, 1, file) != 1) {
            fprintf(stderr, "error: reading metadata block header failed\n");
            exit(1);
        }

        len = buf[3] + (buf[2] << 8) + (buf[1] << 16);
        printf("reading metadata block of size %d%s\n", len, (buf[0] & 0x80) ? ", last block" : "");

        ptr = realloc(ptr, len);

        if (fread(ptr, len, 1, file) != 1) {
            fprintf(stderr, "error: reading metadata block failed\n");
            exit(1);
        }
    }
    while (!(buf[0] & 0x80));

    GetBitContext gb;
    gb.file = file;
    gb.buf = malloc(512);
    gb.pos = 4096;

    if ((out = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "error: failed to open `%s': %s", argv[2], strerror(errno));
        exit(1);
    }

    while (decode_frame(&gb, out) == 0);

    fclose(out);
    fclose(file);
    return 0;
}

static int decode_frame(GetBitContext *gb, FILE *out)
{
    int i, ret, sample_shift = 0; // sample_shift: shift required to make samples 16-bit wide
    FLACFrameInfo fi;

    if ((ret = ff_flac_decode_frame_header(gb, &fi)) < 0) {
        av_log(s->avctx, AV_LOG_ERROR, "invalid frame header\n");
        return ret;
    }

    /*
    if (s->channels && fi.channels != s->channels && s->got_streaminfo) {
        s->channels = s->avctx->channels = fi.channels;
        ff_flac_set_channel_layout(s->avctx);
        ret = allocate_buffers(s);
        if (ret < 0)
            return ret;
    }
    s->channels = s->avctx->channels = fi.channels;
    if (!s->avctx->channel_layout)
        ff_flac_set_channel_layout(s->avctx);
    s->ch_mode = fi.ch_mode;

    if (!s->bps && !fi.bps) {
        av_log(s->avctx, AV_LOG_ERROR, "bps not found in STREAMINFO or frame header\n");
        return AVERROR_INVALIDDATA;
    }
    if (!fi.bps) {
        fi.bps = s->bps;
    } else if (s->bps && fi.bps != s->bps) {
        av_log(s->avctx, AV_LOG_ERROR, "switching bps mid-stream is not "
                                       "supported\n");
        return AVERROR_INVALIDDATA;
    }

    if (!s->bps) {
        s->bps = s->avctx->bits_per_raw_sample = fi.bps;
        flac_set_bps(s);
    }

    if (!s->max_blocksize)
        s->max_blocksize = FLAC_MAX_BLOCKSIZE;
    if (fi.blocksize > s->max_blocksize) {
        av_log(s->avctx, AV_LOG_ERROR, "blocksize %d > %d\n", fi.blocksize,
               s->max_blocksize);
        return AVERROR_INVALIDDATA;
    }
    s->blocksize = fi.blocksize;

    if (!s->samplerate && !fi.samplerate) {
        av_log(s->avctx, AV_LOG_ERROR, "sample rate not found in STREAMINFO"
                                        " or frame header\n");
        return AVERROR_INVALIDDATA;
    }
    if (fi.samplerate == 0)
        fi.samplerate = s->samplerate;
    s->samplerate = s->avctx->sample_rate = fi.samplerate;

    if (!s->got_streaminfo) {
        ret = allocate_buffers(s);
        if (ret < 0)
            return ret;
        ff_flacdsp_init(&s->dsp, s->avctx->sample_fmt, s->bps);
        s->got_streaminfo = 1;
        dump_headers(s->avctx, (FLACStreaminfo *)s);
    }
    */

//    dump_headers(s->avctx, (FLACStreaminfo *)s);

    int32_t **decoded = malloc(fi.channels * sizeof(void *));

    /* subframes */
    for (i = 0; i < fi.channels; i++) {
        decoded[i] = malloc(fi.blocksize * 4);

        if ((ret = decode_subframe(gb, &fi, decoded[i], i)) < 0)
            return ret;
    }

    align_get_bits(gb);

    /* frame footer */
    skip_bits(gb, 16); /* data crc */

    int16_t **out_data = malloc(fi.channels * sizeof(void *));

    for (i = 0; i < fi.channels; i++)
        out_data[i] = malloc(fi.blocksize * 2);

    switch (fi.ch_mode) {
        case FLAC_CHMODE_INDEPENDENT: flac_decorrelate_indep_c(out_data, decoded, fi.channels, fi.blocksize, sample_shift); break;
        case FLAC_CHMODE_LEFT_SIDE:   flac_decorrelate_ls_c(out_data, decoded, fi.channels, fi.blocksize, sample_shift); break;
        case FLAC_CHMODE_RIGHT_SIDE:  flac_decorrelate_rs_c(out_data, decoded, fi.channels, fi.blocksize, sample_shift); break;
        case FLAC_CHMODE_MID_SIDE:    flac_decorrelate_ms_c(out_data, decoded, fi.channels, fi.blocksize, sample_shift); break;
    }

    for (i = 0; i < fi.blocksize; i++) {
        fwrite(&out_data[0][i], 2, 1, out);
        fwrite(&out_data[1][i], 2, 1, out);
    }

    for (i = 0; i < fi.channels; i++) {
        free(decoded[i]);
        free(out_data[i]);
    }

    free(decoded);
    free(out_data);
    return 0;
}
