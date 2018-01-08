#include "bsp_control.h"

/* 初始化的时候赋值就行 */
float Aim_Side_X;
float Aim_Side_Y;
float Star_Side_X;
float Star_Side_Y;

float vSideDataprocess(float Now_Side_X, float Now_Side_Y)
{
    static uint8_t Sideflag = 1;
    static GPSside_Typedef GPS_Side = 
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0
    };
    
    if(Sideflag == 0)
    {
        GPS_Side.Aimside_X = Aim_Side_X;
        GPS_Side.Aimside_Y = Aim_Side_Y;
        GPS_Side.Starside_X = Star_Side_X;
        GPS_Side.Starside_Y = Star_Side_Y;
        GPS_Side.k = ((GPS_Side.Aimside_Y - GPS_Side.Starside_Y) / (GPS_Side.Aimside_X - GPS_Side.Starside_X));
        GPS_Side.b = (GPS_Side.Aimside_Y - (GPS_Side.k) * GPS_Side.Aimside_X);
        Sideflag = 1;
    }
    GPS_Side.OUT = Now_Side_Y - (GPS_Side.k) * Now_Side_X - GPS_Side.b;
    
    return GPS_Side.OUT;
}

void vDisplacementSinglePid(int32_t lGPS_Side_err)
{
    static PID_Typedef PID_Displacement_Left =
    {
		0,	    //比例系数
		0,		//积分系数
		0,		//微分系数
		0,		//积分值
		0,		//本次偏差
		0,		//上次偏差
		0,		//上上次偏差
		0,		//是否需要积分，用于积分分离
		0,		//本次微分值
		0.03,	//积分分离阈值
		1000,	//抗饱和积分阈值
		0,		//是否使用微分先行的PID策略
		1,		//是否需要限制PID输出幅度
		500,	//pid输出正限幅值
		-500,	//pid输出负的限幅值
		0			//PID输出  
    };
    
    static PID_Typedef PID_Displacement_Right =
    {
		0,	    //比例系数
		0,		//积分系数
		0,		//微分系数
		0,		//积分值
		0,		//本次偏差
		0,		//上次偏差
		0,		//上上次偏差
		0,		//是否需要积分，用于积分分离
		0,		//本次微分值
		0.03,	//积分分离阈值
		1000,	//抗饱和积分阈值
		0,		//是否使用微分先行的PID策略
		1,		//是否需要限制PID输出幅度
		500,	//pid输出正限幅值
		-500,	//pid输出负的限幅值
		0			//PID输出  
    };    
    
    
    
    PID_Displacement_Left.err = lGPS_Side_err;
    PID_Displacement_Left.integral += PID_Displacement_Left.err;
    PID_Displacement_Left.differential = PID_Displacement_Left.err - PID_Displacement_Left.err_last;
    PID_Displacement_Left.err_last = PID_Displacement_Left.err;
    
    PID_Displacement_Right.err = lGPS_Side_err;
    PID_Displacement_Right.integral += PID_Displacement_Right.err;
    PID_Displacement_Right.differential = PID_Displacement_Right.err - PID_Displacement_Right.err_last;
    PID_Displacement_Right.err_last = PID_Displacement_Right.err;
    
    /* 计算PID的OUT值 */
    vCalculatepxPid(&PID_Displacement_Left);
    vCalculatepxPid(&PID_Displacement_Right);
    
    /* 将PID的OUT的值赋值给定时器，改变PWM的占空比 */
    vTim1_MotoPwmOut((int32_t)PID_Displacement_Left.OUT, (int32_t)PID_Displacement_Right.OUT);
}

void Check_side_speed(float Now_Side_X, float Now_Side_Y)
{
    int32_t transfer;
    transfer = vSideDataprocess(Now_Side_X, Now_Side_Y);
    vDisplacementSinglePid(transfer);
}
