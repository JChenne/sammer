#ifndef _bsp_PWM_Tim1_h_
#define _bsp_PWM_Tim1_h_

#include "stm32f10x.h"

#define PWM_TIM1                     TIM1
#define PWM_TIM1_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define PWM_TIM1_CLK                 RCC_APB2Periph_TIM1

/* 配置中断 */
#define PWM_TIM1_IRQ                 TIM1_UP_IRQn
#define PWM_TIM1_IRQHandler          TIM1_UP_IRQHandler

/* TIM1 输出比较通道1 */
#define PWM_TIM1_CH1_GPIO_CLK       RCC_APB2Periph_GPIOA
#define PWM_TIM1_CH1_PORT           GPIOA
#define PWM_TIM1_CH1_PIN            GPIO_Pin_8

/* TIM1 输出比较通道2 */
#define PWM_TIM1_CH2_GPIO_CLK       RCC_APB2Periph_GPIOA
#define PWM_TIM1_CH2_PORT           GPIOA
#define PWM_TIM1_CH2_PIN            GPIO_Pin_9


#define MOTO_START_PWM_PAUSE        ((int32_t)1000)
#define MOTO_MAX_PWM_PAUSE          ((int32_t)999)

static void vADVANCE_TIM_GPIO_Config(void);
static void vADVANCE_TIM_Mode_Config(uint32_t ADVANCE_TIM_PERIOD, uint32_t ADVANCE_TIM_PSC, uint32_t ADVANCE_TIM_PULSE);
void vADVANCE_TIM_Init(uint32_t ADVANCE_TIM_PERIOD, uint32_t ADVANCE_TIM_PSC, uint32_t ADVANCE_TIM_PULSE);
void vTim1_MotoPwmOut(int32_t channel1, int32_t channel2);

#endif
