#ifndef _GSM_H_
#define _GSM_H_
#include "common.h"
#include "devices.h"



uchar hand(uchar *ptr);//和gsm模块进行握手函数
void clear_rec_data();//清除串口二接收到的数据

void GSM_Sync_Baudrate();//同步波特率函数
void GSM_Hand();//握手函数
void GSM_Signal_quality();//信号质量
void GSM_SIM_Check();//是否检测到SIM卡片
void GSM_Network_Check();//是否连接到网络

void GSM_SMS_Send(uchar *lat_str,uchar *lng_str);	//以下是发送英文短信短信
uchar GSM_Ring_Check();//检查是否有电话打来
#endif