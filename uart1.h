#ifndef _UART1_H_
#define _UART1_H_
#include "common.h"
extern uchar  rev_start ;     //接收开始标志      
extern uchar  rev_stop ;     //接收停止标志     
extern uchar  gps_flag ;      //GPS处理标志     
extern char   rev_buf[80];        //接收缓存 
 
void Uart_Init(void);
void UART_T (unsigned char UART_data);

void UART_TC (unsigned char *str);
void Uart_Receive(void);

#endif