#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H
#include <netinet/in.h> // sockaddr_in
#include <sys/stat.h> // stat
#include <fcntl.h> // fcntl
#include <sys/epoll.h>
#include <unistd.h> // close
#include <string.h> // string
#include <errno.h>
#include <cstdio> // printf
#include <stdlib.h> // atol
#include <sys/mman.h> //mmap
#include <sys/uio.h> // writev
#include <stdarg.h> // 
#include "buffer.h"
#include <string>
#include <map>

#include "lst_timer.h"
class http_conn
{

public:

    enum METHOD
    {
        GET,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATCH,
        INVALID
    };

    enum CHECK_STATE{
        CHECK_STATE_REQUESTLINE,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };

    enum HTTP_CODE
    {
        NO_REQUEST, // 请求不完整
        GET_REQUEST, // 完整的请求
        BAD_REQUEST, // 语法错误
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };

    enum LINE_STATUS
    {
        LINE_OK,
        LINE_BAD,
        LINE_OPEN
    };

    http_conn(){}
    ~http_conn(){}

    void init(int sockfd, const sockaddr_in& addr);

    void close_conn(bool real_close = true);

    void process();

    int read(int* savedErrno);

    bool write(int* savedErrno);

    static int m_epollfd;

    static int m_user_count;
    util_timer* timer;


private:

    void init();

    HTTP_CODE process_read();

    bool process_write(HTTP_CODE ret);

    
    // http请求相关操作
    // HTTP_CODE parse_request_line(char* text);
    HTTP_CODE parse_request_line(const char* begin, const char* end);
    bool setMthod(const char* begin, const char* end);
    void setPath(const char* begin, const char* end);
    void setQuery(const char* begin, const char* end);
    void setVersion(const char* begin, const char* end);

    void parse_headers(const char* start, const char* colon, const char* end);
  
    HTTP_CODE parse_content(char* text);  
    HTTP_CODE do_request();
    // char* get_line(){   return m_read_buf + m_start_line; }
    LINE_STATUS parse_line();

    // http回复相关操作
    void unmap();
    // bool add_response(const char* format, ...);
    bool add_content(const char* content);
    void add_status_line(int status, const char* title);
    void add_headers(int content_length);
    void add_content_length(int content_length);
    void add_linger();
    void add_blank_line();
    
    bool isKeepAlive(){
        if(m_headers.find("Connection") != m_headers.end()){
            if(m_headers["Connection"] == "Keep-Alive"){
                return true;
            }else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    std::string getContentLength(){
        if(m_headers.find("Content-Length") != m_headers.end()){
            return m_headers["Content-Length"];
        }
        else{
            return "-1";
        }
    }
    int m_sockfd;
    sockaddr_in m_address;

    buffer m_read_buf;
    buffer m_write_buf;
    // 读缓冲区
    // char m_read_buf[READ_BUFFER_SIZE];
    // int m_read_idx;
    // int m_checked_idx;
    // int m_start_line;
    // 写缓冲区
    // char m_write_buf[WRITE_BUFFER_SIZE];
    // int m_write_idx;

    // 主状态机所处的状态
    CHECK_STATE m_check_state;

    METHOD m_method;

    std::string m_read_file;

    std::string m_path;
    std::string m_query;
    // char* m_url;
    std::string m_version;
    // char* m_version;
    std::map<std::string, std::string> m_headers;
    // char* m_host;
    // int m_content_length;
    // bool m_linger; // 是否保持连接


    // 客户请求的目标文件被mmap到内存中的起始位置
    char* m_file_address;

    struct stat m_file_stat;
    

    // struct iovec m_iv[2];
    // int m_iv_count;

};
#endif