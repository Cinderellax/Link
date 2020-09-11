#ifndef __LINK_TEST_ECHO_H__
#define __LINK_TEST_ECHO_H__

#include "TcpServer.h"

class echo : public Link::Net::TcpServer
{
public:
    echo(Link::Net::EventLoop* loop, Link::Net::SocketAddr& addr);
    ~echo(){}
    //考虑到tcpConnect可能关闭后内存被自动释放，传shared_ptr比较安全。
    void connectCallback(std::shared_ptr<Link::Net::TcpConnect> tcpConnect) override;
    void messageCallback(std::shared_ptr<Link::Net::TcpConnect>, Link::Net::Buffer&) override;
    void writeCompleteCallback(std::shared_ptr<Link::Net::TcpConnect> tcpConnect) override;
    void connectCloseCallback( std::shared_ptr<Link::Net::TcpConnect>) override;
};

#endif