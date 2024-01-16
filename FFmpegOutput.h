#ifndef YOLOAUDIOPLAYER_H
#define YOLOAUDIOPLAYER_H
#include <SDL.h>
#include <SDL_audio.h>
#include <iostream>
#include <QWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <qopenglshaderprogram.h>
#include <QOpenGLTexture>
#include <qopenglpixeltransferoptions.h>
#define USE_WINDOW 0    // 1:使用QOpenGLWindow显示, 0：使用QOpenGLWidget显示
#include <QOpenGLWidget>
#include <FFmpegPublic.h>
#include <QPainter>
struct AVFrame;

namespace FFmpegOutput {
class AudioPlayer
{


public:
    AudioPlayer( int sample_rate,  SDL_AudioFormat sample_fmt,int channels);
    ~AudioPlayer();
    static int initAudioPlayer();
    static void exitAudioPlayer();
    int openAudioPlayer();

    int addQueueAudioValue(const void *data, Uint32 len,bool waitTime);
    void pause(int time);
    int closeAudioPlayer();

    Uint32 getQueuedAudioSize();
    SDL_AudioSpec  getObtained();
    SDL_AudioSpec obtained;
public:

private:
    SDL_AudioDeviceID __deviceId;
    int __sample_rate; //采样率
    int __channels;
    int64_t __channel_layout;
    SDL_AudioFormat __sample_fmt;

    SDL_AudioSpec __desired;
};
}

//namespace FFmpegOutput {



//#if USE_WINDOW
//#include <QOpenGLWindow>
//class VideoPlayer : public QOpenGLWindow, public  QOpenGLFunctions_3_3_Core
//        #else

//class VideoPlayer : public QOpenGLWidget, public  QOpenGLFunctions_3_3_Core
//        #endif
//{
//    Q_OBJECT
//public:
//#if USE_WINDOW
//    explicit VideoPlayer(UpdateBehavior updateBehavior = NoPartialUpdate, QWindow *parent = nullptr);
//#else
//    explicit VideoPlayer(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
//#endif
//    ~VideoPlayer() override;

//    virtual  void repaint(AVFrame* frame);             // 重绘


//protected:
//    void initializeGL() override;               // 初始化gl
//    void resizeGL(int w, int h) override;       // 窗口尺寸变化
//    void paintGL() override;                    // 刷新显示

//private:
//    QOpenGLShaderProgram* m_program = nullptr;
//    QOpenGLTexture* m_texY = nullptr;           // 存储YUV图像中的Y数据
//    QOpenGLTexture* m_texU = nullptr;           // 存储YUV图像中的U数据
//    QOpenGLTexture* m_texV = nullptr;           // 存储YUV图像中的V数据
//    QOpenGLPixelTransferOptions m_options;

//    GLuint VBO = 0;       // 顶点缓冲对象,负责将数据从内存放到缓存，一个VBO可以用于多个VAO
//    GLuint VAO = 0;       // 顶点数组对象,任何随后的顶点属性调用都会储存在这个VAO中，一个VAO可以有多个VBO
//    GLuint EBO = 0;       // 元素缓冲对象,它存储 OpenGL 用来决定要绘制哪些顶点的索引
//    QSize  m_size;
//    QSizeF  m_zoomSize;
//    QPointF m_pos;
//};
//}
namespace  FFmpegOutput{
class VideoPlayer: public QWidget
{
public:
    explicit VideoPlayer(QWidget *parent = nullptr,int image_width=0,int image_height=0,bool image_background=false);
    ~VideoPlayer();
    void initVideoPlayer(int image_width,int image_height,bool image_background);
    void exitVideoPlayer();
    virtual  void repaint(AVFrame* frame);
    virtual void paintEvent(QPaintEvent *event) override;

private:
    FFmpegPublic::VideoTranscoder *__pVideoTranscoder=nullptr;
    uint8_t * __dstImageData[4];
    int __image_width;
    int __image_height;
    bool __image_background=false;
};
}

#endif // YOLOAUDIOPLAYER_H
