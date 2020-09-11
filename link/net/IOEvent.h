#ifndef __LINK_NET_IOEVENT__
#define __LINK_NET_IOEVENT__

#include <stdint.h>
#include <functional>
#include <memory>
#include <sys/epoll.h>

namespace Link{
namespace Net{

class EventLoop;

class IOEvent :public std::enable_shared_from_this<IOEvent> {
public:
    //定义智能指针别名
    using ptr = std::shared_ptr<IOEvent>;
    //定义回调函数别名
    using Func = std::function<void()>;

    IOEvent(EventLoop* loop, int fd);
    ~IOEvent();
    const uint32_t getEvents();
    const int getFd();
    //设定事件类型标志（可读，可写，出错）
    void enableRead(bool enable = true);
    void enableWrite(bool enable = true);
    void enableError(bool enable = true);
    //清空事件类型标志
    void disableAll();

    bool isWriting();
    bool isReading();
    //从loop中删除该事件
    void removeFromLoop();
    //根据已准备好的事件条件来运行对应的回调函数
    void handle(uint32_t revents);
    //设定各种回调函数
    void setReadFunc(const Func& func);
    void setWriteFunc(const Func& func);
    void setErrorFunc(const Func& func);
    void setCloseFunc(const Func& func);
private:
    //设定事件类型标志
    enum {
        noneEventFlag = 0,
        readEventFlag = EPOLLIN | EPOLLPRI,
        writeEventFlag = EPOLLOUT,
        errorEventFlag = EPOLLERR
    };

    EventLoop* m_loop;
    int m_fd;
    uint32_t m_events;
    Func readHandle;
    Func writeHandle;
    Func errorHandle;
    Func closeHandle;
    void update();
};
    
}
}

#endif