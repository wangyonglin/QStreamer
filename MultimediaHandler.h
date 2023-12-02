#ifndef RUNNABLEMANAGER_H
#define RUNNABLEMANAGER_H
#include <OpenGLWidget.h>
#include <DexmuxThread.h>
#include <DecodecThread.h>
#include <VideoHandler.h>
#include <AudioHandler.h>

#include <DexmuxRunnable.h>
#include <DecodecRunnable.h>

class MultimediaHandler
{
public:
    MultimediaHandler();
    int Init(const char *url);
    int width();
    int height();
    int Start();
    int Stop();

private:
    int __video_width=0;
    int __video_height=0;
public:
    Synchronized avsync;
    OpenGLWidget * openglWidget = nullptr;
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
    DexmuxRunnable * __dexmux_runnable=nullptr;
    DecodecRunnable * __audio_decodec_runnable =nullptr;
     DecodecRunnable * __video_decodec_runnable =nullptr;
};

#endif // RUNNABLEMANAGER_H
