#include "buffer.h"
#include <cstring>
#include <unistd.h> // write
#include <sys/uio.h> //iovec
ssize_t buffer::readFd(int fd, int* savedErrno){
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writeableBytes();
    vec[0].iov_base = begin() + _writeIndex;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);
    const ssize_t n = readv(fd, vec, 2);
    // std::cout << n << std::endl;
    if(n < 0) {
        printf("[buffer:readFd]fd = %d readv : %s\n", fd, strerror(errno));
        *savedErrno = errno;
    } 
    else if(static_cast<size_t>(n) <= writable){
        _writeIndex += n;
        // std::cout << _writeIndex << std::endl;
        // std::cout << "***************" << std::endl;
    }
        
    else {
        _writeIndex = _buffer.size();
        append(extrabuf, n - writable);
    }
    return n;
}

ssize_t buffer::writeFd(int fd, int* savedErrno)
{
    size_t nLeft = readableBytes();
    char* bufPtr = begin() + _readIndex;
    ssize_t n;
    if((n = write(fd, bufPtr, nLeft)) <= 0) {
        if(n < 0 && n == EINTR)
            return 0;
        else {
            printf("[Buffer:writeFd]fd = %d write : %s\n", fd, strerror(errno));
            *savedErrno = errno;
            return -1;
        }
    } else {
        _readIndex += n;
        return n;
    }
}
