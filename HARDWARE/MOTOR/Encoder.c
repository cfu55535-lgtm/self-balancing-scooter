#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "Encoder.h"


/**
	*@brief		定时器2编码器函数
	*@param		arr：自动重装值
	*@param		psc：分频系数
	*@retval	无
	*/
void Encoder_TIM2_Init(u16 arr,u16 psc)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//初始化GPIO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入模式或上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//TIM2的CH1和CH2对应Pin
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//初始化TIM2,设置ARR和RCC:	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	//输入捕获
	TIM_ICStructInit(&TIM_ICInitStructure);//初始化IC的其他参数，防止出现意外情况
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//选择通道1
	TIM_ICInitStructure.TIM_ICFilter=10;//滤波参数
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;//选择通道2
	TIM_ICInitStructure.TIM_ICFilter=10;//滤波参数
	TIM_ICInit(TIM2,&TIM_ICInitStructure);

	//配置TIMx编码器接口
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	//清除TIMx的挂起标志
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//启用TIM2中断
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
	
	//设置TIM2计数器寄存器值
	TIM_SetCounter(TIM2,0);
	
	//开启定时器
	TIM_Cmd(TIM2, ENABLE);

}


//TIM2中断函数
void TIM2_IRQHander(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==1)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);		
	}
}


/**
	*@brief		定时器4编码器函数
	*@param		arr：自动重装值
	*@param		psc：分频系数
	*@retval	无
	*/

void Encoder_TIM4_Init(u16 arr,u16 psc)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//初始化GPIO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入模式或上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;//TIM4的CH1和CH2对应Pin
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//初始化TIM4,设置ARR和RCC:	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	//输入捕获
	TIM_ICStructInit(&TIM_ICInitStructure);//初始化IC的其他参数，防止出现意外情况
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//选择通道1
	TIM_ICInitStructure.TIM_ICFilter=10;//滤波参数
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;//选择通道2
	TIM_ICInitStructure.TIM_ICFilter=10;//滤波参数
	TIM_ICInit(TIM2,&TIM_ICInitStructure);

	//配置TIMx编码器接口
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	//清除TIMx的挂起标志
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	
	//启用TIM2中断
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
	
	//设置TIM4计数器寄存器值
	TIM_SetCounter(TIM4,0);
	
	//开启定时器
	TIM_Cmd(TIM4, ENABLE);

}

//TIM4中断函数
void TIM4_IRQHander(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==1)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);		
	}
}


/**
	*@brief		获取编码器速度函数，非实际速度，即CNT的值
	*@param		TIMx：选择定时器
	*@retval	speed：速度
	*/
int read_speed(int TIMx)
{
	int speed;	
	switch(TIMx)
	{
		case 2:	speed=(short)TIM_GetCounter(TIM2);	//采集编码器的值,short:-32768
				TIM_SetCounter(TIM2,0);				//将定时器的计数值清零
				break;
		case 4:	speed=(short)TIM_GetCounter(TIM4);	//采集编码器的值
				TIM_SetCounter(TIM4,0);				//将定时器的计数值清零				
				break;	
		default:speed=0;
	}	
	return speed;
}
