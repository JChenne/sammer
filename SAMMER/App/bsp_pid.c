#include "bsp_pid.h"

void vCleanpxPid(PID_Typedef * pxPid)
{
    pxPid->err = 0;
    pxPid->err_last = 0;
    pxPid->err_last_last = 0;
    pxPid->differential = 0;
    pxPid->integral = 0;
    pxPid->isNeedKi = 0;
    pxPid->OUT = 0;
}

void vCalculatepxPid(PID_Typedef * pxPid)
{
    if(pxPid->isUseDifferentialAhead)
    {
        
    }else
    {
        if(((pxPid->err) > (pxPid->IntegralSeparation)) || ((pxPid->err) < (-(pxPid->IntegralSeparation))))
        {
            pxPid->isNeedKi = 0;
        }else
        {
            pxPid->isNeedKi = 1;
            
            if(((pxPid->integral) > (pxPid->AntiSaturationIntegral)) && ((pxPid->err) < 0))
            {
                pxPid->integral += pxPid->err;
            }else if(((pxPid->integral) < -(pxPid->AntiSaturationIntegral)) && ((pxPid->err) > 0))
            {
                pxPid->integral += pxPid->err;
            }else
            {
                pxPid->integral += pxPid->err;
            } 
        }
        if(pxPid->isNeedKi)
        {
            pxPid->OUT = ((pxPid->Kp) * (pxPid->err)) + ((pxPid->Ki) * (pxPid->integral)) + ((pxPid->Kd) * (pxPid->differential));
        }else
        {
            pxPid->OUT = ((pxPid->Kp) * (pxPid->err)) + ((pxPid->Kd) * (pxPid->differential));
        }
    }
        
    if(pxPid->isNeedLimitPidOut)
    {
        if((pxPid->OUT) > (pxPid->pidOutLimitPlus))
        {
            pxPid->OUT = pxPid->pidOutLimitPlus;
        }else if((pxPid->OUT) < (pxPid->pidOutLimitMinus))
        {
            pxPid->OUT = pxPid->pidOutLimitMinus;
        }
    }
}
