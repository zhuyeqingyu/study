#ifndef _MY_USART_H
#define _MY_USART_H

#include "main.h"

#define RXBUFFER_LEN 20

typedef struct User_USART
{
		uint8_t Rx_flag;									//接收完成标志
		uint8_t Rx_len;										//接收长度
		uint8_t frame_head;						//帧头
		uint8_t frame_tail;								//帧尾
		int x,y,mode,rxflag;
		uint8_t RxBuffer[RXBUFFER_LEN];		//数据存储
}User_USART;

extern User_USART BT_Data;
void BTData_Process(uint8_t *RxBuffer);
void User_USART_Init(User_USART *Data);
#endif


