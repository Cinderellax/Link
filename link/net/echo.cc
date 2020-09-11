#include "echo.h"
#include <iostream>

using namespace Link::Net;
using namespace std;


echo::echo(EventLoop* loop,SocketAddr& addr)
    :TcpServer(loop,addr)
{
}

void echo::connectCallback(std::shared_ptr<TcpConnect> tcpConnect)
{
    cout<<"new connect:"<<tcpConnect->getAddr().toString() <<"<count>" <<getConnectCount()<<endl;
}
void echo::messageCallback(std::shared_ptr<TcpConnect> tcpConnect, Buffer& buffer)
{
    cout<<"thread id:"<<std::this_thread::get_id()<<endl;
    string addr = tcpConnect->getAddr().toString();
    string data = "HTTP/1.1 400 BAD REQUEST\r\n\r\n";
    //buffer.readAllAsString(data);
    //cout<<"receive data form "<<addr<<":"<<data<<endl;
    //base::Log::OUT(base::Log::Info)<<"receive data form "<<addr<<":"<<data;
    //data+=" kick back!";
    tcpConnect->write(data);
}
void echo::writeCompleteCallback(std::shared_ptr<TcpConnect> tcpConnect)
{
    cout<<"thread id:"<<std::this_thread::get_id()<<endl;
    string addr = tcpConnect->getAddr().toString();
    cout<<addr<<":"<<"write complet "<<endl;
    //base::Log::OUT(base::Log::Info)<<addr<<":"<<"write complet ";
}

void echo::connectCloseCallback( std::shared_ptr<TcpConnect> connect)
{
    string addr = connect->getAddr().toString();
    cout<<"close connect :"<<addr<<endl;
    //base::Log::OUT(base::Log::Info)<<"close connect : "<<addr;
}