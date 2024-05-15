#include "sys_time.h"

uint32_t Get_sys_time_ms(void)
{
    return HAL_GetTick();
}

uint32_t Get_sys_time_us(void)
{
    uint32_t sys_us = 1000*HAL_GetTick()+TIM1->CNT;
    return sys_us;
}
uint32_t Get_sys_time_only_us(void)
{
    uint32_t sys_only_us = TIM1->CNT;
    return sys_only_us;
}
