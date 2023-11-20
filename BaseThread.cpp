#include "BaseThread.h"


BaseThread::BaseThread()
{

}

BaseThread::~BaseThread()
{
    if(pth){
       Stop();
    }
}

int BaseThread::Start()
{
    return 0;
}

int BaseThread::Stop()
{
    if(pth){
        pth->join();
        delete pth;
    }

return 0;
}
