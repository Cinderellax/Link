#include "IOMultiplex.h"
#include <iostream>

using namespace Link::Net;

Epoll::Epoll()
: epollFd(epoll_create1(EPOLL_CLOEXEC))
{
    if(epollFd < 0)
    std::cout<<"create epollfd error"<<std::endl;
}

Epoll::~Epoll(){
    ::close(epollFd);
}

int Epoll::epollCtrl(IOEvent* ioEvent, const int& operation){
    epoll_event event = {0};
    event.events = ioEvent->getEvents();
    event.data.fd = ioEvent->getFd();
    int res = epoll_ctl(epollFd,operation,ioEvent->getFd(),&event);
    return res;
}

bool Epoll::addEvent(IOEvent* ioEvent){
    if(epollCtrl(ioEvent, EPOLL_CTL_ADD) < 0)
    {
        std::cout<<"error add event to epoll, fd = "<<ioEvent->getFd()<<" event = "<<ioEvent->getEvents()<<std::endl;
        return false;
    }
    return true;
}

bool Epoll::removeEvent(IOEvent* ioEvent){
    if(epollCtrl(ioEvent, EPOLL_CTL_DEL) < 0)
    {
        std::cout<<"error add event to epoll, fd = "<<ioEvent->getFd()<<" event = "<<ioEvent->getEvents()<<std::endl;
        return false;
    }
    return true;
} 

bool Epoll::modifyEvent(IOEvent* ioEvent){
    if(epollCtrl(ioEvent, EPOLL_CTL_MOD) < 0)
    {
        std::cout<<"error add event to epoll, fd = "<<ioEvent->getFd()<<" event = "<<ioEvent->getEvents()<<std::endl;
        return false;
    }
    return true;
}

int Epoll::waitEpollEvent(epoll_event* eventList, int eventSize, int timeMs){
    int res = epoll_wait(epollFd, eventList, eventSize, timeMs);
    return res;
}

//还未写
int Epoll::waitEvent(){
return 0;
}

//-------------------------------------------------Class Poll--------------------------------------------------------------------------

Poll::Poll(){}

Poll::~Poll(){}

bool Poll::addEvent(IOEvent* ioEvent){
    pollfd fd={ioEvent->getFd(),(short int)ioEvent->getEvents(),0};
    pollfds.push_back(fd);
    return true;
}

bool Poll::removeEvent(IOEvent* ioEvent){
    auto it = pollfds.begin();
    for(; it != pollfds.end(); ++it){
        if(it->fd == ioEvent->getFd())
            break;
    }
    if(it != pollfds.end()){
        pollfds.erase(it);
    }
    else
    {
        std::cout<<"error add event to epoll, fd = "<<ioEvent->getFd()<<" event = "<<ioEvent->getEvents()<<std::endl;
        return false;
    }
    return true;
}

bool Poll::modifyEvent(IOEvent* ioEvent){
    auto it = pollfds.begin();
    for(; it != pollfds.end(); ++it){
        if(it->fd == ioEvent->getFd())
            break;
    }
    if(it != pollfds.end()){
        it->events = ioEvent->getEvents();
    }
    else
    {
        std::cout<<"error add event to epoll, fd = "<<ioEvent->getFd()<<" event = "<<ioEvent->getEvents()<<std::endl;
        return false;       
    }
    return true;
}

int Poll::waitPollEvent(int timeMs){
    int res = poll(&*pollfds.begin(), pollfds.size(), timeMs);
    return res;
}

int Poll::waitEvent(){
    return 0;
}

//-------------------------------------------Class Select---------------------------------------------------------------------------
Select::Select()
{
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);
}
Select::~Select(){
    //FD_CLR.....也没啥必要
}
bool Select::addEvent(IOEvent* ioEvent){
    int state = ioEvent->getEvents();
    maxFd = std::max(maxFd,ioEvent->getFd());
    switch(state){
        case 0:
                FD_SET(ioEvent->getFd(), &readfds);
                if(!FD_ISSET(ioEvent->getFd(), &readfds)){
                    std::cout<<"error addevent in select fd = "<<ioEvent->getFd()<<std::endl;
                    return false;
                }
                break;
        case 1:
                FD_SET(ioEvent->getFd(), &writefds);
                if(!FD_ISSET(ioEvent->getFd(), &writefds)){
                    std::cout<<"error addevent in select fd = "<<ioEvent->getFd()<<std::endl;
                    return false;
                }
                break;
        case 2:
                FD_SET(ioEvent->getFd(), &exceptfds);
                if(!FD_ISSET(ioEvent->getFd(), &exceptfds)){
                    std::cout<<"error addevent in select fd = "<<ioEvent->getFd()<<std::endl;
                    return false;
                }
                break;
        default:
                break;
    }
    return true;
}

bool Select::removeEvent(IOEvent* ioEvent){
    FD_CLR(ioEvent->getFd(), &readfds);
    FD_CLR(ioEvent->getFd(), &writefds);
    FD_CLR(ioEvent->getFd(), &exceptfds);
    return true;
}

bool Select::modifyEvent(IOEvent* ioEvent){
    bool res = (removeEvent(ioEvent) && addEvent(ioEvent));
    return res;
}

int Select::waitSelectEvent(int timeMs){
    int res=0;
    if(timeMs == -1)
        res = select(maxFd+1,&readfds,&writefds,&exceptfds,NULL);
    else{
        timeval time = {timeMs/1000,timeMs%1000};
        res = select(maxFd+1,&readfds,&writefds,&exceptfds,&time);
    }
    return res;

}
//还未写
int Select::waitEvent(){
    return 0;
}

