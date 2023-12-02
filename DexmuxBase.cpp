#include "DexmuxBase.h"

DexmuxBase::DexmuxBase()
{

}

DexmuxBase::~DexmuxBase()
{

}

int DexmuxBase::initDexmux(const std::string &url)
{
    int ret = -1;
    AVDictionary* dict = nullptr;
    av_dict_set(&dict, "rtsp_transport", "tcp", 0);      // 设置rtsp流使用tcp打开，如果打开失败错误信息为【Error number -135 occurred】可以切换（UDP、tcp、udp_multicast、http），比如vlc推流就需要使用udp打开
    av_dict_set(&dict, "max_delay", "3", 0);             // 设置最大复用或解复用延迟（以微秒为单位）。当通过【UDP】 接收数据时，解复用器尝试重新排序接收到的数据包（因为它们可能无序到达，或者数据包可能完全丢失）。这可以通过将最大解复用延迟设置为零（通过max_delayAVFormatContext 字段）来禁用。
    av_dict_set(&dict, "timeout", "1000000", 0);         // 以微秒为单位设置套接字 TCP I/O 超时，如果等待时间过短，也可能会还没连接就返回了。

    /* open input file, and allocate format context */
    if ((ret=avformat_open_input(&__fmt_ctx, url.c_str(), nullptr, &dict)) != 0) {
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
    if (avformat_find_stream_info(__fmt_ctx, nullptr) < 0) {
        avformat_close_input(&__fmt_ctx);
        fprintf(stderr,"Could not find stream information\n");
        return -1;
    }
    __audio_stream_idx = av_find_best_stream(__fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (__audio_stream_idx < 0) {
        av_strerror(__audio_stream_idx, err2str, sizeof(err2str));
        fprintf(stderr, "Could not find %s stream in input file err2str: %s\n", av_get_media_type_string(AVMEDIA_TYPE_AUDIO),err2str);

    }
    __video_stream_idx = av_find_best_stream(__fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (__video_stream_idx < 0) {
        av_strerror(__video_stream_idx, err2str, sizeof(err2str));
        fprintf(stderr, "Could not find %s stream in input file  err2str: %s\n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO),err2str);

    }
    av_dump_format(__fmt_ctx, 0, url.c_str(), 0);
    return 0;
}

void DexmuxBase::deInitDexmux()
{
    if(__fmt_ctx){
        avformat_close_input(&__fmt_ctx);
        __fmt_ctx=nullptr;
    }
}

int DexmuxBase::getVideoWidth()
{
    if(!__fmt_ctx)return 0;
    int idx=-1;
    if( (idx=av_find_best_stream(__fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
        return 0;
    }
    return __fmt_ctx->streams[idx]->codecpar->width;
}

int DexmuxBase::getVideoHeight()
{
    if(!__fmt_ctx)return 0;
    int idx=-1;
    if( (idx=av_find_best_stream(__fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
        return -1;
    }
    return __fmt_ctx->streams[idx]->codecpar->height;
}

AVCodecParameters *DexmuxBase::getCodecParameters(DexmuxMidea type)
{
    if(!__fmt_ctx)return nullptr;
    int idx=-1;
    if( (idx=av_find_best_stream(__fmt_ctx, (enum AVMediaType) type, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)type));
        return nullptr;
    }
    return __fmt_ctx->streams[idx]->codecpar;
}

AVRational DexmuxBase::getTimebase(DexmuxMidea type)
{
    if(!__fmt_ctx)return  AVRational{0, 0};
    int idx=-1;
    if( (idx=av_find_best_stream(__fmt_ctx, (enum AVMediaType)type, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)type));
        return AVRational{0, 0};
    }
    return __fmt_ctx->streams[idx]->time_base;
}

int DexmuxBase::getStreamIndex(DexmuxMidea midea)
{
    if(!__fmt_ctx)return -1;
    return  av_find_best_stream(__fmt_ctx, (enum AVMediaType)midea, -1, -1, NULL, 0);
}

int DexmuxBase::readFrame(AVPacket *packet)
{
    if(!__fmt_ctx)return -1;
    return av_read_frame(__fmt_ctx, packet);
}

