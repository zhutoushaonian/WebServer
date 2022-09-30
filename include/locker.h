#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>
// ��װ�ź�������
class sem
{
private:
    sem_t m_sem;

public:
    // ��������ʼ���ź���
    sem();
    // �����ź���
    ~sem();
    // �ȴ��ź���
    bool wait();
    // �����ź���
    bool post();
};

// ��װ����������
class locker
{
private:
    pthread_mutex_t m_mutex;

public:
    locker();
    ~locker();
    bool lock();
    bool unlock();
};
// ��װ������������
class cond
{
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;

public:
    cond();
    ~cond();
    bool wait();
    bool signal();
};
#endif