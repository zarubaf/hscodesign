#ifndef FLAC_DEC_H
#define FLAC_DEC_H

#include "utils.h"
#include "get_bits.h"

#define FLAC_MAX_CHANNELS 8

enum {
FLAC_CHMODE_INDEPENDENT = 0,
FLAC_CHMODE_LEFT_SIDE = 1,
FLAC_CHMODE_RIGHT_SIDE = 2,
FLAC_CHMODE_MID_SIDE = 3,
};

#define FLACCOMMONINFO \
    int samplerate; /**< sample rate */\
    int channels; /**< number of channels */\
    int bps; /**< bits-per-sample */\

typedef struct FLACFrameInfo {
    FLACCOMMONINFO
    int blocksize; /**< block size of the frame */
    int ch_mode; /**< channel decorrelation mode */
    int64_t frame_or_sample_num; /**< frame number or sample number */
    int is_var_size; /**< specifies if the stream uses variable block sizes or a fixed block size; also determines the meaning of frame_or_sample_num */
} FLACFrameInfo;

int ff_flac_decode_frame_header(GetBitContext *gb, FLACFrameInfo *fi);
int decode_subframe(GetBitContext *gb, FLACFrameInfo *fi, int32_t *decoded, int channel);

#endif // FLAC_DEC_H
