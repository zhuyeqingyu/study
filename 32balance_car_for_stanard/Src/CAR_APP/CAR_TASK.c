#include "CAR_TASK.h"
#include "tim.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "echo.h"
#include "stdio.h"
#include "contrl.h"
#include "delay.h"
#include "my_usart.h"
#include "oled.h"
#define RXBUFFER_LEN 20


struct mpu6050_data outMpu;
int Balance_PWm,Velocity_Pwm,Turn_Pwm=0;//ֱ�������ٶȻ������pwm
int Encoder_left,Encoder_right;//������ֵ
int Motor1,Motor2;//���ҵ�����������pwm
float  Movement = 0;//Ŀ���ٶ�
float taget_yaw = 0;//Ŀ��Ƕ�
int Contrl_Turn = 64;//
int hf_Mode = 0;//С������ģʽ 0 ң��ģʽ��1 ����ģʽ��2 ���������� 3Ѳ��ģʽ��ûд
int srflow_dis = 250,dis_err = 0;//����������������
int rxloss_flag = 1,avoidance_flag = 0;
int rx_cnt = 0;//ʧ�ر��


//�������ݲɼ�����
void Car_Task_200HZ(void)
{
    //static struct mpu6050_data Last_Data;
    mpu_dmp_get_data(&outMpu.pitch,&outMpu.roll,&outMpu.yaw);

}


/**************************************************************************************************************
*������:Car_Task_100HZ(void)
*���ܣ�100hz����
*�β�:��
*����ֵ:��
*ȷ��ֱ�������ٶȻ���ת���Լ����õ��pwm
**************************************************************************************************************/
void Car_Task_100HZ(void)
{
		hf_Mode = BT_Data.mode;
    switch(hf_Mode) {
    case 0://ң��ģʽ
        Movement = BT_Data.y*1.2;
        break;
    case 1://����ģʽ
        if(dis<250 && BT_Data.y>0) {
            Movement=0;
            avoidance_flag = 1;
        }
        else if(dis<200)  {
            Movement = -(200-dis)*0.25;
            avoidance_flag = 1;
        }
        else {
					avoidance_flag = 0;
					Movement = BT_Data.y*0.5;
				}
        break;
		case 2://����������
			dis_err = srflow_dis-dis;
			if (dis>500||(dis_err<30&&dis_err>-30)){//̫Զ�˾����˲�����
				Movement = 0;
			}
			else{
			Movement = -dis_err*0.15;
			}
			break;

    default:
        break;

    }
		taget_yaw -= BT_Data.x*0.015;
    if(taget_yaw >180) taget_yaw -= 360;
    if(taget_yaw <-180) taget_yaw +=360;
    Encoder_left = Read_Encoder(1);
    Encoder_right = -Read_Encoder(2);//��������
    Balance_PWm =Vertical_Ring_PD(outMpu.pitch,outMpu.gyro_y);//ֱ����
    Velocity_Pwm = Vertical_speed_PI(Encoder_left,Encoder_right,outMpu.pitch,Movement );//�ٶȻ�
    Turn_Pwm = -Vertical_turn_PD(taget_yaw,outMpu.yaw,outMpu.gyro_z);
    Motor1 = Balance_PWm+Velocity_Pwm+Turn_Pwm ;
    Motor2 = Balance_PWm+Velocity_Pwm-Turn_Pwm ;
		Motor2*=1.08;
    PWM_Limiting(&Motor1,&Motor2);//pwm�޷�
		if(Turn_off(outMpu.pitch)!=1) Set_PWM(Motor1,Motor2);

   
}



/**************************************************************************************************************
*������:Car_Task_5HZ(void)
*���ܣ�5hz����
*�β�:��
*����ֵ:��
*ȷ��ֱ�������ٶȻ���ת���Լ����õ��pwm
**************************************************************************************************************/
void Car_Task_5HZ(void)
{
	Getdistance();
}

void Car_Task_Interaction(void)
{
	
   if(BT_Data.rxflag == 0){
//		rx_cnt ++;
	}
	else{
		rx_cnt =0;
		BT_Data.rxflag =0;
	}
			
		if (rx_cnt >5){
			if(rxloss_flag ==0) {
				OLED_CLS();
				OLED_ShowStr(40,3,"rxloss",2);
				rxloss_flag = 1;
			}
			OLED_ShowNum(40,5,rx_cnt,8,2);
			
		}
		
		else{
			if(rxloss_flag ==1){
				OLED_CLS();
				OLED_ShowStr (1,1,"pitch:",1);
				OLED_ShowStr (1,3,"roll:",1);
				OLED_ShowStr (1,5,"yaw:",1);
				OLED_ShowStr (1,7,"dis:",1);
				rxloss_flag = 0;
		}
			if(outMpu.pitch<0)
    {
        OLED_ShowChar(46,1,'-',2);
        OLED_ShowFloat(50,1,-outMpu.pitch,3,2);
    }
    else
    {
        OLED_ShowChar(46,1,' ',2);
        OLED_ShowFloat(50,1,outMpu.pitch,3,2);
    }

    if(outMpu.roll<0)
    {
        OLED_ShowChar(46,3,'-',2);
        OLED_ShowFloat(50,3,-outMpu.roll,3,2);
    }
    else
    {
        OLED_ShowChar(46,3,' ',2);
        OLED_ShowFloat(50,3,outMpu.roll,3,2);
    }

    if(outMpu.yaw<0)
    {
        OLED_ShowChar(46,5,'-',2);
        OLED_ShowFloat(50,5,-outMpu.yaw,3,2);
    }
    else
    {
        OLED_ShowChar(46,5,' ',2);
        OLED_ShowFloat(50,5,outMpu.yaw,3,2);
    }
		
        OLED_ShowChar(46,7,' ',2);
       // OLED_ShowFloat(50,7,dis,4,2);
			 OLED_ShowNum(56,7,dis,4,2);
			OLED_ShowStr(80,7,"mm",1);
		if (avoidance_flag == 1) OLED_ShowStr(95,7,"!!",1);
		else OLED_ShowStr(95,7,"  ",1);
	}
}
