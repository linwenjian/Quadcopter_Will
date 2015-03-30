#include "quad_common.h"

extern uint16_t sampleTimes;

void motor_pwm_reflash(uint32_t uDutyCyclePercent0 ,uint32_t uDutyCyclePercent1,
                       uint32_t uDutyCyclePercent2 ,uint32_t uDutyCyclePercent3 )
{
  uint16_t uMod, uCnv0, uCnv1,uCnv2,uCnv3;
  uint32_t ftmBaseAddr = g_ftmBaseAddr[0];

//  //set the dutyCycle to 50% (no speed on motor) if RC is lock
//  if(isRCunlock == false)
//  {
//    uDutyCyclePercent0 = 50;
//    uDutyCyclePercent1 = 50;
//    uDutyCyclePercent2 = 50;
//    uDutyCyclePercent3 = 50;
//  }  //this function should be in motor_pid_control

  uMod = FTM_HAL_GetMod(ftmBaseAddr);
  uCnv0 = uMod * uDutyCyclePercent0 / 100;
  uCnv1 = uMod * uDutyCyclePercent1 / 100;
  uCnv2 = uMod * uDutyCyclePercent2 / 100;
  uCnv3 = uMod * uDutyCyclePercent3 / 100;

  /* For 100% duty cycle */
  if(uCnv0 >= uMod)
  {
    uCnv0 = uMod + 1;
  }
  if(uCnv1 >= uMod)
  {
    uCnv1 = uMod + 1;
  }
  if(uCnv2 >= uMod)
  {
    uCnv2 = uMod + 1;
  }
  if(uCnv3 >= uMod)
  {
    uCnv3 = uMod + 1;
  }
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 0, uCnv0);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 1, uCnv1);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 2, uCnv2);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 3, uCnv3);

  FTM_HAL_SetSoftwareTriggerCmd(g_ftmBaseAddr[0], true);
}

void motor_cnv_reflash(uint16_t uCnv0 ,uint16_t uCnv1,
                       uint16_t uCnv2 ,uint16_t uCnv3 )
{
//  uint16_t uMod, uCnv0, uCnv1,uCnv2,uCnv3;
  uint16_t uMod;
  uint32_t ftmBaseAddr = g_ftmBaseAddr[0];

//  //set the dutyCycle to 50% (no speed on motor) if RC is lock
//  if(isRCunlock == false)
//  {
//    uDutyCyclePercent0 = 50;
//    uDutyCyclePercent1 = 50;
//    uDutyCyclePercent2 = 50;
//    uDutyCyclePercent3 = 50;
//  }  //this function should be in motor_pid_control

  uMod = FTM_HAL_GetMod(ftmBaseAddr);
//  uCnv0 = uMod * uDutyCyclePercent0 / 100;
//  uCnv1 = uMod * uDutyCyclePercent1 / 100;
//  uCnv2 = uMod * uDutyCyclePercent2 / 100;
//  uCnv3 = uMod * uDutyCyclePercent3 / 100;

  /* For 100% duty cycle */
  if(uCnv0 >= uMod)
  {
    uCnv0 = uMod + 1;
  }
  if(uCnv1 >= uMod)
  {
    uCnv1 = uMod + 1;
  }
  if(uCnv2 >= uMod)
  {
    uCnv2 = uMod + 1;
  }
  if(uCnv3 >= uMod)
  {
    uCnv3 = uMod + 1;
  }
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 0, uCnv0);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 1, uCnv1);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 2, uCnv2);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 3, uCnv3);

  FTM_HAL_SetSoftwareTriggerCmd(g_ftmBaseAddr[0], true);
}

//void motor_pid_control(uint32_t throttleDutyCycle,
//                       volatile imu_float_euler_angle_t * expectAngel,
//                       volatile imu_float_euler_angle_t * currentAngel,
//                       bool RCunlock )  //unfinished.20150105
//{
//  pitch_p,pitch_i,pitch_d;
//  yaw_p,yaw_i,yaw_d;
//  roll_p,roll_i,roll_d;
//
//
//
//
//  if(RCunlock == true)
//  {
//    motor_pwm0 = throttle + pitch_out + 0        + yaw_out ;
//    motor_pwm1 = throttle + 0         + roll_out - yaw_out ;
//    motor_pwm2 = throttle - pitch_out - roll_out + yaw_out ;
//    motor_pwm3 = throttle - pitch_out - 0        - yaw_out ;
//  }
//  else
//  {
//    motor_pwm0 = 50 ;
//    motor_pwm1 = 50 ;
//    motor_pwm2 = 50 ;
//    motor_pwm3 = 50 ;
//  }
//
//  motor_pwm_reflash(motor_pwm0,motor_pwm1,motor_pwm2,motor_pwm3);
//}


void IncPIDInit(pid_t *sptr)
{
  sptr->SumError   = 0;
  sptr->LastError = 0;    //Error[-1]
  sptr->PrevError = 0;    //Error[-2]

  sptr->LastDError = 0;    //dError[-1]

  sptr->Proportion = 0;    //比例常数 Proportional Const
  sptr->Integral   = 0;    //积分常数 Integral Const
  sptr->Derivative = 0;    //微分常数 Derivative Const
  sptr->ExpectPoint   = 0;
}


////IncPIDCalc ( )
////原        形  int IncPIDCalc(int)
////描        述  增量式 PID 计算
////输入参数  PID 调节当前采样值
////输出参数  计算增量
////头  文  件  Spmc75_PID.h
////库  文  件  DigitalPID_V100
////注意事项  （式 2－5）增量式 PID 算法的实现。
////例        子  uiGoalvalue  ＋＝  IncPIDCalc  (1998);  //位置式 PID 控制算法通过增量式控
////制算法递推实现，当前采样得到转速 1998rpm。
////增量式 PID 控制设计
//int IncPIDCalc(int CurrentPoint) //
//{
//  register int iError, iIncpid;
//  //当前误差
//  iError = sptr->ExpectPoint - CurrentPoint;
//  //增量计算
//  iIncpid = sptr->Proportion * iError               //E[k]项
//    - sptr->Integral   * sptr->LastError     //E[k－1]项
//      + sptr->Derivative * sptr->PrevError;   //E[k－2]项
//  //存储误差，用于下次计算
//  sptr->PrevError = sptr->LastError;
//  sptr->LastError = iError;
//  //返回增量值
//  return(iIncpid);
//}

int16_t IncPIDCalc(double CurrentPoint , pid_t *sptr)
{
  int16_t  iIncpid;
  double iError;
  iError = sptr->ExpectPoint - CurrentPoint;    //当前误差

  //增量计算
  iIncpid =(int16_t)(sptr->Proportion * iError              //E[k]项
          - sptr->Integral   * sptr->LastError     //E[k－1]项
          + sptr->Derivative * sptr->PrevError);    //E[k－2]项

  //存储误差，用于下次计算
  sptr->PrevError = sptr->LastError;
  sptr->LastError = iError;

  //返回增量值
  return(iIncpid);
}

double List_MaxMin(double num, double max, double min)
{
    if(num > max)
    {
        return max;
    }
    else if(num < min)
    {
        return min;
    }
    else
    {
        return num;
    }
}

double LocPIDCalc(double CurrentPoint , pid_t *sptr , double gyro_d_value ,double dError_lowpass_percent,bool isYaw1stPID)
{
  double  iError,dError,dError_for_use;
  double re_value;
  iError = sptr->ExpectPoint - CurrentPoint;       //偏差
  if(isYaw1stPID == true)
  {
    if(iError >= 180)
    {
      iError -= 360;
    }
    else if(iError <= -180)
    {
      iError += 360;
    }
  }
  sptr->SumError += iError;               //积分

  //积分限幅
  sptr->SumError = List_MaxMin(sptr->SumError, (0.8* (ftm_uMod_global+1) / (sptr->Integral + 0.000001)), ((-0.8) * (ftm_uMod_global+1)/ (sptr->Integral + 0.000001)) );
//  if(sptr->SumError > ftm_cnv_max_global)
//  { sptr->SumError  = ftm_cnv_max_global ;}
//  if(sptr->SumError < ((-1)*ftm_cnv_max_global))
//  { sptr->SumError  = (-1)*ftm_cnv_max_global) ;}
    //sptr->SumError = 0;

  dError = iError - sptr->LastError;     //微分

  dError_for_use = dError * dError_lowpass_percent + sptr->LastDError*(1-dError_lowpass_percent);
 // dError = -gyro_d_value;
  sptr->LastError = iError;
  sptr->LastDError = dError;

  re_value = (double)(sptr->Proportion * iError  //比例项
           + sptr->Integral * sptr->SumError   //积分项
           + sptr->Derivative * dError_for_use);
  return(re_value);      //微分项
}


//pid_t pitch_pid = {
//  .ExpectPoint = 0,
//  .SumError    = 0,
//
//  .Proportion = 10,
//  .Integral   = 0.10,//0.1 ,
//  .Derivative = 9,//0.08 * 100,//0.05,
//
//  .LastError = 0,
//  .PrevError = 0,
//};

////////////////////roll
pid_t roll_pid00 = {
  .ExpectPoint = 0,
  .SumError    = 0,

  .Proportion = 4.0,
  .Integral   = 0,
  .Derivative = 0,//,//5,//,//0.08 * 100,//0.05,

  .LastError = 0,
  .PrevError = 0,
};

pid_t roll_pid11 = {
  .ExpectPoint = 0,
  .SumError    = 0,

  .Proportion = 4.0,//8,
  .Integral   = 0.02,//0.07,//0.1,//0.1,//0.05,//0.05 ,
  .Derivative = 4.5,//4.5,//,//5,//,//0.08 * 100,//0.05,

  .LastError = 0,
  .PrevError = 0,
};
////////////////////pitch
pid_t pitch_pid00 = {
  .ExpectPoint = 0,
  .SumError    = 0,

  .Proportion = 4.0,//3,
  .Integral   = 0,
  .Derivative = 0,//,//5,//,//0.08 * 100,//0.05,

  .LastError = 0,
  .PrevError = 0,
};

pid_t pitch_pid11 = {
  .ExpectPoint = 0,
  .SumError    = 0,

  .Proportion = 4.0,//,//8,
  .Integral   = 0.02,//0.07,//0.1,//0.1,//0.05,//0.05 ,
  .Derivative = 4.5 ,//4.5,//4.5,//,//5,//,//0.08 * 100,//0.05,

  .LastError = 0,
  .PrevError = 0,
};
//////////////////////////
////////////////////yaw
pid_t yaw_pid00 = {
  .ExpectPoint = 0,
  .SumError    = 0,

  .Proportion = 4.5,//3,
  .Integral   = 0,
  .Derivative = 0,//,//5,//,//0.08 * 100,//0.05,

  .LastError = 0,
  .PrevError = 0,
};

pid_t yaw_pid11 = {
  .ExpectPoint = 0,
  .SumError    = 0,

  .Proportion = 4.5,//,//8,
  .Integral   = 0.03,//0.06,//0.1,//0.1,//0.05,//0.05 ,
  .Derivative = 4.2,//4.5,//4.5,//,//5,//,//0.08 * 100,//0.05,

  .LastError = 0,
  .PrevError = 0,
};
////////////////////////////////////////////////////
uint16_t motor_pwm0_cnv  = 0;
uint16_t motor_pwm1_cnv  = 0;
uint16_t motor_pwm2_cnv  = 0;
uint16_t motor_pwm3_cnv  = 0;

void motor_pid_control(uint32_t throttleDutyCycle,
                       volatile imu_float_euler_angle_t * expectAngel,
                        volatile imu_float_euler_angle_t * currentAngel,
                       pid_t *pitch_pid0,
                       pid_t *pitch_pid1,
                       pid_t *yaw_pid0,
                       pid_t *yaw_pid1,
                       pid_t *roll_pid0,
                       pid_t *roll_pid1,
                       bool RCunlock )
{
  /*          这个是pitch轴，左低右高，pitch值为负值，差距越大值越大。翻过-180度后变为179...
  *               |
  *      Motor0   |    Motor1
  *         ╲    |    ／
  *           ╲  |  ／
  *   __________╲|／___________这个是ROLL轴，前（上）高，（下）低，ROLL值为正，差距越大值越大。翻过180度后变为-179...
  *             ／|╲
  *           ／  |  ╲
  *         ／    |    ╲
  *      Motor3   |    Motor2
  *               |
  */
  //
  static double  pitch_out0 = 0,roll_out0 = 0,yaw_out0 = 0;
  static int32_t pitch_out1 = 0,roll_out1 = 0,yaw_out1 = 0;
////  static uint16_t pitch_out = 0,roll_out = 0,yaw_out = 0;
////  static int16_t motor_pwm0_duty  = 0;
////  static int16_t motor_pwm1_duty  = 0;
////  static int16_t motor_pwm2_duty  = 0;
////  static int16_t motor_pwm3_duty  = 0;
//  static uint16_t motor_pwm0_cnv  = 0;
//  static uint16_t motor_pwm1_cnv  = 0;
//  static uint16_t motor_pwm2_cnv  = 0;
//  static uint16_t motor_pwm3_cnv  = 0;

  pitch_pid0->ExpectPoint = expectAngel->imu_pitch;
  roll_pid0->ExpectPoint  = expectAngel->imu_roll;
  yaw_pid0->ExpectPoint  = expectAngel->imu_yaw;

  //sendLineX(0x1f,(float)expectAngel->imu_yaw);
  //sendLineX(0x4f,(float)yaw_pid0->ExpectPoint);

  if(throttleDutyCycle > THROTTLE_DUTY_MAX) {throttleDutyCycle = THROTTLE_DUTY_MAX;} ////油门最大值限制，做保护

  //做一个转换，比如 umod=59999;所以PWM占空比50%~100%是 30000 - 60000 ，所以要
  //throttleDutyCycle = throttleDutyCycle * 600
  uint32_t throttleDutyCycle_cnv = throttleDutyCycle * (ftm_uMod_global+1) / 100 ;

  static bool firstSumErrorFlag = false;
  if(firstSumErrorFlag == false)
  {
    pitch_pid00.SumError = 0;
    pitch_pid11.SumError = 0;
    roll_pid00.SumError  = 0;
    roll_pid11.SumError  = 0;
    yaw_pid00.SumError   = 0;
    yaw_pid11.SumError   = 0;
  }

  //if((gyro_offset_done == true)&&(RCunlock == true) && (throttleDutyCycle_cnv > ftm_cnv_min_global))// && (isAngleProtected == false))
  if((sampleTimes > 400) && (RCunlock == true) && (throttleDutyCycle_cnv > ftm_cnv_min_global))// && (isAngleProtected == false))
  {
    if(throttleDutyCycle_cnv > (55 * (ftm_uMod_global+1) / 100))
    {
      firstSumErrorFlag = true;
    }
    roll_out0 = (double)(LocPIDCalc(currentAngel->imu_roll ,roll_pid0,gyro_roll_global,1,false));
    roll_pid1->ExpectPoint  = -1 * roll_out0;
    roll_out1  =(int32_t)(LocPIDCalc( (double)(gyro_roll_global) ,roll_pid1,gyro_roll_global,0.5,false));

    pitch_out0 = (double)(LocPIDCalc(currentAngel->imu_pitch ,pitch_pid0,gyro_pitch_global,1,false));
    pitch_pid1->ExpectPoint  = -1 * pitch_out0;
    pitch_out1  =(int32_t)(LocPIDCalc( (double)(gyro_pitch_global) ,pitch_pid1,gyro_pitch_global,0.5,false));

    yaw_out0 = (double)(LocPIDCalc(currentAngel->imu_yaw ,yaw_pid0,gyro_yaw_global,1,true));
    yaw_pid1->ExpectPoint = yaw_out0;
    yaw_out1 =(int32_t)(LocPIDCalc( (double)(gyro_yaw_global) ,yaw_pid1,gyro_yaw_global,0.5,false));

 //   yaw_out1 = 0;


#if 0
    sendLineX(0x1f,(((float)currentAngel->imu_roll)));
    sendLineX(0x2f,(((float)currentAngel->imu_pitch)));
    sendLineX(0x3f,(((float)currentAngel->imu_yaw)));
    sendLineX(0x4f,(((float)roll_out1)));
    sendLineX(0x5f,(((float)pitch_out1)));
    sendLineX(0x6f,(((float)yaw_out1)));
    //sendLineX(0x6f,(float)memsRawDate.accel_z);
    //sendLineX(0x4f,(((float)motor_pwm3_cnv)/ftm_uMod_global));
#endif

#if 1
    motor_pwm0_cnv  = (uint16_t)(throttleDutyCycle_cnv - pitch_out1 + roll_out1 + yaw_out1) ;
    motor_pwm1_cnv  = (uint16_t)(throttleDutyCycle_cnv - pitch_out1 - roll_out1 - yaw_out1) ;
    motor_pwm2_cnv  = (uint16_t)(throttleDutyCycle_cnv + pitch_out1 - roll_out1 + yaw_out1) ;
    motor_pwm3_cnv  = (uint16_t)(throttleDutyCycle_cnv + pitch_out1 + roll_out1 - yaw_out1) ;
#else
//    motor_pwm0_cnv  = (uint16_t)(throttleDutyCycle_cnv + pitch_out1 + yaw_out1) ;
//    motor_pwm1_cnv  = (uint16_t)(throttleDutyCycle_cnv + roll_out1  - yaw_out1) ;
//    motor_pwm2_cnv  = (uint16_t)(throttleDutyCycle_cnv - pitch_out1 + yaw_out1) ;
//    motor_pwm3_cnv  = (uint16_t)(throttleDutyCycle_cnv - roll_out1  - yaw_out1) ;
#endif
    if(motor_pwm0_cnv > ftm_cnv_max_global){ motor_pwm0_cnv = ftm_cnv_max_global;}
    if(motor_pwm1_cnv > ftm_cnv_max_global){ motor_pwm1_cnv = ftm_cnv_max_global;}
    if(motor_pwm2_cnv > ftm_cnv_max_global){ motor_pwm2_cnv = ftm_cnv_max_global;}
    if(motor_pwm3_cnv > ftm_cnv_max_global){ motor_pwm3_cnv = ftm_cnv_max_global;}

    if(motor_pwm0_cnv < ftm_cnv_min_global){ motor_pwm0_cnv = ftm_cnv_min_global;}
    if(motor_pwm1_cnv < ftm_cnv_min_global){ motor_pwm1_cnv = ftm_cnv_min_global;}
    if(motor_pwm2_cnv < ftm_cnv_min_global){ motor_pwm2_cnv = ftm_cnv_min_global;}
    if(motor_pwm3_cnv < ftm_cnv_min_global){ motor_pwm3_cnv = ftm_cnv_min_global;}
  }
  else
  {
    firstSumErrorFlag = false;
    motor_pwm0_cnv  = ftm_cnv_stop_global;
    motor_pwm1_cnv  = ftm_cnv_stop_global;
    motor_pwm2_cnv  = ftm_cnv_stop_global;
    motor_pwm3_cnv  = ftm_cnv_stop_global;
//    pitch_pid00.SumError = 0;
//    pitch_pid11.SumError = 0;
//    roll_pid00.SumError  = 0;
//    roll_pid11.SumError  = 0;
//    yaw_pid00.SumError   = 0;
//    yaw_pid11.SumError   = 0;
  }

//  PRINTF("pwm0 = %3d ,pwm1 = %3d ,pwm2 = %3d ,pwm3 = %3d\r\n" ,motor_pwm0_duty,motor_pwm1_duty,motor_pwm2_duty,motor_pwm3_duty);
//   PRINTF("cnv0 = %3d ,cnv1 = %3d ,cnv2 = %3d ,cnv3 = %3d\r\n" ,motor_pwm0_cnv,motor_pwm1_cnv,motor_pwm2_cnv,motor_pwm3_cnv);
#if 0
motor_pwm0_cnv = ftm_cnv_stop_global;
motor_pwm2_cnv = ftm_cnv_stop_global;
motor_pwm1_cnv = ftm_cnv_stop_global;
motor_pwm3_cnv = ftm_cnv_stop_global;
#endif
  motor_cnv_reflash((uint16_t)motor_pwm0_cnv,
                    (uint16_t)motor_pwm1_cnv,
                    (uint16_t)motor_pwm2_cnv,
                    (uint16_t)motor_pwm3_cnv);
}

//{
//  uint32_t throttleDutyCycle_cnv = throttleDutyCycle * (ftm_uMod_global+1) / 100 ;
//  if((RCunlock == true) && (throttleDutyCycle_cnv > ftm_cnv_min_global))
//  {
//    motor_pwm0_cnv = throttleDutyCycle_cnv;
//    motor_pwm1_cnv = throttleDutyCycle_cnv;
//    motor_pwm2_cnv = throttleDutyCycle_cnv;
//    motor_pwm3_cnv = throttleDutyCycle_cnv;
//  }
//  else
//  {
//    motor_pwm0_cnv = ftm_cnv_stop_global;
//    motor_pwm1_cnv = ftm_cnv_stop_global;
//    motor_pwm2_cnv = ftm_cnv_stop_global;
//    motor_pwm3_cnv = ftm_cnv_stop_global;
//  }
//
//  motor_cnv_reflash((uint16_t)motor_pwm0_cnv,
//                    (uint16_t)motor_pwm1_cnv,
//                    (uint16_t)motor_pwm2_cnv,
//                    (uint16_t)motor_pwm3_cnv);
//}



/*******************************************************************************
 * EOF
 ******************************************************************************/
