#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "24l01.h" 	 

 int main(void)
 {	 
//	  char i[2];
	  char careg[1];
	//	u16 count=0;
//		double LONG;
//	  u8 LAT;
		char SpeedX[5];
//	  char display_data[30];
		delay_init();	    	 														//��ʱ������ʼ��	  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		uart_init(9600);	 															//���ڳ�ʼ��Ϊ960000
		LED_Init();		  																//��ʼ����LED���ӵ�Ӳ���ӿ�
		NRF24L01_Init();    														//��ʼ��NRF24L01 
		NRF24L01_RX_Mode();		  
	      
//			LAT = NRF24L01_Check();			
	//		printf("%d",LAT);
		while(1)
		{	
					
			
//					if(NRF24L01_RxPacket(SpeedX)==0)
//					{
//						printf("%d,    %d     ",SpeedX[0],SpeedX[1]);
//					}
//					if(NRF24L01_RxPacket(S)==0)
//					{
//						printf("%d\n",S[0]);
//					}
			NRF24L01_Read_Buf(STATUS,careg,1);
			if(NRF24L01_RxPacket(SpeedX)==0)
				{
					
					printf("%d ", careg[0]);
					if(((careg[0]&0x0e)>>1)==0)
						printf("ͨ��0���յ�������Ϊ��%d %d\r",SpeedX[0],SpeedX[1]);	
					if(((careg[0]&0x0e)>>1)==1)
						printf("ͨ��3���յ�������Ϊ��%d %d\r\n",SpeedX[0],SpeedX[1]);
//					if(((careg[0]&0x0e)>>1)==2)
//						printf("ͨ��2���յ�������Ϊ��%d %d\r\n",SpeedX[0],SpeedX[1]);	
//					if(((careg[0]&0x0e)>>1)==3)
//						printf("ͨ��3���յ�������Ϊ��%d %d\r\n",SpeedX[0],SpeedX[1]);	
//					if(((careg[0]&0x0e)>>1)==4)
//						printf("ͨ��4���յ�������Ϊ��%d %d\r\n",SpeedX[0],SpeedX[1]);	
//					if(((careg[0]&0x0e)>>1)==5)
//						printf("ͨ��5���յ�������Ϊ��%d %d\r\n",SpeedX[0],SpeedX[1]);	
				}
					
//			 i = NRF24L01_RxPacket(buf_c);
//			printf("%d",i);
			
		
//				NRF24L01_RxPacket(buf_c);
//				for(i = 2; i <= 22; i++)
//				{
//					display_data[i] = buf_c[i]-48;
//				}
				
//					printf("****************************************************\r\n");
				
//					printf("The lift distance is: %d cm\r    The right distance is: %d cm\r\n",buf_c[0],buf_c[1]);
//	        printf("%d,%d,%d,%d,%d\n",SpeedX[0],SpeedX[1],SpeedX[2],SpeedX[3],SpeedX[4]);			
//					printf("N_S: %c\r  Latiude:%d%d��%d%d��%d%d��\r\n",buf_c[9],display_data[2],display_data[3],display_data[4],
//																		display_data[5],display_data[7],display_data[8]);
//				
//					printf("E_W: %c\r  Longitude:%d%d%d��%d%d��%d%d��\r\n",buf_c[18],display_data[10],display_data[11],
//																		display_data[12],display_data[13],display_data[14],display_data[16],display_data[17]);
//					LONG = display_data[2];
//					for(i = 3; i <= 5; i++)
//					{
//						LONG = LONG * 10 + display_data[i];
//					}
//					for(i = 7; i <= 8; i++)
//					{
//						LONG = LONG * 10 + display_data[i];
//					}
//					printf("%lf\r\n",LONG);
//					
//					LAT = display_data[10];
//					for(i = 11; i <= 14; i++)
//					{
//						LAT = LAT * 10 + display_data[i];
//					}
//					for(i = 16; i <= 17; i++)
//					{
//						LAT = LAT * 10 + display_data[i];
//					}
				
//			count++;
//			if(count==6)																//��Լ1s�Ӹı�һ��״̬
//			{
//				count=0;
//				LED0=!LED0;
//			} 	
  		 delay_ms(80);	
		}
}



