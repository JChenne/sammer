/*
***********************************************************************
* 			YanJunFly V1.0 - Copyright (c) 2017
* All rights reserved.More information please browse www.yanjun.tech
* 燕骏智控-最给力的开源飞控社区，这里有最优秀的学习教程。YJ飞行器社区 -- 永不止步！
* 
* 其他声明：
*						对代码中产生的bug，本人不承担任何直接或间接的损害赔偿
*						若使用本代码即视为默认本条款。若发现bug，请将详细信息
*						发送下面链接至评论区，谢谢！
*						http://www.yanjun.tech/forum.php?gid=87
*
* 文件名称：usart1_dma.c,usart1_dma.h
* 文件摘要：
*						这个文件是usart1的底层驱动文件，使用DMA进行读取和发送数据
*						重要资料参考 《STM32中文参考手册》DMA章节 usart章节 以及
*           《3.5固件库参考手册》DMA章节  usart章节
*						本文件中函数，可以实现串口DMA发送和接收任意长度数据。
*						串口DMA发送完成 触发DMA发送完成中断，DMA采用单次发送模式
*						每次发送数据，均要设置长度和起始地址
*						串口数据接收完成 实际触发的是串口空闲中断。使用这个中断，
*						才能完成接收任意数据功能，DMA接收中断只能接收固定长度数据
*
*						使用引脚：PA9 - TX  PA10 - RX
*
*	注    意：
*						这个文件实现的DMA发送串口数据和DMA接收串口数据。
*						查看《stm32中文参考手册》DMA 章节第143页DMA框图以及第148页DMA1请求表
*						发现USART1在DMA1上，其中USART1_TX（发送）在DMA1的通道4上
*						USART1_RX(接收)在DMA1的通道5上。DMA1有7路通道，DMA2有5路通道。所以说当有
*						多个外设使用同一个DMA通道的时候，就需要进行手动的 使能 和 失能DMA通道
*						实现分时复用的功能
*						
* 当前版本：v1.0
* 当前作者：YJ_朱子文
* 完成日期：2017-3-16 17:42:59
* 改动说明：创建文件
*
* 取代版本：v1.0
* 原 作 者：zzw
* 完成日期：2017-3-16 17:53:04
***********************************************************************
*/

#include "usart1_dma.h"

/* 标志位先设置为1 使其自启动 */
uint32_t ulUsart1TxOkFlag = 1;
uint32_t ulUsart1RxOkFlag = 1;
/* 缓存DMA接收到的真正的数据的长度 */
uint32_t ulUsart1RecvRealSize = 0;
/* 存储上次发送数据的长度 */
uint32_t ulUsart1Dma1Channe5Increase = 0;
/* 存储上次发送的位置 */
uint32_t ulUsart1SendPreIndex = 0;
/* vu8 点击代码 看底层 也就是volatile 类型 代表这个变量是易变的，不稳定的，
 * 需要每次读取到内存进行读取 否则编译器可能进行优化 可能导致错误 
 * 这个变量用于缓存 接收到的数据的长度
 */

vu8 vucUSART1_TxBuffer[USART1_TX_SIZE];
vu8 vucUSART1_RxBuffer[USART1_RX_SIZE];

/*
**********************************************************************
*函数名称：void vUSART1_TxDmaNvicConfig(void)
*
*函数功能：DMA1_NVIC发送通道配置函数 配置优先级 分组在主函数中设置 
*					全部配置分组为0	NVIC_Configuration(NVIC_PriorityGroup_0);
*					NVIC	详细的向量分组，见system->system.h文件中注释
*
*使用说明：在下面的初始化函数中调用
*入口参数：无
*返 回 值：无
*
*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:52:42
*函数版本：v1.0
*修改原因：创建文件
*
*取代版本：无
*原 作 者：zzw
*完成日期：2017-3-16 17:52:54
**********************************************************************
*/

void vUSART1_TxDmaNvicConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* 查看《STM32中文参考手册》找到DMA1各个通道 对应的硬件，USART1_TX对应通道4 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;	
	/* 这里优先级使用sys.h中的宏定义 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMA1_Channel4PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = DMA1_Channel4SubPriority;	
	/* 该参数指定了在成员 NVIC_IRQChannel 中定义的 IRQ 通道被使能还是失能。*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

/*
**********************************************************************
*函数名称：void vUSART1_NvicConfig(void)
*
*函数功能：USART1_NVIC 配置函数 配置优先级 分组在主函数中设置 
*					全部配置分组为0	NVIC_Configuration(NVIC_PriorityGroup_0);
*					NVIC	详细的向量分组，见system->sys.h文件中注释
*					这里我们使用串口空闲中断代替串口接收DMA中断，可以实现接收任意长度数据
*
*使用说明：在下面的初始化函数中调用
*入口参数：无
*返 回 值：无

*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:53:46
*函数版本：v1.0
*修改原因：创建文件
*
*取代版本：无
*原 作 者：zzw
*完成日期：2017-3-16 17:54:20
**********************************************************************
*/

void vUSART1_NvicConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
    
    
	
	/* 通道设置为串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;    
	/* 工程使用时，替换成sys.h中的优先级宏定义 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART1_PreemptionPriority;         
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART1_SubPriority;  
	/* 使能串口中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	
	NVIC_Init(&NVIC_InitStructure);  
}

/*
**********************************************************************
*函数名称：void vUSART1_TxDmaConfig(void)
*
*函数功能：串口tx的DMA 配置函数 配置相关的DMA设置，详情 参考《STM32中文参考手册》
*					以及《STM32固件库手册》查看相关章节 相关参数
*
*使用说明：在下面的初始化函数中调用
*入口参数：无
*返 回 值：无

*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:46:03
*函数版本：v1.0
*修改原因：创建文件
*
*取代版本：无
*原 作 者：zzw
*完成日期：2017-3-16 17:54:38
**********************************************************************
*/

void vUSART1_TxDmaConfig(void)
{
	/* DMA参数的结构体 */
	DMA_InitTypeDef DMA_InitStructure;
	/* Enable DMA clock 如何看 -- 查看时钟树 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* 首先关闭DMA1_Channel4 然后进行配置 */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	/* USART1 对应DMA的 通道1 将 DMA 的通道 x 寄存器重设为默认值 
		这里调用一下初始化 比较好DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel4);
		
	/* 首先配置 串口发送的DMA */
	
	/* 指定的外设地址 设置 成USART1的地址 */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1 -> DR);				
	/* 采集的数据的目的地内存地址 暂时没设置，到下面发送的时候，我再设置的 */
	//DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_TX_Buffer;	
	/* DMA数据的方向为内存到外设 */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;		
	/* 这里首先设置为0 需要发送的时候 再进行调整 */
	DMA_InitStructure.DMA_BufferSize = 0;
	/* 外设本身的地址固定不变 */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	/* 内存地址递增，也就是缓存数据的数据地址增加 */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	/* 外设数据宽度设置为8位 */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	
	/* 内存的数据宽度为8位 也就是1字节 */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					
		
	/*注意：友情提醒当指定 DMA 通道数据传输配置为内存到内存时，不能使用循环缓存模式。*/
	
	/* 传输模式设置成 单次传输模式 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	
	/* DMA通道优先级设置为高优先级 当有多个DMA运行时 仲裁 */
	DMA_InitStructure.DMA_Priority = DMA1_USART1_TX_CHANNEL4_PRIORITY;
	/* 禁止内存到内存的模式 */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	/* 使能DMA1_Channel1完成传输后的中断 */
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);        											
	/* DISABLE DMA channel4 首先关闭这个 DMA待用 */
	DMA_Cmd(DMA1_Channel4, DISABLE);	
}

/*
**********************************************************************
*函数名称：void vUSART1_RxDmaConfig(void)
*
*函数功能：串口rx的DMA 配置函数 配置相关的DMA设置，详情 参考《STM32中文参考手册》
*					以及《STM32固件库手册》查看相关章节 相关参数
*
*使用说明：在下面的初始化函数中调用
*入口参数：无
*返 回 值：无
*
*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:46:10
*函数版本：v1.0 
*修改原因：创建文件
*
*取代版本：无
*原 作 者：zzw
*完成日期：2017-3-16 17:54:48
**********************************************************************
*/

void vUSART1_RxDmaConfig(void)
{
	//DMA参数的结构体
	DMA_InitTypeDef DMA_InitStructure;
	/* Enable DMA clock 如何看 -- 查看时钟树 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*首先关闭这个DMA的通道5*/
	DMA_Cmd(DMA1_Channel5, DISABLE);
	/*usart1对应DMA的 通道1 将 DMA 的通道 x 寄存器重设为默认值 其实不用调用 DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel5);
	
	//首先配置 串口接收的DMA
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1 -> DR);	
	/* 采集的数据的目的地内存地址 */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)vucUSART1_RxBuffer;	
	/* DMA方向为外设到内存 */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;		
	/* 数据尺寸暂时设置为0，后面根据实际设置 */
	DMA_InitStructure.DMA_BufferSize = 0;		
	/* 外设本身的地址固定不变 */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	/* 内存地址递增 */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  		
	/* 外设的数据宽度为8位 因为这里设置的是 vu8 ADC_ConvertedValue[SAMPLE_NUM][CHANNEL_NUM];  （根本原因是 采集的数据是 8位的） */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		
	/* 内存的数据宽度为8位 也就是1字节 */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;					
		
	/* 注意：当指定 DMA 通道数据传输配置为内存到内存时，不能使用循环缓存模式。*/
	/* 单次传输模式 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;																
	DMA_InitStructure.DMA_Priority = DMA1_USART1_RX_CHANNEL5_PRIORITY;		
	/* 禁止内存到内存的模式 */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	/* 使能DMA1_Channel1完成传输后的中断 */
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, DISABLE);        											
	/* DISABLE DMA channel5*/
	DMA_Cmd(DMA1_Channel5, DISABLE);
}

/*
**********************************************************************
*函数名称：void vUSART1_RX_DMA_Config(void)
*
*函数功能：相关的GPIO口的配置函数，参考《STM32中文参考手册》第110页 外设GPIO配置
*
*使用说明：在下面的初始化函数中调用
*入口参数：无
*返 回 值：无
*
*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:46:15
*函数版本：v1.0 
*修改原因：创建文件
*
*取代版本：无
*原 作 者：无
*完成日期：无
**********************************************************************
*/

void vUSART1_GpioConfig(void)
{
	/* GPIO端口设置 外设GPIO口设置 参见 《STM32中文参考手册》 110页 */
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 使能USART1，GPIOA时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/* USART1_TX   PA.9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* 复用推挽输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                            
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
	/* USART1_RX PA.10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	/* 浮空输入 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

/*
**********************************************************************
*函数名称：void vUSART1_DMA1_Init(uint16_t bound)
*
*函数功能：进行相关的串口配置 参考《STM32固件库手册》USART以及《STM32中文参考手册》相关章节
*
*使用说明：在main函数中调用
*入口参数：bound -波特率
*返 回 值：无
*
*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:46:21
*函数版本：v1.0 
*修改原因：创建文件
*
*取代版本：无
*原 作 者：zzw
*完成日期：2017-3-16 17:57:03
**********************************************************************
*/

void vUSART1_Init(uint32_t ulBound)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1，GPIOA时钟

	vUSART1_NvicConfig();
	/* USART1的TX接收端DMA的NVIC的配置函数 */
	vUSART1_TxDmaNvicConfig();
	/* USART1的RX接收端DMA的配置函数 */
	vUSART1_RxDmaConfig();
	/* USART1的TX接收端DMA的配置函数 */
	vUSART1_TxDmaConfig();
	/* USART1的 GPIO 配置函数 （在上面写的） */
	vUSART1_GpioConfig();

	/* 波特率设置，一般为9600 */
	USART_InitStructure.USART_BaudRate = ulBound;                                
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                     
	USART_InitStructure.USART_Parity = USART_Parity_No;                         
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	            

	USART_Init(USART1, &USART_InitStructure);                        

	/**
	*中断配置
	*USART_IT_TXE 发送中断   
	*USART_IT_TC 传输完成中断  
	*USART_IT_RXNE 接收中断
	*USART_IT_IDLE 空闲中断
	*这里使能的是空闲中断
	**/
	USART_ITConfig(USART1,USART_IT_TC|USART_IT_RXNE|USART_IT_TXE,DISABLE);  
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);   
	/* Enable the DMA transfer on Rx and Tx action for USART1 */
	USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
	/* 使能串口 */
	USART_Cmd(USART1, ENABLE);                                                   
}

/*
***********************************************************************
*函数名称：ErrorStatus vUSART1_DmaTxStart(uint8_t * pucBuffer, uint32_t ulSize, uint32_t ulIsNeedCopyData)
*
*函数功能：通过串口1发送字符串
*
*使用说明：无
*入口参数：pucBuffer：发送字符串指针
					ulSize：发送字符串的长度
					ulIsNeedCopyData：是否需要把字符串复制到内部缓冲区中
*返 回 值：是否发生错误
*
*当前作者：石雄涛
*创建日期：2017-3-16 17:46:26
*函数版本：v1.0
*修改原因：创建文件
*
*取代版本：无
*原 作 者：石雄涛
*完成日期：2017-3-16 17:57:25
***********************************************************************
*/

ErrorStatus vUSART1_DmaTxStart(uint16_t * pucBuffer, uint32_t ulSize, uint32_t ulIsNeedCopyData)
{
	uint32_t ulIndex = 0;
	
	/* 判断是不是需要 copy 数据 */
	if(ulIsNeedCopyData)
	{
		/* 需要复制数据到缓冲区 */
		
		/* 先判断 DMA 发送是否完成，通过DMA 发送数据计数器 */
		if(DMA1_Channel4->CNDTR)
		{
			/* 上次的没有发送完成 */
			
			/* 检查这次的数据会不会超过缓冲区 */
			if((((uint32_t)vucUSART1_TxBuffer + USART1_TX_SIZE) - DMA1_Channel4->CMAR) > ulSize)
			{
				/* 不会超过缓冲区，开始复制 */
				
				/* 配置寄存器前，需要先关闭 */
				//DMA_Cmd(DMA1_Channel4, DISABLE);
				DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);
				
				/* 改变发送首地址 */
				/* 计算本次地址增量 */
				DMA1_Channel4->CMAR += ((ulUsart1SendPreIndex - DMA1_Channel4->CNDTR) - ulUsart1Dma1Channe5Increase);
				/* 记录本次地址增量 */
				ulUsart1Dma1Channe5Increase = (ulUsart1SendPreIndex - DMA1_Channel4->CNDTR);
				
				/* 记录数据长度，这里不是直接赋值，而是直接加上 */
				DMA1_Channel4->CNDTR += ulSize;
				
				/* 先计算，现在发送到了那里 */
				ulIndex = ulUsart1SendPreIndex;
				
				/* 同时记录本次发送的位置，给下次使用 */
				ulUsart1SendPreIndex += ulSize;
				
				/* 复制缓冲区数据到FIFO中 */
				while(ulSize--)
				{
					vucUSART1_TxBuffer[ulIndex++] = *pucBuffer++;
				}
		
				/* 使能DMA 开始发送 */
				//DMA_Cmd(DMA1_Channel4, ENABLE);
				DMA1_Channel4->CCR |= DMA_CCR1_EN;
				
				/* 返回操作成功 */
				return SUCCESS;     
			}else
			{
				/* 超过了，直接返回异常 */
				return ERROR;
			}
		}else
		{
			/* 上次的发送完成，直接开始新的发送 */
			
			/* 先进行安全判断，看会不会超过缓冲区 */
			if(ulSize < USART1_TX_SIZE)
			{
				/* 配置寄存器前，需要先关闭 */
				//DMA_Cmd(DMA1_Channel4, DISABLE);
				DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);
				
				/* 记录数据长度 */
				DMA1_Channel4->CNDTR = ulSize;
				
				/* 记录本次发送的在数组中的位置，给下次判断使用 */
				ulUsart1SendPreIndex = ulSize;
				
				/* 清零地址增量 */
				ulUsart1Dma1Channe5Increase = 0;   
				 
				/* 从0地址开始复制 */
				ulIndex = 0;
				
				/* 复制缓冲区数据到FIFO中 */
				while(ulSize--)
				{
					vucUSART1_TxBuffer[ulIndex++] = *pucBuffer++;
				}
				
				/* 改变 DMA 首地址 */
				DMA1_Channel4->CMAR = (uint32_t)vucUSART1_TxBuffer;
				
				/* 使能DMA 开始发送 */
				//DMA_Cmd(DMA1_Channel4, ENABLE);
				DMA1_Channel4->CCR |= DMA_CCR1_EN;
			
				/* 返回操作成功 */
				return SUCCESS;
			}else
			{
				/* 发送数据长度太长，返回错误 */
				return ERROR;
			}
		}
	}else
	{
		/* 先判断 DMA 发送是否完成，通过DMA 发送数据计数器 */
		if(DMA1_Channel4->CNDTR)
		{
			/* 上次的没有发送完成 */
			return ERROR; 
		}else
		{
			/* 不需要复制数据到缓冲区 */
			
			/* 配置寄存器前，需要先关闭 */
			//DMA_Cmd(DMA1_Channel4, DISABLE);
			DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);
			
			/* 注意这里不能调用库函数直接初始化了，那样会把其他设置给搞没了，直接配置寄存器即可 */
			DMA1_Channel4 -> CNDTR = ulSize;
			
			/* 这里判断是不是需要 */
			DMA1_Channel4 -> CMAR = (uint32_t)pucBuffer;
			
			/* 使能DMA 开始发送 */
			//DMA_Cmd(DMA1_Channel4, ENABLE);
			DMA1_Channel4->CCR |= DMA_CCR1_EN;
			
			/* 返回操作成功 */
			return SUCCESS;
		}
	}
}

/*
**********************************************************************
*函数名称：void vUSART1_DmaRxStart(void)
*
*函数功能：因为是单次传输模式，所以需要每次发送都使能接收DMA然后关闭 
*
*使用说明：在下面的初始化函数中调用
*入口参数：无
*返 回 值：无
*
*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:46:39
*函数版本：v1.0 
*修改原因：创建文件
*
*取代版本：无
*原 作 者：zzw
*完成日期：2017-3-16 18:00:57
**********************************************************************
*/

void vUSART1_DmaRxStart(void)
{
	DMA_Cmd(DMA1_Channel5, DISABLE);
	/* 注意这里不能调用库函数直接初始化了，那样会把其他设置给搞没了，直接配置寄存器即可 */
	DMA1_Channel5 -> CNDTR = USART1_RX_SIZE;
	DMA1_Channel5 -> CMAR = (uint32_t)vucUSART1_RxBuffer;
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

/*
**********************************************************************
*函数名称：void DMA1_Channel4_IRQHandler(void)
*
*函数功能：DMA发送通道4中断服务函数
*
*使用说明：在下面的初始化函数中调用
*入口参数：无
*返 回 值：无
*
*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:46:44
*函数版本：v1.0 
*修改原因：创建文件
*
*取代版本：无
*原 作 者：zzw
*完成日期：2017-3-16 18:01:09
**********************************************************************
*/

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4))
	{
		DMA_ClearITPendingBit(DMA1_IT_GL4); 																			
		ulUsart1TxOkFlag = 1;
		DMA_Cmd(DMA1_Channel4, DISABLE);
	}
}

/*
**********************************************************************
*函数名称：USART1_IRQHandler(void)
*
*函数功能：使用串口空闲中断，用于确定任意长度数据接收完成
*函数说明：应该是要接受数据以后，首次进入空闲，才会产生空闲中断。
*					USART_ClearITPendingBit(USART1,USART_IT_IDLE);
*					注意：这里不能这么调用库函数清中断标志 goto进底层看代码如下注意点
*   			- PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun 
*    			 error) and IDLE (Idle line detected) pending bits are cleared by 
*     		software sequence: a read operation to USART_SR register 
*    			(USART_GetITStatus()) followed by a read operation to USART_DR register 
*     		(USART_ReceiveData()).
*入口参数：无
*返 回 值：无
*
*当前作者：YJ_朱子文
*创建日期：2017-3-16 17:46:50
*函数版本：v1.0 
*修改原因：创建文件
*
*取代版本：无
*原 作 者：zzw
*完成日期：2017-3-16 18:01:20
**********************************************************************
*/

void USART1_IRQHandler(void)
{
	/* 总线空闲标志位 */
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		/* 清USART_IT_IDLE标志 */
		USART1 -> SR;  
    USART1 -> DR;  
		ulUsart1RecvRealSize = DMA_GetCurrDataCounter(DMA1_Channel5);
		ulUsart1RecvRealSize = USART1_RX_SIZE - ulUsart1RecvRealSize;
		ulUsart1RxOkFlag = 1;
		/* 单次模式 每次接收完成都要停止 */
		DMA_Cmd(DMA1_Channel5, DISABLE);
	}
}

/* end of file cppyright reserve by team of yanjun ,More information please browse www.yanjun.tech */
