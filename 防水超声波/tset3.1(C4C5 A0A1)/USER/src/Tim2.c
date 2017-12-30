/********************     (C) COPYRIGHT 2017      **************************
 * �ļ���  ��Tim2.c
 * ����    ����ʱ��timer2 ��������       
 * ʵ��ƽ̨��STM32F103VET6
 * ��汾  ��ST3.5.0
 *
 * ��д���ڣ�2017-04-10
 * �޸����ڣ�2017-04-10
 * ����    :
****************************************************************************/
#include "Tim2.h"

/*
 * ��������TIM2_NVIC_Configuration
 * ����  ��TIM2�ж����ȼ�����
 * ����  ����
 * ���  ����	
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

/*TIM_Period--200   TIM_Prescaler--(72M/20000) -->�ж�����Ϊ
  2000*(1/72M)*(72M/2000)=1S    1S ��ʱ�� */
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=2000;		 /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t)(SystemCoreClock/2000) - 1;	/* ʱ��Ԥ��Ƶ�� 72M/20000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							/* �������жϱ�־ */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);														/* ����ʱ�� */    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/* �ȹرյȴ�ʹ�� */    
}

/*
 * ��������TIM2_Config
 * ����  ��TIM2����
 * ����  ����
 * ���  ����	
 */
void TIM2_Config(void)
{
  TIM2_Configuration();
	TIM2_NVIC_Configuration();
}


/*******************      (C) COPYRIGHT 2017       *END OF FILE************/
