#ifndef __LINK_NET_TCPACCEPT_H__
#define __LINK_NET_TCPACCEPT_H__

#include "EventLoop.h"
#include "Socket.h"

#include <functional>

namespace Link{
namespace Net{

class TcpAccept{
public:
    using ptr = std::shared_ptr<TcpAccept>;
    using NewConnectCallback = std::function<void (int sockfd, SocketAddr&)>;

    TcpAccept(EventLoop* loop, SocketAddr& addr);
    ~TcpAccept();
    void listen();
    bool isListen();
    void setNewConnectCallback(const NewConnectCallback callback);
private:
    EventLoop* mLoop;
    std::shared_ptr<Socket> socket;
    std::shared_ptr<IOEvent> event;
    bool listening;
    NewConnectCallback newConnectCallback;
    void acceptHandle();
};
}
}

#endif