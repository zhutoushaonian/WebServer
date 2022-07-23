#include <iostream>
#include <signal.h> // sigaction
#include <arpa/inet.h>
#include <cassert>

#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"
#include "lst_timer.h"
#define MAX_FD 65536
#define MAX_EVENT_NUMBER 65536
#define TIMESLOT 1

static int pipefd[2];
static sort_timer_lst timer_lst;
static int epollfd;
extern void addfdLT(int epollfd, int fd);
extern int addfd(int epollfd, int fd, bool one_shot);
extern int removefd(int epollfd, int fd);
// https://www.cnblogs.com/wanghao-boke/p/12172293.html
// https://zhuanlan.zhihu.com/p/188390490 压测工具，用第二个 https://github.com/link1st/go-stress-testing
void addsig(int sig, void(handler)(int), bool restart = true){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if(restart){
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

void sig_handler( int sig )
{
    int save_errno = errno;
    int msg = sig;
    send( pipefd[1], ( char* )&msg, 1, 0 );
    errno = save_errno;
}

void timer_handler(){
    timer_lst.tick();
    alarm(TIMESLOT);
}
// 信号回调函数
void cb_func(http_conn* user_data )
{
    /**
     * 功能 : 从 epollfd 上删除 user_data->sockfd 对应的事件
     * EPOLL_CTL_DEL : 删除 epollfd 上的事件;
     */ 
    // epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->m_epollfd, 0);
    user_data->close_conn();
    // std::cout << "cb func " << std::endl;
    assert(user_data);
    /**
     * 关闭套接字连接
     */ 
    // close(user_data->m_epollfd);
    // printf("close fd %d\n", user_data->m_epollfd);
}

void show_error(int connfd, const char* info){
    printf("show_error : %s", info);
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int setnonblocking( int fd )
{
    /**
     * 该函数用于设置文件描述符为非阻塞;
     * 
     * F_GETFL : 获取 fd 的状态标志
     * F_SETFL : 设置 fd 的状态标志
     */ 
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}


int main(int argc, char* argv[]) {

    // if(argc <= 2){
    //     printf("usage: %s ip_addresss port_number\n", basename(argv[0]));
    //     return 1;
    // }

    // const char* ip = argv[1];
    // int port = atoi(argv[2]);

    const char* ip = "127.0.0.1";
    int port = 7777;

    addsig(SIGPIPE, SIG_IGN);

    threadpool<http_conn>* pool = NULL;
    try
    {
        pool = new threadpool<http_conn>;
    }
    catch(...)
    {
        return 1;
    }
    addsig(SIGALRM, sig_handler);
    http_conn* users = new http_conn[MAX_FD];
    bool timeout = false;
    alarm(TIMESLOT);
    assert(users);
    int user_count = 0;

    int listenfd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    assert(listenfd >= 0);
    struct linger tmp = {1, 0};
    setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret >= 0);

    ret = listen(listenfd, 1024);
    assert(ret >= 0);
    std::cout << "listening........................."<< std::endl;
    epoll_event events[MAX_EVENT_NUMBER];
    epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfdLT(epollfd, listenfd);

    ret = socketpair( PF_UNIX, SOCK_STREAM, 0, pipefd );
    assert( ret != -1 );
    setnonblocking(pipefd[1]);
    addfd(epollfd, pipefd[0], false);

    http_conn::m_epollfd = epollfd;

    bool stop_server = false;
    while(!stop_server){
        // std::cout << "http_conn:" << http_conn::m_user_count << std::endl;


        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        // std::cout << "epoll_wait number is :" << number << std::endl;
        if((number < 0) && (errno != EINTR)){
            printf("epoll failure %d\n", errno);
            break;
        }
        for(int i = 0; i < number; i++){

            int sockfd = events[i].data.fd;
            // std:: cout << sockfd << std::endl;
            if(sockfd == listenfd){
                // std::cout << "listenfd" << std::endl;
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr*)&client_address,
                                    &client_addrlength);
                // std::cout << "client accept" << http_conn::m_user_count << std::endl;
                if(connfd < 0){
                    printf("errno is: %d\n", errno);
                    continue;
                }
                
                // std::cout << "listenfd " << http_conn::m_user_count << std::endl;
                if(http_conn::m_user_count >= MAX_FD){
                    show_error(connfd, "Internal server busy");
                    continue;
                }
                /**
                 * 新建定时器连接并加入定时器升序链表中;
                 * 设置其回调函数与超时时间，绑定定时器与用户数据;
                 */ 
                users[connfd].init(connfd, client_address);
                util_timer* timer = new util_timer;
                timer->user_data = &users[connfd];
                timer->cb_func = cb_func;
                time_t cur = time(NULL);
                /**
                 * 指定任务超时时间
                 */ 
                timer->expire = cur + 3 * TIMESLOT;
                users[connfd].timer = timer;
                timer_lst.add_timer(timer);
            }
            else if(( sockfd == pipefd[0] ) && ( events[i].events & EPOLLIN )){
                // std::cout << "pipefd" << std::endl;

                int sig;
                char signals[1024];
                // std::cout << "signal" << std::endl;
                /**
                 * 从 pipefd 读取数据到 signals 缓冲区，成功则返回接受到的字节数
                 * 信号发生时，在信号处理函数中将信号标识码发送到了 pipefd[1] 中
                 */ 
                ret = recv(pipefd[0], signals, sizeof(signals), 0 );
                
                if(ret == -1)
                {
                    // handle the error
                    continue;
                }
                else if(ret == 0)
                {
                    continue;
                }
                else
                {
                    for( int i = 0; i < ret; ++i )
                    {
                        switch( signals[i] )
                        {
                            // 定时器信号
                            case SIGALRM:
                            {
                                // std::cout << "SIGALRM" << std::endl;
                                timeout = true;
                                break;
                            }
                            // 终止信号，软件终止
                            case SIGTERM:
                            {
                                std::cout << "SIGTERM SIGNAL COMING" << std::endl;
                                stop_server = true;
                            }
                        }
                    }
                }
            }
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                // std::cout << "epoll signal" << std::endl;

                users[sockfd].close_conn();
            }
            else if(events[i].events & EPOLLIN){
                //std::cout << "read" << std::endl;

                int* err;
                util_timer* timer = users[sockfd].timer;
                if(users[sockfd].read(err)){// 同一个sockfd不能够重复加入到线程池中,会导致无序,通过EPOLLONESHOT解决
                    if(timer)
                    {
                        time_t cur = time( NULL );
                        timer->expire = cur + 3 * TIMESLOT;
                        // printf( "adjust timer once\n" );
                        timer_lst.adjust_timer( timer );
                    }
                    pool->append(users + sockfd);

                }
                else{
                    // cb_func(&users[sockfd]);
                    if(timer)
                    {
                        timer_lst.del_timer( timer );
                    }
                    users[sockfd].close_conn();
                }
            }
            else if(events[i].events & EPOLLOUT){
                // std::cout << "write" << std::endl;
                util_timer* timer = users[sockfd].timer;
                // std::cout << "fd is " << i << std::endl;
                if(!users[sockfd].write(0)){
                    
                    
                    if(timer)
                    {
                        timer_lst.del_timer( timer );
                    }
                    users[sockfd].close_conn();
                     
                }

            }
            
            else{
              std::cout << "else cout " << std::endl;
            }
        }
        if(timeout){
            // std::cout << "time_handler" << std::endl;
            timer_handler();
            timeout = false;
        }
    }

    close(epollfd);
    close(listenfd);
    close(pipefd[1]);
    close(pipefd[0]);
    delete []users;
    delete pool;
    return 0;
    
}
