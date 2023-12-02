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
#include <MultimediaHandler.h>
#include <OpenGLWidget.h>
#include <TranscodeUtils.h>

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
    TranscodeUtils * __transcodeUtils = nullptr;
    int __image_width;
    int __image_height;
    uint8_t * __dst_data[4];
    MultimediaHandler multimedia;


signals:

};

#endif // EFFECTDISPLAY_H
