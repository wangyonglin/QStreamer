#ifndef FFmpegPublic_H
#define FFmpegPublic_H
#include <FFmpegBase.h>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <thread>
namespace  FFmpegPublic{
namespace  QueueTemplates{
template<typename  T>
class QueueBase
{
public:

    void Abort(){
        abort =1;
        cond.notify_all();
    }
    int Push(T val){
        std::lock_guard<std::mutex> lock(mutex);
        if(abort == 1){
            return -1;
        }
        queue.push(val);
        cond.notify_one();
        return 0;
    }
    int Pop(T &val,const int timeout =0){
        std::unique_lock<std::mutex> lock(mutex);
        if(queue.empty()){
            cond.wait_for(lock,std::chrono::milliseconds(timeout),[this]{
                return !queue.empty()|abort;
            });
        }
        if(abort == 1){
            return  -1;
        }
        if(queue.empty()){
            return -2;
        }
        val = queue.front();
        queue.pop();
        return 0;
    }
    int Front(T &val){
        std::lock_guard<std::mutex> lock(mutex);
        if(abort ==1){
            return -1;
        } if(queue.empty()){
            return -2;
        }
        val = queue.front();
        return 0;
    }
    int Size(){
         std::lock_guard<std::mutex> lock(mutex);
         return  queue.size();
    }
private:
    int abort = 0;
    std::mutex mutex;
    std::condition_variable cond;
    std::queue<T> queue;
};
};
namespace Thread {
class Runnable
{
public:
    Runnable(){

    }
    ~Runnable(){
        if(pRunnableBase){
            stopRunnable();
        }
    }
    template<typename _Callable, typename... _Args>
    std::thread * startRunnable(_Callable&& __func, _Args&&... __args)
    {
        if(!pRunnableBase){
            pRunnableBase = new std::thread(std::forward<_Callable>(__func),
                                            std::forward<_Args>(__args)...);
            if(!pRunnableBase){
                fprintf(stderr,"Runnable::Start() failed");
                return nullptr;
            }
            return pRunnableBase;
        }
        return pRunnableBase;
    }
    int stopRunnable(){
        if(pRunnableBase){
            pRunnableBase->join();
            delete pRunnableBase;
        }

        return 0;
    }
    int abort=0;
    bool loop =false;
    std::thread *pRunnableBase=nullptr;
    virtual void entityRunnable() = 0;
};
}
}
namespace FFmpegPublic {
typedef enum {
    Audio_Master=0,
    Video_Master,
    External_Master
}ClockType;
class SyncTime{
public:
    void initClock(const ClockType & clockType= External_Master);
    void setClock(int64_t ptstime);
    int64_t getDriftTime();
public:
    int64_t lastPtsTime=0;
    int64_t lastPtsDrift=0;
    int64_t startTime=0;
private:
    time_t getCurrentMicroseconds();
    ClockType __clockType;
};
}

namespace FFmpegPublic {
class VideoTranscoder
{
public:

    VideoTranscoder(int dst_width, int dst_height, AVPixelFormat dst_format);
    ~VideoTranscoder();

    int VideoTranscoderConvert(uint8_t **dst_data, AVFrame *frame);

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

namespace FFmpegPublic {

class CameraFFmpeg{
    void  findCameras();

};
}
namespace FFmpegPublic {
typedef  enum{
    Video_Midea=AVMEDIA_TYPE_VIDEO,
    Audio_Midea=AVMEDIA_TYPE_AUDIO
}MideaFFmpeg;

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
}
namespace  FFmpegPublic {

namespace Queue {
class Frame
{
public:
    Frame();
    ~Frame();
    void abortFrame();
    int addFramePlus(FFmpegPublic::FramePlus * framePlus);
    int getFramePlus(FFmpegPublic::FramePlus **framePlus,const int timeout);
    FFmpegPublic::FramePlus *frontFramePlus();
    int getFrametPlusSize();

private:
    void releaseFrame();
    QueueTemplates::QueueBase< FFmpegPublic::FramePlus*> queue;
};


class Packet
{
public:
    Packet();
    ~Packet();
    void abortPacketPlus();
    void releasePacketPlus();
    int getPacketPlusSize();
    int addPacketPlus(AVPacket *pak,AVRational time_base);

    int getPacketPlus(FFmpegPublic::PacketPlus **packetPlus,const int timeout);
private:
      QueueTemplates::QueueBase< FFmpegPublic::PacketPlus *> queue;

};
}
}

namespace FFmpegPublic {
namespace Funcation {
int SendPacketPlus(AVCodecContext * codec_ctx,PacketPlus *packetPlus);
int ReceiveFramePlus(AVCodecContext * codec_ctx,FramePlus ** framePlus);
}
}
#endif // FFmpegPublic_H
