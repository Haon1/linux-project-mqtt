#ifndef _MQTT_H__
#define  _MQTT_H__


extern int Socket;
extern char Mqtt_Receive_buf[512];     //消息接收区
extern char Mqtt_send_buf[512];        //tx buf

//初始化连接参数
void mqtt_iot_param_init();
//连接阿里云代理服务器
int mqtt_connect_iot_server();

void  mqtt_send_connectpack();


#endif  //_MQTT_H__
