#ifndef YOLOFFMPEG_H
#define YOLOFFMPEG_H
extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
#include <SDL.h>
#include <SDL_audio.h>
}
#include <string>
#include <QDebug>
namespace  YoloFFmpeg{
    typedef  enum{
        Video_Midea=AVMEDIA_TYPE_VIDEO,
        Audio_Midea=AVMEDIA_TYPE_AUDIO
    }MideaFFmpeg;
    typedef enum {
        Audio_Master=0,
        Video_Master,
        External_Master
    }SyncTimeType;
    class FramePlus{
    public:
        static bool    initFramePlus(FramePlus **framePlus,AVFrame *frame,AVRational time_base);
        static  void freeFramePlus(FramePlus **framePlus);
    public:
        AVFrame * frame;
        int64_t ptsTime;
         int64_t pts;
        AVRational time_base;
    };
    class PacketPlus{
    public:
        static bool    initPacketPlus(PacketPlus **packetPlus, AVPacket * packet,AVRational time_base);
        static  void freePacketPlus(PacketPlus **packetPlus);


    public:
        AVPacket * packet;
        int64_t ptsTime;
        AVRational time_base;
    };
    class PublicFFmpeg{
        int findCameras();
    };
    class DexmuxFFmpeg
    {

    public:
        DexmuxFFmpeg();
        ~DexmuxFFmpeg();
        int initDexmuxFFmpeg(const std::string &url);
        void freeDexmuxFFmpeg();
        int getWidth();
        int getHeight();
        int getTotalTime();
        int getFrameRate();
        int receiveFrame(AVPacket *packet);
        AVFormatContext * getFormatContext();

        AVRational getAudioTimebase();
        AVRational getVideoTimebase();

        int getAudioStreamIndex();
        int getVideoStreamIndex();
         AVCodecParameters * getAudioCodecParameters();
         AVCodecParameters * getVideoCodecParameters();
        int getStreamIndex(MideaFFmpeg midea);


    private:
        char err2str[256]={0};
        AVFormatContext * fmt_ctx=nullptr;
        int __audio_stream_idx=-1;
        int __video_stream_idx=-1;
    };
    class DecodecFFmpeg
    {
    public:
        DecodecFFmpeg();
        ~DecodecFFmpeg();
        int initDecodecFFmpeg(AVCodecParameters *codecParameters);
        void freeDecodecFFmpeg();
        int sendPacket(AVPacket *packet);
        int receiveFrame(AVFrame *frame);
        int SendPacketPlus(PacketPlus *packetPlus);
         int ReceiveFramePlus(FramePlus ** framePlus);
        AVCodecContext * getCodeContext();
        AVRational getTimebase();
    private:
         char err2str[256]={0};
         AVCodecContext * codec_ctx = nullptr;
         AVRational timeBase;
    };


}
namespace YoloFFmpeg{

    class SyncTimeFFmpeg{
    public:
        void initClock(const SyncTimeType & syncTimeType= External_Master);
        void setClock(int64_t ptstime);
        int64_t getDriftTime();
    public:
        int64_t lastPtsTime=0;
        int64_t lastPtsDrift=0;
        int64_t startTime=0;
    private:
         time_t getMicroseconds();
         SyncTimeType syncTimeType;
    };
}
namespace YoloFFmpeg {

    class VideoTranscoder
    {
    public:

        VideoTranscoder(int dst_width, int dst_height, AVPixelFormat dst_format);
        ~VideoTranscoder();

        int VideoTranscodertoImage(uint8_t **dst_data, AVFrame *frame);

    private:
         int initVideoTranscoder(struct SwsContext **img_convert_ctx,AVFrame * frame);
         void freeVideoTranscoder(struct SwsContext *img_convert_ctx);

        int __dst_width;
        int __dst_height;
        AVPixelFormat __dst_format;
        uint8_t  *__dst_data[4];
        int  __dst_linesize[4];
        SwsContext *__img_convert_ctx=nullptr;
    };
}
#endif // YOLOFFMPEG_H
