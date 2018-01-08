#ifndef _SYSTEM_H
#define _SYSTEM_H	

#include "stm32f10x.h"
#include "systick.h"														    
	 
/* 位带操作,实现51类似的GPIO控制功能 */
/* 具体实现思想,参考<<CM3权威指南>>第五章(87页~92页). */
/* IO口操作宏定义 */
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
/* IO口地址映射 */
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
/* IO口操作,只对单一的IO口! */
/* 确保n的值小于16! */
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)   

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)   

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  

/*
* 全局中断优先级配置宏定义
*/

/* 应该把，中断分组，放到全局system.h文件中 */
/* 串口发送通道 DMA中断的 NVIC配置 */
#define DMA1_Channel4PreemptionPriority						(0X06)
#define DMA1_Channel4SubPriority									(0X00)

/* 串口1中断优先级 */
#define UART1_PreemptionPriority									(0X06)
#define UART1_SubPriority													(0X00)

/**
*DMA优先级定义
*
*/

/* IIC2的接受通道，也就是DMA1的通道4的优先级 */
#define DMA1_USART1_TX_CHANNEL4_PRIORITY				DMA_Priority_VeryHigh
/* IIC2的发送通道，也就是DMA1的通道5的优先级 */
#define DMA1_USART1_RX_CHANNEL5_PRIORITY				DMA_Priority_VeryHigh

/* 串口2相关的优先级宏定义 */
#define DMA1_Channel7PreemptionPriority	0x04
#define DMA1_Channel7SubPriority				0x00
#define USART2_PreemptionPriority				0x05
#define USART2_SubPriority							0x00
/* IIC2的接受通道，也就是DMA1的通道4的优先级 */
#define DMA1_USART2_TX_CHANNEL7_PRIORITY				DMA_Priority_VeryHigh
/* IIC2的发送通道，也就是DMA1的通道5的优先级 */
#define DMA1_USART2_RX_CHANNEL6_PRIORITY				DMA_Priority_VeryHigh

/* 这些宏定义 在实际项目中应该放在 system.h中方便管理 */
#define TIM8_PreemptionPriority						(0x03)
#define TIM8_SubPriority									(0x00)

/*  定时器1中断优先级，在真正项目中 应该放在system.h文件中 方便管理  */
#define TIM1_PreemptionPriority						(0X00)
#define TIM1_SubPriority									(0X00)

#define TIM4_IC_PreemptionPriority 				0x01
#define TIM4_IC_SubPriority 							0x00

#define TIM3_IC_PreemptionPriority	 			0x02
#define TIM3_IC_SubPriority 							0x00

/* TIM8控制的是PITCH 的A B C 相 */
#define TIM8_OC_Polarity_High		TIM_OCPolarity_High

#endif

/* end of file cppyright reserve by team of yanjun ,More information please browse www.yanjun.tech */
