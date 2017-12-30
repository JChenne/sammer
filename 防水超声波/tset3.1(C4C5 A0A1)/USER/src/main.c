#include "usart1.h"
#include "Sonic.h"
#include "SysTick.h"


extern u32  Distance_lift , Distance_right;   //外部变量

 
int main(void)
{	 
		SysTick_Init();										//Systick定时器触发超声波模块
		USART1_Config(115200);						// 初始化USART1 
		TIM6_Init();											//TIM6 Init
		TIM4_Init();											//TIM4 Init
		Sonic_Init();											//Sonic_Init 
		while(1)
		{ 
				Delay_ms(300);
				printf("The lift distance is: %d cm\r    The right distance is: %d cm\n",Distance_lift,Distance_right);
		}
}



