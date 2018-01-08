#ifndef _SYSTEM_H
#define _SYSTEM_H	

#include "stm32f10x.h"
#include "systick.h"														    
	 
/* λ������,ʵ��51���Ƶ�GPIO���ƹ��� */
/* ����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ). */
/* IO�ڲ����궨�� */
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
/* IO�ڵ�ַӳ�� */
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
 
/* IO�ڲ���,ֻ�Ե�һ��IO��! */
/* ȷ��n��ֵС��16! */
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

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
* ȫ���ж����ȼ����ú궨��
*/

/* Ӧ�ðѣ��жϷ��飬�ŵ�ȫ��system.h�ļ��� */
/* ���ڷ���ͨ�� DMA�жϵ� NVIC���� */
#define DMA1_Channel4PreemptionPriority						(0X06)
#define DMA1_Channel4SubPriority									(0X00)

/* ����1�ж����ȼ� */
#define UART1_PreemptionPriority									(0X06)
#define UART1_SubPriority													(0X00)

/**
*DMA���ȼ�����
*
*/

/* IIC2�Ľ���ͨ����Ҳ����DMA1��ͨ��4�����ȼ� */
#define DMA1_USART1_TX_CHANNEL4_PRIORITY				DMA_Priority_VeryHigh
/* IIC2�ķ���ͨ����Ҳ����DMA1��ͨ��5�����ȼ� */
#define DMA1_USART1_RX_CHANNEL5_PRIORITY				DMA_Priority_VeryHigh

/* ����2��ص����ȼ��궨�� */
#define DMA1_Channel7PreemptionPriority	0x04
#define DMA1_Channel7SubPriority				0x00
#define USART2_PreemptionPriority				0x05
#define USART2_SubPriority							0x00
/* IIC2�Ľ���ͨ����Ҳ����DMA1��ͨ��4�����ȼ� */
#define DMA1_USART2_TX_CHANNEL7_PRIORITY				DMA_Priority_VeryHigh
/* IIC2�ķ���ͨ����Ҳ����DMA1��ͨ��5�����ȼ� */
#define DMA1_USART2_RX_CHANNEL6_PRIORITY				DMA_Priority_VeryHigh

/* ��Щ�궨�� ��ʵ����Ŀ��Ӧ�÷��� system.h�з������ */
#define TIM8_PreemptionPriority						(0x03)
#define TIM8_SubPriority									(0x00)

/*  ��ʱ��1�ж����ȼ�����������Ŀ�� Ӧ�÷���system.h�ļ��� �������  */
#define TIM1_PreemptionPriority						(0X00)
#define TIM1_SubPriority									(0X00)

#define TIM4_IC_PreemptionPriority 				0x01
#define TIM4_IC_SubPriority 							0x00

#define TIM3_IC_PreemptionPriority	 			0x02
#define TIM3_IC_SubPriority 							0x00

/* TIM8���Ƶ���PITCH ��A B C �� */
#define TIM8_OC_Polarity_High		TIM_OCPolarity_High

#endif

/* end of file cppyright reserve by team of yanjun ,More information please browse www.yanjun.tech */
