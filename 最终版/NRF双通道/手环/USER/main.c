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
		delay_init();	    	//延时函数初始化	  																		//串口初始化，设置波特率为9600		  																			
		T_NRF24L01_Init();    	//初始化NRF24L01 
		T_NRF24L01_TX_Mode(); 	//将NRF24L01设置为发送模式																					//清空GPS内存			
//	 	Adc_Init();		  		//ADC初始化
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





