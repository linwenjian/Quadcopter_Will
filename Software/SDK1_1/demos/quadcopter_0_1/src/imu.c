#include "quad_common.h"

/*******************************************************************************
 * Defination
 ******************************************************************************/
//#define PI             3.1415926f
//#define Kp             10.0f     /* proportional gain governs rate of convergence to accelerometer/magnetometer */
//#define Ki             0.005f     /* integral gain governs rate of convergence of gyroscope biases */
//#define halfT          0.002f //陀螺仪采集周期的一半
//#define Gyro_G         0.0610351f
//#define Gyro_Gr        0.0010653f //弧度/秒    
////根据MPU6050手册， 2000满量程，Sensitivity Scale Factor = 16.4
////陀螺仪的值x * (1 / 16.4) 度/秒， 换算成弧度 x*(1/16.4)*0.01745329252 = x * 0.001064225154
////1° = pi / 180 = 0.01745329252 rad


#define PI             3.1415926f
#define Kp             10.0f     /* proportional gain governs rate of convergence to accelerometer/magnetometer */
#define Ki             0.005f     /* integral gain governs rate of convergence of gyroscope biases */
#define halfT          0.001f //陀螺仪采集周期的一半
#define Gyro_G         (0.07f)
#define Gyro_Gr        (0.00122173f) //弧度/秒 
//L3G4200D 陀螺仪的值x * 70 / 1000 度/秒， 换算成弧度 x * 70 / 1000 *0.01745329252 = x * 0.00122173


static imu_io_install_t * gpIOInstallStruct;   /* install struct	*/
 /*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t imu_io_install(imu_io_install_t * IOInstallStruct)
{
    gpIOInstallStruct = IOInstallStruct;
    return 0;
}

/* sliding filter */
static uint32_t imu_sliding_filter(imu_raw_data_t raw_data, imu_raw_data_t * filter_data)
{
    int32_t sum_accel_x = 0;
    int32_t sum_accel_y = 0;
    int32_t sum_accel_z = 0;
    int32_t sum_gyro_x = 0;
    int32_t sum_gyro_y = 0;
    int32_t sum_gyro_z = 0;
    
    /* fifo */
    static int16_t fifo_accel_x[SLIDING_FILTER_DEEP];
    static int16_t fifo_accel_y[SLIDING_FILTER_DEEP];
    static int16_t fifo_accel_z[SLIDING_FILTER_DEEP];
    static int16_t fifo_gyro_x[SLIDING_FILTER_DEEP];
    static int16_t fifo_gyro_y[SLIDING_FILTER_DEEP];
    static int16_t fifo_gyro_z[SLIDING_FILTER_DEEP];
    
    for(int i = SLIDING_FILTER_DEEP-1; i > 0; i--)
    {
        fifo_accel_x[i] =  fifo_accel_x[i-1];
        fifo_accel_y[i] =  fifo_accel_y[i-1];
        fifo_accel_z[i] =  fifo_accel_z[i-1];		
        fifo_gyro_x[i] =  fifo_gyro_x[i-1];
        fifo_gyro_y[i] =  fifo_gyro_y[i-1];
        fifo_gyro_z[i] =  fifo_gyro_z[i-1];
    }
    
    /* fifo input */
    fifo_accel_x[0] = raw_data.ax;
    fifo_accel_y[0] = raw_data.ay;
    fifo_accel_z[0] = raw_data.az;
    fifo_gyro_x[0] = raw_data.gx;
    fifo_gyro_y[0] = raw_data.gy;
    fifo_gyro_z[0] = raw_data.gz;
    
    /** fifo calculate */
    for(int i=0; i < SLIDING_FILTER_DEEP; i++)
    {
        sum_accel_x += fifo_accel_x[i];
        sum_accel_y += fifo_accel_y[i];
        sum_accel_z += fifo_accel_z[i];
        sum_gyro_x += fifo_gyro_x[i];
        sum_gyro_y += fifo_gyro_y[i];
        sum_gyro_z += fifo_gyro_z[i];	
    }
    filter_data->ax = sum_accel_x/SLIDING_FILTER_DEEP;
    filter_data->ay = sum_accel_y/SLIDING_FILTER_DEEP; 
    filter_data->az = sum_accel_z/SLIDING_FILTER_DEEP; 
    filter_data->gx = sum_gyro_x/SLIDING_FILTER_DEEP;
    filter_data->gy = sum_gyro_y/SLIDING_FILTER_DEEP; 
    filter_data->gz = sum_gyro_z/SLIDING_FILTER_DEEP; 
    filter_data->mx = raw_data.mx;
    filter_data->my = raw_data.my;
    filter_data->mz = raw_data.mz; 
    return 0;
}


//!< format data into float value
static uint32_t imu_format_data(imu_raw_data_t * raw_data, imu_float_data_t * float_data)
{
    float_data->ax = (double)raw_data->ax;
    float_data->ay = (double)raw_data->ay;
    float_data->az = (double)raw_data->az;
    
    float_data->gx = (double)raw_data->gx;
    float_data->gy = (double)raw_data->gy;
    float_data->gz = (double)raw_data->gz;
    
    float_data->mx = (double)raw_data->mx;
    float_data->my = (double)raw_data->my;
    float_data->mz = (double)raw_data->mz;
    return 0;
}

//=====================================================================================================
// AHRS.c
// S.O.H. Madgwick
// 25th August 2010
//=====================================================================================================
// Description:
//
// Quaternion implementation of the 'DCM filter' [Mayhony et al].  Incorporates the magnetic distortion
// compensation algorithms from my filter [Madgwick] which eliminates the need for a reference
// direction of flux (bx bz) to be predefined and limits the effect of magnetic distortions to yaw
// axis only.
//
// User must define 'halfT' as the (sample period / 2), and the filter gains 'Kp' and 'Ki'.
//
// Global variables 'q0', 'q1', 'q2', 'q3' are the quaternion elements representing the estimated
// orientation.  See my report for an overview of the use of quaternions in this application.
//
// User must call 'AHRSupdate()' every sample period and parse calibrated gyroscope ('gx', 'gy', 'gz'),
// accelerometer ('ax', 'ay', 'ay') and magnetometer ('mx', 'my', 'mz') data.  Gyroscope units are
// radians/second, accelerometer and magnetometer units are irrelevant as the vector is normalised.
//
//=====================================================================================================

#if 0
static void updateAHRS(double gx,double gy,double gz,
                       double ax,double ay,double az,
                       double mx,double mz,double my, 
                       imu_float_euler_angle_t * angle)
{
  double norm;
  double hx, hy, hz, bx, bz;
  double vx, vy, vz, wx, wy, wz;
  double ex, ey, ez;
  
  static double q0 = 1.0; 
  static double q1 = 0.0; 
  static double q2 = 0.0;
  static double q3 = 0.0;
  static double exInt = 0.0, eyInt = 0.0, ezInt = 0.0;
  
  double iq0,iq1,iq2,iq3;
  
  double q0q0 = q0*q0;
  double q0q1 = q0*q1;
  double q0q2 = q0*q2;
  double q0q3 = q0*q3;
  double q1q1 = q1*q1;
  double q1q2 = q1*q2;
  double q1q3 = q1*q3;
  double q2q2 = q2*q2;
  double q2q3 = q2*q3;
  double q3q3 = q3*q3;
  
//  if(ax*ay*az==0)
//  {
//    return;    
//  }
  
  norm = sqrt(ax*ax + ay*ay + az*az);
  if(0 == norm)
  {
    return;    
  }
  ax = ax / norm;
  ay = ay / norm;
  az = az / norm;
  
  norm = sqrt(mx*mx + my*my + mz*mz);
  if(0 == norm)
  {
    return;    
  }
  mx = mx / norm;
  my = my / norm;
  mz = mz / norm;
  
  /* compute reference direction of flux */
  hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
  hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
  hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);         
  
  bx = sqrt((hx*hx) + (hy*hy));
  bz = hz; 
  
  /* estimated direction of gravity and flux (v and w) */
  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;
  
  wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
  wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
  wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);
  
  /* error is sum of cross product between reference direction of fields and direction measured by sensors */
  ex = (ay*vz - az*vy) + (my*wz - mz*wy);
  ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx) + (mx*wy - my*wx);
  
  exInt = exInt + ex * Ki;
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;
  
  /* adjusted gyroscope measurements */
  gx = gx + Kp*ex + exInt;
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;
  
  /* integrate quaternion rate and normalise */
  iq0 = (-q1*gx - q2*gy - q3*gz)*halfT;
  iq1 = (q0*gx + q2*gz - q3*gy)*halfT;
  iq2 = (q0*gy - q1*gz + q3*gx)*halfT;
  iq3 = (q0*gz + q1*gy - q2*gx)*halfT;
  
  q0 += iq0;
  q1 += iq1;
  q2 += iq2;
  q3 += iq3;
  
  /* normalise quaternion */
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
  
  /* output data */
  angle->imu_yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; 
  angle->imu_pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;																			// pitcho???
  angle->imu_roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;

return;
}

#else
static void updateAHRS(double gx,double gy,double gz,
                       double ax,double ay,double az,
                       double mx,double mz,double my, 
                       imu_float_euler_angle_t * angle)
{
  
  double norm;
  double hx, hy, hz, bx, bz;
  double vx, vy, vz, wx, wy, wz;
  double ex, ey, ez;
  
  static double q0 = 1.0; 
  static double q1 = 0.0; 
  static double q2 = 0.0;
  static double q3 = 0.0;
  static double exInt = 0.0, eyInt = 0.0, ezInt = 0.0;
	
	//?2μ?2?ê?:???aêy3?·¨????
	double q0q0 = q0 * q0;							
	double q0q1 = q0 * q1;
	double q0q2 = q0 * q2;
	double q1q1 = q1 * q1;
	double q1q3 = q1 * q3;
	double q2q2 = q2 * q2;
	double q2q3 = q2 * q3;
	double q3q3 = q3 * q3;
	//	
	//?2μ?2?ê?:1éò??ˉ′|àí
	norm = sqrt(ax*ax + ay*ay + az*az);     
	if(norm==0) return ;	
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;   

	//?2μ?2?ê?:?¨á￠D????á×?±ê?μ	
	vx = 2*(q1q3 - q0q2);								
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	//
	//?2μ?2?ê?:×?±ê?μoí??á|2??y????
	ex = (ay*vz - az*vy);								
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);
	//
	//?2μ?2?ê?:±èày????
	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;
	//
	//?2μ?2?ê?:íó?Yò?èúo?
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;
	//
	//?2μ?2?ê?:??o????aêy?ê
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

	//?2μ?2?ê?:1éò??ˉ′|àí
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	if(norm==0) return ;	
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;
  /* output data */
//  angle->imu_yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3;
  
  angle->imu_yaw =  angle->imu_yaw + gz*halfT*2*57.3;
  angle->imu_pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3f;																			// pitcho???
  angle->imu_roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3f;

return;
}

#endif



//!< this functino must be called about every 2ms to get accurate eular angles
uint32_t imu_get_euler_angle(imu_float_euler_angle_t * angle, mems_data_t * pRawDdata)
{
//  static imu_raw_data_t  raw_data_for_filter;
//  static imu_raw_data_t filter_data;
//  static imu_float_data_t float_data; 
//  memcpy(&raw_data_for_filter,pRawDdata,sizeof(mems_data_t));
//  imu_sliding_filter(raw_data_for_filter, &filter_data);
//  
//  imu_format_data(&filter_data, &float_data);
//  
//      updateAHRS( float_data.gx * Gyro_Gr,
//                  float_data.gy * Gyro_Gr,
//                  float_data.gz * Gyro_Gr,
//                  float_data.ax,
//                  float_data.ay,
//                  float_data.az,
//                  float_data.mx,
//                  float_data.my,
//                  float_data.mz,
//                  angle);
    
  double gx = ((double)pRawDdata->gyro_x)* Gyro_Gr;
  double gy = ((double)pRawDdata->gyro_y)* Gyro_Gr;
  double gz = ((double)pRawDdata->gyro_z)* Gyro_Gr;
  double ax = (double)pRawDdata->accel_x;
  double ay = (double)pRawDdata->accel_y;
  double az = (double)pRawDdata->accel_z;
  double mx = (double)pRawDdata->magn_x;
  double my = (double)pRawDdata->magn_y;
  double mz = (double)pRawDdata->magn_z;
  /* I need float data I give you euler angles */
  updateAHRS( gx,gy,gz,
              ax,ay,az,
              mx,my,mz,
              angle );
  return 0;
//    uint8_t ret = 0;
//    int16_t ax,ay,az,gx,gy,gz,mx,my,mz;
//    imu_raw_data_t filter_data;
//    imu_float_data_t float_data;
//    ret += gpIOInstallStruct->imu_get_accel(&ax, &ay, &az);
//    ret += gpIOInstallStruct->imu_get_gyro(&gx, &gy, &gz); 
//    ret += gpIOInstallStruct->imu_get_mag(&mx, &my, &mz);
//    if(ret >0)
//    {
//      return ret;
//    }
//    raw_data->ax = ax;
//    raw_data->ay = ay;
//    raw_data->az = az;
//    raw_data->gx = gx;
//    raw_data->gy = gy;
//    raw_data->gz = gz;
//    raw_data->mx = mx;
//    raw_data->my = my;
//    raw_data->mz = mz;
//
//    /* I need rawdata I give you filtered data */
//  imu_sliding_filter(*raw_data, &filter_data);
//
//    /* I need filtered data I give you float data */
//    imu_format_data(&filter_data, &float_data);
//    
//    /* I need float data I give you euler angles */
//    updateAHRS( float_data.gx * Gyro_Gr,
//                float_data.gy * Gyro_Gr,
//                float_data.gz * Gyro_Gr,
//                float_data.ax,
//                float_data.ay,
//                float_data.az,
//                float_data.mx,
//                float_data.my,
//                float_data.mz,
//                angle);
//    return 0;
}

// 1/sqrt(x)  ?????is it really OK ?
double invSqrt(double x) 
{
	double halfx = 0.5f * x;
	double y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(double*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}