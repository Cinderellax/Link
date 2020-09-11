#include "SocketAddr.h"
#include "SocketOperation.h"

using namespace Link::Net;

//默认构造函数设置ip为本机地址，端口为0
SocketAddr::SocketAddr(uint16_t port)
{
    SocketOperation::getAddrAnyIpv4(sockaddr,port);
}

SocketAddr::SocketAddr(struct sockaddr_in addr)
:sockaddr(addr)
{
}

SocketAddr::SocketAddr(const std::string& addrPort)
:SocketAddr(std::stoi(addrPort))
{
}

SocketAddr::SocketAddr(const std::string& addr,uint16_t port)
{
    SocketOperation::toAddrIpv4(addr,port,sockaddr);
}

void SocketAddr::setAddr(struct sockaddr_in addr)
{
    sockaddr = addr;
    //使用placment new在this指针上调用构造函数
    //new (this) SockAddr(addr);
}

struct sockaddr_in SocketAddr::getAddr()
{
    return sockaddr;
}

struct sockaddr_in* SocketAddr::getAddrPtr()
{
    return &sockaddr;
}

std::string SocketAddr::toString()
{
    return SocketOperation::ipToString(sockaddr);
}

