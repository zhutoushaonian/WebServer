#include "lst_timer.h"

// ��������
sort_timer_lst::~sort_timer_lst()
{
    // ���������ͷ������еĽڵ����
    util_timer *tmp = head;
    while (tmp)
    {
        head = tmp->next;
        delete tmp;
        tmp = head;
    }
}
// �ú������ڽ�Ŀ�궨ʱ����ӵ�������
void sort_timer_lst::add_timer(util_timer *timer)
{
    // timer Ϊ����ֱ�ӷ���
    if (!timer)
    {
        return;
    }
    // �׽ڵ�Ϊ�գ�����������δ��������������
    if (!head)
    {
        head = tail = timer;
        return;
    }
    /**
     * �˴�����Ϊ��ȷ�������еĶ�ʱ��ʱ��������
     * ����ͷ�ڵ��д�ŵĶ�ʱ����ʱʱ������С��
     * �������Ķ�ʱ����ʱʱ���ͷ�ڵ��еĶ�ʱ����ʱʱ��С��������뵽ͷ�ڵ�֮ǰ
     * ����Ѱ�Һ��ʵ�λ�ò��붨ʱ��
     */
    if (timer->expire < head->expire)
    {
        timer->next = head;
        head->prev = timer;
        head = timer;
        return;
    }
    /**
     *
     */
    add_timer(timer, head);
}
/**
 * ��ĳ����ʱ�������仯ʱ��������Ӧ�Ķ�ʱ���������е�λ��
 * ����һ : ֻ�����˶�ʱ����ʱʱ���ӳ������
 * ������ : �����ڶ�ʱ����ʱʱ���ӳ������̵����
 */
void sort_timer_lst::adjust_timer(util_timer *timer)
{
    // timer Ϊ����ֱ�ӷ���
    if (!timer)
    {
        return;
    }
    // ����һ
    util_timer *tmp = timer->next;
    if (!tmp || (timer->expire < tmp->expire))
    {
        return;
    }
    // timer ��ͷ�ڵ㣬�򽫸ö�ʱ����������ȡ�������¼�������
    if (timer == head)
    {
        head = head->next;
        head->prev = NULL;
        timer->next = NULL;
        add_timer(timer, head);
    }
    // timer ����ͷ�ڵ㣬�� timer ȡ�������뵽��ʱ��ԭλ��֮���������
    else
    {
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        // ����һ
        // add_timer( timer, timer->next );
        // ������
        add_timer(timer, head);
    }
}
// ��Ŀ�궨ʱ����������ɾ��
void sort_timer_lst::del_timer(util_timer *timer)
{
    if (!timer)
    {
        return;
    }
    if ((timer == head) && (timer == tail))
    {
        delete timer;
        head = NULL;
        tail = NULL;
        return;
    }
    if (timer == head)
    {
        head = head->next;
        head->prev = NULL;
        delete timer;
        return;
    }
    if (timer == tail)
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
 * SIGALRM �ź�ÿ�α������������źŴ�������ִ��һ�� tick ������
 * �Ӷ����������ϵ��ڵ�����
 */
void sort_timer_lst::tick()
{
    // printf( "timer tick\n" );
    if (!head)
    {
        return;
    }

    // ��ȡϵͳ��ǰʱ��
    time_t cur = time(NULL);
    /**
     * ������ͷ�ڵ㿪ʼ���δ���ÿ����ʱ��
     * ֱ������һ����δ���ڵĶ�ʱ��
     */
    util_timer *tmp = head;
    while (tmp)
    {
        if (cur < tmp->expire)
        {
            break;
        }
        // ���ö�ʱ���ص�����
        tmp->cb_func(tmp->user_data);
        head = tmp->next;
        // ִ����ϵĶ�ʱ��������ɾ��
        if (head)
        {
            head->prev = NULL;
        }
        delete tmp;
        tmp = head;
    }
}

// �ú���������ĺ��ʵ�λ�ò��붨ʱ��
void sort_timer_lst::add_timer(util_timer *timer, util_timer *lst_head)
{
    // prev ����ָ��ǰ�ڵ��ǰ���ڵ�
    util_timer *prev = lst_head;
    // tmp ����ָ��ǰ�ڵ�
    util_timer *tmp = prev->next;
    while (tmp)
    {
        if (timer->expire < tmp->expire)
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
    // ������Ķ�ʱ���ĳ�ʱʱ���ǵ�ǰ����������
    if (!tmp)
    {
        prev->next = timer;
        timer->prev = prev;
        timer->next = NULL;
        tail = timer;
    }
}
