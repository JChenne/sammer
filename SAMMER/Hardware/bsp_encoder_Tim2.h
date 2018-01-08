#ifndef __bsp_encoder_Tim2_h
#define __bsp_encoder_Tim2_h

#include "stm32f10x.h"

#define ENCODER_TIM2                 TIM2
#define ENCODER_TIM2_CLK             RCC_APB1Periph_TIM2
#define ENCODER_TIM2_Period          330 * 4     //330线（这个330是我根据当前有的编码器设置的，不同编码器应该值不一样）
#define ENCODER_TIM2_Prescaler       0           //不需要分频，编码器模式由脉冲决定

#define ENCODER_TIM2_CH1_GPIO_CLK    RCC_APB2Periph_GPIOA
#define ENCODER_TIM2_CH1_PORT        GPIOA
#define ENCODER_TIM2_CHI_PIN         GPIO_Pin_0

#define ENCODER_TIM2_CH2_GPIO_CLK    RCC_APB2Periph_GPIOA
#define ENCODER_TIM2_CH2_PORT        GPIOA
#define ENCODER_TIM2_CH2_PIN         GPIO_Pin_1

#define ENCODER_TIM2_IRQ             TIM2_IRQn

static void vENCODER_TIM2_NVIC_Config(void);
static void vENCODER_TIM2_GPIO_Config(void);
static void vENCODER_TIM2_Mode_Config(void);
void vENCODER_TIM2_Init(void);

#endif
