#include "http_conn.h"
#include <iostream>
/*定义HTTP响应的一些状态信息*/
const char* ok_200_title = "OK" ;
const char* error_400_title = "Bad Request" ;
const char* error_400_form = "400 Bad Request. Your request has bad syntax or is inherently impossible to satisfy. \n" ;
const char* error_403_title = "Forbidden" ;
const char* error_403_form = "403 Forbidden. You do not have permission to get file from this server. \n" ;
const char* error_404_title = "Not Found" ;
const char* error_404_form = "404 Not Found. The requested file was not found on this server.\n" ;
const char* error_500_title = "Internal Error" ;
const char* error_500_form = "500 Internal Error. There was an unusual problem serving the requested file.\n" ;
/*网站的根目录*/
const std::string doc_root = "/home/luhuitong/HttpServer";

int setnonblock(int fd){

    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd, bool one_shot){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    if(one_shot){
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblock(fd);
}

void addfdLT(int epollfd, int fd){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP;
    
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblock(fd);
}

void removefd(int epollfd, int fd){
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
    //std::cout << "close:" << err << std::endl;
}

void modfd(int epollfd, int fd, int ev){
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

// 类的静态变量初始化
int http_conn::m_user_count = 0;
int http_conn::m_epollfd = -1;

void http_conn::close_conn(bool real_close){
    if(real_close && (m_sockfd != -1)){
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_user_count--;
    }
}

void http_conn::init(int sockfd, const sockaddr_in& addr){
    // std::cout << "init : sockfd is " << sockfd << std::endl;
    m_sockfd = sockfd;
    m_address = addr;
    // int reuse = 1;
    // setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    addfd(m_epollfd, sockfd, true);
    m_user_count++;
    init();
}

void http_conn::init(){
    m_check_state = CHECK_STATE_REQUESTLINE;
}

// 每次read都是
int http_conn::read(int* savedErrno){
    
    int ret = m_read_buf.readFd(m_sockfd, savedErrno);
    // std::cout << "read number" << ret << std::endl;
    
    return ret;
}

bool http_conn::write(int* savedErrno){
    

    int write_size = m_write_buf.writeFd(m_sockfd, savedErrno);
    // std::cout << "write function : fd is " << m_sockfd << std::endl;
    bool ret = isKeepAlive();
    // std::cout << ret << std::endl;
    return ret;
}

// 解析HTTP请求行，获得请求方法、目标URL以及HTTP版本号
http_conn::HTTP_CODE http_conn::parse_request_line(const char* begin, const char* end){
    const char* start = begin;
    const char* space = std::find(start, end, ' ');
    if(space == end){
        std::cout << "------ parse request line error  ------" << std::endl;
        return BAD_REQUEST;
    }
    if(!setMthod(start, space)){
        std::cout << "------ Method is INVAILD ------" << std::endl;
    }
    start = space + 1;
    space = std::find(start, end, ' ');
    if(space == end){
        std::cout << "------ parse request line error  ------" << std::endl;
        return BAD_REQUEST;
    }
    const char* query = std::find(start, space, '?');
    if(query != space){
        setPath(start, query);
        setQuery(query, space);
    }else{
        setPath(start, space);
    }
    start = space + 1;
    bool succeed = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");
    if(succeed) {
        if(*(end - 1) == '1')
            m_version = "HTTP/1.1";
        else if(*(end - 1) == '0')
            m_version = "HTTP/1.0";
        else{
            std::cout << "------ Http version can't parse ------" << std::endl;
            return BAD_REQUEST;
        }
            
    } 
    
    return NO_REQUEST;

}
// 设置请求方法
bool http_conn::setMthod(const char* start, const char* end){
    std::string m(start, end);
    if(m == "GET")
        m_method = GET;
    else if(m == "POST")
        m_method = POST;
    else if(m == "HEAD")
        m_method = HEAD;
    else if(m == "PUT")
        m_method = PUT;
    else if(m == "DELETE")
        m_method = DELETE;
    else if(m == "TRACE")
        m_method = TRACE;
    else if(m == "OPTIONS")
        m_method = OPTIONS;
    else if(m == "CONNECT")
        m_method = CONNECT;
    else if(m == "PATCH")
        m_method = PATCH;
    else
        m_method = INVALID;
    
    return m_method != INVALID;
}
// 设置请求路径
void http_conn::setPath(const char* begin, const char* end){
    std::string subPath;
    subPath.assign(begin, end);
    if(subPath == "/")
        subPath = "/index.html";
    m_path = subPath;
}
// 设置请求参数
void http_conn::setQuery(const char* begin, const char* end){
    m_query.assign(begin, end);
}
// 解析请求头
void http_conn::parse_headers(const char* start, const char* colon, const char* end){
    std::string key(start, colon);
    ++colon;
    while(colon < end && *colon == ' ')
        ++colon;
    std::string val(colon, end);
    while(!val.empty() && val[val.size() - 1] == ' ')
        val.resize(val.size() - 1);
    m_headers[key] = val;

}
/*解析HTTP请求的一个头部信息*/


http_conn::HTTP_CODE http_conn::parse_content(char* text){
    // if(m_read_idx >= (m_content_length + m_checked_idx)){
    //     text[m_content_length] = '\0';
    //     return GET_REQUEST;
    // }
    return NO_REQUEST;
}


http_conn::HTTP_CODE http_conn::process_read(){

    HTTP_CODE ret = NO_REQUEST;
    bool hasMore = true;
    
    while(hasMore){

        switch (m_check_state)
        {
            case CHECK_STATE_REQUESTLINE:
            {
                const char* crlf = m_read_buf.findCRLF();
                if(crlf){
                    ret = parse_request_line(m_read_buf.peek(), crlf);
                    if(ret != BAD_REQUEST){
                        m_check_state = CHECK_STATE_HEADER;
                        m_read_buf.retrieveUntil(crlf + 2);
                    }else{
                        return BAD_REQUEST;
                    }
                }else{
                    hasMore = false;
                }
                
                break;
            }
            case CHECK_STATE_HEADER:
            {
                const char* crlf = m_read_buf.findCRLF();
                if(crlf){
                    const char* colon = std::find(m_read_buf.peek(), crlf, ':');
                    if(colon != crlf){
                       // 解析头部
                       parse_headers(m_read_buf.peek(), colon, crlf);
                    }else{
                        // 这个时候已经说明，已经收到了完整的HTTP报文了，并且头部信息已经处理完毕
                        m_check_state = CHECK_STATE_CONTENT;
                        ret = do_request();
                    
                    }
                    // std::cout << "process_read(): m_read_buf.size()" << m_read_buf.readableBytes() << std::endl;
                    // std::cout << "m_read_buf.peek() :" << m_read_buf.peek() << std::endl;
                    m_read_buf.retrieveUntil(crlf + 2);
                }else{
                    hasMore = false;
                }
                break;
            }
            case CHECK_STATE_CONTENT:
            {
                // ret = parse_content(text);
                // if(ret == GET_REQUEST){
                //     return do_request();
                // }
                // line_status = LINE_OPEN;
                // m_read_buf.retrieveUntil()
                
                m_read_buf.retrieve(m_read_buf.readableBytes());
                // std::cout << "process_read(): m_read_buf.size()" << m_read_buf.readableBytes() << std::endl;
                hasMore = false;
                break;
            }
            default:
            {
                return INTERNAL_ERROR;
            }
        }
    }


    return ret;
   
}

http_conn::HTTP_CODE http_conn::do_request(){
    // strcpy(m_read_file, doc_root);
    // strcat(m_read_file, m_url);
    m_read_file = doc_root + m_path;
    // int len = strlen(doc_root);
    m_read_buf.append(m_path);
    // strncpy(m_read_buf + len, m_url, FILENAME_LEN - len - 1);
    if(stat(m_read_file.data(), &m_file_stat) < 0){
        return NO_RESOURCE;
    }
    if(!(m_file_stat.st_mode & S_IROTH)){
        return FORBIDDEN_REQUEST;
    }
    if(S_ISDIR(m_file_stat.st_mode)){
        return BAD_REQUEST;
    }
    int fd = open(m_read_file.data(), O_RDONLY);
    m_file_address = (char*)mmap(0, m_file_stat.st_size,PROT_READ,
                        MAP_PRIVATE, fd, 0);

    close(fd);
    return FILE_REQUEST;
}

void http_conn::unmap(){
    if(m_file_address){
        munmap(m_file_address, m_file_stat.st_size);
        m_file_address = 0;
    }
}



void http_conn::add_status_line(int status, const char* title){
    // return add_response("%s %d %s\r\n", "HTTP/1.1", status, title);
    m_write_buf.append("HTTP/1.1 " + std::to_string(status) + " " + title + "\r\n");
}

void http_conn::add_headers(int content_len){
    add_content_length(content_len);
    add_linger();
    add_blank_line();
    
}

void http_conn::add_content_length(int content_len){
    // return add_response("Content-Length: %d\r\n", content_len);
    // std::string content_length = getContentLength();

    m_write_buf.append("Content-Length: " + std::to_string(content_len) + "\r\n");
}

void http_conn::add_linger(){
    // return add_response("Connection: %s\r\n", (m_linger == true) ? "keep-alive" : "close");
    std::string connect;
    if(isKeepAlive()){
        connect = "keep-alive";
    }else{
        connect = "close";
    }
    m_write_buf.append("Connection: " + connect + "\r\n");
}

void http_conn::add_blank_line(){
    m_write_buf.append("\r\n");
}

bool http_conn::add_content(const char* content){
    // return add_response("%s", content);
    m_write_buf.append(content);
    return true;
}

bool http_conn::process_write(HTTP_CODE ret){
    switch (ret)
    {
        case INTERNAL_ERROR:
        {
            add_status_line(500, error_500_title);
            add_headers(strlen(error_500_form));
            if(!add_content(error_500_form)){
                return false;
            }
            break;
        
        }
        case BAD_REQUEST://访问文件为文件夹时
        {
            add_status_line(400, error_400_title);
            add_headers(strlen(error_400_form));
            if(!add_content(error_400_form)){
                return false;
            }
            break;
        
        }    
        case NO_RESOURCE:// 访问的文件不存在
        {
            add_status_line(404, error_404_title);
            add_headers(strlen(error_404_form));
            if(!add_content(error_404_form)){
                return false;
            }
            break;
        
        }
        case FORBIDDEN_REQUEST:// 权限不足
        {
            add_status_line(403, error_403_title);
            add_headers(strlen(error_403_form));
            if(!add_content(error_403_form)){
                return false;
            }
            break;
        
        }
        case FILE_REQUEST:
        {
            add_status_line(200, ok_200_title);
            if(m_file_stat.st_size != 0){
                add_headers(m_file_stat.st_size);
                // m_iv[0].iov_base = m_write_buf;
                // m_iv[0].iov_len = m_write_idx;
                // m_iv[1].iov_base = m_file_address;
                // m_iv[1].iov_len = m_file_stat.st_size;
                // m_iv_count = 2;
                m_write_buf.append(m_file_address, m_file_stat.st_size);
                unmap();
                return true;
                
            }
            else{
                const char* ok_string = "<html><body>file size is zero.</body></html>";
                add_headers(strlen(ok_string));
                if(!add_content(ok_string)){
                    return false;
                }
            }
            break;
        
        }
        default:
            return false;
        }

    // m_iv[0].iov_base = m_write_buf;
    // m_iv[0].iov_len = m_write_idx;
    // m_iv_count = 1;
    return true;
}

void http_conn::process(){
    HTTP_CODE read_ret = process_read();
    if(read_ret == NO_REQUEST){
        modfd(m_epollfd, m_sockfd, EPOLLIN);
        return;
    }
    bool write_ret = process_write(read_ret);
    // std::cout << "write success" << std::endl;
    if(!write_ret){
        close_conn();
    }
    modfd(m_epollfd, m_sockfd, EPOLLOUT);
}