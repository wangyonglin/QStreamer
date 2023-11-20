#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include <FFmpegThread.h>
//#include <Canvas.h>
//#include <readthread.h>
//#include <videodecode.h>
//#include <DexmuxThread.h>
//#include <DecodecThread.h>
//#include "VideoHandler.h"
//#include <AudioHandler.h>
  #include <MultimediaHandler.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_StartButton_clicked();
     //  void on_playState(ReadThread::PlayState state);
       void on_pushButton_clicked();


private:
    Ui::Widget *ui;
       MultimediaHandler multimedia;
    Canvas * mainCanvas = nullptr;

     Canvas * footerCanvas = nullptr;
//    ReadThread* m_readThread = nullptr;
//    DexmuxThread *dexmuxThread = nullptr;
//    DecodecThread *audio_decodec_thread= nullptr;
//    DecodecThread *video_decodec_thread= nullptr;
//    VideoHandler * video_handler= nullptr;
//    AudioHandler* audio_handler =nullptr;

};
#endif // WIDGET_H
