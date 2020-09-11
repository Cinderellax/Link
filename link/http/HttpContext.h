#ifndef __LINK_HTTP_HTTPCONTEXT_H__
#define __LINK_HTTP_HTTPCONTEXT_H__

#include "HttpRequest.h"

namespace Link{
namespace Net{

class Buffer;

class HttpContext{
public:
    //解析的阶段
    enum ParseState
    {
        ExpectRequestLine,
        ExpectHeaders,
        ExpectBody,
        GotAll
    };
public:
    //构造函数
    inline HttpContext();
    //判断是否解析完毕
    inline bool gotAll();
    //将状态转换为初始状态
    //就是state的转换为初始状态
    //同时将需要解析的报文也清空
    inline void reset();
    //返回一个引用
    inline HttpRequest& getRequest();
    //解析请求
    inline bool parseRequest(Buffer* buf, std::string receiveTime);
private:
    //解析请求种的请求行部分
    bool processRequestLine(const char* begin, const char* end);

    ParseState state;
    HttpRequest request;
};
}
}

#endif