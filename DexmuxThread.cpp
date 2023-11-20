#include "DexmuxThread.h"
#include <thread>
#include <string>



DexmuxThread::DexmuxThread(AVPacketQueue *audio_packet_queue, AVPacketQueue *video_packet_queue)
    :audio_packet_queue(audio_packet_queue),video_packet_queue(video_packet_queue)
{

}

DexmuxThread::~DexmuxThread()
{
    qInfo("~DexmuxThread");
      if(pth) {
          Stop();
      }
}

int DexmuxThread::Init(const char *_url)
{
    qInfo("DexmuxThread::Init  \n");

    url = _url;
    AVDictionary* dict = nullptr;
    av_dict_set(&dict, "rtsp_transport", "tcp", 0);      // 设置rtsp流使用tcp打开，如果打开失败错误信息为【Error number -135 occurred】可以切换（UDP、tcp、udp_multicast、http），比如vlc推流就需要使用udp打开
    av_dict_set(&dict, "max_delay", "3", 0);             // 设置最大复用或解复用延迟（以微秒为单位）。当通过【UDP】 接收数据时，解复用器尝试重新排序接收到的数据包（因为它们可能无序到达，或者数据包可能完全丢失）。这可以通过将最大解复用延迟设置为零（通过max_delayAVFormatContext 字段）来禁用。
    av_dict_set(&dict, "timeout", "1000000", 0);         // 以微秒为单位设置套接字 TCP I/O 超时，如果等待时间过短，也可能会还没连接就返回了。

    /* open input file, and allocate format context */
    if (avformat_open_input(&fmt_ctx, url.c_str(), nullptr, &dict) < 0) {
        qDebug( "Could not open source file (%s)",url.c_str());
        return -1;
    }
    // 释放参数字典
    if(dict)
    {
        av_dict_free(&dict);
    }
    /* retrieve stream information */
    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        qDebug("Could not find stream information\n");
        return -1;
    }
    audio_stream_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audio_stream_idx < 0) {
        qDebug( "Could not find %s stream in input file \n", av_get_media_type_string(AVMEDIA_TYPE_AUDIO));
        return -1;
    }
    video_stream_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (video_stream_idx < 0) {
        qDebug( "Could not find %s stream in input file \n", av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
        return -1;
    }
       av_dump_format(fmt_ctx, 0, url.c_str(), 0);
       qInfo("Init leave");
    return 0;
}

int DexmuxThread::Start()
{
    qInfo("DexmuxThread::Start  \n");
    pth= new std::thread(&DexmuxThread::Run,this);
    if(!pth){
        qDebug("dexmux_thread::start  failed\n");
        return -1;
    }
    return 0;
}

int DexmuxThread::Stop()
{
   // qInfo("DexmuxThread::Stop  \n");
    DexmuxThread::Stop();

    if(fmt_ctx){
        avformat_close_input(&fmt_ctx);
    }
    return 0;
}

void DexmuxThread::Run()
{
    int ret =-1;
    qInfo("DexmuxThread::Run\n");
    AVPacket pkt;
    while (abort!=1) {
//        if(audio_packet_queue->Size() > 100 || video_packet_queue->Size() > 100) {
//                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
//                    continue;
//                }
        /* read frames from the file */
        if ((ret=av_read_frame(fmt_ctx, &pkt)) < 0) {
            av_strerror(ret, err2str, sizeof(err2str));
            qDebug("av_read_frame failed, ret:%d, err2str:%s", ret, err2str);
            break;
        }
        if (pkt.stream_index == video_stream_idx){
            video_packet_queue->Push(&pkt);

        }else if (pkt.stream_index == audio_stream_idx){
            audio_packet_queue->Push(&pkt);
        }else{
            av_packet_unref(&pkt);
        }

    }
    qInfo("DexmuxThread::Run finish\n");
}
AVCodecParameters* DexmuxThread::getAudioCodecParameters(){
    qInfo("DexmuxThread::getAudioCodecParameters\n");
    AVCodecParameters * codec_param=nullptr;
    if(audio_stream_idx>=0){
        codec_param=  fmt_ctx->streams[audio_stream_idx]->codecpar;
    }
    return codec_param;
}

AVCodecParameters* DexmuxThread::getVideoCodecParameters(){
    qInfo("DexmuxThread::getVideoCodecParameters\n");
    AVCodecParameters * codec_param=nullptr;
    if(video_stream_idx>=0){
        codec_param=  fmt_ctx->streams[video_stream_idx]->codecpar;
    }
    return codec_param;

}

AVRational DexmuxThread::AudioStreamTimebase()
{
    if(audio_stream_idx != -1) {
          return fmt_ctx->streams[audio_stream_idx]->time_base;
      } else {
          return AVRational{0, 0};
      }
}

AVRational DexmuxThread::VideoStreamTimebase()
{
    if(video_stream_idx != -1) {
          return fmt_ctx->streams[video_stream_idx]->time_base;
      } else {
          return AVRational{0, 0};
      }
}


