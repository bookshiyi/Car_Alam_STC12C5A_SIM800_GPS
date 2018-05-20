#ifndef __GPS_H_
#define __GPS_H_

#define uchar unsigned char
#define uint  unsigned int



typedef struct{
	int year;  
	int month; 
	int  day;
	int hour;
	int minute;
	int second;
}DATE_TIME;

typedef  struct{
	double  latitude; //γ�� 
	double  longitude; //����
	
//		double  resultlng;  //����  ת���ɶȵ���ʽ
//		double  resultlat; //γ��		ת���ɶȵ���ʽ
	uchar 	NS;
	uchar 	EW;
	DATE_TIME D;
}GPS_INFO_TypDef;



int GPS_RMC_Parse(char *line,GPS_INFO_TypDef *GPS);
static double Get_Double_Number(char *s);
static double Str_To_Double(char *buf);
uchar gnrmc_handler(GPS_INFO_TypDef *GPS);//�������ݽ�������
void GPS_Search();//gps���ǵȴ�

#endif  //__GPS_H_