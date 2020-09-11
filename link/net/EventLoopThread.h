#ifndef __LINK_NET_EVENTLOOPTHREAD_H__
#define __LINK_NET_EVENTLOOPTHREAD_H__

#include "../base/Thread.h"
#include "EventLoop.h"
#include <condition_variable>

namespace Link{
namespace Net{

class EventLoopThread : public Link::Base::Thread{
public:
    EventLoopThread();
    ~EventLoopThread(){}
    void run() override;
    EventLoop* getLoopInThread();
private:
    EventLoop* loop;
    //和基类中的重名，按命名域查找没得问题
    std::mutex mutex;
    std::condition_variable condition;
};


}
}
#endif