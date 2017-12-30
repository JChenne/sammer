#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "GPS.h"

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//设置优先级
	uart_init(9600);	 //串口初始化为9600
	delay_init();
	clrStruct();
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();
	}
}
