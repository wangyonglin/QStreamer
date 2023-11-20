#ifndef DEXMUXTHREAD_H
#define DEXMUXTHREAD_H
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
#include <QDebug>
#include <BaseThread.h>
#include <AVPacketQueue.h>

class DexmuxThread : public BaseThread
{
public:
    DexmuxThread(AVPacketQueue *audio_packet_queue,AVPacketQueue *video_packet_queue);
    ~DexmuxThread();
    int Init(const char * _url);
    int Start();
    int Stop();
    void Run() override;
     AVCodecParameters * getAudioCodecParameters();
     AVCodecParameters * getVideoCodecParameters();
     AVRational AudioStreamTimebase();
        AVRational VideoStreamTimebase();
    std::string url;
    int video_stream_idx = -1, audio_stream_idx = -1;
private:
    char err2str[256]={0};
    AVPacketQueue *audio_packet_queue=nullptr;
    AVPacketQueue *video_packet_queue=nullptr;
    AVFormatContext * fmt_ctx=nullptr;

};

#endif // DEXMUXTHREAD_H
