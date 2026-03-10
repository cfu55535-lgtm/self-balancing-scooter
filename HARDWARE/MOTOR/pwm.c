#include "stm32f10x.h"                  // Device header
#include "pwm.h"


void PWM_TIM1_Init(u16 arr,u16 psc)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//初始化GPIO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	   //复用推完输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;//TIM1的CH1和CH4对应Pin
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//初始化TIM1,设置ARR和RCC:	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	//初始化输出比较参数
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//pwm1模式输出
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//极性不反转
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_Pulse=0;//设置通道1，CCR的值
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//pwm1模式输出
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//极性不反转
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_Pulse=0;//设置通道4，CCR的值
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);

	//预装载寄存器使能
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC1预装载寄存器使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//OC4预装载寄存器使能
	
	//ARR上预装载寄存器使能
	TIM_ARRPreloadConfig(TIM1,ENABLE);//TIM1在ARR上预装载寄存器使能

	//定时器使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//TIM1是高级定时器，需要总输出使能
	
	//开启定时器
	TIM_Cmd(TIM1, ENABLE);

	//先置占空比为0
	TIM_SetCompare1(TIM1,0);
	TIM_SetCompare4(TIM1,0);	
}

