#include "my_usart.h"
#include "stdlib.h"
User_USART BT_Data;		//��������

//��ʼ������
void User_USART_Init(User_USART *Data)
{
    for(uint8_t i=0; i < RXBUFFER_LEN; i++)	Data->RxBuffer[i] = 0;
    Data->frame_head = 0xA5;
    Data->frame_tail = 0x5A;
    Data->Rx_flag = 0;
    Data->Rx_len = 0;
    Data->x = 0;
    Data->y = 0;
    Data->mode = 0;
    Data->rxflag = 0;
}

void BTData_Process(uint8_t *RxBuffer)
{
    //���֡ͷ֡β
    if(RxBuffer[0] != BT_Data.frame_head) return;
    if(RxBuffer[6] != BT_Data.frame_tail) return;

//		BT_Data.x = (RxBuffer[3]<<8)|RxBuffer[2];
//		BT_Data.y = (RxBuffer[5]<<8)|RxBuffer[4];
//		BT_Data.w = (RxBuffer[7]<<8)|RxBuffer[6];
//		BT_Data.h = (RxBuffer[9]<<8)|RxBuffer[8];
    BT_Data.x = RxBuffer[1];
    BT_Data.y = RxBuffer[2];
    BT_Data.mode = RxBuffer[3];
    BT_Data.rxflag = RxBuffer[4];

    if (BT_Data.x>127) BT_Data.x-=255;
    if (abs(BT_Data.x)<10) BT_Data.x = 0;//����ת�򶶶�
    if (BT_Data.y>127) BT_Data.y-=255;
    if (abs(BT_Data.y)<10) BT_Data.y = 0;//����ǰ������
}

