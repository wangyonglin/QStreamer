#ifndef BASEQUEUE_H
#define BASEQUEUE_H
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename  T>
class BaseQueue
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

#endif // QUEUE_H
