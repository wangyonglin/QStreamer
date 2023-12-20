#include "YoloPlayer.h"

YoloPlayer::YoloPlayer(QWidget *parent)
    :QWidget(parent)
{

}

YoloPlayer::~YoloPlayer()
{

}

int YoloPlayer::initYoloPlayer(const std::string &url,YoloSystems::AudioPlayer * audioPlayer,YoloSystems::VideoPlayer * videoPlayer)
{

    __audioPacketQueue =new YoloQueue::PacketQueue();
    __videoPacketQueue =new YoloQueue::PacketQueue();
    __audioFrameQueue= new YoloQueue::FrameQueue();
    __videoFrameQueue= new YoloQueue::FrameQueue();
    __dexmuxThread= new YoloThread::DexmuxThread(__audioPacketQueue,__videoPacketQueue);
    __dexmuxThread->startThread(url);


    __audioDecidecThread =new YoloThread::DecodecThread(__audioPacketQueue,__audioFrameQueue);
    __audioDecidecThread->startThread(__dexmuxThread->getAudioCodecParameters());

    __videoDecidecThread =new YoloThread::DecodecThread(__videoPacketQueue,__videoFrameQueue);
    __videoDecidecThread->startThread(__dexmuxThread->getVideoCodecParameters());

    __audioRouter= new YoloRouter::AudioRouter(__audioFrameQueue);
    __audioRouter->startAudioRouter(audioPlayer,__audioDecidecThread->getCodeContext());


    __videoRouter=new  YoloRouter::VideoRouter(__videoFrameQueue);
    __videoRouter->startVideoRouter(videoPlayer,__videoDecidecThread->getCodeContext());
        return 0;
}

bool YoloPlayer::initVideo()
{

}

void YoloPlayer::freeYoloPlayer()
{
    if(__dexmuxThread){
        __dexmuxThread->stopThread();
        __dexmuxThread->freeThread();
    }
}

void YoloPlayer::playYoloPlayer()
{

}

void YoloPlayer::stopYoloPlayer()
{

}
