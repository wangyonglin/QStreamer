#ifndef AUDIOOUTPUTTHREAD_H
#define AUDIOOUTPUTTHREAD_H

#include <QThread>
#include <QObject>
#include <QElapsedTimer>
#include <QThread>
#include <QTime>
#include <BaseThread.h>
#include <AVFrameQueue.h>
#include <bits/ios_base.h>
#include <AVFrameQueue.h>
#include <Synchronized.h>

typedef struct AudioParams
{
    int freq; //采样率
    int channels;
    int64_t channel_layout;
    enum AVSampleFormat fmt;
    int frame_size;
}AudioParams;
class AudioHandler
{
    //Q_OBJECT
public:
    AudioHandler();
    ~AudioHandler();
//    explicit AudioOutputThread(QObject *parent = nullptr);
//    ~AudioOutputThread();
    int Init(Synchronized *_avsync, AVRational _time_base,AVCodecParameters* _audio_codec_parameters,AVFrameQueue * _frame_queue);
    int64_t pts_ = AV_NOPTS_VALUE;
       AudioParams src_tgt_; // 解码后的参数
       AudioParams dst_tgt_; // SDL实际输出的格式
       AVFrameQueue *frame_queue= NULL;

       struct SwrContext *swr_ctx_ = NULL;

       uint8_t *audio_buf_ = NULL;
       uint8_t *audio_buf1_ = NULL;
       uint32_t audio_buf_size = 0;
       uint32_t audio_buf1_size = 0;
       uint32_t audio_buf_index = 0;

       Synchronized *avsync = NULL;
          AVRational time_base;

    AVCodecParameters* audio_codec_parameters;
    void DeInit();

};

#endif // AUDIOOUTPUTTHREAD_H
