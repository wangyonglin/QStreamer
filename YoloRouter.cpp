#include "YoloRouter.h"
#include <QDebug>


YoloRouter::AudioRouter::AudioRouter(YoloQueue::FrameQueue *frameQueue)
    :__frameQueue(frameQueue)
{

}

void YoloRouter::AudioRouter::startAudioRouter(YoloSystems::AudioPlayer *player, AVCodecContext *codec_ctx)
{
    initAudioRouter(player,codec_ctx);
    startRunnable(&YoloRouter::AudioRouter::Runnable,this);
}

int YoloRouter::AudioRouter::initAudioRouter(YoloSystems::AudioPlayer *player, AVCodecContext *codec_ctx)
{
    resampleValues.channel_layout=av_get_default_channel_layout(2);
    resampleValues.channels=player->obtained.channels;
    resampleValues.sample_fmt=toSampleFormat(player->obtained.format);
    resampleValues.sample_rate=player->obtained.freq;
    this->au_codec_ctx=codec_ctx;
    this->au_player = player;
    return 0;
}

void YoloRouter::AudioRouter::Runnable()
{
    uint8_t* dst_data=nullptr;
    SwrContext * au_convert_ctx=nullptr;
    unsigned int dst_datasize=0;
    while (true) {
        if(au_player){
            YoloFFmpeg::FramePlus* framePlus=nullptr;
           ;
            if( __frameQueue->getFramePlus(&framePlus,10)==0){

                if(resampleInit(&au_convert_ctx,resampleValues,framePlus->frame)==0){
                    resampleConvert(au_convert_ctx,&dst_data,&dst_datasize,framePlus->frame);
                     qDebug() << "getTimebase： " << framePlus->frame->pts * av_q2d(framePlus->time_base);
                    au_player->addQueueAudioValue(dst_data,dst_datasize,true);
                    if(dst_data){
                        av_freep(&dst_data);
                    }
                    resampleFree(au_convert_ctx);
                }
            }
          YoloFFmpeg::FramePlus::freeFramePlus(&framePlus);
        }
    }
}

AVSampleFormat YoloRouter::AudioRouter::toSampleFormat(const SDL_AudioFormat &sample_fmt)
{
    AVSampleFormat fmt;
    switch (sample_fmt) {
        case AUDIO_U8:
            fmt = AV_SAMPLE_FMT_U8;
            break;
        case AUDIO_S16SYS:
            fmt = AV_SAMPLE_FMT_S16;
            break;
        case AUDIO_S32SYS:
            fmt = AV_SAMPLE_FMT_S32;
            break;
        // ... 其他格式
        default:
            fmt = AV_SAMPLE_FMT_NONE;
            break;
    }
    return fmt;
}

int YoloRouter::AudioRouter::resampleConvert(SwrContext *au_convert_ctx,
                                                     uint8_t **dst_data,
                                                     unsigned int *dst_datasize,
                                                     AVFrame * frame)
{

    if(!au_convert_ctx&&!frame )return -1;

    int out_samples = frame->nb_samples * resampleValues.sample_rate/frame->sample_rate + 256;
    int out_bytes = av_samples_get_buffer_size(NULL, resampleValues.channels, out_samples, resampleValues.sample_fmt, 0);
    if(out_bytes <0) {
        qDebug() << "av_samples_get_buffer_size failed";
        return-1;
    }
    av_fast_malloc(&(*dst_data), dst_datasize, out_bytes);
    int error;
    /* Convert the samples using the resampler. */
    if ((error = swr_convert(au_convert_ctx,dst_data, out_samples, (const uint8_t **)frame->extended_data, frame->nb_samples)) < 0) {
         av_strerror(error, err2str, sizeof(err2str));
        qDebug() << "Could not convert input samples errr:" <<  err2str;
        return error;
    }
    *dst_datasize = av_samples_get_buffer_size(NULL, resampleValues.channels, error, resampleValues.sample_fmt, 1);

    return 0;
}

int YoloRouter::AudioRouter::resampleInit(SwrContext **au_convert_ctx,
                                                  YoloRouter::AudioRouter::ResampleValues &resampleValues,
                                                   AVFrame *frame)
{
    (*au_convert_ctx) = swr_alloc_set_opts(NULL,
                                      resampleValues.channel_layout,
                                      resampleValues.sample_fmt,
                                      resampleValues.sample_rate,
                                      frame->channel_layout,
                                      (enum AVSampleFormat)frame->format,
                                      frame->sample_rate,
                                      0, NULL);
    if (!(*au_convert_ctx) || swr_init((*au_convert_ctx)) < 0) {
        qDebug(
                    "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
                    frame->sample_rate,
                    "D" /*av_get_sample_fmt_name((enum AVSampleFormat)frame->format)*/,
                    frame->channels,
                    resampleValues.sample_rate,
                    "D"/*av_get_sample_fmt_name((enum AVSampleFormat)is->dst_tgt_.fmt)*/,
                    resampleValues.channels);
        swr_free((SwrContext **)(au_convert_ctx));
        (*au_convert_ctx)=nullptr;
        return -1;
    }
    return 0;
}

void YoloRouter::AudioRouter::resampleClear(uint8_t *dst_data)
{
    if(dst_data){
     av_free(dst_data);
    }
}

void YoloRouter::AudioRouter::resampleFree(SwrContext *au_convert_ctx)
{
    if(au_convert_ctx){
         swr_free((SwrContext **)(&au_convert_ctx));
    }
}

YoloRouter::VideoRouter::VideoRouter(YoloQueue::FrameQueue *frameQueue)
    :__frameQueue(frameQueue)
{

}

YoloRouter::VideoRouter::~VideoRouter()
{

}

void YoloRouter::VideoRouter::startVideoRouter(YoloSystems::VideoPlayer * videoPlayer,AVCodecContext *codec_ctx)
{

    initVideoRouter(videoPlayer,codec_ctx);
    startRunnable(&YoloRouter::VideoRouter::Runnable,this);
}

int YoloRouter::VideoRouter::initVideoRouter(YoloSystems::VideoPlayer * videoPlayer,AVCodecContext *codec_ctx)
{
    Q_UNUSED(codec_ctx);

    videoPlayer->connect(this, &YoloRouter::VideoRouter::repaint, videoPlayer, &YoloSystems::VideoPlayer::repaint, Qt::BlockingQueuedConnection);
    return 0;
}

int YoloRouter::VideoRouter::initVideoRouter(QImage *image, AVCodecContext *codec_ctx)
{

}

void YoloRouter::VideoRouter::Runnable()
{
    while (true) {
         refresh();
    }

}
// 0.01秒
void YoloRouter::VideoRouter::refresh()
{
    YoloFFmpeg::FramePlus * framePlus=nullptr;
    if(__frameQueue->getFramePlus(&framePlus,1)==0) {
        emit repaint(framePlus->frame);
        YoloFFmpeg::FramePlus::freeFramePlus(&framePlus);
    }
}
