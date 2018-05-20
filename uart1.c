
#include "common.h"
#include "uart1.h"
#include "devices.h"

  
uchar num = 0; 
uchar rev_start = 0;     //接收开始标志      
uchar rev_stop  = 0;     //接收停止标志     
uchar gps_flag = 0;      //GPS处理标志     
char  rev_buf[80];        //接收缓存 


void Uart_Init(void)
{
    //因为使用12Mhz晶振所以无法使用9600波特率
    SCON=0X50;			//设置为工作方式1
    TMOD=0X20;			//设置计数器工作方式2，TL1作为计数，TH1作为重装值
    PCON=0X80;			//80波特率加倍  00不加倍
    TH1=0xfa;				//计数器初始值设置，注意波特率是4800的（12c5a 9600 对应0xfa）
    TL1=0x00;
    ES=1;						//打开接收中断
    EA=1;						//打开总中断
    TR1=1;					//打开计数器
}

void UART_T (unsigned char UART_data){ //定义串口发送数据变量
	ES=0;
	SBUF = UART_data;	//将接收的数据发送回去
	while(TI == 0);		//检查发送中断标志位
	TI = 0;			//令发送中断标志位为0（软件清零）
	ES=1;
}

void UART_TC (unsigned char *str){
	while(*str != '\0'){
		UART_T(*str++);
		//*str++;
	}
	*str = 0;
}

		
void Uart_Receive(void) interrupt 4
{
	uchar ch;
	ES =0;	
	if (RI)
	{
		ch = SBUF;  
		//send_char(ch);
		if ((ch == '$') && (gps_flag == 0))  //如果收到字符'$'，便开始接收           
		{
			rev_start = 1;
			rev_stop  = 0;
		}
	
		if (rev_start == 1)  //标志位为1，开始接收            
		{
			rev_buf[num++] = ch;  //字符存到数组中           
			if (ch == '\n')     //如果接收到换行           
			{
				rev_buf[num] = '\0';
				rev_start = 0;
				rev_stop  = 1;
				gps_flag = 1;
				num = 0;
			}
		}
	}
	RI = 0;	 //RI清0，重新接收            
	ES = 1;	
}
//重写putchar函数
char putchar(char c)
{
    UART_T(c);
    return c;
}
