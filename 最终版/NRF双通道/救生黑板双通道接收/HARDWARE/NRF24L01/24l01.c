#include "24l01.h"
#include "lcd.h"
#include "delay.h"
#include "spi.h"
#include "usart.h"
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
    
const char TX_ADDRESS0[TX_ADR_WIDTH]=   {0x01,0x43,0x10,0x10,0x01}; //���͵�ַ
const char RX_ADDRESS0[RX_ADR_WIDTH]=   {0x01,0x43,0x10,0x10,0x01};
const char RX_RX_ADDRESS0[RX_ADR_WIDTH]={0x01,0x43,0x10,0x10,0x01};

const char TX_ADDRESS1[TX_ADR_WIDTH]=   {0x11,0x43,0x10,0x10,0x01}; 
const char RX_ADDRESS1[RX_ADR_WIDTH]=   {0x11,0x43,0x10,0x10,0x01}; 
const char RX_RX_ADDRESS1[RX_ADR_WIDTH]={0x11,0x43,0x10,0x10,0x01}; 


//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,G�˿�ʱ��
    	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PB12���� ��ֹW25X�ĸ���
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);//����				
 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;	//PG8 7 ���� 	  
 	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��ָ��IO
  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PG6 ����  
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);//PG6,7,8����					 
		 
  SPI2_Init();    		//��ʼ��SPI	 
 
	SPI_Cmd(SPI2, DISABLE); // SPI���費ʹ��

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI����
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
			 
	NRF24L01_CE=1; 			//ʹ��24L01
	NRF24L01_CSN=1;			//SPIƬѡȡ��  
	 		 	 
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
char NRF24L01_Check(void)
{
	char buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	char i;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	 
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
char NRF24L01_Write_Reg(char reg,char value)
{
	char status;	
   	NRF24L01_CSN=0;                 //ʹ��SPI����
  	status =SPI2_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI2_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	NRF24L01_CSN=1;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
char NRF24L01_Read_Reg(char reg)
{
	char reg_val;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����		
  	SPI2_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI2_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  	NRF24L01_CSN = 1;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
char NRF24L01_Read_Buf(char reg,char *pBuf,char len)
{
	char status,char_ctr;	       
  	NRF24L01_CSN = 0;           //ʹ��SPI����
  	status=SPI2_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(char_ctr=0;char_ctr<len;char_ctr++)pBuf[char_ctr]=SPI2_ReadWriteByte(0XFF);//��������
  	NRF24L01_CSN=1;       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
char NRF24L01_Write_Buf(char reg, char *pBuf, char len)
{
	char status,char_ctr;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����
  	status = SPI2_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(char_ctr=0; char_ctr<len; char_ctr++)SPI2_ReadWriteByte(*pBuf++); //д������	 
  	NRF24L01_CSN = 1;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
char NRF24L01_TxPacket(char *txbuf)
{ 	
		char sta;
//	NRF24L01_CE=1;//��������

 	SPI2_SetSpeed(SPI_BaudRatePrescaler_8);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
//	NRF24L01_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
	   
	while(NRF24L01_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
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
char NRF24L01_RxPacket(char *rxbuf)
{
	char sta;		    							   
	SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}					    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{  	

		NRF24L01_CE=0;	 
	  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(char*)RX_RX_ADDRESS0,RX_ADR_WIDTH);
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);

  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P1,(char*)RX_RX_ADDRESS1,RX_ADR_WIDTH);
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P1,RX_PLOAD_WIDTH);
	
		NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x3f);    //ʹ������ͨ�����Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x3f);//ʹ������ͨ���Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
   	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
		NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}						 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
//void NRF24L01_TX_Mode(void)
//{		
//											 
//	  NRF24L01_CE=0;	    
//  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(char*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
//  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(char*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

//  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x3f);     //ʹ��ͨ��0���Զ�Ӧ��    
//  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x3f); //ʹ��ͨ��0�Ľ��յ�ַ  
//  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
//  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
//  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
//  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
//		NRF24L01_CE=1;//CEΪ��,10us����������		
//}




