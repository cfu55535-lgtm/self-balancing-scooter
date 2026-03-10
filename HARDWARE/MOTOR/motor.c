#include "stm32f10x.h"      
#include "motor.h"
#include "pwm.h"
#include "Encoder.h"
#include "delay.h"
#include "OLED.h"

extern float pwmout;	//速度环+直立环输出PWM


void motor_Init(void)
{
	Motor_GPIO_Init();
	Encoder_TIM4_Init(65536-1,0);
	Encoder_TIM2_Init(65536-1,0);
	PWM_TIM1_Init(7200-1,1-1);
	
}


//PWM：   左PA11
//        右PA8
//正反转：左 AIN1:PB3   AIN2:PB15
//        右 BIN1:PB4   BIN2:PB5

void Motor_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//PA15默认为JTAG引脚，变为普通引脚
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;		//AIN1对应Pin
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//AIN2,BIN1,BIN2对应Pin
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}

/**
	*@brief		设置PWM函数
	*@param		PID输出值，即PWM
	*@retval	无
	*/
void SetPwm(int Left_PWM,int Right_PWM)
{
	if(Left_PWM>0)		
	{
		BIN1=1,BIN2=0;		
		TIM_SetCompare4(TIM1,Left_PWM);		
	}
	
	if(Left_PWM<0)		
	{
		BIN1=0,BIN2=1;		
		TIM_SetCompare4(TIM1,-Left_PWM);		
	}
	
	if(Right_PWM>0)	
	{
		AIN1=1,AIN2=0;		
		TIM_SetCompare1(TIM1,Right_PWM);
	}
	
	if(Right_PWM<0)	
	{
		AIN1=0,AIN2=1;		
		TIM_SetCompare1(TIM1,-Right_PWM);
	}
	
//	else					//关闭电机
//	{
//		AIN1=0;
//		AIN2=0;
//		BIN1=0;
//		BIN2=0;
//		TIM_SetCompare1(TIM1,0);
//		TIM_SetCompare4(TIM4,0);		
//	}
		
}


/**
	*@brief		PWM限幅函数
	*@param		PwmMax：PWM最大值
	*@param		Left_PWM：PWM目前实际值
	*@param		Right_PWM：PWM目前实际值
	*@retval	无
	*/
void Pwm_xianfu(int PwmMax,int Left_PWM,int Right_PWM)
{
	if(Left_PWM >= PwmMax)
		Left_PWM = PwmMax;
	
	if(Left_PWM <= -PwmMax)
		Left_PWM = -PwmMax;
	
	if(Right_PWM >= PwmMax)
		Right_PWM = PwmMax;
	
	if(Right_PWM <= -PwmMax)
		Right_PWM = -PwmMax;

}
