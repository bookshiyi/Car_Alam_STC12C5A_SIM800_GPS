
#include "uart2.h"
#include "GSM.h"


extern uchar rec_data[50]=0;
extern uchar rec_num=0;
 //=========baud 9600=======================
void Uart2_Init()			 //����2��ʼ��������������9600
{
	S2CON=  0x50   ;							//��ʽ2���������
//	BRT  =  RELOAD_TWO ;
	BRT = 0xfd;									 //���ò�����9600
	AUXR =  AUXR |0X10   ;		 	 //������������ʷ���������
//	BRTx12_enable();
	AUXR1 =  AUXR1&0xef   ;	 //S2_P4=0,��uart2�л���P1��
	IE2  =  IE2|0X01;			//������2�ж�
}


void UART2_T (unsigned char UART_data)
{ //���崮�ڷ������ݱ���
	ES=0;
	S2BUF = UART_data;	//�����յ����ݷ��ͻ�ȥ
	while((S2CON&S2TI)!=0x02);   //��鷢���жϱ�־λ
  S2CON &= ~S2TI;              //����жϱ�־λΪ0��������㣩
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

void Uart2_Receive(void) interrupt 8//����2�Ľ����ն˷������
{
	uchar ch;
	ES = 0;

	if(S2CON & S2RI)//����ǽ����ж�
	{
		ch = S2BUF; 	
			rec_data[rec_num++]=ch;
			if(rec_num>=50)
			rec_num=0;
	}  

		S2CON &=~S2RI;				//��0 S2RI	         
	ES = 1;	
}




