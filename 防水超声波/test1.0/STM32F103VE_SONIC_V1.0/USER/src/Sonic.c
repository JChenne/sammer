#include "Sonic.h"

u32  Distance_lift = 0 ; //Distance_right = 0;
u8   Done;
u32 __IO time_1ms = 0;

void TIM6_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure_lift;
  //NVIC_InitTypeDef 		   NVIC_InitStructure;
  /* TIM6 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure_lift.TIM_Period = 0xFFFF; 
	TIM_TimeBaseStructure_lift.TIM_Prescaler = 142;          //144分频，500K的计数器
	TIM_TimeBaseStructure_lift.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure_lift.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure_lift);
  TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);	
	TIM_Cmd(TIM6, DISABLE);
}

//void TIM4_Init(void)
//{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure_right;
//  //NVIC_InitTypeDef 		   NVIC_InitStructure;
//  /* TIM6 clock enable */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//  /* Time base configuration */
//  TIM_TimeBaseStructure_right.TIM_Period = 0xFFFF; 
//	TIM_TimeBaseStructure_right.TIM_Prescaler = 142;          //144分频，500K的计数器
//	TIM_TimeBaseStructure_right.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure_right.TIM_CounterMode = TIM_CounterMode_Up;

//  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure_right);
//  TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);	
//	TIM_Cmd(TIM4, DISABLE);
//}

void Sonic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure_lift;
	EXTI_InitTypeDef EXTI_InitStructure_lift;
	GPIO_InitTypeDef GPIO_InitStructure_lift;
//	
//	NVIC_InitTypeDef NVIC_InitStructure_right;
//	EXTI_InitTypeDef EXTI_InitStructure_right;
//	GPIO_InitTypeDef GPIO_InitStructure_right;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF| RCC_APB2Periph_AFIO,ENABLE);
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF| RCC_APB2Periph_AFIO,ENABLE);
	
//	GPIO_InitStructure_lift.GPIO_Mode = GPIO_Mode_Out_PP;       
//	GPIO_InitStructure_lift.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure_lift.GPIO_Pin = GPIO_Pin_1;						//PC4 Trig_lift
//	GPIO_Init(GPIOF,&GPIO_InitStructure_lift);
//	
//	GPIO_InitStructure_lift.GPIO_Mode= GPIO_Mode_IPD;
//	GPIO_InitStructure_lift.GPIO_Pin=GPIO_Pin_2;           		//PC5 Echo_lift
//	GPIO_Init(GPIOF,&GPIO_InitStructure_lift);   
	
	GPIO_InitStructure_lift.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure_lift.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure_lift.GPIO_Pin = GPIO_Pin_1;						//PF1 Trig_right
	GPIO_Init(GPIOF,&GPIO_InitStructure_lift);
	
	GPIO_InitStructure_lift.GPIO_Mode= GPIO_Mode_IPD;
	GPIO_InitStructure_lift.GPIO_Pin=GPIO_Pin_2;           	//PF2 Echo_right
	GPIO_Init(GPIOF,&GPIO_InitStructure_lift); 
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_1,(BitAction)0); 			//trig
	
//	GPIO_WriteBit(GPIOF,GPIO_Pin_1,(BitAction)0);
	
	//EXTI_DeInit();
	EXTI_ClearITPendingBit(EXTI_Line5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource2);
	EXTI_InitStructure_lift.EXTI_Line= EXTI_Line5; 
	EXTI_InitStructure_lift.EXTI_Mode= EXTI_Mode_Interrupt; 
	EXTI_InitStructure_lift.EXTI_Trigger= EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure_lift.EXTI_LineCmd=ENABLE;
  EXTI_Init(&EXTI_InitStructure_lift);
	
//	EXTI_ClearITPendingBit(EXTI_Line2);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource2);
//	EXTI_InitStructure_right.EXTI_Line= EXTI_Line2; 
//	EXTI_InitStructure_right.EXTI_Mode= EXTI_Mode_Interrupt; 
//	EXTI_InitStructure_right.EXTI_Trigger= EXTI_Trigger_Rising_Falling;
//	EXTI_InitStructure_right.EXTI_LineCmd=ENABLE;
//  EXTI_Init(&EXTI_InitStructure_right);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure_lift.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure_lift.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStructure_lift.NVIC_IRQChannelSubPriority= 0;        
	NVIC_InitStructure_lift.NVIC_IRQChannelCmd=ENABLE;   
	NVIC_Init(&NVIC_InitStructure_lift);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	NVIC_InitStructure_right.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure_right.NVIC_IRQChannelPreemptionPriority= 0;
//	NVIC_InitStructure_right.NVIC_IRQChannelSubPriority= 0;        
//	NVIC_InitStructure_right.NVIC_IRQChannelCmd=ENABLE;   
//	NVIC_Init(&NVIC_InitStructure_right);
	
	Distance_lift = 0;
	//Distance_right = 0;
	Done = 1;
}

void Sonic_Trig(void)
{
	u16 i = 0;
	if((Done == 1)&&(time_1ms > 100))
	{
		time_1ms = 0;
//		GPIO_WriteBit(GPIOC,GPIO_Pin_4,(BitAction)1);
//		for(i=0;i<0xf0;i++);
//		GPIO_WriteBit(GPIOC,GPIO_Pin_4,(BitAction)0); 
		
		GPIO_WriteBit(GPIOF,GPIO_Pin_1,(BitAction)1);
		for(i=0;i<0xf0;i++);
		GPIO_WriteBit(GPIOF,GPIO_Pin_1,(BitAction)0); 
		Done = 0;
	}
}

void EXTI9_5_IRQHandler(void)
{
	static u8 flag_Sta_l = 0;
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)		
	{
		EXTI_ClearITPendingBit(EXTI_Line2);  
		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)==1)
		{
			 TIM_SetCounter(TIM6,0);	
//			 TIM_SetCounter(TIM4,0);
			 flag_Sta_l=1;
			 TIM_Cmd(TIM6, ENABLE);  	
//			 TIM_Cmd(TIM4, ENABLE);  	
		}
		else
		{
			TIM_Cmd(TIM6, DISABLE); 	
//			TIM_Cmd(TIM4, DISABLE);			
			if(flag_Sta_l)
			{		
				Distance_lift = TIM_GetCounter(TIM6);
				Distance_lift = 34*Distance_lift/1000;
//				Distance_right = TIM_GetCounter(TIM4);
//				Distance_right = 34*Distance_lift/1000;
//				if(Distance > 300)
//					Distance = 300;
				Done = 1;
			}
			flag_Sta_l=0;
		}
	}
}

//void EXTI9_5_IRQHandlerr(void)
//{
//	static u8 flag_Sta_r = 0;
//	if(EXTI_GetITStatus(EXTI_Line2) != RESET)		
//	{
//		EXTI_ClearITPendingBit(EXTI_Line2);  
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)==1)
//		{
//			 TIM_SetCounter(TIM6,0);	
//			 flag_Sta_r=1;
//			 TIM_Cmd(TIM6, ENABLE);  			
//		}
//		else
//		{
//			TIM_Cmd(TIM6, DISABLE); 		
//			if(flag_Sta_r)
//			{		
//				Distance_right = TIM_GetCounter(TIM6);
//				Distance_right =456 ;//34*Distance_right/1000;
////				if(Distance > 300)
////					Distance = 300;
//				Done = 1;
//			}
//			flag_Sta_r=0;
//		}
//	}
//}


