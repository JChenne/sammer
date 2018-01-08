#include "bsp_GeneralTime.h"

TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};

static void GENERAL_TIM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CHI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);
}

static void GENERAL_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure; 
    
    /*开启定时器时钟，即内部时钟CK_INT = 72M */
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
    
    
    /* 时基结构体初始化 */
    TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
    
    /* 输入捕获结构体初始化 */
    TIM_ICInitStructure.TIM_Channel = GENERAL_TIM_CHANNEL_x;
    TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    
    TIM_ICInit(GENERAL_TIM, &TIM_ICInitStructure);
    
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update | GENERAL_TIM_IT_CCx);
    TIM_ITConfig(GENERAL_TIM, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE);
    TIM_Cmd(GENERAL_TIM, ENABLE);
}

static void GENENRAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void vGENERAL_TIM_Init(void)
{
    GENERAL_TIM_GPIO_Config();
    GENERAL_TIM_Mode_Config();
    GENENRAL_TIM_NVIC_Config();
}

