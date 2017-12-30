#include "usart1.h"
#include "Sonic.h"
#include "SysTick.h"


extern u32  Distance_lift , Distance_right;   //�ⲿ����

 
int main(void)
{	 
		SysTick_Init();										//Systick��ʱ������������ģ��
		USART1_Config(115200);						// ��ʼ��USART1 
		TIM6_Init();											//TIM6 Init
		TIM4_Init();											//TIM4 Init
		Sonic_Init();											//Sonic_Init 
		while(1)
		{ 
				Delay_ms(300);
				printf("The lift distance is: %d cm\r    The right distance is: %d cm\n",Distance_lift,Distance_right);
		}
}



