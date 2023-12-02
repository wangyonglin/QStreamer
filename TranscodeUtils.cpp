#include "TranscodeUtils.h"


TranscodeUtils::TranscodeUtils(int dst_width, int dst_height, AVPixelFormat dst_format)
    :__dst_width(dst_width),__dst_height(dst_height),__dst_format(dst_format)
{

}

TranscodeUtils::~TranscodeUtils()
{
    __free(__img_convert_ctx);
}

int TranscodeUtils::transcoding(uint8_t **dst_data, AVFrame *frame)
{
    if(!frame){
         qDebug( "TranscodeUtils::transcoding frame not null\n");
         return -1;
    }
    if(__img_convert_ctx==nullptr){
      if(__init(&__img_convert_ctx,frame)!=0){
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

uint8_t *TranscodeUtils::dst_data()
{
    return __dst_data[0];
}
int TranscodeUtils::__init(struct SwsContext **img_convert_ctx,AVFrame * frame)
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

void TranscodeUtils::__free(struct SwsContext *img_convert_ctx)
{
    if(img_convert_ctx){
        sws_freeContext(img_convert_ctx);
        img_convert_ctx=nullptr;
    }
     av_freep(&__dst_data[0]);
}
