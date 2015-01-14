/*************************************************************************************************************
圆点博士小四轴飞行器2014版配套源代码声明:
该源代码仅供参考,圆点博士不对源代码提供任何形式的担保,也不对因使用该源代码而出现的损失负责.
用户可以以学习的目的修改和使用该源代码.
但用户在修改该源代码时,不得移除该部分版权信息，必须保留原版声明.

更多信息，请访问官方网站www.etootle.com, 官方博客:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "b_parameter.h" 
//
unsigned int b_load_par_sign=0;
//
void B_Load_Fly_Parameter(void)
{
	b_load_par_sign=0; 	//圆点博士:装载参数标志        
	//
	B_Load_Filter_Parameter();			//圆点博士:装载参数
	B_Load_Pid_Parameter();         //圆点博士:装载参数
	B_Load_Calibration_Parameter(); //圆点博士:装载参数
	//
	B_COM1_Send_Str_Head();
	B_COM1_Send_Str_Body("load user parameter .");
	B_COM1_Send_4bits_BCD_Num(b_load_par_sign);
	B_COM1_Send_Str_Tail();
}

void B_Load_Filter_Parameter(void)
{
	int b_filter_par[12];
	unsigned char i;
	unsigned int *b_parameter_addr;
	unsigned int b_parameter_data,b_parameter_sign,b_parameter_valid;
	unsigned char b_parameter_byte_data[32];
	//	
	for(i=0;i<7;i++) 											
	{
		b_parameter_addr=(unsigned int *)(0x803F400+i*4);				//圆点博士:更新地址
		b_parameter_data=(unsigned int)*(b_parameter_addr);	//圆点博士:从地址读取数据
		//
		b_parameter_byte_data[i*4]=b_parameter_data & 0xff;
		b_parameter_byte_data[i*4+1]=(b_parameter_data>>8) & 0xff;
		b_parameter_byte_data[i*4+2]=(b_parameter_data>>16) & 0xff;
		b_parameter_byte_data[i*4+3]=(b_parameter_data>>24) & 0xff;			
	}
	//
	b_parameter_sign=b_parameter_data&0x0ffff;
	b_parameter_valid=(b_parameter_data&0x0ffff0000)>>16;
	//
	if(b_parameter_valid==0x4c46)
	{
		b_load_par_sign++;
		//
		for(i=0;i<12;i++)
		{
			b_filter_par[i]=((b_parameter_byte_data[i*2]&0xf0)>>4)*1000;
			b_filter_par[i]+=(b_parameter_byte_data[i*2]&0x0f)*100;
			b_filter_par[i]+=((b_parameter_byte_data[i*2+1]&0xf0)>>4)*10;
			b_filter_par[i]+=b_parameter_byte_data[i*2+1]&0x0f;
		}
		//
		if((b_parameter_sign&0x0f0)==0xd0)
		{
			if((b_parameter_sign&0x0100)==0) b_filter_par[0]=-b_filter_par[0];
			if((b_parameter_sign&0x0200)==0) b_filter_par[1]=-b_filter_par[1];		
			if((b_parameter_sign&0x0400)==0) b_filter_par[2]=-b_filter_par[2];	
			if((b_parameter_sign&0x0800)==0) b_filter_par[3]=-b_filter_par[3];	
			if((b_parameter_sign&0x1000)==0) b_filter_par[4]=-b_filter_par[4];	
			if((b_parameter_sign&0x2000)==0) b_filter_par[5]=-b_filter_par[5];	
			if((b_parameter_sign&0x4000)==0) b_filter_par[6]=-b_filter_par[6];	
			if((b_parameter_sign&0x8000)==0) b_filter_par[7]=-b_filter_par[7];	
			if((b_parameter_sign&0x0001)==0) b_filter_par[8]=-b_filter_par[8];	
			if((b_parameter_sign&0x0002)==0) b_filter_par[9]=-b_filter_par[9];
			if((b_parameter_sign&0x0004)==0) b_filter_par[10]=-b_filter_par[10];
			if((b_parameter_sign&0x0008)==0) b_filter_par[11]=-b_filter_par[11];			
		}	
	}
	//
	b_filter_high=(float)b_filter_par[0]/1000.0f;						//圆点博士:互补滤波参数
	b_filter_low=(float)b_filter_par[1]/1000.0f;						//圆点博士:互补滤波参数	
	b_filter_time=(float)b_filter_par[2]/1000.0f;						//圆点博士:互补滤波参数		
	b_sys_timer_period=(unsigned int)b_filter_par[3]-1; 		//圆点博士:传感器采样频率
	b_mpu6050_gyro_scale=1.0f/(float)b_filter_par[4];				//圆点博士:陀螺仪灵敏度
	b_mpu6050_acc_scale=(float)b_filter_par[5]/1000.0f;			//圆点博士:加速度灵敏度
	b_hmc5883l_mag_scale=(float)b_filter_par[6]/1000.0f;		//圆点博士:磁力计灵敏度
	b_motor_pwm_period=(unsigned int)b_filter_par[7]*36;	  //圆点博士:电机控制周期
	B_Motor_Scale=(unsigned int)b_filter_par[8];          	//圆点博士:电机控制灵敏度
	b_quad_halfT=(float)b_filter_par[9]/1000.0f;						//圆点博士:四元数时间系数
	b_quad_Kp=(float)b_filter_par[10]/1000.0f;							//圆点博士:四元数比例系数
	b_quad_Ki=(float)b_filter_par[11]/1000.0f;							//圆点博士:四元数积分系数	
}

void B_Load_Pid_Parameter(void)
{
	int b_pid_par[12];
	unsigned char i;
	unsigned int *b_parameter_addr;
	unsigned int b_parameter_data,b_parameter_sign,b_parameter_valid;
	unsigned char b_parameter_byte_data[32];
	//	
	for(i=0;i<7;i++) 											
	{
		b_parameter_addr=(unsigned int *)(0x803F41C+i*4);				//圆点博士:更新地址
		b_parameter_data=(unsigned int)*(b_parameter_addr);	//圆点博士:从地址读取数据
		//
		b_parameter_byte_data[i*4]=b_parameter_data & 0xff;
		b_parameter_byte_data[i*4+1]=(b_parameter_data>>8) & 0xff;
		b_parameter_byte_data[i*4+2]=(b_parameter_data>>16) & 0xff;
		b_parameter_byte_data[i*4+3]=(b_parameter_data>>24) & 0xff;			
	}
	//
	b_parameter_sign=b_parameter_data&0x0ffff;
	b_parameter_valid=(b_parameter_data&0x0ffff0000)>>16;
	//
	if(b_parameter_valid==0x4450)
	{
		b_load_par_sign++;
		//
		for(i=0;i<12;i++)
		{
			b_pid_par[i]=((b_parameter_byte_data[i*2]&0xf0)>>4)*1000;
			b_pid_par[i]+=(b_parameter_byte_data[i*2]&0x0f)*100;
			b_pid_par[i]+=((b_parameter_byte_data[i*2+1]&0xf0)>>4)*10;
			b_pid_par[i]+=b_parameter_byte_data[i*2+1]&0x0f;
		}
		//
		if((b_parameter_sign&0x0f0)==0xd0)
		{
			if((b_parameter_sign&0x0100)==0) b_pid_par[0]=-b_pid_par[0];
			if((b_parameter_sign&0x0200)==0) b_pid_par[1]=-b_pid_par[1];		
			if((b_parameter_sign&0x0400)==0) b_pid_par[2]=-b_pid_par[2];	
			if((b_parameter_sign&0x0800)==0) b_pid_par[3]=-b_pid_par[3];	
			if((b_parameter_sign&0x1000)==0) b_pid_par[4]=-b_pid_par[4];	
			if((b_parameter_sign&0x2000)==0) b_pid_par[5]=-b_pid_par[5];	
			if((b_parameter_sign&0x4000)==0) b_pid_par[6]=-b_pid_par[6];	
			if((b_parameter_sign&0x8000)==0) b_pid_par[7]=-b_pid_par[7];	
			if((b_parameter_sign&0x0001)==0) b_pid_par[8]=-b_pid_par[8];	
			if((b_parameter_sign&0x0002)==0) b_pid_par[9]=-b_pid_par[9];
			if((b_parameter_sign&0x0004)==0) b_pid_par[10]=-b_pid_par[10];
			if((b_parameter_sign&0x0008)==0) b_pid_par[11]=-b_pid_par[11];			
		}	
	}
	//
	for(i=0;i<3;i++) pid_setting_P_value[i]=(unsigned int)b_pid_par[i];
	for(i=0;i<3;i++) pid_setting_I_value[i]=(unsigned int)b_pid_par[i+3];
	for(i=0;i<3;i++) pid_setting_D_value[i]=(unsigned int)b_pid_par[i+6];
	b_fly_gas_scale=(float)b_pid_par[9]/1.0f;
	b_fly_pitch_scale=(float)b_pid_par[10]/1.0f;
	b_fly_roll_scale=(float)b_pid_par[10]/1.0f;
	b_fly_yaw_scale=(float)b_pid_par[11]/1.0f;
}

void B_Load_Calibration_Parameter(void)
{
	int b_cal_par[12];
	unsigned char i;
	unsigned int *b_parameter_addr;
	unsigned int b_parameter_data,b_parameter_sign,b_parameter_valid;
	unsigned char b_parameter_byte_data[32];
	//	
	for(i=0;i<7;i++) 											
	{
		b_parameter_addr=(unsigned int *)(0x803F438+i*4);				//圆点博士:更新地址
		b_parameter_data=(unsigned int)*(b_parameter_addr);	//圆点博士:从地址读取数据
		//
		b_parameter_byte_data[i*4]=b_parameter_data & 0xff;
		b_parameter_byte_data[i*4+1]=(b_parameter_data>>8) & 0xff;
		b_parameter_byte_data[i*4+2]=(b_parameter_data>>16) & 0xff;
		b_parameter_byte_data[i*4+3]=(b_parameter_data>>24) & 0xff;			
	}
	//
	b_parameter_sign=b_parameter_data&0x0ffff;
	b_parameter_valid=(b_parameter_data&0x0ffff0000)>>16;
	//
	if(b_parameter_valid==0x4c43)
	{			
		b_load_par_sign++;
		//
		for(i=0;i<12;i++)
		{
			b_cal_par[i]=((b_parameter_byte_data[i*2]&0xf0)>>4)*1000;
			b_cal_par[i]+=(b_parameter_byte_data[i*2]&0x0f)*100;
			b_cal_par[i]+=((b_parameter_byte_data[i*2+1]&0xf0)>>4)*10;
			b_cal_par[i]+=b_parameter_byte_data[i*2+1]&0x0f;
		}
		//
		if((b_parameter_sign&0x0f0)==0xd0)
		{
			if((b_parameter_sign&0x0100)==0) b_cal_par[0]=-b_cal_par[0];
			if((b_parameter_sign&0x0200)==0) b_cal_par[1]=-b_cal_par[1];		
			if((b_parameter_sign&0x0400)==0) b_cal_par[2]=-b_cal_par[2];	
			if((b_parameter_sign&0x0800)==0) b_cal_par[3]=-b_cal_par[3];	
			if((b_parameter_sign&0x1000)==0) b_cal_par[4]=-b_cal_par[4];	
			if((b_parameter_sign&0x2000)==0) b_cal_par[5]=-b_cal_par[5];	
			if((b_parameter_sign&0x4000)==0) b_cal_par[6]=-b_cal_par[6];	
			if((b_parameter_sign&0x8000)==0) b_cal_par[7]=-b_cal_par[7];	
			if((b_parameter_sign&0x0001)==0) b_cal_par[8]=-b_cal_par[8];	
			if((b_parameter_sign&0x0002)==0) b_cal_par[9]=-b_cal_par[9];
			if((b_parameter_sign&0x0004)==0) b_cal_par[10]=-b_cal_par[10];
			if((b_parameter_sign&0x0008)==0) b_cal_par[11]=-b_cal_par[11];			
		}			
	}
	//
	b_mpu6050_gyro_pitch_cal=(signed short int)b_cal_par[0];
	b_mpu6050_gyro_roll_cal=(signed short int)b_cal_par[1];
	b_mpu6050_gyro_yaw_cal=(signed short int)b_cal_par[2];
	b_mpu6050_acc_roll_cal=(signed short int)b_cal_par[3];
	b_mpu6050_acc_pitch_cal=(signed short int)b_cal_par[4];
	b_mpu6050_acc_yaw_cal=(signed short int)b_cal_par[5];
	b_hmc5883l_mag_pitch_cal=(signed short int)b_cal_par[6];
	b_hmc5883l_mag_roll_cal=(signed short int)b_cal_par[7];
	b_hmc5883l_mag_yaw_cal=(signed short int)b_cal_par[8];
}
