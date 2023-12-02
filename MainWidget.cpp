#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "EffectCreator.h"
Q_DECLARE_METATYPE(AVFrame)  //注册结构体，否则无法通过信号传递AVFrame
EffectCreator *effectCreator;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    layout.setMargin(0); // 控件与窗体的边距
     layout.setSpacing(0); // 控件之间的间距
    multimedia.Init("C:/Users/wangyonglin/Videos/av.mp4");
    openglWidget =  new OpenGLWidget(parent);


    connect(multimedia.video_handler, &VideoHandler::repaint, openglWidget, &OpenGLWidget::repaint, Qt::BlockingQueuedConnection);
    multimedia.Start();

    effectCreator=new EffectCreator(QString("C:/Users/wangyonglin/Videos/av.mov"),this);
 // effectCreator=new EffectCreator(QString("C:/Users/wangyonglin/Videos/002.mp4"),this);
    effectCreator->play();


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


    effectCreator->resize(this->width(), this->height());

}

void MainWidget::moveEvent(QMoveEvent *event)
{
    effectCreator->move(event->pos());

}
