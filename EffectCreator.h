#ifndef EFFECTDISPLAY_H
#define EFFECTDISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QFont>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <FFmpegPublic.h>
#include <FFmpegOutput.h>



class EffectCreator : public FFmpegOutput::VideoPlayer
{

public:
    explicit EffectCreator(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~EffectCreator();
public:

//    void repaint(AVFrame *frame);
//    void play();
//    void stop();
//    virtual void paintEvent(QPaintEvent *event);
//    virtual void resizeEvent(QResizeEvent *event);

private:
//    QString     __url;
//    FFmpegPublic::VideoTranscoder * __pVideoTranscoder=nullptr;
//    int __image_width;
//    int __image_height;
//    uint8_t * __dst_data[4];



signals:

};

#endif // EFFECTDISPLAY_H
