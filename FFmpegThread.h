#ifndef YOLOTHREAD_H
#define YOLOTHREAD_H

#include <FFmpegBase.h>
#include <FFmpegPublic.h>
namespace FFmpegThread {

    class DexmuxThread :public FFmpegPublic::Thread::Runnable,public FFmpegBase::DexmuxFFmpeg {
    public:
        DexmuxThread(FFmpegPublic::Queue::Packet* audioPacketQueue,FFmpegPublic::Queue::Packet* videoPacketQueue);
        ~DexmuxThread();
        void freeThread();
        void startThread(const std::string &url);
        void stopThread();
        virtual void entityRunnable() override;
    private:
        QString urlValue;
        char err2str[256]={0};

      FFmpegPublic::Queue::Packet* __audioPacketQueue=nullptr;
      FFmpegPublic::Queue::Packet* __videoPacketQueue=nullptr;
    };
    class DecodecThread :public FFmpegPublic::Thread::Runnable ,public FFmpegBase::DecodecFFmpeg{
    public:
        DecodecThread(FFmpegPublic::Queue::Packet* packetQueue,FFmpegPublic::Queue::Frame * frameQueue);
        ~DecodecThread();
        void startThread(AVCodecParameters *codecParameters);
        void stopThread();
        virtual void entityRunnable() override;
    private:
        FFmpegPublic::Queue::Packet* __packetQueue=nullptr;
        FFmpegPublic::Queue::Frame * __frameQueue=nullptr;
     char err2str[256]={0};
        AVRational __timeBase;
    };
}


#endif // YOLOTHREAD_H
