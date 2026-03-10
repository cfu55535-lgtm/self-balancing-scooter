#ifndef __PID_H
#define __PID_H


int Vertical_Ring_PD(float measure,float theory);	//直立环
int Velocity_Ring_PI(int speed);					//速度环
void Integral_Limit(float max,float velocity_sum);	//积分限幅


#endif

