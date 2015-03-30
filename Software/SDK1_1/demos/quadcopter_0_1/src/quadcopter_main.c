
///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////


#include "quad_common.h"
#include "quad_i2c_config.h"
#include "fsl_hwtimer.h"

#include "core_cm4.h"

#include "mpu6050dmp_attitude_angle_cal.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define PWM_HZ (400)

#define HWTIMER_LL_DEVIF    kSystickDevif
#define HWTIMER_LL_SRCCLK   kCoreClock
#define HWTIMER_LL_ID       0

#define HWTIMER_PERIOD     40000//  7000//   4000 //us  =4ms ,
////要死要死要死！ 最快只能7ms 扫描姿态， 遥控器由于用IO上升下降沿中断，1~2ms触发，如果用4ms定时刚好重合...
////now change to use PIT !!!!
//现在这个值 只是随便设置一个，让systemtick知道从哪个数开始递减
#define BSWAP_16(x) (uint16_t)((uint16_t)(((uint16_t)(x) & (uint16_t)0xFF00) >> 0x8) | (uint16_t)(((uint16_t)(x) & (uint16_t)0xFF) << 0x8))

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////

extern const hwtimer_devif_t kSystickDevif;
extern const hwtimer_devif_t kPitDevif;
hwtimer_t hwtimer;

ftm_user_config_t ftmInfo;

ftm_pwm_param_t ftmParam0 = {
  .mode                   = kFtmEdgeAlignedPWM,
  .edgeMode               = kFtmHighTrue,
  .uFrequencyHZ           = PWM_HZ,
  .uDutyCyclePercent      = 5,
  .uFirstEdgeDelayPercent = 0,
};
ftm_pwm_param_t ftmParam1 = {
  .mode                   = kFtmEdgeAlignedPWM,
  .edgeMode               = kFtmHighTrue,
  .uFrequencyHZ           = PWM_HZ,
  .uDutyCyclePercent      = 5,
  .uFirstEdgeDelayPercent = 0,
};
ftm_pwm_param_t ftmParam2 = {
  .mode                   = kFtmEdgeAlignedPWM,
  .edgeMode               = kFtmHighTrue,
  .uFrequencyHZ           = PWM_HZ,
  .uDutyCyclePercent      = 5,
  .uFirstEdgeDelayPercent = 0,
};
ftm_pwm_param_t ftmParam3 = {
  .mode                   = kFtmEdgeAlignedPWM,
  .edgeMode               = kFtmHighTrue,
  .uFrequencyHZ           = PWM_HZ,
  .uDutyCyclePercent      = 5,
  .uFirstEdgeDelayPercent = 0,
};

const char trans_header_table[3] = {0x88, 0xAF, 0x1C};
const char trans_header_table_pwm[3] = {0x88, 0xAE, 0x12};
//char trans_header_table1[] = {0x88, 0xA1, 2,0,0,0};
trans_packet_t packet_upper_PC ;
trans_packet_pwm_t packet_pwm_upper_PC ;
volatile mems_data_t   memsRawDate ;
volatile imu_float_euler_angle_t quadAngle;

bool gyro_offset_done = false;
bool isFXOS8700Int1Trigger = false;

/*!
 * @brief Main function
 */

int temp_duty = 1;
int temp_count = 0;
int temp_flag = 0;
int temp_count1 = 0;
int temp_count_test = 0;
int temp_count_add = 1;

const _ADC_Pin_Mux_Table_t kADCPinMuxTable[] = {
//  {HW_ADC0,0,12},
  {HW_ADC0,0,5},
  {HW_ADC0,0,13},
  {HW_ADC1,0,10},
  {HW_ADC1,0,11},
  {HW_ADC1,0,12},
  {HW_ADC1,0,13},
  {HW_ADC1,0,14},
  {HW_ADC1,0,15}
};

volatile uint32_t ADC_count_n = 0;
adc16_chn_config_t MyChnConfig;
volatile uint32_t ADC_value[8]= {0};
volatile uint32_t ADC_value_min[8]= { 0xffffffff,
                                      0xffffffff,
                                      0xffffffff,
                                      0xffffffff,
                                      0xffffffff,
                                      0xffffffff,
                                      0xffffffff,
                                      0xffffffff};
volatile uint32_t ADC_value_max[8]= {0};
volatile bool g_AdcConvIntCompleted = false;

volatile uint32_t remoteControlValue[8] = {0};
volatile uint32_t remoteControlIntTimes[8] = {0};
uint32_t remoteControlPinNum[] = {2,3,4,5,6,7,10,11};

uint32_t uDutyCycle_add = 0;
// 500Hz PWM， start from 50% duty cylce.
//%0 duty cycle for 2 seconds , then 50% duty cycle for 2 seconds.

void hwtimer_callback(void* data)
   {
     ///*Start 简化版更改占空比***************/
     ////     uint16_t uMod, uCnv0, uCnv1,uCnv2,uCnv3;
     ////     uint32_t ftmBaseAddr = g_ftmBaseAddr[0];
     ////
     ////     uMod = FTM_HAL_GetMod(ftmBaseAddr);
     ////
     ////     uCnv0 = uMod * ftmParam0.uDutyCyclePercent / 100;
     ////     /* For 100% duty cycle */
     ////     if(uCnv >= uMod)
     ////     {
     ////       uCnv = uMod + 1;
     ////     }
     ////     uCnv1 = uMod * ftmParam1.uDutyCyclePercent / 100;
     ////     uCnv2 = uMod * ftmParam2.uDutyCyclePercent / 100;
     ////     uCnv3 = uMod * ftmParam3.uDutyCyclePercent / 100;
     ///*End 简化版更改占空比*************/
     //
     //     FTM_DRV_PwmChangeDutyCycle(0, &ftmParam0, 0);
     //     FTM_DRV_PwmChangeDutyCycle(0, &ftmParam1, 1);
     //     FTM_DRV_PwmChangeDutyCycle(0, &ftmParam2, 2);
     //     FTM_DRV_PwmChangeDutyCycle(0, &ftmParam3, 3);
     //
     //     FTM_HAL_SetSoftwareTriggerCmd(g_ftmBaseAddr[0], true);
     //
     //     if(i==0)
     //     {
     //       LED2_ON;i=1;
     //     }
     //     else
     //     {
     //       LED2_OFF;i=0;
     //     }
   }


//PTD2_UART_rx, PTD3_UART_tx
//PTC1,2,3,4
uint32_t ftm_uMod_global=0;
uint32_t ftm_cnv_max_global=0;
uint32_t ftm_cnv_min_global=0;
uint32_t ftm_cnv_stop_global=0;




union
{
    float f32;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
    uint8_t send_2val[2];
    uint8_t send_4val[4];
}send_muxByte_Tmp;

// flag = 0x1f ~ 0x6f 上位机上面代表的六条曲线
// 如果显示不正常则在每两个发送之间添加一点延时
void sendLineX(uint8_t flag, float val)
{
	send_muxByte_Tmp.f32 = val;
//        int i =0;
//        int j = 300;
        uint8_t send_data[7];
        send_data[0] = 0x55;
        send_data[1] = flag;
        send_data[2] = send_muxByte_Tmp.send_4val[0];
        send_data[3] = send_muxByte_Tmp.send_4val[1];
        send_data[4] = send_muxByte_Tmp.send_4val[2];
        send_data[5] = send_muxByte_Tmp.send_4val[3];
        send_data[6] = 0xaa;

        UART_HAL_SendDataPolling(BOARD_DEBUG_UART_BASEADDR,send_data,7);

//	UART_HAL_Putchar(BOARD_DEBUG_UART_BASEADDR,0x55);	for(i=0;i<j;i++);
//	UART_HAL_Putchar(BOARD_DEBUG_UART_BASEADDR,flag);	for(i=0;i<j;i++);
//	UART_HAL_Putchar(BOARD_DEBUG_UART_BASEADDR,send_muxByte_Tmp.send_4val[0]);	for(i=0;i<j;i++);
//	UART_HAL_Putchar(BOARD_DEBUG_UART_BASEADDR,send_muxByte_Tmp.send_4val[1]);	for(i=0;i<j;i++);
//	UART_HAL_Putchar(BOARD_DEBUG_UART_BASEADDR,send_muxByte_Tmp.send_4val[2]);	for(i=0;i<j;i++);
//	UART_HAL_Putchar(BOARD_DEBUG_UART_BASEADDR,send_muxByte_Tmp.send_4val[3]);	for(i=0;i<j;i++);
//	UART_HAL_Putchar(BOARD_DEBUG_UART_BASEADDR,0xaa);	for(i=0;i<j;i++);
}

  //fgz define
uint32_t checkTimes = 0;
double checkTmpVal[2] = {0,0};
double X_Y_changeTmp[2] = {0,0};
/*************************************************************************************************************************************/
  /*************************************************************************************************************************************/
  float My_Pitch, My_Roll, My_Yaw;
  short My_gyro[3];
  float My_Yaw_tmp = 0;
  uint16_t sampleTimes;
  /*************************************************************************************************************************************/
  /*************************************************************************************************************************************/

int main (void)
{
  memcpy(packet_upper_PC.trans_header, trans_header_table, sizeof(trans_header_table));
  memcpy(packet_pwm_upper_PC.trans_header, trans_header_table_pwm, sizeof(trans_header_table_pwm));
  // RX buffers
  //! @param receiveBuff Buffer used to hold received data
  uint8_t receiveBuff;

  // Initialize standard SDK demo application pins
  hardware_init();

  OSA_Init();
  // Call this function to initialize the console UART. This function
  // enables the use of STDIO functions (printf, scanf, etc.)
  dbg_uart_init();
  // Print the initial banner
  PRINTF("\r\nHello World!\n\n\r");

  /*Start***FTM Init*************************************************************/
  memset(&ftmInfo, 0, sizeof(ftmInfo));
  ftmInfo.syncMethod = kFtmUseSoftwareTrig;
  FTM_DRV_Init(0, &ftmInfo);
  /*End*****FTM Init*************************************************************/

  LED2_EN;    LED3_EN;    LED4_EN;    LED5_EN;
  LED2_OFF;   LED3_OFF;   LED4_OFF;   LED5_OFF;

  I2C_fxos8700Init();
  //I2C_l3g4200dInit();
    //初始化MPU6050
  init_MPU6050();

  sampleTimes = 0;

LED2_ON;
  FTM_DRV_PwmStart(0, &ftmParam0, 0);
  FTM_DRV_PwmStart(0, &ftmParam1, 1);
  FTM_DRV_PwmStart(0, &ftmParam2, 2);
  FTM_DRV_PwmStart(0, &ftmParam3, 3);

  //SWSYNC bit is cleared at the next selected loading point after that the
  //software trigger event occurred
  //SYNCMODE = 1 ; SWRSTCNT =0 ; CNTMIN=1 ; CNTMAX=1 ;
  uint32_t ftmBaseAddr = g_ftmBaseAddr[0];
  ftm_uMod_global = FTM_HAL_GetMod(ftmBaseAddr);
  ftm_cnv_max_global  = THROTTLE_DUTY_MAX *(ftm_uMod_global+1) / 100;
  ftm_cnv_min_global  = THROTTLE_DUTY_MIN *(ftm_uMod_global+1) / 100;
  ftm_cnv_stop_global = THROTTLE_DUTY_STOP*(ftm_uMod_global+1) / 100;

  FTM_HAL_SetCounterSoftwareSyncModeCmd(ftmBaseAddr, false);
  FTM_HAL_SetMaxLoadingCmd(ftmBaseAddr, true);
  FTM_HAL_SetMinLoadingCmd(ftmBaseAddr, true);

  FTM_HAL_SetSoftwareTriggerCmd(g_ftmBaseAddr[0], true);

  // Hwtimer initialization
  if (kHwtimerSuccess != HWTIMER_SYS_Init(&hwtimer, &HWTIMER_LL_DEVIF, HWTIMER_LL_ID, 5, NULL))
  {
    PRINTF("\r\nError: hwtimer initialization.\r\n");
  }
  if (kHwtimerSuccess != HWTIMER_SYS_SetPeriod(&hwtimer, HWTIMER_LL_SRCCLK, HWTIMER_PERIOD))
  {
    PRINTF("\r\nError: hwtimer set period.\r\n");
  }
  //    if (kHwtimerSuccess != HWTIMER_SYS_RegisterCallback(&hwtimer, hwtimer_callback, NULL))
  //    {
  //        PRINTF("\r\nError: hwtimer callback registration.\r\n");
  //    }
  //    if (kHwtimerSuccess != HWTIMER_SYS_Start(&hwtimer))
  //    {
  //        PRINTF("\r\nError: hwtimer start.\r\n");
  //    }
  /* A write of any value to current value register clears the field to 0, and also clears the SYST_CSR COUNTFLAG bit to 0. */
  SysTick->VAL = 0U;
  /* Run timer and disable interrupt */
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk ;//| SysTick_CTRL_TICKINT_Msk;

  GPIO_DRV_Init(remoteControlPins,NULL);
  //    GPIO_DRV_Init(fxos8700IntPins,NULL);
  //    I2C_fxos8700AutoCalibration(); //cannot work , shit!

  /*Start***********************PIT Init*****************************************/
  // Structure of initialize PIT channel No.0
  pit_user_config_t chn0Confg = {
    .isInterruptEnabled = true,
    .isTimerChained = false,
    .periodUs = 8000u //1000000 us
  };

  // Structure of initialize PIT channel No.1
  pit_user_config_t chn1Confg = {
    .isInterruptEnabled = true,
    .isTimerChained = false,
    .periodUs = 8000u
  };

  // Init pit module and enable run in debug
  PIT_DRV_Init(BOARD_PIT_INSTANCE, false);

  // Initialize PIT timer instance for channel 0 and 1
//  PIT_DRV_InitChannel(BOARD_PIT_INSTANCE, 0, &chn0Confg);
  PIT_DRV_InitChannel(BOARD_PIT_INSTANCE, 1, &chn1Confg);

  // Start channel 0
  //    printf ("\n\rStarting channel No.0 ...");
//  PIT_DRV_StartTimer(BOARD_PIT_INSTANCE, 0);

  // Start channel 1
  //    printf ("\n\rStarting channel No.1 ...");
  PIT_DRV_StartTimer(BOARD_PIT_INSTANCE, 1);

  /*End*************************PIT Init*****************************************/

  //    NVIC_SetPriority(SysTick_IRQn, 3);
  //    NVIC_SetPriority(PORTB_IRQn,0);
  NVIC_SetPriority(PIT0_IRQn,3);
  //must set the pit isr priority lower than i2c because I run the i2c interrupt in pit_isr.

  //PTD0 to measure the cycle with the oscilloscope  or logic analyzer
  PORT_HAL_SetMuxMode(PORTD_BASE, 0, kPortMuxAsGpio);
  GPIO_DRV_SetPinDir(GPIO_MAKE_PIN(HW_GPIOD, 0U), kGpioDigitalOutput) ;
  GPIO_DRV_WritePinOutput(GPIO_MAKE_PIN(HW_GPIOD, 0U),1);

  while(1)
  {
    /*****start while(1) in mian loop*****/

static uint32_t led2_times = 0;
static int32_t led2_flag = 1;

    if (pitIsrFlag1 == true)
    {
      pitIsrFlag1 = false;
//      if(led2_times++ > 50)
//      {
//        led2_times = 0;
//        led2_flag = led2_flag * (-1);
//      }
//      if(led2_flag == 1)
//      {
//        LED2_ON;
//      }
//      else
//      {
//        LED2_OFF;
//      }

      get_AttitudeVal(My_gyro);
#if 1
      quadAngle.imu_roll = ((double)My_Roll * 0.70710678) - ((double)My_Pitch * 0.70710678);
      quadAngle.imu_pitch = (double)((double)(((double)My_Pitch * 0.70710678) + ((double)My_Roll * 0.70710678)) - 1.8) ;

      gyro_roll_global = ((double)My_gyro[1] * -0.70710678 + (double)My_gyro[0] * 0.70710678) / 16.4;
      gyro_pitch_global  = ((double)My_gyro[0] * 0.70710678 + (double)My_gyro[1] * 0.70710678) / 16.4;
      gyro_yaw_global   = ((double)My_gyro[2]) / 16.4;
#else

      quadAngle.imu_roll = (double)My_Roll;
      quadAngle.imu_pitch  = (double)My_Pitch;
      gyro_roll_global = (double)My_gyro[0] / 16.4;
      gyro_pitch_global  = (double)My_gyro[1] / 16.4;
      gyro_yaw_global   = (double)My_gyro[2] / 16.4;
#endif

      if(isRCunlock == true)
      {
        LED3_ON;
        if(sampleTimes <= 400)
        {
            sampleTimes++;
        }
      }
      else
      {
        LED3_OFF;
        LED2_OFF;
        sampleTimes = 0;
        My_Yaw_tmp = 0;
      }
      if(sampleTimes <= 400 && isRCunlock == true)
      {
          My_Yaw_tmp += My_Yaw;
      }
      else if(sampleTimes > 400)
      {
          //quadAngle.imu_yaw = (double)My_Yaw;
          LED2_ON;
          quadAngle.imu_yaw = (double)My_Yaw - My_Yaw_tmp / 400;
      }

#if 0
      sendLineX(0x1f,(float)quadAngle.imu_pitch);
      sendLineX(0x2f,(float)quadAngle.imu_roll);
      sendLineX(0x3f,(float)quadAngle.imu_yaw);

      sendLineX(0x4f,(float)gyro_pitch_global);
      sendLineX(0x5f,(float)gyro_roll_global);
      sendLineX(0x6f,(float)gyro_yaw_global);
#endif

      static uint16_t led5_i =0;
      if(led5_i==0)
      {
        LED5_ON;led5_i=1;
      }
      else
      {
        LED5_OFF;led5_i=0;
      }

      uint32_t waitTimes3s = (3000 / (chn1Confg.periodUs / 1000) ) ;
      /*Start*********匿名上位机发送的串口数据***********/
      {
        packet_upper_PC.user_data.trans_accel[0] = BSWAP_16(memsRawDate.accel_x);
        packet_upper_PC.user_data.trans_accel[1] = BSWAP_16(memsRawDate.accel_y);
        packet_upper_PC.user_data.trans_accel[2] = BSWAP_16(memsRawDate.accel_z);
        packet_upper_PC.user_data.trans_gyro[0]  = BSWAP_16((int16_t)gyro_roll_global);//memsRawDate.gyro_x);//gyro_y_before);//memsRawDate.gyro_x);
        packet_upper_PC.user_data.trans_gyro[1]  = BSWAP_16((int16_t)gyro_pitch_global);//memsRawDate.gyro_y);//gyro_y_after);//memsRawDate.gyro_y);
        packet_upper_PC.user_data.trans_gyro[2]  = BSWAP_16(memsRawDate.gyro_z);
        packet_upper_PC.user_data.trans_mag[0]  = BSWAP_16(memsRawDate.magn_x);
        packet_upper_PC.user_data.trans_mag[1]  = BSWAP_16(memsRawDate.magn_y);
        packet_upper_PC.user_data.trans_mag[2]  = BSWAP_16(memsRawDate.magn_z);

        packet_upper_PC.user_data.trans_roll = BSWAP_16((int16_t)(quadAngle.imu_roll*100));
        packet_upper_PC.user_data.trans_pitch = BSWAP_16((int16_t)(quadAngle.imu_pitch*100));
        packet_upper_PC.user_data.trans_yaw =  BSWAP_16((int16_t)(quadAngle.imu_yaw*10));

        //        unt16_t throt;
        //    unt16_t yaw;
        //    unt16_t roll;
        //    unt16_t pitch;
        //    unt16_t aux[5];
        //    unt16_t pwm[4];
        //    unt16_t votage;
        packet_pwm_upper_PC.user_data.throt = BSWAP_16((uint16_t)(remoteControlValue[kThrottle]/10));
        packet_pwm_upper_PC.user_data.yaw   = BSWAP_16((uint16_t)(remoteControlValue[kYaw]/10));
        packet_pwm_upper_PC.user_data.roll  = BSWAP_16((uint16_t)(remoteControlValue[kRoll]/10));
        packet_pwm_upper_PC.user_data.pitch = BSWAP_16((uint16_t)(remoteControlValue[kPitch]/10));

        packet_pwm_upper_PC.user_data.aux[0] = BSWAP_16((uint16_t)(100));
        packet_pwm_upper_PC.user_data.aux[1] = BSWAP_16((uint16_t)(200));
        packet_pwm_upper_PC.user_data.aux[2] = BSWAP_16((uint16_t)(300));
        packet_pwm_upper_PC.user_data.aux[3] = BSWAP_16((uint16_t)(400));
        packet_pwm_upper_PC.user_data.aux[4] = BSWAP_16((uint16_t)(500));

        packet_pwm_upper_PC.user_data.pwm[0] = BSWAP_16((uint16_t)(motor_pwm0_cnv / 600));
        packet_pwm_upper_PC.user_data.pwm[1] = BSWAP_16((uint16_t)(motor_pwm1_cnv / 600));
        packet_pwm_upper_PC.user_data.pwm[2] = BSWAP_16((uint16_t)(motor_pwm2_cnv / 600));
        packet_pwm_upper_PC.user_data.pwm[3] = BSWAP_16((uint16_t)(motor_pwm3_cnv / 600));

//        uint8_t *q = (uint8_t*)&packet_pwm_upper_PC;
//        UART_HAL_SendDataPolling(BOARD_DEBUG_UART_BASEADDR,q,32);
//
//        uint8_t *p = (uint8_t*)&packet_upper_PC;
//        UART_HAL_SendDataPolling(BOARD_DEBUG_UART_BASEADDR,p,32);

//        sendLineX(0x1f,(((float)motor_pwm0_cnv)/ftm_uMod_global));
//        sendLineX(0x2f,(((float)motor_pwm1_cnv)/ftm_uMod_global));
//        sendLineX(0x3f,(((float)motor_pwm2_cnv)/ftm_uMod_global));
//        sendLineX(0x4f,(((float)motor_pwm3_cnv)/ftm_uMod_global));
//        sendLineX(0x4f,(float)(0.25));


#if 0
        sendLineX(0x1f,(((float)quadAngle.imu_pitch)));
        sendLineX(0x2f,(((float)quadAngle.imu_roll)));
        sendLineX(0x3f,(((float)quadAngle.imu_yaw)));
            //sendLineX(0x3f,(((float)currentAngel->imu_yaw)));
    //sendLineX(0x4f,(((float)memsRawDate.accel_x)));
    //sendLineX(0x5f,(float)memsRawDate.accel_y);
                 //sendLineX(0x6f,(((float)memsRawDate.accel_z)));
        //sendLineX(0x4f,(((float)pitch_pid1->SumError)));
//        sendLineX(0x1f,(float)((((double)((double)remoteControlValue[kPitch]/1000) -180)/3)));
//        sendLineX(0x4f,(float)((((double)((double)remoteControlValue[kRoll] /1000) -180)/3)));
        //sendLineX(0x4f,(((float)motor_pwm3_cnv)/ftm_uMod_global));
#endif

      }
      /*End*********匿名上位机发送的串口数据***********/

      /*Start************Remote Controller Unlock *************/
      {

//        if(isRCunlock == true)
//        {
//          LED3_ON;
//        }
//        else
//        {
//          LED3_OFF;
//          sampleTimes = 0;
//          LED2_OFF;
//        }
        static uint32_t unlock_times = 0;
        static uint32_t lock_times = 0;

        //     PRINTF("ThrottleValue = %6d ,YawValue = %6d \r\n" ,remoteControlValue[kThrottle],remoteControlValue[kYaw]);
        if(isRCunlock == false)
        {
          if((remoteControlValue[kThrottle] < RC_THRESHOLD_L) && (remoteControlValue[kYaw] > RC_THRESHOLD_H))
          {
            unlock_times++;
          }
          else
          {
            unlock_times = 0;
          }
          if(unlock_times > (waitTimes3s/2) )
          {
            isRCunlock = true;
          }
        }
        else
        {
          if((remoteControlValue[kThrottle] < RC_THRESHOLD_L) && (remoteControlValue[kYaw] < RC_THRESHOLD_L))
          {
            lock_times++;
          }
          else
          {
            lock_times = 0;
          }
          if(lock_times > (waitTimes3s/4))
          {
            isRCunlock = false;
          }
        }
      }
      /*End************Remote Controller Unlock *************/

      /*Start*********** Reflash the motor PWM **************/
      {
        static uint32_t motor_init_times = 0 ;
        motor_init_times++;
        //      uint32_t waitTimes3s = (3000 / (chn1Confg.periodUs / 1000) ) ;
        if(motor_init_times > waitTimes3s )
        {
          motor_init_times = (waitTimes3s+1) ;
          static uint32_t test_throttle_pwm = 50;
          test_throttle_pwm = remoteControlValue[kThrottle] / 2400 - 8; //对应42%-92%占空比
 //         test_throttle_pwm = remoteControlValue[kThrottle] / 3600+8;// 8; //对应40%-70%占空比
          ////遥控器信号 50Hz , 范围1~2ms，周期20ms，1.5ms中值.对应 120 000 - 240 000
          //      motor_pwm_reflash(test_throttle_pwm,
          //                        test_throttle_pwm,
          //                        test_throttle_pwm,
          //                        test_throttle_pwm);
          static imu_float_euler_angle_t expectAngel = {
            .imu_pitch = 0,
            .imu_roll = 0,
            .imu_yaw = 0,
          };
//
          ///
          /////////////////////////////////////////////////////
          ///////////////////////////////////////////////////////
          ////////////////////////////////////////////////////////
          /////////////////////////////////////////////////////////
#if 1
          X_Y_changeTmp[1] = (((double)((double)remoteControlValue[kPitch]/1000) -180)/3);
          X_Y_changeTmp[0] = (((double)((double)remoteControlValue[kRoll]/ 1000) -180)/3);
          expectAngel.imu_roll = (X_Y_changeTmp[0]) * cos(quadAngle.imu_yaw / 180 * PI) + (X_Y_changeTmp[1]) * sin(quadAngle.imu_yaw / 180 * PI);
          expectAngel.imu_pitch = (X_Y_changeTmp[1]) * cos(quadAngle.imu_yaw / 180 * PI) + (-1 * X_Y_changeTmp[0]) * sin(quadAngle.imu_yaw / 180 * PI);
          //expectAngel.imu_pitch *= -1;
#else
          expectAngel.imu_pitch = (((double)((double)remoteControlValue[kPitch]/1000) -180)/3);
          expectAngel.imu_roll = (((double)((double)remoteControlValue[kRoll]/ 1000) -180)/3);
#endif

#if 1
          if((sampleTimes > 400) && (isRCunlock == true))
          {

              checkTmpVal[0] = ((double)remoteControlValue[kYaw] / 1000 - 180) / 150;
              if(checkTmpVal[0] < 0.08 && checkTmpVal[0] > -0.08)
              {
                  checkTmpVal[0] = 0;
              }

              checkTmpVal[1] += checkTmpVal[0];
              if(checkTmpVal[1] >= 360 || checkTmpVal[1] <= -360)
              {
                  checkTmpVal[1] = 0;
              }
              expectAngel.imu_yaw = -1 * checkTmpVal[1];
          }
          else
          {
              checkTmpVal[0] = 0;
              checkTmpVal[1] = 0;
              expectAngel.imu_yaw = 0;
          }
          //sendLineX(0x5f,(float)checkTmpVal[1]);
          //sendLineX(0x4f,(float)checkTmpVal[1]);
#endif

#if 0
          {
          checkTmpVal[0] = (((double)((double)remoteControlValue[kPitch]/1000) -180)/600);
          checkTmpVal[1] = (((double)((double)remoteControlValue[kRoll]/ 1000) -180)/600);

          if(checkTmpVal[0] < 0.003 && checkTmpVal[0] > -0.003)
          {
            checkTmpVal[0] = 0;
          }
          if(checkTmpVal[1] < 0.003 && checkTmpVal[1] > -0.003)
          {
            checkTmpVal[1] = 0;
          }
          expectAngel.imu_pitch += checkTmpVal[0];
          expectAngel.imu_roll += checkTmpVal[1];

          if(expectAngel.imu_pitch > 15)
          {
            expectAngel.imu_pitch = 15;
          }
          else if(expectAngel.imu_pitch < -15)
          {
            expectAngel.imu_pitch = -15;
          }

          if(expectAngel.imu_roll > 15)
          {
            expectAngel.imu_roll = 15;
          }
          else if(expectAngel.imu_roll < -15)
          {
            expectAngel.imu_roll = -15;
          }
          }
#endif

          //expectAngel.imu_roll += (((double)((double)remoteControlValue[kRoll]/ 1000) -180)/600);// - checkTmpVal[1];

          //sendLineX(0x1f,(((float)expectAngel.imu_pitch)));
          //sendLineX(0x0f,(((float)checkTmpVal[0])));
          //sendLineX(0x0f,(((float)expectAngel.imu_pitch)));
          //sendLineX(0x4f,(((float)expectAngel.imu_roll)));
          //sendLineX(0x5f,(((float)checkTmpVal[1])));
          //sendLineX(0x6f,(((float)checkTmpVal[0])));

          static bool isAngleProtected = false;
          if( quadAngle.imu_pitch > PROTECTED_ANGLE || quadAngle.imu_pitch < ((-1)*PROTECTED_ANGLE)
             ||quadAngle.imu_roll > PROTECTED_ANGLE || quadAngle.imu_roll < ((-1)*PROTECTED_ANGLE) )
          {
            // isAngleProtected = true;
            isRCunlock = false;
          }

          motor_pid_control(test_throttle_pwm,
                            &expectAngel,
                            &quadAngle,
                            &pitch_pid00,
                            &pitch_pid11,
                            &yaw_pid00,
                            &yaw_pid11,
                            &roll_pid00,
                            &roll_pid11,
                            isRCunlock);



        }
      }
      /*End************* Reflash the motor PWM **************/
    }
  }
  /*****end while(1) in mian loop*****/
}

volatile bool isRCunlock = false;
//below define value is in quad_common.h
//#define RC_THRESHOLD_H (220000U)
//#define RC_THRESHOLD_L (140000U)
//#define RC_THRESHOLD_ERROR (300000U)//由于IO采两个边沿中断，有可能算成低电平的时间，所以做一个剔除算法。
//#define HW_DIVIDER (2400000U)
////120M core clock , 2400000 / 120 000 000 = 0.02 s , 50Hz ,
////遥控器信号 50Hz , 范围1~2ms，周期20ms，1.5ms中值.对应 120 000 - 240 000
void PORTB_IRQHandler(void)
{
  uint32_t intFlag = PORT_HAL_GetPortIntFlag(PORTB_BASE);
  uint32_t i =  0;
  uint32_t value = 0;
  static  uint32_t remoteControlValue1st[8] = {0};
  static  uint32_t remoteControlValue2nd[8] = {0};
  static  uint32_t remoteControlValueFlag[8] = {0};
  for(i=0 ; i<8;i++)
  {
    if (intFlag & (1 << remoteControlPinNum[i]))
    {
      remoteControlIntTimes[i] = 0;//进入中断，就把这个值清零
      if (remoteControlValueFlag[i] == 0)
      {
        remoteControlValue1st[i] = (SysTick->VAL);
        remoteControlValueFlag[i] = 1;
      }
      else
      {
        remoteControlValueFlag[i] = 0;
        remoteControlValue2nd[i] = (SysTick->VAL);
        if ( remoteControlValue1st[i] > remoteControlValue2nd[i] )
        {
          value = remoteControlValue1st[i] - remoteControlValue2nd[i];
        }
        else
        {
          value = remoteControlValue1st[i] + HW_DIVIDER - remoteControlValue2nd[i];//hwtimer.divider
        }
        //防止不小心从下降沿开始采样，算成低电平的占空比。
        if( value > RC_THRESHOLD_ERROR)
        {
          remoteControlValueFlag[i] = 1;
          remoteControlValue1st[i] = (SysTick->VAL);
        }
        else
        {
          remoteControlValue[i] = value;
//          if(((remoteControlValue[3] <180000) ||(remoteControlValue[3] > 190000))&&remoteControlValue[3]> 100)
//            LED4_ON;
        }
      }
    }
    PORT_HAL_ClearPinIntFlag(PORTB_BASE,remoteControlPinNum[i]);
  }
  /* Clear interrupt flag.*/
 //   PORT_HAL_ClearPortIntFlag(PORTB_BASE);
}

void PORTE_IRQHandler(void)
{
  uint32_t intFlag = PORT_HAL_GetPortIntFlag(PORTE_BASE);
  if (intFlag & (1 << 11))
  {
    isFXOS8700Int1Trigger = true;
      PRINTF("\r\n PTE11 irq");
  }

  /* Clear interrupt flag.*/
  PORT_HAL_ClearPortIntFlag(PORTE_BASE);
}

void PIT0_IRQHandler(void)
{
  /* Clear interrupt flag.*/
  PIT_HAL_ClearIntFlag(PIT_BASE, 0U);

  //I2C_getAccelMangData(&memsRawDate);
  //I2C_getGyroData(&memsRawDate);

//  if(gyro_offset_done == true)
//  {
//    imu_get_euler_angle(&quadAngle,&memsRawDate);
//  }


 get_AttitudeVal(My_gyro);
  static uint16_t led2_i =0;
  if(led2_i==0)
  {
    LED2_ON;led2_i=1;
  }
  else
  {
    LED2_OFF;led2_i=0;
  }
}

bool pitIsrFlag1 = false;
void PIT1_IRQHandler(void)
{
  /* Clear interrupt flag.*/
  PIT_HAL_ClearIntFlag(PIT_BASE, 1U);
  static uint16_t led4_i =0;
  if(led4_i==0)
  {
    LED4_ON;led4_i=1;
  }
  else
  {
    LED4_OFF;led4_i=0;
  }
//  uint32_t i = 0;
//  for(i=0;i<8;i++)
//  {
//    remoteControlIntTimes[i] += 1;
//    if(remoteControlIntTimes[i] > 3) //20ms，4个周期都没有中断，说明没有遥控器信号，清零。
//                                     //貌似 遥控器 掉电了 也继续有信号发出，得示波器量一下
//    {
//      remoteControlIntTimes[i] = 10;
//      remoteControlValue[i] = 0;
//    }
//  }
  //    pitIsrFlag[1] = true;
  pitIsrFlag1 = true;
}