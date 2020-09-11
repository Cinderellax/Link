#include "IOEvent.h"
#include "EventLoop.h"
#include <sys/epoll.h>
#include <unistd.h>

using namespace Link::Net;

IOEvent::IOEvent(EventLoop* loop, int fd)
:m_loop(loop),m_fd(fd),m_events(0)
{
    //要不要在构造的时候就将这个事件添加到loop中？？？
    //要不要问题都不是特别大
    //loop->add(this);?????

}
IOEvent::~IOEvent(){
    //这些工作留给手动调用
    //::close(m_fd);  ??
    //loop->remove(this); ????
}

const uint32_t  IOEvent::getEvents(){
    return m_events;
}

const int IOEvent::getFd(){
    return m_fd;
}

//下面几个函数中的update()会往eventCtrl中的事件池中修改该事件，同时也会修改epoll事件表
void IOEvent::enableRead(bool enable){
    if(enable)
        m_events |= readEventFlag;
    else
        m_events &= ~readEventFlag;
    update();
}
void IOEvent::enableWrite(bool enable){
    if(enable)
        m_events |= writeEventFlag;
    else
        m_events &= ~writeEventFlag;
    update();
}
void IOEvent::enableError(bool enable){
    if(enable)
        m_events |= errorEventFlag;
    else
        m_events &= ~errorEventFlag;
    update();
}

void IOEvent::disableAll(){
    m_events = noneEventFlag;
    update();
}

bool IOEvent::isWriting(){
    return m_events & writeEventFlag;
}
bool IOEvent::isReading(){
    return m_events & readEventFlag;
}
//从loop中删除事件
void IOEvent::removeFromLoop(){
    m_loop->removeEvent(shared_from_this());
}
//看epoll返回的事件准备条件来决定使用何种回调函数
void IOEvent::handle(uint32_t revents){
    
    if((revents & EPOLLHUP) && !(revents & EPOLLIN)){
        if(closeHandle)
            closeHandle();
    }
    if(revents & EPOLLERR){
        if(errorHandle)
            errorHandle();
    }
    if(revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
        if(readHandle)
            readHandle();
    }
    if(revents & EPOLLOUT){
        if(writeHandle)
            writeHandle();
    }
}
//设定回调函数
void IOEvent::setReadFunc(const Func& func){
    readHandle = func;
}
void IOEvent::setWriteFunc(const Func& func){
    writeHandle = func;
}
void IOEvent::setErrorFunc(const Func& func){
    errorHandle = func;
}
void IOEvent::setCloseFunc(const Func& func){
    closeHandle = func;
}
//更新事件状态
void IOEvent::update(){
    m_loop->modifyEvent(shared_from_this());
}