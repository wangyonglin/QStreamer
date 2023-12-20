#include "YoloQueue.h"


YoloQueue::FrameQueue::FrameQueue()
{

}

YoloQueue::FrameQueue::~FrameQueue()
{

}

void YoloQueue::FrameQueue::abortFrame()
{
    releaseFrame();
    queue.Abort();
}

int YoloQueue::FrameQueue::addFramePlus(YoloFFmpeg::FramePlus *framePlus)
{
       return queue.Push(framePlus);
}



int YoloQueue::FrameQueue::getFramePlus( YoloFFmpeg::FramePlus **framePlus, const int timeout)
{
   return queue.Pop((*framePlus),timeout);
}

 YoloFFmpeg::FramePlus *YoloQueue::FrameQueue::frontFramePlus()
{
      YoloFFmpeg::FramePlus * framePlus =nullptr;
    int ret  = queue.Front(framePlus);
    if(ret <0){
        return nullptr;
    }
    return framePlus;
}

int YoloQueue::FrameQueue::getFrametPlusSize()
{
    return queue.Size();
}

void YoloQueue::FrameQueue::releaseFrame()
{
    while (true) {
           YoloFFmpeg::FramePlus * framePlus =nullptr;
        int ret = queue.Pop(framePlus,1);
        if(ret<0){
            break;
        }else
        {
           // av_frame_free(&frame);
            YoloFFmpeg::FramePlus::freeFramePlus(&framePlus);
            continue;
        }
    }
}




YoloQueue::PacketQueue::PacketQueue()
{

}
YoloQueue::PacketQueue::~PacketQueue()
{

}

int YoloQueue::PacketQueue::addPacketPlus(AVPacket *packet, AVRational time_base)
{
   YoloFFmpeg::PacketPlus * packetPlus=nullptr;
   YoloFFmpeg::PacketPlus::initPacketPlus(&packetPlus,packet,time_base);
    return queue.Push(packetPlus);
}
void YoloQueue::PacketQueue::abortPacketPlus()
{
    releasePacketPlus();
    queue.Abort();
}

void YoloQueue::PacketQueue::releasePacketPlus()
{
    while (true) {
        YoloFFmpeg::PacketPlus * packetPlus=nullptr;
        int ret = queue.Pop(packetPlus,1);
        if(ret<0){
            break;
        }else
        {
           // av_packet_free(&pkt);
              YoloFFmpeg::PacketPlus::freePacketPlus(&packetPlus);
            continue;
        }
    }
}

int YoloQueue::PacketQueue::getPacketPlusSize()
{
    return queue.Size();
}



 int YoloQueue::PacketQueue::getPacketPlus( YoloFFmpeg::PacketPlus ** packetPlus,const int timeout)
{
    return queue.Pop((* packetPlus),timeout);

}






