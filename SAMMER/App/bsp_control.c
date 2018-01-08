#include "bsp_control.h"

/* ��ʼ����ʱ��ֵ���� */
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
		0,	    //����ϵ��
		0,		//����ϵ��
		0,		//΢��ϵ��
		0,		//����ֵ
		0,		//����ƫ��
		0,		//�ϴ�ƫ��
		0,		//���ϴ�ƫ��
		0,		//�Ƿ���Ҫ���֣����ڻ��ַ���
		0,		//����΢��ֵ
		0.03,	//���ַ�����ֵ
		1000,	//�����ͻ�����ֵ
		0,		//�Ƿ�ʹ��΢�����е�PID����
		1,		//�Ƿ���Ҫ����PID�������
		500,	//pid������޷�ֵ
		-500,	//pid��������޷�ֵ
		0			//PID���  
    };
    
    static PID_Typedef PID_Displacement_Right =
    {
		0,	    //����ϵ��
		0,		//����ϵ��
		0,		//΢��ϵ��
		0,		//����ֵ
		0,		//����ƫ��
		0,		//�ϴ�ƫ��
		0,		//���ϴ�ƫ��
		0,		//�Ƿ���Ҫ���֣����ڻ��ַ���
		0,		//����΢��ֵ
		0.03,	//���ַ�����ֵ
		1000,	//�����ͻ�����ֵ
		0,		//�Ƿ�ʹ��΢�����е�PID����
		1,		//�Ƿ���Ҫ����PID�������
		500,	//pid������޷�ֵ
		-500,	//pid��������޷�ֵ
		0			//PID���  
    };    
    
    
    
    PID_Displacement_Left.err = lGPS_Side_err;
    PID_Displacement_Left.integral += PID_Displacement_Left.err;
    PID_Displacement_Left.differential = PID_Displacement_Left.err - PID_Displacement_Left.err_last;
    PID_Displacement_Left.err_last = PID_Displacement_Left.err;
    
    PID_Displacement_Right.err = lGPS_Side_err;
    PID_Displacement_Right.integral += PID_Displacement_Right.err;
    PID_Displacement_Right.differential = PID_Displacement_Right.err - PID_Displacement_Right.err_last;
    PID_Displacement_Right.err_last = PID_Displacement_Right.err;
    
    /* ����PID��OUTֵ */
    vCalculatepxPid(&PID_Displacement_Left);
    vCalculatepxPid(&PID_Displacement_Right);
    
    /* ��PID��OUT��ֵ��ֵ����ʱ�����ı�PWM��ռ�ձ� */
    vTim1_MotoPwmOut((int32_t)PID_Displacement_Left.OUT, (int32_t)PID_Displacement_Right.OUT);
}

void Check_side_speed(float Now_Side_X, float Now_Side_Y)
{
    int32_t transfer;
    transfer = vSideDataprocess(Now_Side_X, Now_Side_Y);
    vDisplacementSinglePid(transfer);
}
