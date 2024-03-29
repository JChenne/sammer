/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h"
#include "led.h"
#include "usart1.h"
#include "SysTick.h"
#include "Sonic.h"


extern u32 __IO time_1ms;
u32 __IO time_120ms = 0;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}





/**
  * @brief  This function handles TIM2 Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{ 
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		#if 0
		GPIO_WriteBit(GPIO_LED1_PORT,GPIO_Pin_LED1,
			(BitAction)(1-GPIO_ReadOutputDataBit(GPIO_LED1_PORT,GPIO_Pin_LED1)));
		#endif
		#if 1
		GPIO_WriteBit(GPIO_LED2_PORT,GPIO_Pin_LED2,
			(BitAction)(1-GPIO_ReadOutputDataBit(GPIO_LED2_PORT,GPIO_Pin_LED2)));
		#endif
		#if 0
		GPIO_WriteBit(GPIO_LED3_PORT,GPIO_Pin_LED3,
			(BitAction)(1-GPIO_ReadOutputDataBit(GPIO_LED3_PORT,GPIO_Pin_LED3)));
		#endif
		#if 0
		GPIO_WriteBit(GPIO_LED3_PORT,GPIO_Pin_LED3,
			(BitAction)(1-GPIO_ReadOutputDataBit(GPIO_LED3_PORT,GPIO_Pin_LED3)));
		#endif
				#if 0
		GPIO_WriteBit(GPIO_LED3_PORT,GPIO_Pin_LED3,
			(BitAction)(1-GPIO_ReadOutputDataBit(GPIO_LED3_PORT,GPIO_Pin_LED3)));
		#endif
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
	}
}


/**
  * @brief  This function handles USART1 Handler.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	#if 1
	uint8_t c;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))           /* 若开了接收中断，需要清中断标志位 */
	{
	  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		c=(uint8_t)USART_ReceiveData(USART1);	
		printf("%c",c);
	}
	#endif
}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	time_1ms++;
	time_120ms++;
	if(time_120ms>=80)
	{
		Sonic_Trig();				//50ms Trig Sonic
		time_120ms=0;
	}
	TimingDelay_Decrement();
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
