#include "FFmpegPlayer.h"

FFmpegPlayer::FFmpegPlayer(QWidget *parent )
    :QWidget(parent)
{

}



FFmpegPlayer::FFmpegPlayer(QWidget *parent, const std::string &urlString)
 :QWidget(parent),__urlString(urlString)
{
    initFFmpegPlayer(__urlString);
}

FFmpegPlayer::~FFmpegPlayer()
{
    freeFFmpegPlayer();
}

int FFmpegPlayer::initFFmpegPlayer(const std::string &urlString)
{

      av_synctime.initClock(FFmpegPublic::ClockType::External_Master);
    __audioPacketQueue =new FFmpegPublic::Queue::Packet();
    __videoPacketQueue =new FFmpegPublic::Queue::Packet();
    __audioFrameQueue= new FFmpegPublic::Queue::Frame();
    __videoFrameQueue= new FFmpegPublic::Queue::Frame();
    __dexmuxThread= new FFmpegThread::DexmuxThread(__audioPacketQueue,__videoPacketQueue);
    __dexmuxThread->startThread(urlString);


    __audioDecidecThread =new FFmpegThread::DecodecThread(__audioPacketQueue,__audioFrameQueue);
    __audioDecidecThread->startThread(__dexmuxThread->getAudioCodecParameters());

    __videoDecidecThread =new FFmpegThread::DecodecThread(__videoPacketQueue,__videoFrameQueue);
    __videoDecidecThread->startThread(__dexmuxThread->getVideoCodecParameters());

    __audioRouter= new FFmpegRouter::AudioRouter(__audioFrameQueue);


    __videoRouter=new  FFmpegRouter::VideoRouter(__videoFrameQueue);

    return 0;
}

void FFmpegPlayer::initAudioPlayer(FFmpegOutput::AudioPlayer *pAudioPlayer)
{
    __audioRouter->startAudioRouter(&av_synctime,pAudioPlayer,__audioDecidecThread->getCodeContext());
}

void FFmpegPlayer::initVideoPlayer(FFmpegOutput::VideoPlayer *pVideoPlayer)
{
     __videoRouter->startVideoRouter(&av_synctime,pVideoPlayer,__videoDecidecThread->getCodeContext());
}



void FFmpegPlayer::freeFFmpegPlayer()
{
    if(__dexmuxThread){
        __dexmuxThread->stopThread();
        __dexmuxThread->freeThread();
    }

    if(__audioDecidecThread){
        __audioDecidecThread->stopThread();
    }
    if(__videoDecidecThread){
        __videoDecidecThread->stopThread();
    }
}

void FFmpegPlayer::playFFmpegPlayer()
{

}

void FFmpegPlayer::stopFFmpegPlayer()
{

}



