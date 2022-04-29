#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "utils_hmac.h"
#include "mqtt_encode.h"

int Socket;     //套接字

char *ProductKey    = "a1EExFlVDfn";
char *DeviceName    = "D001";
char *DeviceSecret  = "e6eaac3c4a814b4b13cc3a6a78a4deb3";

char Mqtt_host[64];     //云域名
int  Mqtt_port;         //端口

char ClientId[128];     //client id
char UserName[128];     //username
char PassWord[128];     //Password

char Mqtt_Receive_buf[512];     //receive buf
char Mqtt_send_buf[512];        //tx buf


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
    //int fixed_len = 1;
    int variable_len = 10;
    int clientid_len = strlen(ClientId);
    int username_len = strlen(UserName);
    int password_len = strlen(PassWord);
    int payload_len = 2+clientid_len + 2+username_len + 2+password_len;
    int length = variable_len + payload_len;
    int send_len = 0;
    
    memset(Mqtt_send_buf,0, sizeof (Mqtt_send_buf));
    Mqtt_send_buf[send_len++] = 0x10;
    
    send_len += MQTTPacket_encode(&Mqtt_send_buf[send_len],length);

    //Protocol Name
    Mqtt_send_buf[send_len++] = 0x00;
    Mqtt_send_buf[send_len++] = 0x04;
    Mqtt_send_buf[send_len++] = 'M';
    Mqtt_send_buf[send_len++] = 'Q';
    Mqtt_send_buf[send_len++] = 'T';
    Mqtt_send_buf[send_len++] = 'T';
    //Protocol Level
    Mqtt_send_buf[send_len++] = 0x04;
    //Connect Flags
    Mqtt_send_buf[send_len++] = 0xc0;
    //Keep Alive
    Mqtt_send_buf[send_len++] = 0x00;       //MSB
    Mqtt_send_buf[send_len++] = 0x3c;       //60s

    Mqtt_send_buf[send_len++] = clientid_len/256;   //MSB
    Mqtt_send_buf[send_len++] = clientid_len%256;   //LSB
    memcpy(&Mqtt_send_buf[send_len],ClientId,clientid_len);
    send_len += clientid_len;

    Mqtt_send_buf[send_len++] = username_len/256;   //MSB
    Mqtt_send_buf[send_len++] = username_len%256;   //LSB
    memcpy(&Mqtt_send_buf[send_len],UserName,username_len);
    send_len += username_len;

    Mqtt_send_buf[send_len++] = password_len/256;   //MSB
    Mqtt_send_buf[send_len++] = password_len%256;   //LSB
    memcpy(&Mqtt_send_buf[send_len],PassWord,password_len);
    send_len += password_len;

    //int ret = write(Socket, Mqtt_send_buf, fixed_len + variable_len + payload_len);
    int ret = write(Socket, Mqtt_send_buf, send_len);

    printf("total %d bytes\n",send_len);
    printf("send %d bytes\n",ret);
}