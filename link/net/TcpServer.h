#ifndef __LINK_NET_TCPSERVER__
#define __LINK_NET_TCPSERVER__

#include "TcpAccept.h"
#include "TcpConnect.h"
#include "EventLoopThreadPool.h"

#include <atomic>

namespace Link{
namespace Net{

class TcpServer{
public:
    TcpServer(EventLoop* loop,SocketAddr& addr);
    //虚析构函数，这个作为基类
    virtual ~TcpServer() {};
    
    void start();

    //下面的几个回调函数声明为纯虚函数，使用子类来实现晚绑定,3个半事件
    virtual void connectCallback(TcpConnect::ptr tcpConnect) = 0;
    virtual void messageCallback(TcpConnect::ptr tcpConnect, Buffer& buffer) = 0;
    virtual void writeCompleteCallback(TcpConnect::ptr tcpConnect) = 0;
    virtual void connectCloseCallback(TcpConnect::ptr tcpConnect) = 0;

    void addConnect(std::string name, TcpConnect::ptr connect);
    void addConnect(std::string name, TcpConnect* connect);

    bool haveConnect(std::string name);
    void removeConnect(std::string name);
    long getConnectCount() const;
    //不需要，写入的时候使用TcpConnect的内容即可
    // void write(TcpConnect& connect, void* data, uint32_t length);
    // void write(TcpConnect::ptr connect, void* data, uint32_t length);
    // void write();
    // void write();
    //不需要？？？
    //void setThreadPoolSize(uint16_t num);
private:
    void newConnected(int sockfd, SocketAddr& addr);
    EventLoop* mloop;
    SocketAddr tcpAddr;
    TcpAccept::ptr tcpAccept;

    std::map<std::string,TcpConnect::ptr> connectPool;
    void connectCloseEvent(TcpConnect::ptr connect);
    std::atomic<bool> isStart;
    std::shared_ptr<EventLoopThreadPool> threadPool;
};
}
}

#endif