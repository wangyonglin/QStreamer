#ifndef AVFRAMEQUEUE_H
#define AVFRAMEQUEUE_H

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

class AVFrameQueue
{
public:
    AVFrameQueue();
    ~AVFrameQueue();
    void Abort();
    int Push(AVFrame * frame);
    AVFrame *Pop(const int timeout);
    AVFrame *Front();
    int Size();

private:
    void Release();
    BaseQueue<AVFrame*> queue;
};

#endif // AVFRAMEQUEUE_H
