#ifndef FFmpegPlayer_H
#define FFmpegPlayer_H
#include <QWidget>
#include <FFmpegBase.h>
#include <FFmpegThread.h>
#include <FFmpegRouter.h>
#include <FFmpegPublic.h>
#include <FFmpegPublic.h>

class FFmpegPlayer : public QWidget
{
public:
    FFmpegPlayer(QWidget *parent = nullptr);
    FFmpegPlayer(QWidget *parent = nullptr,const std::string &urlString=nullptr);
    ~FFmpegPlayer();
    void initAudioPlayer(FFmpegOutput::AudioPlayer * pAudioPlayer);
    void initVideoPlayer(FFmpegOutput::VideoPlayer * pVideoPlayer);
    void freeFFmpegPlayer();
    void playFFmpegPlayer();
    void stopFFmpegPlayer();
private:
    int initFFmpegPlayer(const std::string &stringURL);




private:
    std::string __urlString;
    FFmpegPublic::SyncTime av_synctime;
    FFmpegThread::DexmuxThread* __dexmuxThread=nullptr;
    FFmpegThread::DecodecThread * __audioDecidecThread =nullptr;
    FFmpegThread::DecodecThread * __videoDecidecThread =nullptr;
    FFmpegRouter::AudioRouter *__audioRouter=nullptr;
    FFmpegRouter::VideoRouter * __videoRouter=nullptr;
    FFmpegPublic::Queue::Packet *__audioPacketQueue=nullptr;
    FFmpegPublic::Queue::Packet *__videoPacketQueue=nullptr;
    FFmpegPublic::Queue::Frame * __audioFrameQueue=nullptr;
    FFmpegPublic::Queue::Frame * __videoFrameQueue=nullptr;
    int __image_width=0;
    int __image_height=0;

};

#endif // FFmpegPlayer_H
