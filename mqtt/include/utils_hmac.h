/*-------------------------------------------------*/
/*               ��γ���ӿ�����                    */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef UTILS_HMAC_H_
#define UTILS_HMAC_H_

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

void utils_hmac_md5(const char *msg, int msg_len, char *digest, const char *key, int key_len);
void utils_hmac_sha1(const char *msg, int msg_len, const char *key, int key_len, char *digest);
int base64_decode( const char * base64, unsigned char * bindata );

#endif

