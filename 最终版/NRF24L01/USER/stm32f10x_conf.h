


#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H













#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"



#include "stm32f10x_rcc.h"


#include "stm32f10x_spi.h"

#include "stm32f10x_usart.h"

#include "misc.h" 







#ifdef  USE_FULL_ASSERT


  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif 

#endif 



