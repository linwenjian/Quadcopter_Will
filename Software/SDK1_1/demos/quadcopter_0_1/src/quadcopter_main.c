/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////


#include "quad_common.h"
#include "quad_i2c_config.h"
#include "fsl_hwtimer.h"

#include "core_cm4.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define HWTIMER_LL_DEVIF    kSystickDevif
#define HWTIMER_LL_SRCCLK   kCoreClock
#define HWTIMER_LL_ID       0

#define HWTIMER_PERIOD          4000 //us  =4ms ,
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
  .uFrequencyHZ           = 500,
  .uDutyCyclePercent      = 0,
  .uFirstEdgeDelayPercent = 0,
};
ftm_pwm_param_t ftmParam1 = {
  .mode                   = kFtmEdgeAlignedPWM,
  .edgeMode               = kFtmHighTrue,
  .uFrequencyHZ           = 500,
  .uDutyCyclePercent      = 50,
  .uFirstEdgeDelayPercent = 0,
};
ftm_pwm_param_t ftmParam2 = {
  .mode                   = kFtmEdgeAlignedPWM,
  .edgeMode               = kFtmHighTrue,
  .uFrequencyHZ           = 500,
  .uDutyCyclePercent      = 50,
  .uFirstEdgeDelayPercent = 0,
};
ftm_pwm_param_t ftmParam3 = {
  .mode                   = kFtmEdgeAlignedPWM,
  .edgeMode               = kFtmHighTrue,
  .uFrequencyHZ           = 500,
  .uDutyCyclePercent      = 50,
  .uFirstEdgeDelayPercent = 0,
};


const char trans_header_table[3] = {0x88, 0xAF, 0x1C};
//char trans_header_table1[] = {0x88, 0xA1, 2,0,0,0};
trans_packet_t packet_upper_PC ;

mems_data_t memsRawDate ;
imu_float_euler_angle_t quadAngle;
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
volatile uint32_t remoteControlValue1st[8] = {0};
volatile uint32_t remoteControlValue2nd[8] = {0};
volatile uint32_t remoteControlValueFlag[8] = {0};
uint32_t remoteControlPinNum[] = {2,3,4,5,6,7,10,11};

uint32_t uDutyCycle_add = 0;
// 500Hz PWM， start from 50% duty cylce.
//%0 duty cycle for 2 seconds , then 50% duty cycle for 2 seconds.
void hwtimer_callback(void* data)
   {

////     GPIO_DRV_WritePinOutput(GPIO_MAKE_PIN(HW_GPIOD, 0U),0);
////this for cycle is about 57.1us from the logic analyzer
//// 0~ 4095  <--> 0V ~ 3.3 V
//     for (ADC_count_n = 0; ADC_count_n < 8U; ADC_count_n++)
//     {
//       MyChnConfig.chnNum = kADCPinMuxTable[ADC_count_n].chnNum;
//
//       MyChnConfig.diffEnable= false;
//       MyChnConfig.intEnable = true;
//
//       if(ADC_count_n == 0 )
//       {
//#if FSL_FEATURE_ADC16_HAS_MUX_SELECT
//         MyChnConfig.chnMux = kAdcChnMuxOfB;
//#endif /* FSL_FEATURE_ADC16_HAS_MUX_SELECT */
//       }
//       else
//       {
//#if FSL_FEATURE_ADC16_HAS_MUX_SELECT
//         MyChnConfig.chnMux = kAdcChnMuxOfDefault;
//#endif /* FSL_FEATURE_ADC16_HAS_MUX_SELECT */
//       }
//
//       ADC16_DRV_ConfigConvChn(kADCPinMuxTable[ADC_count_n].instance,
//                               kADCPinMuxTable[ADC_count_n].chnGroup,
//                               &MyChnConfig);
//       /* Wait the interrupt for conversion completed. */
//       while (!g_AdcConvIntCompleted) {}
//       g_AdcConvIntCompleted = false;
////       PRINTF("ADC_count_%d = %d \r\n",ADC_count_n, ADC_value[ADC_count_n]);
//
//       if ( ADC_value_min[ADC_count_n] > ADC_value [ADC_count_n])
//       {
//         ADC_value_min[ADC_count_n] = ADC_value [ADC_count_n];
//       }
//
//       if ( ADC_value_max[ADC_count_n] < ADC_value [ADC_count_n])
//       {
//         ADC_value_max[ADC_count_n] = ADC_value [ADC_count_n];
//       }
//     }
//     PRINTF("\r\n");
//     GPIO_DRV_WritePinOutput(GPIO_MAKE_PIN(HW_GPIOD, 0U),1);

static int i=0;
//     int j=0;
//     //PRINTF(".");
     I2C_getAccelMangData(&memsRawDate);
     I2C_getGyroData(&memsRawDate);
     
     imu_get_euler_angle(&quadAngle,&memsRawDate);

/*Start*********匿名上位机发送的串口数据***********/
     packet_upper_PC.user_data.trans_accel[0] = BSWAP_16(memsRawDate.accel_x);
     packet_upper_PC.user_data.trans_accel[1] = BSWAP_16(memsRawDate.accel_y);
     packet_upper_PC.user_data.trans_accel[2] = BSWAP_16(memsRawDate.accel_z);
     packet_upper_PC.user_data.trans_gyro[0]  = BSWAP_16(memsRawDate.gyro_x);
     packet_upper_PC.user_data.trans_gyro[1]  = BSWAP_16(memsRawDate.gyro_y);
     packet_upper_PC.user_data.trans_gyro[2]  = BSWAP_16(memsRawDate.gyro_z);
     packet_upper_PC.user_data.trans_mag[0]  = BSWAP_16(memsRawDate.magn_x);
     packet_upper_PC.user_data.trans_mag[1]  = BSWAP_16(memsRawDate.magn_y);
     packet_upper_PC.user_data.trans_mag[2]  = BSWAP_16(memsRawDate.magn_z);     

     packet_upper_PC.user_data.trans_roll = BSWAP_16((int16_t)(quadAngle.imu_roll*100));
     packet_upper_PC.user_data.trans_pitch = BSWAP_16((int16_t)(quadAngle.imu_pitch*100));
     packet_upper_PC.user_data.trans_yaw = BSWAP_16((int16_t)(quadAngle.imu_yaw*10));

     uint8_t *p = (uint8_t*)&packet_upper_PC;

     UART_HAL_SendDataPolling(BOARD_DEBUG_UART_BASEADDR,p,32);
/*End*********匿名上位机发送的串口数据***********/


//Do not change the first 100 cylces PWM after Powen On
//it would be better that the dutyCycle = 0%
     if(temp_count <= 100 && temp_flag ==0 )
     {
       temp_count++;
     }

//All PWM duty cycle change to 50% for the next 100 cycle.
//50% dutyCycle for the Electronic Speed Controller , the BLDC would not turning
     if(temp_count > 100 && temp_count < 200 && temp_flag ==0 )
     {
       // temp_count = 0;
       temp_count++;
       ftmParam0.uDutyCyclePercent = 50;
       ftmParam1.uDutyCyclePercent = 50;
       ftmParam2.uDutyCyclePercent = 50;
       ftmParam3.uDutyCyclePercent = 50;
     }
//Then, the BLDC can start to turning.
     if(temp_count >= 200 && temp_flag ==0 )
     {
       temp_flag = 1;
      }
//测试代码，电机从 52-->70-->52 占空比变化
     if( temp_flag == 1 )
     {

       if(remoteControlValue[0] > 120000)
       {
         uDutyCycle_add = (uint32_t)((remoteControlValue[0] - 120000)/2400) ;
       }
       if(uDutyCycle_add < 50)
       {
         ftmParam0.uDutyCyclePercent = 46 + uDutyCycle_add;
       }

//
//       temp_count_test++;
//       if( temp_count_test > 10 )
//       {
//         temp_count_test = 0;
//         if (temp_count_add == 1)
//         {
//           ftmParam0.uDutyCyclePercent++;
//           ftmParam1.uDutyCyclePercent++;
//           ftmParam2.uDutyCyclePercent++;
//           ftmParam3.uDutyCyclePercent++;
//         }
//         else
//         {
//           ftmParam0.uDutyCyclePercent--;
//           ftmParam1.uDutyCyclePercent--;
//           ftmParam2.uDutyCyclePercent--;
//           ftmParam3.uDutyCyclePercent--;
//         }
//         if( ftmParam0.uDutyCyclePercent  > 70 )
//         {
//           temp_count_add = 0;
//         }
//         if( ftmParam0.uDutyCyclePercent  < 52)
//         {
//           temp_count_add = 1;
//         }
//       }
     }

/*Start 简化版更改占空比***************/
//     uint16_t uMod, uCnv0, uCnv1,uCnv2,uCnv3;
//     uint32_t ftmBaseAddr = g_ftmBaseAddr[0];
//
//     uMod = FTM_HAL_GetMod(ftmBaseAddr);
//
//     uCnv0 = uMod * ftmParam0.uDutyCyclePercent / 100;
//     /* For 100% duty cycle */
//     if(uCnv >= uMod)
//     {
//       uCnv = uMod + 1;
//     }
//     uCnv1 = uMod * ftmParam1.uDutyCyclePercent / 100;
//     uCnv2 = uMod * ftmParam2.uDutyCyclePercent / 100;
//     uCnv3 = uMod * ftmParam3.uDutyCyclePercent / 100;
/*End 简化版更改占空比*************/

     FTM_DRV_PwmChangeDutyCycle(0, &ftmParam0, 0);
     FTM_DRV_PwmChangeDutyCycle(0, &ftmParam1, 1);
     FTM_DRV_PwmChangeDutyCycle(0, &ftmParam2, 2);
     FTM_DRV_PwmChangeDutyCycle(0, &ftmParam3, 3);

     FTM_HAL_SetSoftwareTriggerCmd(g_ftmBaseAddr[0], true);

     if(i==0)
     {
       LED2_ON;i=1;
     }
     else
     {
       LED2_OFF;i=0;
     }
   }


//PTD2_UART_rx, PTD3_UART_tx
//PTC1,2,3,4
int main (void)
{
    memcpy(packet_upper_PC.trans_header, trans_header_table, sizeof(trans_header_table));
    // RX buffers
    //! @param receiveBuff Buffer used to hold received data
    uint8_t receiveBuff;

    // Initialize standard SDK demo application pins
    hardware_init();
    OSA_Init();

    // Call this function to initialize the console UART. This function
    // enables the use of STDIO functions (printf, scanf, etc.)
    dbg_uart_init();

/*Start***FTM Init*************************************************************/
    memset(&ftmInfo, 0, sizeof(ftmInfo));
    ftmInfo.syncMethod = kFtmUseSoftwareTrig;
    FTM_DRV_Init(0, &ftmInfo);
/*End*****FTM Init*************************************************************/

///*Start***ADC Init*************************************************************/
//
//#if FSL_FEATURE_ADC16_HAS_CALIBRATION
//    adc16_calibration_param_t MyAdcCalibraitionParam0;
//    adc16_calibration_param_t MyAdcCalibraitionParam1;
//#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
//    adc16_user_config_t MyAdcUserConfig0;
////    adc16_chn_config_t MyChnConfig0;
//
//    adc16_user_config_t MyAdcUserConfig1;
////    adc16_chn_config_t MyChnConfig1;
//
//#if FSL_FEATURE_ADC16_HAS_CALIBRATION
//    /* Auto calibraion for ADC0 */
//    ADC16_DRV_GetAutoCalibrationParam(0u, &MyAdcCalibraitionParam0);
//    ADC16_DRV_SetCalibrationParam(0u, &MyAdcCalibraitionParam0);
//    /* Auto calibraion for ADC1 */
//    ADC16_DRV_GetAutoCalibrationParam(1u, &MyAdcCalibraitionParam1);
//    ADC16_DRV_SetCalibrationParam(1u, &MyAdcCalibraitionParam1);
//#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
//
//    /* Initialize the ADC0 converter. */
//    ADC16_DRV_StructInitUserConfigDefault(&MyAdcUserConfig0);
//    /* Initialize the ADC1 converter. */
//    ADC16_DRV_StructInitUserConfigDefault(&MyAdcUserConfig1);
//
//    MyAdcUserConfig0.intEnable = true;
//    MyAdcUserConfig1.intEnable = true;
//    ADC16_DRV_Init(0u, &MyAdcUserConfig0);
//    ADC16_DRV_Init(1u, &MyAdcUserConfig1);
//
//    /* Configuration for ADC channel. */
//    MyChnConfig.chnNum = kADCPinMuxTable[ADC_count_n].chnNum;
//    MyChnConfig.diffEnable= false;
//    MyChnConfig.intEnable = true;
//#if FSL_FEATURE_ADC16_HAS_MUX_SELECT
//    MyChnConfig.chnMux = kAdcChnMuxOfA;
//#endif /* FSL_FEATURE_ADC16_HAS_MUX_SELECT */
//
////void isr_20ms_vector(void)
////{
////    MyChnConfig.chnNum = kADCPinMuxTable[ADC_count_n].chnNum;
////    ADC_count_n++;
////    if(8 == ADC_count_n++)
////    {
////      ADC_count_n = 0;
////    }
////    MyChnConfig.diffEnable= false;
////    MyChnConfig.intEnable = true;
////    ADC16_DRV_ConfigConvChn(kADCPinMuxTable[ADC_count_n].instance,
////                            kADCPinMuxTable[ADC_count_n].chnGroup,
////                            &MyChnConfig);
////}
////
////void isr_adc0(void)
////{
////    value(n) = ADC_DRV_GetConvValueRAW(0U);
////}
////
////void isr_adc0(void)
////{
////    value(n) = ADC_DRV_GetConvValueRAW(0U);
////}
//
//
///*End*****ADC Init*************************************************************/

    // Print the initial banner
    PRINTF("\r\nHello World!\n\n\r");

    LED2_EN;    LED3_EN;    LED4_EN;    LED5_EN;
    LED2_OFF;   LED3_OFF;   LED4_OFF;   LED5_OFF;

    PORT_HAL_SetMuxMode(PORTA_BASE, 6, kPortMuxAsGpio);
    GPIO_DRV_SetPinDir(GPIO_MAKE_PIN(HW_GPIOA, 6U), kGpioDigitalInput) ;

    PORT_HAL_SetMuxMode(PORTA_BASE, 7, kPortMuxAsGpio);
    GPIO_DRV_SetPinDir(GPIO_MAKE_PIN(HW_GPIOA, 7U), kGpioDigitalInput) ;


    PORT_HAL_SetMuxMode(PORTD_BASE, 0, kPortMuxAsGpio);
    GPIO_DRV_SetPinDir(GPIO_MAKE_PIN(HW_GPIOD, 0U), kGpioDigitalOutput) ;

    GPIO_DRV_WritePinOutput(GPIO_MAKE_PIN(HW_GPIOD, 0U),1);

    PORT_HAL_SetMuxMode(PORTD_BASE,1u,kPortPinDisabled);


    I2C_fxos8700Init();
    I2C_l3g4200dInit();

    FTM_DRV_PwmStart(0, &ftmParam0, 0);
    FTM_DRV_PwmStart(0, &ftmParam1, 1);
    FTM_DRV_PwmStart(0, &ftmParam2, 2);
    FTM_DRV_PwmStart(0, &ftmParam3, 3);
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
    if (kHwtimerSuccess != HWTIMER_SYS_RegisterCallback(&hwtimer, hwtimer_callback, NULL))
    {
        PRINTF("\r\nError: hwtimer callback registration.\r\n");
    }
    if (kHwtimerSuccess != HWTIMER_SYS_Start(&hwtimer))
    {
        PRINTF("\r\nError: hwtimer start.\r\n");
    }

    GPIO_DRV_Init(remoteControlPins,NULL);

    while(1)
    {
//      LED2_ON;
//      OSA_TimeDelay(200);
      LED3_ON;
      OSA_TimeDelay(200);
      LED4_ON;
      OSA_TimeDelay(200);
      LED5_ON;
      OSA_TimeDelay(200);

//      LED2_OFF;
//      OSA_TimeDelay(200);
      LED3_OFF;
      OSA_TimeDelay(200);
      LED4_OFF;
      OSA_TimeDelay(200);
      LED5_OFF;
      OSA_TimeDelay(200);
    }
}

#define HW_DIVIDER 2400000
void PORTB_IRQHandler(void)
{
  uint32_t intFlag = PORT_HAL_GetPortIntFlag(PORTB_BASE);
  uint32_t i =  0;
  for(i=0 ; i<8;i++)
  {
    if (intFlag & (1 << remoteControlPinNum[i]))
    {
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
          remoteControlValue[i] = remoteControlValue1st[i] - remoteControlValue2nd[i];
        }
        else
        {
          remoteControlValue[i] = remoteControlValue1st[i] + HW_DIVIDER - remoteControlValue2nd[i];//hwtimer.divider
        }
      }
    }
  }
  /* Clear interrupt flag.*/
    PORT_HAL_ClearPortIntFlag(PORTB_BASE);
}