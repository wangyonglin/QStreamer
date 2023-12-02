#ifndef DEXMUXUTILS_H
#define DEXMUXUTILS_H

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
typedef  enum{
    DEXMUX_MIDEA_VIDEO=AVMEDIA_TYPE_VIDEO,
    DEXMUX_MIDEA_AUDIO=AVMEDIA_TYPE_AUDIO
}DexmuxMidea;
class DexmuxBase
{

public:
    DexmuxBase();
    ~DexmuxBase();
    int initDexmux(const std::string &url);
    void deInitDexmux();
    int getVideoWidth();
    int getVideoHeight();
    AVCodecParameters * getCodecParameters(DexmuxMidea midea);
    AVRational getTimebase(DexmuxMidea midea);
    int getStreamIndex(DexmuxMidea midea);
    int readFrame(AVPacket *packet);

private:
    char err2str[256]={0};
    AVFormatContext * __fmt_ctx=nullptr;
    int __audio_stream_idx=-1;
    int __video_stream_idx=-1;
};

#endif // DEXMUXUTILS_H
