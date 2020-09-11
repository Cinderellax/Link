#include "HttpRequest.h"

using namespace Link::Net;

using Version = HttpRequest::Version;
using Method = HttpRequest::Method;

HttpRequest::HttpRequest():
method(Invalid),
version(UnKnown)
{}

void HttpRequest::setVersion(const Version& v)
{
    version = v;
}

Version HttpRequest::getVersion() const
{
    return version;
}

bool HttpRequest::setMethod(const char* start, const char* end)
{
    if(!method) return false;
    std::string m(start, end);
    //下面的部分映射到method的枚举类型
    if (m == "GET")
    {
        method = Get;
    }
    else if (m == "POST")
    {
        method = Post;
    }
    else if (m == "HEAD")
    {
        method = Head;
    }
    else if (m == "PUT")
    {
        method = Put;
    }
    // else if (m == "DELETE")
    // {
    //     method = Delete;
    // }
    else
    {
        method = Invalid;
    }
    return !!method;
}

Method HttpRequest::getMethod() const
{
    return method;
}

const char* HttpRequest::methodString()
{
    const char* result = "UNKNOWN";
    switch(method)
    {
        case Get:
            result = "GET";
            break;
        case Post:
            result = "POST";
            break;
        case Head:
            result = "HEAD";
            break;
        case Put:
            result = "PUT";
            break;
        // case Delete:
        //     result = "DELETE";
        //     break;
        default:
            break;
    }
    return result;
}

void HttpRequest::setPath(const char* start, const char* end)
{
    path.assign(start,end);
}

const std::string& HttpRequest::getPath() const
{
    return path;
}

void HttpRequest::setQuery(const char* start, const char* end)
{
    query.assign(start, end);
}

const std::string& HttpRequest::getQuery() const
{
    return query;
}

void HttpRequest::setReceiveTime(const std::string& t)
{
    receiveTime = t;
}

const std::string& HttpRequest::getReceiveTime() const
{
    return receiveTime;
}

bool HttpRequest::addHeader(const char* start, const char* colon, const char* end)
{
    //colon指向:，左闭右开区间
    std::string field(start, colon);
    //:之后是一个空格
    //循环的去除冒号之后的所有空格
    for(++colon; colon<end&&isspace(*colon); ++colon);
    //检查到末尾全部是空格的情况下是失败的
    if(colon == end) return false;
    std::string value(colon, end);
    for(;   \
            !value.empty()&&isspace(value[value.size()-1]); \
                    value.resize(value.size()-1));
    headers[field] = value;
}

std::string HttpRequest::getHeader(const std::string& field) const
{
    std::string result;
    std::map<std::string, std::string>::const_iterator it = headers.find(field);
    if (it != headers.end())
    {
        result = it->second;
    }
    return result;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const
{
    return headers;
}

void HttpRequest::swap(HttpRequest& that)
{
    std::swap(method, that.method);
    std::swap(version, that.version);
    path.swap(that.path);
    query.swap(that.query);
    receiveTime.swap(that.receiveTime);
    headers.swap(that.headers);
}
