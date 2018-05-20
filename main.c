
#include "common.h"
#include "GPS.h"
#include "stdio.h"
#include "string.h"
#include "GSM.h"
#include "delay.h"
#include "uart1.h"
#include "devices.h"
#include "uart2.h"

/*beep - P2.5*/
/*继电器 - P2.4*/
/*震动模块 - P3.2*/
/*遥控模块 (全部断开) - P3.3*/
/*int0 - p3.2*/
/*int1 - p3.3*/
/*int - p3.4*/
/*int - p3.5*/

/*串口1 - 3.0 3.1 */
/*串口2 - 1.2 1.3*/
/*ADC - 1.0~1.7  （1.5-1.7）*/


extern uchar arming_flag ;//布放 撤防
extern uchar shake_flag ;//震动
extern uchar buzz_flag ;//鸣叫

GPS_INFO_TypDef   GPS_Data; 


/**************			初始化函数					*********/
static void variable_init()	//初始化变量
{
    arming_flag = 0;//布放 撤防
    shake_flag = 0;//震动
    buzz_flag = 0;//鸣叫
    relay = 1;//默认接通引擎继电器
}


/**************			主函数				**********/
void main()
{
		uchar Latitude_String[12],Longitude_String[12];
	double temp;
	
		beep(100,1);
		delay_ms(200);
    arming_init();	//布放输入初始化
    variable_init();	//变量初始化
    Uart_Init();
    Uart2_Init();
    UART_TC("--系统初始化成功——\r\n");
		GPS_Search();//gps搜星等待

		GSM_Sync_Baudrate();//同步波特率函数
		GSM_Hand();//握手函数
		GSM_Signal_quality();//信号质量
		GSM_SIM_Check();//是否检测到SIM卡片
		GSM_Network_Check();//是否连接到网络
	
    while(1)
    {

        if(buzz_flag)//布放两声，撤防一声
        {
						UART_TC("——布放/撤防——\r\n");
            beep(200,buzz_flag);
            buzz_flag=0;
        }
        if(shake_flag == 1)//检测到震动输入
        {
						UART_TC("——异常震动——\r\n");
            beep(80,5);
            shake_flag=0;

					
						GSM_SMS_Send(Longitude_String,Latitude_String);
        }
				
				gnrmc_handler(&GPS_Data);
				
				temp = GPS_Data.latitude;		
				//printf("lat = %f\r\n",temp);
				sprintf(Latitude_String, "%f", (double)temp);
				printf("lat_str = %s\r\n",Latitude_String);

				temp = GPS_Data.longitude;		
				//printf("lng = %f\r\n",temp);
				sprintf(Longitude_String, "%f", (double)temp);
				printf("lng_str = %s\r\n",Longitude_String);
				
				if(GSM_Ring_Check())//如果有电话打来
				{
					UART_TC("——有电话打来——\r\n");
					GSM_SMS_Send(Longitude_String,Latitude_String);
				}
				
				
			  delay_ms(500);
				UART_TC("——运行正常——\r\n");
				
    }

}


