/********************       (C) COPYRIGHT 2017  ***************************
 * 文件名  ：led.h
 * 描述    ：led 应用函数库         
 * 实验平台：
 * 硬件连接：-----------------------
 *          |   PE2 - LED(RUN)     |  
 *          |   PE3 - LED4     		 | 
 *          |   PE4 - LED1     		 | 
 *          |   PE5 - LED2    		 | 
 *          |   PE6 - LED3     		 |  
 *           ----------------------- 
 * 库版本  ：ST3.5.0
 * 编写日期：2017-04-10
 * 修改日期：2017-04-10
 * 作者    ：
****************************************************************************/
#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"


/* the macro definition to trigger the led on or off 
 * 1 - on
 - 0 - off
 */
 
#define 		ON  		1
#define 		OFF 		0

#define RCC_LED1						RCC_APB2Periph_GPIOE
#define GPIO_LED1_PORT			GPIOE
#define GPIO_Pin_LED1				GPIO_Pin_4

#define RCC_LED2						RCC_APB2Periph_GPIOE
#define GPIO_LED2_PORT			GPIOE
#define GPIO_Pin_LED2				GPIO_Pin_5

#define RCC_LED3						RCC_APB2Periph_GPIOE
#define GPIO_LED3_PORT			GPIOE
#define GPIO_Pin_LED3				GPIO_Pin_6

#define RCC_LED4						RCC_APB2Periph_GPIOE
#define GPIO_LED4_PORT			GPIOE
#define GPIO_Pin_LED4				GPIO_Pin_3

#define RCC_LEDR						RCC_APB2Periph_GPIOE
#define GPIO_LEDR_PORT			GPIOE
#define GPIO_Pin_LEDR				GPIO_Pin_2

#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIO_LED1_PORT,GPIO_Pin_LED1);\
					else		\
					GPIO_ResetBits(GPIO_LED1_PORT,GPIO_Pin_LED1)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIO_LED2_PORT,GPIO_Pin_LED2);\
					else		\
					GPIO_ResetBits(GPIO_LED2_PORT,GPIO_Pin_LED2)
					
#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIO_LED3_PORT,GPIO_Pin_LED3);\
					else		\
					GPIO_ResetBits(GPIO_LED3_PORT,GPIO_Pin_LED3)

#define LED4(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED4_PORT,GPIO_Pin_LED4);\
					else		\
					GPIO_SetBits(GPIO_LED4_PORT,GPIO_Pin_LED4)
					
#define LEDR(a)	if (a)	\
					GPIO_ResetBits(GPIO_LEDR_PORT,GPIO_Pin_LEDR);\
					else		\
					GPIO_SetBits(GPIO_LEDR_PORT,GPIO_Pin_LEDR)				

void LED_GPIO_Config(void);

#endif /* __LED_H */
