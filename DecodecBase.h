#ifndef DECODECBASE_H
#define DECODECBASE_H

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
#include <string>

class DecodecBase
{
public:
    DecodecBase();
    int initDecodec(AVCodecParameters *codec_param);
    void deInitDecodec();
    int SendPacket(AVPacket * packet);
    int ReceiveFrame(AVFrame * frame);
private:
     char err2str[256]={0};
     AVCodecContext * __codec_ctx = nullptr;
};

#endif // DECODECBASE_H
