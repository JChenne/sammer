#include "Sonic.h"

u32  Distance_lift = 0;
u32  Distance_right = 0;
u32  lengthtemp_lift = 0;
u32  lengthtemp_right = 0;
u32  __IO time_1ms = 0;
u32  sum_right = 0;
u32  sum_lift = 0;
u8   Done;

void TIM6_Init(void)							//定时器测时间
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		//NVIC_InitTypeDef 		   NVIC_InitStructure;
		/* TIM6 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
		TIM_TimeBaseStructure.TIM_Prescaler = 142;          				//144分频，500K的计数器
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
		TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);	
		TIM_Cmd(TIM6, DISABLE);
}
void TIM4_Init(void)						
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		//NVIC_InitTypeDef 		   NVIC_InitStructure;
		/* TIM6 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
		TIM_TimeBaseStructure.TIM_Prescaler = 142;         				 //144分频，500K的计数器
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);	
		TIM_Cmd(TIM4, DISABLE);
}

void Sonic_Init(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;												//PA1 Trig
		GPIO_Init(GPIOA,&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;												//PC4 Trig
		GPIO_Init(GPIOC,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;           							//PA0 Echo
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;           							//PC5 Echo
		GPIO_Init(GPIOC,&GPIO_InitStructure);
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,(BitAction)0); 									//PA1  Trig
		GPIO_WriteBit(GPIOC,GPIO_Pin_4,(BitAction)0); 									//PC4  Trig
		
		//EXTI_DeInit();																								//将Echo设置成外部中断
		EXTI_ClearITPendingBit(EXTI_Line0);															//线4
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
		EXTI_InitStructure.EXTI_Line= EXTI_Line0; 											
		EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt; 
		EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Rising_Falling;   //上升/下降均触发
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		EXTI_ClearITPendingBit(EXTI_Line5);															//线5
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
		EXTI_InitStructure.EXTI_Line= EXTI_Line5; 											
		EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt; 
		EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Rising_Falling;		//上升/下降均触发
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);									//设置优先级
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;				
		NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0;        
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;   
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;				//区分两次优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1;        
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;   
		NVIC_Init(&NVIC_InitStructure);
		
		Distance_lift = 0;
		Distance_right = 0;
		Done = 1;
}

void Sonic_Trig(void)
{
		u16 i = 0;
		if((Done == 1)&&(time_1ms > 100))
		{
				time_1ms = 0;
				GPIO_WriteBit(GPIOA,GPIO_Pin_1,(BitAction)1);
				for(i=0;i<0xf0;i++);
				GPIO_WriteBit(GPIOA,GPIO_Pin_1,(BitAction)0); 
				
				GPIO_WriteBit(GPIOC,GPIO_Pin_4,(BitAction)1);
				for(i=0;i<0xf0;i++);
				GPIO_WriteBit(GPIOC,GPIO_Pin_4,(BitAction)0); 
				Done = 0;
			}
}

void EXTI0_IRQHandler(void)								//线4	
{
		static u8 flag_Sta_1 = 0;
		int count = 0;
		if(EXTI_GetITStatus(EXTI_Line0) != RESET)		
		{
				EXTI_ClearITPendingBit(EXTI_Line0);  
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
				{
					 TIM_SetCounter(TIM6,0);	
					 flag_Sta_1=1;
					 TIM_Cmd(TIM6, ENABLE);     
				}
				else
				{
						TIM_Cmd(TIM6, DISABLE); 					
						if(flag_Sta_1)
						{		
								for(count = 0; count < 5; count++)
								{
										lengthtemp_lift = TIM_GetCounter(TIM6);
										lengthtemp_lift = 34*lengthtemp_lift/1000;      //公式：2* L = VT = 340m/s * n * 2us = 340*2*n/1000000（m）=34*2*n/1000（cm）
										sum_lift = lengthtemp_lift + sum_lift;
								}
								Done = 1;
								Distance_lift = sum_lift / 5.0;
								sum_lift = 0;
						}
						flag_Sta_1=0;
				}
		}
}

void EXTI9_5_IRQHandler(void)						//线5-9
{
		static u8 flag_Sta_2 = 0;
    int count = 0;
		if(EXTI_GetITStatus(EXTI_Line5) != RESET)		
		{
				EXTI_ClearITPendingBit(EXTI_Line5);  
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==1)
				{
						 TIM_SetCounter(TIM4,0);	
						 flag_Sta_2=1;
						 TIM_Cmd(TIM4, ENABLE);     
				}
				else
				{
						TIM_Cmd(TIM4, DISABLE); 					
						if(flag_Sta_2)
						{		
								for(count = 0; count < 5; count++)
								{
									lengthtemp_right = TIM_GetCounter(TIM4);
									lengthtemp_right = 34*lengthtemp_right/1000;
									sum_right = lengthtemp_right + sum_right;
								}
								Done = 1;
								Distance_right = sum_right / 5.0;
								sum_right = 0;
						}
						flag_Sta_2=0;
				}
		}
}


