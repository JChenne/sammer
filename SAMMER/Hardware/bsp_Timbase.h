#ifndef _bsp_timbase_h_
#define _bsp_timbase_h_

#include "stm32f10x.h"
#include "bsp_control.h"

#define BASIC_TIM_MAX               1000

#define BASIC_TIM6
#ifdef  BASIC_TIM6

#define BASIC_TIM                   TIM6
#define BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define BASIC_TIM_IRQ               TIM6_IRQn
#define BASIC_TIM_IRQHandler        TIM6_IRQHandler

#else
#define BASIC_TIM                   TIM7
#define BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define BASIC_TIM_CLK               RCC_APB1Periph_TIM7
#define BASIC_TIM_IRQ               TIM7_IRQn
#define BASIC_TIM_IRQHandler        TIM7_IRQHandler 



#endif

void vBASIC_TIM_Config(void);
void vBASIC_TIM_NVIC_Config(void);
void vBASIC_TIM_IRQHandler(void);
void vBASIC_TIM_STAR(void);
void vBASIC_TIM_CLOSE(void);

#endif
