#ifndef RUNNABLEMANAGER_H
#define RUNNABLEMANAGER_H
#include <Canvas.h>
#include <DexmuxThread.h>
#include <DecodecThread.h>
#include <VideoHandler.h>
#include <AudioHandler.h>

class Runnable{


};

class MultimediaHandler :public Runnable
{
public:
    MultimediaHandler();
    int Init();
    int Start();
    int Stop();

public:
    Synchronized avsync;

    Canvas * canvas = nullptr;
    DexmuxThread *dexmuxThread = nullptr;
    DecodecThread *audio_decodec_thread= nullptr;
    DecodecThread *video_decodec_thread= nullptr;
    VideoHandler * video_handler= nullptr;
    AudioHandler* audio_handler =nullptr;
private:
    AVPacketQueue *audio_packet_queue  =  nullptr;
    AVPacketQueue* video_packet_queue=  nullptr;
    AVFrameQueue * audio_frame_queue= nullptr;
    AVFrameQueue * video_frame_queue= nullptr;
};

#endif // RUNNABLEMANAGER_H
