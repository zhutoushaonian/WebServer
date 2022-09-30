#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>
// 封装信号量的类
class sem
{
private:
    sem_t m_sem;

public:
    // 创建并初始化信号量
    sem();
    // 销毁信号量
    ~sem();
    // 等待信号量
    bool wait();
    // 增加信号量
    bool post();
};

// 封装互斥锁的类
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
// 封装条件变量的类
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