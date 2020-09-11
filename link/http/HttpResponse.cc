#include <stdio.h>
#include "HttpResponse.h"
#include "../net/Buffer.h"


using namespace Link::Net;

HttpResponse::HttpResponse(const bool& close):
httpVersion("HTTP/1.1"),
statusCode(_UnKnown),
//另外三个使用默认初始化操作
//使用传入构造函数的参数来初始化close状态
closeConnection(close)
{
}

void HttpResponse::setStatusCode(const StatusCode& code)
{
    statusCode = code;
}

void HttpResponse::setStatusMessage(const std::string& message)
{
    statusMessage = message;
}
//设置关闭连接
void HttpResponse::setCloseConnection(const bool& on)
{
    closeConnection = on;
}
//获取关闭连接的情况
bool HttpResponse::getCloseConnection()
{
    return closeConnection;
}
//设置content类型
void HttpResponse::setContentType(const std::string& contentType)
{
    addHeader("Content-Type", contentType);
}

void HttpResponse::addHeader(const std::string& key, const std::string& value)
{
    headers[key] = value;
}

void HttpResponse::setBody(const std::string& body)
{
    this->body = body;
}

void HttpResponse::addToBuffer(Buffer* output)
{
    char buf[32] = {0};
    //添加协议版本的同时添加状态码
    snprintf(buf,sizeof(buf),"HTTP/1.1 %d ",statusCode);
    output->append(buf);
    //添加状态文字
    output->append(statusMessage);
    //添加CRLF
    output->append("\r\n");
    //添加响应头信息
    //判断是否为最后的信息并且依次来使用不同的选项
    if(closeConnection)
    {
        output->append("Connection: close\r\n");
    }
    else
    {
        //可能不支持head方法
        //这里一旦不为空则添加 body部分的长度
        snprintf(buf, sizeof(buf), "Content-Length: %zd\r\n",body.size());
        output->append(buf);
        //keepalive机制的详细
        output->append("Connection: Keep-Alive\r\n");   
    }
    //添加头部的信息（所有的）
    for(auto it : headers)
    {
        output->append(it.first);
        output->append(": ");
        output->append(it.second);
        //注意每个头部信息中都需要空行
        output->append("\r\n");
    }

    output->append("\r\n");
    output->append(body);
}