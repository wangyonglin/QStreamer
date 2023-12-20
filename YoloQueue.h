#ifndef YOLOLIST_H
#define YOLOLIST_H
#include <BaseQueue.h>
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

#include <YoloFFmpeg.h>
namespace YoloQueue{

    class FrameQueue
    {
    public:
        FrameQueue();
        ~FrameQueue();
        void abortFrame();
        int addFramePlus(YoloFFmpeg::FramePlus * framePlus);
        int getFramePlus(YoloFFmpeg::FramePlus **framePlus,const int timeout);
         YoloFFmpeg::FramePlus *frontFramePlus();
        int getFrametPlusSize();

    private:
        void releaseFrame();
        BaseQueue< YoloFFmpeg::FramePlus*> queue;
    };


    class PacketQueue
    {
    public:
        PacketQueue();
        ~PacketQueue();
        void abortPacketPlus();
        void releasePacketPlus();
        int getPacketPlusSize();
        int addPacketPlus(AVPacket *pak,AVRational time_base);

         int getPacketPlus(YoloFFmpeg::PacketPlus **packetPlus,const int timeout);
    private:
        BaseQueue< YoloFFmpeg::PacketPlus *> queue;

    };

}


#endif // YOLOLIST_H
