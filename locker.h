#ifndef LOCKER_H
#define LOCKER_H

#include<exception>
#include<pthread.h>
#include<semaphore.h>
// ��װ�ź�������
class sem{
    private: sem_t m_sem;
    public:
    // ��������ʼ���ź���
    sem(){
        if(sem_init(&m_sem, 0, 0) != 0){
            // ���캯��û�з���ֵ������ͨ���׳��쳣���������
            throw std::exception();
        }
    }
    // �����ź���
    ~sem(){
        sem_destroy(&m_sem);
    }
    // �ȴ��ź���
    bool wait(){
        return sem_wait(&m_sem) == 0;
    }
    // �����ź���
    bool post(){
        return sem_post(&m_sem) == 0;
    }
};

// ��װ����������
class locker{
    private: 
    	pthread_mutex_t m_mutex;
    public:
   		locker(){
            if(pthread_mutex_init(&m_mutex, NULL) != 0){
                throw std::exception();
            }
        }
    	~locker(){
            pthread_mutex_destroy(&m_mutex);
        }
    	bool lock(){
            return pthread_mutex_lock(&m_mutex) == 0;
        }
    	bool unlock(){
            return pthread_mutex_unlock(&m_mutex) == 0;
        }
    
};
// ��װ������������
class cond{
    private:
    	pthread_mutex_t m_mutex;
    	pthread_cond_t m_cond;
    public:
    	cond(){
            if(pthread_mutex_init(&m_mutex, NULL) != 0){
                throw std::exception();
            }
            if(pthread_cond_init(&m_cond, NULL) != 0){
                pthread_mutex_destroy(&m_mutex);
                throw std::exception();
            }
        }
    	~cond(){
         	pthread_mutex_destroy(&m_mutex);
            pthread_cond_destroy(&m_cond);
        }
    	bool wait(){
            int ret = 0;
            pthread_mutex_lock(&m_mutex);
            ret = pthread_cond_wait(&m_cond, &m_mutex);
            pthread_mutex_unlock(&m_mutex);
            return ret = 0;
        }
    	
    	bool signal(){
            return pthread_cond_signal(&m_cond) == 0;
        }

};
#endif