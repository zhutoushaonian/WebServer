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
    // �ɶ���С
    size_t readableBytes() const
    {
        // std::cout << "_writeIndex:" << _writeIndex << std::endl;
        // std::cout << "_readIndex:" << _readIndex << std::endl;
        return _writeIndex - _readIndex;
    }
    // ��д��С
    size_t writeableBytes() const { return _buffer.size() - _writeIndex; }
    // Ԥ���ռ��С
    // size_t prependableBytes() const { return _readIndex; }

    // ���ص�һ���ɶ�λ��
    const char *peek() const { return begin() + _readIndex; }

    // ���ؿ�дλ��
    char *beginWrite()
    {
        return begin() + _writeIndex;
    }
    const char *beginWrite() const
    {
        return begin() + _writeIndex;
    }
    // д�����ݺ��ƶ�_writeIndex
    void hasWrite(size_t len)
    {
        _writeIndex += len;
    }
    // todo private
    char *begin() { return &(*_buffer.begin()); }
    const char *begin() const { return &(*_buffer.begin()); }
    // ��\r\n��λ��
    const char *findCRLF() const
    {
        const char CRLF[] = "\r\n";
        // ���ص�һ��"\r\n"��λ��
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
        // ���ص�һ��"\r\n"��λ��
        const char *crlf = std::search(peek(), beginWrite(), CRLF, CRLF + 2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    // ������
    void retrieve(size_t len)
    {
        assert(len <= readableBytes());
        _readIndex += len;
    }
    // ������endΪֹ
    void retrieveUntil(const char *end)
    {
        // std::cout << "end : " << end - begin() << std::endl;
        // std::cout << "peek : " << peek() - begin() << std::endl;
        assert(peek() <= end); // ���ﱨ��
        assert(end <= beginWrite());
        // std::cout << end - peek() << std::endl;
        retrieve(end - peek()); // ���ﵼ��������
    }
    // ����ȫ������
    void retrieveAll()
    {
        _readIndex = 0;
        _writeIndex = 0;
    }
    // ��string��ʽ����ȫ������
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
    // д������ ֧���������͵�д��

    // ȷ����д
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
