#ifndef _SOFT_IIC_H
#define _SOFT_IIC_H

#include "system.h"
#include "systick.h"
/**
*软件模拟IIC端口的宏定义
*
*/

/* IIC_SCL */
#define GPIO_PORT_IIC_SCL			GPIOB
#define GPIO_PIN_IIC_SCL			GPIO_Pin_10
#define RCC_IIC_SCL						RCC_APB2Periph_GPIOB
/* IIC_SDA */
#define GPIO_PORT_IIC_SDA			GPIOB
#define GPIO_PIN_IIC_SDA			GPIO_Pin_11
#define RCC_IIC_SDA						RCC_APB2Periph_GPIOB


#define vI2CDelay() {__NOP();__NOP();__NOP();__NOP();}
/***
*因为软件IIC，要改变IO 口的读写方向，这里给出定义,要修改IO口的时候，这个地方也要同时修改
*
*/

#define I2C_SDA_IN()  	{GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH |= 8<<12;}
#define I2C_SDA_OUT() 	{GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH |= 3<<12;}

/**
*定义IIC的IO 口直接操作的位带操作宏定义,要修改IO口的时候，这个地方也要同时修改
*
*/

/* SCL写 */
#define I2C_SCL    						PBout(10) 
/* SDA写 */
#define I2C_SDA    						PBout(11) 
/* SDA读 */
#define I2C_READ_SDA   				PBin(11)  

/* 在IIC读取ACK的时候的等待次数 */
#define ANALOG_IIC_WAIT_ACK_TIMES			((uint32_t)250)

void vI2C_Init(void);
ErrorStatus xI2C_Write(uint8_t ucI2cSlaveAddr, uint8_t ucRegAddr, uint8_t * pucBuffer, uint32_t ulLength);
ErrorStatus xI2C_Read(uint8_t ucI2cSlaveAddr, uint8_t ucRegAddr, uint8_t * pucBuffer, uint32_t ulLength);
ErrorStatus xI2C_CheckDevice(uint8_t ucI2cSlaveAddr);

#endif

