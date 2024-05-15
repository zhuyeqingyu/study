#ifndef _CAR_TASK_H
#define _CAR_TASK_H

struct mpu6050_data{
	short acc_x;
	short acc_y;
	short acc_z;
	
	short gyro_x;
	short gyro_y;
	short gyro_z;
	
	float pitch;
	float roll;
	float yaw;
	
};




extern struct mpu6050_data outMpu;
extern float Movement;
extern int Contrl_Turn;

void Car_Task_200HZ(void);
void Car_Task_100HZ(void);
void Car_Task_5HZ(void);
void HC_SRC04_Start(void);
void Car_Task_Interaction(void);
#endif
