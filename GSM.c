#include "GSM.h"
#include "uart2.h"
#include "uart1.h"
#include "string.h"
#include "delay.h"
#include "devices.h"

extern uchar rec_data[50];
extern uchar rec_num;
extern uchar rec_num;

uchar hand(uchar *ptr)//和gsm模块进行握手检测
{
	if(strstr(rec_data,ptr)!=NULL)
		return 1;
	else
		return 0;
}

void clear_rec_data()//清除串口二接收到的数据
{
	uchar i;
	for(i=0;i<49;i++)
	{
		rec_data[i]='0';
	}
	rec_num=0;
}

void GSM_Sync_Baudrate()//同步波特率函数
{
		clear_rec_data();
	UART2_TC("AT\r\n"); 
		while(!hand("OK")) 
	{
		clear_rec_data();
		UART2_TC("AT\r\n");//
		delay_ms(1000);//延时
		UART_TC("GSM正在同步波特率...\r\n");
		beep(20,1);//短鸣叫
	}
		UART_TC("——GSM波特率同步成功——\r\n");
		beep(500,1);//握手成功
}
 void GSM_Hand()//握手函数
{
		clear_rec_data();
	UART2_TC("ATI\r\n"); 
		while(!hand("OK")) 
	{
		clear_rec_data();
		UART2_TC("ATI\r\n");//
		delay_ms(1000);//延时
		UART_TC("GSM正在握手...\r\n");
		beep(20,1);//短鸣叫
	}
		UART_TC("——GSM握手成功——\r\n");
		beep(500,1);//握手成功
}
void GSM_Signal_quality()//信号质量
{
		clear_rec_data();
	UART2_TC("AT+CSQ\r\n"); 
		while(!hand("OK")) 
	{
		clear_rec_data();
		UART2_TC("AT+CSQ\r\n");//
		delay_ms(1000);//延时
		UART_TC("信号质量正在查询...\r\n");
		beep(20,1);//短鸣叫
	}
		UART_TC("——信号质量通过——\r\n");
		beep(500,1);
}
void GSM_SIM_Check()//是否检测到SIM卡片
{
		clear_rec_data();
	UART2_TC("AT+CPIN?\r\n"); 
		while(!hand("READY")) 
	{
		clear_rec_data();
		UART2_TC("AT+CPIN?\r\n");//
		delay_ms(1000);//延时
		UART_TC("SIM卡正在检测...\r\n");
		beep(20,1);//短鸣叫
	}
		UART_TC("——SIM卡已检测到——\r\n");
		beep(500,1);
}
void GSM_Network_Check()//是否连接到网络
{
		clear_rec_data();
	UART2_TC("AT+COPS?\r\n"); 
		while(!hand("CHINA")) 
	{
		clear_rec_data();
		UART2_TC("AT+COPS?\r\n");//
		delay_ms(1000);//延时
		UART_TC("注册网络正在检测...\r\n");
		beep(20,1);//短鸣叫
	}
		UART_TC("——已注册到网络——\r\n");
		beep(500,1);
}
void GSM_SMS_Send(uchar status,uchar *lat_str,uchar *lng_str)	//以下是发送英文短信短信
{                  
	uchar num=0,i=0;     
	uchar send_flag=1;
	static uchar message_1_A[]={"Your car may be stolen!!!\r\n http://uri.amap.com/marker?position="};
	static uchar message_1_B[]={"Your car is here:\r\n http://uri.amap.com/marker?position="};
	uchar message_2[]={"121.287689"};//默认数值
	uchar message_3[]={"31.234527"};//默认数值
	static uchar message_4[]={"&name=MyCar&src=mypage&coordinate=wgs84&callnative=1"};
	

	for(i=0;i<10;i++)
	{
		message_2[i]= lat_str[i];
		message_3[i]= lng_str[i];
	}
	
	
		clear_rec_data();
		UART2_TC("AT+CSCS=\"GSM\"\r\n"); //
		delay_ms(800);//延时
	beep(10,1);//短鸣叫
		UART2_TC("AT+CSCA?\r\n"); //短信中心号码
		delay_ms(800);//延时
	beep(10,1);//短鸣叫
		UART2_TC("AT+CMGF=1\r\n");  //方式1
		delay_ms(800);//延时
	beep(10,1);//短鸣叫
		UART2_TC("AT+CMGS=\"+8613812345678\"\r\n");  //此处修改短信接收方电话号//////////////////////////////////////////15133849421
		delay_ms(800);//延时
	beep(10,1);//短鸣叫
	if(status==1)//可能被盗
		UART2_TC(&message_1_A);  //此处修改短信内容
	else//用户主动查询
		UART2_TC(&message_1_B);  //此处修改短信内容
	delay_ms(100);//延时
	UART2_TC(&message_2);  //此处修改短信内容
	delay_ms(100);//延时
	UART2_T(',');  //此处修改短信内容
	delay_ms(100);//延时
	UART2_TC(&message_3);  //此处修改短信内容
	delay_ms(100);//延时
	UART2_TC(&message_4);  //此处修改短信内容
		delay_ms(800);//延时
	beep(10,1);//短鸣叫
		clear_rec_data();
		UART2_T(0X1A);
		delay_ms(800);//延时
	beep(10,1);//短鸣叫
	
	while(!hand("OK")) 
	{
		clear_rec_data();
		num++;
		delay_ms(800);//延时
		UART_TC("短信正在发送...\r\n");
		beep(10,1);//短鸣叫
		if (num>20)
		{
			send_flag=0;
			break;
		}
	}
	if(send_flag==0)
	{
			UART_TC("——短信发送失败——\r\n");
			beep(500,2);//发送失败鸣叫两声
	}
	else
	{
		UART_TC("——短信发送成功——\r\n");
		beep(500,1);
	}
}
uchar GSM_Ring_Check()//检查是否有电话打来
{
	if(hand("RI"))///////RING
	{
		delay_ms(3000);
		UART2_TC("ATH\r\n");//挂断打进来的电话
		clear_rec_data();
		return 1;
	}
	else
	{
		clear_rec_data();
		return 0;
	}
}


