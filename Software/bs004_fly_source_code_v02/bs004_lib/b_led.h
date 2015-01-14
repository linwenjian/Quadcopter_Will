/*************************************************************************************************************
圆点博士小四轴飞行器2014版配套源代码声明:
该源代码仅供参考,圆点博士不对源代码提供任何形式的担保,也不对因使用该源代码而出现的损失负责.
用户可以以学习的目的修改和使用该源代码.
但用户在修改该源代码时,不得移除该部分版权信息，必须保留原版声明.

更多信息，请访问官方网站www.etootle.com, 官方博客:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "stm32f10x_it.h"
//
#define AnBT_MOTOR_LED_A				GPIO_Pin_15		//PA15
#define AnBT_MOTOR_LED_A_PORT		GPIOA
#define AnBT_MOTOR_LED_B				GPIO_Pin_6		//PB6
#define AnBT_MOTOR_LED_B_PORT		GPIOB
#define AnBT_MOTOR_LED_C				GPIO_Pin_5		//PB5
#define AnBT_MOTOR_LED_C_PORT		GPIOB
#define AnBT_MOTOR_LED_D				GPIO_Pin_7		//PB7
#define AnBT_MOTOR_LED_D_PORT		GPIOB

void B_LED_GPIO_Configuration(void);
void B_MOTOR_LED_ON(void);
void B_MOTOR_LED_OFF(void);

