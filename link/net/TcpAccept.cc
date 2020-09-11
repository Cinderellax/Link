#include "TcpAccept.h"

using namespace Link::Net;

TcpAccept::TcpAccept(EventLoop* loop,SocketAddr& addr):
mLoop(loop),
socket(new Socket(SocketOperation::createNonblockingSocket())),
event(new IOEvent(loop,socket->getFd())),
listening(false)
{
    loop->addEvent(event);
    socket->bind(addr);
    event->setReadFunc(std::bind(&TcpAccept::acceptHandle,this));
}

TcpAccept::~TcpAccept()
{
    event->disableAll();
    event->removeFromLoop();
}

void TcpAccept::listen()
{
    socket->listen();
    event->enableRead();
}

bool TcpAccept::isListen()
{
    return listening;
}

void TcpAccept::setNewConnectCallback(const NewConnectCallback callback)
{
    newConnectCallback = callback;
}

void TcpAccept::acceptHandle()
{
    SocketAddr connectAddr;
    int connectfd = -1;
    if((connectfd = socket->accept(connectAddr))>0)
    {
        if(!!newConnectCallback)
        {
            newConnectCallback(connectfd,connectAddr);
        }
        else
        {
            ::close(connectfd);
        }
    }
}