/*
**************************************************
*Copyright(c) 2017 �࿥ LHX
*ALL rights reserved.More information please browse www.yanjun.tech
*
*�����������Դ����в�����bug�����˲��е��κ�ֱ�ӻ��ӵ����⳥
*                           ��ʹ�ñ����뼴��ΪĬ�ϱ����������bug,�뽫��ϸ��Ϣ
*                           ��������������лл��
*                           1027259561@qq.com
*
*�ļ�����bsp_i2c.h
**�ļ�ժҪ��
*
*��ǰ�汾��v1.0
*��ǰ���ߣ�LHX
*������ڣ�2017.06.13
***************************************************
*/

#include "bsp_i2c.h"
#include "bsp_usart.h"

static __IO uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;

static uint32_t usI2C_TIMEOUT_UserCallback(uint8_t errorCode);

uint16_t TARGET_ADDRESS;

/*
***********************************************************************
*�������ƣ�vI2C_GPIO_Config(void)
*
*�������ܣ�I2C_GPIO��ʼ��
*
*ʹ��˵������
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ������
*�������ڣ�2017-6-13
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ������
*������ڣ�2017-6-13
***********************************************************************
*/
static void vI2C_GPIO_Config(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    /* I2Cʱ�� */
    I2C_APBxClock_FUN (I2C_CLK, ENABLE);
    /* GPIOʱ�� */
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
*�������ƣ�vI2C_Mode_Config(void)
*
*�������ܣ�I2C��ʼ������
*
*ʹ��˵������
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ������
*�������ڣ�2017-6-13
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ������
*������ڣ�2017-6-13
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
    /*    ��I2C    */
    I2C_Cmd(I2Cx, ENABLE);
}

/*
***********************************************************************
*�������ƣ�vI2C_INIT(void)
*
*�������ܣ���ʼ���ĵ���, Ŀ���ַ
*
*ʹ��˵������
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ������
*�������ڣ�2017-6-13
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ������
*������ڣ�2017-6-13
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
*�������ƣ�usI2C_ByteWrite(u8 *pBuffer, uint8_t WriteAddr)
*
*�������ܣ���Ŀ���ַд��һ���ֽ�
*
*ʹ��˵������
*��ڲ�����pBuffer��д�����ݵĵ�ַ  WriteAddr��д���ַ
*�� �� ֵ��0��ʧ��  1���ɹ�
*
*��ǰ���ߣ������
*�������ڣ�2017-6-13
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ������
*������ڣ�2017-6-13
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
    
    /*���EV5�¼������־*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(0);
        }
    }
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    /*EEPROM�ĵ�ַ*/
    I2C_Send7bitAddress(I2Cx, TARGET_ADDRESS, I2C_Direction_Transmitter);
    
    /*���EV6�¼������־*/    
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(1);
        }
    }
    /*�ڲ����ݵĵ�ַ*/
    I2C_SendData(I2Cx, WriteAddr);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /*���EV8�¼������־*/   
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
//    /*���EV8�¼������־*/ 
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
*�������ƣ�vI2C_Wait_StandbyState(void)
*
*�������ܣ��ȴ���д���
*
*ʹ��˵������
*��ڲ�������
*�� �� ֵ����
*
*��ǰ���ߣ������
*�������ڣ�2017-6-13
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ������
*������ڣ�2017-6-13
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
*�������ƣ�usI2C_ByetsWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
*
*�������ܣ�д��һ������
*
*ʹ��˵������
*��ڲ�����*pBuffer д�����ݵĵ�ַ  WriteAddr  д���ַ  NumByteToWrite  ���ݴ�С
*�� �� ֵ��0��ʧ��  1���ɹ�
*
*��ǰ���ߣ������
*�������ڣ�2017-6-13
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ������
*������ڣ�2017-6-13
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
*�������ƣ�usI2C_ByetsRead(uint8_t *pBuffer, uint8_t ReadAddr, uint8_t NumByteToRead)
*
*�������ܣ���ȡһ������
*
*ʹ��˵������
*��ڲ�����*pBuffer ��������  WriteAddr  ��ȡ��ַ  NumByteToWrite  ���ݴ�С
*�� �� ֵ��1���ɹ�  
*
*��ǰ���ߣ������
*�������ڣ�2017-6-13
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ������
*������ڣ�2017-6-13
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
    /*    ��ʼ�ź�    */
    I2C_GenerateSTART(I2Cx, ENABLE);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* ��� EV5 �¼��������־ */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(10);
        }
    }
    /*     �����豸��ַ     */
    I2C_Send7bitAddress(I2Cx, TARGET_ADDRESS, I2C_Direction_Transmitter);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* ��� EV6 �¼��������־ */ 
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(11);
        }
    }
    
    /*ͨ���������� PE λ��� EV6 �¼� */
    I2C_Cmd(I2Cx, ENABLE);
    
    /* ����Ҫ��ȡ�� EEPROM �ڲ���ַ(�� EEPROM �ڲ��洢���ĵ�ַ) */
    I2C_SendData(I2Cx, ReadAddr);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* ��� EV8 �¼��������־*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(12);
        }
    }
    /* �����ڶ��� I2C ��ʼ�ź� */
    I2C_GenerateSTART(I2Cx, ENABLE);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* ��� EV5 �¼��������־*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(13);
        }
    }  
    /* ���� EEPROM �豸��ַ */
    I2C_Send7bitAddress(I2Cx, TARGET_ADDRESS, I2C_Direction_Receiver); 
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* ��� EV6 �¼��������־*/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
        {
            return usI2C_TIMEOUT_UserCallback(14);
        }
    }
    
    /* ��ȡ NumByteToRead ������*/
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
*�������ƣ�usI2C_TIMEOUT_UserCallback(uint8_t errorCode)
*
*�������ܣ�I2CͨѶ����ص�����
*
*ʹ��˵������
*��ڲ�����ucErrorCode:��������
*�� �� ֵ��0
*
*��ǰ���ߣ������
*�������ڣ�2017-6-9
*�����汾��v1.0 
*�޸�ԭ����
*
*ȡ���汾����
*ԭ �� �ߣ������
*������ڣ�2017-6-9
***********************************************************************
*/
static uint32_t usI2C_TIMEOUT_UserCallback(uint8_t ucErrorCode)
{
    EEPROM_ERROR("I2C �ȴ���ʱ!errorCode = %d",ucErrorCode);
    return 0;
}
