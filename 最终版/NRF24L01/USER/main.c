#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24l01.h" 	 

u8 careg[1];

 int main(void)
 {	 
	u8 key,mode;
	u16 t=0;			 
	u8 tmp_buf[33];		    
	delay_init();	    	 
	NVIC_Configuration(); 	 
	uart_init(9600);	 	
 	NRF24L01_Init();    	
 	while(NRF24L01_Check())	
	{
		printf("未检测到HARNWARE\r\n");
		delay_ms(10);
	}			
	printf("statr\r\n");

#if 0			    
		printf("发送模式\r\n");	
		NRF24L01_TX_Mode();
		mode=' ';
		while(1)
		{	  		   				 
			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				key=mode;
				for(t=0;t<32;t++)
				{
					key++;
					if(key>('~'))key=' ';
					tmp_buf[t]=key;	
				}
				mode++; 
				if(mode>'~')mode=' ';  	  
				tmp_buf[32]=0;
				printf("ok\r\n");
			}else
			{										   	
 				printf("发送失败\r\n");	   
			};
			delay_ms(1500);				    
		}
	   
#endif	
#if 1
		printf("接收模式\r\n");
		NRF24L01_RX_Mode();	
		while(1)
		{
			NRF24L01_Read_Buf(STATUS,careg,1);

			if(NRF24L01_RxPacket(tmp_buf)==0)
				{
					tmp_buf[32]=0;
					if(((careg[0]&0x0e)>>1)==0)
						printf("通道0接收到的数据为：%s \r\n",tmp_buf);	
					if(((careg[0]&0x0e)>>1)==1)
						printf("通道1接收到的数据为：%s \r\n",tmp_buf);
					if(((careg[0]&0x0e)>>1)==2)
						printf("通道2接收到的数据为：%s \r\n",tmp_buf);
					if(((careg[0]&0x0e)>>1)==3)
						printf("通道3接收到的数据为：%s \r\n",tmp_buf);
					if(((careg[0]&0x0e)>>1)==4)
						printf("通道4接收到的数据为：%s \r\n",tmp_buf);
					if(((careg[0]&0x0e)>>1)==5)
						printf("通道5接收到的数据为：%s \r\n",tmp_buf);
				}
				else
				{
					delay_ms(10);
				}
		}
#endif
}



