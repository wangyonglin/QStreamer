#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "EffectCreator.h"


//Q_DECLARE_METATYPE(AVFrame)  //注册结构体，否则无法通过信号传递AVFrame
EffectCreator *effectCreator;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    layout.setMargin(0); // 控件与窗体的边距
    layout.setSpacing(0); // 控件之间的间距
    YoloSystems::AudioPlayer::initAudioPlayer();
    __audio_player= new  YoloSystems::AudioPlayer(44100,AUDIO_S16SYS,2);

    __audio_player->open();

    __videoPlayer =new  YoloSystems::VideoPlayer();
    yoloPlayer=new YoloPlayer();
    yoloPlayer->initYoloPlayer("C:/Users/wangyonglin/Videos/av.mp4",__audio_player,__videoPlayer);



   layout.addWidget(__videoPlayer);

    setLayout(&layout);




}

MainWidget::~MainWidget()
{
    delete __videoPlayer;
    delete ui;
}


void MainWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
  //  effectCreator->resize(this->width(), this->height());

}

void MainWidget::moveEvent(QMoveEvent *event)
{
     Q_UNUSED(event)
   // effectCreator->move(event->pos());

}
