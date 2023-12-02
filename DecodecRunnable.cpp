#include "DecodecRunnable.h"


DecodecRunnable::DecodecRunnable(AVPacketQueue *packet_queue, AVFrameQueue *frame_queue)
      :__packet_queue(packet_queue),__frame_queue(frame_queue)
{

}

DecodecRunnable::~DecodecRunnable()
{
    if(this->pRunnableBase){
        stopDecodec();
    }
    this->deInitDecodec();
}

int DecodecRunnable::startDecodec()
{
    if(!this->pRunnableBase){

        if(!this->startRunnable(&DecodecRunnable::Runnable,this)){
            qDebug("DecodecRunnable::startDecodec() startRunnable failed");
            return -1;
        }


        if(!( frame= av_frame_alloc())){
            qDebug("DecodecRunnable::startDecodec() av_frame_alloc failed");
            return -2;
        }
    }

    return 0;
}

void DecodecRunnable::stopDecodec()
{
    if(this->pRunnableBase){
            abort=1;
            __packet_queue->Release();
           this->stopRunnable();
            if(frame){
                av_frame_free(&frame);
            }

        }
}

void DecodecRunnable::Runnable()
{
    qInfo("#########################");
    while (abort != 1) {

        AVPacket * pkt = __packet_queue->Pop(10);
        if(pkt){
            int ret =this->SendPacket(pkt);
            av_packet_free(&pkt);
            if(ret <0){
                break;
            }

            while (true) {

                if(frame){
                    ret = this->ReceiveFrame(frame);
                    if(ret == 0){
                        __frame_queue->Push(frame);
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

