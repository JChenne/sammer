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
*文件名：bsp_adc.c, bsp_adc.h
**文件摘要：采用独立模式单通道检测电压值
*
*当前版本：v1.0
*当前作者：LHX
*完成日期：2017.05.30
***************************************************
*/
#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;

/*
***************************************************
*函数名称：vADCx_GPIO_Config(void)
*
*函数功能：配置并初始化 ADC 的 GPIO 引脚
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.30
*函数版本：v1.0
***************************************************
*/

static void vADCx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = ADC_PIN1
                                    |ADC_PIN2
                                    |ADC_PIN3
                                    |ADC_PIN4
                                    |ADC_PIN5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//输入模式
    
    GPIO_Init(ADC_PORT, &GPIO_InitStructure);
}

/*
***************************************************
*函数名称：vADCx_Mode_Config(void)
*
*函数功能：配置ADC的采集方式
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.30
*函数版本：v1.0
***************************************************
*/
static void vADCx_Mode_Config(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_APBxClock_FUN(ADC_CLK, ENABLE);
    
    /*  独立模式，不用扫描，连续转换，无需外部触发事件，右对齐，转换通道1 */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;  
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
    ADC_InitStructure.ADC_NbrOfChannel = 1;  
    ADC_Init(ADCx, &ADC_InitStructure); 
    
    /*  8分频，即9MHz  */
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);  
    
    /* 通道转换顺序位1，第一个转换，采样时间为ADC_SampleTime_55Cycles5(55.5个时钟周期)  */
    ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
    
    ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
    
    ADC_Cmd(ADCx,ENABLE);
    
    ADC_ResetCalibration(ADCx);
    
    /* 等待校准寄存器初始化完成 */
    while(ADC_GetResetCalibrationStatus(ADCx));
    
    /* ADC开始校准 */
    ADC_StartCalibration(ADCx);
    
    /* 等待校准寄存器初始化完成 */
    while(ADC_GetCalibrationStatus(ADCx));
    
    /* 软件触发 */
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

/*
***************************************************
*函数名称：vADCx_Mode_Config(void)
*
*函数功能：配置ADC的中断优先级，并初始化
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.30
*函数版本：v1.0
***************************************************
*/
static void vADC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************
*函数名称：vADCx_Mode_Config(void)
*
*函数功能：整合其中的三个函数，在主函数中被调用
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.30
*函数版本：v1.0
***************************************************
*/
void vADCx_Init(void)
{
	vADCx_GPIO_Config();
	vADCx_Mode_Config();
	vADC_NVIC_Config();
}
