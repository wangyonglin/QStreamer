#ifndef YOLORENDER_H
#define YOLORENDER_H

#include <FFmpegOutput.h>

#include <OpenGLWidget.h>
#include <QObject>
#include <QThread>
#include <FFmpegPublic.h>
Q_DECLARE_METATYPE(AVFrame)
namespace FFmpegRouter{

    class AudioRouter :public FFmpegPublic::Thread::Runnable{
    public:
        typedef struct{
            int channels;
            int64_t channel_layout;
            int   sample_rate;
            enum AVSampleFormat sample_fmt;
        }ResampleValues;
    public:
        AudioRouter(FFmpegPublic::Queue::Frame * frameQueue);
        ~AudioRouter();
        void startAudioRouter(FFmpegPublic::SyncTime * synctime,FFmpegOutput::AudioPlayer *player,AVCodecContext *codec_ctx);
    private:
        int initAudioRouter(FFmpegPublic::SyncTime * synctime,FFmpegOutput::AudioPlayer *player,AVCodecContext *codec_ctx);
        virtual void entityRunnable() override;
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
        FFmpegPublic::SyncTime * av_synctime=nullptr;
        FFmpegOutput::AudioPlayer *au_player=nullptr;
        AVCodecContext *au_codec_ctx=nullptr;
        ResampleValues resampleValues;
        FFmpegPublic::Queue::Frame * __frameQueue=nullptr;
    };

    class VideoRouter :public QObject,public FFmpegPublic::Thread::Runnable  {

    Q_OBJECT
    public:
        VideoRouter(FFmpegPublic::Queue::Frame *frameQueue);
        ~VideoRouter();
        void startVideoRouter(FFmpegPublic::SyncTime * synctime,FFmpegOutput::VideoPlayer * pVideoPlayer,AVCodecContext *codec_ctx);
    signals:
        void repaint(AVFrame* frame);               // 重绘
    private:
        FFmpegPublic::SyncTime * av_synctime;
        FFmpegPublic::Queue::Frame *__frameQueue=nullptr;
        AVCodecContext *__video_codec_ctx=nullptr;

          int initVideoRouter(FFmpegPublic::SyncTime * synctime,FFmpegOutput::VideoPlayer * pVideoPlayer,AVCodecContext *codec_ctx);
        int initVideoRouter(FFmpegPublic::SyncTime * synctime,QImage * image,AVCodecContext *codec_ctx);
        void refresh();
         virtual void entityRunnable() override;
    };
}
#endif // YOLORENDER_H
