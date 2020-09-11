#include "HttpServer.h"
#include "HttpContext.h"
#include <iostream>

using namespace Link::Net;
using namespace std;

HttpServer::HttpServer(EventLoop* loop, SocketAddr& addr):
TcpServer(loop,addr)
{

}

//析构函数什么也不做
HttpServer::~HttpServer()
{
}

void HttpServer::connectCallback(TcpConnect::ptr tcpConnect)
{

}

void HttpServer::messageCallback(TcpConnect::ptr tcpConnect, Buffer& buffer)
{
    HttpContext context;
    
    if(!context.parseRequest(&buffer, Timer::getNowTImeDate()))
    {

    }
    if(context.gotAll())
    {
        onRequest(tcpConnect, context.getRequest());
        context.reset();
    }
}

void HttpServer::writeCompleteCallback(TcpConnect::ptr tcpConnect)
{

}

void HttpServer::connectCloseCallback(TcpConnect::ptr tcpConnect)
{

}

//默认情况下的HTTPCallBack
//会直接传输状态码为404
void HttpServer::httpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::_404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

void HttpServer::onRequest( std::shared_ptr<TcpConnect> conn, const HttpRequest& req)
{
    const string& connection = req.getHeader("Connection");
    //这种情况使用的是短连接
    bool close = (connection == "close" ||(req.getVersion() == HttpRequest::Http10 && connection != "Keep-Alive"));
    HttpResponse response(close);
    //调用HttpCallback
    httpCallback(req, &response);
    //将其中的内容写到tcp中
    Buffer buf;
    response.addToBuffer(&buf);
    string str;
    buf.readAllAsString(str);
    conn->write(str);
    //如果是短连接，则直接关闭
    if (response.getCloseConnection())
    {
        conn->shutdownWrite();
    }
}