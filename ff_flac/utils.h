#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>

#define av_log(par1, par2, format, ...) fprintf(stderr, format, ##__VA_ARGS__)

#define GET_UTF8(val, GET_BYTE, ERROR)\
    val= GET_BYTE;\
    {\
        int ones= 7 - av_log2(val ^ 255);\
        if(ones==1)\
            ERROR\
        val&= 127>>ones;\
        while(--ones > 0){\
            int tmp= GET_BYTE - 128;\
            if(tmp>>6)\
                ERROR\
            val= (val<<6) + tmp;\
        }\
    }

typedef struct {
    FILE *file;
    uint8_t *buf;
    int pos;
} GetBitContext;

unsigned int get_bits(GetBitContext *gb, int n);
int get_sbits(GetBitContext *gb, int n);
unsigned int get_bits1(GetBitContext *gb);
int get_sbits_long(GetBitContext *gb, int n);
int64_t get_utf8(GetBitContext *gb);
unsigned int align_get_bits(GetBitContext *gb);
void skip_bits(GetBitContext *gb, int n);
int get_unary(GetBitContext *gb, int stop, int len);

int av_log2(unsigned int v);

#endif // UTILS_H
