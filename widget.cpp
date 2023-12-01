#include "widget.h"
#include "ui_widget.h"
#include "AVPacketQueue.h"
#include "AVFrameQueue.h"
#include "DecodecThread.h"
#include "VideoHandler.h"
#include <Synchronized.h>
#include <MultimediaHandler.h>
#include <MainLayout.h>
MainLayout *pMainLayout=nullptr;
Q_DECLARE_METATYPE(AVFrame)  //注册结构体，否则无法通过信号传递AVFrame

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt+ffmpeg视频播放（软解码 + OpenGL显示YUV）Demo V%1").arg(1));

    multimedia.Init();
    canvasWidget = new CanvasWidget;


    multimedia.Start();


// pMainLayout = new MainLayout(this);

//    // 使用QOpenGLWindow绘制

//     footerCanvas = new Canvas;
//#if USE_WINDOW
//  //  ui->verticalLayout->addWidget(QWidget::createWindowContainer(mainCanvas));   // 这一步加载速度要比OpenGLWidget慢一点
// //   ui->verticalLayout->addWidget(QWidget::createWindowContainer(footerCanvas));   // 这一步加载速度要比OpenGLWidget慢一点
//#else



//  //   ui->verticalLayout->addWidget(footerCanvas);


//pMainLayout->addWidget(canvasWidget);
//#endif


    //  connect(multimedia.video_handler, &VideoHandler::repaint, canvasWidget, &CanvasWidget::repaint, Qt::BlockingQueuedConnection);
  //    connect(multimedia.video_handler, &VideoHandler::repaint, footerCanvas, &Canvas::repaint, Qt::BlockingQueuedConnection);

   //
    //  setLayout(pMainLayout);
}

Widget::~Widget()
{
   // delete pMainLayout;
  //  delete edit;
    delete ui;
    //delete pMainLayout;
}



void Widget::on_StartButton_clicked()
{




}
/**
 * @brief        根据视频播放状态切换界面设置
 * @param state
 */
//void Widget::on_playState(ReadThread::PlayState state)
//{
//    if(state == ReadThread::play)
//    {
//        this->setWindowTitle(QString("正在播放：%1").arg(m_readThread->url()));
//      //  ui->but_open->setText("停止播放");
//    }
//    else
//    {
//      //  ui->but_open->setText("开始播放");
//     //   ui->but_pause->setText("暂停");
//        this->setWindowTitle(QString("Qt+ffmpeg视频播放（软解码 + OpenGL显示YUV）Demo "));
//    }
//}

void Widget::on_pushButton_clicked()
{

//    if(state == ReadThread::play)
//    {
//        this->setWindowTitle(QString("正在播放：%1").arg(ffmpegThread->url()));
//      //  ui->but_open->setText("停止播放");
//    }
//    else
//    {
//      //  ui->but_open->setText("开始播放");
//     //   ui->but_pause->setText("暂停");
//        this->setWindowTitle(QString("Qt+ffmpeg视频播放（软解码 + OpenGL显示YUV）Demo "));
//    }
}
