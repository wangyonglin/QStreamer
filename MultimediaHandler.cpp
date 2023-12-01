#include "MultimediaHandler.h"

MultimediaHandler::MultimediaHandler()
{

}

int MultimediaHandler::Init(const char *url)
{
    int ret = -1;
     avsync.InitClock();


    audio_packet_queue  =  new AVPacketQueue();
    video_packet_queue=  new AVPacketQueue();
    audio_frame_queue= new AVFrameQueue();
    video_frame_queue= new AVFrameQueue();

    // 1.解复用
    dexmuxThread= new DexmuxThread(audio_packet_queue,video_packet_queue);
    ret=dexmuxThread->Init(url);
    if(ret<0){
        qDebug("dexmuxThread.init(...) failed");
        return -1;
    }else{
        __video_width= dexmuxThread->video_width();
        __video_height=dexmuxThread->video_height();
    }

    // 2.解码线程初始化
    audio_decodec_thread =  new DecodecThread(audio_packet_queue,audio_frame_queue);

    ret= audio_decodec_thread->Init(dexmuxThread->getAudioCodecParameters());
    if(ret<0){
        qDebug("DecodecThread.init(...) failed");
        return -1;
    }

    video_decodec_thread =  new DecodecThread(video_packet_queue,video_frame_queue);
    ret= video_decodec_thread->Init(dexmuxThread->getVideoCodecParameters());
    if(ret<0){
        qDebug("DecodecThread.init(...) failed");
        return -1;
    }

    video_handler=new  VideoHandler();
    ret= video_handler->Init(&avsync, dexmuxThread->VideoStreamTimebase(),video_frame_queue);

    if(ret<0){
        qDebug("VideoHandler.init(...) failed");
        return -1;
    }


    audio_handler = new AudioHandler();

    ret= audio_handler->Init(&avsync, dexmuxThread->AudioStreamTimebase(),dexmuxThread->getAudioCodecParameters(),audio_frame_queue);
    if(ret<0){
        qDebug("AudioHandler.init(...) failed");
        return -1;
    }
    return 0;
}

int MultimediaHandler::Start()
{
    int ret =-1;
    ret = dexmuxThread->Start();
    if(ret < 0) {
        qDebug("RunnableManager.Start()->dexmuxThread->Start() failed");
        return -1 ;
    }

    ret=audio_decodec_thread->Start();
    if(ret<0){
        qDebug("RunnableManager.Start(...)->audio_decodec_thread->Start() failed");
        return -1;
    }

    ret=video_decodec_thread->Start();
    if(ret<0){
        qDebug("RunnableManager.Start(...)->video_decodec_thread->Start() failed");
        return  -1;
    }
    ret= video_handler->Start();
    if(ret<0){
        qDebug("RunnableManager.Start()->video_handler->Start() failed");
        return  -1;
    }
    return 0;
}

int MultimediaHandler::Stop()
{

}

int MultimediaHandler::width()
{
    return __video_width;
}

int MultimediaHandler::height()
{
    return __video_height;
}

