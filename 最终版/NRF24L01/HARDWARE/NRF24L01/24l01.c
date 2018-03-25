#include "24l01.h"
#include "lcd.h"
#include "delay.h"
#include "spi.h"
#include "usart.h"

    
const u8 TX_ADDRESS0[TX_ADR_WIDTH]=	  {0x01,0x43,0x10,0x10,0x01}; 
const u8 RX_ADDRESS0[RX_ADR_WIDTH]=	  {0x01,0x43,0x10,0x10,0x01}; 
const u8 RX_RX_ADDRESS0[RX_ADR_WIDTH]={0x01,0x43,0x10,0x10,0x01}; 

const u8 TX_ADDRESS1[TX_ADR_WIDTH]=   {0x02,0x43,0x10,0x10,0x01}; 
const u8 RX_ADDRESS1[RX_ADR_WIDTH]=   {0x02,0x43,0x10,0x10,0x01}; 
const u8 RX_RX_ADDRESS1[RX_ADR_WIDTH]={0x02,0x43,0x10,0x10,0x01}; 

const u8 TX_ADDRESS2[TX_ADR_WIDTH]=   {0x03,0x43,0x10,0x10,0x01}; 
const u8 RX_ADDRESS2[RX_ADR_WIDTH]=   {0x03,0x43,0x10,0x10,0x01};
const u8 RX_RX_ADDRESS2[RX_ADR_WIDTH]={0x43,0x10,0x10,0x01,0x03}; 

const u8 TX_ADDRESS3[TX_ADR_WIDTH]=   {0x04,0x43,0x10,0x10,0x01};  
const u8 RX_ADDRESS3[RX_ADR_WIDTH]=   {0x04,0x43,0x10,0x10,0x01};
const u8 RX_RX_ADDRESS3[RX_ADR_WIDTH]={0x43,0x10,0x10,0x01,0x04}; 

const u8 TX_ADDRESS4[TX_ADR_WIDTH]=   {0x05,0x43,0x10,0x10,0x01}; 
const u8 RX_ADDRESS4[RX_ADR_WIDTH]=   {0x05,0x43,0x10,0x10,0x01};
const u8 RX_RX_ADDRESS4[RX_ADR_WIDTH]={0x43,0x10,0x10,0x01,0x05}; 

const u8 TX_ADDRESS5[TX_ADR_WIDTH]=   {0x06,0x43,0x10,0x10,0x01}; 
const u8 RX_ADDRESS5[RX_ADR_WIDTH]=   {0x06,0x43,0x10,0x10,0x01}; 
const u8 RX_RX_ADDRESS5[RX_ADR_WIDTH]={0x43,0x10,0x10,0x01,0x06}; 


void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
		 
    SPI1_Init();    		
 
	SPI_Cmd(SPI1, DISABLE); 

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStructure);  
 
	SPI_Cmd(SPI1, ENABLE); 
			 
	NRF24L01_CE=0; 			
	NRF24L01_CSN=1;			
	 		 	 
}


u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4); 
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,buf,5);
	NRF24L01_Read_Buf(TX_ADDR,buf,5); 
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;
	return 0;		 
}	 	 



u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN=0;                 
  	status =SPI1_ReadWriteByte(reg);
  	SPI1_ReadWriteByte(value);      
  	NRF24L01_CSN=1;                 
  	return(status);       			
}


u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN = 0;          
  	SPI1_ReadWriteByte(reg);   
  	reg_val=SPI1_ReadWriteByte(0XFF);
  	NRF24L01_CSN = 1;          
  	return(reg_val);           
}	





u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN = 0;           
  	status=SPI1_ReadWriteByte(reg);
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);
  	NRF24L01_CSN=1;       
  	return status;        
}





u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          
  	status = SPI1_ReadWriteByte(reg);
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); 
  	NRF24L01_CSN = 1;       
  	return status;          
}				   



u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
	NRF24L01_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
 	NRF24L01_CE=1;
	while(NRF24L01_IRQ!=0);
	sta=NRF24L01_Read_Reg(STATUS);  
	NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,sta); 
	if(sta&MAX_TX)
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);
		return MAX_TX; 
	}
	if(sta&TX_OK)
	{
		return TX_OK;
	}
	return 0xff;
}



u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); 
	sta=NRF24L01_Read_Reg(STATUS);  
	NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,sta); 
	if(sta&RX_OK)
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);
		NRF24L01_Write_Reg(FLUSH_RX,0xff);
		return 0; 
	}	   
	return 1;
}					    



void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE=0;	  
	
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_RX_ADDRESS0,RX_ADR_WIDTH);
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH);

  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P1,(u8*)RX_RX_ADDRESS1,RX_ADR_WIDTH);
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P1,RX_PLOAD_WIDTH);
	
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P2,(u8*)RX_RX_ADDRESS2,RX_ADR_WIDTH);
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P2,RX_PLOAD_WIDTH);
	
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P3,(u8*)RX_RX_ADDRESS3,RX_ADR_WIDTH);
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P3,RX_PLOAD_WIDTH);
	
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P4,(u8*)RX_RX_ADDRESS4,RX_ADR_WIDTH);
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P4,RX_PLOAD_WIDTH);
	
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P5,(u8*)RX_RX_ADDRESS5,RX_ADR_WIDTH);
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P5,RX_PLOAD_WIDTH);
		  
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x3F);    
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x3F);
	
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,40);	     
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);
  	NRF24L01_Write_Reg(WRITE_REG_NRF+CONFIG, 0x0f);
  	NRF24L01_CE = 1; 
}						 





void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE=0;	    
#if 0
  	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS0,TX_ADR_WIDTH);
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS0,RX_ADR_WIDTH); 
#endif	
	
#if 0
  	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS1,TX_ADR_WIDTH);
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS1,RX_ADR_WIDTH); 
#endif	
	
#if 0
  	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS2,TX_ADR_WIDTH);
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS2,RX_ADR_WIDTH); 
#endif	
	
#if 1
  	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS3,TX_ADR_WIDTH);
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS3,RX_ADR_WIDTH); 
#endif	
	
#if 0
  	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS4,TX_ADR_WIDTH);
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS4,RX_ADR_WIDTH); 
#endif	

#if 0
  	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS5,TX_ADR_WIDTH);
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS5,RX_ADR_WIDTH); 
#endif	

  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x3F);     
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x3F); 
	
  	NRF24L01_Write_Reg(WRITE_REG_NRF+SETUP_RETR,0x1a);
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,40);       
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);  
  	NRF24L01_Write_Reg(WRITE_REG_NRF+CONFIG,0x0e);    
	NRF24L01_CE=1;
}		  






