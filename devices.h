#ifndef _DEVICES_H_
#define _DEVICES_H_
#include "common.h"

sbit buzz=P2^5;
sbit relay=P2^4;
sbit arm=P3^5;/*���ų�������*/

extern uchar arming_flag ;//���� ����
extern uchar shake_flag ;//��
extern uchar buzz_flag ;//����

void beep(uint time,uchar num);
void shake_input(uchar i);
void arming_init();
void exint1();	/*����/����*/
void exint0();	/*�񶯴�����*/
#endif