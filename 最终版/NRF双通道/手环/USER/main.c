#include "delay.h"
#include "sys.h" 
#include "24l01.h" 	 
#include "stm32f10x.h"
#include "adc.h"
#include "key.h"


extern __IO uint16_t ADC_ConvertedValue[2];


int main(void)
{	 
		u8 S[1];			//TIM4 Init																				//Sonic_Init 
		delay_init();	    	//��ʱ������ʼ��	  																		//���ڳ�ʼ�������ò�����Ϊ9600		  																			
		T_NRF24L01_Init();    	//��ʼ��NRF24L01 
		T_NRF24L01_TX_Mode(); 	//��NRF24L01����Ϊ����ģʽ																					//���GPS�ڴ�			
//	 	Adc_Init();		  		//ADC��ʼ��
		KEY_Init();    
		while(1)
		{	  
//				if(ADC_ConvertedValue[0]>=4000)
//					SpeedX[0] = 0;
//				if(ADC_ConvertedValue[0]>=3000&ADC_ConvertedValue[0]<4000)
//					SpeedX[0] = 1;
//				if(ADC_ConvertedValue[0]>2000&ADC_ConvertedValue[0]<3000)
//					SpeedX[0] = 2;
//				if(ADC_ConvertedValue[0]<=2000&ADC_ConvertedValue[0]>100)
//					SpeedX[0] = 3;
//				if(ADC_ConvertedValue[0]<=100)
//					SpeedX[0] = 4;
//				
//				if((ADC_ConvertedValue[1]>=100)&&(ADC_ConvertedValue[1]<= 4000))
//					SpeedX[1] = 0;
//				if(ADC_ConvertedValue[1]>=4000)
//					SpeedX[1] = 1;		
//				if(ADC_ConvertedValue[1]<100)
//					SpeedX[1] = 2;
				delay_ms(30);
				S[0] = 44;
//				if(KEY_Scan() == 0)
//				{
					T_NRF24L01_TxPacket(S);
//				}									
		}
} 





