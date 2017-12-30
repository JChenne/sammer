/********************     (C) COPYRIGHT 2017      **************************
 * 文件名  ：Tim2.c
 * 描述    ：定时器timer2 测试例程       
 * 实验平台：STM32F103VET6
 * 库版本  ：ST3.5.0
 *
 * 编写日期：2017-04-10
 * 修改日期：2017-04-10
 * 作者    :
****************************************************************************/
#include "Tim2.h"

/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*TIM_Period--200   TIM_Prescaler--(72M/20000) -->中断周期为
  2000*(1/72M)*(72M/2000)=1S    1S 定时器 */
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=2000;		 /* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t)(SystemCoreClock/2000) - 1;	/* 时钟预分频数 72M/20000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							/* 清除溢出中断标志 */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);														/* 开启时钟 */    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/* 先关闭等待使用 */    
}

/*
 * 函数名：TIM2_Config
 * 描述  ：TIM2配置
 * 输入  ：无
 * 输出  ：无	
 */
void TIM2_Config(void)
{
  TIM2_Configuration();
	TIM2_NVIC_Configuration();
}


/*******************      (C) COPYRIGHT 2017       *END OF FILE************/
