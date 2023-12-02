#include "DecodecBase.h"

DecodecBase::DecodecBase()
{

}

int DecodecBase::initDecodec(AVCodecParameters *codec_param)
{


    if(!codec_param)return -1;

    __codec_ctx= avcodec_alloc_context3(NULL);
    int ret = avcodec_parameters_to_context(__codec_ctx,codec_param);
    if(ret <0){
        av_strerror(ret,err2str,sizeof (err2str));
        fprintf(stderr,"avcodec_parameters_to_context failed, ret%d,err2str:%s",ret,err2str);
        return -1;
    }

    const AVCodec *codec = avcodec_find_decoder(__codec_ctx->codec_id);
    if(!codec){
       fprintf(stderr,"avcodec_find_decoder failed");
        return -1;
    }

    /* Init the decoders */
    if ((ret = avcodec_open2(__codec_ctx, codec, NULL)) < 0) {
        av_strerror(ret,err2str,sizeof (err2str));
        fprintf(stderr,"avcodec_open2 failed, ret%d,err2str:%s",ret,err2str);
        return -1;
    }


    return ret;
}

void DecodecBase::deInitDecodec()
{
    if(__codec_ctx){
         avcodec_free_context(&__codec_ctx);
    }
}

int DecodecBase::SendPacket(AVPacket *packet)
{
    if(__codec_ctx){
        return   avcodec_send_packet(__codec_ctx,packet);
    }
    return -1;
}

int DecodecBase::ReceiveFrame(AVFrame *frame)
{
    if(__codec_ctx){
        return   avcodec_receive_frame(__codec_ctx,frame);
    }
    return -1;
}
