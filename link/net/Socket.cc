#include "Socket.h"

using namespace Link::Net;

Socket::Socket(int fd):
sockfd(fd)
{
}

Socket::~Socket()
{
    SocketOperation::close(sockfd);
}

int Socket::getFd()
{
    return sockfd;
}

void Socket::bind(SocketAddr& addr)
{
    SocketOperation::bind(sockfd,addr.getAddrPtr());
}

void Socket::listen()
{
    SocketOperation::listen(sockfd);
}

int Socket::accept(SocketAddr& addr)
{   
    struct sockaddr_in temp;
    int ret = SocketOperation::accept(sockfd,&temp);
    if(ret > 0)
        addr.setAddr(temp);
    return ret;
}

void Socket::setTcpNoDelay()
{
    SocketOperation::setTcpNoDelay(sockfd);
}

int Socket::shutDownWrite()
{
    return SocketOperation::shutdownWrite(sockfd);
}
