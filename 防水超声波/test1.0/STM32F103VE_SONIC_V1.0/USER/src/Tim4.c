/********************     (C) COPYRIGHT 2017      **************************
 * �ļ���  ��Tim4.c
 * ����    ����ʱ��timer4 ��������       
 * ʵ��ƽ̨��STM32F103VET6
 * ��汾  ��ST3.5.0
 *
 * ��д���ڣ�2017-04-10
 * �޸����ڣ�2017-04-10
 * ����    :
****************************************************************************/
#include "Tim4.h"

/*
 * ��������TIM4_NVIC_Configuration
 * ����  ��TIM4�ж����ȼ�����
 * ����  ����
 * ���  ����	
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

/*TIM_Period--200   TIM_Prescaler--(72M/20000) -->�ж�����Ϊ
  2000*(1/72M)*(72M/2000)=1S    1S ��ʱ�� */
void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure_right;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure_right.TIM_Period=2000;		 /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure_right.TIM_Prescaler= (uint16_t)(SystemCoreClock/2000) - 1;	/* ʱ��Ԥ��Ƶ�� 72M/20000 */
    TIM_TimeBaseStructure_right.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure_right.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure_right);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);							/* �������жϱ�־ */
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM4, ENABLE);														/* ����ʱ�� */    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/* �ȹرյȴ�ʹ�� */    
}

/*
 * ��������TIM4_Config
 * ����  ��TIM4����
 * ����  ����
 * ���  ����	
 */
void TIM4_Config(void)
{
  TIM4_Configuration();
	TIM4_NVIC_Configuration();
}


/*******************      (C) COPYRIGHT 2017       *END OF FILE************/
