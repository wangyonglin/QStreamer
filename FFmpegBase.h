#ifndef FFmpegBase_H
#define FFmpegBase_H
extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include <libswresample/swresample.h>
#include <SDL.h>
#include <SDL_audio.h>
}
#include <string>
#include <QDebug>
namespace  FFmpegBase{


    class DexmuxFFmpeg
    {

    public:
        DexmuxFFmpeg();
        ~DexmuxFFmpeg();
        int initDexmuxFFmpeg(const std::string &url);
        void freeDexmuxFFmpeg();
        int getWidth();
        int getHeight();
        int getTotalTime();
        int getFrameRate();
        int receiveFrame(AVPacket *packet);
        AVFormatContext * getFormatContext();

        AVRational getAudioTimebase();
        AVRational getVideoTimebase();

        int getAudioStreamIndex();
        int getVideoStreamIndex();
         AVCodecParameters * getAudioCodecParameters();
         AVCodecParameters * getVideoCodecParameters();

    private:
        char err2str[256]={0};
        AVFormatContext * fmt_ctx=nullptr;
        int __audio_stream_idx=-1;
        int __video_stream_idx=-1;
    };
    class DecodecFFmpeg
    {
    public:
        DecodecFFmpeg();
        ~DecodecFFmpeg();
        int initDecodecFFmpeg(AVCodecParameters *codecParameters);
        void freeDecodecFFmpeg();
        int sendPacket(AVPacket *packet);
        int receiveFrame(AVFrame *frame);

        AVCodecContext * getCodeContext();
        AVRational getTimebase();
    private:
         char err2str[256]={0};
         AVCodecContext * codec_ctx = nullptr;
         AVRational timeBase;
    };


}

namespace FFmpegBase {
namespace Funcation {
    int64_t getTime();
}
}


#endif // FFmpegBase_H
