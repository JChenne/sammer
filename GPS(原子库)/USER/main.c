#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "GPS.h"

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//�������ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	delay_init();
	clrStruct();
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();
	}
}
