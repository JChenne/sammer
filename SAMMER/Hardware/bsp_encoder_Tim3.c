/*
***********************************************************************
* 							YanJunFly V1.0 - Copyright (c) 2017
* All rights reserved.More information please browse www.yanjun.tech
*
*����������                  �Դ����в�����bug�����˲��е��κ�ֱ�ӻ��ӵ����⳥
*                           ��ʹ�ñ����뼴��ΪĬ�ϱ����������bug,�뽫��ϸ��Ϣ
*                           ��������������лл��
*                           1027259561@qq.com
*
* �ļ����ƣ�bsp_encoder_Tim3.h bsp_encoder_Tim3.c
* �ļ�ժҪ��
*						�����STM32��ʱ��������ģʽ��Ӧ�ã�Ӧ����TIM3����ȡ��������ֵ
*						
* ��ǰ�汾��v1.0
* ��ǰ���ߣ�YJ_�����
* ������ڣ�2017-7-19 14:51:53
* �Ķ�˵������
*
* ȡ���汾����
* ԭ �� �ߣ�YJ_�����
* ������ڣ�2017-7-19 14:51:56
***********************************************************************
*/
#include "bsp_encoder_Tim3.h"

/*
***************************************************
*�������ƣ�vENCODER_TIM3_NVIC_Config(void)
*
*�������ܣ����ö�ʱ�����ж����ȼ�  0-0-3
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
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
*�������ƣ�vENCODER_TIM3_GPIO_Config(void)
*
*�������ܣ�����GPIO����
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
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
*�������ƣ�vENCODER_TIM3_Mode_Config(void)
*
*�������ܣ����ö�ʱ����ģʽ
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
***************************************************
*/
static void vENCODER_TIM3_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure; 
    
    RCC_APB1PeriphClockCmd(ENCODER_TIM3_CLK, ENABLE);
    
    /* ��Ԥ��Ƶ���ɱ�������������� */
    TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM3_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = ENCODER_TIM3_Prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(ENCODER_TIM3, &TIM_TimeBaseStructure);
    
    /* ����������ģʽ3��A B��ͬʱ������ �����½�����������˼ǵõ���Ϊ�ı� */
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
*�������ƣ�vENCODER_TIM3_Init(void)
*
*�������ܣ�ȫ���ʼ��
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
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
*�������ƣ�TIM3_IRQHandler(void)
*
*�������ܣ���ʱ��3���жϺ����������־λ
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
***************************************************
*/
void TIM3_IRQHandler(void)
{
    /* ����жϱ�־λ */
    if(TIM_GetFlagStatus(ENCODER_TIM3,TIM_FLAG_Update) != RESET)
    {
        /* ����жϱ�־λ */
        TIM_ClearFlag(ENCODER_TIM3, TIM_FLAG_Update );
    }
}

