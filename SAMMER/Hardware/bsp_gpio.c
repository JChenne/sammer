/*
***********************************************************************
* 							YanJunFly V1.0 - Copyright (c) 2017
* All rights reserved.More information please browse www.yanjun.tech
*
* 其他声明：
*						对代码中产生的bug，本人不承担任何直接或间接的损害赔偿
*						若使用本代码即视为默认本条款。若发现bug，请将详细信息
*						发送下面链接至邮箱，谢谢！
*						1027259561@qq.com
*
* 文件名称：bsp_gpio.h bsp_gpio.c
* 文件摘要：
*						用于控制电机正反转 所用引脚为PC1 PC2   PB0 PB1
*						
* 当前版本：v1.0
* 当前作者：YJ_李红禧
* 完成日期：2017-7-21 10:28:53
* 改动说明：无
*
* 取代版本：无
* 原 作 者：YJ_李红禧
* 完成日期：2017-7-21 10:28:53
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

