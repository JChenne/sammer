/********************     (C) COPYRIGHT 2015     ***************************
 * 文件名 	：usart1.c
 * 描述   	：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *        	  打印到PC上的超级终端或串口调试助手。         
 * 实验平台	：STM32F10x
 * 硬件连接	：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  	：ST3.5.0
 * 编写日期	：2017-04-11
 * 修改日期	：
 * 作者    	：
*****************************************************************************/
#include "usart1.h"


/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置
 * 输入  ：uint32_t uBaud
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART1_Config(uint32_t uBaud)
{
	USART1_Configuration(uBaud);
	USART1_NVIC_Configuration();
}


/*
 * 函数名：USART1_Configuration
 * 描述  ：USART1 GPIO 配置,工作模式配置。uBaud 8-N-1
 * 输入  ：uint32_t uBaud
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART1_Configuration(uint32_t uBaud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = uBaud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}


void USART1_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	//while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}


/******************* (C) COPYRIGHT 2017 *****END OF FILE************/

