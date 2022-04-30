
#include <stdio.h>

/**
 * @brief 计算剩余长度并返回占用字节数
 * 
 * @param buf 转换后要存放的地址
 * @param length 要转换的字节数
 * @return int 转换后占用的字节数
 */
int MQTTPacket_encode(unsigned char *buf, int length)
{
    int bytes = 0;
 
    do {
        char d = length % 128;
        length /= 128;
        /* if there are more digits to encode, set the top bit of this digit */
        if (length > 0) {
            d |= 0x80;
        }
        buf[bytes++] = d;
    } while (length > 0);
    return bytes;
}
 