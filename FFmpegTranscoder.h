#ifndef FFMPEGTRANSCODER_H
#define FFMPEGTRANSCODER_H
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
#include <libavutil/avassert.h>
#include <SDL.h>
#include <SDL_audio.h>
}
#include <QImage>
#include <QDebug>
namespace  FFmpegTranscoder{

    class VideoTranscoder
    {
    public:

        VideoTranscoder(int dst_width, int dst_height, AVPixelFormat dst_format);
        ~VideoTranscoder();

       int convertFrame(uint8_t **dst_data, AVFrame *frame);
        uint8_t * dst_data();

    private:
         int __initVideo(struct SwsContext **img_convert_ctx,AVFrame * frame);
         void __freeVideo(struct SwsContext *img_convert_ctx);
    int __dst_width;
    int __dst_height;
    AVPixelFormat __dst_format;
    uint8_t  *__dst_data[4];
    int  __dst_linesize[4];
    SwsContext *__img_convert_ctx=nullptr;
    };
    class AudioTranscoder{
    public:
        AudioTranscoder(int dst_rate,
                        AVChannelLayout dst_ch_layout,
                        enum AVSampleFormat dst_sample_fmt);
        ~AudioTranscoder();
        int convert(AVFrame **dst_frame, AVFrame *frame);

    private :
        int initResampler(struct SwrContext **au_convert_ctx,AVFrame * frame);
        void exitResampler(struct SwrContext *au_convert_ctx);

        SwrContext *__au_convert_ctx = nullptr;
        int __dst_rate;
        AVChannelLayout __dst_ch_layout;
        enum AVSampleFormat __dst_sample_fmt;
    };
}


#endif // FFMPEGTRANSCODER_H
