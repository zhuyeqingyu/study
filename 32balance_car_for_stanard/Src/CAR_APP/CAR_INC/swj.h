#ifndef SWJ_H
#define SWJ_H
#include "stm32f1xx.h"
#include "usart.h"
#include "stdint.h"
//以下定义为避免与mpu 6050的原始数据调用
//使用accx1
#define aacx1 accel[0]
#define aacy1 accel[1]
#define aacz1 accel[2]
#define gyrox1 gyro[0]
#define gyroy1 gyro[1]
#define gyroz1 gyro[2]
void usart1_send_char(uint8_t c);//发送一个字节给上位机
void usart1_niming_report(uint8_t fun,uint8_t*data,uint8_t len);//向上位机发送数据
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);//发送陀螺仪数据
//飞控模型
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
void swj(void);
extern	float pitch,roll,yaw; 		//欧拉角
//extern	short aacx,aacy,aacz;		//加速度传感器原始数据
//extern	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
		
#endif
