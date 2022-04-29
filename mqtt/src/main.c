#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "mqtt.h"
#include "thread.h"


void handle_exit(int arg)
{
    free(tp);
    exit(-1);
}

int main(int argc, char *argv[])
{

    signal(SIGINT,handle_exit);

    //连接参数初始化
    mqtt_iot_param_init();
    //连接阿里云代理服务器
    mqtt_connect_iot_server();

    thread_param_init();

    //连接报文
    mqtt_send_connectpack();
    
    sleep(2);
    //订阅主题
    mqtt_subscribe_topic(Subscribe_topic,0);
    
    
    pause();
}