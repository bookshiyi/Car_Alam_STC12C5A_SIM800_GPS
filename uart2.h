#ifndef _UART2_H_
#define _UART2_H_
#include "common.h"
#include "devices.h"

#define S2TI 0x02      // 串口2发送中断请求标志位
#define S2RI 0x01      // 串口2接收中断请求标志位




void Uart2_Init();
void UART2_T (unsigned char UART_data);
void UART2_TC (unsigned char *str);
void Uart2_Receive(void);

#endif