#include "FFmpegPublic.h"
#include <QDebug>
#include <QCameraInfo>
void FFmpegPublic::SyncTime::initClock(const FFmpegPublic::ClockType &clockType)
{
    this->__clockType=clockType;
    this->lastPtsTime=0;
    this->lastPtsDrift=0;
    if(this->__clockType==External_Master){
          startTime = getCurrentMicroseconds() / 1000000.0;
    }else{
        setClock(0);
    }
}

void FFmpegPublic::SyncTime::setClock(int64_t ptstime)
{
    int64_t currentTime = getCurrentMicroseconds() / 1000000.0;
    lastPtsDrift=ptstime-currentTime;
}

int64_t FFmpegPublic::SyncTime::getDriftTime()
{
    int64_t resultTime =0;
    int64_t currentTime = getCurrentMicroseconds() / 1000000.0;
    if(this->__clockType==External_Master){
        resultTime= currentTime - startTime;
    }else{
        resultTime =this->lastPtsDrift +currentTime;
    }
    return resultTime;
}

time_t FFmpegPublic::SyncTime::getCurrentMicroseconds()
{
    std::chrono::system_clock::time_point time_point_new =  std::chrono::system_clock::now();  // 时间一直动
    std::chrono::system_clock::duration duration = time_point_new.time_since_epoch();
    return   std::chrono::duration_cast< std::chrono::microseconds>(duration).count();
}

FFmpegPublic::VideoTranscoder::VideoTranscoder(int dst_width, int dst_height, AVPixelFormat dst_format)
 :__dst_width(dst_width),__dst_height(dst_height),__dst_format(dst_format)
{

}

FFmpegPublic::VideoTranscoder::~VideoTranscoder()
{
    if(__img_convert_ctx)
    freeVideoTranscoder(__img_convert_ctx);
}

int FFmpegPublic::VideoTranscoder::VideoTranscoderConvert(uint8_t **dst_data, AVFrame *frame)
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

int FFmpegPublic::VideoTranscoder::initVideoTranscoder(SwsContext **img_convert_ctx, AVFrame *frame)
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

void FFmpegPublic::VideoTranscoder::freeVideoTranscoder(SwsContext *img_convert_ctx)
{
    if(img_convert_ctx){
        sws_freeContext(img_convert_ctx);
        img_convert_ctx=nullptr;
    }
     av_freep(&__dst_data[0]);
}

void FFmpegPublic::CameraFFmpeg::findCameras()
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


FFmpegPublic::Queue::Frame::Frame()
{

}

FFmpegPublic::Queue::Frame::~Frame()
{

}

void FFmpegPublic::Queue::Frame::abortFrame()
{
    releaseFrame();
    queue.Abort();
}

int FFmpegPublic::Queue::Frame::addFramePlus(FFmpegPublic::FramePlus *framePlus)
{
       return queue.Push(framePlus);
}



int FFmpegPublic::Queue::Frame::getFramePlus( FFmpegPublic::FramePlus **framePlus, const int timeout)
{
    return queue.Pop((*framePlus),timeout);
}

FFmpegPublic::FramePlus *FFmpegPublic::Queue::Frame::frontFramePlus()
{
    FFmpegPublic::FramePlus * framePlus =nullptr;
    int ret  = queue.Front(framePlus);
    if(ret <0){
        return nullptr;
    }
    return framePlus;
}



int FFmpegPublic::Queue::Frame::getFrametPlusSize()
{
    return queue.Size();
}

void FFmpegPublic::Queue::Frame::releaseFrame()
{
    while (true) {
           FFmpegPublic::FramePlus * framePlus =nullptr;
        int ret = queue.Pop(framePlus,1);
        if(ret<0){
            break;
        }else
        {
           // av_frame_free(&frame);
            FFmpegPublic::FramePlus::freeFramePlus(&framePlus);
            continue;
        }
    }
}




FFmpegPublic::Queue::Packet::Packet()
{

}
FFmpegPublic::Queue::Packet::~Packet()
{

}

int FFmpegPublic::Queue::Packet::addPacketPlus(AVPacket *packet, AVRational time_base)
{
   FFmpegPublic::PacketPlus * packetPlus=nullptr;
   FFmpegPublic::PacketPlus::initPacketPlus(&packetPlus,packet,time_base);
    return queue.Push(packetPlus);
}
void FFmpegPublic::Queue::Packet::abortPacketPlus()
{
    releasePacketPlus();
    queue.Abort();
}

void FFmpegPublic::Queue::Packet::releasePacketPlus()
{
    while (true) {
        FFmpegPublic::PacketPlus * packetPlus=nullptr;
        int ret = queue.Pop(packetPlus,1);
        if(ret<0){
            break;
        }else
        {
           // av_packet_free(&pkt);
              FFmpegPublic::PacketPlus::freePacketPlus(&packetPlus);
            continue;
        }
    }
}

int FFmpegPublic::Queue::Packet::getPacketPlusSize()
{
    return queue.Size();
}



 int FFmpegPublic::Queue::Packet::getPacketPlus( FFmpegPublic::PacketPlus ** packetPlus,const int timeout)
{
    return queue.Pop((* packetPlus),timeout);

}



 bool FFmpegPublic::FramePlus::initFramePlus(FFmpegPublic::FramePlus **framePlus, AVFrame *frame, AVRational time_base)
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

 void FFmpegPublic::FramePlus::freeFramePlus(FFmpegPublic::FramePlus **framePlus)
 {
     if((*framePlus)){
         if((*framePlus)->frame){
             av_frame_free(&((*framePlus)->frame));
         }
     }
 }

 bool FFmpegPublic::PacketPlus::initPacketPlus(FFmpegPublic::PacketPlus **packetPlus, AVPacket *packet, AVRational time_base)
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

 void FFmpegPublic::PacketPlus::freePacketPlus(FFmpegPublic::PacketPlus **packetPlus)
 {
     if((*packetPlus)){
         if((*packetPlus)->packet){
             av_packet_free(&((*packetPlus)->packet));
         }
     }
 }




 int FFmpegPublic::Funcation::SendPacketPlus(AVCodecContext * codec_ctx,FFmpegPublic::PacketPlus *packetPlus)
 {
     if(codec_ctx && packetPlus){
         return   avcodec_send_packet(codec_ctx,packetPlus->packet);
     }
     return -1;
 }

 int FFmpegPublic::Funcation::ReceiveFramePlus(AVCodecContext * codec_ctx,FFmpegPublic::FramePlus **framePlus)
 {
     int ret = -1;
     if(!codec_ctx )return -1;
         AVFrame * frame = av_frame_alloc();
         if(frame){
             if((ret=avcodec_receive_frame(codec_ctx,frame))==0){
                   FFmpegPublic::FramePlus::initFramePlus(framePlus,frame,codec_ctx->time_base);
             }
             av_frame_free(&frame);
             return ret;
         }
     return -1;
 }

