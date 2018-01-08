/*
***********************************************************************
* Copyright (c) 2017 燕骏lhx
* All rights reserved.More information please browse www.yanjun.tech
*
* 其他声明：对代码中产生的bug，本人不承担任何直接或间接的损害赔偿
*                           若使用本代码即视为默认本条款。若发现bug,请将详细信息
*                           发送至下面邮箱谢谢！
*                           1027259561@qq.com
*
* 文件名称：bsp_timbase.c bsp_timbase.h
* 文件摘要：记录时间(毫秒级)，time_1的值为所记录的时间
*						
* 当前版本：v1.0
* 当前作者：李红禧
* 完成日期：2017-7-16 22:26:17
* 改动说明：无
*
* 取代版本：v1.0
* 原 作 者：李红禧
* 完成日期：2017-7-16 22:26:17
***********************************************************************
*/

#include "bsp_timbase.h"

uint8_t time_1;

void vBASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /*             设置中断组为3               */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    /*              设置中断源                 */
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ;
    /*            设置主优先级为7               */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    /*            设置抢占优先级0                */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
}

void vBASIC_TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    /*  开启定时器时钟，即内部时钟CK_INT = 72M  */
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
    
    /*      自动重装寄存器周的值（计数器）      */
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_MAX;
    /*累计TIM_Period个频率后产生一个更新或者中断*/
    /* 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71 + 1) = 1M */
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
//    /*时钟分频因子*/
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIVI;
//  /*计数器计数模式*/  
//    TIM_TimeBaseStructure.CounterMode = TIM_CounterMode_Up;
//  /*重复计数器的值*/
//    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    
    /*            初始化定时器                */
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
    /*          清除计数器中断标志位          */
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
    /*           开启计数器中断               */
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);
    /*            使能计数器                  */
    TIM_Cmd(BASIC_TIM, ENABLE);
    /*      暂时关闭定时器的时钟，等待使用     */
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, DISABLE);
    
    vBASIC_TIM_NVIC_Config();
}

void vBASIC_TIM_IRQHandler(void)
{
    if(TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET)
    {
        time_1++;
        TIM_ClearITPendingBit(BASIC_TIM, TIM_FLAG_Update);
    }
    if(time_1 >= 20)
    {
        /* 应放入GPS的值 */
        Check_side_speed(1, 2);
    }
    
}


void vBASIC_TIM_STAR(void)
{
    /* 打开计时器 */
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
}

void vBASIC_TIM_CLOSE(void)
{
    /* 关闭计时器 */
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, DISABLE);
}
