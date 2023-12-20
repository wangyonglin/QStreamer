#ifndef YOLOTHREAD_H
#define YOLOTHREAD_H
#include <YoloRunnable.h>
#include <YoloFFmpeg.h>
#include <YoloQueue.h>
namespace YoloThread {

    class DexmuxThread :public YoloRunnable,public YoloFFmpeg::DexmuxFFmpeg {
    public:
        DexmuxThread(YoloQueue::PacketQueue* audioPacketQueue,YoloQueue::PacketQueue* videoPacketQueue);
        ~DexmuxThread();
        void freeThread();
        void startThread(const std::string &url);
        void stopThread();
        virtual void Runnable() override;
    private:
        QString urlValue;
        char err2str[256]={0};

      YoloQueue::PacketQueue* __audioPacketQueue=nullptr;
      YoloQueue::PacketQueue* __videoPacketQueue=nullptr;
    };
    class DecodecThread :public YoloRunnable ,public YoloFFmpeg::DecodecFFmpeg{
    public:
        DecodecThread(YoloQueue::PacketQueue* packetQueue,YoloQueue::FrameQueue * frameQueue);
        ~DecodecThread();
        void startThread(AVCodecParameters *codecParameters);
        void stopThread();
        virtual void Runnable() override;
    private:
        YoloQueue::PacketQueue* __packetQueue=nullptr;
        YoloQueue::FrameQueue * __frameQueue=nullptr;
     char err2str[256]={0};
        AVRational __timeBase;
    };
}


#endif // YOLOTHREAD_H
