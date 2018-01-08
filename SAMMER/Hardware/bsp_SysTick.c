#include "bsp_SysTick.h"
#include "core_cm3.h"
#include "misc.h"


static __IO u32 TimingDelay;

/*
*****************************
*函数名称：SysTick_Init(void)
*
*函数功能：初始化定时器
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.25
*函数版本：v1.0
*****************************
*/
void SysTick_Init(void)
{
    if(SysTick_Config(SystemCoreClock / 100000) )
    {
     /************************************
             大小超出范围进入死循环
     ************************************/
        while(1);
    }
}

/*
*****************************
*函数名称：Delay_us(__IO u32 nTime)
*
*函数功能：微秒延时
*入口参数：__IO u32 nTime
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.25
*函数版本：v1.0
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
*函数名称：TimingDelay_Decrement(void)
*
*函数功能：被定时器中断调用，实现延时
*入口参数：无
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.25
*函数版本：v1.0
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
*函数名称：vSysTick_Delay_us(__IO uint32_t ulUs)
*
*函数功能：利用寄存器CTRL的第16位进行微秒延时, 无需调用初始化库定时器
*入口参数：__IO uint32_t ulUs
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.25
*函数版本：v1.0
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
                 关闭定时器
    ***************************************/
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
/*
*****************************
*函数名称：vSysTick_Delay_ms(__IO uint32_t ulMs)
*
*函数功能：利用寄存器CTRL的第16位进行毫秒延时，无需调用初始化库定时器
*入口参数：__IO uint32_t ulMs
*返回值：  无
*
*当前作者：LHX
*创建日期：2017.05.25
*函数版本：v1.0
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
                 关闭定时器
    ***************************************/
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

