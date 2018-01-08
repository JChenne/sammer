#ifndef  _bsp_i2c_h_
#define  _bsp_i2c_h_

#include "stm32f10x.h"

#define     I2Cx                     I2C1

/*    I2C以及对应的GPIO时钟端口定义    */
#define     I2C_APBxClock_FUN        RCC_APB1PeriphClockCmd
#define     I2C_CLK                  RCC_APB1Periph_I2C1
#define     I2C_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
#define     I2C_GPIO_CLK             RCC_APB2Periph_GPIOB

/*    时钟，数据线的定义    */
#define     I2C_SCL_PORT             GPIOB
#define     I2C_SCL_PIN              GPIO_Pin_6
#define     I2C_SDA_PORT             GPIOB
#define     I2C_SDA_PIN              GPIO_Pin_7

/*    I2C的速度    */
#define     I2C_Speed                       400000

/*    I2C的本机地址    */
#define     I2Cx_OWN_ADDRESS7               0X0A

/*    标志位    */
#define     I2CT_FLAG_TIMEOUT               ((uint32_t)0x1000)
#define     I2CT_LONG_TIMEOUT               ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

#define     EEPROM_DEBUG_ON                 0

#define     EEPROM_INFO(fmt,arg...)         printf("<<-EEPROM-INFO->>" fmt "\n",##arg)
#define     EEPROM_ERROR(fmt,arg...)        printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define     EEPROM_DEBUG(fmt,arg...)        do{\
                                            if(EEPROM_DEBUG_ON)\
                                            printf("<<-EEPROM-DEBUG->> [%d]" fmt "\n",__LINE__,##arg);\
                                            }while(0)

/*    目标地址    */
#define     Block0_ADDRESS           0xA0 
                                            
                                            
                                            
void vI2C_INIT(void);
                                            
static void vI2C_GPIO_Config(void);
                                            
static void vI2C_Mode_Config(void);
                                            
uint32_t usI2C_ByteWrite(u8 *pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);
                                            
void vI2C_WaitStandbyState(void);
                                            
uint8_t usI2C_ByetsWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
                                            
static uint32_t usI2C_TIMEOUT_UserCallback(uint8_t ucErrorCode);  
                                            
uint8_t usI2C_ByetsRead (uint8_t *pBuffer, uint8_t ReadAddr, uint8_t NumByteToRead);                                            

#endif
