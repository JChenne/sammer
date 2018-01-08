#ifndef _bsp_control_h_
#define _bsp_control_h_

#include "stm32f10x.h"
#include "bsp_pid.h"
#include "bsp_gpio.h"
#include "bsp_PWM_Tim1.h"

typedef struct
{
    float Aimside_X;                            
    float Aimside_Y;                            
    float Starside_X;                          
	float Starside_Y;   
    float k;
    float b;
    float OUT;
}GPSside_Typedef;

void vDisplacementSinglePid(int32_t ulsimplependulumside);
void Check_side_speed(float Now_Side_X, float Now_Side_Y);

#define  MIDDLE_MIN                 1685
#define  MIDDLE_MAX                 1797

#endif
