#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "24l01.h" 	 
#include "Sonic.h"
#include "SysTick.h"

extern u32  Distance_lift , Distance_right;   //外部变量

 int main(void)
 {	 
//		u8 key,mode;	
		u16 count = 0;
		u8 distance_buf[2];		    
		delay_init();	    			//延时函数初始化	  
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
		uart_init(115200); 		//串口初始化为115200
		LED_Init();		  				//初始化与LED连接的硬件接口
//		KEY_Init();							//初始化按键
//		NRF24L01_Init();    		//初始化NRF24L01 
		SysTick_Init();					//Systick定时器触发超声波模块
		TIM6_Init();						//TIM6 Init
		TIM4_Init();						//TIM4 Init
		Sonic_Init();						//Sonic_Init 
//		while(NRF24L01_Check())
//		{
//				delay_ms(200);
//		} 	 
//		while(1)
//		{	
//				key=KEY_Scan(0);
//				if(key==WKUP_PRES)
//				{
//						mode=0;   
//						break;
//				}
//				else if(key==KEY1_PRES)
//				{
//						mode=1;
//						break;
//				}
//		}   
//		if(mode==0)//RX模式
//		{
//			NRF24L01_RX_Mode();		  
//			while(1)
//			{	  		    		    				 
//					if(NRF24L01_RxPacket(distance_buf)==0)//一旦接收到信息,则显示出来.
//					{
//					printf("The lift distance is: %d cm\r    The right distance is: %d cmx\n",distance_buf[0],distance_buf[1]);
//					}
//					else delay_us(100);	   
//					count++;
//					if(count == 10000)//大约1s钟改变一次状态
//					{
//						count = 0;
//						LED0 =! LED0;
//					} 				    
//			}	
//		}else//TX模式
//		{						
//			NRF24L01_TX_Mode(); 
			while(1)
			{	  		   				 
				
				distance_buf[0] = Distance_lift;
				distance_buf[1] = Distance_right;
//				NRF24L01_TxPacket(distance_buf);
				
				printf("The lift distance is: %d cm\r    The right distance is: %d cm\n",distance_buf[0],distance_buf[1]);	
				
				count++;
				if(count == 30)
				{
					count = 0;
					LED0=!LED0;
				}
				delay_ms(300);
			}
		} 
//}


