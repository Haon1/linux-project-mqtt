#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "thread.h"
#include "mqtt.h"

thread_t *tp = NULL;


//心跳线程
void *handle_pingreq(void *arg)
{
    //心跳包
    char pingpak[2] = {0xc0,0x00};
    while(1)
    {
        pthread_cond_wait(&tp->cond_ping,&tp->mutex_ping);
        int bytes = write(Socket,pingpak,2);
        if(bytes == 2)
            printf("pingreq send success\n");
        pthread_mutex_unlock(&tp->mutex_ping);
        sleep(3);
    }
}

//接收数据线程
void *handle_recvmsg(void *arg)
{
    while(1)
    {
        //pthread_cond_wait(&tp->cond,&tp->mutex);
        int bytes = read(Socket,Mqtt_Receive_buf,sizeof(Mqtt_Receive_buf));
        printf("receive bytes %d\n",bytes);
        if(bytes == 0)  //offline
        {
            //mqtt_send_connectpack();
            exit(-1);
        }
        //可以加个处理函数
        if(Mqtt_Receive_buf[0]==0x20)
        {
            switch(Mqtt_Receive_buf[3])
            {
                case 0x00:  printf("receive mqtt connectack , success\n");
                            pthread_cond_signal(&tp->cond_ping);
                            break;
            }
        }
        else if(Mqtt_Receive_buf[0]==0xd0)
        {
            printf("receive mqtt pingresp\n");
            pthread_cond_signal(&tp->cond_ping);
        }

        memset(Mqtt_Receive_buf,0,sizeof(Mqtt_Receive_buf));
    }
}

//创建线程,初始化线程资源
void thread_param_init()
{
    int ret;

    tp = (thread_t *)malloc(sizeof(thread_t));
    if(NULL == tp)
    {
        perror("");
        return ;
    }

    pthread_mutex_init(&tp->mutex_ping, NULL);
    pthread_cond_init(&tp->cond_ping, NULL);

    //创建心跳线程
    ret = pthread_create(&tp->pingreg_thir,NULL,handle_pingreq,NULL);
    if(ret != 0)
    {
        printf("pingreg_thir  create fail\n");
        return ;
    }
    printf("pingreg_thir  create success\n");
    //创建接收数据线程
    ret = pthread_create(&tp->receive_thir,NULL,handle_recvmsg,NULL);
    if(ret != 0)
    {
        printf("receive_thir  create fail\n");
        return ;
    }
    printf("receive_thir  create success\n");
    
}

