#include "EventLoopThreadPool.h"

using namespace Link::Net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop)
:mainLoop(loop),
started(false),
threadNum(0),
threadIndex(0)
{
}

void EventLoopThreadPool::init(int num)
{
    threadNum = num;
    for(int i=0;i<num;++i)
    {
        std::shared_ptr<EventLoopThread> thread(new EventLoopThread());
        thread->start();
        threadPool.push_back(thread);
        eventLoopPool.push_back(thread->getLoopInThread());
    }
    started = true;
}

bool EventLoopThreadPool::isStarted()
{
    return started;
}

EventLoop* EventLoopThreadPool::getOneLoopFromPool()
{
    if(eventLoopPool.empty())
    {
        return mainLoop;
    }
    threadIndex = (threadIndex+1)%threadNum;
    return eventLoopPool[threadIndex];
}

