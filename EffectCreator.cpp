#include "EffectCreator.h"

EffectCreator::EffectCreator(const QString &url,QWidget *parent)
    :QWidget(parent),__url(url)
{

    setAttribute(Qt::WA_TranslucentBackground); //背景透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    multimedia.Init(__url.toStdString().data());
    __image_width=multimedia.width();
    __image_height=multimedia.height();
   __transcodeUtils= new TranscodeUtils(__image_width,__image_height,AV_PIX_FMT_RGB32);
    connect(multimedia.video_handler, &VideoHandler::repaint, this, &EffectCreator::repaint, Qt::BlockingQueuedConnection);
    multimedia.Start();
}

EffectCreator::~EffectCreator()
{
    delete __transcodeUtils;
}

void EffectCreator::repaint(AVFrame *frame)
{
    if(!frame || frame->width == 0 || frame->height == 0)return;
    if(__transcodeUtils->transcoding(__dst_data,frame)==0){
     update();
    }
    av_frame_unref(frame);

}

void EffectCreator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QImage image(__dst_data[0], __image_width, __image_height, QImage::Format_RGB32);
    painter.drawImage(rect(), image);

}

void EffectCreator::resizeEvent(QResizeEvent *event)
{
    resize(this->width(),this->height());
}

