/*
***********************************************************************
* Copyright (c) 2016 �࿥zzw
* All rights reserved.More information please browse www.yanjun.tech
*
* ����������
*						�Դ����в�����bug�����˲��е��κ�ֱ�ӻ��ӵ����⳥
*						��ʹ�ñ����뼴��ΪĬ�ϱ����������bug���뽫��ϸ��Ϣ
*						����������������������лл��
*						http://www.yanjun.tech/forum.php?gid=87
*
* �ļ����ƣ�soft_iic.c soft_iic.h
* �ļ�ժҪ�����ģ���iicͨ��Э�����
*						
* ��ǰ�汾��v1.0
* ��ǰ���ߣ�������
* ������ڣ�2017-6-2 21:02:17
* �Ķ�˵�����޸�����Ϊ�Լ���
*
* ȡ���汾��v1.0
* ԭ �� �ߣ�������
* ������ڣ�2017-6-2 21:02:20
***********************************************************************
*/

#include "soft_iic.h"

/*
**********************************************************************
*�������ƣ�void vI2C_Init(void)
*
*�������ܣ����IIC��ʼ������ʼ������IO����Ϊ���ģ��IIC��SCL,SDA
*ע    �⣺ע�������IIC����Ӳ��IIC������ͬʱʹ��
*
*ʹ��˵����ֱ������Ҫ��ʱ�� ����һ�¼���
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�������
*�������ڣ�2017-6-2 21:05:50
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾��v1.0
*ԭ �� �ߣ�������
*������ڣ�2017-6-2 21:05:53
**********************************************************************
*/

void vI2C_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_IIC_SCL|RCC_IIC_SDA, ENABLE); 
						
	/* ��ʼ��IO�� PB10 ��Ϊ ���ģ��IIC �� IIC_SCL */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IIC_SCL;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIO_PORT_IIC_SCL, &GPIO_InitStructure); 		
	
	/* ��ʼ��IO�� PB11 ��Ϊ ���ģ��IIC �� IIC_SDA */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IIC_SDA;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIO_PORT_IIC_SDA, &GPIO_InitStructure); 	

	/* Ĭ������IIC���� */
	GPIO_SetBits(GPIO_PORT_IIC_SDA, GPIO_PIN_IIC_SCL|GPIO_PIN_IIC_SDA);
}

/*
**********************************************************************
*�������ƣ�void vIIC_Start(void)
*
*�������ܣ����II������ʼ
*ע    �⣺��
*
*ʹ��˵����ֱ������Ҫ��ʱ�� ����һ�¼���
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�������
*�������ڣ�2017-6-2 21:06:37
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ�������
*������ڣ�2017-6-2 21:06:40
**********************************************************************
*/

void vI2C_Start(void)
{
	/* IIC_SDA ����ѡ����� */
	I2C_SDA_OUT();          

	/* ������IIC���� */
	I2C_SDA = 1;	  	  
	I2C_SCL = 1;
	/* ��ʱ 4US */
	vI2CDelay();
	/* ��SCLΪ�ߵ�ƽ��ʱ�� SDA ����һ���½��� */
 	I2C_SDA = 0;         
	/* ��ʱ 4US */
	vI2CDelay();
	/* ����SCL ׼�����з������� */
	/* ÿһ��������ȥ֮ǰ������֤SCL�ǵ͵�ƽ */
	I2C_SCL = 0;          
}	

/*
**********************************************************************
*�������ƣ�void vI2C_Stop(void)
*
*�������ܣ����II����ֹͣλ�ĺ���
*ע    �⣺��
*
*ʹ��˵����ֱ������Ҫ��ʱ�� ����һ�¼���
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�zzw
*�������ڣ�2016.12.27
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾��wu 
*ԭ �� �ߣ���
*������ڣ���
**********************************************************************
*/
void vI2C_Stop(void)
{
	/* IIC_SDA ����ѡ����� */
	I2C_SDA_OUT();      
	
	/* ������IIC���� */
	I2C_SCL = 0;
	I2C_SDA = 0; 
	/* ��ʱ 4US */
 	vI2CDelay();
	/* ��SCLΪ�ߵ�ƽ��ʱ�� SDA ����һ�������� */
	I2C_SCL = 1; 
	/* ��ʱ 4US */
	vI2CDelay();
	/* SDA���������� */
	I2C_SDA = 1;  
	/* �����ʱ */
	vI2CDelay();			
	/* ���������IICʱ�����������SCL�Ǹߵ�ƽ */
}

/*
**********************************************************************
*�������ƣ�ErrorStatus xI2C_WaitAck(uint32_t delayTimes)
*
*�������ܣ��ȴ�IIC�����ظ�ACK
*ע    �⣺��
*
*ʹ��˵�����ȴ�ACK�����߷�����ʱ���� ֱ������Ҫ��ʱ�� ����һ�¼���
*��ڲ�����delayTimes:��ʱ�ȴ�ʱ��
*�� �� ֵ���Ƿ���ACK
*
*��ǰ���ߣ�zzw
*�������ڣ�2017-6-2 21:21:49
*�����汾��v1.0 
*�޸�ԭ��������Ϊ�Լ�ϰ�߸�ʽ
*
*ȡ���汾����
*ԭ �� �ߣ�������
*������ڣ�2017-6-2 21:21:52
**********************************************************************
*/

ErrorStatus xI2C_WaitAck(uint32_t ulDelayTimes)
{
	/* IIC_SDA ����ѡ������ */
	I2C_SDA_IN();    
	/*  */
	I2C_SDA = 1;             
	vI2CDelay();	 
	/* ����SCL ��ʼ��ȡSDA���� */
	I2C_SCL = 1;
	vI2CDelay();
	/* ��ȡSDA�Ƿ���ACK */
	while(I2C_READ_SDA)         
	{
		if(ulDelayTimes-- == 0)
		{
			/* ������ʱ���� */
			vI2C_Stop();
			return ERROR;  
		}
	}
	/* ÿһ��������ȥ֮ǰ������֤SCL�ǵ͵�ƽ */
	I2C_SCL = 0;              	   
	return SUCCESS;  
} 

/*
**********************************************************************
*�������ƣ�void vI2C_Ack(void)
*
*�������ܣ��ȴ�IIC�����ظ�ACK
*ע    �⣺��
*
*ʹ��˵������
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�������
*�������ڣ�2017-6-2 21:08:07
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ�������
*������ڣ�2017-6-2 21:08:17
**********************************************************************
*/

void vI2C_Ack(void)
{
	/* ����SCL׼�� ��SDA���Ϸ������� */
	I2C_SCL = 0;
	/* I2C_SDA_OUT ����ı�SDA�������䲻���޸��ڳ����е�˳�� */
	I2C_SDA_OUT();
	/* ��SDA�Ϸ��� ACK */
	I2C_SDA = 0;
	/* �����ʱ */
	vI2CDelay();
	/* ����SCL �ñ��˶�ȡ SDA �ϵ����� */
	I2C_SCL = 1;
	/* �����ʱ */
	vI2CDelay();
	/* ÿһ��������ȥ֮ǰ������֤SCL�ǵ͵�ƽ */
	I2C_SCL = 0;
}

/*
**********************************************************************
*�������ƣ�void vI2C_NAck(void)
*
*�������ܣ��ú������ڲ���NackӦ���ź�
*ע    �⣺��
*
*ʹ��˵����I2C_SDA_OUT ����ı�SDA�������䲻���޸��ڳ����е�˳��
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ�������
*�������ڣ�2017-6-2 21:08:54
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ�������
*������ڣ�2017-6-2 21:09:07
**********************************************************************
*/

void vI2C_NAck(void)
{
	/* ����SCL׼�� ��SDA���Ϸ������� */
	I2C_SCL = 0;
	/* I2C_SDA_OUT ����ı�SDA�������䲻���޸��ڳ����е�˳�� */
	I2C_SDA_OUT();
	/* ��SDA�Ϸ��� NACK */
	I2C_SDA = 1;
	/* �����ʱ */
	vI2CDelay();
	/* ����SCL �ñ��˶�ȡ SDA �ϵ����� */
	I2C_SCL = 1;
	/* �����ʱ */
	vI2CDelay();
	/* ÿһ��������ȥ֮ǰ������֤SCL�ǵ͵�ƽ */
	I2C_SCL = 0;
}

/*
**********************************************************************
*�������ƣ�void vI2C_SendByte(uint8_t ucData)
*
*�������ܣ���IIC�����Ϸ���һ���ֽ�
*ע    �⣺��
*
*ʹ��˵������
*��ڲ�����data:Ҫ���͵�����
*�� �� ֵ����
*
*��ǰ���ߣ�������
*�������ڣ�2017-6-2 21:09:47
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ���
*������ڣ���
**********************************************************************
*/

void vI2C_SendByte(uint8_t ucData)
{   
	uint8_t ucMask = 0;                              
	/* ѡ��SDA����Ϊ��� */
	I2C_SDA_OUT();  
							  
	for(ucMask = 0x80; ucMask != 0; ucMask >>= 1)       
	{
		if((ucMask & ucData) == 0)              
		{
			I2C_SDA = 0;
		}else
		{
			I2C_SDA = 1;
		}
		vI2CDelay();
		I2C_SCL = 1;                       
		vI2CDelay();
		I2C_SCL = 0;                       
		vI2CDelay();
	}
}

/*
**********************************************************************
*�������ƣ�uint8_t ucI2C_RecvByte(uint8_t ucIsNeedAck)
*
*�������ܣ���IIC�����Ͻ���һ���ֽ�
*ע    �⣺��
*
*ʹ��˵������
*��ڲ�����isNeedAck:Ҫ����ACK����NACK
*�� �� ֵ����ȡ��������
*
*��ǰ���ߣ�������
*�������ڣ�2017-6-2 21:11:08
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ�������
*������ڣ�2017-6-2 21:11:18
**********************************************************************
*/

uint8_t ucI2C_RecvByte(uint8_t ucIsNeedAck)
{
	uint8_t ucMask = 0;
	uint8_t ucReceive = 0;
	/* IIC_SDA ����ѡ���� */
	I2C_SDA_IN();                              
	/* �Ƚ��ո��ֽ� */
	for(ucMask = 0x80; ucMask != 0; ucMask >>= 1)
	{
		/* ����SCLʱ�� */
		I2C_SCL=0; 
		vI2CDelay();
		I2C_SCL=1;
		/* ��ȡSDA���ϵ����� */
		if(I2C_READ_SDA)
		{
			ucReceive |= ucMask;
		}			
		vI2CDelay(); 
	}					 
	if (ucIsNeedAck)
	{
		vI2C_Ack();                         
	}else
	{
		vI2C_NAck();
	}
	
	return ucReceive;
}

/*
**********************************************************************
*�������ƣ�ErrorStatus xI2C_Write(uint8_t ucI2cSlaveAddr, uint8_t ucRegAddr, 
*																	uint8_t * pucBuffer, uint32_t ulLength)
*
*
*�������ܣ���IIC����д����
*ע    �⣺��
*
*ʹ��˵������
*��ڲ�����
*					IicSlaveAddr:IIC������ַ
*					regAddr			:�Ĵ�����ַ
*					buffer			:������ָ��
*					length			:�������ݳ���
*�� �� ֵ���Ƿ����
*
*��ǰ���ߣ�zzw
*�������ڣ�2016.12.27
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ���
*������ڣ���
**********************************************************************
*/

ErrorStatus xI2C_Write(uint8_t ucI2cSlaveAddr, uint8_t ucRegAddr, uint8_t * pucBuffer, uint32_t ulLength)
{
	vI2C_Start();
	vI2C_SendByte(ucI2cSlaveAddr);
	if(xI2C_WaitAck(ANALOG_IIC_WAIT_ACK_TIMES) == ERROR)
	{
		return ERROR;
	}
	vI2C_SendByte(ucRegAddr);
	if(xI2C_WaitAck(ANALOG_IIC_WAIT_ACK_TIMES) == ERROR)
	{
		return ERROR;
	} 
	
	while(ulLength--)
	{
		vI2C_SendByte(*pucBuffer++);
		if(xI2C_WaitAck(ANALOG_IIC_WAIT_ACK_TIMES) == ERROR)
		{
			return ERROR;
		} 
	}
	
	vI2C_Stop();
	return SUCCESS;
}

/*
**********************************************************************
*�������ƣ�ErrorStatus xI2C_Read(uint8_t ucI2cSlaveAddr, uint8_t ucRegAddr, 
*																uint8_t * pucBuffer, uint32_t ulLength)
*
*�������ܣ���IIC���߶�ȡ����
*ע    �⣺��
*
*ʹ��˵������
*��ڲ�����IicSlaveAddr:IIC������ַ
*					regAddr			:�Ĵ�����ַ
*					buffer			:������ָ��
*					length			:�������ݳ���
*�� �� ֵ���Ƿ����
*
*��ǰ���ߣ�������
*�������ڣ�2017-6-2 21:26:30
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ�������
*������ڣ�2017-6-2 21:26:33
**********************************************************************
*/

ErrorStatus xI2C_Read(uint8_t ucI2cSlaveAddr, uint8_t ucRegAddr, uint8_t * pucBuffer, uint32_t ulLength)
{
	vI2C_Start();

	vI2C_SendByte(ucI2cSlaveAddr); 

	if(xI2C_WaitAck(ANALOG_IIC_WAIT_ACK_TIMES) == ERROR)
	{
		return ERROR;
	}
	vI2C_SendByte(ucRegAddr);       
	if(xI2C_WaitAck(ANALOG_IIC_WAIT_ACK_TIMES) == ERROR)
	{
		return ERROR;
	}
	vI2C_Start();
	vI2C_SendByte(ucI2cSlaveAddr+1);
	if(xI2C_WaitAck(ANALOG_IIC_WAIT_ACK_TIMES) == ERROR)
	{
		return ERROR;
	}

	/* ��ʼ�������� */
	/* ����������Ϊ���һ����ȡ������Ҫ����NACK */
	ulLength--;
	while(ulLength--)
	{
		/* �������һ������ACK */
		*pucBuffer++ = ucI2C_RecvByte(1);
	}

	/* ���һ������NACK */
	*pucBuffer = ucI2C_RecvByte(0);

	vI2C_Stop();	
	return SUCCESS;
}

/*
**********************************************************************
*�������ƣ�ErrorStatus vI2C_CheckDevice(uint8_t IicSlaveAddr)
*
*�������ܣ����IIC�������Ƿ�����Ӧ��ַ������
*ע    �⣺��
*
*ʹ��˵������
*��ڲ�����IicSlaveAddr:IIC������ַ
*�� �� ֵ���Ƿ�ɹ����ɹ����� SUCCESS
*
*��ǰ���ߣ�zzw
*�������ڣ�2017-6-2 21:26:21
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ�������
*������ڣ�2017-6-2 22:26:17
**********************************************************************
*/

ErrorStatus xI2C_CheckDevice(uint8_t ucI2cSlaveAddr)
{
  ErrorStatus ack;
	
	/* ������ʼλ */
	vI2C_Start();
	/* ����IIC������ַ ������ѡ�� д �� 0 ����д 1 ���� ��  */
	vI2C_SendByte(ucI2cSlaveAddr);
	/* �ȴ�ACK */
	ack = xI2C_WaitAck(ANALOG_IIC_WAIT_ACK_TIMES);
	/* ����ֹͣλ */
	vI2C_Stop();
	
	return ack;
}

/* end of file cppyright reserve by team of yanjun More information please browse www.yanjun.tech */
