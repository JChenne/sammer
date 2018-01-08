#ifndef _bsp_GeneralTime_h_
#define _bsp_GeneralTime_h_

#include "stm32f10x.h"

#define GENERAL_TIM                 TIM5
#define GENERAL_TIM_APBxClock_FUN   RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK             RCC_APB1Periph_TIM5
#define GENERAL_TIM_Period          0XFFFF
#define GENERAL_TIM_Prescaler       (72 - 1)

#define GENERAL_TIM_CH1_GPIO_CLK    RCC_APB2Periph_GPIOA
#define GENERAL_TIM_CH1_PORT        GPIOA
#define GENERAL_TIM_CHI_PIN         GPIO_Pin_0
#define GENERAL_TIM_CHANNEL_x       TIM_Channel_1

#define GENERAL_TIM_IT_CCx          TIM_IT_CC1
#define GENERAL_TIM_IRQ             TIM5_IRQn
#define GENERAL_TIM_INT_FUN         TIM5_IRQHandler

#define GENERAL_TIM_GetCapturex_FUN     TIM_GetCapture1

#define GENERAL_TIM_OCxPolarityConfig_FUN   TIM_OC1PolarityConfig
/* 测量的起始边缘 */
#define GENERAL_TIM_STRAT_ICPolarity    TIM_ICPolarity_Rising
/* 测量的结束边缘 */
#define GENERAL_TIM_END_ICPolarity      TIM_ICPolarity_Falling

typedef struct
{
    uint8_t Capture_FinishFlag;
    uint8_t Capture_StartFlag;
    uint8_t Capture_CcrValue;
    uint8_t Capture_Period;
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure;
void vGENERAL_TIM_Init(void);
void GENERAL_TIM_INT_FUN(void);

#endif
