#ifndef _UART1_H_
#define _UART1_H_
#include "common.h"
extern uchar  rev_start ;     //���տ�ʼ��־      
extern uchar  rev_stop ;     //����ֹͣ��־     
extern uchar  gps_flag ;      //GPS�����־     
extern char   rev_buf[80];        //���ջ��� 
 
void Uart_Init(void);
void UART_T (unsigned char UART_data);

void UART_TC (unsigned char *str);
void Uart_Receive(void);

#endif