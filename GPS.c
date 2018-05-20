#include "GPS.h"
#include <string.h>
#include <stdio.h>
#include "uart1.h"

#include "devices.h"
#include "delay.h"




static uchar GetComma(uchar num,char* str);
static void UTC2BTC(DATE_TIME *GPS);


//-----------------------------------------------------------------------
// ��  ��: GPS_RMC_Parse(char *line,GPS_INFO *GPS)
// ��  ��: ��������$GPRMC����֡����þ�γ�ȵ�GPS����
//***********************************************************************
// ��  ��:	
//Input: num--����;	 *str --�ַ���
//Output������num��','���λ��
//***********************************************************************
//$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh
//<1> UTC ʱ��,hhmmss(ʱ����)��ʽ
//<2> ��λ״̬��A=��Ч��λ��V=��Ч��λ
//<3> γ��ddmm.mmmm(�ȷ�)��ʽ(ǰ���0 Ҳ��������)
//<4> γ�Ȱ���N(������)��S(�ϰ���)
//<5> ����dddmm.mmmm(�ȷ�)��ʽ(ǰ���0 Ҳ��������)
//<6> ���Ȱ���E(����)��W(����)
//<7> ��������(000.0~999.9 �ڣ�ǰ���0 Ҳ��������)
//<8> ���溽��(000.0~359.9 �ȣ����汱Ϊ�ο���׼��ǰ���0 Ҳ��������)
//<9> UTC ���ڣ�ddmmyy(������)��ʽ
//<10> ��ƫ��(000.0~180.0 �ȣ�ǰ���0 Ҳ��������)
//<11> ��ƫ�Ƿ���E(��)��W(��)
//<12> ģʽָʾ(��NMEA0183 3.00 �汾�����A=������λ��D=��֣�E=���㣬N=������Ч)
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

	if (ch == 'C')  //���������ַ���C��($GPRMC)       
	{
		if (status == 'A')  //���������Ч�������      
		{
			GPS -> NS       = buf[GetComma(4, buf)];  		// γ�Ȱ���N(������)��S(�ϰ���)
			GPS -> EW       = buf[GetComma(6, buf)];		// ���Ȱ���E(����)��W(����)
			
                                                         	
			GPS->latitude   = Get_Double_Number(&buf[GetComma(3, buf)]);  	//γ��ddmm.mmmm(�ȷ�)
			GPS->longitude  = Get_Double_Number(&buf[GetComma(5, buf)]);	//����dddmm.mmmm(�ȷ�)
			
			//ת��Ϊ�ȵ���ʽ
			GPS->latitude = (int)(GPS->latitude/100) + (GPS->latitude/100.0 - (int)(GPS->latitude/100)) *100.0 / 60.0;
			GPS->longitude = (int)(GPS->longitude/100) + (GPS->longitude/100.0 - (int)(GPS->longitude/100)) *100.0 / 60.0;

			 //printf("lng = %f\r\n",GPS->longitude);////////////////////////////////////////////////////////////////////////////////
//			sprintf(GPS->latitude_str, "%G", (double)resultlat); //�����ַ���
//			sprintf(GPS->longitude_str, "%g", (double)resultlng); //�����ַ���
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
// ��  ��: double Get_Double_Number(char *s)
// ��  ��: ���2����֮�������
//***********************************************************************
// ��  ��:	
//Input: 
//Output��
//-----------------------------------------------------------------------
static double Get_Double_Number(char *s)
{
	char buf[10];
	uchar i;
	double rev;
	i = GetComma(1, s);		//�õ���һ����λ��
	i = i - 1;
	strncpy(buf, s, i);	    //���2����֮�������
	buf[i] = 0;
	rev=Str_To_Double(buf);	//����ת��
	return rev;	
}


//-----------------------------------------------------------------------
// ��  ��: uchar GetComma(uchar num,char *str)
// ��  ��: ����num�������������ַ�����λ��
//***********************************************************************
// ��  ��:	
//Input: num--����;	 *str --�ַ���
//Output������num��','���λ�� 
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
	    if (rev_stop)   //���������һ��
    {
        if (GPS_RMC_Parse(rev_buf, GPS)) ////////////////����GPRMC
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
void GPS_Search()//gps���ǵȴ�
{
	GPS_INFO_TypDef gps_temp;
		while(!gnrmc_handler(&gps_temp))//������������
		{
			beep(20,1);
			UART_TC("������������...\r\n");
			delay_ms(1000);
		}
    UART_TC("����GPS��λ�ɹ�����\r\n");
		beep(500,1);
}