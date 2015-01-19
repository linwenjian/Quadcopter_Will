/*
 * attitude_Control.h
 *
 *  Created on: 2014-11-12
 *      Author: FGZ
 */

#ifndef ATTITUDE_CONTROL_H_
#define ATTITUDE_CONTROL_H_

#include "Common.h"
#include "Motor.h"
#include "My_UART.h"

#define START_TIME 500 // Ms

#define angleProtect 2025 // 45 angle

extern uint8_t stopFlag;
extern uint8_t startFlag;
extern uint16_t delayTimes;
extern float myYawVal;

//#define change_Yaw_Val

typedef struct
{
    float P[2];
    float I[2];
    float D[2];
}PID;

typedef struct
{
    float ideal_Pit;
    float ideal_Rol;
    float ideal_Yaw;
    float ideal_gyro[3];
    float PIT_PID_out;
    float ROL_PID_out;
    float YAW_PID_out;
    float Throttle_Hight;
}ideal_VAL;
extern ideal_VAL Attitude;

typedef struct
{
    float PIT;
    float ROL;
    float YAW;
    float GYRO_X;
    float GYRO_Y;
    float GYRO_Z;
}integration;

extern PID PID_ROL,PID_PIT,PID_YAW;
extern ideal_VAL Attitude;

#define Isum_MAX 3500
#define Isum_MIN -3500
#define lowPassVal 0.5

#define GyroSensitivity 16.4
#define AcclSensitivity 16384
#define debug_PIT_ROL

void init_Control(void);
void clearIntegration(uint8_t flag);
void PID_Val_Set(uint8_t flag, float Val);
void control_Attitudes(short *gyroVal);

#endif /* ATTITUDE_CONTROL_H_ */
