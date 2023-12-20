#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include <QtWidgets>

#include <OpenGLWidget.h>
#include <MainLayout.h>

#include <SpecialEffects.h>
#include <QString>

#include <YoloSystems.h>
#include <YoloPlayer.h>
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
    YoloSystems::AudioPlayer * __audio_player=nullptr;
    YoloSystems::VideoPlayer * __videoPlayer=nullptr;
    YoloPlayer *yoloPlayer=nullptr;

};

#endif // MAINWIDGET_H
