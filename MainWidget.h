#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include <QtWidgets>

#include <OpenGLWidget.h>
#include <MainLayout.h>

#include <SpecialEffects.h>
#include <QString>

#include <FFmpegOutput.h>
#include <FFmpegPlayer.h>
namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    virtual void resizeEvent(QResizeEvent *event) ;
    virtual void moveEvent(QMoveEvent *event) ;
private:
    Ui::MainWidget *ui;
    QHBoxLayout layout;
    FFmpegOutput::AudioPlayer * __audio_player=nullptr;
    FFmpegOutput::VideoPlayer * __videoPlayer=nullptr;
    FFmpegPlayer *__main_player=nullptr;
     FFmpegPlayer *__effect_player=nullptr;
       FFmpegOutput::VideoPlayer *__pVideoPlayer=nullptr;

};

#endif // MAINWIDGET_H
