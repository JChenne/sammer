#ifndef __JSN_SR04T_H__
#define __JSN_SR04T_H__

#include "stm32f10x.h"

void Hcsr04Init(void);
void hcsr04_NVIC(void);
void TIM6_IRQHandler(void);
u32 GetEchoTimer(void);
float Hcsr04GetLength(void);
void Delay_Ms(uint16_t time);
void Delay_Us(uint16_t time);

#endif	
