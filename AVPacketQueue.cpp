#include "AVPacketQueue.h"

AVPacketQueue::AVPacketQueue()
{

}

AVPacketQueue::~AVPacketQueue()
{

}

void AVPacketQueue::Abort()
{
    Release();
    queue.Abort();
}

void AVPacketQueue::Release()
{
    while (true) {
        AVPacket*pkt=nullptr;
        int ret = queue.Pop(pkt,1);
        if(ret<0){
            break;
        }else
        {
            av_packet_free(&pkt);
            continue;
        }
    }
}

int AVPacketQueue::Size()
{
    return queue.Size();
}

int AVPacketQueue::Push(AVPacket *src_pkt)
{
    AVPacket * dst_pkt = av_packet_alloc();
    if(!dst_pkt){
        return -1;
    }
    av_packet_move_ref(dst_pkt,src_pkt);
    return queue.Push(dst_pkt);
}

AVPacket *AVPacketQueue::Pop(const int timeout)
{
    AVPacket * dst_pkt= NULL;
    int ret =  queue.Pop(dst_pkt,timeout);
    if(ret <0){
        return dst_pkt;
    }
    return dst_pkt;
}
