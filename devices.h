#ifndef _DEVICES_H_
#define _DEVICES_H_
#include "common.h"

sbit buzz=P2^5;
sbit relay=P2^4;
sbit arm=P3^5;/*布放撤防按键*/

extern uchar arming_flag ;//布放 撤防
extern uchar shake_flag ;//震动
extern uchar buzz_flag ;//鸣叫

void beep(uint time,uchar num);
void shake_input(uchar i);
void arming_init();
void exint1();	/*布放/撤防*/
void exint0();	/*振动传感器*/
#endif