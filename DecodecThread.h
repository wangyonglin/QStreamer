#ifndef DECODECTHREAD_H
#define DECODECTHREAD_H
#include <BaseThread.h>
#include <AVFrameQueue.h>
#include <AVPacketQueue.h>
class DecodecThread : public BaseThread
{
public:
    DecodecThread(AVPacketQueue*packet_queue,AVFrameQueue * frmae_queue);
    ~DecodecThread();
    int Init(AVCodecParameters * codec_param);
    int Start();
    int Stop();
    void Run();
private:
    char err2str[256]={0};
    AVCodecContext * codec_ctx = nullptr;
    AVPacketQueue * packet_queue =nullptr;
    AVFrameQueue *frame_queue =nullptr;
    AVFrame *frame = nullptr;

   // AVCodecContext *video_dec_ctx = NULL, *audio_dec_ctx;
};

#endif // DECODECTHREAD_H
