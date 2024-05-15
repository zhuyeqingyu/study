#include "math.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "contrl.h"



//��Ҫ�������ļ���ĳ���ı�������.c����pwm(.h)��Ӧ�ļĴ���

int   Dead_Zone=3500;    //�������4250
int   control_turn=64;                             //ת�����


//PID���ڲ���
struct pid_arg PID = {
    .Balance_Kp=200, //-261 183.7  100
    .Balance_Kd=0.8,//-0.68 0.335   0.8
    .Velocity_Kp=50,//-98 44
    .Velocity_Ki=1,//-1.13 0.6
    .Turn_Kp = 0,//���� 30 
    .Turn_Kd = 0,//���� 0.1
};

/**************************************************************************************************************
*������:Read_Encoder()
*����:��ȡ������ֵ(����С����ǰǰ�����ٶ�)
*�β�:(u8 TIMX):xΪ������1����2
*����ֵ:��
*************************************************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;

    switch(TIMX)
    {
    case 1:
        Encoder_TIM= (short)TIM2 -> CNT;
        TIM2 -> CNT=0;
        break;
    case 2:
        Encoder_TIM= (short)TIM4 -> CNT;
        TIM4 -> CNT=0;
        break;
    default:
        Encoder_TIM=0;
    }
    return Encoder_TIM;
}



/**************************************************************************************************************
*������:Vertical_Ring_PD()
*����:ֱ����PD����
*�β�:(float Angle):x��ĽǶ�/(float Gyro):x��Ľ��ٶ�
*����ֵ:����PIDת��֮���PWMֵ
**************************************************************************************************************/
//ֱ������PD


int	Vertical_Ring_PD(float Angle,float Gyro)
{
    float Bias;//Ŀ��ƫ��
    int balance;
    Bias=Angle-Mechanical_balance;//Ŀ��ֵ��ȥ��е��ֵ����һ��Ϊ0��
    balance=PID.Balance_Kp*Bias+ Gyro*PID.Balance_Kd;

    return balance;

    //printf("balance = %f\n",balance);
}


/**************************************************************************************************************
*������:Vertical_speed_PI()
*���ܣ��ٶȻ�PI����
*�β�:(int encoder_left):���ֱ�����ֵ/(int encoder_right):���������ֵ�ֵ/(float Angle):x��Ƕ�ֵ
*����ֵ:
**************************************************************************************************************/

int Vertical_speed_PI(int encoder_left,int encoder_right,float Angle,float Movement )
{
    static float Velocity,Encoder_Least,Encoder;
    static float Encoder_Integral;
    Encoder_Least =(encoder_left+encoder_right)-0;    //��ȡ�����ٶ�ƫ��=�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩
    Encoder *= 0.8f;																	//һ�׵�ͨ�˲��� ���ϴε��ٶ�ռ85%
    Encoder += Encoder_Least*0.2f;                   //һ�׵�ͨ�˲����� ���ε��ٶ�ռ15%
    Encoder_Integral +=Encoder;                       //���ֳ�λ�� ����ʱ�䣺10ms
    Encoder_Integral=Encoder_Integral-Movement;
    //if(Movement == 0 ) Encoder_Integral=0;
    if(Encoder_Integral>10000)  	Encoder_Integral=10000;           //�����޷�
    if(Encoder_Integral<-10000)	  Encoder_Integral=-10000;            //�����޷�

    Velocity=Encoder*PID.Velocity_Kp+Encoder_Integral*PID.Velocity_Ki;      //�ٶȿ���


    if(Turn_off(Angle)==1)   Encoder_Integral=0;            //����رպ��������
    return Velocity;
}


/**************************************************************************************************************
*������:Vertical_turn_PD()
*����:ת��PD
*�β�:taget_yaw Ŀ��yaw�� yaw ������yaw �� gyro ������yaw������ٶ�
*����ֵ:��
***************************************************************************************************************/
int Vertical_turn_PD(float taget_yaw,float yaw,float gyro)
{
    float Turn;
    float Bias_yaw;
    Bias_yaw=taget_yaw-yaw;
		if (Bias_yaw<-180) Bias_yaw+=360;
		if (Bias_yaw>180) Bias_yaw-=360;
	
    Turn=-Bias_yaw*PID.Turn_Kp-gyro*PID.Turn_Kd;
    return Turn;
}



/**************************************************************************************************************
*������:PWM_Limiting()
*����:PWM�޷�����
*�β�:��
*����ֵ:��
***************************************************************************************************************/
void PWM_Limiting(int *motor1,int *motor2)
{
    int Amplitude=7999;//3204
    if(*motor1<-Amplitude) *motor1=-Amplitude;
    if(*motor1>Amplitude)  *motor1=Amplitude;
    if(*motor2<-Amplitude) *motor2=-Amplitude;
    if(*motor2>Amplitude)  *motor2=Amplitude;
}


/**************************************************************************************************************
*������:Turn_off()
*����:�رյ��
*�β�:(const float Angle):x��Ƕ�ֵ
*����ֵ:1:С����ǰ����ֹͣ״̬/0:С����ǰ��������״̬
***************************************************************************************************************/
u8 FS_state;

u8 Turn_off(const float Angle)
{
    u8 temp;
    if(fabs(Angle)>60) {
        FS_state=1;
        temp=1;
        PWMA1=0;
        PWMA2=0;
        PWMB1=0;
        PWMB2=0;
    }
    else
        temp=0;
    FS_state=0;
    return temp;
}

/**************************************************************************************************************
*������:Set_PWM()
*����:���PWM���Ƶ��
*�βΣ�(int motor1):���1��Ӧ��PWMֵ/(int motor2):���2��Ӧ��PWMֵ
*����ֵ:��
*************************************************************************************************************/


//void Set_PWM(int motor1,int motor2)
//{
//	if(motor1<0)	{
//		AIN2(0);
//		PWMA=Dead_Zone+(abs(motor1))*1.17;
//	}
//	else 	{
//		AIN2(1);
//	PWMA=7999-Dead_Zone-(abs(motor1))*1.17;
//	}
//	if(motor2>0)	{
//		BIN2(1);
//		PWMB=7999-Dead_Zone-(abs(motor2))*1.17;
//	}
//	else {
//		BIN2(0);
//		PWMB=Dead_Zone+(abs(motor2))*1.17;
//	}
//	printf("PWMA = %d\n",PWMA);
//	printf("PWMB = %d\n",7999-PWMB);
//}


void Set_PWM(int motor1,int motor2)
{
    if(motor1<0)
    {
        PWMA2=Dead_Zone+(abs(motor1))*1.17;
        PWMA1=0;

    }
    else
    {
        PWMA2 =0;
        PWMA1=Dead_Zone+(abs(motor1))*1.17;
    }


    if(motor2>0)		{

        PWMB2 = Dead_Zone+(abs(motor2))*1.17;
        PWMB1 = 0;
    }
    else
    {   PWMB2 = 0;
        PWMB1=Dead_Zone+(abs(motor2))*1.17;
    }
}