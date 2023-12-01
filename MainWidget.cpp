#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "EffectCreator.h"
Q_DECLARE_METATYPE(AVFrame)  //注册结构体，否则无法通过信号传递AVFrame
EffectCreator *effectCreator;
EffectCreator *effectCreatord;
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    multimedia.Init("C:/Users/wangyonglin/Videos/298998_video.mp4");
    openglWidget =  new OpenGLWidget(parent);


    connect(multimedia.video_handler, &VideoHandler::repaint, openglWidget, &OpenGLWidget::repaint, Qt::BlockingQueuedConnection);
    multimedia.Start();

    effectCreator=new EffectCreator(QString("C:/Users/wangyonglin/Videos/298998_video.mp4"),this);
    effectCreator->show();



    layout.addWidget(openglWidget);
    setLayout(&layout);




}

MainWidget::~MainWidget()
{
    delete effectCreator;
    delete ui;
}


void MainWidget::resizeEvent(QResizeEvent *event)
{


    effectCreator->resize(this->width()/2, this->height()/2);

}

void MainWidget::moveEvent(QMoveEvent *event)
{
    effectCreator->move(event->pos());

}
