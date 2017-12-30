/********************     (C) COPYRIGHT 2017      **************************
 * 文件名  ：Tim4.c
 * 描述    ：定时器timer4 测试例程       
 * 实验平台：STM32F103VET6
 * 库版本  ：ST3.5.0
 *
 * 编写日期：2017-04-10
 * 修改日期：2017-04-10
 * 作者    :
****************************************************************************/
#include "Tim4.h"

/*
 * 函数名：TIM4_NVIC_Configuration
 * 描述  ：TIM4中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure_right; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  													
    NVIC_InitStructure_right.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure_right.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure_right.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure_right.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure_right);
}

/*TIM_Period--200   TIM_Prescaler--(72M/20000) -->中断周期为
  2000*(1/72M)*(72M/2000)=1S    1S 定时器 */
void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure_right;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure_right.TIM_Period=2000;		 /* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure_right.TIM_Prescaler= (uint16_t)(SystemCoreClock/2000) - 1;	/* 时钟预分频数 72M/20000 */
    TIM_TimeBaseStructure_right.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure_right.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure_right);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);							/* 清除溢出中断标志 */
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM4, ENABLE);														/* 开启时钟 */    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/* 先关闭等待使用 */    
}

/*
 * 函数名：TIM4_Config
 * 描述  ：TIM4配置
 * 输入  ：无
 * 输出  ：无	
 */
void TIM4_Config(void)
{
  TIM4_Configuration();
	TIM4_NVIC_Configuration();
}


/*******************      (C) COPYRIGHT 2017       *END OF FILE************/
