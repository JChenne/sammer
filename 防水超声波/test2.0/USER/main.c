#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "jsn_sr04t.h"

 int main(void)
 {		
	float length;
	Hcsr04Init();   	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	while(1)
	{
		length = Hcsr04GetLength();
		printf("the distance is:%f/r/n",length);
	}	 
 }

