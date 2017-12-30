
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "adc.h"
#include "key.h"

extern __IO uint16_t ADC_ConvertedValue[2];


int main(void)
{	 
	u8 KEY;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200			 	
 	Adc_Init();		  		//ADC初始化
	KEY_Init();    
	while(1)
	{
		while(KEY == 1)；
		KEY = KEY_Scan();
		if(KEY == 1)
		{
			while(1)
			{
				printf("%d, %d\n", ADC_ConvertedValue[0],ADC_ConvertedValue[1]);
				delay_ms(80);
				KEY = KEY_Scan();
				if(KEY == 1)
				break;
			}
		} 
	}
 }

