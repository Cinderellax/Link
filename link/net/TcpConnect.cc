#include "TcpConnect.h"
#include <thread>
#include <iostream>
#include <cstring>
#include <functional>

using namespace Link::Net;

TcpConnect::TcpConnect(EventLoop* l,SocketAddr& a,int fd):
loop(l),
addr(a),
name(addr.toString()),
socket(new Socket(fd)),
event(new IOEvent(loop,fd)),
state(Disconnecting)
{
    setNodDelay();
    loop->addEvent(event);
    event->setReadFunc(std::bind(&TcpConnect::readEvent,this));
    event->setCloseFunc(std::bind(&TcpConnect::closeEvent,this));
    event->setWriteFunc(std::bind(&TcpConnect::writeEvent,this));
    event->setErrorFunc(std::bind(&TcpConnect::errorEvent,this));
}

TcpConnect::~TcpConnect()
{
    event->disableAll();
    event->removeFromLoop();
}

void TcpConnect::setMessageCallback(const MessageCallback callback)
{
    messageCallback = callback;
}

void TcpConnect::setCloseCallback(const CloseCallback callback)
{
    closeCallback = callback;
}

void TcpConnect::setWriteCompleteCallback(const WriteCompleteCallback callback)
{
    writeCompleteCallback = callback;
}

void TcpConnect::setNodDelay()
{
    socket->setTcpNoDelay();
}

void TcpConnect::shutdownWrite()
{
    if(state == Connected)
    {
        state = Disconnecting;
        socket->shutDownWrite();
    }
}

void TcpConnect::readEvent()
{
    int error = 0;
    int n = readBuffer.readFromIO(event->getFd(),error);
    if(n > 0)
    {
        if(!!messageCallback)
        {
            messageCallback(shared_from_this(),readBuffer);
        }
    }
    else if(n ==0)
    {
        closeEvent();
    }
    else
    {
        std::cout<<"Error::Tcpconnect read error::"<<std::to_string(error);
        closeEvent();
    }
}

void TcpConnect::closeEvent()
{
    state = Disconnected;
    if(closeCallback)
    {
        closeCallback(shared_from_this());
    }
}

void TcpConnect::writeEvent()
{
    if(event->isWriting())
    {
        int n = SocketOperation::write(event->getFd(),writeBuffer.readIndexPtr(),writeBuffer.readableBytes());
        if(n > 0)
        {
            writeBuffer.clearReadIndex(n);
            if(writeBuffer.isEmpty())
            {
                event->enableWrite(false);
                if(!!writeCompleteCallback)
                {
                    //我怀疑这段有问题重复使用
                    std::shared_ptr<TcpConnect> tmp(this);
                    writeCompleteCallback(tmp);
                    if(writeCompleteCallback)
                        writeCompleteCallback(shared_from_this());
                }
            }
        }
        else
        {
            std::cout<<"error:: Tcpconnect write event error"<<std::endl;
        }
    }
    else
    {
        std::cout<<"Connect fd = "<<std::to_string(event->getFd())<<"is down, no more writing"<<std::endl;
    }
}

void TcpConnect::errorEvent()
{
    closeEvent();
}

void TcpConnect::connectHandle()
{
    state = Connected;
    event->enableRead();
    //epoll为电平触发
    //event->enablewrite
    event->enableError();
}

void TcpConnect::write(const char* data)
{
    int len = ::strlen(data);
    write((void*)data, len);
}

void TcpConnect::write(const std::string& data)
{
    write(&(*data.begin()),data.length());
}

void TcpConnect::write(const void* data,uint32_t length)
{
    int n = 0;
    size_t remaining = length;
    bool faultError = false;
    if(state == Disconnected)
    {
        std::cout<<"disconnected! in TcpConnect, give up write!"<<std::endl;
        return;
    }

    if(!event->isWriting() && writeBuffer.isEmpty())
    {
        n = SocketOperation::write(event->getFd(), data, length);
        if(n >= 0)
        {
            remaining = length - n;
            if(remaining == 0 && !!writeCompleteCallback)
            {
                writeCompleteCallback(shared_from_this());
            }
        }
        else
        {
            n = 0;
            if(errno != EWOULDBLOCK)
            {
                std::cout<<"write data error!"<<std::endl;
                if(errno == EPIPE || errno == ECONNRESET)
                {
                    faultError = true;
                }
            }
        }
    }
    if (!faultError && remaining > 0)
    {
    #if 0
        size_t oldLen = outputBuffer_.readableBytes();
        if (oldLen + remaining >= highWaterMark_
            && oldLen < highWaterMark_
            && highWaterMarkCallback_)
        {
        loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
        }
    #endif // 0
        writeBuffer.append(static_cast<const char*>(data)+n, remaining);
        if (!event->isWriting())
        {
            event->enableWrite(true);
        }
    }
}

// void TcpConnect::writeInLoop(const void* data, uint32_t len)
// {
//     loop->runInLoop([this,data,len]{
//         write(data,len);
//     });
// }

