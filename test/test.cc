#include "../link/net/echo.h"
#include <iostream>
#include "../link/net/EventLoop.h"
//#include "../net/EventLoopThread.h"

using namespace std;
using namespace Link::Net;


int main()
{
    //创建主循环事件分发器
    EventLoop loop;

    //监听地址和端口
    SocketAddr addr(10002);

    //创建服务器
    echo server(&loop,addr);

    //服务器开始运行
    server.start();

    //事件循环机制运行
    loop.run();
}
