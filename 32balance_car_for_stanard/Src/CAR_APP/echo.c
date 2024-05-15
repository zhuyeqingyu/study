#include "tim.h"
#include "stm32f1xx_hal_tim.h"
#include "echo.h"
#include "delay.h"
#include "sys_time.h"

#include "stdio.h"
#include "cmsis_os.h"
#include "math.h"

#include "CAR_TASK.h"
uint16_t capture_Buf[4] = {0};
uint8_t capture_Cnt = 0;
uint32_t high_time;
float dis;
float i=0;


void Getdistance(void)
{
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
//    delay_us(20);
//		osDelay(1);	三种延时方式选其一即可
		uint32_t time;
		time = Get_sys_time_us() + 10;
		while(Get_sys_time_us()<time){}

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
    capture_Cnt++;

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin ==GPIO_PIN_8) {

        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==GPIO_PIN_SET)//如果是上升沿
        {
            capture_Buf[1] = Get_sys_time_ms();
            capture_Buf[0] = Get_sys_time_only_us();
            capture_Cnt++;

        }
        else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==GPIO_PIN_RESET)//如果是下降沿
        {
            capture_Buf[3] = Get_sys_time_ms();
            capture_Buf[2] = Get_sys_time_only_us();
            high_time = (capture_Buf[3]- capture_Buf[1])*1000+capture_Buf[2]-capture_Buf[0];
            dis=(float)high_time*0.17*cos(fabs(outMpu.pitch)*3.14/180)*0.8+dis*0.2;//单位是mm 角度补偿加一阶滤波
            printf("dis = %f\n",dis);
            capture_Cnt=0;
        }

    }

}
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//	if(TIM1 == htim->Instance)
//	{
//		switch(capture_Cnt)
//		{
//			case 1:
//				capture_Buf[0] = HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_4);
//				__HAL_TIM_SET_CAPTUREPOLARITY(&htim1,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
//				capture_Cnt++;
//				break;
//			case 2:
//				capture_Buf[1] = HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_4);
//				capture_Cnt++;
//		}
//	}
//	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);
//}


