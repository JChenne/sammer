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
*�ļ�����main.c
**�ļ�ժҪ��
*
*��ǰ�汾��v1.0
*��ǰ���ߣ�LHX
*������ڣ�2017.05.30
***************************************************
*/


#include "main.h"

void vALLINIT(void)
{
    vAIN_GPIO_Config();
    SysTick_Init();
    vADVANCE_TIM_Init((100 - 1), (72 - 1), 70);
    vENCODER_TIM2_Init();
    vENCODER_TIM3_Init();
    vUSART1_Init(115200);
}

int main(void)
{
    vALLINIT();
}

