#ifndef OPENGLTHREAD_H
#define OPENGLTHREAD_H
#include <QThread>
#include <QObject>

#include <QElapsedTimer>
#include <QThread>
#include <QTime>
#include <BaseThread.h>
#include <AVFrameQueue.h>
#include <Synchronized.h>
class VideoHandler : public QThread
{
    Q_OBJECT
public:
    explicit VideoHandler(QObject *parent = nullptr);
    ~VideoHandler() override;
    Synchronized *avsync = NULL;
       AVRational time_base;
    int Init(Synchronized *_avsync, AVRational _time_base,AVFrameQueue * frame_queue);
    int Start();
signals:
    void repaint(AVFrame* frame);               // 重绘

protected:
    void run() override;
private:
    AVFrameQueue * frame_queue = NULL;
    void refresh(double *remaining_time);
};

#endif // OPENGLTHREAD_H
