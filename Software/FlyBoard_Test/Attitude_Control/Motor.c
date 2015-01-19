/*
 * Motor.c
 *
 *  Created on: 2014-11-15
 *      Author: FGZ
 */
#include "Motor.h"
#include "My_PWM.h"

void Motor_OUTPut(uint8_t flag, uint16_t *motor_Val)
{
    if(flag != 1)
    {
      PWM_duty_Set(PWM1, motor_Val[0]);
      PWM_duty_Set(PWM2, motor_Val[1]);
      PWM_duty_Set(PWM3, motor_Val[2]);
      PWM_duty_Set(PWM4, motor_Val[3]);
    }
    else
    {
        PWM_duty_Set(PWM1, Motor_Closed);
        PWM_duty_Set(PWM2, Motor_Closed);
        PWM_duty_Set(PWM3, Motor_Closed);
        PWM_duty_Set(PWM4, Motor_Closed);
    }
}

