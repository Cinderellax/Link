#ifndef __LINK_NET_IOEVENTCTRL_H__
#define __LINK_NET_IOEVENTCTRL_H__

#include <memory>
#include "IOEvent.h"
#include "IOMultiplex.h"
#include <vector>
#include <map>
#include <memory>

namespace Link{
namespace Net{
class IOEventCtrl{
public:
        IOEventCtrl();
        ~IOEventCtrl();
        //增加，删除，修改事件
        void addEvent(IOEvent::ptr event);
        void deleteEvent(IOEvent::ptr event);
        void modifyEvent(IOEvent::ptr event);
        //使用epollwait等待事件就绪并且运行事件中的相应的回调函数
        void waitAndRunHandle(int timeMs);
private:
        static const int activeEventLength;
        //事件池，所有的事件！！！
        //之所以使用weak_ptr是为了防止多出引用计数而不会自动删除完成的事件
        //如果事件已经完成即使在事件池中也不会出问题，weak_ptr的lock()可以判断是否存在，不存在则直接删除即可
        std::map<int,std::weak_ptr<IOEvent>> eventPool;
        Epoll epoll;
        //就绪的事件队列，用来存放从epollwait中准备就绪的事件
        std::vector<epoll_event> activeEvents;
};
}
}
#endif