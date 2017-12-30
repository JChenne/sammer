#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(uint32_t uBaud);
void USART1_Configuration(uint32_t uBaud);
int fputc(int ch, FILE *f);
void USART1_NVIC_Configuration(void);

#endif /* __USART1_H */
