#include "GPS.h"
#include <string.h>
#include <stdio.h>
#include "uart1.h"

#include "devices.h"
#include "delay.h"




static uchar GetComma(uchar num,char* str);
static void UTC2BTC(DATE_TIME *GPS);


//-----------------------------------------------------------------------
// 函  数: GPS_RMC_Parse(char *line,GPS_INFO *GPS)
// 描  述: 分析处理$GPRMC数据帧，获得经纬度等GPS数据
//***********************************************************************
// 参  数:	
//Input: num--个数;	 *str --字符串
//Output：返回num个','后的位置
//***********************************************************************
//$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh
//<1> UTC 时间,hhmmss(时分秒)格式
//<2> 定位状态，A=有效定位，V=无效定位
//<3> 纬度ddmm.mmmm(度分)格式(前面的0 也将被传输)
//<4> 纬度半球N(北半球)或S(南半球)
//<5> 经度dddmm.mmmm(度分)格式(前面的0 也将被传输)
//<6> 经度半球E(东经)或W(西经)
//<7> 地面速率(000.0~999.9 节，前面的0 也将被传输)
//<8> 地面航向(000.0~359.9 度，以真北为参考基准，前面的0 也将被传输)
//<9> UTC 日期，ddmmyy(日月年)格式
//<10> 磁偏角(000.0~180.0 度，前面的0 也将被传输)
//<11> 磁偏角方向，E(东)或W(西)
//<12> 模式指示(仅NMEA0183 3.00 版本输出，A=自主定位，D=差分，E=估算，N=数据无效)
//-----------------------------------------------------------------------
int GPS_RMC_Parse(char *line,GPS_INFO_TypDef *GPS)
{
	uchar ch, status;
		
////////////////////	float lati_cent_tmp, lati_second_tmp;
////////////////////	float long_cent_tmp, long_second_tmp;
	char *buf = line;
	
	
	ES=0;
	
	ch = buf[5];  					//$GPRMC---C
	status = buf[GetComma(2, buf)];

	if (ch == 'C')  //如果第五个字符是C，($GPRMC)       
	{
		if (status == 'A')  //如果数据有效，则分析      
		{
			GPS -> NS       = buf[GetComma(4, buf)];  		// 纬度半球N(北半球)或S(南半球)
			GPS -> EW       = buf[GetComma(6, buf)];		// 经度半球E(东经)或W(西经)
			
                                                         	
			GPS->latitude   = Get_Double_Number(&buf[GetComma(3, buf)]);  	//纬度ddmm.mmmm(度分)
			GPS->longitude  = Get_Double_Number(&buf[GetComma(5, buf)]);	//经度dddmm.mmmm(度分)
			
			//转换为度的形式
			GPS->latitude = (int)(GPS->latitude/100) + (GPS->latitude/100.0 - (int)(GPS->latitude/100)) *100.0 / 60.0;
			GPS->longitude = (int)(GPS->longitude/100) + (GPS->longitude/100.0 - (int)(GPS->longitude/100)) *100.0 / 60.0;

			 //printf("lng = %f\r\n",GPS->longitude);////////////////////////////////////////////////////////////////////////////////
//			sprintf(GPS->latitude_str, "%G", (double)resultlat); //产生字符串
//			sprintf(GPS->longitude_str, "%g", (double)resultlng); //产生字符串
			ES=1;
			return 1;
		}	
	}
	ES=1;
	return 0;
}



static double Str_To_Double(char *buf)
{
	double rev = 0;
	double dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;
}
												
//-----------------------------------------------------------------------
// 函  数: double Get_Double_Number(char *s)
// 描  述: 获得2个，之间的数据
//***********************************************************************
// 参  数:	
//Input: 
//Output：
//-----------------------------------------------------------------------
static double Get_Double_Number(char *s)
{
	char buf[10];
	uchar i;
	double rev;
	i = GetComma(1, s);		//得到下一个，位置
	i = i - 1;
	strncpy(buf, s, i);	    //获得2个，之间的数据
	buf[i] = 0;
	rev=Str_To_Double(buf);	//数据转换
	return rev;	
}


//-----------------------------------------------------------------------
// 函  数: uchar GetComma(uchar num,char *str)
// 描  述: 查找num个“，”所在字符串的位置
//***********************************************************************
// 参  数:	
//Input: num--个数;	 *str --字符串
//Output：返回num个','后的位置 
//-----------------------------------------------------------------------
static uchar GetComma(uchar num,char *str)
{
	uchar i,j = 0;
	int len = strlen(str);

	for(i = 0;i < len;i ++)
	{
		if(str[i] == ',')
			j++;
		if(j == num)
			return i + 1;	
	}

	return 0;	
}

uchar gnrmc_handler(GPS_INFO_TypDef *GPS)
{
	    if (rev_stop)   //如果接收完一行
    {
        if (GPS_RMC_Parse(rev_buf, GPS)) ////////////////解析GPRMC
        {
            gps_flag = 0;
            rev_stop  = 0;
					return 1;
        }
        else
        {
            gps_flag = 0;
            rev_stop  = 0;
        }
    }
		return 0;
}
void GPS_Search()//gps搜星等待
{
	GPS_INFO_TypDef gps_temp;
		while(!gnrmc_handler(&gps_temp))//持续搜索卫星
		{
			beep(20,1);
			UART_TC("正在搜索卫星...\r\n");
			delay_ms(1000);
		}
    UART_TC("——GPS定位成功——\r\n");
		beep(500,1);
}