#include "DexmuxRunnable.h"



DexmuxRunnable::DexmuxRunnable( AVPacketQueue *audio_packet_queue, AVPacketQueue *video_packet_queue)
 :__audio_packet_queue(audio_packet_queue),__video_packet_queue(video_packet_queue)
{

}

DexmuxRunnable::~DexmuxRunnable()
{
    this->deInitDexmux();
}



int DexmuxRunnable::startRunnable()
{
    if(pth==nullptr){
        pth= new std::thread(&DexmuxRunnable::Run,this);
        if(!pth){
            qDebug("dexmux_thread::start  failed\n");
            return -1;
        }
    }

    return 0;
}

void DexmuxRunnable::Run()
{
    int ret =-1;
    qInfo("DexmuxThread::Run\n");

     int video_stream_index=this->getStreamIndex(DEXMUX_MIDEA_VIDEO);
     int audio_stream_index=this->getStreamIndex(DEXMUX_MIDEA_AUDIO);
    AVPacket pkt;
    while (abort!=1) {
        //        if(audio_packet_queue->Size() > 100 || video_packet_queue->Size() > 100) {
        //                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //                    continue;
        //                }
        /* read frames from the file */
        if ((ret=this->readFrame(&pkt)) < 0) {
            av_strerror(ret, err2str, sizeof(err2str));
            qDebug("av_read_frame failed, ret:%d, err2str:%s", ret, err2str);
            break;
        }
        if (pkt.stream_index == video_stream_index){
            if(__video_packet_queue){
                  __video_packet_queue->Push(&pkt);
            }


        }else if (pkt.stream_index == audio_stream_index){
            if(__audio_packet_queue){
                  __audio_packet_queue->Push(&pkt);
            }

        }else{
            av_packet_unref(&pkt);
        }

    }
    qInfo("DexmuxThread::Run finish\n");
}
