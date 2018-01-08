#ifndef  __bsp_gpio_h
#define  __bsp_gpio_h

#include "stm32f10x.h"

#define AIN1_1GPIO_CLK      RCC_APB2Periph_GPIOB
#define AIN1_1GPIO_PORT     GPIOB
#define AIN1_1GPIO_PIN      GPIO_Pin_1

#define AIN1_2GPIO_CLK      RCC_APB2Periph_GPIOB
#define AIN1_2GPIO_PORT     GPIOB
#define AIN1_2GPIO_PIN      GPIO_Pin_0

#define TURN_RIGHT_1         {AIN1_1GPIO_PORT->BSRR = AIN1_1GPIO_PIN;\
                              AIN1_2GPIO_PORT->BRR  = AIN1_2GPIO_PIN;}
#define TURN_LEFT_1          {AIN1_1GPIO_PORT->BRR  = AIN1_1GPIO_PIN;\
                              AIN1_2GPIO_PORT->BSRR = AIN1_2GPIO_PIN;}
#define TURN_TOGGLE_1        {AIN1_1GPIO_PORT->ODR ^= AIN1_1GPIO_PIN;\
                              AIN1_2GPIO_PORT->ODR ^= AIN1_2GPIO_PIN;}

                            

#define AIN2_1GPIO_CLK      RCC_APB2Periph_GPIOC
#define AIN2_1GPIO_PORT     GPIOC
#define AIN2_1GPIO_PIN      GPIO_Pin_1

#define AIN2_2GPIO_CLK      RCC_APB2Periph_GPIOC
#define AIN2_2GPIO_PORT     GPIOC
#define AIN2_2GPIO_PIN      GPIO_Pin_2

#define TURN_RIGHT_2         {AIN2_1GPIO_PORT->BSRR = AIN2_1GPIO_PIN;\
                              AIN2_2GPIO_PORT->BRR  = AIN2_2GPIO_PIN;}
#define TURN_LEFT_2          {AIN2_1GPIO_PORT->BRR  = AIN2_1GPIO_PIN;\
                              AIN2_2GPIO_PORT->BSRR = AIN2_2GPIO_PIN;}
#define TURN_TOGGLE_2        {AIN2_1GPIO_PORT->ODR ^= AIN2_1GPIO_PIN;\
                              AIN2_2GPIO_PORT->ODR ^= AIN2_2GPIO_PIN;}                            
                            

void vAIN_GPIO_Config(void);

#endif
