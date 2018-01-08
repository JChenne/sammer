#include "bsp_PWM_Tim1.h"

/*
***************************************************
*函数名称：vADVANCE_TIM_GPIO_Config(void)
*
*函数功能：配置定时器的GPIO口
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
***************************************************
*/
static void vADVANCE_TIM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 输出比较通道 GPIO 初始化*/
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
*函数名称：vADVANCE_TIM_Mode_Config(uint32_t PWM_TIM1_PERIOD, uint32_t PWM_TIM1_PSC, uint32_t PWM_TIM1_PULSE)
*
*函数功能：配置定时器的GPIO口
*入口参数：uint32_t PWM_TIM1_PERIOD 装载值  uint32_t PWM_TIM1_PSC 时钟分频  uint32_t PWM_TIM1_PULSE 占空比的值
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
***************************************************
*/
static void vADVANCE_TIM_Mode_Config(uint32_t PWM_TIM1_PERIOD, uint32_t PWM_TIM1_PSC, uint32_t PWM_TIM1_PULSE)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    /* 开启定时器时钟，即内部时钟CK_INT = 72M */
    PWM_TIM1_APBxClock_FUN(PWM_TIM1_CLK, ENABLE);
    
    
    
    /*---------------时基结构体初始化--------------*/

    /* 自动重装载寄存器的值，累计 TIM_Period+1 个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Period = PWM_TIM1_PERIOD;
    /* 驱动CNT计数器的时钟 = Fck_int/(psc + 1) */
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_TIM1_PSC;
    /* 时钟分频因子，配置死区时间时用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    /* 计数器计数模式，设置为向上计数 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    /* 重复计数器的值 */
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    /* 初始化定时器 */
    TIM_TimeBaseInit(PWM_TIM1, &TIM_TimeBaseStructure);
    
    
    
    /*--------------输出比较通道1初始化-----------*/
    
    /* 配置PWM模式1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    /* 输出使能 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    /* 设置占空比大小 */
    TIM_OCInitStructure.TIM_Pulse = PWM_TIM1_PULSE;
    /* 输出通道电平极性配置 */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    /* 输出通道空闲电平极性配置 */
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    
    /* 根据 TIM1_OCInitStruct 中指定的参数初始化 TIM1 通道 1 */
    TIM_OC1Init(PWM_TIM1, &TIM_OCInitStructure);
    /*  设置或者重置 TIM1 捕获比较控制位  */
    TIM_OC1PreloadConfig(PWM_TIM1, TIM_OCPreload_Enable);  
    
    
    /*--------------输出比较通道2初始化-----------*/
    
    /* 配置PWM模式1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    /* 输出使能 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    /* 设置占空比大小 */
    TIM_OCInitStructure.TIM_Pulse = PWM_TIM1_PULSE;
    /* 输出通道电平极性配置 */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    /* 输出通道空闲电平极性配置 */
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    
    /* 根据 TIM1_OCInitStruct 中指定的参数初始化 TIM1 通道 1 */
    TIM_OC2Init(PWM_TIM1, &TIM_OCInitStructure);
    /*  设置或者重置 TIM1 捕获比较控制位  */
    TIM_OC2PreloadConfig(PWM_TIM1, TIM_OCPreload_Enable);     
    
    
    
    /* 使能计数器 */
    TIM_Cmd(PWM_TIM1, ENABLE);
    
    /* 主输出使能，当使用的是通用定时器，不用 */
    TIM_CtrlPWMOutputs(PWM_TIM1, ENABLE);
    
}

/*
***************************************************
*函数名称：vTim1_MotoPwmOut(int32_t channel1, int32_t channel2)
*
*函数功能：改变PWM波
*入口参数：int32_t channel1 通道1的PWM  int32_t channel2 通道2的PWM
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
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
*函数名称：vADVANCE_TIM_Init(uint32_t PWM_TIM1_PERIOD, uint32_t PWM_TIM1_PSC, uint32_t PWM_TIM1_PULSE)
*
*函数功能：初始化全体
*入口参数：uint32_t PWM_TIM1_PERIOD 装载值  uint32_t PWM_TIM1_PSC 时钟分频  uint32_t PWM_TIM1_PULSE 占空比的值
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.07.19
*函数版本：v1.0
***************************************************
*/
void vADVANCE_TIM_Init(uint32_t PWM_TIM1_PERIOD, uint32_t PWM_TIM1_PSC, uint32_t PWM_TIM1_PULSE)
{
    uint32_t ADVANCE_TIM_PULSE_REAL = 0;
    
    ADVANCE_TIM_PULSE_REAL = (PWM_TIM1_PERIOD * PWM_TIM1_PULSE) / 100;
    
    vADVANCE_TIM_GPIO_Config();
    vADVANCE_TIM_Mode_Config(PWM_TIM1_PERIOD, PWM_TIM1_PSC, ADVANCE_TIM_PULSE_REAL);
    
}
