#ifndef _THREAD_H__
#define _THREAD_H__

#include <pthread.h>

typedef struct thread
{
    pthread_t pingreg_thir;     //ping 线程
    pthread_t publish_thir;
    pthread_t receive_thir;     //接收数据线程

    pthread_cond_t cond_ping;        //条件变量
    pthread_mutex_t mutex_ping;      //互斥锁
}thread_t;

extern thread_t *tp;    


void thread_param_init();

#endif //_THREAD_H__
