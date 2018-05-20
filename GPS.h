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
	double  latitude; //纬度 
	double  longitude; //经度
	
//		double  resultlng;  //经度  转化成度的形式
//		double  resultlat; //纬度		转化成度的形式
	uchar 	NS;
	uchar 	EW;
	DATE_TIME D;
}GPS_INFO_TypDef;



int GPS_RMC_Parse(char *line,GPS_INFO_TypDef *GPS);
static double Get_Double_Number(char *s);
static double Str_To_Double(char *buf);
uchar gnrmc_handler(GPS_INFO_TypDef *GPS);//卫星数据解析函数
void GPS_Search();//gps搜星等待

#endif  //__GPS_H_