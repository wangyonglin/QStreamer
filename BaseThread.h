#ifndef BASETHREAD_H
#define BASETHREAD_H
#include <string>
#include <thread>


class BaseThread
{
public:
    BaseThread();
    ~BaseThread();
    int Start();
    int Stop();
    int abort=0;
    std::thread *pth=nullptr;
    virtual void Run() = 0;
private:


};




#endif // THREAD_H
