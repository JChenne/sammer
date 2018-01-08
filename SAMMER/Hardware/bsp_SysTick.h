#ifndef _bsp_SysTick_h_
#define _bsp_SysTick_h_

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
void TimingDelay_Decrement(void);
void vSysTick_Delay_us(__IO u32 ulUs);
void vSysTick_Delay_ms(__IO u32 ulMs);
#endif
