/*
***********************************************************************
* Copyright (c) 2017 �࿥lhx
* All rights reserved.More information please browse www.yanjun.tech
*
* �����������Դ����в�����bug�����˲��е��κ�ֱ�ӻ��ӵ����⳥
*                           ��ʹ�ñ����뼴��ΪĬ�ϱ����������bug,�뽫��ϸ��Ϣ
*                           ��������������лл��
*                           1027259561@qq.com
*
* �ļ����ƣ�bsp_timbase.c bsp_timbase.h
* �ļ�ժҪ����¼ʱ��(���뼶)��time_1��ֵΪ����¼��ʱ��
*						
* ��ǰ�汾��v1.0
* ��ǰ���ߣ������
* ������ڣ�2017-7-16 22:26:17
* �Ķ�˵������
*
* ȡ���汾��v1.0
* ԭ �� �ߣ������
* ������ڣ�2017-7-16 22:26:17
***********************************************************************
*/

#include "bsp_timbase.h"

uint8_t time_1;

void vBASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /*             �����ж���Ϊ3               */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    /*              �����ж�Դ                 */
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ;
    /*            ���������ȼ�Ϊ7               */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    /*            ������ռ���ȼ�0                */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
}

void vBASIC_TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    /*  ������ʱ��ʱ�ӣ����ڲ�ʱ��CK_INT = 72M  */
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
    
    /*      �Զ���װ�Ĵ����ܵ�ֵ����������      */
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_MAX;
    /*�ۼ�TIM_Period��Ƶ�ʺ����һ�����»����ж�*/
    /* ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71 + 1) = 1M */
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
//    /*ʱ�ӷ�Ƶ����*/
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIVI;
//  /*����������ģʽ*/  
//    TIM_TimeBaseStructure.CounterMode = TIM_CounterMode_Up;
//  /*�ظ���������ֵ*/
//    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    
    /*            ��ʼ����ʱ��                */
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
    /*          ����������жϱ�־λ          */
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
    /*           �����������ж�               */
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);
    /*            ʹ�ܼ�����                  */
    TIM_Cmd(BASIC_TIM, ENABLE);
    /*      ��ʱ�رն�ʱ����ʱ�ӣ��ȴ�ʹ��     */
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
        /* Ӧ����GPS��ֵ */
        Check_side_speed(1, 2);
    }
    
}


void vBASIC_TIM_STAR(void)
{
    /* �򿪼�ʱ�� */
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
}

void vBASIC_TIM_CLOSE(void)
{
    /* �رռ�ʱ�� */
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, DISABLE);
}
