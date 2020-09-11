#ifndef __LINK_NET_SOCKET_H__
#define __LINK_NET_SOCKET_H__

#include "SocketOperation.h"
#include "SocketAddr.h"

namespace Link{
namespace Net{

class Socket{
public:
    Socket(int fd);
    ~Socket();
    int getFd();
    void bind(SocketAddr& addr);
    void listen();
    int accept(SocketAddr& addr);
    void setTcpNoDelay();
    int shutDownWrite();
private:
    int sockfd;
};
}
}

#endif