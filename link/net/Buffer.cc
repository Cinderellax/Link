#include "Buffer.h"
#include "SocketOperation.h"
#include <errno.h>
#include <sys/uio.h>

using namespace Link::Net;

const uint32_t Buffer::InitSize = 1024;

Buffer::Buffer(uint32_t size):
readIndex(0),
writeIndex(0),
buffer(size)
{
}

int Buffer::readFromIO(int fd, int& errorNo)
{
    char extraBuf[65536];
    struct iovec vec[2];
    uint32_t writeable = writeableBytes();
    vec[0].iov_base = &(*buffer.begin())+writeIndex;
    vec[0].iov_len = writeable;
    vec[1].iov_base = extraBuf;
    vec[1].iov_len = sizeof(extraBuf);

    uint32_t iovcnt = (writeable < sizeof(extraBuf))?2:1;
    int n = SocketOperation::readv(fd,vec,iovcnt);
    if(n<0)
    {
        errorNo = errno;
    }
    else if((uint32_t)n<=writeable)
    {
        writeIndex += n;
    }
    else
    {
        writeIndex = buffer.size();
        append(extraBuf, n-writeable);
    }
    return n;
}

uint32_t Buffer::writeableBytes()
{
    return buffer.size()-writeIndex;
}

uint32_t Buffer::readableBytes()
{
    return writeIndex-readIndex;
}

bool Buffer::isReadable()
{
    return (writeIndex>readIndex);
}

bool Buffer::isEmpty()
{
    return !isReadable();
}

void Buffer::ensureWriteableBytes(std::size_t len)
{
    if(writeableBytes() < len)
    {
        buffer.resize(writeIndex+len);
    }
}

void Buffer::append(const char* data, std::size_t len)
{
    ensureWriteableBytes(len);
    std::copy(data,data+len,buffer.begin()+writeIndex);
    writeIndex += len;
}

void Buffer::append(const std::string& str)
{
    const char* data = &(*str.begin());
    append(data,str.size());
}

void Buffer::swap(Buffer& rhs)
{
    buffer.swap(rhs.buffer);
    std::swap(readIndex, rhs.readIndex);
    std::swap(writeIndex, rhs.writeIndex);
}

const uint8_t* Buffer::readIndexPtr() const
{
    return bufferPtr()+readIndex;
}

void Buffer::resetIndex()
{
    readIndex = 0;
    writeIndex = 0;
}

void Buffer::clearReadIndex(uint32_t len)
{
    if(len < readableBytes())
    {
        readIndex += len;
    }
    else
    {
        resetIndex();
    }
}

int Buffer::readAllAsString(std::string& readBuf)
{
    return readAsString(readBuf,readableBytes());
}

int Buffer::readAsString(std::string& readBuf, uint32_t len)
{
    len = std::min(readableBytes(),len);
    readBuf.assign((const char*)readIndexPtr(), len);
    clearReadIndex(len);
    return len;
}

int Buffer::readAllAsByte(std::vector<uint8_t>& data)
{
    return readBytes(data, readableBytes());
}

int Buffer::readBytes(std::vector<uint8_t>& data, uint32_t len)
{
    len = std::min(readableBytes(),len);
    data.assign(readIndexPtr(),readIndexPtr()+len);
    clearReadIndex(len);
    return len;
}

const char* Buffer::findCRLF() const
{
    char kCRLF[] = "\r\n";
    // const char* writePtr = bufferPtr()+writeIndex;
    const char* crlf = std::search((char*)readIndexPtr(),(char *)bufferPtr()+writeIndex, kCRLF, kCRLF+2);
    return crlf == ((char*)bufferPtr()+writeIndex) ? nullptr : crlf;
}

void Buffer::retrieveUntil(const char* end)
{
    if(!(readIndexPtr() <= (uint8_t*)end))
    {
        std::cout<<"error retrieveUntil"<<std::endl;
        return ;
    }
    if(!(end <=  ((char*)bufferPtr()+writeIndex) ))
    {
        std::cout<<"error retrieveUntil"<<std::endl;
        return ;
    }
    clearReadIndex((uint8_t*)end - readIndexPtr());
}


