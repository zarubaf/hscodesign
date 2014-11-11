#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "get_bits.h"
#include "flac_dec.h"
#include "flac_dsp.h"

static int32_t **decoded;
static int16_t **out_data;
static int last_blocksize = 0,
           last_channels = 0;

static int decode_frame(GetBitContext *gb, FILE *out);

int main(int argc, char *argv[])
{
    uint32_t buf;
    FILE *file, *out;
    int len, i;

    if (argc < 3) {
        fprintf(stderr, "usage: %s <input> <output>\n", argv[0]);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "error: failed to open `%s': %s", argv[1], strerror(errno));
        exit(1);
    }

    GetBitContext gb;
    gb.file = file;
    gb.buf = malloc(512);
    gb.pos = 4096;

    buf = get_bits(&gb, 32);

    if (strncmp((char *)&buf, "CaLf", 4)) {
        fprintf(stderr, "error: wrong file type\n");
        exit(1);
    }

    do {
        buf = get_bits1(&gb);
        skip_bits(&gb, 7);

        len = get_bits(&gb, 24);
        printf("reading metadata block of size %d%s\n", len, buf ? ", last block" : "");

        skip_bits(&gb, len * 8);
    }
    while (!buf);

    if ((out = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "error: failed to open `%s': %s", argv[2], strerror(errno));
        exit(1);
    }

    decoded = malloc(8 * sizeof(void *));
    out_data = malloc(8 * sizeof(void *));

    for (i = 0; i < 8; i++) {
        decoded[i] = NULL;
        out_data[i] = NULL;
    }

    while (decode_frame(&gb, out) == 0);

    for (i = 0; i < last_channels; i++) {
        free(decoded[i]);
        free(out_data[i]);
    }

    free(decoded);
    free(out_data);

    fclose(out);
    fclose(file);
    return 0;
}

static int decode_frame(GetBitContext *gb, FILE *out)
{
    int i, ret;
    FLACFrameInfo fi;

    if ((ret = ff_flac_decode_frame_header(gb, &fi)) < 0) {
        fprintf(stderr, "invalid frame header\n");
        return ret;
    }

    if (fi.blocksize != last_blocksize)
        for (i = 0; i < fi.channels; i++) {
            decoded[i] = realloc(decoded[i], fi.blocksize * 4);
            out_data[i] = realloc(out_data[i], fi.blocksize * 2);
        }

    last_blocksize = fi.blocksize;
    last_channels = fi.channels;

    /* subframes */
    for (i = 0; i < fi.channels; i++)
        if ((ret = decode_subframe(gb, &fi, decoded[i], i)) < 0)
            return ret;

    align_get_bits(gb);

    /* frame footer */
    skip_bits(gb, 16); /* data crc */

    switch (fi.ch_mode) {
        case FLAC_CHMODE_INDEPENDENT: flac_decorrelate_indep_c(out_data, decoded, fi.channels, fi.blocksize); break;
        case FLAC_CHMODE_LEFT_SIDE:   flac_decorrelate_ls_c(out_data, decoded, fi.channels, fi.blocksize); break;
        case FLAC_CHMODE_RIGHT_SIDE:  flac_decorrelate_rs_c(out_data, decoded, fi.channels, fi.blocksize); break;
        case FLAC_CHMODE_MID_SIDE:    flac_decorrelate_ms_c(out_data, decoded, fi.channels, fi.blocksize); break;
    }

    for (i = 0; i < fi.blocksize; i++) {
        fwrite(&out_data[0][i], 2, 1, out);
        fwrite(&out_data[1][i], 2, 1, out);
    }

    return 0;
}
