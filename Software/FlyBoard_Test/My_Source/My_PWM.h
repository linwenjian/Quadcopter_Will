/*
 * My_PWM.h
 *
 *  Created on: 2014-8-10
 *      Author: FGZ
 */

#ifndef MY_PWM_H_
#define MY_PWM_H_

#include"Common.h"

#if 0

#define ESC_50_Hz

#endif

#define PWM1 1
#define PWM2 2
#define PWM3 3
#define PWM4 4

#ifdef ESC_50_Hz

#define Throttle_MAX 5000
#define Throttle_MIN 2630
#define Motor_Closed 2500

#else

#define Throttle_MAX 5620
#define Throttle_MIN 2890
#define Throttle_START 3800
#define Motor_Closed 2735

#endif

void init_PWM(void);
void enable_PWM(void);
uint8_t PWM_duty_Set(uint8_t PWM_n,uint16_t duty);
void setMotorPowerMax(void);
void setMotorPowerMin(void);



#endif /* MY_PWM_H_ */
