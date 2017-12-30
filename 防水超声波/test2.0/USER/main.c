#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "jsn_sr04t.h"

 int main(void)
 {		
	float length;
	Hcsr04Init();   	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	while(1)
	{
		length = Hcsr04GetLength();
		printf("the distance is:%f/r/n",length);
	}	 
 }

