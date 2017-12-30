#include "usart.h"
#include "Sonic.h"
#include "delay.h"
#include "timer.h"
//#include "Systick.h"

extern u32  Distance_lift , Distance_right;   //外部变量

 
int main(void)
{	 
	
		uart_init(115200);						// 初始化USART1 
		TIM6_Init();											//TIM6 Init
		TIM4_Init();											//TIM4 Init
		Sonic_Init();											//Sonic_Init 
	  TIM3_Int_Init(4,7199);
		delay_init();
		while(1)
		{ 
				delay_ms(25);
				printf("The lift distance is: %d cm\r    The right distance is: %d cm\n",Distance_lift,Distance_right);
		}
}

