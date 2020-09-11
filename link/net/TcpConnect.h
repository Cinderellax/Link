#ifndef __LINK_BASE_TCPCONNECT_H__
#define __LINK_BASE_TCPCONNECT_H__

#include "EventLoop.h"
#include "Socket.h"
#include "Buffer.h"

#include <functional>
#include <memory>

namespace Link{
namespace Net{

class TcpConnect: public std::enable_shared_from_this<TcpConnect>{
public:
    using ptr = std::shared_ptr<TcpConnect>;

    using MessageCallback = std::function<void (std::shared_ptr<TcpConnect>, Buffer&)>;
    using CloseCallback = std::function<void (std::shared_ptr<TcpConnect>)>;
    using WriteCompleteCallback = std::function<void (std::shared_ptr<TcpConnect>)>;

    enum ConnectState{
        Disconnected,
        Connecting,
        Connected,
        Disconnecting
    };
public:
    TcpConnect(EventLoop* l,SocketAddr& a,int fd);
    ~TcpConnect();

    void setMessageCallback(const MessageCallback callback);
    void setCloseCallback(const CloseCallback callback);
    void setWriteCompleteCallback(const WriteCompleteCallback callback);

    SocketAddr& getAddr(){return addr;}
    const std::string getName() const{return name;}

    void setNodDelay();
    void shutdownWrite();

    void write(const char* data);
    void write(const std::string& data);
    void write(const void* data, uint32_t len);

    // void writeInLoop(const void* data, uint32_t len);
    
    void connectHandle();

private:
    EventLoop* loop;
    SocketAddr addr;
    std::string name;
    std::shared_ptr<Socket> socket;
    std::shared_ptr<IOEvent> event;

    MessageCallback messageCallback;
    CloseCallback closeCallback;
    WriteCompleteCallback writeCompleteCallback;

    Buffer readBuffer;
    Buffer writeBuffer;

    int state;

    TcpConnect& getRefer(){return *this;}

    void readEvent();
    void closeEvent();
    void writeEvent();
    void errorEvent();
};
}
}
#endif