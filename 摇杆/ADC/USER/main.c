
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "adc.h"
#include "key.h"

extern __IO uint16_t ADC_ConvertedValue[2];


int main(void)
{	 
	u8 KEY;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200			 	
 	Adc_Init();		  		//ADC��ʼ��
	KEY_Init();    
	while(1)
	{
		while(KEY == 1)��
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

