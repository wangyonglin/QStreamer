#include "DecodecThread.h"
#include <thread>
#include <QDebug>

DecodecThread::DecodecThread(AVPacketQueue *packet_queue, AVFrameQueue *frame_queue)
    :packet_queue(packet_queue),frame_queue(frame_queue)
{


}

DecodecThread::~DecodecThread()
{
    if(pth){
        Stop();
    }
    avcodec_close(codec_ctx);
}

int DecodecThread::Init(AVCodecParameters *codec_param)
{

    qInfo("DecodecThread::Init(...) init");
    if(!codec_param)return -1;

    codec_ctx= avcodec_alloc_context3(NULL);
    int ret = avcodec_parameters_to_context(codec_ctx,codec_param);
    if(ret <0){
        av_strerror(ret,err2str,sizeof (err2str));
        qDebug("avcodec_parameters_to_context failed, ret%d,err2str:%s",ret,err2str);
        return -1;
    }

    const AVCodec *codec = avcodec_find_decoder(codec_ctx->codec_id);
    if(!codec){
        qDebug("avcodec_find_decoder failed");
        return -1;
    }

    /* Init the decoders */
    if ((ret = avcodec_open2(codec_ctx, codec, NULL)) < 0) {
        av_strerror(ret,err2str,sizeof (err2str));
        qDebug("avcodec_open2 failed, ret%d,err2str:%s",ret,err2str);
        return -1;
    }

    qInfo("DecodecThread::Init(...) finish");
    return ret;
}

int DecodecThread::Start()
{
    if(!pth){
        pth = new std::thread(&DecodecThread::Run,this);
        if(!pth){
            qDebug("DecodecThread::Start() failed");
            return -1;
        }

        if(!( frame= av_frame_alloc())){
            qDebug("DecodecThread::Start() failed");
            return -2;
        }
    }

    return 0;
}

int DecodecThread::Stop()
{   if(pth){
        abort=1;
        packet_queue->Release();
        BaseThread::Stop();
        if(frame){
            av_frame_free(&frame);
        }

    }

    return 0;
}

void DecodecThread::Run()
{

    while (abort != 1) {

        AVPacket * pkt = packet_queue->Pop(10);
        if(pkt){
            int ret = avcodec_send_packet(codec_ctx,pkt);
            av_packet_free(&pkt);
            if(ret <0){
                break;
            }

            while (true) {

                if(frame){
                    ret = avcodec_receive_frame(codec_ctx,frame);
                    if(ret == 0){
                        frame_queue->Push(frame);
                        continue;
                    }else if(AVERROR(EAGAIN)==ret){
                        break;
                    }else{

                        abort =1;
                        break;
                    }

                }

            }

        }else{
            qDebug("not packet data");
            break;
        }

    }




}




