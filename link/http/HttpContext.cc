#include "../net/Buffer.h"
#include "HttpContext.h"
#include <algorithm>

using namespace Link::Net;

//构造函数中将解析的状态设置为初始的需要解析报文的状态
HttpContext::HttpContext():
state(ExpectRequestLine)
{}

bool HttpContext::gotAll()
{
    return state == GotAll;
}

void HttpContext::reset()
{
    state = ExpectRequestLine;
    HttpRequest temp;
    //感觉这边实现的不是很好
    //感觉可以在HttpRequest中定义一个reset函数比较实在
    request.swap(temp);
}

HttpRequest& HttpContext::getRequest()
{
    return request;
}

//解析请求
bool HttpContext::parseRequest(Buffer* buf, std::string receiveTime)
{
    bool res = true;
    bool hasMore = true;
    while(hasMore)
    {
        //有限状态机
        switch(state)
        {
            //解析请求行
            case ExpectRequestLine:
                {
                    const char* crlf = buf->findCRLF();
                    if(crlf)
                    {
                        res = processRequestLine((const char*)(buf->readIndexPtr()),crlf);
                        if(res)
                        {
                            request.setReceiveTime(receiveTime);
                            buf->retrieveUntil(crlf + 2);
                            state = ExpectHeaders;
                            continue;
                        }
                    }
                    hasMore = false;
                }
                break;
            //解析头部信息
            case ExpectHeaders:
                {
                    const char* crlf = buf->findCRLF();
                    if (crlf)
                    {
                        const char* colon = std::find((const char*)buf->readIndexPtr(), crlf, ':');
                        //如果出现CRLF表示没有主体信息了（可能就是HEAD）
                        if (colon != crlf)
                        {
                            request.addHeader((const char*)buf->readIndexPtr(), colon, crlf);
                        }
                        else
                        {
                            state = GotAll;
                            hasMore = false;
                        }
                        buf->retrieveUntil(crlf + 2);
                        continue;
                    }
                    hasMore = false;
                }
                break;
            case ExpectBody:
                {
                    //主体信息部分不解析，主体部分的信息保留在buffer中
                }
                break;
            default:
                break;
        }
    }
    return res;
}

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');
    //解析出Method
    if (space != end && request.setMethod(start, space))
    {
        start = space+1;
        space = std::find(start, end, ' ');
        if (space != end) 
        {
            const char* question = std::find(start, space, '?');
            //解析路径，若有query则也会解析
            if (question != space)
            {
                request.setPath(start, question);
                request.setQuery(question, space);
            }
            else
            {
                request.setPath(start, space);
            }
            start = space+1;
            //协议版本
            succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
            if (succeed)
            {
                //设置不同的协议版本
                if (*(end-1) == '1')
                {
                    request.setVersion(HttpRequest::Http11);
                }
                else if (*(end-1) == '0')
                {
                    request.setVersion(HttpRequest::Http10);
                }
                else
                {
                    succeed = false;
                }
            }
        }
    }
    return succeed;
}