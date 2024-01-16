#include "FFmpegBase.h"
#include <QList>
#include <QCamera>
#include <QCameraInfo>
#include <QDebug>


FFmpegBase::DecodecFFmpeg::DecodecFFmpeg()
{

}

FFmpegBase::DecodecFFmpeg::~DecodecFFmpeg()
{

}

int FFmpegBase::DecodecFFmpeg::initDecodecFFmpeg(AVCodecParameters *codecParameters)
{
    if(!codecParameters)return -1;

    codec_ctx= avcodec_alloc_context3(NULL);
    int ret = avcodec_parameters_to_context(codec_ctx,codecParameters);
    if(ret <0){
        av_strerror(ret,err2str,sizeof (err2str));
        qDebug() << "avcodec_parameters_to_context failed, ret" <<ret<<"err2str:"<<  err2str;
        return -1;
    }

    const AVCodec *codec = avcodec_find_decoder(codec_ctx->codec_id);
    if(!codec){
       fprintf(stderr,"avcodec_find_decoder failed");
        return -1;
    }

    /* Init the decoders */
    if ((ret = avcodec_open2(codec_ctx, codec, NULL)) < 0) {
        av_strerror(ret,err2str,sizeof (err2str));
        fprintf(stderr,"avcodec_open2 failed, ret%d,err2str:%s",ret,err2str);
        return -1;
    }
    return 0;
}

void FFmpegBase::DecodecFFmpeg::freeDecodecFFmpeg()
{
    if(codec_ctx){
         avcodec_free_context(&codec_ctx);
    }
}

int FFmpegBase::DecodecFFmpeg::sendPacket(AVPacket *packet)
{
    if(codec_ctx){
        return   avcodec_send_packet(codec_ctx,packet);
    }
    return -1;
}

int FFmpegBase::DecodecFFmpeg::receiveFrame(AVFrame *frame)
{
    if(codec_ctx){
        return   avcodec_receive_frame(codec_ctx,frame);
    }
    return -1;
}



AVCodecContext *FFmpegBase::DecodecFFmpeg::getCodeContext()
{
    return  codec_ctx;
}


AVRational FFmpegBase::DecodecFFmpeg::getTimebase()
{
    if(codec_ctx){
       return  codec_ctx->time_base;
    }
    return {0, 0};
}

FFmpegBase::DexmuxFFmpeg::DexmuxFFmpeg()
{

}

FFmpegBase::DexmuxFFmpeg::~DexmuxFFmpeg()
{

}

int FFmpegBase::DexmuxFFmpeg::initDexmuxFFmpeg(const std::string &url)
{
    int ret = -1;
    AVDictionary* dict = nullptr;
    av_dict_set(&dict, "rtsp_transport", "tcp", 0);      // 设置rtsp流使用tcp打开，如果打开失败错误信息为【Error number -135 occurred】可以切换（UDP、tcp、udp_multicast、http），比如vlc推流就需要使用udp打开
    av_dict_set(&dict, "max_delay", "3", 0);             // 设置最大复用或解复用延迟（以微秒为单位）。当通过【UDP】 接收数据时，解复用器尝试重新排序接收到的数据包（因为它们可能无序到达，或者数据包可能完全丢失）。这可以通过将最大解复用延迟设置为零（通过max_delayAVFormatContext 字段）来禁用。
    av_dict_set(&dict, "timeout", "1000000", 0);         // 以微秒为单位设置套接字 TCP I/O 超时，如果等待时间过短，也可能会还没连接就返回了。

    /* open input file, and allocate format context */
    if ((ret=avformat_open_input(&fmt_ctx, url.c_str(), nullptr, &dict)) != 0) {
        av_strerror(ret, err2str, sizeof(err2str));
        fprintf(stderr, "Could not open source file (%s) err2str: %s",url.c_str(),err2str);
        return -1;
    }
    // 释放参数字典
    if(dict)
    {
        av_dict_free(&dict);
    }
    /* retrieve stream information */
    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        avformat_close_input(&fmt_ctx);
        fprintf(stderr,"Could not find stream information\n");
        return -1;
    }
    __audio_stream_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (__audio_stream_idx < 0) {
        av_strerror(__audio_stream_idx, err2str, sizeof(err2str));
        fprintf(stderr, "Could not find %s stream in input file err2str: %s\n", av_get_media_type_string(AVMEDIA_TYPE_AUDIO),err2str);

    }
    __video_stream_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (__video_stream_idx < 0) {
        av_strerror(__video_stream_idx, err2str, sizeof(err2str));
        fprintf(stderr, "Could not find %s stream in input file  err2str: %s\n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO),err2str);

    }
    av_dump_format(fmt_ctx, 0, url.c_str(), 0);
    return 0;
}

void FFmpegBase::DexmuxFFmpeg::freeDexmuxFFmpeg()
{
    if(fmt_ctx){
        avformat_close_input(&fmt_ctx);
        fmt_ctx=nullptr;
    }
}

int FFmpegBase::DexmuxFFmpeg::getWidth()
{
    if(!fmt_ctx)return 0;
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
        return 0;
    }
    return fmt_ctx->streams[idx]->codecpar->width;
}

int FFmpegBase::DexmuxFFmpeg::getHeight()
{
    if(!fmt_ctx)return 0;
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
        return 0;
    }
    return fmt_ctx->streams[idx]->codecpar->height;
}

int FFmpegBase::DexmuxFFmpeg::getTotalTime()
{
    if(!fmt_ctx)return -1;
    return  fmt_ctx->duration / (AV_TIME_BASE / 1000); // 计算视频总时长（毫秒）
}

int FFmpegBase::DexmuxFFmpeg::receiveFrame(AVPacket *packet)
{
    if(!fmt_ctx)return -1;
    return av_read_frame(fmt_ctx, packet);
}

AVFormatContext *FFmpegBase::DexmuxFFmpeg::getFormatContext()
{
    return  fmt_ctx;
}


AVRational FFmpegBase::DexmuxFFmpeg::getAudioTimebase()
{
    if(!fmt_ctx)return  AVRational{0, 0};
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, (enum AVMediaType)AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)AVMEDIA_TYPE_AUDIO));
        return AVRational{0, 0};
    }
    return fmt_ctx->streams[idx]->time_base;
}

AVRational FFmpegBase::DexmuxFFmpeg::getVideoTimebase()
{
    if(!fmt_ctx)return  AVRational{0, 0};
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, (enum AVMediaType)AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)AVMEDIA_TYPE_VIDEO));
        return AVRational{0, 0};
    }
    return fmt_ctx->streams[idx]->time_base;
}

int FFmpegBase::DexmuxFFmpeg::getAudioStreamIndex()
{
    if(!fmt_ctx)return -1;
    return  av_find_best_stream(fmt_ctx, (enum AVMediaType)AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

}

int FFmpegBase::DexmuxFFmpeg::getVideoStreamIndex()
{
    if(!fmt_ctx)return -1;
    return  av_find_best_stream(fmt_ctx, (enum AVMediaType)AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
}

AVCodecParameters *FFmpegBase::DexmuxFFmpeg::getAudioCodecParameters()
{
    if(!fmt_ctx)return nullptr;
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, (enum AVMediaType) AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)AVMEDIA_TYPE_AUDIO));
        return nullptr;
    }
    return fmt_ctx->streams[idx]->codecpar;
}

AVCodecParameters *FFmpegBase::DexmuxFFmpeg::getVideoCodecParameters()
{

    if(!fmt_ctx)return nullptr;
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, (enum AVMediaType) AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)AVMEDIA_TYPE_VIDEO));
        return nullptr;
    }
    return fmt_ctx->streams[idx]->codecpar;
}





int64_t FFmpegBase::Funcation::getTime()
{
    return av_gettime();
}
