/*
***********************************************************************
* 							YanJunFly V1.0 - Copyright (c) 2017
* All rights reserved.More information please browse www.yanjun.tech
*
*其他声明：                  对代码中产生的bug，本人不承担任何直接或间接的损害赔偿
*                           若使用本代码即视为默认本条款。若发现bug,请将详细信息
*                           发送至下面邮箱谢谢！
*                           1027259561@qq.com
*
* 文件名称：bsp_encoder_Tim3.h bsp_encoder_Tim3.c
* 文件摘要：
*						这个是STM32定时器编码器模式的应用，应用了TIM3，读取编码器的值
*						
* 当前版本：v1.0
* 当前作者：YJ_李红禧
* 完成日期：2017-7-19 14:51:53
* 改动说明：无
*
* 取代版本：无
* 原 作 者：YJ_李红禧
* 完成日期：2017-7-19 14:51:56
***********************************************************************
*/
#include "bsp_encoder_Tim3.h"

/*
***************************************************
*函数名称：vENCODER_TIM3_NVIC_Config(void)
*
*函数功能：配置定时器的中断优先级  0-0-3
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
***************************************************
*/
static void vENCODER_TIM3_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = ENCODER_TIM3_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************
*函数名称：vENCODER_TIM3_GPIO_Config(void)
*
*函数功能：配置GPIO引脚
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
***************************************************
*/
static void vENCODER_TIM3_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(ENCODER_TIM3_CH1_GPIO_CLK|ENCODER_TIM3_CH2_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = ENCODER_TIM3_CHI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER_TIM3_CH1_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = ENCODER_TIM3_CH2_PIN;
    GPIO_Init(ENCODER_TIM3_CH2_PORT, &GPIO_InitStructure);
}

/*
***************************************************
*函数名称：vENCODER_TIM3_Mode_Config(void)
*
*函数功能：配置定时器的模式
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
***************************************************
*/
static void vENCODER_TIM3_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure; 
    
    RCC_APB1PeriphClockCmd(ENCODER_TIM3_CLK, ENABLE);
    
    /* 无预分频，由编码器的脉冲计数 */
    TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM3_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = ENCODER_TIM3_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(ENCODER_TIM3, &TIM_TimeBaseStructure);
    
    /* 开启编码器模式3，A B相同时计数， 上升下降都计数，因此记得的数为四倍 */
    TIM_EncoderInterfaceConfig(ENCODER_TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);
    
    TIM_ICStructInit(&TIM_ICInitStructure);
//    TIM_ICInitStructure.TIM_Channel = ENCODER_TIM_CHANNEL_1;
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(ENCODER_TIM3, &TIM_ICInitStructure);
    
    TIM_ClearFlag(ENCODER_TIM3, TIM_FLAG_Update);
    TIM_ITConfig(ENCODER_TIM3, TIM_IT_Update, ENABLE);
    TIM3->CNT = 0;
    TIM_Cmd(ENCODER_TIM3, ENABLE);
}

/*
***************************************************
*函数名称：vENCODER_TIM3_Init(void)
*
*函数功能：全体初始化
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
***************************************************
*/
void vENCODER_TIM3_Init(void)
{
    vENCODER_TIM3_NVIC_Config();
    vENCODER_TIM3_GPIO_Config();
    vENCODER_TIM3_Mode_Config();
}

/*
***************************************************
*函数名称：TIM3_IRQHandler(void)
*
*函数功能：定时器3的中断函数，清除标志位
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
***************************************************
*/
void TIM3_IRQHandler(void)
{
    /* 检测中断标志位 */
    if(TIM_GetFlagStatus(ENCODER_TIM3,TIM_FLAG_Update) != RESET)
    {
        /* 清除中断标志位 */
        TIM_ClearFlag(ENCODER_TIM3, TIM_FLAG_Update );
    }
}

