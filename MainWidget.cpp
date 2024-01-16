#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "EffectCreator.h"


//Q_DECLARE_METATYPE(AVFrame)  //注册结构体，否则无法通过信号传递AVFrame
FFmpegPlayer * pMainPlayer;
FFmpegPlayer * pEffectPlayer;
FFmpegOutput::VideoPlayer *pMainVideoPlayer;
FFmpegOutput::VideoPlayer *pEffectVideoPlayer;
FFmpegOutput::AudioPlayer *pAudioPlayer;


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    // layout.setMargin(0); // 控件与窗体的边距
    // layout.setSpacing(0); // 控件之间的间距

    FFmpegOutput::AudioPlayer::initAudioPlayer();
    pAudioPlayer= new  FFmpegOutput::AudioPlayer(44100,AUDIO_S16SYS,2);
    pAudioPlayer->openAudioPlayer();




    pMainPlayer=new FFmpegPlayer(nullptr,"C:/Users/wangyonglin/Videos/av.mp4");
    pMainPlayer->initAudioPlayer(pAudioPlayer);
    pMainVideoPlayer= new FFmpegOutput::VideoPlayer(this,width(),height(),false);
    pMainPlayer->initVideoPlayer(pMainVideoPlayer);
    pEffectPlayer=new FFmpegPlayer(nullptr,"C:/Users/wangyonglin/Videos/av.mov");
    pEffectPlayer->initAudioPlayer(pAudioPlayer);
    pEffectVideoPlayer= new FFmpegOutput::VideoPlayer(pMainVideoPlayer,width(),height(),true);
    pEffectPlayer->initVideoPlayer(pEffectVideoPlayer);


}

MainWidget::~MainWidget()
{
    delete ui;
}


void MainWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    pMainVideoPlayer->resize(width(), height());
    pEffectVideoPlayer->resize(width(), height());

}

void MainWidget::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event)

}
