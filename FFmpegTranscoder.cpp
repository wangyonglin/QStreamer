#include "FFmpegTranscoder.h"


FFmpegTranscoder::VideoTranscoder::VideoTranscoder(int dst_width, int dst_height, AVPixelFormat dst_format)
    :__dst_width(dst_width),__dst_height(dst_height),__dst_format(dst_format)
{

}

FFmpegTranscoder::VideoTranscoder::~VideoTranscoder()
{
    __freeVideo(__img_convert_ctx);
}

int FFmpegTranscoder::VideoTranscoder::convertFrame(uint8_t **dst_data, AVFrame *frame)
{
    if(!frame){
         qDebug( "FFmpegTranscoder::transcoding frame not null\n");
         return -1;
    }
    if(__img_convert_ctx==nullptr){
      if(__initVideo(&__img_convert_ctx,frame)!=0){
          qDebug( "Could not allocate destination image\n");
          return -1;
      }
    }

    /* convert to destination format */
    if( sws_scale(__img_convert_ctx, (const uint8_t * const*)frame->data,
                  frame->linesize, 0, frame->height, __dst_data, __dst_linesize)<0){
        qDebug( "Could not sws_scale destination format\n");
        return -2;
    }
    *dst_data=__dst_data[0];
    return 0;

}

uint8_t *FFmpegTranscoder::VideoTranscoder::dst_data()
{
    return __dst_data[0];
}
int FFmpegTranscoder::VideoTranscoder::__initVideo(struct SwsContext **img_convert_ctx,AVFrame * frame)
{
    int  ret =-1;
    /* create scaling context */
        (*img_convert_ctx) = sws_getContext(
                    frame->width,
                    frame->height,
                    static_cast<AVPixelFormat>(frame->format),
                    __dst_width,
                    __dst_height,
                    __dst_format,
                    SWS_BILINEAR,
                    nullptr,
                    nullptr,
                    nullptr);

        if (!(*img_convert_ctx)) {
            qDebug(
                    "Impossible to create scale context for the conversion "
                    "fmt:%s s:%dx%d -> fmt:%s s:%dx%d\n",
                    av_get_pix_fmt_name( static_cast<AVPixelFormat>(frame->format)), frame->width, frame->height,
                    av_get_pix_fmt_name(AV_PIX_FMT_RGB32), __dst_width, __dst_height);
            ret = AVERROR(EINVAL);
            return  -1;
        }
        /* buffer is going to be written to rawvideo file, no alignment */
        if (av_image_alloc(__dst_data, __dst_linesize,
                           __dst_width, __dst_height, __dst_format, 1) < 0) {
            qDebug( "Could not allocate destination image\n");
            return -2;
        }

        return  0;
}

void FFmpegTranscoder::VideoTranscoder::__freeVideo(struct SwsContext *img_convert_ctx)
{
    if(img_convert_ctx){
        sws_freeContext(img_convert_ctx);
        img_convert_ctx=nullptr;
    }
     av_freep(&__dst_data[0]);
}






FFmpegTranscoder::AudioTranscoder::AudioTranscoder(int dst_rate, AVChannelLayout dst_ch_layout, AVSampleFormat dst_sample_fmt)
    :__dst_rate(dst_rate),__dst_ch_layout(dst_ch_layout),__dst_sample_fmt(dst_sample_fmt)
{

}

FFmpegTranscoder::AudioTranscoder::~AudioTranscoder()
{
    exitResampler(this->__au_convert_ctx);
}

int FFmpegTranscoder::AudioTranscoder::convert(AVFrame **dst_frame, AVFrame *frame)
{
    int ret =-1;
    if(!frame)return -1;
    if(__au_convert_ctx==nullptr){
        if((initResampler(&__au_convert_ctx,frame))!=0){
            return -1;
        }
    }


    int dst_samples = frame->ch_layout.nb_channels * av_rescale_rnd(swr_get_delay(__au_convert_ctx, frame->sample_rate)
                                                       + frame->nb_samples,
                                                       __dst_rate,
                                                       frame->sample_rate,
                                                       AV_ROUND_UP);
    uint8_t* dst_data = NULL;
    ret = av_samples_alloc(&dst_data,NULL,1,dst_samples,__dst_sample_fmt,1);
    dst_samples = frame->ch_layout.nb_channels * swr_convert( __au_convert_ctx,&dst_data,
                                                 dst_samples,
                                                 (const uint8_t**)frame->data,
                                                 frame->nb_samples);
    ret = av_samples_fill_arrays((*dst_frame)->data,(*dst_frame)->linesize,dst_data,1,dst_samples,__dst_sample_fmt,1);
    av_freep(dst_data);
    return ret;
}

int FFmpegTranscoder::AudioTranscoder::initResampler(struct SwrContext **au_convert_ctx,AVFrame * frame)
{
    if(!frame)return -1;
    int ret =-1;
    /* create resampler context */
        if (!((*au_convert_ctx) = swr_alloc())) {
            fprintf(stderr, "Could not allocate resampler context\n");
            ret = AVERROR(ENOMEM);
            return -1;
        }

        /* set options */
        av_opt_set_chlayout((*au_convert_ctx), "in_chlayout",    &frame->ch_layout, 0);
        av_opt_set_int((*au_convert_ctx), "in_sample_rate",       frame->sample_rate, 0);
        av_opt_set_sample_fmt((*au_convert_ctx), "in_sample_fmt",  static_cast<AVSampleFormat>(frame->format), 0);

        av_opt_set_chlayout((*au_convert_ctx), "out_chlayout",    &__dst_ch_layout, 0);
        av_opt_set_int((*au_convert_ctx), "out_sample_rate",       __dst_rate, 0);
        av_opt_set_sample_fmt((*au_convert_ctx), "out_sample_fmt", __dst_sample_fmt, 0);

        /* initialize the resampling context */
        if ((ret = swr_init((*au_convert_ctx))) < 0) {
            fprintf(stderr, "Failed to initialize the resampling context\n");
            return -1;
        }

    return 0;
}

void FFmpegTranscoder::AudioTranscoder::exitResampler(SwrContext *au_convert_ctx)
{
    if(au_convert_ctx){
        swr_free(&au_convert_ctx);
        au_convert_ctx=nullptr;
    }

}
