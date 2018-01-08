#ifndef _USART1_DMA_H
#define _USART1_DMA_H

/*   ���뷢�Ͳ�ͬ���������� ��Ҫ���� vUSART1_DmaTxStart��߹���ָ�������  */
#include "system.h"

/* ���ڷ��͵�ָ��� */
#define USART1_TX_SIZE  ((uint32_t)200)
/* ���ڽ��յ����ݵĳ��� */
#define USART1_RX_SIZE  ((uint32_t)200)

extern vu8 vucUSART1_RxBuffer[USART1_RX_SIZE];
extern uint32_t ulUsart1TxOkFlag;
extern uint32_t ulUsart1RxOkFlag;
extern uint32_t ulUsart1RecvRealSize;

void vUSART1_Init(uint32_t ulBound);
void vUSART1_DmaRxStart(void);
ErrorStatus vUSART1_DmaTxStart(uint16_t * pucBuffer, uint32_t ulSize, uint32_t ulIsNeedCopyData);

#endif

/* end of file cppyright reserve by team of yanjun ,More information please browse www.yanjun.tech */
