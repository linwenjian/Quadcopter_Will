/*************************************************************************************************************
圆点博士小四轴飞行器2014版配套源代码声明:
该源代码仅供参考,圆点博士不对源代码提供任何形式的担保,也不对因使用该源代码而出现的损失负责.
用户可以以学习的目的修改和使用该源代码.
但用户在修改该源代码时,不得移除该部分版权信息，必须保留原版声明.

更多信息，请访问官方网站www.etootle.com, 官方博客:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "stm32f10x_lib.h"
#include "b_bluetooth.h"
//
#define B_VOLTAGE_CHA 		GPIO_Pin_0	 	//圆点博士:PB0
#define B_VOLTAGE_CHB 		GPIO_Pin_1	  //圆点博士:PB1
#define B_VOLTAGE_PORT   GPIOB
//
#define	B_ADC_Address	((u32)0x4001244C)
//
void B_ADC_IO_Configuration(void);
void B_ADC_DEVICE_Configuration(void);
void B_ADC_Configuration(void);
void B_ADC_Get_ADC_Value(void);

