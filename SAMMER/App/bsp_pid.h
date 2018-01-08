#ifndef _bsp_pid_H_
#define _bsp_pid_H_

#include "stm32f10x.h"

typedef struct
{
    const float Kp;                             //比例系数
    const float Ki;                             //积分系数
    const float Kd;                             //微分系数
    
    float integral;                             //积分值
    float err;                                  //本次偏差
    float err_last;                             //上次偏差
    float err_last_last;                        //上上次偏差
    
    uint32_t isNeedKi;	                        //是否需要积分，用于积分分离
    
 	float differential;			    			//本次微分值
    
	const float IntegralSeparation;				//积分分离阈值
	const float AntiSaturationIntegral;		    //抗饱和积分阈值
    
	const uint32_t isUseDifferentialAhead;		//是否使用微分先行的PID策略
	const uint32_t isNeedLimitPidOut;			//是否需要限制PID输出幅度
    
	const float pidOutLimitPlus;				//PID正限幅值
	const float pidOutLimitMinus;				//PID负限幅值
	float	OUT;      
}PID_Typedef;

void vCleanpxPid(PID_Typedef * pxPid);
void vCalculatepxPid(PID_Typedef * pxPid);

#endif
