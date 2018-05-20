#include "devices.h"
#include "delay.h"


uchar arming_flag =0;//���� ����
uchar shake_flag  =0;//��
uchar buzz_flag   =0;//����


void beep(uint time,uchar num)
{
	uint i,j;
	for(i=0;i<num;i++)
	{
		for(j=0;j<time;j++)
		{
			buzz=!buzz;
			delay_ms(2);
		}
		delay_ms(time);
	}
}
void shake_input(uchar i)		//�𶯴��������뿪��
{
	if(i)//���𶯴���������
	{
		IT0=1;
		EX0=1;
	}
	else	//�ر��𶯴���������
	{
		IT0=0;
		EX0=0;
		
	}
}
void arming_init()
{
	//WAKE_CLKO = 0X20;		//����P3.5�½��ػ���CPU
	IT1=1;									//�½��ش���
	EX1=1;
	EA=1;
}


/*******************            �ж�          *********************/
void exint1() interrupt 2	/*����/����*/
{
    arming_flag = !arming_flag;
    if(arming_flag == 1)//����
    {
        relay=0;				//�Ͽ�����̵���
        buzz_flag = 2;	//��������
        shake_input(1);	//��������
    }
    else if(arming_flag == 0)//����
    {
        relay=1;			//��ͨ����̵���
        buzz_flag = 1;//����һ��
        shake_input(0);	//�ر�������
    }
}
void exint0() interrupt 0	/*�񶯴�����*/
{
    static uchar trig_num=0;	//�ֲ���̬����(�𶯴�������)
    trig_num++;
    if(trig_num>5)					//��ֹ�󴥷���ģ��
    {
        trig_num=0;
        shake_flag = 1;
    }
}