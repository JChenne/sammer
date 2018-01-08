/*
***********************************************************************
* Copyright (c) 2016 燕骏zzw
* All rights reserved.More information please browse www.yanjun.tech
*
* 其他声明：
*						对代码中产生的bug，本人不承担任何直接或间接的损害赔偿
*						若使用本代码即视为默认本条款。若发现bug，请将详细信息
*						发送下面链接至评论区，谢谢！
*						http://www.yanjun.tech/forum.php?gid=87
*
* 文件名称：soft_iic.c soft_iic.h
* 文件摘要：软件模拟的iic通信协议代码
*						
* 当前版本：v1.0
* 当前作者：朱子文
* 完成日期：2017-6-2 21:02:17
* 改动说明：修改命名为自己的
*
* 取代版本：v1.0
* 原 作 者：朱子文
* 完成日期：2017-6-2 21:02:20
***********************************************************************
*/

#include "soft_iic.h"

/*
**********************************************************************
*函数名称：void vI2C_Init(void)
*
*函数功能：软件IIC初始化，初始化两个IO口作为软件模拟IIC的SCL,SDA
*注    意：注意了软件IIC，和硬件IIC，不能同时使用
*
*使用说明：直接在需要的时候 调用一下即可
*入口参数：无
*返 回 值：无
*
*当前作者：朱子文
*创建日期：2017-6-2 21:05:50
*函数版本：v1.0 
*修改原因：无
*
*取代版本：v1.0
*原 作 者：朱子文
*完成日期：2017-6-2 21:05:53
**********************************************************************
*/

void vI2C_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_IIC_SCL|RCC_IIC_SDA, ENABLE); 
						
	/* 初始化IO口 PB10 作为 软件模拟IIC 的 IIC_SCL */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IIC_SCL;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIO_PORT_IIC_SCL, &GPIO_InitStructure); 		
	
	/* 初始化IO口 PB11 作为 软件模拟IIC 的 IIC_SDA */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IIC_SDA;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIO_PORT_IIC_SDA, &GPIO_InitStructure); 	

	/* 默认拉高IIC总线 */
	GPIO_SetBits(GPIO_PORT_IIC_SDA, GPIO_PIN_IIC_SCL|GPIO_PIN_IIC_SDA);
}

/*
**********************************************************************
*函数名称：void vIIC_Start(void)
*
*函数功能：软件II产生起始
*注    意：无
*
*使用说明：直接在需要的时候 调用一下即可
*入口参数：无
*返 回 值：无
*
*当前作者：朱子文
*创建日期：2017-6-2 21:06:37
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：朱子文
*完成日期：2017-6-2 21:06:40
**********************************************************************
*/

void vI2C_Start(void)
{
	/* IIC_SDA 方向选择输出 */
	I2C_SDA_OUT();          

	/* 先拉高IIC总线 */
	I2C_SDA = 1;	  	  
	I2C_SCL = 1;
	/* 延时 4US */
	vI2CDelay();
	/* 在SCL为高电平的时候 SDA 产生一个下降沿 */
 	I2C_SDA = 0;         
	/* 延时 4US */
	vI2CDelay();
	/* 拉低SCL 准备进行发送数据 */
	/* 每一个函数出去之前，都保证SCL是低电平 */
	I2C_SCL = 0;          
}	

/*
**********************************************************************
*函数名称：void vI2C_Stop(void)
*
*函数功能：软件II产生停止位的函数
*注    意：无
*
*使用说明：直接在需要的时候 调用一下即可
*入口参数：无
*返 回 值：无
*
*当前作者：zzw
*创建日期：2016.12.27
*函数版本：v1.0 
*修改原因：无
*
*取代版本：wu 
*原 作 者：无
*完成日期：无
**********************************************************************
*/
void vI2C_Stop(void)
{
	/* IIC_SDA 方向选择输出 */
	I2C_SDA_OUT();      
	
	/* 先拉低IIC总线 */
	I2C_SCL = 0;
	I2C_SDA = 0; 
	/* 延时 4US */
 	vI2CDelay();
	/* 在SCL为高电平的时候 SDA 产生一个上升沿 */
	I2C_SCL = 1; 
	/* 延时 4US */
	vI2CDelay();
	/* SDA产生上升沿 */
	I2C_SDA = 1;  
	/* 软件延时 */
	vI2CDelay();			
	/* 这个函数把IIC时序结束了所以SCL是高电平 */
}

/*
**********************************************************************
*函数名称：ErrorStatus xI2C_WaitAck(uint32_t delayTimes)
*
*函数功能：等待IIC器件回复ACK
*注    意：无
*
*使用说明：等待ACK，或者发生超时错误 直接在需要的时候 调用一下即可
*入口参数：delayTimes:延时等待时间
*返 回 值：是否有ACK
*
*当前作者：zzw
*创建日期：2017-6-2 21:21:49
*函数版本：v1.0 
*修改原因：命名改为自己习惯格式
*
*取代版本：无
*原 作 者：朱子文
*完成日期：2017-6-2 21:21:52
**********************************************************************
*/

ErrorStatus xI2C_WaitAck(uint32_t ulDelayTimes)
{
	/* IIC_SDA 方向选择输入 */
	I2C_SDA_IN();    
	/*  */
	I2C_SDA = 1;             
	vI2CDelay();	 
	/* 拉高SCL 开始读取SDA数据 */
	I2C_SCL = 1;
	vI2CDelay();
	/* 读取SDA是否有ACK */
	while(I2C_READ_SDA)         
	{
		if(ulDelayTimes-- == 0)
		{
			/* 发生超时错误 */
			vI2C_Stop();
			return ERROR;  
		}
	}
	/* 每一个函数出去之前，都保证SCL是低电平 */
	I2C_SCL = 0;              	   
	return SUCCESS;  
} 

/*
**********************************************************************
*函数名称：void vI2C_Ack(void)
*
*函数功能：等待IIC器件回复ACK
*注    意：无
*
*使用说明：无
*入口参数：无
*返 回 值：无
*
*当前作者：朱子文
*创建日期：2017-6-2 21:08:07
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：朱子文
*完成日期：2017-6-2 21:08:17
**********************************************************************
*/

void vI2C_Ack(void)
{
	/* 拉低SCL准备 向SDA线上发送数据 */
	I2C_SCL = 0;
	/* I2C_SDA_OUT 这个改变SDA方向的语句不能修改在程序中的顺序 */
	I2C_SDA_OUT();
	/* 在SDA上发送 ACK */
	I2C_SDA = 0;
	/* 软件延时 */
	vI2CDelay();
	/* 拉高SCL 让别人读取 SDA 上的数据 */
	I2C_SCL = 1;
	/* 软件延时 */
	vI2CDelay();
	/* 每一个函数出去之前，都保证SCL是低电平 */
	I2C_SCL = 0;
}

/*
**********************************************************************
*函数名称：void vI2C_NAck(void)
*
*函数功能：该函数用于产生Nack应答信号
*注    意：无
*
*使用说明：I2C_SDA_OUT 这个改变SDA方向的语句不能修改在程序中的顺序
*入口参数：无
*返 回 值：无
*
*当前作者：朱子文
*创建日期：2017-6-2 21:08:54
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：朱子文
*完成日期：2017-6-2 21:09:07
**********************************************************************
*/

void vI2C_NAck(void)
{
	/* 拉低SCL准备 向SDA线上发送数据 */
	I2C_SCL = 0;
	/* I2C_SDA_OUT 这个改变SDA方向的语句不能修改在程序中的顺序 */
	I2C_SDA_OUT();
	/* 在SDA上发送 NACK */
	I2C_SDA = 1;
	/* 软件延时 */
	vI2CDelay();
	/* 拉高SCL 让别人读取 SDA 上的数据 */
	I2C_SCL = 1;
	/* 软件延时 */
	vI2CDelay();
	/* 每一个函数出去之前，都保证SCL是低电平 */
	I2C_SCL = 0;
}

/*
**********************************************************************
*函数名称：void vI2C_SendByte(uint8_t ucData)
*
*函数功能：在IIC总线上发送一个字节
*注    意：无
*
*使用说明：无
*入口参数：data:要发送的数据
*返 回 值：无
*
*当前作者：朱子文
*创建日期：2017-6-2 21:09:47
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：无
*完成日期：无
**********************************************************************
*/

void vI2C_SendByte(uint8_t ucData)
{   
	uint8_t ucMask = 0;                              
	/* 选择SDA方向为输出 */
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
*函数名称：uint8_t ucI2C_RecvByte(uint8_t ucIsNeedAck)
*
*函数功能：在IIC总线上接收一个字节
*注    意：无
*
*使用说明：无
*入口参数：isNeedAck:要发送ACK还是NACK
*返 回 值：读取到的数据
*
*当前作者：朱子文
*创建日期：2017-6-2 21:11:08
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：朱子文
*完成日期：2017-6-2 21:11:18
**********************************************************************
*/

uint8_t ucI2C_RecvByte(uint8_t ucIsNeedAck)
{
	uint8_t ucMask = 0;
	uint8_t ucReceive = 0;
	/* IIC_SDA 方向选择输 */
	I2C_SDA_IN();                              
	/* 先接收高字节 */
	for(ucMask = 0x80; ucMask != 0; ucMask >>= 1)
	{
		/* 给出SCL时钟 */
		I2C_SCL=0; 
		vI2CDelay();
		I2C_SCL=1;
		/* 读取SDA线上的数据 */
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
*函数名称：ErrorStatus xI2C_Write(uint8_t ucI2cSlaveAddr, uint8_t ucRegAddr, 
*																	uint8_t * pucBuffer, uint32_t ulLength)
*
*
*函数功能：用IIC总线写数据
*注    意：无
*
*使用说明：无
*入口参数：
*					IicSlaveAddr:IIC器件地址
*					regAddr			:寄存器地址
*					buffer			:缓冲区指针
*					length			:发送数据长度
*返 回 值：是否错误
*
*当前作者：zzw
*创建日期：2016.12.27
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：无
*完成日期：无
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
*函数名称：ErrorStatus xI2C_Read(uint8_t ucI2cSlaveAddr, uint8_t ucRegAddr, 
*																uint8_t * pucBuffer, uint32_t ulLength)
*
*函数功能：用IIC总线读取数据
*注    意：无
*
*使用说明：无
*入口参数：IicSlaveAddr:IIC器件地址
*					regAddr			:寄存器地址
*					buffer			:缓冲区指针
*					length			:发送数据长度
*返 回 值：是否错误
*
*当前作者：朱子文
*创建日期：2017-6-2 21:26:30
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：朱子文
*完成日期：2017-6-2 21:26:33
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

	/* 开始接受数据 */
	/* 这样做是因为最后一个读取的数据要给出NACK */
	ulLength--;
	while(ulLength--)
	{
		/* 不是最后一个给出ACK */
		*pucBuffer++ = ucI2C_RecvByte(1);
	}

	/* 最后一个给出NACK */
	*pucBuffer = ucI2C_RecvByte(0);

	vI2C_Stop();	
	return SUCCESS;
}

/*
**********************************************************************
*函数名称：ErrorStatus vI2C_CheckDevice(uint8_t IicSlaveAddr)
*
*函数功能：检查IIC总线上是否有相应地址的器件
*注    意：无
*
*使用说明：无
*入口参数：IicSlaveAddr:IIC器件地址
*返 回 值：是否成功，成功返回 SUCCESS
*
*当前作者：zzw
*创建日期：2017-6-2 21:26:21
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：朱子文
*完成日期：2017-6-2 22:26:17
**********************************************************************
*/

ErrorStatus xI2C_CheckDevice(uint8_t ucI2cSlaveAddr)
{
  ErrorStatus ack;
	
	/* 产生起始位 */
	vI2C_Start();
	/* 发送IIC器件地址 方向上选择 写 ， 0 代表写 1 代表 读  */
	vI2C_SendByte(ucI2cSlaveAddr);
	/* 等待ACK */
	ack = xI2C_WaitAck(ANALOG_IIC_WAIT_ACK_TIMES);
	/* 产生停止位 */
	vI2C_Stop();
	
	return ack;
}

/* end of file cppyright reserve by team of yanjun More information please browse www.yanjun.tech */
