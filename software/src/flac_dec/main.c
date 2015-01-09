#include <alt_types.h>
//#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "sys/alt_flash.h"
#include <io.h>
#include <system.h>
#include <malloc.h>
#include <ctype.h>
#include "flac_dec.h"
#include "flac_dsp.h"
#include "utils.h"
#include "fft.h"
#include "display.h"

#define SPECTRO_LOG
#include "spectrum.h"

#include <sys/alt_alarm.h>

//#define printf printf
#define fprintf(shit, fmt, ...) printf(fmt, ##__VA_ARGS__)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../adc128s022.h"

#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_regs.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_avalon_performance_counter.h"

#define READ_BLOCK 17

#define SD_CARD_STATUS      0x0234
#define SD_CARD_ARGUMENT    0x022C
#define SD_CARD_COMMAND     0x0230

typedef struct {
    int32_t *freq;
    int16_t *data_l, *data_r;
    int pos;
} audio_buf_t;

#define AUDIO_BUF_CNT 4
#define AUDIO_BUF_MSK 3
#define AUDIO_BUF_SMP 2940
static audio_buf_t audio_buf[AUDIO_BUF_CNT];
static volatile int audio_buf_read, audio_buf_write;

static uint32_t sdcard_buf[512 / 4];

alt_up_sd_card_dev *sd_card_dev;
static int *sdcard_com_arg_reg;
static short int *sdcard_com_reg;
static short int *sdcard_aux_stat_reg;

alt_up_audio_dev *audio_dev;

volatile uint32_t *display_base;

static int decode_main();

static void read_block(GetBitContext *gb)
{
    int status, i;

    while ((status = IORD_16DIRECT(sdcard_aux_stat_reg, 0)) & 0x04)
        if ((status & 0x02) == 0) {
            printf("sd card was removed\n");
            exit(1);
        }

    // swap bytes
    for (i = 0; i < 128; i++) {

        gb->buf[i] =  (uint32_t) IORD_32DIRECT(sd_card_dev->base, 4*i); //
        //gb->buf[i] = ((int32_t *)sd_card_dev->base)[i];
        //printf("%08x\t",gb->buf[i]);
        gb->buf[i] = (gb->buf[i]<<24) | (gb->buf[i]<<8&0x00FF0000) | (gb->buf[i]>>8&0x0000FF00) | (gb->buf[i]>>24);
    }
	//printf("\n");
    //*sdcard_com_arg_reg = ++gb->blk_cnt * 512;
    IOWR_32DIRECT(sdcard_com_arg_reg, 0,  ++gb->blk_cnt* 512);
    IOWR_16DIRECT(sdcard_com_reg, 0, READ_BLOCK);
    //*sdcard_com_reg = READ_BLOCK;

    gb->pos -= 4096;
}

static void audio_write_isr(void *ctx, alt_u32 id)
{
    register int32_t left, right;
    register unsigned int fifo_space;
    audio_buf_t *act_buf;

    while (1) {
        act_buf = &audio_buf[audio_buf_read & AUDIO_BUF_MSK];

        while (act_buf->pos < AUDIO_BUF_SMP) {
            fifo_space = IORD_ALT_UP_AUDIO_FIFOSPACE(audio_dev->base);
            fifo_space &= ALT_UP_AUDIO_FIFOSPACE_WSLC_MSK | ALT_UP_AUDIO_FIFOSPACE_WSRC_MSK;

            if (fifo_space) {
                left = act_buf->data_l[act_buf->pos];
                right = act_buf->data_r[act_buf->pos];
                IOWR_ALT_UP_AUDIO_LEFTDATA(audio_dev->base, left);
                IOWR_ALT_UP_AUDIO_RIGHTDATA(audio_dev->base, right);
                act_buf->pos++;
            } else
                return;
        }

        if (audio_buf_read + 1 == audio_buf_write)
            break;

        audio_buf_read++;
        shift_display_left(&display_base);
        draw_fft(display_base, audio_buf[audio_buf_read & AUDIO_BUF_MSK].freq);
    }

    alt_up_audio_disable_write_interrupt(audio_dev);
    alt_printf("warning: audio buffer critically low\n");
}

int main()
{
    sd_card_dev = alt_up_sd_card_open_dev(SDCARD_NAME);

    sdcard_com_arg_reg = ((int *)(sd_card_dev->base + SD_CARD_ARGUMENT));
    sdcard_com_reg = ((short int *)(sd_card_dev->base + SD_CARD_COMMAND));
    sdcard_aux_stat_reg = ((short int *)(sd_card_dev->base + SD_CARD_STATUS));

    audio_dev = alt_up_audio_open_dev(AUDIO_NAME);

    if (audio_dev == NULL) {
        printf("audio dev could not be opened\n");
        return 1;
    }

    display_base = display_init();
    clear(display_base, 0);

    printf("waiting for sd card\n");
    while ((IORD_16DIRECT(sdcard_aux_stat_reg, 0) & 0x02) == 0);
    printf("sd card connected\n");

    alt_up_audio_disable_write_interrupt(audio_dev);
    alt_irq_register(AUDIO_IRQ, NULL, audio_write_isr);

    PERF_RESET (PERFORMANCE_COUNTER_BASE);

    IOWR_32DIRECT(sdcard_com_arg_reg, 0, 0);
    IOWR_16DIRECT(sdcard_com_reg, 0, READ_BLOCK);

    PERF_START_MEASURING (PERFORMANCE_COUNTER_BASE);
    decode_main();
    PERF_STOP_MEASURING (PERFORMANCE_COUNTER_BASE);

    perf_print_formatted_report(PERFORMANCE_COUNTER_BASE, 140000000, 4, "idle", "permute", "fft", "spectrum");

    return 0;
}

/*
static void scale_frequencies(fft_complex *freq_l, fft_complex *freq_r, int32_t *out)
{
    fft_complex *zl, *zr;
    int a, i, h;

    for (h = 0; h < DISPLAY_HEIGHT; h++) {
        i = h;

        zl = &freq_l[i];
        //zr = &freq_r[i];
        a = fast_sqrt(64 * fast_sqrt((int32_t)zl->re * (int32_t)zl->re + (int32_t)zl->im * (int32_t)zl->im));
        //a += fast_sqrt(4 * fast_sqrt((int32_t)zr->re * (int32_t)zr->re + (int32_t)zr->im * (int32_t)zr->im));

        if (a > 255)
            a = 255;

        out[h] = a;
    }
}
*/

static int decode_main()
{
    uint32_t buf;
    int len, chmod, i;

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

    audio_buf_read = 0;
    audio_buf_write = 0;

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

                PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 2);
                fft_4096_permute(prev_buf->data_l, act_buf->data_l, prev_buf->data_r, act_buf->data_r, AUDIO_BUF_SMP, fft_buf);
                PERF_END (PERFORMANCE_COUNTER_BASE, 2);

                PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 3);
                fft_4096(fft_buf);
                PERF_END (PERFORMANCE_COUNTER_BASE, 3);

                PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 4);
                get_spectrum(fft_buf, act_buf->freq);
                PERF_END (PERFORMANCE_COUNTER_BASE, 4);

                PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 1);
                while (audio_buf_write + 1 == audio_buf_read + AUDIO_BUF_CNT);
                PERF_END (PERFORMANCE_COUNTER_BASE, 1);

                if (audio_buf_write++)
                    alt_up_audio_enable_write_interrupt(audio_dev);

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

        PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 2);
        fft_4096_permute(prev_buf->data_l, act_buf->data_l, prev_buf->data_r, act_buf->data_r, AUDIO_BUF_SMP, fft_buf);
        PERF_END (PERFORMANCE_COUNTER_BASE, 2);

        PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 3);
        fft_4096(fft_buf);
        PERF_END (PERFORMANCE_COUNTER_BASE, 3);

        PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 4);
        get_spectrum(fft_buf, act_buf->freq);
        PERF_END (PERFORMANCE_COUNTER_BASE, 4);

        PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 1);
        while (audio_buf_write + 1 == audio_buf_read + AUDIO_BUF_CNT);
        PERF_END (PERFORMANCE_COUNTER_BASE, 1);

        audio_buf_write++;
        alt_up_audio_enable_write_interrupt(audio_dev);
    }

    return 0;
}
