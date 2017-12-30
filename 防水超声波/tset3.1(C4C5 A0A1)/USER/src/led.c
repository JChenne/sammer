/********************       (C) COPYRIGHT 2017  ***************************
 * �ļ���  ��led.c
 * ����    ��led Ӧ�ú�����         
 * ʵ��ƽ̨��
 * Ӳ�����ӣ�-----------------------
 *          |   PE2 - LED(RUN)     |  
 *          |   PE3 - LED4     		 | 
 *          |   PE4 - LED1     		 | 
 *          |   PE5 - LED2    		 | 
 *          |   PE6 - LED3     		 |  
 *           ----------------------- 
 * ��汾  ��ST3.5.0
 * ��д���ڣ�2017-04-10
 * �޸����ڣ�2017-04-10
 * ����    ��
****************************************************************************/
#include "led.h"

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*���� LED1 ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LED1, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LED1 GPIOx����*/															   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED1;	
	/*��������ģʽΪͨ���������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LED1 ��GPIOx*/
  GPIO_Init(GPIO_LED1_PORT, &GPIO_InitStructure);		  

		/*���� LED2 ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LED2, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LED2 GPIOx����*/															   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED2;	
	/*��������ģʽΪͨ���������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LED2 ��GPIOx*/
  GPIO_Init(GPIO_LED2_PORT, &GPIO_InitStructure);	
	
	
		/*���� LED3 ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LED3, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LED3 GPIOx����*/															   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED3;	
	/*��������ģʽΪͨ���������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LED3 ��GPIOx*/
  GPIO_Init(GPIO_LED3_PORT, &GPIO_InitStructure);	
	
	
		/*���� LED4 ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LED4, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LED4 GPIOx����*/															   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED4;	
	/*��������ģʽΪͨ���������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LED4 ��GPIOx*/
  GPIO_Init(GPIO_LED4_PORT, &GPIO_InitStructure);	
	
	
		/*���� LEDR ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LEDR, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LEDR GPIOx����*/															   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LEDR;	
	/*��������ģʽΪͨ���������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LEDR ��GPIOx*/
  GPIO_Init(GPIO_LEDR_PORT, &GPIO_InitStructure);	
	
	/* �ر�LED1 LED2 LED3 LED4 LEDR	*/
	LED1(OFF);
	LED2(OFF);
	LED3(OFF);
	LED4(OFF);
	LEDR(OFF);	
}




/******************* (C) COPYRIGHT  2014 *****END OF FILE************/
