#include "YoloThread.h"

YoloThread::DexmuxThread::DexmuxThread(YoloQueue::PacketQueue* audioPacketQueue,
                                       YoloQueue::PacketQueue* videoPacketQueue)
    :__audioPacketQueue(audioPacketQueue),
      __videoPacketQueue(videoPacketQueue)
{

}


YoloThread::DexmuxThread::~DexmuxThread()
{

}


void YoloThread::DexmuxThread::freeThread()
{
    this->freeDexmuxFFmpeg();
}

void YoloThread::DexmuxThread::startThread(const std::string &url)
{
    initDexmuxFFmpeg(url);
     startRunnable(&YoloThread::DexmuxThread::Runnable,this);
}

void YoloThread::DexmuxThread::stopThread()
{
     stopRunnable();
}

void YoloThread::DexmuxThread::Runnable()
{
    qInfo("Run into");
       int ret = 0;

       AVPacket pkt;

       while (abort != 1) {

//           if(__audioPacketQueue->getPacketSize() > 100 || __videoPacketQueue->getPacketSize() > 100) {
//               std::this_thread::sleep_for(std::chrono::milliseconds(10));
//               continue;
//           }

           ret = av_read_frame(getFormatContext(), &pkt);
           if(ret < 0) {
               av_strerror(ret, err2str, sizeof(err2str));
               qDebug("av_read_frame failed, ret:%d, err2str:%s", ret, err2str);
               break;
           }
           if(pkt.stream_index == getAudioStreamIndex()) {

               ret = __audioPacketQueue->addPacketPlus(&pkt,getAudioTimebase());
               av_packet_unref(&pkt);
               qInfo("audio pkt queue size:%d", __audioPacketQueue->getPacketPlusSize());
           } else if(pkt.stream_index == getVideoStreamIndex()) {

               ret = __videoPacketQueue->addPacketPlus(&pkt,getVideoTimebase());
               av_packet_unref(&pkt);
               qInfo("video pkt queue size:%d", __videoPacketQueue->getPacketPlusSize());
           } else {
               av_packet_unref(&pkt);
           }
       }
       qInfo("Run finish");
}



YoloThread::DecodecThread::DecodecThread(YoloQueue::PacketQueue* packetQueue,YoloQueue::FrameQueue * frameQueue)
    :__packetQueue(packetQueue),__frameQueue(frameQueue)
{

}

YoloThread::DecodecThread::~DecodecThread()
{

}

void YoloThread::DecodecThread::startThread(AVCodecParameters *codecParameters)
{
    initDecodecFFmpeg(codecParameters);
     startRunnable(&YoloThread::DecodecThread::Runnable,this);
}

void YoloThread::DecodecThread::stopThread()
{
    stopRunnable();
}



void YoloThread::DecodecThread::Runnable()
{

    while (abort != 1) {
//        if(packetList->Size() > 10) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(10));
//            continue;
//        }

        YoloFFmpeg::PacketPlus *packetPlus=nullptr;
        if(__packetQueue->getPacketPlus(&packetPlus,10)==0){
            int ret = SendPacketPlus(packetPlus);
            YoloFFmpeg::PacketPlus::freePacketPlus(&packetPlus);
            if(ret < 0) {
                av_strerror(ret, err2str, sizeof(err2str));
                qDebug() << "avcodec_send_packet failed ,err2str:" << err2str;
                break;
            }

            // 读取解码后的frame
            while (ret >= 0) {
                YoloFFmpeg::FramePlus * framePlus=nullptr;
                ret = ReceiveFramePlus(&framePlus);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                    break;
                else if (ret < 0) {
                    qDebug() << "Error during decoding";
                    break;
                }
                if(ret == 0) {
                    __frameQueue->addFramePlus(framePlus);
                    continue;
                }
            }

        }/*else{
            qDebug() << "not packet data";
        }*/

    }

}
