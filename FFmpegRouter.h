#ifndef YOLORENDER_H
#define YOLORENDER_H

#include <YoloSystems.h>
#include <YoloQueue.h>
#include <OpenGLWidget.h>
#include <QObject>
#include <QThread>
Q_DECLARE_METATYPE(AVFrame)
namespace YoloRouter{

    class AudioRouter :public YoloRunnable{
    public:
        typedef struct{
            int channels;
            int64_t channel_layout;
            int   sample_rate;
            enum AVSampleFormat sample_fmt;
        }ResampleValues;
    public:
        AudioRouter(YoloQueue::FrameQueue * frameQueue);
        ~AudioRouter();
        void startAudioRouter(YoloSystems::AudioPlayer *player,AVCodecContext *codec_ctx);
    private:
        int initAudioRouter(YoloSystems::AudioPlayer *player,AVCodecContext *codec_ctx);
        virtual void Runnable() override;
         AVSampleFormat toSampleFormat(const  SDL_AudioFormat &sample_fmt);
         int resampleConvert( SwrContext*au_convert_ctx,
                              uint8_t **dst_data,
                              unsigned int * dst_datasize,
                              AVFrame * frame);
         int resampleInit(SwrContext **au_convert_ctx,
                          ResampleValues &resampleValues,
                          AVFrame *frame);
         void resampleClear(uint8_t *dst_data);
         void resampleFree(SwrContext *au_convert_ctx);

         char err2str[256]={0};

        YoloSystems::AudioPlayer *au_player=nullptr;
        AVCodecContext *au_codec_ctx=nullptr;
        ResampleValues resampleValues;
        YoloQueue::FrameQueue * __frameQueue=nullptr;
    };

    class VideoRouter :public QObject,public YoloRunnable  {

    Q_OBJECT
    public:
        VideoRouter(YoloQueue::FrameQueue *frameQueue);
        ~VideoRouter();
        void startVideoRouter(YoloSystems::VideoPlayer * videoPlayer,AVCodecContext *codec_ctx);
    signals:
        void repaint(AVFrame* frame);               // 重绘
    private:
        YoloQueue::FrameQueue *__frameQueue=nullptr;
        AVCodecContext *__video_codec_ctx=nullptr;
        int initVideoRouter(YoloSystems::VideoPlayer * videoPlayer,AVCodecContext *codec_ctx);
        int initVideoRouter(QImage * image,AVCodecContext *codec_ctx);
        void refresh();
        virtual void Runnable() override;
    };
}
#endif // YOLORENDER_H
