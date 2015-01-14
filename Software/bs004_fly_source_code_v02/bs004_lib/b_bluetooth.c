/*************************************************************************************************************
圆点博士小四轴飞行器2014版配套源代码声明:
该源代码仅供参考,圆点博士不对源代码提供任何形式的担保,也不对因使用该源代码而出现的损失负责.
用户可以以学习的目的修改和使用该源代码.
但用户在修改该源代码时,不得移除该部分版权信息，必须保留原版声明.

更多信息，请访问官方网站www.etootle.com, 官方博客:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "b_bluetooth.h" 
#include "b_motor.h" 
#include "b_adc.h" 
#include "b_led.h" 
//
#define AnBT_Command_Head     58		//圆点博士数据头标致 :
#define AnBT_Command_Tail	    47    //圆点博士数据头标致 /
#define AnBT_COM_Buf_Length		64		//圆点博士接收缓冲长度
//
unsigned char  pid_data_buffer_valid=0;			//圆点博士:PID数据合法标志
unsigned int   pid_data_buffer=0;         	//圆点博士:PID数据缓冲
unsigned int   pid_setting_P_value[3]={0};	//圆点博士:PID的P参数，含X轴,Y轴,Z轴
unsigned int   pid_setting_I_value[3]={0};	//圆点博士:PID的I参数，含X轴,Y轴,Z轴	
unsigned int   pid_setting_D_value[3]={0,0,0};				//圆点博士:PID的D参数，含X轴,Y轴,Z轴	
//
unsigned char  ctrl_data_buffer_valid=0;          //圆点博士:控制数据合法标志
unsigned char  remote_ctrl_data_buffer[4]={0};		//圆点博士:遥控数据缓冲
//
unsigned int   b_bat_value=0;									//圆点博士:电池电压值
//
unsigned char com_status_is_idle=1;												//圆点博士:串口通讯空闲标志
unsigned char valid_command_was_received=0;								//圆点博士:合法命令接收标志
unsigned char b_com_command_ops=0;											//圆点博士:接收到的命令
unsigned char com_receive_str_index=0;										//圆点博士:接收缓冲地址索引
unsigned char com_receive_str_buf[AnBT_COM_Buf_Length]; 	//圆点博士:接收字符缓冲
unsigned char com_receive_data_buf[AnBT_COM_Buf_Length]; 	//圆点博士:接收数据缓冲
//
unsigned char B_Ctrl_Gas=0,B_Ctrl_Valid=0,B_Ctrl_Gas_Noise=20;
signed char   B_Ctrl_Pitch=0,B_Ctrl_Roll=0,B_Ctrl_Yaw=0,B_Ctrl_Dir_Noise=20;
signed short  b_fly_gas=0,b_fly_pitch=0,b_fly_roll=0,b_fly_yaw=0;
//
void B_COM1_Interrupt(void)	//圆点博士:蓝牙接收中断
{
	unsigned char com_receive_data=0;
	unsigned char com_receive_data_checksum=0;
	unsigned char com_receive_data_checksum_low,com_receive_data_checksum_high;
	unsigned char com_data_checksum=0;
	unsigned char anbt_com_command_HH=0,anbt_com_command_HL=0,anbt_com_command_LH=0,anbt_com_command_LL=0;
	unsigned char i;
	//
	com_receive_data=USART_ReceiveData(USART1);   
	//	
	if(com_receive_data==AnBT_Command_Head)   
	{
			com_receive_str_index=0;     
			valid_command_was_received=0;			
			com_data_checksum=0;
	}
	else if(com_receive_data==AnBT_Command_Tail)  
	{
			if(com_receive_str_index>1) 
			{			
				for(i=0;i<com_receive_str_index-2;i++) com_data_checksum += com_receive_str_buf[i];    //圆点博士:计算数据校验和标志
				com_receive_data_checksum_low=com_receive_str_buf[com_receive_str_index-1];
				com_receive_data_checksum_high=com_receive_str_buf[com_receive_str_index-2];				
				if(com_receive_data_checksum_low>58) com_receive_data_checksum_low-=55;
				else com_receive_data_checksum_low-=48;
				if(com_receive_data_checksum_high>58) com_receive_data_checksum_high-=55;
				else com_receive_data_checksum_high-=48;	
				com_receive_data_checksum=((com_receive_data_checksum_high<<4)&0xf0)|(com_receive_data_checksum_low&0x0f);
				com_data_checksum=com_data_checksum+com_receive_data_checksum;
				if(com_data_checksum==0) valid_command_was_received=1;
			}
			if(com_status_is_idle && valid_command_was_received) //data is correct and allowed to receive
			{				
				anbt_com_command_HH=com_receive_str_buf[0];
				anbt_com_command_HL=com_receive_str_buf[1];
				anbt_com_command_LH=com_receive_str_buf[2];
				anbt_com_command_LL=com_receive_str_buf[3];
				pid_data_buffer_valid=0;
				ctrl_data_buffer_valid=0;
				//
				if((anbt_com_command_HH=='R')&&(anbt_com_command_HL=='C'))	b_com_command_ops=0xA0;					//remote ctrol
				else if((anbt_com_command_HH=='B')&&(anbt_com_command_HL=='A')&&(anbt_com_command_LH=='T'))  	b_com_command_ops=0xA1;		//read voltage
				else if((anbt_com_command_HH=='V')&&(anbt_com_command_HL=='E')&&(anbt_com_command_LH=='R'))		b_com_command_ops=0xA2;		//return version
				else if((anbt_com_command_HH=='C')&&(anbt_com_command_HL=='T')&&(anbt_com_command_LH=='R')&&(anbt_com_command_LL=='L'))	b_com_command_ops=0xA3;		//read PI data
				else if((anbt_com_command_HH=='P')&&(anbt_com_command_HL=='A')&&(anbt_com_command_LH=='L')&&(anbt_com_command_LL=='L'))	b_com_command_ops=0xA4;		//read PI data
				else if((anbt_com_command_HH=='D')&&(anbt_com_command_HL=='A')&&(anbt_com_command_LH=='L')&&(anbt_com_command_LL=='L'))	b_com_command_ops=0xA5;		//read PI data
				else if((anbt_com_command_HH=='W')&&(anbt_com_command_HL=='R')&&(anbt_com_command_LH=='P')&&(anbt_com_command_LL=='X'))	b_com_command_ops=0xA6;		//write PX
				else if((anbt_com_command_HH=='W')&&(anbt_com_command_HL=='R')&&(anbt_com_command_LH=='P')&&(anbt_com_command_LL=='Y'))	b_com_command_ops=0xA7;		//write PY
				else if((anbt_com_command_HH=='W')&&(anbt_com_command_HL=='R')&&(anbt_com_command_LH=='P')&&(anbt_com_command_LL=='Z'))	b_com_command_ops=0xA8;		//write PZ
				else if((anbt_com_command_HH=='W')&&(anbt_com_command_HL=='R')&&(anbt_com_command_LH=='D')&&(anbt_com_command_LL=='X'))	b_com_command_ops=0xA9;		//write IX
				else if((anbt_com_command_HH=='W')&&(anbt_com_command_HL=='R')&&(anbt_com_command_LH=='D')&&(anbt_com_command_LL=='Y'))	b_com_command_ops=0xAA;		//write IY
				else if((anbt_com_command_HH=='W')&&(anbt_com_command_HL=='R')&&(anbt_com_command_LH=='D')&&(anbt_com_command_LL=='Z'))	b_com_command_ops=0xAB;		//write IZ
				else if((anbt_com_command_HH=='P')&&(anbt_com_command_HL=='W')&&(anbt_com_command_LH=='O')&&(anbt_com_command_LL=='N'))	b_com_command_ops=0xAC;		//motor pwm on
				else if((anbt_com_command_HH=='P')&&(anbt_com_command_HL=='W')&&(anbt_com_command_LH=='O')&&(anbt_com_command_LL=='F'))	b_com_command_ops=0xAD;		//motor pwm on		
				else b_com_command_ops=0;
				//
				if(com_receive_str_index>4)	
				{
					if(com_receive_str_index==10)
					{
						for(i=0;i<4;i++) 
						{
							com_receive_data_buf[i]=com_receive_str_buf[i+4]-48;
							if(com_receive_data_buf[i]>9) com_receive_data_buf[i]=0;
						}
						//
						pid_data_buffer=com_receive_data_buf[0]*1000+com_receive_data_buf[1]*100+com_receive_data_buf[2]*10+com_receive_data_buf[3];	
						pid_data_buffer_valid=1;
					}
					else if(com_receive_str_index==12)
					{
						for(i=0;i<8;i++) 
						{			
							com_receive_data_buf[i]=com_receive_str_buf[i+2];
							if(com_receive_data_buf[i]>58) com_receive_data_buf[i]-=55;
							else com_receive_data_buf[i]-=48;
						}
						//
						for(i=0;i<4;i++) remote_ctrl_data_buffer[i]=com_receive_data_buf[i*2]*16+com_receive_data_buf[i*2+1];
						ctrl_data_buffer_valid=1;
					}
				}
				//		
				if(b_com_command_ops==0xA0)
				{
					B_Ctrl_Gas=remote_ctrl_data_buffer[0];
					B_Ctrl_Pitch=remote_ctrl_data_buffer[1]&0x7f;
					B_Ctrl_Roll=remote_ctrl_data_buffer[2]&0x7f;
					B_Ctrl_Yaw=remote_ctrl_data_buffer[3]&0x7f;
					//
					if(remote_ctrl_data_buffer[1]&0x80) B_Ctrl_Pitch=-B_Ctrl_Pitch;
					if(remote_ctrl_data_buffer[2]&0x80) B_Ctrl_Roll=-B_Ctrl_Roll;
					if((remote_ctrl_data_buffer[3]&0x80)==0x80) B_Ctrl_Yaw=-B_Ctrl_Yaw;					
					//
					B_Ctrl_Valid=1;
				}
		 }
	}
	else
	{
			com_receive_str_buf[com_receive_str_index] = com_receive_data;
			if(com_receive_str_index<AnBT_COM_Buf_Length-1) com_receive_str_index++;			//圆点博士:接收缓冲地址加1
			else com_receive_str_index=0;					//圆点博士:清0接受缓冲地址,防止数组溢出
	}
}

//---------------------------------------------------------------------------------------------------

void B_COM1_Send_Char(unsigned char ascii_code) 						//圆点博士:发送一个字符
{
	USART_SendData(USART1,ascii_code);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}	//圆点博士:等待直到发送完成
}

void B_COM1_Send_Num(unsigned char number) 	//圆点博士:发送一个数字
{
	unsigned char num_low,num_high;
	num_low=number&0x0f;													//圆点博士:取数据低位
	num_high=(number&0xf0)>>4;										//圆点博士:取数据高位
	if(num_high<10)USART_SendData(USART1,num_high+48);
	else USART_SendData(USART1,num_high+55);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}								//圆点博士:等待直到发送完成
	if(num_low<10)USART_SendData(USART1,num_low+48);
	else USART_SendData(USART1,num_low+55);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}								//圆点博士:等待直到发送完成
}

void B_COM1_Send_Str_Head(void)
{
	B_COM1_Send_Char(':');  		//圆点博士:发送字符:
	B_COM1_Send_Char('D');  		//圆点博士:发送字符D
	B_COM1_Send_Char('-');  		//圆点博士:发送字符-
}

void B_COM1_Send_Str_Body(unsigned char* str_buf)		//圆点博士:发送一个指定长度的字符串
{
	unsigned char i,str_len;
	str_len=0;
	for(i=0;i<64;i++)
	{	
		str_len++;
		if(str_buf[i]=='.') break;
	}
  for(i=0;i<str_len;i++) B_COM1_Send_Char(str_buf[i]); 	//圆点博士:发送字符:
}

void B_COM1_Send_Str_Tail(void)
{
	B_COM1_Send_Char('/');																//圆点博士:发送字符/
	B_COM1_Send_Char(13);																	//圆点博士:发送回车字符
}

void B_COM1_Send_4bits_BCD_Num(int number) 	//圆点博士:发送一个字符
{
	unsigned int num;
	unsigned char num_th[4];
	unsigned char i,bcd_code_len;
	//
	num=fabs(number);
	if(number<0) B_COM1_Send_Char('-');  		//圆点博士:发送字符-
	if(num>9999) num=9999;
	if(num>999) bcd_code_len=4;
	else if(num>99) bcd_code_len=3;
	else if(num>9) bcd_code_len=2;		
	else bcd_code_len=1;
	//
	if(num>999) 	{num_th[3]=num/1000;	num-=num_th[3]*1000;}
	if(num>99) 	 	{num_th[2]=num/100;		num-=num_th[2]*100; }
	if(num>9) 		{num_th[1]=num/10;		num-=num_th[1]*10;	}	
	num_th[0]=num;
	//
	for(i=0;i<bcd_code_len;i++) B_COM1_Send_Char(num_th[bcd_code_len-1-i]+48);
}

//---------------------------------------------------------------------------------------------------

void B_COM1_Communication(void)
{
	unsigned char i;
	//
	if(b_com_command_ops==0xA1) 			//圆点博士:返回电池电压,注意,如果电机电源没开,将返回0
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("read bat voltage .");
		B_ADC_Get_ADC_Value();
		B_COM1_Send_4bits_BCD_Num(b_bat_value);	
		B_COM1_Send_Str_Tail();
	}
	//
	else if(b_com_command_ops==0xA2) //圆点博士:返回版本号
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("version 02.");
		B_COM1_Send_Str_Tail();
	}
	//
	else if(b_com_command_ops==0xA3) //圆点博士:返回遥控器数据
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("read remote control data .");
		//	
		B_COM1_Send_4bits_BCD_Num(B_Ctrl_Gas);			
		B_COM1_Send_Char(',');				
		B_COM1_Send_4bits_BCD_Num(B_Ctrl_Pitch);			
		B_COM1_Send_Char(',');			
		B_COM1_Send_4bits_BCD_Num(B_Ctrl_Roll);			
		B_COM1_Send_Char(',');	
		B_COM1_Send_4bits_BCD_Num(B_Ctrl_Yaw);		
		//		
		B_COM1_Send_Str_Tail();
	}
	//
	else if(b_com_command_ops==0xA4) 	//圆点博士:返回P值
	{		
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("read All P data .");
		//
		for(i=0;i<3;i++)
		{
			B_COM1_Send_4bits_BCD_Num(pid_setting_P_value[i]);			
			B_COM1_Send_Char(',');		
		}
		//
		B_COM1_Send_Str_Tail();
	}
	//
	else if(b_com_command_ops==0xA5) 	//圆点博士:返回I值
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("read All D data .");
		//
		for(i=0;i<3;i++)
		{
			B_COM1_Send_4bits_BCD_Num(pid_setting_D_value[i]);			
			B_COM1_Send_Char(',');		
		}
		//
		B_COM1_Send_Str_Tail();
	}
	//
	else if(b_com_command_ops==0xA6)  //圆点博士:设置X轴P值
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("write PX data .");
		//
		if(pid_data_buffer_valid==1)
		{
			pid_setting_P_value[0]=pid_data_buffer;
			pid_data_buffer_valid=0;
		}
		//
		B_COM1_Send_4bits_BCD_Num(pid_data_buffer);	
		B_COM1_Send_Str_Tail();		
	}
	//
	else if(b_com_command_ops==0xA7) //圆点博士:设置Y轴P值
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("write PY data .");		
		//
		if(pid_data_buffer_valid==1)
		{
			pid_setting_P_value[1]=pid_data_buffer;
			pid_data_buffer_valid=0;
		}
		//
		B_COM1_Send_4bits_BCD_Num(pid_data_buffer);	
		B_COM1_Send_Str_Tail();		
	}
	//
	else if(b_com_command_ops==0xA8) //圆点博士:设置Z轴P值
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("write PZ data .");				
		//
		if(pid_data_buffer_valid==1)
		{
			pid_setting_P_value[2]=pid_data_buffer;
			pid_data_buffer_valid=0;
		}
		//
		B_COM1_Send_4bits_BCD_Num(pid_data_buffer);	
		B_COM1_Send_Str_Tail();		
	}
	//
		else if(b_com_command_ops==0xA9) //圆点博士:设置X轴I值
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("write DX data .");	
		//
		if(pid_data_buffer_valid==1)
		{
			pid_setting_D_value[0]=pid_data_buffer;
			pid_data_buffer_valid=0;
		}
		//
		B_COM1_Send_4bits_BCD_Num(pid_data_buffer);	
		B_COM1_Send_Str_Tail();			
	}
	//
		else if(b_com_command_ops==0xAA) //圆点博士:设置Y轴I值
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("write DY data .");	
		//
		if(pid_data_buffer_valid==1)
		{
			pid_setting_D_value[1]=pid_data_buffer;;
			pid_data_buffer_valid=0;
		}
		//
		B_COM1_Send_4bits_BCD_Num(pid_data_buffer);	
		B_COM1_Send_Str_Tail();		
	}
	//
		else if(b_com_command_ops==0xAB) //圆点博士:设置Z轴I值
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("write DZ data .");	
		//
		if(pid_data_buffer_valid==1)
		{
			pid_setting_D_value[2]=pid_data_buffer;
			pid_data_buffer_valid=0;
		}
		//
		B_COM1_Send_4bits_BCD_Num(pid_data_buffer);	
		B_COM1_Send_Str_Tail();		
	}
	//
	else if(b_com_command_ops==0xAC) //圆点博士:打开电机电源
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("motor power on.");	
		B_COM1_Send_Str_Tail();		
		//
		B_Ctrl_Gas=0;					//圆点博士:复位遥控数据
		B_Ctrl_Pitch=0;				//圆点博士:复位遥控数据
		B_Ctrl_Roll=0;				//圆点博士:复位遥控数据
		B_Ctrl_Yaw=0;					//圆点博士:复位遥控数据
		B_MOTOR_LED_ON();			//圆点博士:点亮LED
		B_Motor_Power_On();   //圆点博士:打开电机电源
	}
	//
	else if(b_com_command_ops==0xAD) //圆点博士:关闭电机电源
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("motor power off.");	
		B_COM1_Send_Str_Tail();		
		//
		B_Ctrl_Gas=0;					//圆点博士:复位遥控数据
		B_Ctrl_Pitch=0;				//圆点博士:复位遥控数据
		B_Ctrl_Roll=0;				//圆点博士:复位遥控数据
		B_Ctrl_Yaw=0;					//圆点博士:复位遥控数据
		B_MOTOR_LED_OFF();		//圆点博士:关闭LED
	  B_Motor_Power_Off();  //圆点博士:关闭电机电源
	}
	else
	{
		B_COM1_Send_Str_Head();
		B_COM1_Send_Str_Body("unknown command.");	//圆点博士:未知命令
		B_COM1_Send_Str_Tail();		
	}
}

//---------------------------------------------------------------------------------------------------

void B_COM1_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	//
  GPIO_InitStructure.GPIO_Pin = B_COM1_TX;					//圆点博士:设置PA9管脚为串口TX
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 		//圆点博士:设置串口TX最大允许输出速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   		//圆点博士:设置串口TX为输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//
  GPIO_InitStructure.GPIO_Pin = B_COM1_RX;					//圆点博士:设置PA9管脚为串口RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //圆点博士:设置串口RX为输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

void B_COM1_Port_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
	//
  USART_InitStructure.USART_BaudRate = 115200;									//圆点博士:设置串口波特率为115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //圆点博士:设置串口数据长度为8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;        //圆点博士:设置串口停止位长度为1位
  USART_InitStructure.USART_Parity = USART_Parity_No ;					//圆点博士:设置串口奇偶校验为无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //圆点博士:设置串口数据流控制为无
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//圆点博士:设置串口为发送和接收模式
  USART_Init(USART1, &USART_InitStructure);			//圆点博士:设置串口参数
	//
	B_COM1_NVIC_Configuration();					//圆点博士:设置中断优先级
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//圆点博士:允许接收中断
  USART_Cmd(USART1, ENABLE);  									//圆点博士:使能串口
	//
	B_COM1_Send_Str_Head();
	B_COM1_Send_Str_Body("finish to init bluetooth device.");	//圆点博士:未知命令
	B_COM1_Send_Str_Tail();		
}

void B_COM1_NVIC_Configuration(void)				//圆点博士:设置串口中断优先级
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

unsigned char B_COM1_Task_Process(void)
{
	if((b_com_command_ops>0xA0)&&(b_com_command_ops<0xAF))
	{
		com_status_is_idle=0;					//圆点博士:设置串口忙标志
		B_COM1_Communication();   //圆点博士:执行命令
		b_com_command_ops=0;			//圆点博士:清空命令操作标志
		com_status_is_idle=1;					//圆点博士:清除串口忙标志
	} 
	else if((b_com_command_ops==0xA0))//  respond to gas
	{
		if(B_Ctrl_Valid==1)					//圆点博士:检查控制数据是否有效
		{	
			com_status_is_idle=0;					//圆点博士:设置串口忙标志			
			//			
			if(B_Ctrl_Gas>B_Ctrl_Gas_Noise) b_fly_gas=B_Ctrl_Gas;
			else b_fly_gas=0;
			if(fabs(b_fly_pitch)>B_Ctrl_Dir_Noise) b_fly_pitch=B_Ctrl_Pitch;
			else b_fly_pitch=0;
			if(fabs(b_fly_roll)>B_Ctrl_Dir_Noise) b_fly_roll=B_Ctrl_Roll;
			else b_fly_roll=0;
			if(fabs(b_fly_yaw)>B_Ctrl_Dir_Noise) b_fly_yaw=B_Ctrl_Yaw;
			b_fly_yaw=0;
			//
			B_Ctrl_Valid=0;					//圆点博士:清除控制信号标志
			com_status_is_idle=1;				//圆点博士:清除串口忙标志
		}
	}	
	return b_fly_gas;
}



