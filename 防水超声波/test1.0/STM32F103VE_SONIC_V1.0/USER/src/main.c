#include "usart1.h"
#include "led.h"
#include "tim2.h"
#include "Sonic.h"
#include "SysTick.h"

extern u32  Distance_lift , Distance_right;
 
int main(void)
{	 
	SysTick_Init();
 	USART1_Config(115200);						/* ��ʼ��USART1 */
	TIM2_Config(); 										/* ��ʱ��TIM2��ʼ�� */	
//	TIM4_Config();
	TIM6_Init();			//TIM6 Init
  //TIM4_Init();
	Sonic_Init();			//Sonic_Init 
	while(1)
	{ 
		Delay_ms(300);
		printf("The lift Distance is:%d\r\n" , Distance_lift );
	}
}



