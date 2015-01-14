/*************************************************************************************************************
圆点博士小四轴飞行器2014版配套源代码声明:
该源代码仅供参考,圆点博士不对源代码提供任何形式的担保,也不对因使用该源代码而出现的损失负责.
用户可以以学习的目的修改和使用该源代码.
但用户在修改该源代码时,不得移除该部分版权信息，必须保留原版声明.

更多信息，请访问官方网站www.etootle.com, 官方博客:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "b_motor.h" 
//
extern float b_fly_m1,b_fly_m2,b_fly_m3,b_fly_m4;
signed short b_fly_m1_out=0,b_fly_m2_out=0,b_fly_m3_out=0,b_fly_m4_out=0;	
//
extern float b_angle_cur_pitch,b_angle_cur_roll,b_angle_cur_yaw;
extern float b_angle_last_pitch,b_angle_last_roll,b_angle_last_yaw;
//
unsigned int b_motor_pwm_period=0;
unsigned int B_Motor_Scale=0;
unsigned int Motor_B_M1=0,Motor_B_M2=0,Motor_B_M3=0,Motor_B_M4=0;
//
void B_Motor_Power_On(void)
{
	GPIO_SetBits(B_MOTOR_POWER_MA_PORT, B_MOTOR_POWER_MA);		//圆点博士:打开电机电源
	GPIO_SetBits(B_MOTOR_POWER_MB_PORT, B_MOTOR_POWER_MB);    //圆点博士:打开电机电源
}

void B_Motor_Power_Off(void)
{
	GPIO_ResetBits(B_MOTOR_POWER_MA_PORT, B_MOTOR_POWER_MA);	//圆点博士:关闭电机电源
	GPIO_ResetBits(B_MOTOR_POWER_MB_PORT, B_MOTOR_POWER_MB);	//圆点博士:关闭电机电源
}

void B_MOTOR_PWM_ON(void)		
{
	GPIO_SetBits(B_MOTOR_PWM_PORT, B_MOTOR_PWM_M1);			//圆点博士:打开PWM输出
	GPIO_SetBits(B_MOTOR_PWM_PORT, B_MOTOR_PWM_M2);			//圆点博士:打开PWM输出
	GPIO_SetBits(B_MOTOR_PWM_PORT, B_MOTOR_PWM_M3);			//圆点博士:打开PWM输出
	GPIO_SetBits(B_MOTOR_PWM_PORT, B_MOTOR_PWM_M4);			//圆点博士:打开PWM输出
}
void B_MOTOR_PWM_OFF(void)		
{
	GPIO_ResetBits(B_MOTOR_PWM_PORT, B_MOTOR_PWM_M1);		//圆点博士:关闭PWM输出
	GPIO_ResetBits(B_MOTOR_PWM_PORT, B_MOTOR_PWM_M2);		//圆点博士:关闭PWM输出
	GPIO_ResetBits(B_MOTOR_PWM_PORT, B_MOTOR_PWM_M3);		//圆点博士:关闭PWM输出
	GPIO_ResetBits(B_MOTOR_PWM_PORT, B_MOTOR_PWM_M4);		//圆点博士:关闭PWM输出
}

//===============================================================

void B_Motor_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_PWM_M1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //圆点博士:设置PWM口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //圆点博士:设置PWM口为输出
  GPIO_Init(B_MOTOR_PWM_PORT, &GPIO_InitStructure); 
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_PWM_M2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //圆点博士:设置PWM口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //圆点博士:设置PWM口为输出
  GPIO_Init(B_MOTOR_PWM_PORT, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_PWM_M3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //圆点博士:设置PWM口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //圆点博士:设置PWM口为输出
  GPIO_Init(B_MOTOR_PWM_PORT, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_PWM_M4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //圆点博士:设置PWM口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //圆点博士:设置PWM口为输出
  GPIO_Init(B_MOTOR_PWM_PORT, &GPIO_InitStructure);
	//	
	B_MOTOR_PWM_OFF();															//圆点博士:设置PWM口输出为低
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_POWER_MA;					
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //圆点博士:设置电机电源控制口为输出
  GPIO_Init(B_MOTOR_POWER_MA_PORT, &GPIO_InitStructure); 
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_POWER_MB;					
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //圆点博士:设置电机电源控制口为输出
  GPIO_Init(B_MOTOR_POWER_MB_PORT, &GPIO_InitStructure); 
	//
	B_Motor_Power_Off();														//圆点博士:关闭电机电源
}

void B_Motor_PWM_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_PWM_M1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //圆点博士:设置LED口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  	//圆点博士:设置LED口为输出
  GPIO_Init(B_MOTOR_PWM_PORT, &GPIO_InitStructure); 
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_PWM_M2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //圆点博士:设置LED口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  	//圆点博士:设置LED口为输出
  GPIO_Init(B_MOTOR_PWM_PORT, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_PWM_M3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //圆点博士:设置LED口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  	//圆点博士:设置LED口为输出
  GPIO_Init(B_MOTOR_PWM_PORT, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = B_MOTOR_PWM_M4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //圆点博士:设置LED口最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  	//圆点博士:设置LED口为输出
  GPIO_Init(B_MOTOR_PWM_PORT, &GPIO_InitStructure);
	//	
	//圆点博士:PWM频率=B_MOTOR_PWM_CLK_36MHZ/（B_MOTOR_PWM_PERIOD+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = B_MOTOR_PWM_SYSCLK_DIV;
	TIM_TimeBaseStructure.TIM_Prescaler = B_MOTOR_PWM_CLK_72MHZ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = b_motor_pwm_period; 
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);								//圆点博士:设置PWM周期和频率			
	//
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Motor_B_M3;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;         
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);											//圆点博士:设置PWM占空比		
	//
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Motor_B_M2;
  TIM_OC2Init(TIM8, &TIM_OCInitStructure);											//圆点博士:设置PWM占空比		
	//
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Motor_B_M4;
  TIM_OC3Init(TIM8, &TIM_OCInitStructure);											//圆点博士:设置PWM占空比		
	//
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Motor_B_M1;
  TIM_OC4Init(TIM8, &TIM_OCInitStructure);											//圆点博士:设置PWM占空比		
	//
	B_Motor_NVIC_Configuration();					//圆点博士:设置PWM中断优先级	
	TIM_ITConfig(TIM8,TIM_IT_Update, ENABLE);	//圆点博士:打开中断
	TIM_Cmd(TIM8, ENABLE);										//圆点博士:启动PWM
	TIM_CtrlPWMOutputs(TIM8,ENABLE);          //圆点博士:允许PWM输出
	//
	B_COM1_Send_Str_Head();
	B_COM1_Send_Str_Body("finish to init motor device.");					//圆点博士:初始化PWM IO
	B_COM1_Send_Str_Tail();	
}

void B_Motor_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;			//圆点博士:设置PWM中断优先级	
	//
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//===============================================================
void B_Motor_Interrupt(void)					//圆点博士:PWM中断函数
{	
	TIM_SetCompare4(TIM8,Motor_B_M1);		//圆点博士:更新PWM占空比  
	TIM_SetCompare2(TIM8,Motor_B_M2);	  //圆点博士:更新PWM占空比  
	TIM_SetCompare1(TIM8,Motor_B_M3);   //圆点博士:更新PWM占空比      
	TIM_SetCompare3(TIM8,Motor_B_M4);		//圆点博士:更新PWM占空比  	    
}

signed short B_Motor_Speed_Scale(float motor_speed_input)
{
	float motor_speed_output;
	//
	if(motor_speed_input>B_FLY_MAX_OUT) motor_speed_output=B_FLY_MAX_OUT;
	else if(motor_speed_input<B_FLY_MIN_OUT) motor_speed_output=B_FLY_MIN_OUT;
	else motor_speed_output=motor_speed_input;
	return motor_speed_output;
}

void B_Motor_Reset(void)
{
	b_fly_m1=0;
	b_fly_m2=0;	
	b_fly_m3=0;
	b_fly_m4=0;
	//
	b_angle_cur_pitch=0;
	b_angle_cur_roll=0;
	b_angle_cur_yaw=0;
	b_angle_last_pitch=0;
	b_angle_last_roll=0;	
	b_angle_last_yaw=0;		
	//
	Motor_B_M1=B_FLY_MIN_OUT;
	Motor_B_M2=B_FLY_MIN_OUT;
	Motor_B_M3=B_FLY_MIN_OUT;
	Motor_B_M4=B_FLY_MIN_OUT;
}

void B_Motor_Control(void)
{
	b_fly_m1_out=B_Motor_Speed_Scale(b_fly_m1);
	b_fly_m2_out=B_Motor_Speed_Scale(b_fly_m2);	
	b_fly_m3_out=B_Motor_Speed_Scale(b_fly_m3);
	b_fly_m4_out=B_Motor_Speed_Scale(b_fly_m4);	
	//
	Motor_B_M1=b_fly_m1_out*B_Motor_Scale;
	Motor_B_M2=b_fly_m2_out*B_Motor_Scale;
	Motor_B_M3=b_fly_m3_out*B_Motor_Scale;
	Motor_B_M4=b_fly_m4_out*B_Motor_Scale;
}




