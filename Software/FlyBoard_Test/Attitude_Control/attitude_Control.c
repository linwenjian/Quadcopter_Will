/*
 * attitude_Control.c
 *
 *  Created on: 2014-11-12
 *      Author: FGZ
 */
#include "attitude_Control.h"

PID PID_ROL, PID_PIT, PID_YAW;
extern ideal_VAL Attitude;
integration SUM_ERROR;
extern uint8_t stopFlag;
extern uint8_t startFlag;
extern uint16_t delayTimes;

void clearIntegration(uint8_t flag)
{
    if(flag == 1)
    {
        SUM_ERROR.PIT = 0;
        SUM_ERROR.ROL = 0;
        SUM_ERROR.YAW = 0;
        SUM_ERROR.GYRO_X = 0;
        SUM_ERROR.GYRO_Y = 0;
        SUM_ERROR.GYRO_Z = 0;
    }
}

void init_Control(void)
{
    clearIntegration(1);

    Attitude.ideal_Pit = 0;
    Attitude.ideal_Rol = 0;
    Attitude.ideal_Yaw = 0;
    Attitude.ideal_gyro[0] = 0;
    Attitude.ideal_gyro[1] = 0;
    Attitude.ideal_gyro[2] = 0;
    Attitude.PIT_PID_out = 0;
    Attitude.ROL_PID_out = 0;
    Attitude.YAW_PID_out = 0;
    Attitude.Throttle_Hight = Throttle_START;

    PID_PIT.P[0] = 3.0f;
    PID_PIT.I[0] = 0;
    PID_PIT.D[0] = 0;
    PID_PIT.P[1] = 2.5f;
    PID_PIT.I[1] = 0.1f;
    PID_PIT.D[1] = 4.5f;

    PID_ROL.P[0] = 3.0f;
    PID_ROL.I[0] = 0;
    PID_ROL.D[0] = 0;
    PID_ROL.P[1] = 2.5f;
    PID_ROL.I[1] = 0.1f;
    PID_ROL.D[1] = 4.5f;

#if 1
    PID_YAW.P[0] = 7.5f;
    PID_YAW.I[0] = 0;
    PID_YAW.D[0] = 0;
    PID_YAW.P[1] = 7.5f;
    PID_YAW.I[1] = 0.05f;
    PID_YAW.D[1] = 0;
#else
    PID_YAW.P[0] = 0;
    PID_YAW.I[0] = 0;
    PID_YAW.D[0] = 0;
    PID_YAW.P[1] = 0;
    PID_YAW.I[1] = 0;
    PID_YAW.D[1] = 0;
#endif

    stopFlag = 1;
    startFlag = 0;
    delayTimes = 0;
}

float List_MaxMin(float num, float max, float min)
{
    if(num > max)
    {
        return max;
    }
    else if(num < min)
    {
        return min;
    }
    else
    {
        return num;
    }
}

float incremental_PID(float Pk, float Ik, float Dk, float idealVal, float nowVal)
{
    static float inc_pidOutPut= 0;
    static float inc_error[3] = {0, 0, 0};

    inc_error[0] = idealVal - nowVal;

    inc_pidOutPut += Pk * (inc_error[0] - inc_error[1]) + Ik * inc_error[0] + Dk * (inc_error[0] + inc_error[2] - 2 * inc_error[1]);

    inc_error[1] = inc_error[0];
    inc_error[2] = inc_error[1];

    return inc_pidOutPut;
}

float positional_PIT_PID(float Pk, float Ik, float Dk, float idealVal, float nowVal)
{
    static float pos_pidOutPut = 0;
    static float pos_error[2] = {0, 0};
    //static float sum_error = 0;

    pos_error[0] = idealVal - nowVal;
    SUM_ERROR.PIT += pos_error[0];

    SUM_ERROR.PIT = List_MaxMin(SUM_ERROR.PIT, Isum_MAX, Isum_MIN);

    pos_pidOutPut = Pk * pos_error[0] + Ik * SUM_ERROR.PIT + Dk * (pos_error[0] - pos_error[1]);

    pos_error[1] = pos_error[0];

    pos_pidOutPut = List_MaxMin(pos_pidOutPut, Isum_MAX, Isum_MIN);

    return pos_pidOutPut;
}

float positional_ROL_PID(float Pk, float Ik, float Dk, float idealVal, float nowVal)
{
    static float pos_pidOutPut = 0;
    static float pos_error[2] = {0, 0};
    //static float sum_error = 0;

    pos_error[0] = idealVal - nowVal;
    SUM_ERROR.ROL += pos_error[0];

    SUM_ERROR.ROL = List_MaxMin(SUM_ERROR.ROL, Isum_MAX, Isum_MIN);

    pos_pidOutPut = Pk * pos_error[0] + Ik * SUM_ERROR.ROL + Dk * (pos_error[0] - pos_error[1]);

    pos_error[1] = pos_error[0];

    pos_pidOutPut = List_MaxMin(pos_pidOutPut, Isum_MAX, Isum_MIN);

    return pos_pidOutPut;
}

float positional_YAW_PID(float Pk, float Ik, float Dk, float idealVal, float nowVal)
{
    static float pos_pidOutPut = 0;
    static float pos_error[2] = {0, 0};
    //static float sum_error = 0;

    pos_error[0] = idealVal - nowVal;
    SUM_ERROR.YAW += pos_error[0];

    SUM_ERROR.YAW = List_MaxMin(SUM_ERROR.YAW, Isum_MAX, Isum_MIN);

    pos_pidOutPut = Pk * pos_error[0] + Ik * SUM_ERROR.YAW + Dk * (pos_error[0] - pos_error[1]);

    pos_error[1] = pos_error[0];

    pos_pidOutPut = List_MaxMin(pos_pidOutPut, Isum_MAX, Isum_MIN);

    return pos_pidOutPut;
}

float positional_X_GYRO_PID(float Pk, float Ik, float Dk, float idealVal, float nowVal)
{
    static float pos_pidOutPut = 0;
    static float pos_error[2] = {0, 0};

    static float lowPass[3] = {0, 0, 0};

    pos_error[0] = idealVal - nowVal;

    SUM_ERROR.GYRO_X += pos_error[0];

    SUM_ERROR.GYRO_X = List_MaxMin(SUM_ERROR.GYRO_X, Isum_MAX / (Ik + 0.0000001f), Isum_MIN / (Ik + 0.0000001f));

    lowPass[0] = pos_error[0] - pos_error[1];

    lowPass[2] = lowPass[0] * lowPassVal + lowPass[1] * (1 - lowPassVal);

    pos_pidOutPut = Pk * pos_error[0] + Ik * SUM_ERROR.GYRO_X + Dk * lowPass[2];

    pos_error[1] = pos_error[0];

    lowPass[1] = lowPass[0];

    pos_pidOutPut = List_MaxMin(pos_pidOutPut, Isum_MAX, Isum_MIN);

#if 0
    sendLineX(MCU1, 0X1F, pos_pidOutPut);
    sendLineX(MCU1, 0X2F, Pk * pos_error[0]);
    sendLineX(MCU1, 0X3F, Ik * SUM_ERROR.GYRO_X);

    sendLineX(MCU1, 0X4F, idealVal);
    sendLineX(MCU1, 0X5F, nowVal);
    sendLineX(MCU1, 0X6F, roll);
#endif

    return pos_pidOutPut;
}

float positional_Y_GYRO_PID(float Pk, float Ik, float Dk, float idealVal, float nowVal)
{
    static float pos_pidOutPut = 0;
    static float pos_error[2] = {0, 0};

    static float lowPass[3] = {0, 0, 0};

    pos_error[0] = idealVal - nowVal;

    SUM_ERROR.GYRO_Y += pos_error[0];

    SUM_ERROR.GYRO_Y = List_MaxMin(SUM_ERROR.GYRO_Y, Isum_MAX / (Ik + 0.0000001f), Isum_MIN / (Ik + 0.0000001f));

    lowPass[0] = pos_error[0] - pos_error[1];

    lowPass[2] = lowPass[0] * lowPassVal + lowPass[1] * (1 - lowPassVal);

    pos_pidOutPut = Pk * pos_error[0] + Ik * SUM_ERROR.GYRO_Y + Dk * lowPass[2];

    pos_error[1] = pos_error[0];

    lowPass[1] = lowPass[0];

    pos_pidOutPut = List_MaxMin(pos_pidOutPut, Isum_MAX, Isum_MIN);

#if 0
    sendLineX(MCU1, 0X1F, pos_pidOutPut);
    sendLineX(MCU1, 0X2F, Pk * pos_error[0]);
    sendLineX(MCU1, 0X3F, Ik * SUM_ERROR.GYRO_Y);

    sendLineX(MCU1, 0X4F, idealVal);
    sendLineX(MCU1, 0X5F, nowVal);
    sendLineX(MCU1, 0X6F, pitch);
#endif

    return pos_pidOutPut;
}

float positional_Z_GYRO_PID(float Pk, float Ik, float Dk, float idealVal, float nowVal)
{
    static float pos_pidOutPut = 0;
    static float pos_error[2] = {0, 0};

    static float lowPass[3] = {0, 0, 0};

    pos_error[0] = idealVal - nowVal;
    SUM_ERROR.GYRO_Z += pos_error[0];

    SUM_ERROR.GYRO_Z = List_MaxMin(SUM_ERROR.GYRO_Z, Isum_MAX / (Ik + 0.0000001f), Isum_MIN / (Ik + 0.0000001f));

    lowPass[0] = pos_error[0] - pos_error[1];

    lowPass[2] = lowPass[0] * lowPassVal + lowPass[1] * (1 - lowPassVal);

    pos_pidOutPut = Pk * pos_error[0] + Ik * SUM_ERROR.GYRO_Z + Dk * lowPass[2];

    pos_error[1] = pos_error[0];

    lowPass[1] = lowPass[0];

    pos_pidOutPut = List_MaxMin(pos_pidOutPut, Isum_MAX, Isum_MIN);

#if 0
    sendLineX(MCU1, 0X4F, Pk * pos_error[0]);
    sendLineX(MCU1, 0X5F, Ik * SUM_ERROR.GYRO_Z);
    sendLineX(MCU1, 0X6F, pos_pidOutPut);
#endif

    return pos_pidOutPut;
}

extern float myYawVal;

void control_Attitudes(short *gyroVal)
{
    uint16_t motor_Val[4] = {Throttle_MIN, Throttle_MIN, Throttle_MIN, Throttle_MIN};
    float gyroTmp[3];
    float pitchTmp, rollTmp, yawTmp;
    static float SUN_yaw = 0;
    static float SUN_gyro[2] = {0, 0};

    gyroTmp[0] = (float)gyroVal[0] / GyroSensitivity;
    gyroTmp[1] = (float)gyroVal[1] / GyroSensitivity;
    gyroTmp[2] = (float)gyroVal[2] / GyroSensitivity;

    if(delayTimes > START_TIME)
    {
#ifdef SIDE_DOWN

        pitchTmp = -1 * pitch;
        rollTmp = roll;
        yawTmp = yaw - SUN_yaw;
        myYawVal = yawTmp;

        gyroTmp[0] = gyroTmp[0] - SUN_gyro[0];
        gyroTmp[1] = gyroTmp[1] - SUN_gyro[1];

#else
        pitchTmp = -1 * pitch;
        rollTmp = roll;
        yawTmp = yaw - SUN_yaw;
        myYawVal = yawTmp;

        gyroTmp[0] = gyroTmp[0] - SUN_gyro[0];
        gyroTmp[1] = -1 * (gyroTmp[1] - SUN_gyro[1]);
        gyroTmp[2] *= -1;
#endif

        clearIntegration(stopFlag);

        Attitude.ideal_gyro[1] = positional_PIT_PID(PID_PIT.P[0], PID_PIT.I[0], PID_PIT.D[0], Attitude.ideal_Pit, pitchTmp);
        Attitude.ideal_gyro[0] = positional_ROL_PID(PID_ROL.P[0], PID_ROL.I[0], PID_ROL.D[0], Attitude.ideal_Rol, rollTmp);
        Attitude.ideal_gyro[2] = positional_YAW_PID(PID_YAW.P[0], PID_YAW.I[0], PID_YAW.D[0], Attitude.ideal_Yaw, yawTmp);

        Attitude.PIT_PID_out = positional_Y_GYRO_PID(PID_PIT.P[1], PID_PIT.I[1], PID_PIT.D[1], Attitude.ideal_gyro[1], gyroTmp[1]);
        Attitude.ROL_PID_out = positional_X_GYRO_PID(PID_ROL.P[1], PID_ROL.I[1], PID_ROL.D[1], Attitude.ideal_gyro[0], gyroTmp[0]);

#ifndef change_Yaw_Val
        Attitude.YAW_PID_out = positional_Z_GYRO_PID(PID_YAW.P[1], PID_YAW.I[1], PID_YAW.D[1], Attitude.ideal_gyro[2], gyroTmp[2]);
#endif

        motor_Val[0] = (uint16_t)(Attitude.Throttle_Hight + Attitude.PIT_PID_out - Attitude.ROL_PID_out - Attitude.YAW_PID_out);
        motor_Val[1] = (uint16_t)(Attitude.Throttle_Hight + Attitude.PIT_PID_out + Attitude.ROL_PID_out + Attitude.YAW_PID_out);
        motor_Val[2] = (uint16_t)(Attitude.Throttle_Hight - Attitude.PIT_PID_out + Attitude.ROL_PID_out - Attitude.YAW_PID_out);
        motor_Val[3] = (uint16_t)(Attitude.Throttle_Hight - Attitude.PIT_PID_out - Attitude.ROL_PID_out + Attitude.YAW_PID_out);

        motor_Val[0] = List_MaxMin(motor_Val[0], Throttle_MAX, Throttle_MIN);
        motor_Val[1] = List_MaxMin(motor_Val[1], Throttle_MAX, Throttle_MIN);
        motor_Val[2] = List_MaxMin(motor_Val[2], Throttle_MAX, Throttle_MIN);
        motor_Val[3] = List_MaxMin(motor_Val[3], Throttle_MAX, Throttle_MIN);

        if(pitch * pitch >= angleProtect || roll * roll >= angleProtect)
        {
            stopFlag = 1;
            startFlag = 0;
            delayTimes = 0;
            clearIntegration(1);
        }
    }
    else
    {
        motor_Val[0] = Throttle_START;
        motor_Val[1] = Throttle_START;
        motor_Val[2] = Throttle_START;
        motor_Val[3] = Throttle_START;

        Attitude.Throttle_Hight = Throttle_START;

        if(startFlag == 1)
        {
            delayTimes++;
        }

        if(delayTimes == 1)
        {
            SUN_yaw = 0;
            SUN_gyro[0] = 0;
            SUN_gyro[1] = 0;
        }

        if(delayTimes >= (START_TIME * 8 / 10)  && delayTimes < START_TIME)
        {
            SUN_yaw += yaw;
            SUN_gyro[0] += gyroTmp[0];
            SUN_gyro[1] += gyroTmp[1];
        }
        else if(delayTimes == START_TIME)
        {
            SUN_yaw /= (START_TIME * 2 / 10);
            SUN_gyro[0] /= (START_TIME * 2 / 10);
            SUN_gyro[1] /= (START_TIME * 2 / 10);
        }
    }

    Motor_OUTPut(!startFlag, motor_Val);

#if 0
    sendLineX(MCU1, 0X1F, Attitude.PIT_PID_out);
    sendLineX(MCU1, 0X2F, Attitude.ROL_PID_out);
    sendLineX(MCU1, 0X3F, Attitude.YAW_PID_out);

    sendLineX(MCU1, 0X4F, -1 * pitch);
    sendLineX(MCU1, 0X5F, roll);
    sendLineX(MCU1, 0X6F, yaw);
#endif
}

void PID_Val_Set(uint8_t flag, float Val)
{
    switch(flag)
    {
        case 1:
#ifdef debug_PIT_ROL
          PID_PIT.P[0] = Val;
          PID_ROL.P[0] = Val;
#else
          PID_YAW.P[0] = Val;
#endif
          break;
        case 2:
#ifdef debug_PIT_ROL
          PID_PIT.I[0] = Val;
          PID_ROL.I[0] = Val;
#else
          PID_YAW.I[0] = Val;
#endif
          break;
        case 3:
#ifdef debug_PIT_ROL
          PID_PIT.D[0] = Val;
          PID_ROL.D[0] = Val;
#else
          PID_YAW.D[0] = Val;
#endif
          break;
        case 4:
#ifdef debug_PIT_ROL
          PID_PIT.P[1] = Val;
          PID_ROL.P[1] = Val;
#else
          PID_YAW.P[1] = Val;
#endif
          break;
        case 5:
#ifdef debug_PIT_ROL
          PID_PIT.I[1] = Val;
          PID_ROL.I[1] = Val;
#else
          PID_YAW.I[1] = Val;
#endif
          break;
        case 6:
#ifdef debug_PIT_ROL
          PID_PIT.D[1] = Val;
          PID_ROL.D[1] = Val;
#else
          PID_YAW.D[1] = Val;
#endif
          break;
        case 11:
          Attitude.Throttle_Hight = Throttle_MIN + Val * 2;
          if(Val == 0)
            {
              stopFlag = 1;
              startFlag = 0;
              delayTimes = 0;
            }
          else
            {
              stopFlag = 0;
            }
          break;
        case 12:
          Attitude.ideal_Pit = Val;
          break;
        case 13:
          Attitude.ideal_Rol = Val;
          break;
        case 14:
          Attitude.ideal_Yaw = Val;
          break;
        case 15:
          stopFlag = 1;
          startFlag = 0;
          delayTimes = 0;
          break;
        case 16:
          startFlag = 1;
          break;
    }
}
