#ifndef __bsp_encoder_Tim3_h
#define __bsp_encoder_Tim3_h

#include "stm32f10x.h"

#define ENCODER_TIM3                 TIM3
#define ENCODER_TIM3_CLK             RCC_APB1Periph_TIM3
#define ENCODER_TIM3_Period          330 * 4     //330线（这个330是我根据当前有的编码器设置的，不同编码器应该值不一样）
#define ENCODER_TIM3_Prescaler       0           //不需要分频，编码器模式由脉冲决定

#define ENCODER_TIM3_CH1_GPIO_CLK    RCC_APB2Periph_GPIOA
#define ENCODER_TIM3_CH1_PORT        GPIOA
#define ENCODER_TIM3_CHI_PIN         GPIO_Pin_6

#define ENCODER_TIM3_CH2_GPIO_CLK    RCC_APB2Periph_GPIOA
#define ENCODER_TIM3_CH2_PORT        GPIOA
#define ENCODER_TIM3_CH2_PIN         GPIO_Pin_7

#define ENCODER_TIM3_IRQ             TIM3_IRQn

static void vENCODER_TIM3_NVIC_Config(void);
static void vENCODER_TIM3_GPIO_Config(void);
static void vENCODER_TIM3_Mode_Config(void);
void vENCODER_TIM3_Init(void);

#endif
