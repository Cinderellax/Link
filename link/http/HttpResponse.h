#ifndef __LINK_HTTP_HTTPRESPONSE_H__
#define __LINK_HTTP_HTTPRESPONSE_H__

#include <map>
#include <iostream>

namespace Link{
namespace Net{

class Buffer;
class HttpResponse
{
public:
    //响应报文:状态码
    enum StatusCode
    {
        _UnKnown = 0,
        _200OK = 200,
        _301MovedPermanently = 301,
        _400BadRequest = 400,
        _404NotFound = 404
    };
    //构造函数
    inline HttpResponse(const bool& close);
    //设置状态码
    inline void setStatusCode(const StatusCode& code);
    //设置状态信息
    inline void setStatusMessage(const std::string& message);
    //设置和获取关闭连接的情况
    inline void setCloseConnection(const bool& on);
    inline bool getCloseConnection();
    //设置content类型
    inline void setContentType(const std::string& type);
    //添加头
    inline void addHeader(const std::string& key, const std::string& value);
    //设置主体信息
    inline void setBody(const std::string& body);
    //将内容添加到Buffer中，以便使用tcp传输
    void addToBuffer(Buffer* output);

private:
    //协议版本
    const std::string httpVersion;
    //状态码
    StatusCode statusCode;
    //状态文字
    std::string statusMessage;
    //响应头部信息
    std::map<std::string, std::string> headers;
    //主体信息
    std::string body;
    //关闭信息？？？
    bool closeConnection;
};
}
}
#endif