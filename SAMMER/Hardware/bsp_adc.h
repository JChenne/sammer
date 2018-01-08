#ifndef _bsp_adc_h_
#define _bsp_adc_h_


#include "stm32f10x.h"

#define ADC_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define ADCx                    ADC2
#define ADC_CLK                 RCC_APB2Periph_ADC2

#define ADC_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK            RCC_APB2Periph_GPIOC
#define ADC_PORT                GPIOC
#define ADC_PIN                 GPIO_Pin_1

#define ADC_CHANNEL             ADC_Channel_11

#define ADC_IRQ                 ADC1_2_IRQn
#define ADC_IRQHandler          ADC1_2_IRQHandler

void vADCx_Init(void);

//#define     ADCx                    ADC1
#define     ADC_APBxClock_FUN       RCC_APB2PeriphClockCmd
//#define     ADC_CLK                 RCC_APB2Periph_ADC1

#define     ADC_GPIO_APBxClock_FUN  RCC_APB2PeriphClockCmd
#define     ADC_GPIO_CLK            RCC_APB2Periph_GPIOC
#define     ADC_PORT                GPIOC

#define     NOFCHANEL               5

#define     ADC_PIN1                GPIO_Pin_0
#define     ADC_CHANNEL1            ADC_Channel_10

#define     ADC_PIN2                GPIO_Pin_1
#define     ADC_CHANNEL2            ADC_Channel_11

#define     ADC_PIN3                GPIO_Pin_3
#define     ADC_CHANNEL3            ADC_Channel_13

#define     ADC_PIN4                GPIO_Pin_4
#define     ADC_CHANNEL4            ADC_Channel_14

#define     ADC_PIN5                GPIO_Pin_5
#define     ADC_CHANNEL5            ADC_Channel_15

#define     ADC_DMA_CHANNEL         DMA1_Channe11

static void ADCx_GPIO_Config(void);

#endif
