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
    __dexmux_runnable = new DexmuxRunnable(audio_packet_queue,video_packet_queue);
    if(__dexmux_runnable->initDexmux(url)!=0){
        qDebug("MultimediaHandler::Init  DexmuxRunnable failed");
        return -1;
    }else{
       __video_width= __dexmux_runnable->getVideoWidth();
       __video_height= __dexmux_runnable->getVideoHeight();
    }

    // 2.解码线程初始化
//    audio_decodec_thread =  new DecodecThread(audio_packet_queue,audio_frame_queue);

//    ret= audio_decodec_thread->Init(__dexmux_runnable->getCodecParameters(DEXMUX_MIDEA_AUDIO));
//    if(ret<0){
//        qDebug("MultimediaHandler::Init  audio_decodec_thread failed");
//        return -1;
//    }

//    video_decodec_thread =  new DecodecThread(video_packet_queue,video_frame_queue);
//    ret= video_decodec_thread->Init(__dexmux_runnable->getCodecParameters(DEXMUX_MIDEA_VIDEO));
//    if(ret<0){
//        qDebug("MultimediaHandler::Init  video_decodec_thread failed");
//        return -1;
//    }

    __audio_decodec_runnable = new DecodecRunnable(audio_packet_queue,audio_frame_queue);
    __audio_decodec_runnable->initDecodec(__dexmux_runnable->getCodecParameters(DEXMUX_MIDEA_AUDIO));

    __video_decodec_runnable = new DecodecRunnable(video_packet_queue,video_frame_queue);
    __video_decodec_runnable->initDecodec(__dexmux_runnable->getCodecParameters(DEXMUX_MIDEA_VIDEO));


    video_handler=new  VideoHandler();
    ret= video_handler->Init(&avsync, __dexmux_runnable->getTimebase(DEXMUX_MIDEA_VIDEO),video_frame_queue);

    if(ret<0){
        qDebug("VideoHandler.init(...) failed");
        return -1;
    }


    audio_handler = new AudioHandler();

    ret= audio_handler->Init(&avsync, __dexmux_runnable->getTimebase(DEXMUX_MIDEA_AUDIO),__dexmux_runnable->getCodecParameters(DEXMUX_MIDEA_AUDIO),audio_frame_queue);
    if(ret<0){
        qDebug("AudioHandler.init(...) failed");
        return -1;
    }
    return 0;
}

int MultimediaHandler::Start()
{
    int ret =-1;
    ret = __dexmux_runnable->startRunnable();
    if(ret < 0) {
        qDebug("RunnableManager.Start()->dexmuxThread->Start() failed");
        return -1 ;
    }
    __audio_decodec_runnable->startDecodec();
    __video_decodec_runnable->startDecodec();

    ret= video_handler->Start();
    if(ret<0){
        qDebug("RunnableManager.Start()->video_handler->Start() failed");
        return  -1;
    }
    return 0;
}

int MultimediaHandler::Stop()
{
    int ret =-1;
    ret = __dexmux_runnable->Stop();
    if(ret < 0) {
        qDebug("RunnableManager.Start()->dexmuxThread->Start() failed");
        return -1 ;
    }

//    ret=audio_decodec_thread->Stop();
//    if(ret<0){
//        qDebug("RunnableManager.Start(...)->audio_decodec_thread->Start() failed");
//        return -1;
//    }

//    ret=video_decodec_thread->Stop();
//    if(ret<0){
//        qDebug("RunnableManager.Start(...)->video_decodec_thread->Start() failed");
//        return  -1;
//    }
    __audio_decodec_runnable->stopDecodec();
    __video_decodec_runnable->stopDecodec();

    ret= video_handler->Stop();
    if(ret<0){
        qDebug("RunnableManager.Start()->video_handler->Start() failed");
        return  -1;
    }
    return 0;
}

int MultimediaHandler::width()
{
    return __video_width;
}

int MultimediaHandler::height()
{
    return __video_height;
}

