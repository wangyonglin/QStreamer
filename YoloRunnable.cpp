#include "RunnableBase.h"

RunnableBase::RunnableBase()
{

}

RunnableBase::~RunnableBase()
{
    if(pRunnableBase){
        stopRunnable();
    }
}
//template<typename _Callable, typename... _Args>
//int RunnableBase::startRunnable(_Callable&& __func, _Args&&... __args)
//{
//    if(!pRunnableBase){
//        pRunnableBase = new std::thread(std::forward<_Callable>(__func),
//                                        std::forward<_Args>(__args)...);
//        if(!pRunnableBase){
//            fprintf(stderr,"DecodecThread::Start() failed");
//            return -1;
//        }
//        return 0;
//    }
//  return 0;
//}

int RunnableBase::stopRunnable()
{
    if(pRunnableBase){
        pRunnableBase->join();
        delete pRunnableBase;
    }

return 0;
}

