#include "EventLoopThread.h"

using namespace Link::Net;

EventLoopThread::EventLoopThread()
:loop(nullptr)
{
}

EventLoop* EventLoopThread::getLoopInThread()
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        if(loop == nullptr)
        {
            condition.wait(lock);
        }
    }
    return loop;
}

void EventLoopThread::run()
{
    EventLoop loop;
    {
        std::unique_lock<std::mutex> lock(mutex);
        this->loop = &loop;
        condition.notify_one();
    }
    loop.run();
}