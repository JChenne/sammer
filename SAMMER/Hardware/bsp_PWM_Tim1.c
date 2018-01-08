#include "bsp_PWM_Tim1.h"

/*
***************************************************
*�������ƣ�vADVANCE_TIM_GPIO_Config(void)
*
*�������ܣ����ö�ʱ����GPIO��
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
***************************************************
*/
static void vADVANCE_TIM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ����Ƚ�ͨ�� GPIO ��ʼ��*/
    RCC_APB2PeriphClockCmd(PWM_TIM1_CH1_GPIO_CLK | PWM_TIM1_CH2_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = PWM_TIM1_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWM_TIM1_CH1_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PWM_TIM1_CH2_PIN;
    GPIO_Init(PWM_TIM1_CH2_PORT, &GPIO_InitStructure);
    
}

/*
***************************************************
*�������ƣ�vADVANCE_TIM_Mode_Config(uint32_t PWM_TIM1_PERIOD, uint32_t PWM_TIM1_PSC, uint32_t PWM_TIM1_PULSE)
*
*�������ܣ����ö�ʱ����GPIO��
*��ڲ�����uint32_t PWM_TIM1_PERIOD װ��ֵ  uint32_t PWM_TIM1_PSC ʱ�ӷ�Ƶ  uint32_t PWM_TIM1_PULSE ռ�ձȵ�ֵ
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
***************************************************
*/
static void vADVANCE_TIM_Mode_Config(uint32_t PWM_TIM1_PERIOD, uint32_t PWM_TIM1_PSC, uint32_t PWM_TIM1_PULSE)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    /* ������ʱ��ʱ�ӣ����ڲ�ʱ��CK_INT = 72M */
    PWM_TIM1_APBxClock_FUN(PWM_TIM1_CLK, ENABLE);
    
    
    
    /*---------------ʱ���ṹ���ʼ��--------------*/

    /* �Զ���װ�ؼĴ�����ֵ���ۼ� TIM_Period+1 ��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Period = PWM_TIM1_PERIOD;
    /* ����CNT��������ʱ�� = Fck_int/(psc + 1) */
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_TIM1_PSC;
    /* ʱ�ӷ�Ƶ���ӣ���������ʱ��ʱ�õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    /* ����������ģʽ������Ϊ���ϼ��� */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    /* �ظ���������ֵ */
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    /* ��ʼ����ʱ�� */
    TIM_TimeBaseInit(PWM_TIM1, &TIM_TimeBaseStructure);
    
    
    
    /*--------------����Ƚ�ͨ��1��ʼ��-----------*/
    
    /* ����PWMģʽ1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    /* ���ʹ�� */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    /* ����ռ�ձȴ�С */
    TIM_OCInitStructure.TIM_Pulse = PWM_TIM1_PULSE;
    /* ���ͨ����ƽ�������� */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    /* ���ͨ�����е�ƽ�������� */
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    
    /* ���� TIM1_OCInitStruct ��ָ���Ĳ�����ʼ�� TIM1 ͨ�� 1 */
    TIM_OC1Init(PWM_TIM1, &TIM_OCInitStructure);
    /*  ���û������� TIM1 ����ȽϿ���λ  */
    TIM_OC1PreloadConfig(PWM_TIM1, TIM_OCPreload_Enable);  
    
    
    /*--------------����Ƚ�ͨ��2��ʼ��-----------*/
    
    /* ����PWMģʽ1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    /* ���ʹ�� */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    /* ����ռ�ձȴ�С */
    TIM_OCInitStructure.TIM_Pulse = PWM_TIM1_PULSE;
    /* ���ͨ����ƽ�������� */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    /* ���ͨ�����е�ƽ�������� */
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    
    /* ���� TIM1_OCInitStruct ��ָ���Ĳ�����ʼ�� TIM1 ͨ�� 1 */
    TIM_OC2Init(PWM_TIM1, &TIM_OCInitStructure);
    /*  ���û������� TIM1 ����ȽϿ���λ  */
    TIM_OC2PreloadConfig(PWM_TIM1, TIM_OCPreload_Enable);     
    
    
    
    /* ʹ�ܼ����� */
    TIM_Cmd(PWM_TIM1, ENABLE);
    
    /* �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ�������� */
    TIM_CtrlPWMOutputs(PWM_TIM1, ENABLE);
    
}

/*
***************************************************
*�������ƣ�vTim1_MotoPwmOut(int32_t channel1, int32_t channel2)
*
*�������ܣ��ı�PWM��
*��ڲ�����int32_t channel1 ͨ��1��PWM  int32_t channel2 ͨ��2��PWM
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
***************************************************
*/
void vTim1_MotoPwmOut(int32_t channel1, int32_t channel2)
{
    if(channel1 > MOTO_MAX_PWM_PAUSE)
    {
        channel1 = MOTO_MAX_PWM_PAUSE;
    }
    
    if(channel1 < 0)
    {
        channel1 = 0;
    }
    
    if(channel2 > MOTO_MAX_PWM_PAUSE)
    {
        channel2 = MOTO_MAX_PWM_PAUSE;
    }
    
    if(channel2 < 0)
    {
        channel2 = 0;
    }
    
    PWM_TIM1->CCR1 = channel1 + MOTO_START_PWM_PAUSE;
    PWM_TIM1->CCR2 = channel2 + MOTO_START_PWM_PAUSE;
    
}

/*
***************************************************
*�������ƣ�vADVANCE_TIM_Init(uint32_t PWM_TIM1_PERIOD, uint32_t PWM_TIM1_PSC, uint32_t PWM_TIM1_PULSE)
*
*�������ܣ���ʼ��ȫ��
*��ڲ�����uint32_t PWM_TIM1_PERIOD װ��ֵ  uint32_t PWM_TIM1_PSC ʱ�ӷ�Ƶ  uint32_t PWM_TIM1_PULSE ռ�ձȵ�ֵ
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.07.19
*�����汾��v1.0
***************************************************
*/
void vADVANCE_TIM_Init(uint32_t PWM_TIM1_PERIOD, uint32_t PWM_TIM1_PSC, uint32_t PWM_TIM1_PULSE)
{
    uint32_t ADVANCE_TIM_PULSE_REAL = 0;
    
    ADVANCE_TIM_PULSE_REAL = (PWM_TIM1_PERIOD * PWM_TIM1_PULSE) / 100;
    
    vADVANCE_TIM_GPIO_Config();
    vADVANCE_TIM_Mode_Config(PWM_TIM1_PERIOD, PWM_TIM1_PSC, ADVANCE_TIM_PULSE_REAL);
    
}
