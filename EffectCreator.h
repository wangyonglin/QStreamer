#ifndef EFFECTDISPLAY_H
#define EFFECTDISPLAY_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>

#include <OpenGLWidget.h>
#include <FFmpegTranscoder.h>

class EffectCreator : public QWidget
{

public:
    explicit EffectCreator(const QString &url, QWidget *parent = nullptr);
    ~EffectCreator();
public:
    void repaint(AVFrame *frame);
    void play();
    void stop();
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    QString     __url;
    FFmpegTranscoder::VideoTranscoder * __video_transcoder = nullptr;
    int __image_width;
    int __image_height;
    uint8_t * __dst_data[4];



signals:

};

#endif // EFFECTDISPLAY_H
