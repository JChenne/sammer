/*
***********************************************************************
* 			YanJunFly V1.0 - Copyright (c) 2017
* All rights reserved.More information please browse www.yanjun.tech
* �࿥�ǿ�-������Ŀ�Դ�ɿ��������������������ѧϰ�̡̳�YJ���������� -- ����ֹ����
* 
* ����������
*						�Դ����в�����bug�����˲��е��κ�ֱ�ӻ��ӵ����⳥
*						��ʹ�ñ����뼴��ΪĬ�ϱ����������bug���뽫��ϸ��Ϣ
*						����������������������лл��
*						http://www.yanjun.tech/forum.php?gid=87
*
* �ļ����ƣ�usart1_dma.c,usart1_dma.h
* �ļ�ժҪ��
*						����ļ���usart1�ĵײ������ļ���ʹ��DMA���ж�ȡ�ͷ�������
*						��Ҫ���ϲο� ��STM32���Ĳο��ֲᡷDMA�½� usart�½� �Լ�
*           ��3.5�̼���ο��ֲᡷDMA�½�  usart�½�
*						���ļ��к���������ʵ�ִ���DMA���ͺͽ������ⳤ�����ݡ�
*						����DMA������� ����DMA��������жϣ�DMA���õ��η���ģʽ
*						ÿ�η������ݣ���Ҫ���ó��Ⱥ���ʼ��ַ
*						�������ݽ������ ʵ�ʴ������Ǵ��ڿ����жϡ�ʹ������жϣ�
*						������ɽ����������ݹ��ܣ�DMA�����ж�ֻ�ܽ��չ̶���������
*
*						ʹ�����ţ�PA9 - TX  PA10 - RX
*
*	ע    �⣺
*						����ļ�ʵ�ֵ�DMA���ʹ������ݺ�DMA���մ������ݡ�
*						�鿴��stm32���Ĳο��ֲᡷDMA �½ڵ�143ҳDMA��ͼ�Լ���148ҳDMA1�����
*						����USART1��DMA1�ϣ�����USART1_TX�����ͣ���DMA1��ͨ��4��
*						USART1_RX(����)��DMA1��ͨ��5�ϡ�DMA1��7·ͨ����DMA2��5·ͨ��������˵����
*						�������ʹ��ͬһ��DMAͨ����ʱ�򣬾���Ҫ�����ֶ��� ʹ�� �� ʧ��DMAͨ��
*						ʵ�ַ�ʱ���õĹ���
*						
* ��ǰ�汾��v1.0
* ��ǰ���ߣ�YJ_������
* ������ڣ�2017-3-16 17:42:59
* �Ķ�˵���������ļ�
*
* ȡ���汾��v1.0
* ԭ �� �ߣ�zzw
* ������ڣ�2017-3-16 17:53:04
***********************************************************************
*/

#include "usart1_dma.h"

/* ��־λ������Ϊ1 ʹ�������� */
uint32_t ulUsart1TxOkFlag = 1;
uint32_t ulUsart1RxOkFlag = 1;
/* ����DMA���յ������������ݵĳ��� */
uint32_t ulUsart1RecvRealSize = 0;
/* �洢�ϴη������ݵĳ��� */
uint32_t ulUsart1Dma1Channe5Increase = 0;
/* �洢�ϴη��͵�λ�� */
uint32_t ulUsart1SendPreIndex = 0;
/* vu8 ������� ���ײ� Ҳ����volatile ���� ��������������ױ�ģ����ȶ��ģ�
 * ��Ҫÿ�ζ�ȡ���ڴ���ж�ȡ ������������ܽ����Ż� ���ܵ��´��� 
 * ����������ڻ��� ���յ������ݵĳ���
 */

vu8 vucUSART1_TxBuffer[USART1_TX_SIZE];
vu8 vucUSART1_RxBuffer[USART1_RX_SIZE];

/*
**********************************************************************
*�������ƣ�void vUSART1_TxDmaNvicConfig(void)
*
*�������ܣ�DMA1_NVIC����ͨ�����ú��� �������ȼ� ������������������ 
*					ȫ�����÷���Ϊ0	NVIC_Configuration(NVIC_PriorityGroup_0);
*					NVIC	��ϸ���������飬��system->system.h�ļ���ע��
*
*ʹ��˵����������ĳ�ʼ�������е���
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:52:42
*�����汾��v1.0
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�zzw
*������ڣ�2017-3-16 17:52:54
**********************************************************************
*/

void vUSART1_TxDmaNvicConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* �鿴��STM32���Ĳο��ֲᡷ�ҵ�DMA1����ͨ�� ��Ӧ��Ӳ����USART1_TX��Ӧͨ��4 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;	
	/* �������ȼ�ʹ��sys.h�еĺ궨�� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMA1_Channel4PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = DMA1_Channel4SubPriority;	
	/* �ò���ָ�����ڳ�Ա NVIC_IRQChannel �ж���� IRQ ͨ����ʹ�ܻ���ʧ�ܡ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

/*
**********************************************************************
*�������ƣ�void vUSART1_NvicConfig(void)
*
*�������ܣ�USART1_NVIC ���ú��� �������ȼ� ������������������ 
*					ȫ�����÷���Ϊ0	NVIC_Configuration(NVIC_PriorityGroup_0);
*					NVIC	��ϸ���������飬��system->sys.h�ļ���ע��
*					��������ʹ�ô��ڿ����жϴ��洮�ڽ���DMA�жϣ�����ʵ�ֽ������ⳤ������
*
*ʹ��˵����������ĳ�ʼ�������е���
*��ڲ�������
*�� �� ֵ����

*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:53:46
*�����汾��v1.0
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�zzw
*������ڣ�2017-3-16 17:54:20
**********************************************************************
*/

void vUSART1_NvicConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
    
    
	
	/* ͨ������Ϊ����1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;    
	/* ����ʹ��ʱ���滻��sys.h�е����ȼ��궨�� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART1_PreemptionPriority;         
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART1_SubPriority;  
	/* ʹ�ܴ����ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	
	NVIC_Init(&NVIC_InitStructure);  
}

/*
**********************************************************************
*�������ƣ�void vUSART1_TxDmaConfig(void)
*
*�������ܣ�����tx��DMA ���ú��� ������ص�DMA���ã����� �ο���STM32���Ĳο��ֲᡷ
*					�Լ���STM32�̼����ֲᡷ�鿴����½� ��ز���
*
*ʹ��˵����������ĳ�ʼ�������е���
*��ڲ�������
*�� �� ֵ����

*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:46:03
*�����汾��v1.0
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�zzw
*������ڣ�2017-3-16 17:54:38
**********************************************************************
*/

void vUSART1_TxDmaConfig(void)
{
	/* DMA�����Ľṹ�� */
	DMA_InitTypeDef DMA_InitStructure;
	/* Enable DMA clock ��ο� -- �鿴ʱ���� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* ���ȹر�DMA1_Channel4 Ȼ��������� */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	/* USART1 ��ӦDMA�� ͨ��1 �� DMA ��ͨ�� x �Ĵ�������ΪĬ��ֵ 
		�������һ�³�ʼ�� �ȽϺ�DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel4);
		
	/* �������� ���ڷ��͵�DMA */
	
	/* ָ���������ַ ���� ��USART1�ĵ�ַ */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1 -> DR);				
	/* �ɼ������ݵ�Ŀ�ĵ��ڴ��ַ ��ʱû���ã������淢�͵�ʱ���������õ� */
	//DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_TX_Buffer;	
	/* DMA���ݵķ���Ϊ�ڴ浽���� */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;		
	/* ������������Ϊ0 ��Ҫ���͵�ʱ�� �ٽ��е��� */
	DMA_InitStructure.DMA_BufferSize = 0;
	/* ���豾��ĵ�ַ�̶����� */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	/* �ڴ��ַ������Ҳ���ǻ������ݵ����ݵ�ַ���� */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	/* �������ݿ������Ϊ8λ */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	
	/* �ڴ�����ݿ��Ϊ8λ Ҳ����1�ֽ� */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					
		
	/*ע�⣺�������ѵ�ָ�� DMA ͨ�����ݴ�������Ϊ�ڴ浽�ڴ�ʱ������ʹ��ѭ������ģʽ��*/
	
	/* ����ģʽ���ó� ���δ���ģʽ */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	
	/* DMAͨ�����ȼ�����Ϊ�����ȼ� ���ж��DMA����ʱ �ٲ� */
	DMA_InitStructure.DMA_Priority = DMA1_USART1_TX_CHANNEL4_PRIORITY;
	/* ��ֹ�ڴ浽�ڴ��ģʽ */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	/* ʹ��DMA1_Channel1��ɴ������ж� */
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);        											
	/* DISABLE DMA channel4 ���ȹر���� DMA���� */
	DMA_Cmd(DMA1_Channel4, DISABLE);	
}

/*
**********************************************************************
*�������ƣ�void vUSART1_RxDmaConfig(void)
*
*�������ܣ�����rx��DMA ���ú��� ������ص�DMA���ã����� �ο���STM32���Ĳο��ֲᡷ
*					�Լ���STM32�̼����ֲᡷ�鿴����½� ��ز���
*
*ʹ��˵����������ĳ�ʼ�������е���
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:46:10
*�����汾��v1.0 
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�zzw
*������ڣ�2017-3-16 17:54:48
**********************************************************************
*/

void vUSART1_RxDmaConfig(void)
{
	//DMA�����Ľṹ��
	DMA_InitTypeDef DMA_InitStructure;
	/* Enable DMA clock ��ο� -- �鿴ʱ���� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*���ȹر����DMA��ͨ��5*/
	DMA_Cmd(DMA1_Channel5, DISABLE);
	/*usart1��ӦDMA�� ͨ��1 �� DMA ��ͨ�� x �Ĵ�������ΪĬ��ֵ ��ʵ���õ��� DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel5);
	
	//�������� ���ڽ��յ�DMA
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1 -> DR);	
	/* �ɼ������ݵ�Ŀ�ĵ��ڴ��ַ */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)vucUSART1_RxBuffer;	
	/* DMA����Ϊ���赽�ڴ� */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;		
	/* ���ݳߴ���ʱ����Ϊ0���������ʵ������ */
	DMA_InitStructure.DMA_BufferSize = 0;		
	/* ���豾��ĵ�ַ�̶����� */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	/* �ڴ��ַ���� */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  		
	/* ��������ݿ��Ϊ8λ ��Ϊ�������õ��� vu8 ADC_ConvertedValue[SAMPLE_NUM][CHANNEL_NUM];  ������ԭ���� �ɼ��������� 8λ�ģ� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		
	/* �ڴ�����ݿ��Ϊ8λ Ҳ����1�ֽ� */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;					
		
	/* ע�⣺��ָ�� DMA ͨ�����ݴ�������Ϊ�ڴ浽�ڴ�ʱ������ʹ��ѭ������ģʽ��*/
	/* ���δ���ģʽ */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;																
	DMA_InitStructure.DMA_Priority = DMA1_USART1_RX_CHANNEL5_PRIORITY;		
	/* ��ֹ�ڴ浽�ڴ��ģʽ */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	/* ʹ��DMA1_Channel1��ɴ������ж� */
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, DISABLE);        											
	/* DISABLE DMA channel5*/
	DMA_Cmd(DMA1_Channel5, DISABLE);
}

/*
**********************************************************************
*�������ƣ�void vUSART1_RX_DMA_Config(void)
*
*�������ܣ���ص�GPIO�ڵ����ú������ο���STM32���Ĳο��ֲᡷ��110ҳ ����GPIO����
*
*ʹ��˵����������ĳ�ʼ�������е���
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:46:15
*�����汾��v1.0 
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ���
*������ڣ���
**********************************************************************
*/

void vUSART1_GpioConfig(void)
{
	/* GPIO�˿����� ����GPIO������ �μ� ��STM32���Ĳο��ֲᡷ 110ҳ */
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ʹ��USART1��GPIOAʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/* USART1_TX   PA.9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* ����������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                            
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
	/* USART1_RX PA.10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	/* �������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

/*
**********************************************************************
*�������ƣ�void vUSART1_DMA1_Init(uint16_t bound)
*
*�������ܣ�������صĴ������� �ο���STM32�̼����ֲᡷUSART�Լ���STM32���Ĳο��ֲᡷ����½�
*
*ʹ��˵������main�����е���
*��ڲ�����bound -������
*�� �� ֵ����
*
*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:46:21
*�����汾��v1.0 
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�zzw
*������ڣ�2017-3-16 17:57:03
**********************************************************************
*/

void vUSART1_Init(uint32_t ulBound)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1��GPIOAʱ��

	vUSART1_NvicConfig();
	/* USART1��TX���ն�DMA��NVIC�����ú��� */
	vUSART1_TxDmaNvicConfig();
	/* USART1��RX���ն�DMA�����ú��� */
	vUSART1_RxDmaConfig();
	/* USART1��TX���ն�DMA�����ú��� */
	vUSART1_TxDmaConfig();
	/* USART1�� GPIO ���ú��� ��������д�ģ� */
	vUSART1_GpioConfig();

	/* ���������ã�һ��Ϊ9600 */
	USART_InitStructure.USART_BaudRate = ulBound;                                
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                     
	USART_InitStructure.USART_Parity = USART_Parity_No;                         
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	            

	USART_Init(USART1, &USART_InitStructure);                        

	/**
	*�ж�����
	*USART_IT_TXE �����ж�   
	*USART_IT_TC ��������ж�  
	*USART_IT_RXNE �����ж�
	*USART_IT_IDLE �����ж�
	*����ʹ�ܵ��ǿ����ж�
	**/
	USART_ITConfig(USART1,USART_IT_TC|USART_IT_RXNE|USART_IT_TXE,DISABLE);  
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);   
	/* Enable the DMA transfer on Rx and Tx action for USART1 */
	USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
	/* ʹ�ܴ��� */
	USART_Cmd(USART1, ENABLE);                                                   
}

/*
***********************************************************************
*�������ƣ�ErrorStatus vUSART1_DmaTxStart(uint8_t * pucBuffer, uint32_t ulSize, uint32_t ulIsNeedCopyData)
*
*�������ܣ�ͨ������1�����ַ���
*
*ʹ��˵������
*��ڲ�����pucBuffer�������ַ���ָ��
					ulSize�������ַ����ĳ���
					ulIsNeedCopyData���Ƿ���Ҫ���ַ������Ƶ��ڲ���������
*�� �� ֵ���Ƿ�������
*
*��ǰ���ߣ�ʯ����
*�������ڣ�2017-3-16 17:46:26
*�����汾��v1.0
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�ʯ����
*������ڣ�2017-3-16 17:57:25
***********************************************************************
*/

ErrorStatus vUSART1_DmaTxStart(uint16_t * pucBuffer, uint32_t ulSize, uint32_t ulIsNeedCopyData)
{
	uint32_t ulIndex = 0;
	
	/* �ж��ǲ�����Ҫ copy ���� */
	if(ulIsNeedCopyData)
	{
		/* ��Ҫ�������ݵ������� */
		
		/* ���ж� DMA �����Ƿ���ɣ�ͨ��DMA �������ݼ����� */
		if(DMA1_Channel4->CNDTR)
		{
			/* �ϴε�û�з������ */
			
			/* �����ε����ݻ᲻�ᳬ�������� */
			if((((uint32_t)vucUSART1_TxBuffer + USART1_TX_SIZE) - DMA1_Channel4->CMAR) > ulSize)
			{
				/* ���ᳬ������������ʼ���� */
				
				/* ���üĴ���ǰ����Ҫ�ȹر� */
				//DMA_Cmd(DMA1_Channel4, DISABLE);
				DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);
				
				/* �ı䷢���׵�ַ */
				/* ���㱾�ε�ַ���� */
				DMA1_Channel4->CMAR += ((ulUsart1SendPreIndex - DMA1_Channel4->CNDTR) - ulUsart1Dma1Channe5Increase);
				/* ��¼���ε�ַ���� */
				ulUsart1Dma1Channe5Increase = (ulUsart1SendPreIndex - DMA1_Channel4->CNDTR);
				
				/* ��¼���ݳ��ȣ����ﲻ��ֱ�Ӹ�ֵ������ֱ�Ӽ��� */
				DMA1_Channel4->CNDTR += ulSize;
				
				/* �ȼ��㣬���ڷ��͵������� */
				ulIndex = ulUsart1SendPreIndex;
				
				/* ͬʱ��¼���η��͵�λ�ã����´�ʹ�� */
				ulUsart1SendPreIndex += ulSize;
				
				/* ���ƻ��������ݵ�FIFO�� */
				while(ulSize--)
				{
					vucUSART1_TxBuffer[ulIndex++] = *pucBuffer++;
				}
		
				/* ʹ��DMA ��ʼ���� */
				//DMA_Cmd(DMA1_Channel4, ENABLE);
				DMA1_Channel4->CCR |= DMA_CCR1_EN;
				
				/* ���ز����ɹ� */
				return SUCCESS;     
			}else
			{
				/* �����ˣ�ֱ�ӷ����쳣 */
				return ERROR;
			}
		}else
		{
			/* �ϴεķ�����ɣ�ֱ�ӿ�ʼ�µķ��� */
			
			/* �Ƚ��а�ȫ�жϣ����᲻�ᳬ�������� */
			if(ulSize < USART1_TX_SIZE)
			{
				/* ���üĴ���ǰ����Ҫ�ȹر� */
				//DMA_Cmd(DMA1_Channel4, DISABLE);
				DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);
				
				/* ��¼���ݳ��� */
				DMA1_Channel4->CNDTR = ulSize;
				
				/* ��¼���η��͵��������е�λ�ã����´��ж�ʹ�� */
				ulUsart1SendPreIndex = ulSize;
				
				/* �����ַ���� */
				ulUsart1Dma1Channe5Increase = 0;   
				 
				/* ��0��ַ��ʼ���� */
				ulIndex = 0;
				
				/* ���ƻ��������ݵ�FIFO�� */
				while(ulSize--)
				{
					vucUSART1_TxBuffer[ulIndex++] = *pucBuffer++;
				}
				
				/* �ı� DMA �׵�ַ */
				DMA1_Channel4->CMAR = (uint32_t)vucUSART1_TxBuffer;
				
				/* ʹ��DMA ��ʼ���� */
				//DMA_Cmd(DMA1_Channel4, ENABLE);
				DMA1_Channel4->CCR |= DMA_CCR1_EN;
			
				/* ���ز����ɹ� */
				return SUCCESS;
			}else
			{
				/* �������ݳ���̫�������ش��� */
				return ERROR;
			}
		}
	}else
	{
		/* ���ж� DMA �����Ƿ���ɣ�ͨ��DMA �������ݼ����� */
		if(DMA1_Channel4->CNDTR)
		{
			/* �ϴε�û�з������ */
			return ERROR; 
		}else
		{
			/* ����Ҫ�������ݵ������� */
			
			/* ���üĴ���ǰ����Ҫ�ȹر� */
			//DMA_Cmd(DMA1_Channel4, DISABLE);
			DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);
			
			/* ע�����ﲻ�ܵ��ÿ⺯��ֱ�ӳ�ʼ���ˣ���������������ø���û�ˣ�ֱ�����üĴ������� */
			DMA1_Channel4 -> CNDTR = ulSize;
			
			/* �����ж��ǲ�����Ҫ */
			DMA1_Channel4 -> CMAR = (uint32_t)pucBuffer;
			
			/* ʹ��DMA ��ʼ���� */
			//DMA_Cmd(DMA1_Channel4, ENABLE);
			DMA1_Channel4->CCR |= DMA_CCR1_EN;
			
			/* ���ز����ɹ� */
			return SUCCESS;
		}
	}
}

/*
**********************************************************************
*�������ƣ�void vUSART1_DmaRxStart(void)
*
*�������ܣ���Ϊ�ǵ��δ���ģʽ��������Ҫÿ�η��Ͷ�ʹ�ܽ���DMAȻ��ر� 
*
*ʹ��˵����������ĳ�ʼ�������е���
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:46:39
*�����汾��v1.0 
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�zzw
*������ڣ�2017-3-16 18:00:57
**********************************************************************
*/

void vUSART1_DmaRxStart(void)
{
	DMA_Cmd(DMA1_Channel5, DISABLE);
	/* ע�����ﲻ�ܵ��ÿ⺯��ֱ�ӳ�ʼ���ˣ���������������ø���û�ˣ�ֱ�����üĴ������� */
	DMA1_Channel5 -> CNDTR = USART1_RX_SIZE;
	DMA1_Channel5 -> CMAR = (uint32_t)vucUSART1_RxBuffer;
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

/*
**********************************************************************
*�������ƣ�void DMA1_Channel4_IRQHandler(void)
*
*�������ܣ�DMA����ͨ��4�жϷ�����
*
*ʹ��˵����������ĳ�ʼ�������е���
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:46:44
*�����汾��v1.0 
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�zzw
*������ڣ�2017-3-16 18:01:09
**********************************************************************
*/

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4))
	{
		DMA_ClearITPendingBit(DMA1_IT_GL4); 																			
		ulUsart1TxOkFlag = 1;
		DMA_Cmd(DMA1_Channel4, DISABLE);
	}
}

/*
**********************************************************************
*�������ƣ�USART1_IRQHandler(void)
*
*�������ܣ�ʹ�ô��ڿ����жϣ�����ȷ�����ⳤ�����ݽ������
*����˵����Ӧ����Ҫ���������Ժ��״ν�����У��Ż���������жϡ�
*					USART_ClearITPendingBit(USART1,USART_IT_IDLE);
*					ע�⣺���ﲻ����ô���ÿ⺯�����жϱ�־ goto���ײ㿴��������ע���
*   			- PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun 
*    			 error) and IDLE (Idle line detected) pending bits are cleared by 
*     		software sequence: a read operation to USART_SR register 
*    			(USART_GetITStatus()) followed by a read operation to USART_DR register 
*     		(USART_ReceiveData()).
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�YJ_������
*�������ڣ�2017-3-16 17:46:50
*�����汾��v1.0 
*�޸�ԭ�򣺴����ļ�
*
*ȡ���汾����
*ԭ �� �ߣ�zzw
*������ڣ�2017-3-16 18:01:20
**********************************************************************
*/

void USART1_IRQHandler(void)
{
	/* ���߿��б�־λ */
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		/* ��USART_IT_IDLE��־ */
		USART1 -> SR;  
    USART1 -> DR;  
		ulUsart1RecvRealSize = DMA_GetCurrDataCounter(DMA1_Channel5);
		ulUsart1RecvRealSize = USART1_RX_SIZE - ulUsart1RecvRealSize;
		ulUsart1RxOkFlag = 1;
		/* ����ģʽ ÿ�ν�����ɶ�Ҫֹͣ */
		DMA_Cmd(DMA1_Channel5, DISABLE);
	}
}

/* end of file cppyright reserve by team of yanjun ,More information please browse www.yanjun.tech */
