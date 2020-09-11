#include "IOEventCtrl.h"

using namespace Link::Net;

const int IOEventCtrl::activeEventLength = 16;

IOEventCtrl::IOEventCtrl()
:activeEvents(activeEventLength)
{
}

IOEventCtrl::~IOEventCtrl(){
    eventPool.clear();
}
//增加，删除，修改事件的函数都必须对事件池和epoll同时进行修改
void IOEventCtrl::addEvent(IOEvent::ptr event){
    eventPool.insert(std::pair<int, std::weak_ptr<IOEvent>>(event->getFd(), event));
    epoll.addEvent(event.get());
}

void IOEventCtrl::deleteEvent(IOEvent::ptr event){
    eventPool.erase(event->getFd());
    epoll.removeEvent(event.get());
}

void IOEventCtrl::modifyEvent(IOEvent::ptr event){
    if(eventPool.count(event->getFd()))
    {
        eventPool[event->getFd()] = event;
        epoll.modifyEvent(event.get());
    }
}
//等待epoll就绪事件并运行
void IOEventCtrl::waitAndRunHandle(int timeMs){
    //会将就绪的事件填入到activeEvents中
    int cnt = epoll.waitEpollEvent(&*activeEvents.begin(), activeEvents.size(), timeMs);
    if(cnt < 0) return;
    //依次运行就绪事件中的回调处理函数
    for(int i=0; i<cnt; ++i){
        int fd = activeEvents[i].data.fd;
        //事件不存在的情况下
        IOEvent::ptr ioevent = eventPool[fd].lock();
        if(ioevent)
        {
            ioevent->handle(activeEvents[i].events);
        }
        else
        {
            //事件不存在则会从事件池和epoll中删除该事件
            deleteEvent(ioevent);
        }
        
    }
}