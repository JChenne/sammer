/*
**************************************************
*Copyright(c) 2017 燕骏 LHX
*ALL rights reserved.More information please browse www.yanjun.tech
*
*其他声明：对代码中产生的bug，本人不承担任何直接或间接的损害赔偿
*                           若使用本代码即视为默认本条款。若发现bug,请将详细信息
*                           发送至下面邮箱谢谢！
*                           1027259561@qq.com
*
*文件名：bsp_i2c.h
**文件摘要：
*
*当前版本：v1.0
*当前作者：LHX
*完成日期：2017.06.13
***************************************************
*/

#include "bsp_i2c.h"
#include "bsp_usart.h"

static __IO uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;

static uint32_t usI2C_TIMEOUT_UserCallback(uint8_t errorCode);

uint16_t TARGET_ADDRESS;

/*
***********************************************************************
*函数名称：vI2C_GPIO_Config(void)
*
*函数功能：I2C_GPIO初始化
*
*使用说明：无
*入口参数：无
*返 回 值：无
*
*当前作者：李红禧
*创建日期：2017-6-13
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：李红禧
*完成日期：2017-6-13
***********************************************************************
*/
static void vI2C_GPIO_Config(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    /* I2C时钟 */
    I2C_APBxClock_FUN (I2C_CLK, ENABLE);
    /* GPIO时钟 */
    I2C_GPIO_APBxClock_FUN (I2C_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
}

/*
***********************************************************************
*函数名称：vI2C_Mode_Config(void)
*
*函数功能：I2C初始化配置
*
*使用说明：无
*入口参数：无
*返 回 值：无
*
*当前作者：李红禧
*创建日期：2017-6-13
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：李红禧
*完成日期：2017-6-13
***********************************************************************
*/
static void vI2C_Mode_Config(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2Cx_OWN_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
    I2C_Init(I2Cx, &I2C_InitStructure);
    /*    打开I2C    */
    I2C_Cmd(I2Cx, ENABLE);
}

/*
***********************************************************************
*函数名称：vI2C_INIT(void)
*
*函数功能：初始化的调用, 目标地址
*
*使用说明：无
*入口参数：无
*返 回 值：无
*
*当前作者：李红禧
*创建日期：2017-6-13
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：李红禧
*完成日期：2017-6-13
***********************************************************************
*/
void vI2C_INIT(void)
{
    vI2C_GPIO_Config();
    vI2C_Mode_Config();
    
    TARGET_ADDRESS = Block0_ADDRESS;
}

/*
***********************************************************************
*函数名称：usI2C_ByteWrite(u8 *pBuffer, uint8_t WriteAddr)
*
*函数功能：向目标地址写入一个字节
*
*使用说明：无
*入口参数：pBuffer：写入数据的地址  WriteAddr：写入地址
*返 回 值：0：失败  1：成功
*
*当前作者：李红禧
*创建日期：2017-6-13
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：李红禧
*完成日期：2017-6-13
***********************************************************************
*/
uint32_t usI2C_ByteWrite(u8 *pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
    I2CTimeout = I2CT_LONG_TIMEOUT;

    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))   
    {
        if((I2CTimeout--) == 0) return usI2C_TIMEOUT_UserCallback(4);
    }
  
    I2C_GenerateSTART(I2Cx, ENABLE);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /*检测EV5事件并清标志*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(0);
        }
    }
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    /*EEPROM的地址*/
    I2C_Send7bitAddress(I2Cx, TARGET_ADDRESS, I2C_Direction_Transmitter);
    
    /*检测EV6事件并清标志*/    
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(1);
        }
    }
    /*内部数据的地址*/
    I2C_SendData(I2Cx, WriteAddr);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /*检测EV8事件并清标志*/   
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(2);
        }
    }
        while(NumByteToWrite)
    {
        if(NumByteToWrite == 1)
        {
            I2C_AcknowledgeConfig(I2Cx, DISABLE);
            
            I2C_GenerateSTOP(I2Cx, ENABLE);
        }
        
        I2CTimeout = I2CT_LONG_TIMEOUT;
        
        while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == 0)
        {
            if((I2CTimeout--) == 0)
            {
                return usI2C_TIMEOUT_UserCallback(3);
            }
        }
        {
        
        I2C_SendData(I2Cx, *pBuffer);
        
        pBuffer++;
        
        NumByteToWrite--;
        }
    }
   
//    I2C_SendData(I2Cx, *pBuffer);
//    
//    I2CTimeout = I2CT_FLAG_TIMEOUT;
//    
//    /*检测EV8事件并清标志*/ 
//    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))    
//    {
//        if((I2CTimeout--) == 0)
//        {
//            return usI2C_TIMEOUT_UserCallback(3);
//        }
//    }
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
    return 1;
}

/*
***********************************************************************
*函数名称：vI2C_Wait_StandbyState(void)
*
*函数功能：等待擦写完毕
*
*使用说明：无
*入口参数：无
*返 回 值：无
*
*当前作者：李红禧
*创建日期：2017-6-13
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：李红禧
*完成日期：2017-6-13
***********************************************************************
*/
void vI2C_Wait_StandbyState(void)
{
    __IO uint16_t SR1_Tmp = 0;
    do{
        I2C_GenerateSTART(I2Cx, ENABLE);
        
        SR1_Tmp = I2C_ReadRegister(I2Cx, I2C_Register_SR1);
        
        I2C_Send7bitAddress(I2Cx, TARGET_ADDRESS, I2C_Direction_Transmitter);
      }
    
    while(!(I2C_ReadRegister(I2Cx, I2C_Register_SR1) & 0X0002));
    
    I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
    
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

/*
***********************************************************************
*函数名称：usI2C_ByetsWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
*
*函数功能：写入一串数据
*
*使用说明：无
*入口参数：*pBuffer 写入数据的地址  WriteAddr  写入地址  NumByteToWrite  数据大小
*返 回 值：0：失败  1：成功
*
*当前作者：李红禧
*创建日期：2017-6-13
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：李红禧
*完成日期：2017-6-13
***********************************************************************
*/
//uint8_t usI2C_ByetsWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
//{
//    uint16_t i;
//    uint8_t res;
//    
//    for(i = 0; i < NumByteToWrite; i++)
//    {
//        res = usI2C_ByteWrite(pBuffer++, WriteAddr++);
//        vI2C_Wait_StandbyState();
//    }
//    return res;
//}

/*
***********************************************************************
*函数名称：usI2C_ByetsRead(uint8_t *pBuffer, uint8_t ReadAddr, uint8_t NumByteToRead)
*
*函数功能：读取一串数据
*
*使用说明：无
*入口参数：*pBuffer 读出数据  WriteAddr  读取地址  NumByteToWrite  数据大小
*返 回 值：1：成功  
*
*当前作者：李红禧
*创建日期：2017-6-13
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：李红禧
*完成日期：2017-6-13
***********************************************************************
*/
uint8_t usI2C_ByetsRead(uint8_t *pBuffer, uint8_t ReadAddr, uint8_t NumByteToRead)
{
//    I2CTimeout = I2CT_LONG_TIMEOUT;
//    
//    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
//    {
//        if((I2CTimeout --) == 0)
//        {
//            return usI2C_TIMEOUT_UserCallback(9);
//        }
//        
//    }
    /*    起始信号    */
    I2C_GenerateSTART(I2Cx, ENABLE);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* 检测 EV5 事件并清除标志 */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(10);
        }
    }
    /*     发送设备地址     */
    I2C_Send7bitAddress(I2Cx, TARGET_ADDRESS, I2C_Direction_Transmitter);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* 检测 EV6 事件并清除标志 */ 
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(11);
        }
    }
    
    /*通过重新设置 PE 位清除 EV6 事件 */
    I2C_Cmd(I2Cx, ENABLE);
    
    /* 发送要读取的 EEPROM 内部地址(即 EEPROM 内部存储器的地址) */
    I2C_SendData(I2Cx, ReadAddr);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* 检测 EV8 事件并清除标志*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(12);
        }
    }
    /* 产生第二次 I2C 起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* 检测 EV5 事件并清除标志*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(13);
        }
    }  
    /* 发送 EEPROM 设备地址 */
    I2C_Send7bitAddress(I2Cx, TARGET_ADDRESS, I2C_Direction_Receiver); 
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* 检测 EV6 事件并清除标志*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(14);
        }
    }
    
    /* 读取 NumByteToRead 个数据*/
    while(NumByteToRead)
    {
        if(NumByteToRead == 1)
        {
            I2C_AcknowledgeConfig(I2Cx, DISABLE);
            
            I2C_GenerateSTOP(I2Cx, ENABLE);
        }
        
        I2CTimeout = I2CT_LONG_TIMEOUT;
        
        while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == 0)
        {
            if((I2CTimeout--) == 0)
            {
                return usI2C_TIMEOUT_UserCallback(3);
            }
        }
        {
        
        *pBuffer = I2C_ReceiveData(I2Cx);
        
        pBuffer++;
        
        NumByteToRead--;
        }
    }
    
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    
    return 1;
}

/*
***********************************************************************
*函数名称：usI2C_TIMEOUT_UserCallback(uint8_t errorCode)
*
*函数功能：I2C通讯错误回调函数
*
*使用说明：无
*入口参数：ucErrorCode:错误类型
*返 回 值：0
*
*当前作者：李红禧
*创建日期：2017-6-9
*函数版本：v1.0 
*修改原因：无
*
*取代版本：无
*原 作 者：李红禧
*完成日期：2017-6-9
***********************************************************************
*/
static uint32_t usI2C_TIMEOUT_UserCallback(uint8_t ucErrorCode)
{
    EEPROM_ERROR("I2C 等待超时!errorCode = %d",ucErrorCode);
    return 0;
}
