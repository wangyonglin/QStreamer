#ifndef DECODECRUNNABLE_H
#define DECODECRUNNABLE_H

#include <BaseThread.h>
#include <AVFrameQueue.h>
#include <AVPacketQueue.h>
#include <DecodecBase.h>
#include <RunnableBase.h>
#include <QDebug>
class DecodecRunnable : public RunnableBase,public DecodecBase
{
public:
    DecodecRunnable(AVPacketQueue*packet_queue,AVFrameQueue * frame_queue);
    ~DecodecRunnable();
    int startDecodec();
    void stopDecodec();
     void Runnable();
private:
    char err2str[256]={0};
    AVCodecContext * codec_ctx = nullptr;
    AVPacketQueue * __packet_queue =nullptr;
    AVFrameQueue *__frame_queue =nullptr;
    AVFrame *frame = nullptr;
};

#endif // DECODECRUNNABLE_H
