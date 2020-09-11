#ifndef __LINK_NET_IOMULTIPLEX_H__
#define __LINK_NET_IOMULTIPLEX_H__

#define MAX_WAIT_NUM 16

#include <sys/epoll.h>
#include <sys/select.h>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

#include "IOEvent.h"

namespace Link{
namespace Net{
//基类IO复用模型
class IOMultiplex{

public:
        IOMultiplex(){}
        virtual ~IOMultiplex(){}
        //不可拷贝！！！
        IOMultiplex(const IOMultiplex&) = delete;
        virtual bool addEvent(IOEvent* ioEvent) = 0;
        virtual bool removeEvent(IOEvent* ioEvent) = 0;
        virtual bool modifyEvent(IOEvent* ioEvent) = 0;
        virtual int waitEvent() = 0;        
};
//子类的epoll模型
class Epoll : public IOMultiplex{
public:
        Epoll();
        ~Epoll() override;
        bool addEvent(IOEvent* ioEvent) override;
        bool removeEvent(IOEvent* ioEvent) override;
        bool modifyEvent(IOEvent* ioEvent) override;
        int waitEvent() override;
        int waitEpollEvent(epoll_event*, int = MAX_WAIT_NUM, int = -1);
private:
        int epollCtrl(IOEvent*, const int&);
        int epollFd;
};

//子类的poll模型
class Poll : public IOMultiplex{
public:
        using PollFds = std::vector<pollfd>;
        Poll();
        ~Poll() override;
        bool addEvent(IOEvent* ioEvent) override;
        bool removeEvent(IOEvent* ioEvent) override;
        bool modifyEvent(IOEvent* ioEvent) override;
        int waitEvent() override;
        int waitPollEvent(int timeMs = -1);
private:
        PollFds pollfds; 
};

//子类的select模型
class Select : public IOMultiplex{
public:
        Select();
        ~Select() override;
        bool addEvent(IOEvent* ioEvent) override;
        bool removeEvent(IOEvent* ioEvent) override;
        bool modifyEvent(IOEvent* ioEvent) override;
        int waitEvent() override;
        int waitSelectEvent(int timeMs = -1);
private:
        fd_set readfds;
        fd_set writefds;
        fd_set exceptfds;
        //select函数中需要传入被监听的文件描述符的总数，这个是进程的所有的文件描述符都会被监听
        //文件描述符从0开始，记录下所有的文件描述符中最大的一个，再+1即为其最大数量
        int maxFd = 0;
};


}
}
#endif