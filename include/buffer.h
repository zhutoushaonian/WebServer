#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <iostream>
static const size_t kInitialSize = 1024;
class buffer
{
private:
    std::vector<char> _buffer;
    size_t _readIndex;
    size_t _writeIndex;

private:
    void makeSpace(size_t len)
    {
        if (writeableBytes() < len)
        {
            _buffer.resize(_writeIndex + len);
        }
        else
        {
            size_t readable = readableBytes();
            std::copy(begin() + _readIndex,
                      begin() + _writeIndex,
                      begin());
            _readIndex = 0;
            _writeIndex = _readIndex + readable;
            assert(readable == readableBytes());
        }
    }

public:
    buffer() : _buffer(kInitialSize), _readIndex(0), _writeIndex(0)
    {
        assert(readableBytes() == 0);
        assert(writeableBytes() == kInitialSize);
    };
    ~buffer(){

    };

    int returnReadIndex()
    {
        return _readIndex;
    }
    std::string returnString()
    {
        return std::string(_buffer.begin(), _buffer.end());
    }
    // 可读大小
    size_t readableBytes() const
    {
        // std::cout << "_writeIndex:" << _writeIndex << std::endl;
        // std::cout << "_readIndex:" << _readIndex << std::endl;
        return _writeIndex - _readIndex;
    }
    // 可写大小
    size_t writeableBytes() const { return _buffer.size() - _writeIndex; }
    // 预留空间大小
    // size_t prependableBytes() const { return _readIndex; }

    // 返回第一个可读位置
    const char *peek() const { return begin() + _readIndex; }

    // 返回可写位置
    char *beginWrite()
    {
        return begin() + _writeIndex;
    }
    const char *beginWrite() const
    {
        return begin() + _writeIndex;
    }
    // 写入数据后移动_writeIndex
    void hasWrite(size_t len)
    {
        _writeIndex += len;
    }
    // todo private
    char *begin() { return &(*_buffer.begin()); }
    const char *begin() const { return &(*_buffer.begin()); }
    // 找\r\n的位置
    const char *findCRLF() const
    {
        const char CRLF[] = "\r\n";
        // 返回第一个"\r\n"的位置
        // const char* s = peek();
        // const char* end = beginWrite() - 1;
        const char *crlf = std::search(peek(), beginWrite(), CRLF, CRLF + 2);

        return crlf == beginWrite() ? nullptr : crlf;
    }

    const char *findCRLF(const char *start) const
    {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const char CRLF[] = "\r\n";
        // 返回第一个"\r\n"的位置
        const char *crlf = std::search(peek(), beginWrite(), CRLF, CRLF + 2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    // 读数据
    void retrieve(size_t len)
    {
        assert(len <= readableBytes());
        _readIndex += len;
    }
    // 读出到end为止
    void retrieveUntil(const char *end)
    {
        // std::cout << "end : " << end - begin() << std::endl;
        // std::cout << "peek : " << peek() - begin() << std::endl;
        assert(peek() <= end); // 这里报错
        assert(end <= beginWrite());
        // std::cout << end - peek() << std::endl;
        retrieve(end - peek()); // 这里导致乱序了
    }
    // 读出全部数据
    void retrieveAll()
    {
        _readIndex = 0;
        _writeIndex = 0;
    }
    // 以string形式读出全部数据
    std::string retrieveAsString()
    {
        std::string str(peek(), readableBytes());
        retrieveAll();
        return str;
    }

    // to
    //  std::string utilRetrieveAsString()
    //  {
    //      std::string str(peek(), readableBytes());

    //     return str;
    // }
    // 写入数据 支持四种类型的写入

    // 确保可写
    void ensureWriteableBytes(size_t len)
    {
        if (writeableBytes() < len)
        {
            makeSpace(len);
            assert(writeableBytes() >= len);
        }
    }

    void append(const char *data, size_t len)
    {
        ensureWriteableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWrite(len);
    }

    void append(const std::string &str)
    {
        append(str.data(), str.length());
    }

    void append(const void *data, size_t len)
    {
        append(static_cast<const char *>(data), len);
    }

    void append(const buffer &otherBuffer)
    {
        append(otherBuffer.peek(), otherBuffer.readableBytes());
    }

    ssize_t readFd(int fd, int *savedErrno);
    ssize_t writeFd(int fd, int *savedErrno);
};
