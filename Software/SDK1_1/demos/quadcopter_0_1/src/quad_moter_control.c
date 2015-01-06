#include "quad_common.h"

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

//void motor_pid_control(uint32_t throttleDutyCycle,
//                       imu_float_euler_angle_t * expectAngel,
//                       imu_float_euler_angle_t * actualAngel,
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

/*******************************************************************************
 * EOF
 ******************************************************************************/
