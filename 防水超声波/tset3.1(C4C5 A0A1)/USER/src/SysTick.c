/**
  ******************************************************************************
  * @file    SysTick.c
  * @author  
  * @version 
  * @date    
  * @brief   SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
  *          ���õ��� 1us 10us 1ms �жϡ�     
  */
  
	
#include "SysTick.h"

static __IO u32 TimingDelay = 0;

/**
  * @brief  ����ϵͳ�δ�ʱ�� SysTick
  * @param  ��
  * @retval ��
  */
void SysTick_Init( void )
{
	if ( SysTick_Config(SystemCoreClock / 1000) )	// ST3.5.0��汾
	{ 
		while (1);
	}
		// �رյδ�ʱ��  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/**
  * @brief   ms��ʱ����,1msΪһ����λ
  * @param  
  *		@arg nTime: Delay_ms( 1 ) ��ʵ�ֵ���ʱΪ 1 * 1ms = 1ms
  * @retval  ��
  */
void Delay_ms( __IO u32 nTime )
{ 
	TimingDelay = nTime;	
	// ʹ�ܵδ�ʱ��  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	while( TimingDelay != 0 );
}

/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
	if ( TimingDelay != 0x00 )
	{ 
		TimingDelay --;
	}
}

/*********************************************END OF FILE**********************/
