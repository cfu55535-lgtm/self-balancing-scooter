#include "stm32f10x.h"                  // Device header
#include "inv_mpu_dmp_motion_driver.h" 
#include "inv_mpu.h"
#include "mpu6050.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "pwm.h"
#include "Encoder.h"
#include "mpuexti.h"
#include "motor.h"
#include "ADC.h"


extern float pitch,roll,yaw;	//欧拉角
extern float velocity_sum;    	//速度误差的累加
extern int Motor_enable;		//电机使能标志
extern int fast;				//测量的最终速度
extern int Left_PWM,Right_PWM;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//2位抢占优先级，2位响应优先级	
	delay_init();      		//延时函数初始化
	OLED_Init();        	//oled初始化
	motor_Init();			//电机初始化
	MPU_Init();				//mpu初始化
	DMP_Init();				//DMP初始化
	mpu_exti_init();		//mpu外部中断初始化

	while(1)
	{
		
		OLED_ShowAngle(roll,yaw); 	//显示欧拉角
		OLED_ShowPWM(read_speed(2));   		//显示速度（非真实速度）


//		if(roll<-40||roll>40)		//安全装置
//		{
//			Motor_enable=0;			//电机使能为0，即关闭电机
//			velocity_sum=0;			//速度积分清零			
//		}
//		else
//		{
//			Motor_enable=1;			//开启电机
//		}
		
	}	
}
