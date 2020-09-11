#include "TcpServer.h"
#include <thread>
#include <iostream>
#include <functional>

using namespace Link::Net;

TcpServer::TcpServer(EventLoop* loop,SocketAddr& addr):
mloop(loop),
tcpAddr(addr),
tcpAccept(new TcpAccept(loop, addr)),
isStart(false),
threadPool(new EventLoopThreadPool(loop))
{
    tcpAccept->setNewConnectCallback(std::bind(&TcpServer::newConnected,this,std::placeholders::_1,std::placeholders::_2));
}

void TcpServer::start()
{
    threadPool->init();
    tcpAccept->listen();
    isStart = true;
}

void TcpServer::newConnected(int sockfd,SocketAddr& addr)
{
    std::cout<<"new connect addr: "<<tcpAddr.toString()<<"cnt: "<<std::to_string(getConnectCount());
    EventLoop* loop = threadPool->getOneLoopFromPool();
    TcpConnect::ptr tcpConnect(new TcpConnect(loop,addr,sockfd));
    addConnect(addr.toString(),tcpConnect);
    tcpConnect->setMessageCallback(std::bind(&TcpServer::messageCallback,this,std::placeholders::_1,std::placeholders::_2));
    tcpConnect->setCloseCallback(std::bind(&TcpServer::connectCloseEvent,this,std::placeholders::_1));
    tcpConnect->connectHandle();
    connectCallback(tcpConnect);
}

void TcpServer::addConnect(std::string name, TcpConnect::ptr connect)
{
    connectPool.insert(std::pair<std::string,TcpConnect::ptr>(name, connect));
}

void TcpServer::addConnect(std::string name, TcpConnect* connect)
{
    TcpConnect::ptr connectPtr(connect);
    addConnect(name,connectPtr);
}

void TcpServer::removeConnect(std::string name)
{
    connectPool.erase(name);
}

long TcpServer::getConnectCount() const
{
    return connectPool.size();
}

bool TcpServer::haveConnect(std::string name)
{
    return !!connectPool.count(name);
}

void TcpServer::connectCloseEvent(TcpConnect::ptr connect)
{
    connectCloseCallback(connect);
    removeConnect(connect->getName());
}