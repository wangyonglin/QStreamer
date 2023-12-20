#include "YoloFFmpeg.h"
#include <QList>
#include <QCamera>
#include <QCameraInfo>
#include <QDebug>


YoloFFmpeg::DecodecFFmpeg::DecodecFFmpeg()
{

}

YoloFFmpeg::DecodecFFmpeg::~DecodecFFmpeg()
{

}

int YoloFFmpeg::DecodecFFmpeg::initDecodecFFmpeg(AVCodecParameters *codecParameters)
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

void YoloFFmpeg::DecodecFFmpeg::freeDecodecFFmpeg()
{
    if(codec_ctx){
         avcodec_free_context(&codec_ctx);
    }
}

int YoloFFmpeg::DecodecFFmpeg::sendPacket(AVPacket *packet)
{
    if(codec_ctx){
        return   avcodec_send_packet(codec_ctx,packet);
    }
    return -1;
}

int YoloFFmpeg::DecodecFFmpeg::receiveFrame(AVFrame *frame)
{
    if(codec_ctx){
        return   avcodec_receive_frame(codec_ctx,frame);
    }
    return -1;
}

int YoloFFmpeg::DecodecFFmpeg::SendPacketPlus(YoloFFmpeg::PacketPlus *packetPlus)
{
    if(codec_ctx && packetPlus){
        return   avcodec_send_packet(codec_ctx,packetPlus->packet);
    }
    return -1;
}

int YoloFFmpeg::DecodecFFmpeg::ReceiveFramePlus(YoloFFmpeg::FramePlus **framePlus)
{
    int ret = -1;
    if(!codec_ctx )return -1;
        AVFrame * frame = av_frame_alloc();
        if(frame){
            if((ret=avcodec_receive_frame(codec_ctx,frame))==0){
                  YoloFFmpeg::FramePlus::initFramePlus(framePlus,frame,codec_ctx->time_base);
            }
            av_frame_free(&frame);
            return ret;
        }
    return -1;
}

AVCodecContext *YoloFFmpeg::DecodecFFmpeg::getCodeContext()
{
    return  codec_ctx;
}


AVRational YoloFFmpeg::DecodecFFmpeg::getTimebase()
{
    if(codec_ctx){
       return  codec_ctx->time_base;
    }
    return {0, 0};
}

YoloFFmpeg::DexmuxFFmpeg::DexmuxFFmpeg()
{

}

YoloFFmpeg::DexmuxFFmpeg::~DexmuxFFmpeg()
{

}

int YoloFFmpeg::DexmuxFFmpeg::initDexmuxFFmpeg(const std::string &url)
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

void YoloFFmpeg::DexmuxFFmpeg::freeDexmuxFFmpeg()
{
    if(fmt_ctx){
        avformat_close_input(&fmt_ctx);
        fmt_ctx=nullptr;
    }
}

int YoloFFmpeg::DexmuxFFmpeg::getWidth()
{
    if(!fmt_ctx)return 0;
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
        return 0;
    }
    return fmt_ctx->streams[idx]->codecpar->width;
}

int YoloFFmpeg::DexmuxFFmpeg::getHeight()
{
    if(!fmt_ctx)return 0;
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
        return 0;
    }
    return fmt_ctx->streams[idx]->codecpar->height;
}

int YoloFFmpeg::DexmuxFFmpeg::getTotalTime()
{
    if(!fmt_ctx)return -1;
    return  fmt_ctx->duration / (AV_TIME_BASE / 1000); // 计算视频总时长（毫秒）
}

int YoloFFmpeg::DexmuxFFmpeg::receiveFrame(AVPacket *packet)
{
    if(!fmt_ctx)return -1;
    return av_read_frame(fmt_ctx, packet);
}

AVFormatContext *YoloFFmpeg::DexmuxFFmpeg::getFormatContext()
{
    return  fmt_ctx;
}


AVRational YoloFFmpeg::DexmuxFFmpeg::getAudioTimebase()
{
    if(!fmt_ctx)return  AVRational{0, 0};
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, (enum AVMediaType)AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)AVMEDIA_TYPE_AUDIO));
        return AVRational{0, 0};
    }
    return fmt_ctx->streams[idx]->time_base;
}

AVRational YoloFFmpeg::DexmuxFFmpeg::getVideoTimebase()
{
    if(!fmt_ctx)return  AVRational{0, 0};
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, (enum AVMediaType)AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)AVMEDIA_TYPE_VIDEO));
        return AVRational{0, 0};
    }
    return fmt_ctx->streams[idx]->time_base;
}

int YoloFFmpeg::DexmuxFFmpeg::getAudioStreamIndex()
{
    if(!fmt_ctx)return -1;
    return  av_find_best_stream(fmt_ctx, (enum AVMediaType)AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

}

int YoloFFmpeg::DexmuxFFmpeg::getVideoStreamIndex()
{
    if(!fmt_ctx)return -1;
    return  av_find_best_stream(fmt_ctx, (enum AVMediaType)AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
}

AVCodecParameters *YoloFFmpeg::DexmuxFFmpeg::getAudioCodecParameters()
{
    if(!fmt_ctx)return nullptr;
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, (enum AVMediaType) AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)AVMEDIA_TYPE_AUDIO));
        return nullptr;
    }
    return fmt_ctx->streams[idx]->codecpar;
}

AVCodecParameters *YoloFFmpeg::DexmuxFFmpeg::getVideoCodecParameters()
{

    if(!fmt_ctx)return nullptr;
    int idx=-1;
    if( (idx=av_find_best_stream(fmt_ctx, (enum AVMediaType) AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))<0){

        fprintf(stderr, "Could not find %s stream in input file  \n", av_get_media_type_string( (enum AVMediaType)AVMEDIA_TYPE_VIDEO));
        return nullptr;
    }
    return fmt_ctx->streams[idx]->codecpar;
}

int YoloFFmpeg::DexmuxFFmpeg::getStreamIndex(YoloFFmpeg::MideaFFmpeg midea)
{
    if(!fmt_ctx)return -1;
    return  av_find_best_stream(fmt_ctx, (enum AVMediaType)midea, -1, -1, NULL, 0);
}



void YoloFFmpeg::SyncTimeFFmpeg::initClock(const YoloFFmpeg::SyncTimeType &syncTimeType)
{
    this->syncTimeType=syncTimeType;
    this->lastPtsTime=0;
    this->lastPtsDrift=0;
    if(this->syncTimeType==External_Master){
          startTime = getMicroseconds() / 1000000.0;
    }else{
        setClock(0);
    }
}

void YoloFFmpeg::SyncTimeFFmpeg::setClock(int64_t ptsTime)
{
    int64_t currentTime = getMicroseconds() / 1000000.0;
    lastPtsDrift=ptsTime-currentTime;
}

int64_t YoloFFmpeg::SyncTimeFFmpeg::getDriftTime()
{
    int64_t resultTime =0;
    int64_t currentTime = getMicroseconds() / 1000000.0;
    if(this->syncTimeType==External_Master){
        resultTime= currentTime -this->startTime;
    }else{
        resultTime =this->lastPtsDrift +currentTime;
    }
    return resultTime;
}

time_t YoloFFmpeg::SyncTimeFFmpeg::getMicroseconds()
{
    std::chrono::system_clock::time_point time_point_new =  std::chrono::system_clock::now();  // 时间一直动
    std::chrono::system_clock::duration duration = time_point_new.time_since_epoch();
    return   std::chrono::duration_cast< std::chrono::microseconds>(duration).count();
}

bool YoloFFmpeg::FramePlus::initFramePlus(YoloFFmpeg::FramePlus **framePlus, AVFrame *frame, AVRational time_base)
{
    if(((*framePlus) = new FramePlus())){
        if( ((*framePlus)->frame= av_frame_alloc())&& frame){
            av_frame_move_ref((*framePlus)->frame,frame);
            if((*framePlus)->frame->pts==AV_NOPTS_VALUE){
              (*framePlus)->pts=0;
            }else{
                   (*framePlus)->pts=frame->pts;
            }

            (*framePlus)->time_base=time_base;
            return true;
        }
    }
    return false;
}

void YoloFFmpeg::FramePlus::freeFramePlus(YoloFFmpeg::FramePlus **framePlus)
{
    if((*framePlus)){
        if((*framePlus)->frame){
            av_frame_free(&((*framePlus)->frame));
        }
    }
}

bool YoloFFmpeg::PacketPlus::initPacketPlus(YoloFFmpeg::PacketPlus **packetPlus, AVPacket *packet, AVRational time_base)
{
    if(((*packetPlus) = new PacketPlus())){
        if( ((*packetPlus)->packet= av_packet_alloc())&& packet){
            av_packet_move_ref((*packetPlus)->packet,packet);
            if((*packetPlus)->packet->pts==AV_NOPTS_VALUE){
                (*packetPlus)->packet->pts=0;
            }

            (*packetPlus)->ptsTime=(*packetPlus)->packet->pts*av_q2d(time_base);
            return true;
        }
    }
    return false;
}

void YoloFFmpeg::PacketPlus::freePacketPlus(YoloFFmpeg::PacketPlus **packetPlus)
{
    if((*packetPlus)){
        if((*packetPlus)->packet){
            av_packet_free(&((*packetPlus)->packet));
        }
    }
}

int YoloFFmpeg::PublicFFmpeg::findCameras()
{
    // 获取可用摄像头列表
        QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
        for(auto camera : cameras)
        {
    #if defined(Q_OS_WIN)
           qDebug("video=%s" , camera.description().toStdString().data());    // ffmpeg在Window下要使用video=description()
    #elif defined(Q_OS_LINUX)
            qDebug(camera.deviceName());                // ffmpeg在linux下要使用deviceName()
    #elif defined(Q_OS_MAC)
    #endif
        }
}

YoloFFmpeg::VideoTranscoder::VideoTranscoder(int dst_width, int dst_height, AVPixelFormat dst_format)
  :__dst_width(dst_width),__dst_height(dst_height),__dst_format(dst_format)
{

}

YoloFFmpeg::VideoTranscoder::~VideoTranscoder()
{
    if(__img_convert_ctx)
    freeVideoTranscoder(__img_convert_ctx);
}
int YoloFFmpeg::VideoTranscoder::VideoTranscodertoImage(uint8_t **dst_data, AVFrame *frame)
{

    if(!frame){
        qDebug( "FFmpegTranscoder::transcoding frame not null\n");
        return -1;
    }
    if(__img_convert_ctx==nullptr){
        if(initVideoTranscoder(&__img_convert_ctx,frame)!=0){
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
    (*dst_data)=__dst_data[0];
    return 0;
}

int YoloFFmpeg::VideoTranscoder::initVideoTranscoder(SwsContext **img_convert_ctx, AVFrame *frame)
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

void YoloFFmpeg::VideoTranscoder::freeVideoTranscoder(SwsContext *img_convert_ctx)
{
    if(img_convert_ctx){
        sws_freeContext(img_convert_ctx);
        img_convert_ctx=nullptr;
    }
     av_freep(&__dst_data[0]);
}
