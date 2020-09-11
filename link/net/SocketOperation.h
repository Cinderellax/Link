#ifndef __LINK_NET_SOCKETOPERATION_H__
#define __LINK_NET_SOCKETOPERATION_H__

#include <sys/types.h>
#include <string>
#include <netinet/in.h>
#include <linux/tcp.h>
#include <sys/uio.h>

namespace Link{

namespace Net{

class SocketOperation{
public:
    static const uint32_t Ipv4AddrAny;
    static int createNonblockingSocket();
    static int bind(int sockfd, const struct sockaddr_in* addr);
    static int listen(int sockfd);
    static int connect(int sockfd,const struct sockaddr* addr);
    static int accept(int sockfd, struct sockaddr_in* addr);

    static ssize_t read(int sockfd,void* buf,size_t count);
    static ssize_t readv(int sockfd,const struct iovec* iov,int iovcnt);
    static ssize_t write(int sockfd,const void* buf,size_t count);

    static void close(int sockfd);
    static int shutdownWrite(int sockfd);

    static void setTcpNoDelay(int sockfd);

    static void getAddrAnyIpv4(struct sockaddr_in& addr,uint16_t port);
    static bool toAddrIpv4(const std::string& addrstr,uint16_t port,struct sockaddr_in& addr);
    static bool toAddrIpv4(const std::string& addrstr,struct sockaddr_in& addr);

    static std::string ipToString(struct sockaddr_in addr);
};

}
}

#endif