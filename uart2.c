
#include "uart2.h"
#include "GSM.h"


extern uchar rec_data[50]=0;
extern uchar rec_num=0;
 //=========baud 9600=======================
void Uart2_Init()			 //串口2初始化函数，波特率9600
{
	S2CON=  0x50   ;							//方式2，允许接收
//	BRT  =  RELOAD_TWO ;
	BRT = 0xfd;									 //设置波特率9600
	AUXR =  AUXR |0X10   ;		 	 //允许独立波特率发生器运行
//	BRTx12_enable();
	AUXR1 =  AUXR1&0xef   ;	 //S2_P4=0,将uart2切换到P1口
	IE2  =  IE2|0X01;			//允许串口2中断
}


void UART2_T (unsigned char UART_data)
{ //定义串口发送数据变量
	ES=0;
	S2BUF = UART_data;	//将接收的数据发送回去
	while((S2CON&S2TI)!=0x02);   //检查发送中断标志位
  S2CON &= ~S2TI;              //令发送中断标志位为0（软件清零）
	ES=1;
}

void UART2_TC (unsigned char *str){
	while(*str != '\0')
	{
		UART2_T(*str++);
		//*str++;
	}
	*str = 0;
}

void Uart2_Receive(void) interrupt 8//串口2的接收终端服务程序
{
	uchar ch;
	ES = 0;

	if(S2CON & S2RI)//如果是接收中断
	{
		ch = S2BUF; 	
			rec_data[rec_num++]=ch;
			if(rec_num>=50)
			rec_num=0;
	}  

		S2CON &=~S2RI;				//清0 S2RI	         
	ES = 1;	
}




