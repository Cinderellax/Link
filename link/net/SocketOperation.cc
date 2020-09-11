#include "SocketOperation.h"

#include <vector>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace Link::Net;

const uint32_t SocketOperation::Ipv4AddrAny = htonl(INADDR_ANY);

int SocketOperation::createNonblockingSocket()
{
    int sockfd = ::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,IPPROTO_TCP);
    if(sockfd < 0)
    {
        std::cout<<"create nonblock socket error"<<std::endl;
    }
    return sockfd;
}

int SocketOperation::bind(int sockfd,const struct sockaddr_in* addr)
{
    int ret = ::bind(sockfd,(struct sockaddr*)addr,sizeof(struct sockaddr));
    if(ret < 0)
    {
        std::cout<<"bind socket error!"<<std::endl;
    }
    return ret;
}

int SocketOperation::listen(int sockfd)
{
    int ret = ::listen(sockfd,SOMAXCONN);
    if(ret < 0)
    {
        std::cout<<"listen socket error!"<<std::endl;
    }
    return ret;
}

int SocketOperation::connect(int sockfd, const struct sockaddr* addr)
{
    int ret = ::connect(sockfd,addr,static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    if(ret < 0)
    {
        std::cout<<"connect error!"<<std::endl;
    }
    return ret;
}

int SocketOperation::accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int ret = ::accept(sockfd,(struct sockaddr*)addr, &addrlen);
    if(ret < 0)
    {
        std::cout<<"accept socket error!"<<std::endl;
    }
    return ret;
}

ssize_t SocketOperation::read(int sockfd,void* buf,size_t count)
{
    return ::read(sockfd,buf,count);
}

ssize_t SocketOperation::readv(int sockfd,const struct iovec* iov,int iovcnt)
{
    return ::readv(sockfd,iov,iovcnt);
}

ssize_t SocketOperation::write(int sockfd,const void* buf,size_t count)
{
    return ::write(sockfd,buf,count);
}

void SocketOperation::close(int sockfd)
{
    if(::close(sockfd))
    {
        std::cout<<"close sockfd error!"<<std::endl;
    }
}

int SocketOperation::shutdownWrite(int sockfd)
{
    int ret = ::shutdown(sockfd,SHUT_WR);
    if(ret < 0)
    {
        std::cout<<"shutdown sockfd error!"<<std::endl;
    }
    return ret;
}

void SocketOperation::setTcpNoDelay(int sockfd)
{
    int opt = 1;
    ::setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,&opt,sizeof(opt));
}

void SocketOperation::getAddrAnyIpv4(struct sockaddr_in& addr,uint16_t port)
{
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = Ipv4AddrAny;
}

bool SocketOperation::toAddrIpv4(const std::string& addrstr,uint16_t port,struct sockaddr_in& addr)
{
    std::vector<int> ip;
    size_t first = 0;
    while(true)
    {
        auto last = addrstr.find('.',first);
        if(last == addrstr.npos)
        {
            ip.push_back(std::stoi(addrstr.substr(first,addrstr.size()-first)));
            break;
        }
        ip.push_back(std::stoi(addrstr.substr(first,last-first)));
        first = last+1;
    }
    if(ip.size() != 4)  return false;
    uint32_t addr32 = 0;
    for(auto i:ip)
    {
        if(i<0 || i>255)
            return false;
        addr32 <<= 8;
        addr32 |= i;
    }
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = addr32;
    return true;
}

bool SocketOperation::toAddrIpv4(const std::string& addrstr,struct sockaddr_in& addr)
{
    auto pos = addrstr.find(':');
    if(pos==addrstr.npos)
        return false;
    return toAddrIpv4(addrstr.substr(0,pos),std::stoi(addrstr.substr(pos+1)),addr);
}

std::string SocketOperation::ipToString(struct sockaddr_in addr)
{
    std::stringstream stream;
    uint8_t* addrArray = (uint8_t*)&addr.sin_addr.s_addr;
    for(int i=0;i<4;++i)
    {
        stream<<(uint16_t)addrArray[i];
        if(i!=3)
            stream<<".";
    }
    stream<<":"<<(((addr.sin_port<<8)&0x00ffff)|(addr.sin_port>>8));
    return stream.str();
}