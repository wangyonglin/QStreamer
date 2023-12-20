#include "EffectCreator.h"
Q_DECLARE_METATYPE(AVFrame)  //注册结构体，否则无法通过信号传递AVFrame
EffectCreator::EffectCreator(const QString &url,QWidget *parent)
    :QWidget(parent),__url(url)
{

    setAttribute(Qt::WA_TranslucentBackground); //背景透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
//    yoloMultimedia= new YoloMultimedia(this);
//    yoloMultimedia->initDexmuxRunnable(__url.toStdString().data());
//    __image_width=yoloMultimedia->width();
//    __image_height=yoloMultimedia->height();

//    __video_transcoder= new FFmpegTranscoder::VideoTranscoder(__image_width,__image_height,AV_PIX_FMT_RGBA);
//    yoloMultimedia->initVideoRunnable(this);

}

EffectCreator::~EffectCreator()
{
    delete __video_transcoder;
}

void EffectCreator::repaint(AVFrame *frame)
{
    if(!frame || frame->width == 0 || frame->height == 0)return;
    if(__video_transcoder->convertFrame(__dst_data,frame)==0){
     update();
    }
    av_frame_unref(frame);

}

void EffectCreator::play()
{

     show();
}

void EffectCreator::stop()
{
    hide();

}

void EffectCreator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QImage image(__dst_data[0], __image_width, __image_height, QImage::Format_RGBA8888);
    painter.drawImage(rect(), image);

}

void EffectCreator::resizeEvent(QResizeEvent *event)
{
     Q_UNUSED(event)
    resize(this->width(),this->height());
}

