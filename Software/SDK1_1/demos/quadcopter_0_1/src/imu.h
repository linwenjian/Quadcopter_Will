
#ifndef _IMU_H_
#define _IMU_H_

#include <math.h>
#include <stdint.h>


#define SLIDING_FILTER_DEEP     (20)

//!< return 0 if you get data succ. reutrn else if data get failed
typedef struct
{
    int (*imu_get_accel)(int16_t * ax, int16_t * ay, int16_t * az);
    int (*imu_get_gyro)(int16_t * gx, int16_t * gy, int16_t * gz);
    int (*imu_get_mag)(int16_t * mx, int16_t * my, int16_t * mz);
}imu_io_install_t;

typedef struct _imu_float_euler_angle
{
    double imu_pitch;
    double imu_roll;
    double imu_yaw;
}imu_float_euler_angle_t;

typedef struct 
{
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
    int16_t mx;
    int16_t my;
    int16_t mz;
}imu_raw_data_t;

typedef struct 
{
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
    double mx;
    double my;
    double  mz;
}imu_float_data_t;

//!< API functions
uint32_t imu_io_install(imu_io_install_t * IOInstallStruct);
uint32_t imu_get_euler_angle(imu_float_euler_angle_t * angle, mems_data_t * pRawDdata);
double invSqrt(double x) ;

extern double gyro_pitch_global;
extern double gyro_roll_global;
#endif
