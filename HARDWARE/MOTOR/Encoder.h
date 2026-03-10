#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"
#include "sys.h"                  

void Encoder_TIM2_Init(u16 arr,u16 psc);
void Encoder_TIM4_Init(u16 arr,u16 psc);
int read_speed(int TIMx);

#endif

