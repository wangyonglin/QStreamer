#include "VideoHandler.h"
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <qimage.h>
#include <thread>
#include <thread>
#include <string>
VideoHandler::VideoHandler(QObject *parent): QThread(parent)
{

}

VideoHandler::~VideoHandler()
{

}

int VideoHandler::Init(Synchronized *_avsync, AVRational _time_base,AVFrameQueue *_frame_queue)
{
    frame_queue =_frame_queue;
    avsync=_avsync;
    time_base=_time_base;
    return 0;
}

int VideoHandler::Start()
{

       emit this->start();

    return 0;
}
#define REFRESH_RATE 0.01
void VideoHandler::run()
{
    double remaining_time = 0.00;
    while (true) {
        if (remaining_time > 0.0)
                   std::this_thread::sleep_for(std::chrono::milliseconds(int64_t(remaining_time * 1000.0)));
               remaining_time = REFRESH_RATE;
               // 尝试刷新画面
               refresh(&remaining_time);
    }

}
// 0.01秒
void VideoHandler::refresh(double *remaining_time)
{
    AVFrame *frame = NULL;
    frame = frame_queue->Front();
    if(frame) {
        double pts = frame->pts * av_q2d(time_base);

        qInfo("video pts:%0.3lf\n", pts);

        double diff = pts - avsync->GetClock();

        if(diff > 0) {
            *remaining_time = FFMIN(*remaining_time, diff);
            return;
        }


        frame = frame_queue->Pop(1);
        emit repaint(frame);
       av_frame_free(&frame);
    }
}
