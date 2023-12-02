#ifndef DEXMUXRUNNABLE_H
#define DEXMUXRUNNABLE_H

#include <BaseThread.h>
#include <AVFrameQueue.h>
#include <AVPacketQueue.h>
#include <DexmuxBase.h>
#include <string>
#include <QDebug>
class DexmuxRunnable: public BaseThread,public DexmuxBase
{
public:
    DexmuxRunnable(AVPacketQueue *audio_packet_queue,AVPacketQueue *video_packet_queue);
    ~DexmuxRunnable();
    int startRunnable();
    int stop();
    virtual void Run() override;
private:
    QString __url;
    char err2str[256]={0};
    AVPacketQueue *__audio_packet_queue=nullptr;
    AVPacketQueue *__video_packet_queue=nullptr;
   // AVFormatContext * __fmt_ctx=nullptr;
};

#endif // DEXMUXRUNNABLE_H
