#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "mpu6050.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "pid.h"
#include "motor.h"
#include "pwm.h"
#include "Encoder.h"

float pitch,roll,yaw;  			//欧拉角
float measure,theory;
float PWM0;						//速度环+直立环输出PWM
float Left_PWM;					//左电机最终PWM
float Right_PWM;				//右电机最终PWM
int fast;						//测量的最终速度
int Motor_enable;				//电机使能标志

/**
	*@brief		是用来配置MPU6050引脚INT的，每当MPU6050有数据输出时，引脚INT有相应的电平输出。
				依次来触发外部中断作为控制周期。保持MPU6050数据的实时性。
	*@param		无
	*@retval	无
	*/
void mpu_exti_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//初始化EXTI的GPIO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//将PA12映射到EXTI中断线12
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	
	//初始化EXTI
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;//中断线12
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	//初始化NVIC
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;	//抢占优先级0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;		//响应优先级0
	NVIC_Init(&NVIC_InitStructure);
		
}


/**
	*@brief		外部中断线12函数，PID执行函数在此，务必保证PID计算与数据接受同步
	*@param		无
	*@retval	无
	*/

void EXTI15_10_IRQHandler(void)
{
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{

		measure=roll;		//测量俯仰角
		theory=0;			//理论角度
		
		fast = ( -read_speed(2) + read_speed(4) )/2;	//TIM4：右电机，TIM2：左电机
		
		PWM0 = Vertical_Ring_PD(measure,theory) + Velocity_Ring_PI(fast);	//计算输出PWM
		
		Left_PWM = PWM0;
		
		Right_PWM = PWM0;
		
		Pwm_xianfu(7000,Left_PWM,Right_PWM);
		
//		if(Motor_enable==1)	//电机使能标志判断，1：给电机PWM，0：停止
//		{
			SetPwm(Left_PWM,Right_PWM);		
//		}
//		else
//		{
//			SetPwm(0,0);		//关闭电机
//		}
	}
	EXTI_ClearITPendingBit(EXTI_Line12); //清除的中断标志位  
}



