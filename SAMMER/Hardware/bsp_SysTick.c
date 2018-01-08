#include "bsp_SysTick.h"
#include "core_cm3.h"
#include "misc.h"


static __IO u32 TimingDelay;

/*
*****************************
*�������ƣ�SysTick_Init(void)
*
*�������ܣ���ʼ����ʱ��
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.25
*�����汾��v1.0
*****************************
*/
void SysTick_Init(void)
{
    if(SysTick_Config(SystemCoreClock / 100000) )
    {
     /************************************
             ��С������Χ������ѭ��
     ************************************/
        while(1);
    }
}

/*
*****************************
*�������ƣ�Delay_us(__IO u32 nTime)
*
*�������ܣ�΢����ʱ
*��ڲ�����__IO u32 nTime
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.25
*�����汾��v1.0
*****************************
*/
void Delay_us(__IO u32 nTime)
{
    TimingDelay = nTime;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while(TimingDelay != 0);
}
/*
*****************************
*�������ƣ�TimingDelay_Decrement(void)
*
*�������ܣ�����ʱ���жϵ��ã�ʵ����ʱ
*��ڲ�������
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.25
*�����汾��v1.0
*****************************
*/
void TimingDelay_Decrement(void)
{
    if(TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

/*
*****************************
*�������ƣ�vSysTick_Delay_us(__IO uint32_t ulUs)
*
*�������ܣ����üĴ���CTRL�ĵ�16λ����΢����ʱ, ������ó�ʼ���ⶨʱ��
*��ڲ�����__IO uint32_t ulUs
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.25
*�����汾��v1.0
*****************************
*/

void vSysTick_Delay_us(__IO u32 ulUs)
{
    u32 i;
    SysTick_Config(SystemCoreClock/1000000);
    for(i = 0; i< ulUs; i++)
    {
        while(!((SysTick->CTRL) & (1<< 16)));
    }
    /***************************************
                 �رն�ʱ��
    ***************************************/
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
/*
*****************************
*�������ƣ�vSysTick_Delay_ms(__IO uint32_t ulMs)
*
*�������ܣ����üĴ���CTRL�ĵ�16λ���к�����ʱ��������ó�ʼ���ⶨʱ��
*��ڲ�����__IO uint32_t ulMs
*����ֵ��  ��
*
*��ǰ���ߣ�LHX
*�������ڣ�2017.05.25
*�����汾��v1.0
*****************************
*/

void vSysTick_Delay_ms(__IO u32 ulMs)
{
    u32 i;
    SysTick_Config(SystemCoreClock/1000);
    for(i = 0; i< ulMs; i++)
    {
        while(!((SysTick->CTRL) & (1<< 16)));
    }
    /***************************************
                 �رն�ʱ��
    ***************************************/
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

