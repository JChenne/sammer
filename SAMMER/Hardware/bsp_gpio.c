/*
***********************************************************************
* 							YanJunFly V1.0 - Copyright (c) 2017
* All rights reserved.More information please browse www.yanjun.tech
*
* ����������
*						�Դ����в�����bug�����˲��е��κ�ֱ�ӻ��ӵ����⳥
*						��ʹ�ñ����뼴��ΪĬ�ϱ����������bug���뽫��ϸ��Ϣ
*						�����������������䣬лл��
*						1027259561@qq.com
*
* �ļ����ƣ�bsp_gpio.h bsp_gpio.c
* �ļ�ժҪ��
*						���ڿ��Ƶ������ת ��������ΪPC1 PC2   PB0 PB1
*						
* ��ǰ�汾��v1.0
* ��ǰ���ߣ�YJ_�����
* ������ڣ�2017-7-21 10:28:53
* �Ķ�˵������
*
* ȡ���汾����
* ԭ �� �ߣ�YJ_�����
* ������ڣ�2017-7-21 10:28:53
***********************************************************************
*/
#include "bsp_gpio.h"

void vAIN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(AIN1_1GPIO_CLK | AIN1_2GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = AIN1_1GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(AIN1_1GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = AIN1_2GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(AIN1_2GPIO_PORT, &GPIO_InitStructure);
    
    
    RCC_APB2PeriphClockCmd(AIN2_1GPIO_CLK | AIN2_2GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = AIN2_1GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(AIN2_1GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = AIN2_2GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(AIN2_2GPIO_PORT, &GPIO_InitStructure);
}

