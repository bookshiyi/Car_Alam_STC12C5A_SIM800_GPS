#ifndef _GSM_H_
#define _GSM_H_
#include "common.h"
#include "devices.h"



uchar hand(uchar *ptr);//��gsmģ��������ֺ���
void clear_rec_data();//������ڶ����յ�������

void GSM_Sync_Baudrate();//ͬ�������ʺ���
void GSM_Hand();//���ֺ���
void GSM_Signal_quality();//�ź�����
void GSM_SIM_Check();//�Ƿ��⵽SIM��Ƭ
void GSM_Network_Check();//�Ƿ����ӵ�����

void GSM_SMS_Send(uchar *lat_str,uchar *lng_str);	//�����Ƿ���Ӣ�Ķ��Ŷ���
uchar GSM_Ring_Check();//����Ƿ��е绰����
#endif