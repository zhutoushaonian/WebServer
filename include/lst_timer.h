#ifndef LST_TIMER
#define LST_TIMER

#include <time.h>
#include <netinet/in.h>
#include <cstdio>
#define BUFFER_SIZE 64
class http_conn;
// class util_timer;
// 客户端数据结构
// struct client_data
// {
//     // 客户端 socket 地址
//     sockaddr_in address;
//     // socket 文件描述符
//     int sockfd;
//     // 数据缓冲区
//     char buf[ BUFFER_SIZE ];
//     // 定时器，client_data 通过 timer 加入到定时器链表中;
//     util_timer* timer;
// };
// 定时器类
class util_timer
{
public:
    // 类的构造函数
    util_timer() : prev(NULL), next(NULL) {}

public:
    // 任务超时时间
    time_t expire;
    // 任务回调函数，此处定义了一个函数指针，该函数接受 client_data 类型入参
    void (*cb_func)(http_conn *);
    // 回调函数处理的客户端数据，该数据由定时器的执行者传递给回调函数
    http_conn *user_data;
    // 指向前一个定时器
    util_timer *prev;
    // 指向后一个定时器
    util_timer *next;
};
// 定时器链表，该链表是一个双向链表
class sort_timer_lst
{
public:
    // 初始化函数
    sort_timer_lst() : head(NULL), tail(NULL) {}
    // 析构函数
    ~sort_timer_lst();
    // 该函数用于将目标定时器添加到链表中
    void add_timer(util_timer *timer);
    /**
     * 当某个定时任务发生变化时，调整对应的定时器在链表中的位置
     * 方案一 : 只考虑了定时器超时时间延长的情况
     * 方案二 : 适用于定时器超时时间延长或缩短的情况
     */
    void adjust_timer(util_timer *timer);
    // 将目标定时器从链表中删除
    void del_timer(util_timer *timer);
    /**
     * SIGALRM 信号每次被触发就在其信号处理函数中执行一次 tick 函数，
     * 从而处理链表上到期的任务
     */
    void tick();

private:
    // 该函数在链表的合适的位置插入定时器
    void add_timer(util_timer *timer, util_timer *lst_head);
    util_timer *head;
    util_timer *tail;
};

#endif