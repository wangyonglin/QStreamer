#include "FFmpegOutput.h"



FFmpegOutput::AudioPlayer::AudioPlayer(int sample_rate, SDL_AudioFormat sample_fmt, int channels)
    :__sample_rate(sample_rate),__sample_fmt(sample_fmt),__channels(channels)
{

}

FFmpegOutput::AudioPlayer::~AudioPlayer()
{

}

int FFmpegOutput::AudioPlayer::initAudioPlayer()
{
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)!=0){
        std::cout << "Could not initialize SDL -   ： " << SDL_GetError() << std::endl;
        return -1;
    }
    return 0;
}

void FFmpegOutput::AudioPlayer::exitAudioPlayer()
{

}

int FFmpegOutput::AudioPlayer::openAudioPlayer()
{

    // 设置音频参数

    SDL_memset(&__desired, 0, sizeof(__desired)); /* or SDL_zero(want) */
    SDL_memset(&obtained, 0, sizeof(obtained));
    __desired.freq = __sample_rate; // 采样率
    __desired.format = __sample_fmt; // 采样格式
    __desired.channels = __channels; // 声道数
    __desired.samples = 2048; // 每个缓冲区的样本数
    __desired.callback = nullptr; // 回调函数
    __desired.userdata = nullptr; // 用户数据

    // 打开音频设备
    __deviceId = SDL_OpenAudioDevice(nullptr, 0, &__desired,&obtained, 0);
    if (__deviceId == 0) {
        std::cout << "无法打开音频设备： " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_PauseAudioDevice(__deviceId, 0);
    return 0;
}



int FFmpegOutput::AudioPlayer::addQueueAudioValue(const void *data, Uint32 dst_datasize,bool waitTime)
{
    int error=-1;
    if(__deviceId && data && dst_datasize >0){
        if((error=  SDL_QueueAudio(__deviceId,data,dst_datasize))!=0){
             std::cout << "AudioPlayer::push{SDL_QueueAudio} " << SDL_GetError() << std::endl;
        }
        if(waitTime == true){
  SDL_Delay((dst_datasize) * 1000.0 / (obtained.freq *
                                       (SDL_AUDIO_BITSIZE(obtained.format) / 8)*
                                       obtained.channels) - 1);
        }

        return 0;
    }
    return error;
}

void FFmpegOutput::AudioPlayer::pause(int time)
{
    SDL_PauseAudioDevice(__deviceId, time);
}

int FFmpegOutput::AudioPlayer::closeAudioPlayer()
{
    if(__deviceId){
          SDL_CloseAudioDevice(__deviceId);
          return 0;
    }
    return -1;
}

Uint32 FFmpegOutput::AudioPlayer::getQueuedAudioSize()
{
    if(__deviceId){
          return SDL_GetQueuedAudioSize(__deviceId);
    }
    return -1;
}

SDL_AudioSpec FFmpegOutput::AudioPlayer::getObtained()
{
    return obtained;
}



//extern "C" {        // 用C规则编译指定的代码
//#include "libavcodec/avcodec.h"

//}

//#if USE_WINDOW
//FFmpegOutput::VideoPlayer::VideoPlayer(QOpenGLWindow::UpdateBehavior updateBehavior, QWindow *parent):QOpenGLWindow(updateBehavior, parent)
//{
//    // 初始化视图大小，由于Shader里面有YUV转RGB的代码，会初始化显示为绿色，这里通过将视图大小设置为0避免显示绿色背景
//    m_pos = QPointF(0, 0);
//    m_zoomSize = QSize(0, 0);
//}
//#else
//FFmpegOutput::VideoPlayer::VideoPlayer(QWidget *parent, Qt::WindowFlags f): QOpenGLWidget(parent, f)
//{
//    // 初始化视图大小，由于Shader里面有YUV转RGB的代码，会初始化显示为绿色，这里通过将视图大小设置为0避免显示绿色背景

//    m_pos = QPointF(0, 0);
//    m_zoomSize = QSize(0, 0);
//}
//#endif


//FFmpegOutput::VideoPlayer::~VideoPlayer()
//{
//    if(!isValid()) return;        // 如果控件和OpenGL资源（如上下文）已成功初始化，则返回true。
//    this->makeCurrent(); // 通过将相应的上下文设置为当前上下文并在该上下文中绑定帧缓冲区对象，为呈现此小部件的OpenGL内容做准备。
//    // 释放纹理
//    if(m_texY)
//    {
//        m_texY->destroy();
//        delete m_texY;
//    }
//    if(m_texU)
//    {
//        m_texU->destroy();
//        delete m_texU;
//    }
//    if(m_texV)
//    {
//        m_texV->destroy();
//        delete m_texV;
//    }
//    this->doneCurrent();    // 释放上下文
//    // 释放
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//    glDeleteVertexArrays(1, &VAO);
//}

//void FFmpegOutput::VideoPlayer::repaint(AVFrame *frame)
//{
//    // 如果帧长宽为0则不需要绘制
//    if(!frame || frame->width == 0 || frame->height == 0) return;

//    // 当切换显示的视频后，如果分辨率不同则需要重新创建纹理，否则会崩溃
//    if(frame->width != m_size.width() || frame->height != m_size.height())
//    {
//        if(m_texY && m_texU && m_texV)
//        {
//            m_texY->destroy();
//            m_texU->destroy();
//            m_texV->destroy();
//            delete m_texY;
//            delete m_texU;
//            delete m_texV;
//            m_texY = nullptr;
//            m_texU = nullptr;
//            m_texV = nullptr;
//        }
//    }
//    if(!m_texY) // 初始化纹理
//    {
//        // 创建2D纹理
//        m_texY = new QOpenGLTexture(QOpenGLTexture::Target2D);
//        m_texU = new QOpenGLTexture(QOpenGLTexture::Target2D);
//        m_texV = new QOpenGLTexture(QOpenGLTexture::Target2D);

//        // 设置纹理大小
//        m_texY->setSize(frame->width, frame->height);
//        m_texU->setSize(frame->width / 2, frame->height / 2);
//        m_texV->setSize(frame->width / 2, frame->height / 2);

//        // 设置放大、缩小过滤器
//        m_texY->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
//        m_texU->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
//        m_texV->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);

//        // 设置图像格式
//        m_texY->setFormat(QOpenGLTexture::R8_UNorm);
//        m_texU->setFormat(QOpenGLTexture::R8_UNorm);
//        m_texV->setFormat(QOpenGLTexture::R8_UNorm);

//        // 分配内存
//        m_texY->allocateStorage();
//        m_texU->allocateStorage();
//        m_texV->allocateStorage();

//        // 记录图像分辨率
//        m_size.setWidth(frame->width);
//        m_size.setHeight(frame->height);
//        resizeGL(this->width(), this->height());
//    }
//    m_options.setImageHeight(frame->height);
//    m_options.setRowLength(frame->linesize[0]);
//    m_texY->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, static_cast<const void *>(frame->data[0]), &m_options);   // 设置图像数据 Y
//    m_options.setRowLength(frame->linesize[1]);
//    m_texU->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, static_cast<const void *>(frame->data[1]), &m_options);   // 设置图像数据 U
//    m_options.setRowLength(frame->linesize[2]);
//    m_texV->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, static_cast<const void *>(frame->data[2]), &m_options);   // 设置图像数据 V

//    av_frame_unref(frame);  //  取消引用帧引用的所有缓冲区并重置帧字段。

//    this->update();
//}

//// 三个顶点坐标XYZ，VAO、VBO数据播放，范围时[-1 ~ 1]直接
//static GLfloat vertices[] = {  // 前三列点坐标，后两列为纹理坐标
//     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,      // 右上角
//     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,      // 右下
//    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,      // 左下
//    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f      // 左上
//};
//static GLuint indices[] = {
//    0, 1, 3,
//    1, 2, 3
//};
//void FFmpegOutput::VideoPlayer::initializeGL()
//{
//    initializeOpenGLFunctions();

//    // 加载shader脚本程序
//    m_program = new QOpenGLShaderProgram(this);
//    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vsh");
//    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.fsh");
//    m_program->link();

//    // 绑定YUV 变量值
//    m_program->bind();
//    m_program->setUniformValue("tex_y", 0);
//    m_program->setUniformValue("tex_u", 1);
//    m_program->setUniformValue("tex_v", 2);

//    // 返回属性名称在此着色器程序的参数列表中的位置。如果名称不是此着色器程序的有效属性，则返回-1。
//    GLuint posAttr = GLuint(m_program->attributeLocation("aPos"));
//    GLuint texCord = GLuint(m_program->attributeLocation("aTexCord"));

//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);

//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glGenBuffers(1, &EBO);    // 创建一个EBO
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


//    // 为当前绑定到的缓冲区对象创建一个新的数据存储target。任何预先存在的数据存储都将被删除。
//    glBufferData(GL_ARRAY_BUFFER,        // 为VBO缓冲绑定顶点数据
//                       sizeof (vertices),      // 数组字节大小
//                       vertices,               // 需要绑定的数组
//                       GL_STATIC_DRAW);        // 指定数据存储的预期使用模式,GL_STATIC_DRAW： 数据几乎不会改变
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  // 将顶点索引数组传入EBO缓存
//    // 设置顶点坐标数据
//    glVertexAttribPointer(posAttr,                     // 指定要修改的通用顶点属性的索引
//                          3,                     // 指定每个通用顶点属性的组件数（如vec3：3，vec4：4）
//                          GL_FLOAT,              // 指定数组中每个组件的数据类型(数组中一行有几个数)
//                          GL_FALSE,              // 指定在访问定点数据值时是否应规范化 ( GL_TRUE) 或直接转换为定点值 ( GL_FALSE)，如果vertices里面单个数超过-1或者1可以选择GL_TRUE
//                          5 * sizeof(GLfloat),   // 指定连续通用顶点属性之间的字节偏移量。
//                          nullptr);              // 指定当前绑定到目标的缓冲区的数据存储中数组中第一个通用顶点属性的第一个组件的偏移量。初始值为0 (一个数组从第几个字节开始读)
//    // 启用通用顶点属性数组
//    glEnableVertexAttribArray(posAttr);                // 属性索引是从调用glGetAttribLocation接收的，或者传递给glBindAttribLocation。

//    // 设置纹理坐标数据
//    glVertexAttribPointer(texCord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<const GLvoid *>(3 * sizeof (GLfloat)));              // 指定当前绑定到目标的缓冲区的数据存储中数组中第一个通用顶点属性的第一个组件的偏移量。初始值为0 (一个数组从第几个字节开始读)
//    // 启用通用顶点属性数组
//    glEnableVertexAttribArray(texCord);                // 属性索引是从调用glGetAttribLocation接收的，或者传递给glBindAttribLocation。

//    // 释放
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);                        // 设置为零以破坏现有的顶点数组对象绑定

//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);        // 指定颜色缓冲区的清除值(背景色)


//}

//void FFmpegOutput::VideoPlayer::resizeGL(int w, int h)
//{
//    if(m_size.width()  < 0 || m_size.height() < 0) return;

//    // 计算需要显示图片的窗口大小，用于实现长宽等比自适应显示
//    if((double(w) / h) < (double(m_size.width()) / m_size.height()))
//    {
//        m_zoomSize.setWidth(w);
//        m_zoomSize.setHeight(((double(w) / m_size.width()) * m_size.height()));   // 这里不使用QRect，使用QRect第一次设置时有误差bug
//    }
//    else
//    {
//        m_zoomSize.setHeight(h);
//        m_zoomSize.setWidth((double(h) / m_size.height()) * m_size.width());
//    }
//    m_pos.setX(double(w - m_zoomSize.width()) / 2);
//    m_pos.setY(double(h - m_zoomSize.height()) / 2);
//    this->update(QRect(0, 0, w, h));
//}

//void FFmpegOutput::VideoPlayer::paintGL()
//{
//    glClear(GL_COLOR_BUFFER_BIT);     // 将窗口的位平面区域（背景）设置为先前由glClearColor、glClearDepth和选择的值
//    glViewport(m_pos.x(), m_pos.y(), m_zoomSize.width(), m_zoomSize.height());  // 设置视图大小实现图片自适应

//    m_program->bind();               // 绑定着色器
//    // 绑定纹理
//    if(m_texY && m_texU && m_texV)
//    {
//        m_texY->bind(0);
//        m_texU->bind(1);
//        m_texV->bind(2);
//    }

//    glBindVertexArray(VAO);           // 绑定VAO

//    glDrawElements(GL_TRIANGLES,      // 绘制的图元类型
//                   6,                 // 指定要渲染的元素数(点数)
//                   GL_UNSIGNED_INT,   // 指定索引中值的类型(indices)
//                   nullptr);          // 指定当前绑定到GL_ELEMENT_array_buffer目标的缓冲区的数据存储中数组中第一个索引的偏移量。
//    glBindVertexArray(0);
//    if(m_texY && m_texU && m_texV)
//    {
//        m_texY->release();
//        m_texU->release();
//        m_texV->release();
//    }
//    m_program->release();
//}

FFmpegOutput::VideoPlayer::VideoPlayer(QWidget *parent,int image_width,int image_height,bool image_background):
    QWidget(parent),__image_width(image_width),__image_height(image_height)
{
    initVideoPlayer(__image_width,__image_height,image_background);
}

FFmpegOutput::VideoPlayer::~VideoPlayer()
{
    exitVideoPlayer();
}

void FFmpegOutput::VideoPlayer::initVideoPlayer(int image_width, int image_height,bool image_background)
{
    __image_width=image_width;
    __image_height=image_height;
    __image_background=image_background;
    if(__image_background){

          __pVideoTranscoder = new FFmpegPublic::VideoTranscoder(__image_width,__image_height,AV_PIX_FMT_RGBA);
    }else{
          __pVideoTranscoder = new FFmpegPublic::VideoTranscoder(__image_width,__image_height,AV_PIX_FMT_RGB24);
    }

}

void FFmpegOutput::VideoPlayer::exitVideoPlayer()
{
     delete  __pVideoTranscoder;
}

void FFmpegOutput::VideoPlayer::repaint(AVFrame *frame)
{

    if(!frame || frame->width == 0 || frame->height == 0 || !__pVideoTranscoder)return;
    if(__pVideoTranscoder->VideoTranscoderConvert(__dstImageData,frame)==0){
        update();
    }
    av_frame_unref(frame);



}

void FFmpegOutput::VideoPlayer::paintEvent(QPaintEvent *event)
{
        Q_UNUSED(event)
        QPainter painter(this);
        if(__image_background){

             QImage image(__dstImageData[0], __image_width, __image_height, QImage::Format_RGBA8888);
                 painter.drawImage(rect(), image);
        }else{
             QImage image(__dstImageData[0], __image_width, __image_height, QImage::Format_RGB888);
                 painter.drawImage(rect(), image);
        }


}
