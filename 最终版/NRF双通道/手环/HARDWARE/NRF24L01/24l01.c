#include "24l01.h"
#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//NRF24L01��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
    
const u8 TX_ADDRESS0[TX_ADR_WIDTH]=	  {0x01,0x43,0x10,0x10,0x01}; 
const u8 RX_ADDRESS0[RX_ADR_WIDTH]=	  {0x01,0x43,0x10,0x10,0x01}; 
const u8 RX_RX_ADDRESS0[RX_ADR_WIDTH]={0x01,0x43,0x10,0x10,0x01}; 


//��ʼ��24L01��IO��
void T_NRF24L01_Init(void)
{ 	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��PC�˿�ʱ��


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 						 	//�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						//PC8 7 ���� 	  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);										//��ʼ��ָ��IO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//PC6 ����  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	GPIO_ResetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);						//PC6,7,8����	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		 
	SPI1_Init();    															//��ʼ��SPI	 
			 
	T_NRF24L01_CE=1; 			//ʹ��24L01
	T_NRF24L01_CSN=1;			//SPIƬѡȡ��  
	 		 	 
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
u8 T_NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 check[5] = {0};
	u8 i;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	T_NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	T_NRF24L01_Read_Buf(NRF_READ_REG|TX_ADDR,check,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)
	{
		if(buf[i]!=check[i])break;	 	
	}		
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	 
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 T_NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	T_NRF24L01_CSN=0;                 //ʹ��SPI����
  	status =SPI1_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	T_NRF24L01_CSN=1;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 T_NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	T_NRF24L01_CSN = 0;          //ʹ��SPI����		
  	SPI1_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI1_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  	T_NRF24L01_CSN = 1;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 T_NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	T_NRF24L01_CSN = 0;           //ʹ��SPI����
  	status=SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	{
		pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//��������
	}
  	T_NRF24L01_CSN=1;       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 T_NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
		u8 status,u8_ctr;	    
		T_NRF24L01_CSN = 0;          //ʹ��SPI����
  	status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //д������	 
  	T_NRF24L01_CSN = 1;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 T_NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta; 	
//	NRF24L01_CE=1;//��������	
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
//	NRF24L01_CE=0;
  	T_NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
   
	while(T_NRF24L01_IRQ!=0);//�ȴ��������
	sta=T_NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	T_NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		T_NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 T_NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=T_NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	T_NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		T_NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		T_NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}					    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//void T_NRF24L01_RX_Mode(void)
//{  	
////		NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
////		NRF24L01_CE=0;	  
//  	T_NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)T_RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
//	  
//  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
//  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
//  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
//  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
//  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
//  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

//}						 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void T_NRF24L01_TX_Mode(void)
{		
//		NRF24L01_CE=1;//CEΪ��,10us����������													 
		T_NRF24L01_CE=0;	    
		T_NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS0,TX_ADR_WIDTH);
  	T_NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS0,RX_ADR_WIDTH); 		
	 
	T_NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x3F);     //ʹ��ͨ�����Զ�Ӧ��    
  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x3F); //ʹ��ͨ���Ľ��յ�ַ  
  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
//		T_NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
//		T_NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P1,RX_PLOAD_WIDTH);//ѡ��ͨ��1����Ч���ݿ��	
  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	T_NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
		T_NRF24L01_CE=1;
}


