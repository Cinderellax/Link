#ifndef __LINK_HTTP_HTTPREQUEST_H__
#define __LINK_HTTP_HTTPREQUEST_H__

#include <iostream>
#include <map>

namespace Link{
namespace Net{

class HttpRequest{
//要不要在里面支持const函数
public:
    //支持的HTTP方法
    enum Method
    {
        //Invalid方法设置为0
        Invalid,
        Get,
        Post,
        Head,
        Put
        //Delete,
    };
    //支持的HTTP版本
    enum Version
    {
        UnKnown,
        Http10,
        Http11
    };
public:
    //构造函数
    HttpRequest();
    //设置版本
    inline void setVersion(const Version& v);
    //获取版本
    inline Version getVersion() const;
    /*
     *设置http请求方法的类型
     *传递char*类型的字符串开头和结尾（左闭右开区间）
     * 返回一个设定成功的bool类型
     */
    inline bool setMethod(const char* start, const char* end);
    //获得方法
    inline Method getMethod() const;
    //将method转换为char*类型的字符串
    inline const char* methodString();
    //通过char*类型设置path
    inline void setPath(const char* start, const char* end);
    //获得path
    inline const std::string& getPath() const;
    //设置query
    inline void setQuery(const char* start, const char* end);
    //获取query
    inline const std::string& getQuery() const;
    //设置获取报文的时间
    inline void setReceiveTime(const std::string& t);
    //获取报文到达的时间
    inline const std::string& getReceiveTime() const;
    //添加头
    //start到colon指定了域的位置
    //colon到end指定了值的位置
    inline bool addHeader(const char* start, const char* colon, const char* end);
    //获取头
    //参数是一个field
    //获取这个域的值
    inline std::string getHeader(const std::string& field) const;
    //获取所有的头
    //返回的是map
    const std::map<std::string, std::string>& getHeaders() const;
    //交换
    void swap(HttpRequest& that);
private:
    //方法
    Method method;
    //uri中解析出的路径，应该是相对路径
    std::string path;
    //版本
    Version version;
    //查询query
    std::string query;
    //时间
    std::string receiveTime;
    //头部的分区map
    std::map<std::string, std::string> headers;
};

}
}
#endif