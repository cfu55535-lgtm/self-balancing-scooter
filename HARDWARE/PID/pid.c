#include "stm32f10x.h"                  // Device header
#include "pwm.h"
#include "motor.h"
#include "Encoder.h"
#include "pid.h"
#include "mpuexti.h"

//直立环参数
float Kp = 600;		//比例系数   Kp = 1000,Td =12
float Kd = -330;
//float T =10;
//float Td =0;
float error;				//误差
float last_error;			//上次误差
float error_sum=0;			//累计误差
float error_difference;		//误差的差值
float Vertical_out;			//直立环输出
	
	
//速度环参数
//float KVp = -40;		// KVp = 200  KVi = 1
//float KVi = -0.2;

float KVp = -200;		// KVp = 200  KVi = 1
float KVi = -1;
//float Ti = 0;
float a=0.3;				//一阶滤波系数
float filt_velocity;     	//滤波后的速度
float last_filt_velocity;	//上一次的滤波后的速度
float velocity_sum=0;    	//速度误差的累加
float Velocity_out;			//速度环输出


/**
    	*@brief		直立环(PD)
    	*@param		measure：角度测量值
    	*@param		theory：角度理论值		
    	*@retval	out：pid计算输出
    	*/	
int Vertical_Ring_PD(float measure,float theory)
{
	error = measure - theory;					//误差值
	last_error = error;							//上次误差
	error_sum += error;							//误差的累计
	error_difference = error - last_error;		//误差的差值
	
//	Vertical_out  = Kp * error + Kp * (Td/T) * error_difference;
	Vertical_out  = Kp * error + Kd * error_difference;
	
	return Vertical_out;
}


/**
	*@brief		速度环(PI)，一般为正反馈
	*@param		编码器测得速度
	*@retval	PI输出
	*/
int Velocity_Ring_PI(int fast)
{
	filt_velocity = fast * a + (1-a) * last_filt_velocity;		//滤波后的速度
	
	last_filt_velocity = filt_velocity;							//上次滤波速度
	
	velocity_sum += filt_velocity;								//速度误差累计
	
	Integral_Limit(3000,velocity_sum);			//积分限幅
	
//	Velocity_out = KVp * filt_velocity + KVp * (T/Ti) * velocity_sum;	//输出
	
	Velocity_out = KVp * filt_velocity + KVi * velocity_sum;	//输出
	
	return Velocity_out;
}


/**
	*@brief		积分限幅
	*@param		max：积分累加最大值
	*@param		velocity_sum：积分累加当前值
	*@retval	返回velocity_sum的值
	*/
void Integral_Limit(float max,float velocity_sum)
{
	if(velocity_sum >= max)
	{
		velocity_sum = max;
	}
	if(velocity_sum <=- max)
	{
		velocity_sum =- max;
	}
	
}



