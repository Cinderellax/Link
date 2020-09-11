#ifndef __LINK_NET_BUFFER__
#define __LINK_NET_BUFFER__

#include <vector>
#include <stdint.h>
#include <iostream>
#include <algorithm>

namespace Link{
namespace Net{

class Buffer{
public:
    static const uint32_t InitSize;
    Buffer(uint32_t size = InitSize);
    ~Buffer(){};
    int readFromIO(int fd, int& errorNo);
    uint32_t writeableBytes();
    uint32_t readableBytes();
    bool isReadable();
    bool isEmpty();
    void ensureWriteableBytes(std::size_t len);
    void append(const char* data, std::size_t len);
    void append(const std::string& str);
    void swap(Buffer& rhs);

    const uint8_t* readIndexPtr() const;
    void resetIndex();
    void clearReadIndex(uint32_t len);
    int readAllAsString(std::string& readBuf);
    int readAsString(std::string& readBuf, uint32_t len);
    int readAllAsByte(std::vector<uint8_t>& data);
    int readBytes(std::vector<uint8_t>& data, uint32_t len);
    //用于在http报文中寻找CRLF
    const char* findCRLF() const;
    //清空在此之前的所有
    void retrieveUntil(const char* end);
private:
    uint32_t readIndex;
    uint32_t writeIndex;
    std::vector<uint8_t> buffer;
    const uint8_t* bufferPtr() const{return &(*buffer.begin());}
};
}
}
#endif