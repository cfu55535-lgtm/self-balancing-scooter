#ifndef __ADC_H
#define __ADC_H

#include "sys.h"   

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
uint16_t ADC_Get_ArrayValue(uint8_t ADC_Channel,uint8_t times);
	
	
#endif


