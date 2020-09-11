#ifndef __LINK_NET_EVENTLOOP_H__
#define __LINK_NET_EVENTLOOP_H__

#include <thread>
#include "IOEventCtrl.h"
#include "IOEvent.h"

namespace Link{
namespace Net{
class IOEventCtrl;
class EventLoop{
public:
        using Func = std::function<void()>;

        EventLoop();
        ~EventLoop();

        //增加，删除，修改事件都是内部中调用eventCtrl的对应方法
        void addEvent(IOEvent::ptr event);
        void removeEvent(IOEvent::ptr event);
        void modifyEvent(IOEvent::ptr event);
        //运行是个while(1),就不停的调用eventCtrl中的epoll_wait并使用对应事件的处理方式来处理
        void run();
private:
        //static const int PollTimeMs;
        enum{ PollTimeMs = 3000}; 
        IOEventCtrl* eventCtrl;
};


}
}

#endif