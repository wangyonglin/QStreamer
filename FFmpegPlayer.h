#ifndef YOLOPLAYER_H
#define YOLOPLAYER_H
#include <QWidget>
#include <YoloFFmpeg.h>
#include <YoloThread.h>
#include <YoloRouter.h>
#include <YoloQueue.h>

class YoloPlayer : public QWidget
{
public:
    YoloPlayer(QWidget *parent = nullptr);
    ~YoloPlayer();
    int initYoloPlayer(const std::string &url,YoloSystems::AudioPlayer * audioPlayer,YoloSystems::VideoPlayer * videoPlayer);
    bool initVideo();
    bool initAudio();
    void freeYoloPlayer();
    void playYoloPlayer();
    void stopYoloPlayer();
private:
    YoloThread::DexmuxThread* __dexmuxThread=nullptr;

    YoloThread::DecodecThread * __audioDecidecThread =nullptr;
    YoloThread::DecodecThread * __videoDecidecThread =nullptr;


    YoloRouter::AudioRouter *__audioRouter=nullptr;
    YoloRouter::VideoRouter * __videoRouter=nullptr;
    YoloQueue::PacketQueue *__audioPacketQueue=nullptr;
    YoloQueue::PacketQueue *__videoPacketQueue=nullptr;
    YoloQueue::FrameQueue * __audioFrameQueue=nullptr;
    YoloQueue::FrameQueue * __videoFrameQueue=nullptr;
};

#endif // YOLOPLAYER_H
