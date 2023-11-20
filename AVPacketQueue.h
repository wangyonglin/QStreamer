#ifndef AVPACKETQUEUE_H
#define AVPACKETQUEUE_H
#include <BaseQueue.h>

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

class AVPacketQueue
{
public:
    AVPacketQueue();
    ~AVPacketQueue();
    void Abort();
    void Release();
    int Size();
    int Push(AVPacket *pak);
    AVPacket *Pop(const int timeout);
private:
    BaseQueue<AVPacket *> queue;
};

#endif // AVPACKETQUEUE_H
