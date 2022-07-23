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
    util_timer() : prev( NULL ), next( NULL ){}
 
public:
   // 任务超时时间
   time_t expire; 
   // 任务回调函数，此处定义了一个函数指针，该函数接受 client_data 类型入参
   void (*cb_func)( http_conn* );
   // 回调函数处理的客户端数据，该数据由定时器的执行者传递给回调函数
   http_conn* user_data;
   // 指向前一个定时器
   util_timer* prev;
   // 指向后一个定时器
   util_timer* next;
};
// 定时器链表，该链表是一个双向链表
class sort_timer_lst
{
public:
    // 初始化函数
    sort_timer_lst() : head( NULL ), tail( NULL ) {}
    // 析构函数
    ~sort_timer_lst()
    {
        // 遍历链表，释放链表中的节点对象
        util_timer* tmp = head;
        while( tmp )
        {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }
    // 该函数用于将目标定时器添加到链表中
    void add_timer( util_timer* timer )
    {
        // timer 为空则直接返回
        if( !timer )
        {
            return;
        }
        // 首节点为空，表明队列尚未创建，创建队列
        if( !head )
        {
            head = tail = timer;
            return; 
        }
        /**
         * 此处处理为了确保链表中的定时器时升序排列
         * 连接头节点中存放的定时器超时时间是最小的
         * 若新增的定时器超时时间比头节点中的定时器超时时间小，则将其插入到头节点之前
         * 否则寻找合适的位置插入定时器
         */
        if( timer->expire < head->expire )
        {
            timer->next = head;
            head->prev = timer;
            head = timer;
            return;
        }
        /**
         * 
         */
        add_timer( timer, head );
    }
    /**
     * 当某个定时任务发生变化时，调整对应的定时器在链表中的位置
     * 方案一 : 只考虑了定时器超时时间延长的情况
     * 方案二 : 适用于定时器超时时间延长或缩短的情况
     */
    void adjust_timer( util_timer* timer )
    {
        // timer 为空则直接返回
        if( !timer )
        {
            return;
        }
        // 方案一
        util_timer* tmp = timer->next;
        if( !tmp || ( timer->expire < tmp->expire ) )
        {
            return;
        }
        // timer 是头节点，则将该定时器从链表中取出再重新加入链表
        if( timer == head )
        {
            head = head->next;
            head->prev = NULL;
            timer->next = NULL;
            add_timer( timer, head );
        }
        // timer 不是头节点，将 timer 取出并加入到定时器原位置之后的链表中
        else
        {
            timer->prev->next = timer->next;
            timer->next->prev = timer->prev;
            // 方案一
            // add_timer( timer, timer->next );
            // 方案二
            add_timer( timer, head );
        }
    }
    // 将目标定时器从链表中删除
    void del_timer( util_timer* timer )
    {
        if( !timer )
        {
            return;
        }
        if( ( timer == head ) && ( timer == tail ) )
        {
            delete timer;
            head = NULL;
            tail = NULL;
            return;
        }
        if( timer == head )
        {
            head = head->next;
            head->prev = NULL;
            delete timer;
            return;
        }
        if( timer == tail )
        {
            tail = tail->prev;
            tail->next = NULL;
            delete timer;
            return;
        }
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        delete timer;
    }
    /**
     * SIGALRM 信号每次被触发就在其信号处理函数中执行一次 tick 函数，
     * 从而处理链表上到期的任务
     */
    void tick()
    {
        // printf( "timer tick\n" );
        if(!head)
        {
            return;
        }
        
        // 获取系统当前时间
        time_t cur = time( NULL );
        /**
         * 从链表头节点开始依次处理每个定时器
         * 直到遇到一个尚未到期的定时器
         */
        util_timer* tmp = head;
        while( tmp )
        {
            if( cur < tmp->expire )
            {
                break;
            }
            // 调用定时器回调函数
            tmp->cb_func( tmp->user_data );
            head = tmp->next;
            // 执行完毕的定时从链表中删除
            if( head )
            {
                head->prev = NULL;
            }
            delete tmp;
            tmp = head;
        }
    }
 
private:
    // 该函数在链表的合适的位置插入定时器
    void add_timer( util_timer* timer, util_timer* lst_head )
    {
        // prev 用于指向当前节点的前驱节点
        util_timer* prev = lst_head;
        // tmp 用于指向当前节点
        util_timer* tmp = prev->next;
        while( tmp )
        {
            if( timer->expire < tmp->expire )
            {
                prev->next = timer;
                timer->next = tmp;
                tmp->prev = timer;
                timer->prev = prev;
                break;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        // 待插入的定时器的超时时间是当前链表中最大的
        if( !tmp )
        {
            prev->next = timer;
            timer->prev = prev;
            timer->next = NULL;
            tail = timer;
        }
        
    }
 
private:
    util_timer* head;
    util_timer* tail;
};
 
#endif