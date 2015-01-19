//-----------------------------------------------------------------
//文件名称：mpu6050dmp_attitude_angle_cal.c
//功能概要：mpu6050 dmp读取姿态角文件
//版本更新：2013-11-06
//-----------------------------------------------------------------

//------------------------------ 编译预处理 ------------------------------//
#include "mpu6050dmp_attitude_angle_cal.h"
#include "Delay.h"
//------------------------------ 内部函数 ------------------------------//

//-----------------------------------------------------------------
//函数名称：inv_row_2_scale  inv_orientation_matrix_to_scalar
//功能概要：These next two functions converts the orientation matrix (see gyro_orientation) to 
//          a scalar representation for use by the DMP.
//函数返回：unsigned short
//参数说明：row
//    NOTE: These functions are borrowed from Invensense's MPL.
//-----------------------------------------------------------------
static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;
    if (row[0] > 0)b = 0;
    else if (row[0] < 0)b = 4;
    else if (row[1] > 0)b = 1;
    else if (row[1] < 0)b = 5;
    else if (row[2] > 0)b = 2;
    else if (row[2] < 0)b = 6;
    else b = 7;      // error
    return b;
}
static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;
    return scalar;
}

//-----------------------------------------------------------------
//函数名称：run_self_test
//功能概要：mpu自检函数,if Test passed, We can trust the gyro data here, so let's push it down to the DMP.
//函数返回：void
//参数说明：void
//-----------------------------------------------------------------
static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];
    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) 
	{
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
    }
}

//------------------------------ 外部接口函数 ------------------------------//

//-----------------------------------------------------------------
//函数名称：mpu6050dmp_init
//功能概要：mpu6050和dmp初始化
//函数返回：void
//参数说明：void
//注    意：DMP的数据被读出后才会进行下一次姿态更新，所以一定要及时读取DMP，即DMP的读取周期一定要小于DMP更新周期
//          (DMP读取速率要快于DMP数据输出速率,否则读出的姿态角将有滞后,加速度计和陀螺仪的值将错乱！
//          这样可能造成相邻周期读取到相同数据，但这是正常现象,所以读取周期只要保证“略小于”即可)！
//-----------------------------------------------------------------
#define DEFAULT_MPU_HZ  (100)  	//starting sampling rate.(ending sampling rate=200HZ)
#define DEFAULT_DMP_HZ  (100)   //dmp output rate(<=DMP_SAMPLE_RATE=200Hz)(Tdmp=5,10,20,40,50,100,200,500,1000ms)
void mpu6050dmp_init(void)
{
    // The sensors can be mounted onto the board in any orientation. The mounting matrix seen below tells the MPL how to rotate the raw data from the driver(s).
    // TODO: The following matrices refer to the configuration on an internal test board at Invensense. If needed, please modify the matrices to match the chip-to-body matrix for your particular set up.
    static signed char gyro_orientation[9] = {-1, 0, 0, 0, -1, 0, 0, 0, 1};
    static int result;
    delay_ms(1000);
    result = mpu_init();
    if(!result)
    {
        mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);		//turn specific sensors on
        mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);	//set GYRO、ACCEL pushed to FIFO
        mpu_set_sample_rate(DEFAULT_MPU_HZ); 				//DIV=(1000/DEFAULT_MPU_HZ)-1
        dmp_load_motion_driver_firmware();					//load the DMP with this image.st.chip_cfg.dmp_sample_rate = DMP_SAMPLE_RATE=200;
        dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//push gyro and accel orientation to the DMP
        dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL);//enable DMP features.
        dmp_set_fifo_rate(DEFAULT_DMP_HZ); 					//set DMP output rate (0X6F = DMP_SAMPLE_RATE/DEFAULT_DMP_HZ-1)
        run_self_test();									//run_self_test
        mpu_set_dmp_state(1);				 				//enable/disable DMP support,  DIV=(1000/st.chip_cfg.dmp_sample_rate)-1=0x04 (即ending sampling rate=200HZ)
    }
}
void init_MPU6050(void)
{
    static signed char gyro_orientation[9] = {-1, 0, 0, 0, -1, 0, 0, 0, 1};

    delay_ms(1000);

    mpu_init();

    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);

    mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);

    mpu_set_sample_rate(DEFAULT_MPU_HZ);

    dmp_load_motion_driver_firmware();

    dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));

    dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |

        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |

        DMP_FEATURE_GYRO_CAL);

    dmp_set_fifo_rate(DEFAULT_MPU_HZ);

    run_self_test();

    mpu_set_dmp_state(1);
}
//-----------------------------------------------------------------
//函数名称：attitude_angle_cal
//功能概要：mpu6050的dmp读取姿态角(Pitch,Roll,Yaw:俯仰角，横滚角，导航角)并返回加速度计、陀螺仪值
//函数返回：void
//参数说明：angle:指向三个姿态角的首地址，放大10倍输出
//       gyro:指向三轴角速度的首地址 ( gyro[0]:Ex(pitch);  gyro[1]:Ey(roll);  gyro[2]:Ez(yaw))(无高通滤波)
//          accel:指向三轴加速度的首地址(accel[0]:-x(pitch); accel[1]:-y(roll); accel[2]:-z(yaw))
//注    意：dmp读出的陀螺仪值有一点点失真，如果要用陀螺仪值进行控制，最好在外面自己单独读取陀螺仪值。
//-----------------------------------------------------------------

#define q30  1073741824.0f
extern float pitch, roll, yaw;

void attitude_angle_cal(int16_t* angle, int16_t* gyro, int16_t* accel)
{
    static unsigned long sensor_timestamp;
    static short sensors;
    static unsigned char more;
    static long quat[4];
    static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    //static float pitch,roll,yaw;
    //static float yaw_last;

    dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);
    //Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units. This behavior is convenient because it
    //keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
    //Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30. The orientation is set by the scalar passed
    //to dmp_set_orientation during initialization.
    if (sensors & INV_WXYZ_QUAT)
    {
        q0=quat[0] / q30;
        q1=quat[1] / q30;
        q2=quat[2] / q30;
        q3=quat[3] / q30;
        pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3f;                                                    //Pitch
        roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) * 57.3f;                     //Roll
        yaw   = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3f;               //Yaw

#ifdef SIDE_DOWN
        if(roll < 0)
        {
            roll +=180;
        }
        else
        {
            roll -=180;
        }
#endif
#if 0
        if(period_count == 1)
        {
                yaw_last = yaw;
        }//标定

        if((yaw-yaw_last) > 180.0f)
        {
                yaw -= 360.0f;
        }
        else if((yaw-yaw_last) < -180.0f)
        {
                yaw += 360.0f;
        }//预处理
        yaw -= yaw_last;//将任意位置的偏航角调整为相对原始偏航角"yaw_last"的-180°~180°的数值。
#endif
        angle[0] = (int16_t)(pitch * 10.0f);
        angle[1] = (int16_t)(roll * 10.0f);
        angle[2] = (int16_t)(yaw * 10.0f);	//放大10倍输出
    }
}

unsigned long nullVal = 0;
void get_AttitudeVal(short *gyroVal)
{
    static unsigned long sensor_timestamp;
    static short sensors;
    static unsigned char more;
    static long quat[4];
    static short gyro[3];
    static short accel[3];
    static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

    dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);

    if (sensors & INV_WXYZ_QUAT)
    {
#if 0
        mpu_get_gyro_reg(gyroVal, &nullVal);
        sendLineX(MCU1, 0X1F, (float)gyroVal[1] / 16.4);
        sendLineX(MCU1, 0X2F, (float)gyro[1] / -16.4);
#endif
        q0=quat[0] / q30;
        q1=quat[1] / q30;
        q2=quat[2] / q30;
        q3=quat[3] / q30;

        pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3f;                                                    //Pitch
        roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3f;                    //Roll
        yaw   = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3f;               //Yaw

#ifdef SIDE_DOWN
        if(roll < 0)
        {
            roll += 180;
        }
        else
        {
            roll -= 180;
        }
#endif
    }

#if 1
    mpu_get_gyro_reg(gyroVal, &nullVal);

    gyroVal[0] = -1 * gyroVal[0];
    gyroVal[1] = -1 * gyroVal[1];
    gyroVal[2] = -1 * gyroVal[2];
#endif
}
