#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"                 


#define	AIN1	PAout(15)
#define AIN2	PBout(3)
#define BIN2	PBout(4)
#define BIN1	PBout(5)

void Motor_GPIO_Init(void);
void motor_Init(void);
void SetPwm(int Left_PWM,int Right_PWM);
void Pwm_xianfu(int PwmMax,int Left_PWM,int Right_PWM);

#endif

