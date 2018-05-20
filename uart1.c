
#include "common.h"
#include "uart1.h"
#include "devices.h"

  
uchar num = 0; 
uchar rev_start = 0;     //���տ�ʼ��־      
uchar rev_stop  = 0;     //����ֹͣ��־     
uchar gps_flag = 0;      //GPS�����־     
char  rev_buf[80];        //���ջ��� 


void Uart_Init(void)
{
    //��Ϊʹ��12Mhz���������޷�ʹ��9600������
    SCON=0X50;			//����Ϊ������ʽ1
    TMOD=0X20;			//���ü�����������ʽ2��TL1��Ϊ������TH1��Ϊ��װֵ
    PCON=0X80;			//80�����ʼӱ�  00���ӱ�
    TH1=0xfa;				//��������ʼֵ���ã�ע�Ⲩ������4800�ģ�12c5a 9600 ��Ӧ0xfa��
    TL1=0x00;
    ES=1;						//�򿪽����ж�
    EA=1;						//�����ж�
    TR1=1;					//�򿪼�����
}

void UART_T (unsigned char UART_data){ //���崮�ڷ������ݱ���
	ES=0;
	SBUF = UART_data;	//�����յ����ݷ��ͻ�ȥ
	while(TI == 0);		//��鷢���жϱ�־λ
	TI = 0;			//����жϱ�־λΪ0��������㣩
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
		if ((ch == '$') && (gps_flag == 0))  //����յ��ַ�'$'���㿪ʼ����           
		{
			rev_start = 1;
			rev_stop  = 0;
		}
	
		if (rev_start == 1)  //��־λΪ1����ʼ����            
		{
			rev_buf[num++] = ch;  //�ַ��浽������           
			if (ch == '\n')     //������յ�����           
			{
				rev_buf[num] = '\0';
				rev_start = 0;
				rev_stop  = 1;
				gps_flag = 1;
				num = 0;
			}
		}
	}
	RI = 0;	 //RI��0�����½���            
	ES = 1;	
}
//��дputchar����
char putchar(char c)
{
    UART_T(c);
    return c;
}
