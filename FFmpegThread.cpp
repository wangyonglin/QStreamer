#include "FFmpegThread.h"

FFmpegThread::DexmuxThread::DexmuxThread(FFmpegPublic::Queue::Packet* audioPacketQueue,
                                       FFmpegPublic::Queue::Packet* videoPacketQueue)
    :__audioPacketQueue(audioPacketQueue),
      __videoPacketQueue(videoPacketQueue)
{

}


FFmpegThread::DexmuxThread::~DexmuxThread()
{

}


void FFmpegThread::DexmuxThread::freeThread()
{
    this->freeDexmuxFFmpeg();
}

void FFmpegThread::DexmuxThread::startThread(const std::string &url)
{
    initDexmuxFFmpeg(url);
     startRunnable(&FFmpegThread::DexmuxThread::entityRunnable,this);
}

void FFmpegThread::DexmuxThread::stopThread()
{
    stopRunnable();
}



void FFmpegThread::DexmuxThread::entityRunnable()
{
    qInfo("Run into");
       int ret = 0;

       AVPacket pkt;

       while (abort != 1) {

           if(__audioPacketQueue->getPacketPlusSize() > 100 || __videoPacketQueue->getPacketPlusSize() > 100) {
               std::this_thread::sleep_for(std::chrono::milliseconds(10));
               continue;
           }

           ret = av_read_frame(getFormatContext(), &pkt);
           if(ret < 0) {
               av_strerror(ret, err2str, sizeof(err2str));
               qDebug("av_read_frame failed, ret:%d, err2str:%s", ret, err2str);
               break;
           }
           if(pkt.stream_index == getAudioStreamIndex()) {

               ret = __audioPacketQueue->addPacketPlus(&pkt,getAudioTimebase());
               av_packet_unref(&pkt);
             //  qInfo("audio pkt queue size:%d", __audioPacketQueue->getPacketPlusSize());
           } else if(pkt.stream_index == getVideoStreamIndex()) {

               ret = __videoPacketQueue->addPacketPlus(&pkt,getVideoTimebase());
               av_packet_unref(&pkt);
            //   qInfo("video pkt queue size:%d", __videoPacketQueue->getPacketPlusSize());
           } else {
               av_packet_unref(&pkt);
           }
       }
       qInfo("Run finish");
}



FFmpegThread::DecodecThread::DecodecThread(FFmpegPublic::Queue::Packet* packetQueue,FFmpegPublic::Queue::Frame * frameQueue)
    :__packetQueue(packetQueue),__frameQueue(frameQueue)
{

}

FFmpegThread::DecodecThread::~DecodecThread()
{

}

void FFmpegThread::DecodecThread::startThread(AVCodecParameters *codecParameters)
{
    initDecodecFFmpeg(codecParameters);
     startRunnable(&FFmpegThread::DecodecThread::entityRunnable,this);
}

void FFmpegThread::DecodecThread::stopThread()
{
    stopRunnable();
}



void FFmpegThread::DecodecThread::entityRunnable()
{

    while (abort != 1) {
//        if(packetList->Size() > 10) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(10));
//            continue;
//        }

        FFmpegPublic::PacketPlus *packetPlus=nullptr;
        if(__packetQueue->getPacketPlus(&packetPlus,10)==0){
            int ret = FFmpegPublic::Funcation::SendPacketPlus(getCodeContext(),packetPlus);
            FFmpegPublic::PacketPlus::freePacketPlus(&packetPlus);
            if(ret < 0) {
                av_strerror(ret, err2str, sizeof(err2str));
                qDebug() << "avcodec_send_packet failed ,err2str:" << err2str;
                break;
            }

            // 读取解码后的frame
            while (ret >= 0) {
                FFmpegPublic::FramePlus * framePlus=nullptr;
                ret = FFmpegPublic::Funcation::ReceiveFramePlus(getCodeContext(),&framePlus);
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
