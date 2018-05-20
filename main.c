
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
/*�̵��� - P2.4*/
/*��ģ�� - P3.2*/
/*ң��ģ�� (ȫ���Ͽ�) - P3.3*/
/*int0 - p3.2*/
/*int1 - p3.3*/
/*int - p3.4*/
/*int - p3.5*/

/*����1 - 3.0 3.1 */
/*����2 - 1.2 1.3*/
/*ADC - 1.0~1.7  ��1.5-1.7��*/


extern uchar arming_flag ;//���� ����
extern uchar shake_flag ;//��
extern uchar buzz_flag ;//����

GPS_INFO_TypDef   GPS_Data; 


/**************			��ʼ������					*********/
static void variable_init()	//��ʼ������
{
    arming_flag = 0;//���� ����
    shake_flag = 0;//��
    buzz_flag = 0;//����
    relay = 1;//Ĭ�Ͻ�ͨ����̵���
}


/**************			������				**********/
void main()
{
		uchar Latitude_String[12],Longitude_String[12];
	double temp;
	
		beep(100,1);
		delay_ms(200);
    arming_init();	//���������ʼ��
    variable_init();	//������ʼ��
    Uart_Init();
    Uart2_Init();
    UART_TC("--ϵͳ��ʼ���ɹ�����\r\n");
		GPS_Search();//gps���ǵȴ�

		GSM_Sync_Baudrate();//ͬ�������ʺ���
		GSM_Hand();//���ֺ���
		GSM_Signal_quality();//�ź�����
		GSM_SIM_Check();//�Ƿ��⵽SIM��Ƭ
		GSM_Network_Check();//�Ƿ����ӵ�����
	
    while(1)
    {

        if(buzz_flag)//��������������һ��
        {
						UART_TC("��������/��������\r\n");
            beep(200,buzz_flag);
            buzz_flag=0;
        }
        if(shake_flag == 1)//��⵽������
        {
						UART_TC("�����쳣�𶯡���\r\n");
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
				
				if(GSM_Ring_Check())//����е绰����
				{
					UART_TC("�����е绰��������\r\n");
					GSM_SMS_Send(Longitude_String,Latitude_String);
				}
				
				
			  delay_ms(500);
				UART_TC("����������������\r\n");
				
    }

}


