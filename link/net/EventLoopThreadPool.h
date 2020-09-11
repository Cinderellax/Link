#ifndef __LINK_NET_EVENTLOOPTHREADPOOL_H__
#define __LINK_NET_EVENTLOOPTHREADPOOL_H__

#include "EventLoop.h"
#include "EventLoopThread.h"
#include <vector>
#include <memory>
#include <stdint.h>

namespace Link{
namespace Net{

class EventLoopThreadPool{
public:
    EventLoopThreadPool(EventLoop* loop);
    void init(int num = 16);
    bool isStarted();
    EventLoop* getOneLoopFromPool();
private:
    EventLoop* mainLoop;
    bool started;
    uint16_t threadNum;
    uint32_t threadIndex;
    std::vector<std::shared_ptr<EventLoopThread>> threadPool;
    std::vector<EventLoop*> eventLoopPool;
};
};
};
#endif