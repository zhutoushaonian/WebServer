#ifndef LST_TIMER
#define LST_TIMER

#include <time.h>
#include <netinet/in.h>
#include <cstdio>
#define BUFFER_SIZE 64
class http_conn;
// class util_timer;
// �ͻ������ݽṹ
// struct client_data
// {
//     // �ͻ��� socket ��ַ
//     sockaddr_in address;
//     // socket �ļ�������
//     int sockfd;
//     // ���ݻ�����
//     char buf[ BUFFER_SIZE ];
//     // ��ʱ����client_data ͨ�� timer ���뵽��ʱ��������;
//     util_timer* timer;
// };
// ��ʱ����
class util_timer
{
public:
    // ��Ĺ��캯��
    util_timer() : prev(NULL), next(NULL) {}

public:
    // ����ʱʱ��
    time_t expire;
    // ����ص��������˴�������һ������ָ�룬�ú������� client_data �������
    void (*cb_func)(http_conn *);
    // �ص���������Ŀͻ������ݣ��������ɶ�ʱ����ִ���ߴ��ݸ��ص�����
    http_conn *user_data;
    // ָ��ǰһ����ʱ��
    util_timer *prev;
    // ָ���һ����ʱ��
    util_timer *next;
};
// ��ʱ��������������һ��˫������
class sort_timer_lst
{
public:
    // ��ʼ������
    sort_timer_lst() : head(NULL), tail(NULL) {}
    // ��������
    ~sort_timer_lst();
    // �ú������ڽ�Ŀ�궨ʱ����ӵ�������
    void add_timer(util_timer *timer);
    /**
     * ��ĳ����ʱ�������仯ʱ��������Ӧ�Ķ�ʱ���������е�λ��
     * ����һ : ֻ�����˶�ʱ����ʱʱ���ӳ������
     * ������ : �����ڶ�ʱ����ʱʱ���ӳ������̵����
     */
    void adjust_timer(util_timer *timer);
    // ��Ŀ�궨ʱ����������ɾ��
    void del_timer(util_timer *timer);
    /**
     * SIGALRM �ź�ÿ�α������������źŴ�������ִ��һ�� tick ������
     * �Ӷ����������ϵ��ڵ�����
     */
    void tick();

private:
    // �ú���������ĺ��ʵ�λ�ò��붨ʱ��
    void add_timer(util_timer *timer, util_timer *lst_head);
    util_timer *head;
    util_timer *tail;
};

#endif