/*************************************************************************************************************
圆点博士小四轴飞行器2014版配套源代码声明:
该源代码仅供参考,圆点博士不对源代码提供任何形式的担保,也不对因使用该源代码而出现的损失负责.
用户可以以学习的目的修改和使用该源代码.
但用户在修改该源代码时,不得移除该部分版权信息，必须保留原版声明.

更多信息，请访问官方网站www.etootle.com, 官方博客:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "stm32f10x_lib.h"
#include <math.h>
#include "b_led.h"
#include "b_bluetooth.h"
#include "b_motor.h"
#include "b_adc.h"
#include "b_sys.h"
#include "b_mpu6050.h"
#include "b_imu.h"
#include "b_parameter.h"

#define IDLE 0
#define UPDATE_COMMAND 1
#define UPDATE_MOTOR   2
#define RESET_MOTOR    3
