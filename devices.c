#include "devices.h"
#include "delay.h"


uchar arming_flag =0;//布放 撤防
uchar shake_flag  =0;//震动
uchar buzz_flag   =0;//鸣叫


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
void shake_input(uchar i)		//震动传感器输入开关
{
	if(i)//打开震动传感器输入
	{
		IT0=1;
		EX0=1;
	}
	else	//关闭震动传感器输入
	{
		IT0=0;
		EX0=0;
		
	}
}
void arming_init()
{
	//WAKE_CLKO = 0X20;		//开启P3.5下降沿唤醒CPU
	IT1=1;									//下降沿触发
	EX1=1;
	EA=1;
}


/*******************            中断          *********************/
void exint1() interrupt 2	/*布放/撤防*/
{
    arming_flag = !arming_flag;
    if(arming_flag == 1)//布放
    {
        relay=0;				//断开引擎继电器
        buzz_flag = 2;	//鸣叫两次
        shake_input(1);	//打开震动输入
    }
    else if(arming_flag == 0)//撤防
    {
        relay=1;			//接通引擎继电器
        buzz_flag = 1;//鸣叫一次
        shake_input(0);	//关闭震动输入
    }
}
void exint0() interrupt 0	/*振动传感器*/
{
    static uchar trig_num=0;	//局部静态变量(震动触发次数)
    trig_num++;
    if(trig_num>5)					//防止误触发震动模块
    {
        trig_num=0;
        shake_flag = 1;
    }
}