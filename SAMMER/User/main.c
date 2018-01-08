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
*文件名：main.c
**文件摘要：
*
*当前版本：v1.0
*当前作者：LHX
*完成日期：2017.05.30
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

