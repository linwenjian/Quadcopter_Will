//-----------------------------------------------------------------
//文件名称：mpu6050dmp_attitude_angle_cal.h
//功能概要：mpu6050 dmp读取姿态角文件
//版本更新：2013-11-06
//-----------------------------------------------------------------
#ifndef _MPU6050DMP_ATTITUDE_ANGLE_CAL_H_
#define _MPU6050DMP_ATTITUDE_ANGLE_CAL_H_

//包含头文件
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"
#include "stdint.h"

//------------------------------ 外部接口函数 ------------------------------//

//-----------------------------------------------------------------
//函数名称：mpu6050dmp_init
//功能概要：mpu6050和dmp初始化
//函数返回：void
//参数说明：void
//-----------------------------------------------------------------

//float pitch, roll, yaw;
extern float My_Pitch, My_Roll, My_Yaw;

void mpu6050dmp_init(void);

//-----------------------------------------------------------------
//函数名称:attitude_angle_cal
//功能概要:mpu6050的dmp读取姿态角(Pitch,Roll,Yaw:俯仰角，横滚角，导航角)并返回加速度计、陀螺仪值
//函数返回:void
//参数说明:angle:指向三个姿态角的首地址，放大10倍输出
//      gyro:指向三轴角速度的首地址 (gyro[0] :y(roll); gyro[1] :x(pitch);gyro[2] :z(yaw))(无高通滤波)
//      accel:指向三轴加速度的首地址(accel[0]:x(pitch);accel[1]:y(roll); accel[2]:z(yaw))
//-----------------------------------------------------------------
void init_MPU6050(void);
void attitude_angle_cal(int16_t* angle, int16_t* gyro, int16_t* accel);
void get_AttitudeVal(short *gyroVal);

#endif
