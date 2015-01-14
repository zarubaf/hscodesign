#ifdef TEST

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../flac_dec.h"
#include "../flac_dsp.h"
#include "../utils.h"
#include "../fft.h"

#define DISPLAY_HEIGHT 480
#define SPECTRO_LOG
#include "../spectrum.h"

typedef struct {
    uint32_t *freq;
    int16_t *data_l, *data_r;
    int pos;
} audio_buf_t;

#define AUDIO_BUF_CNT 4
#define AUDIO_BUF_MSK 3
#define AUDIO_BUF_SMP 2940
static audio_buf_t audio_buf[AUDIO_BUF_CNT];
static volatile int audio_buf_read, audio_buf_write;

static FILE *input, *output, *display;
static uint32_t sdcard_buf[512 / 4];

static void read_block(GetBitContext *gb)
{
    int i;

    if (fread(gb->buf, 1, 512, input) != 512)
        fprintf(stderr, "warning: reading file block failed\n");

    // swap bytes
    for (i = 0; i < 128; i++)
        gb->buf[i] = (gb->buf[i]<<24) | (gb->buf[i]<<8&0x00FF0000) | (gb->buf[i]>>8&0x0000FF00) | (gb->buf[i]>>24);

    gb->pos -= 4096;
}

static void draw_fft(int32_t *freq)
{
    int i;

    for (i = 0; i < 480; i++) {
        fputc((freq[i] >> 16) & 0xff, display);
        fputc((freq[i] >> 8) & 0xff, display);
        fputc(freq[i] & 0xff, display);
    }
}

static void audio_write_isr()
{
    int32_t left, right;
    audio_buf_t *act_buf;

    while (1) {
        act_buf = &audio_buf[audio_buf_read & AUDIO_BUF_MSK];

        while (act_buf->pos < AUDIO_BUF_SMP) {
            left = act_buf->data_l[act_buf->pos];
            right = act_buf->data_r[act_buf->pos];
            fwrite(&left, 2, 1, output);
            fwrite(&right, 2, 1, output);
            act_buf->pos++;
        }

        if (audio_buf_read + 1 == audio_buf_write)
            break;

        audio_buf_read++;
        draw_fft(audio_buf[audio_buf_read & AUDIO_BUF_MSK].freq);
    }
}

int main(int argc, char *argv[])
{
    uint32_t buf;
    int len, chmod, i;

    if (argc < 4) {
        fprintf(stderr, "usage: %s <input>.flac <output>.pcm <display>.ppm\n", argv[0]);
        exit(1);
    }

    if ((input = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "error opening input file '%s': %s\n", argv[1], strerror(errno));
        exit(1);
    }

    if ((output = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "error opening input file '%s': %s\n", argv[2], strerror(errno));
        exit(1);
    }

    if ((display = fopen(argv[3], "w")) == NULL) {
        fprintf(stderr, "error opening input file '%s': %s\n", argv[3], strerror(errno));
        exit(1);
    }

    fprintf(display, "P6\n%d %d\n255\n", DISPLAY_HEIGHT, 2160);

    GetBitContext gb;
    gb.read_block = read_block;
    gb.blk_cnt = 0;
    gb.buf = sdcard_buf;
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

    for (i = 0; i < AUDIO_BUF_CNT; i++) {
        audio_buf[i].freq = malloc(DISPLAY_HEIGHT * 4);
        audio_buf[i].data_l = malloc(AUDIO_BUF_SMP * 4);
        audio_buf[i].data_r = malloc(AUDIO_BUF_SMP * 4);
        audio_buf[i].pos = 0;
    }

    int32_t *decoded_l, *decoded_r;
    decoded_l = malloc(4608 * 4);
    decoded_r = malloc(4608 * 4);

    fft_complex *fft_buf;
    fft_buf = malloc(4096 * sizeof(fft_complex));

    audio_buf_t *prev_buf, *act_buf = &audio_buf[0];

    while ((len = decode_frame(&gb, decoded_l, decoded_r, &chmod)) >= 0) {
        int pos = 0;
        //audio_buf_t *act_buf = &audio_buf[audio_buf_write & AUDIO_BUF_MSK];

        while (pos < len) {
            if (act_buf->pos == AUDIO_BUF_SMP) {
                act_buf->pos = 0;
                prev_buf = &audio_buf[(audio_buf_write - 1) & AUDIO_BUF_MSK];

                fft_4096_permute(prev_buf->data_l, act_buf->data_l, prev_buf->data_r, act_buf->data_r, AUDIO_BUF_SMP, fft_buf);
                fft_4096(fft_buf);

                get_spectrum(fft_buf, act_buf->freq);

                if (audio_buf_write++)
                    audio_write_isr();

                act_buf = &audio_buf[audio_buf_write & AUDIO_BUF_MSK];
                act_buf->pos = 0;
            }

            int cnt = len - pos;

            if (cnt > AUDIO_BUF_SMP - act_buf->pos)
                cnt = AUDIO_BUF_SMP - act_buf->pos;

            switch (chmod) {
                case FLAC_CHMODE_INDEPENDENT:
                    flac_decorrelate_indep_c(decoded_l + pos, decoded_r + pos, act_buf->data_l + act_buf->pos, act_buf->data_r + act_buf->pos, cnt);
                    break;
                case FLAC_CHMODE_LEFT_SIDE:
                    flac_decorrelate_ls_c(decoded_l + pos, decoded_r + pos, act_buf->data_l + act_buf->pos, act_buf->data_r + act_buf->pos, cnt);
                    break;
                case FLAC_CHMODE_RIGHT_SIDE:
                    flac_decorrelate_rs_c(decoded_l + pos, decoded_r + pos, act_buf->data_l + act_buf->pos, act_buf->data_r + act_buf->pos, cnt);
                    break;
                case FLAC_CHMODE_MID_SIDE:
                    flac_decorrelate_ms_c(decoded_l + pos, decoded_r + pos, act_buf->data_l + act_buf->pos, act_buf->data_r + act_buf->pos, cnt);
                    break;
            }

            act_buf->pos += cnt;
            pos += cnt;
        }
    }

    if (act_buf->pos) {
        for (; act_buf->pos < AUDIO_BUF_SMP; act_buf->pos++) {
            act_buf->data_l[act_buf->pos] = 0;
            act_buf->data_r[act_buf->pos] = 0;
        }

        act_buf->pos = 0;

        fft_4096_permute(prev_buf->data_l, act_buf->data_l, prev_buf->data_r, act_buf->data_r, AUDIO_BUF_SMP, fft_buf);
        fft_4096(fft_buf);

        get_spectrum(fft_buf, act_buf->freq);

        audio_buf_write++;
        audio_write_isr();
    }

    fclose(input);
    fclose(output);
    fclose(display);

    return 0;
}

#endif // TEST
