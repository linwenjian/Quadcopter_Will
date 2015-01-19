/*
 * Motor.h
 *
 *  Created on: 2014-11-15
 *      Author: FGZ
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include "Common.h"
#include "attitude_Control.h"
#include "My_PWM.h"
#include "mpu6050dmp_attitude_angle_cal.h"

#define MOTOR1 1
#define MOTOR2 2
#define MOTOR3 3
#define MOTOR4 4

//#define

void Motor_OUTPut(uint8_t flag, uint16_t *motor_Val);

#endif /* MOTOR_H_ */
