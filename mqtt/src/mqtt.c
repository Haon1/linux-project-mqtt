#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "utils_hmac.h"

int Socket;     //套接字

char *ProductKey    = "a1EExFlVDfn";
char *DeviceName    = "D001";
char *DeviceSecret  = "e6eaac3c4a814b4b13cc3a6a78a4deb3";

char Mqtt_host[64];     //云域名
int  Mqtt_port;         //端口

char ClientId[128];     //client id
char UserName[128];     //username
char PassWord[128];     //Password

char Mqtt_Receive_buf[512];     //消息接收区


/**
 * @brief 初始化连接参数
 * 
 */
void mqtt_iot_param_init()
{
    char data[512]={0};

    sprintf(Mqtt_host,"%s.iot-as-mqtt.cn-shanghai.aliyuncs.com",ProductKey);
    Mqtt_port = 1883;
    sprintf(ClientId,"%s|securemode=3,signmethod=hmacsha1|",DeviceName);
    sprintf(UserName,"%s&%s",DeviceName,ProductKey);

    sprintf(data,"clientId%sdeviceName%sproductKey%s",DeviceName,DeviceName,ProductKey);
    utils_hmac_sha1( data, strlen(data), DeviceSecret, strlen(DeviceSecret), PassWord);

    printf("ServerHost: %s:%d\n",Mqtt_host,Mqtt_port);
    printf("ClientId:%s\n",ClientId);
    printf("UserName:%s\n",UserName);
    printf("PassWord:%s\n",PassWord);
}

//连接阿里云代理服务器
int mqtt_connect_iot_server()
{
    struct sockaddr_in server;

    //Create socket
    Socket = socket(AF_INET, SOCK_STREAM , 0);
    if (Socket == -1) {
        printf("Could not create socket");
    }

    char ip[20] = {0};
    struct hostent *hp;
    if ((hp = gethostbyname(Mqtt_host)) == NULL) {
        return 1;
    }

    strcpy(ip, inet_ntoa(*(struct in_addr *)hp->h_addr_list[0]));
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(Mqtt_port);

    //Connect to remote server
    if (connect(Socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("connect error: %d", errno);
        return 1;
    }

    printf("connect broker success\n");
}

//发送连接报文
void  mqtt_send_connectpack()
{
    int fixed_len = 1;
    int variable_len = 10;
    int clientid_len = strlen(ClientId);
    int username_len = strlen(UserName);
    int password_len = strlen(PassWord);
    int payload_len = 2+clientid_len + 2+username_len + 2+password_len;
    int length = variable_len + payload_len;
    int send_len = 0;
    //data buf
    char mqtt_send_buf[256]={0};

    mqtt_send_buf[send_len++] = 0x10;

    do
    {
        char d = length % 128;
        length = length / 128;
        if(length > 0)
            d |= 0x80;
        mqtt_send_buf[send_len++] = d;
        fixed_len++;
    } while (length>0);

    //Protocol Name
    mqtt_send_buf[send_len++] = 0x00;
    mqtt_send_buf[send_len++] = 0x04;
    mqtt_send_buf[send_len++] = 'M';
    mqtt_send_buf[send_len++] = 'Q';
    mqtt_send_buf[send_len++] = 'T';
    mqtt_send_buf[send_len++] = 'T';
    //Protocol Level
    mqtt_send_buf[send_len++] = 0x04;
    //Connect Flags
    mqtt_send_buf[send_len++] = 0xc0;
    //Keep Alive
    mqtt_send_buf[send_len++] = 0x00;       //MSB
    mqtt_send_buf[send_len++] = 0x3c;       //60s

    mqtt_send_buf[send_len++] = clientid_len/256;   //MSB
    mqtt_send_buf[send_len++] = clientid_len%256;   //LSB
    memcpy(&mqtt_send_buf[send_len],ClientId,clientid_len);
    send_len += clientid_len;

    mqtt_send_buf[send_len++] = username_len/256;   //MSB
    mqtt_send_buf[send_len++] = username_len%256;   //LSB
    memcpy(&mqtt_send_buf[send_len],UserName,username_len);
    send_len += username_len;

    mqtt_send_buf[send_len++] = password_len/256;   //MSB
    mqtt_send_buf[send_len++] = password_len%256;   //LSB
    memcpy(&mqtt_send_buf[send_len],PassWord,password_len);
    send_len += password_len;

    int bytes = write(Socket, mqtt_send_buf, fixed_len + variable_len + payload_len);

    printf("len = %d\n",fixed_len + variable_len + payload_len);
    printf("total %d bytes\n",send_len);
    printf("send %d bytes\n",bytes);
}