#ifndef TRANSCODEUTILS_H
#define TRANSCODEUTILS_H
extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
#include <SDL.h>
#include <SDL_audio.h>
}
#include <QImage>
#include <QDebug>

class TranscodeUtils
{
public:

    TranscodeUtils(int dst_width, int dst_height, AVPixelFormat dst_format);
    ~TranscodeUtils();

   int transcoding(uint8_t **dst_data, AVFrame *frame);
    uint8_t * dst_data();

private:
     int __init(struct SwsContext **img_convert_ctx,AVFrame * frame);
     void __free(struct SwsContext *img_convert_ctx);
int __dst_width;
int __dst_height;
AVPixelFormat __dst_format;
uint8_t  *__dst_data[4];
int  __dst_linesize[4];
SwsContext *__img_convert_ctx=nullptr;
};

#endif // TRANSCODEUTILS_H
