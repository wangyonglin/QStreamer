#include "AVFrameQueue.h"

AVFrameQueue::AVFrameQueue()
{

}

AVFrameQueue::~AVFrameQueue()
{

}

void AVFrameQueue::Abort()
{
    Release();
    queue.Abort();
}

int AVFrameQueue::Push(AVFrame *frame)
{
    AVFrame *tmpFrame = av_frame_alloc();
    av_frame_move_ref(tmpFrame,frame);
    return queue.Push(tmpFrame);
}

AVFrame *AVFrameQueue::Pop(const int timeout)
{
    AVFrame *tmpFrame =nullptr;
    int ret  = queue.Pop(tmpFrame,timeout);
    if(ret <0){
        return nullptr;
    }
    return tmpFrame;
}

AVFrame *AVFrameQueue::Front()
{
    AVFrame *tmpFrame =nullptr;
    int ret  = queue.Front(tmpFrame);
    if(ret <0){
        return nullptr;
    }
    return tmpFrame;
}

int AVFrameQueue::Size()
{
    return queue.Size();
}

void AVFrameQueue::Release()
{
    while (true) {
        AVFrame*frame=nullptr;
        int ret = queue.Pop(frame,1);
        if(ret<0){
            break;
        }else
        {
            av_frame_free(&frame);
            continue;
        }
    }
}
