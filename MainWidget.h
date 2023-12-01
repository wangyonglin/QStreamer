#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include <QtWidgets>
#include <MultimediaHandler.h>
#include <OpenGLWidget.h>
#include <MainLayout.h>

#include <SpecialEffects.h>
#include <QString>
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
    MultimediaHandler multimedia;
    OpenGLWidget * openglWidget = nullptr;

};

#endif // MAINWIDGET_H
