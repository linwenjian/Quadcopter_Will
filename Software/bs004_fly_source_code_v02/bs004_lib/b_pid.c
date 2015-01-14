
#include "b_pid.h" 
//
extern unsigned int pid_setting_P_value[3];		//圆点博士:PID的P参数，含X轴 , Y轴 , Z轴
extern unsigned int pid_setting_D_value[3];		//圆点博士:PID的I参数，含X轴 , Y轴 , Z轴
float b_pitch_p , b_roll_p , b_yaw_p;
float b_pitch_i , b_roll_i , b_yaw_i;
float b_pitch_d , b_roll_d , b_yaw_d;
//
extern signed short b_fly_gas , b_fly_pitch , b_fly_roll , b_fly_yaw;
float b_fly_gas_scale = 0 , b_fly_pitch_scale = 0 , b_fly_roll_scale = 0 , b_fly_yaw_scale = 0;
//
extern float b_imu_pitch , b_imu_roll , b_imu_yaw;
float b_angle_cur_pitch  = 0 , b_angle_cur_roll  = 0 , b_angle_cur_yaw  = 0;
float b_angle_last_pitch = 0 , b_angle_last_roll = 0 , b_angle_last_yaw = 0;
float b_angle_dif_pitch  = 0 , b_angle_dif_roll  = 0 , b_angle_dif_yaw  = 0;
//
float b_fly_m1 = 0 , b_fly_m2 = 0 , b_fly_m3 = 0 , b_fly_m4 = 0;
//
void B_PID_Control(void)
{
	b_roll_p  = pid_setting_P_value[0] * 1.0f;
	b_pitch_p = pid_setting_P_value[1] * 1.0f;
	b_yaw_p   = pid_setting_P_value[2] * 1.0f;
	b_roll_d  = pid_setting_D_value[0] * 1.0f;
	b_pitch_d = pid_setting_D_value[1] * 1.0f;
	b_yaw_d   = pid_setting_D_value[2] * 1.0f;
	//
	//圆点博士:融合遥控器控制信号
	b_fly_m1 = b_fly_gas * b_fly_gas_scale - b_fly_pitch * b_fly_pitch_scale  +  b_fly_yaw * b_fly_yaw_scale;
	b_fly_m3 = b_fly_gas * b_fly_gas_scale + b_fly_pitch * b_fly_pitch_scale  +  b_fly_yaw * b_fly_yaw_scale;
	b_fly_m2 = b_fly_gas * b_fly_gas_scale - b_fly_roll  * b_fly_roll_scale   -  b_fly_yaw * b_fly_yaw_scale;
	b_fly_m4 = b_fly_gas * b_fly_gas_scale + b_fly_roll  * b_fly_roll_scale   -  b_fly_yaw * b_fly_yaw_scale;
	
	b_angle_cur_pitch = b_imu_pitch;
	b_angle_cur_roll  = b_imu_roll;
	b_angle_cur_yaw   = b_imu_yaw;
	//
	//圆点博士:融合角度比例控制
	b_fly_m1 = b_fly_m1 + b_pitch_p * b_angle_cur_pitch - b_roll_p * b_angle_cur_roll - b_yaw_p * b_angle_cur_yaw;
	b_fly_m2 = b_fly_m2 - b_pitch_p * b_angle_cur_pitch - b_roll_p * b_angle_cur_roll + b_yaw_p * b_angle_cur_yaw;
	b_fly_m3 = b_fly_m3 - b_pitch_p * b_angle_cur_pitch + b_roll_p * b_angle_cur_roll - b_yaw_p * b_angle_cur_yaw;
	b_fly_m4 = b_fly_m4 + b_pitch_p * b_angle_cur_pitch + b_roll_p * b_angle_cur_roll + b_yaw_p * b_angle_cur_yaw;
	//
	b_angle_dif_pitch = b_angle_cur_pitch - b_angle_last_pitch;
	b_angle_dif_roll  = b_angle_cur_roll  - b_angle_last_roll;
	b_angle_dif_yaw   = b_angle_last_yaw  - b_angle_cur_yaw;
	//
	b_fly_m1 = b_fly_m1 + b_pitch_d * b_angle_dif_pitch - b_roll_d * b_angle_dif_roll - b_yaw_d * b_angle_dif_yaw;
	b_fly_m2 = b_fly_m2 - b_pitch_d * b_angle_dif_pitch - b_roll_d * b_angle_dif_roll + b_yaw_d * b_angle_dif_yaw;
	b_fly_m3 = b_fly_m3 - b_pitch_d * b_angle_dif_pitch + b_roll_d * b_angle_dif_roll - b_yaw_d * b_angle_dif_yaw;
	b_fly_m4 = b_fly_m4 + b_pitch_d * b_angle_dif_pitch + b_roll_d * b_angle_dif_roll + b_yaw_d * b_angle_dif_yaw;
	//
	b_angle_last_pitch = b_angle_cur_pitch;
	b_angle_last_roll  = b_angle_cur_roll;	
	b_angle_last_yaw   = b_angle_cur_yaw;	
}
