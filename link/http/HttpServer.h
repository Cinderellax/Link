#ifndef __LINK_HTTP_HTTPSERVER_H__
#define __LINK_HTTP_HTTPSERVER_H__

#include "../net/TcpServer.h"
#include "HttpResponse.h"
#include "HttpRequest.h"

namespace Link{
namespace Net{
//继承自TcpServer
//实现为虚基类
class HttpServer : public TcpServer
{
public:
    //构造函数中需要参数来初始化基类
    HttpServer(EventLoop* loop, SocketAddr& addr);
    //使用虚析构函数方便HttpServer被继承
    ~HttpServer();
private:
    //下面的是四个被重写的事件（所谓的三个半事件）
    virtual void connectCallback(TcpConnect::ptr tcpConnect);
    virtual void messageCallback(TcpConnect::ptr tcpConnect, Buffer& buffer);
    virtual void writeCompleteCallback(TcpConnect::ptr tcpConnect);
    virtual void connectCloseCallback(TcpConnect::ptr tcpConnect);

    virtual void httpCallback(const HttpRequest&, HttpResponse*);

    void onRequest( std::shared_ptr<TcpConnect> conn, const HttpRequest& req);    

};

}
}

#endif