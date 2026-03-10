#include "sys.h"     
#include "ADC.h"     
#include "delay.h"     


void AD_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//禁止外部通道触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;//设置规则序列的长度
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);//开启ADC
	
	ADC_ResetCalibration(ADC1);//ADC校准开始
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

uint16_t AD_GetValue(uint8_t ADC_Channe2)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channe2, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能指定的ADC1的软件转换启动功能
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//转换完成标志，等待转换结束
	return ADC_GetConversionValue(ADC1);//返回ADC转换的结果数据
}


//ADC_Channel通道数，times：次数
uint16_t ADC_Get_ArrayValue(uint8_t ADC_Channe2,uint8_t times)
{
	uint32_t ValueNum=0;
	int i;
	for(i=0;i<times;i++)
	{
		ValueNum+=AD_GetValue(ADC_Channe2);
		delay_ms(5);
	}	
	return ValueNum/times;	
}
