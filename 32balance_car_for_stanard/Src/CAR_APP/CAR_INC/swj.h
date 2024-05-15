#ifndef SWJ_H
#define SWJ_H
#include "stm32f1xx.h"
#include "usart.h"
#include "stdint.h"
//���¶���Ϊ������mpu 6050��ԭʼ���ݵ���
//ʹ��accx1
#define aacx1 accel[0]
#define aacy1 accel[1]
#define aacz1 accel[2]
#define gyrox1 gyro[0]
#define gyroy1 gyro[1]
#define gyroz1 gyro[2]
void usart1_send_char(uint8_t c);//����һ���ֽڸ���λ��
void usart1_niming_report(uint8_t fun,uint8_t*data,uint8_t len);//����λ����������
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);//��������������
//�ɿ�ģ��
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
void swj(void);
extern	float pitch,roll,yaw; 		//ŷ����
//extern	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
//extern	short gyrox,gyroy,gyroz;	//������ԭʼ����
		
#endif
