#include "GSM.h"
#include "uart2.h"
#include "uart1.h"
#include "string.h"
#include "delay.h"
#include "devices.h"

extern uchar rec_data[50];
extern uchar rec_num;
extern uchar rec_num;

uchar hand(uchar *ptr)//��gsmģ��������ּ��
{
	if(strstr(rec_data,ptr)!=NULL)
		return 1;
	else
		return 0;
}

void clear_rec_data()//������ڶ����յ�������
{
	uchar i;
	for(i=0;i<49;i++)
	{
		rec_data[i]='0';
	}
	rec_num=0;
}

void GSM_Sync_Baudrate()//ͬ�������ʺ���
{
		clear_rec_data();
	UART2_TC("AT\r\n"); 
		while(!hand("OK")) 
	{
		clear_rec_data();
		UART2_TC("AT\r\n");//
		delay_ms(1000);//��ʱ
		UART_TC("GSM����ͬ��������...\r\n");
		beep(20,1);//������
	}
		UART_TC("����GSM������ͬ���ɹ�����\r\n");
		beep(500,1);//���ֳɹ�
}
 void GSM_Hand()//���ֺ���
{
		clear_rec_data();
	UART2_TC("ATI\r\n"); 
		while(!hand("OK")) 
	{
		clear_rec_data();
		UART2_TC("ATI\r\n");//
		delay_ms(1000);//��ʱ
		UART_TC("GSM��������...\r\n");
		beep(20,1);//������
	}
		UART_TC("����GSM���ֳɹ�����\r\n");
		beep(500,1);//���ֳɹ�
}
void GSM_Signal_quality()//�ź�����
{
		clear_rec_data();
	UART2_TC("AT+CSQ\r\n"); 
		while(!hand("OK")) 
	{
		clear_rec_data();
		UART2_TC("AT+CSQ\r\n");//
		delay_ms(1000);//��ʱ
		UART_TC("�ź��������ڲ�ѯ...\r\n");
		beep(20,1);//������
	}
		UART_TC("�����ź�����ͨ������\r\n");
		beep(500,1);
}
void GSM_SIM_Check()//�Ƿ��⵽SIM��Ƭ
{
		clear_rec_data();
	UART2_TC("AT+CPIN?\r\n"); 
		while(!hand("READY")) 
	{
		clear_rec_data();
		UART2_TC("AT+CPIN?\r\n");//
		delay_ms(1000);//��ʱ
		UART_TC("SIM�����ڼ��...\r\n");
		beep(20,1);//������
	}
		UART_TC("����SIM���Ѽ�⵽����\r\n");
		beep(500,1);
}
void GSM_Network_Check()//�Ƿ����ӵ�����
{
		clear_rec_data();
	UART2_TC("AT+COPS?\r\n"); 
		while(!hand("CHINA")) 
	{
		clear_rec_data();
		UART2_TC("AT+COPS?\r\n");//
		delay_ms(1000);//��ʱ
		UART_TC("ע���������ڼ��...\r\n");
		beep(20,1);//������
	}
		UART_TC("������ע�ᵽ���硪��\r\n");
		beep(500,1);
}
void GSM_SMS_Send(uchar status,uchar *lat_str,uchar *lng_str)	//�����Ƿ���Ӣ�Ķ��Ŷ���
{                  
	uchar num=0,i=0;     
	uchar send_flag=1;
	static uchar message_1_A[]={"Your car may be stolen!!!\r\n http://uri.amap.com/marker?position="};
	static uchar message_1_B[]={"Your car is here:\r\n http://uri.amap.com/marker?position="};
	uchar message_2[]={"121.287689"};//Ĭ����ֵ
	uchar message_3[]={"31.234527"};//Ĭ����ֵ
	static uchar message_4[]={"&name=MyCar&src=mypage&coordinate=wgs84&callnative=1"};
	

	for(i=0;i<10;i++)
	{
		message_2[i]= lat_str[i];
		message_3[i]= lng_str[i];
	}
	
	
		clear_rec_data();
		UART2_TC("AT+CSCS=\"GSM\"\r\n"); //
		delay_ms(800);//��ʱ
	beep(10,1);//������
		UART2_TC("AT+CSCA?\r\n"); //�������ĺ���
		delay_ms(800);//��ʱ
	beep(10,1);//������
		UART2_TC("AT+CMGF=1\r\n");  //��ʽ1
		delay_ms(800);//��ʱ
	beep(10,1);//������
		UART2_TC("AT+CMGS=\"+8615313289652\"\r\n");  //�˴��޸Ķ��Ž��շ��绰��//////////////////////////////////////////15133849421
		delay_ms(800);//��ʱ
	beep(10,1);//������
	if(status==1)//���ܱ���
		UART2_TC(&message_1_A);  //�˴��޸Ķ�������
	else//�û�������ѯ
		UART2_TC(&message_1_B);  //�˴��޸Ķ�������
	delay_ms(100);//��ʱ
	UART2_TC(&message_2);  //�˴��޸Ķ�������
	delay_ms(100);//��ʱ
	UART2_T(',');  //�˴��޸Ķ�������
	delay_ms(100);//��ʱ
	UART2_TC(&message_3);  //�˴��޸Ķ�������
	delay_ms(100);//��ʱ
	UART2_TC(&message_4);  //�˴��޸Ķ�������
		delay_ms(800);//��ʱ
	beep(10,1);//������
		clear_rec_data();
		UART2_T(0X1A);
		delay_ms(800);//��ʱ
	beep(10,1);//������
	
	while(!hand("OK")) 
	{
		clear_rec_data();
		num++;
		delay_ms(800);//��ʱ
		UART_TC("�������ڷ���...\r\n");
		beep(10,1);//������
		if (num>20)
		{
			send_flag=0;
			break;
		}
	}
	if(send_flag==0)
	{
			UART_TC("�������ŷ���ʧ�ܡ���\r\n");
			beep(500,2);//����ʧ����������
	}
	else
	{
		UART_TC("�������ŷ��ͳɹ�����\r\n");
		beep(500,1);
	}
}
uchar GSM_Ring_Check()//����Ƿ��е绰����
{
	if(hand("RI"))///////RING
	{
		delay_ms(3000);
		UART2_TC("ATH\r\n");//�Ҷϴ�����ĵ绰
		clear_rec_data();
		return 1;
	}
	else
	{
		clear_rec_data();
		return 0;
	}
}


