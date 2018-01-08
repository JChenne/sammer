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
*�ļ�����bsp_adc.c, bsp_adc.h
**�ļ�ժҪ�����ö���ģʽ��ͨ������ѹֵ
*
*��ǰ�汾��v1.0
*��ǰ���ߣ�LHX
*������ڣ�2017.05.30
***************************************************
*/
#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;

/*
***************************************************
*�������ƣ�vADCx_GPIO_Config(void)
*
*�������ܣ����ò���ʼ�� ADC �� GPIO ����
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.30
*�����汾��v1.0
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//����ģʽ
    
    GPIO_Init(ADC_PORT, &GPIO_InitStructure);
}

/*
***************************************************
*�������ƣ�vADCx_Mode_Config(void)
*
*�������ܣ�����ADC�Ĳɼ���ʽ
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.30
*�����汾��v1.0
***************************************************
*/
static void vADCx_Mode_Config(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_APBxClock_FUN(ADC_CLK, ENABLE);
    
    /*  ����ģʽ������ɨ�裬����ת���������ⲿ�����¼����Ҷ��룬ת��ͨ��1 */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;  
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
    ADC_InitStructure.ADC_NbrOfChannel = 1;  
    ADC_Init(ADCx, &ADC_InitStructure); 
    
    /*  8��Ƶ����9MHz  */
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);  
    
    /* ͨ��ת��˳��λ1����һ��ת��������ʱ��ΪADC_SampleTime_55Cycles5(55.5��ʱ������)  */
    ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
    
    ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
    
    ADC_Cmd(ADCx,ENABLE);
    
    ADC_ResetCalibration(ADCx);
    
    /* �ȴ�У׼�Ĵ�����ʼ����� */
    while(ADC_GetResetCalibrationStatus(ADCx));
    
    /* ADC��ʼУ׼ */
    ADC_StartCalibration(ADCx);
    
    /* �ȴ�У׼�Ĵ�����ʼ����� */
    while(ADC_GetCalibrationStatus(ADCx));
    
    /* ������� */
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

/*
***************************************************
*�������ƣ�vADCx_Mode_Config(void)
*
*�������ܣ�����ADC���ж����ȼ�������ʼ��
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.30
*�����汾��v1.0
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
*�������ƣ�vADCx_Mode_Config(void)
*
*�������ܣ��������е��������������������б�����
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.30
*�����汾��v1.0
***************************************************
*/
void vADCx_Init(void)
{
	vADCx_GPIO_Config();
	vADCx_Mode_Config();
	vADC_NVIC_Config();
}
