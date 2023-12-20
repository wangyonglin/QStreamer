#ifndef RUNNABLEBASE_H
#define RUNNABLEBASE_H
#include <string>
#include <thread>

class YoloRunnable
{
public:
    YoloRunnable();
    ~YoloRunnable();
    template<typename _Callable, typename... _Args>
    std::thread * startRunnable(_Callable&& __func, _Args&&... __args)
    {
        if(!pRunnableBase){
            pRunnableBase = new std::thread(std::forward<_Callable>(__func),
                                            std::forward<_Args>(__args)...);
            if(!pRunnableBase){
                fprintf(stderr,"YoloRunnable::Start() failed");
                return nullptr;
            }
            return pRunnableBase;
        }
      return pRunnableBase;
    }
    int stopRunnable();
    int abort=0;
    std::thread *pRunnableBase=nullptr;
    virtual void Runnable() = 0;
};

#endif // RUNNABLEBASE_H
