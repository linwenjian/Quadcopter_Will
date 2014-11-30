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

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define HWTIMER_LL_DEVIF    kSystickDevif
#define HWTIMER_LL_SRCCLK   kCoreClock
#define HWTIMER_LL_ID       0

#define HWTIMER_PERIOD          20000

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




/*!
 * @brief Main function
 */

int temp_duty = 1;
int temp_count = 0;
int temp_flag = 0;
int temp_count1 = 0;
int temp_count_test = 0;
int temp_count_add = 1;



// 500Hz PWM£¬ start from 50% duty cylce.
//%0 duty cycle for 2 seconds , then 50% duty cycle for 2 seconds.
void hwtimer_callback(void* data)
   {
     static int i=0;
     PRINTF(".");
     I2C_acceInterrupt();
     I2C_gyroInterrupt();
    
     if(temp_count <= 100 && temp_flag ==0 )
     {
       temp_count++;
     }
     
     if(temp_count > 100 && temp_count < 200 && temp_flag ==0 )
     {
       // temp_count = 0; 
       temp_count++;
       ftmParam0.uDutyCyclePercent = 50;
       ftmParam1.uDutyCyclePercent = 50;
       ftmParam2.uDutyCyclePercent = 50;
       ftmParam3.uDutyCyclePercent = 50;
       
     }
     
     if(temp_count >= 200 && temp_flag ==0 )
     {
       // temp_count = 0;
     //  ftmParam0.uDutyCyclePercent = 55;
       temp_flag = 1;
      // ftmParam0.uDutyCyclePercent = 60;
      }
     
     if( temp_flag == 1 )
     {
       //             ftmParam0.uDutyCyclePercent ++;
       //             
       //             if (ftmParam0.uDutyCyclePercent>58)
       //                            ftmParam0.uDutyCyclePercent = 59;
       
       temp_count_test++;
       
       if( temp_count_test > 10 )
       {
         temp_count_test = 0;
         
         
         if (temp_count_add == 1)
         {
           ftmParam0.uDutyCyclePercent++;
           ftmParam1.uDutyCyclePercent++;
           ftmParam2.uDutyCyclePercent++;
           ftmParam3.uDutyCyclePercent++;
         }
         else
         {
           ftmParam0.uDutyCyclePercent--;
           ftmParam1.uDutyCyclePercent--;
           ftmParam2.uDutyCyclePercent--;
           ftmParam3.uDutyCyclePercent--;
         }
         
         
         if( ftmParam0.uDutyCyclePercent  > 70 )
           
         { 
           temp_count_add = 0;
         }
         if( ftmParam0.uDutyCyclePercent  < 52)
           
         { temp_count_add = 1;
         }
       }   
     }
     
     
     
     
//          if( temp_flag == 1 && temp_count1 < 200)
//     {
//       //             ftmParam0.uDutyCyclePercent ++;
//       //             
//       //             if (ftmParam0.uDutyCyclePercent>58)
//       //                            ftmParam0.uDutyCyclePercent = 59;
//       temp_count1++;
//       ftmParam0.uDutyCyclePercent = 70;
//       
//     }
//     else if( temp_flag == 1 && temp_count1 < 400)
//     {
//       temp_count1++;
//       ftmParam0.uDutyCyclePercent = 50;
//     }
//     else if( temp_flag == 1 && temp_count1 < 600)
//     {
//       temp_count1++;
//       ftmParam0.uDutyCyclePercent = 54;
//     }
//     else
//     { temp_count1 = 1000;}
//     
     
       
    
     
//     ftmParam0.uDutyCyclePercent += temp_duty;
//     ftmP
//     ftmParam2.uDutyCyclePercent += temp_duty;
//     ftmParam3.uDutyCyclePercent += temp_duty;
//
//     if (ftmParam0.uDutyCyclePercent >= 70)
//     {
//       temp_duty = -2;
//     }
//     if (ftmParam0.uDutyCyclePercent <= 52)
//     {
//       temp_duty = 2;
//     }
//     

//     if (ftmParam0.uDutyCyclePercent >= 70)
//     {
//       ftmParam0.uDutyCyclePercent = 5;
//     }
//     if (ftmParam1.uDutyCyclePercent >= 70)
//     {
//       ftmParam1.uDutyCyclePercent = 5;
//     }
//     if (ftmParam2.uDutyCyclePercent >= 70)
//     {
//       ftmParam2.uDutyCyclePercent = 5;
//     }
//     if (ftmParam3.uDutyCyclePercent >= 70)
//     {
//       ftmParam3.uDutyCyclePercent = 5;
//     }

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
    // RX buffers
    //! @param receiveBuff Buffer used to hold received data
    uint8_t receiveBuff;


    // Initialize standard SDK demo application pins
    hardware_init();
    OSA_Init();

    // Call this function to initialize the console UART. This function
    // enables the use of STDIO functions (printf, scanf, etc.)
    dbg_uart_init();

    memset(&ftmInfo, 0, sizeof(ftmInfo));

    ftmInfo.syncMethod = kFtmUseSoftwareTrig;
    configure_ftm_pins();

    FTM_DRV_Init(0, &ftmInfo);

    // Print the initial banner
    PRINTF("\r\nHello World!\n\n\r");

    LED2_EN;    LED3_EN;    LED4_EN;    LED5_EN;
    LED2_OFF;   LED3_OFF;   LED4_OFF;   LED5_OFF;

    PORT_HAL_SetMuxMode(PORTA_BASE, 6, kPortMuxAsGpio);
    GPIO_DRV_SetPinDir(GPIO_MAKE_PIN(HW_GPIOA, 6U), kGpioDigitalInput) ;

    PORT_HAL_SetMuxMode(PORTA_BASE, 7, kPortMuxAsGpio);
    GPIO_DRV_SetPinDir(GPIO_MAKE_PIN(HW_GPIOA, 7U), kGpioDigitalInput) ;

    I2C_acceInit();
    I2C_gyroInit();

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

    FTM_DRV_PwmStart(0, &ftmParam0, 0);
    FTM_DRV_PwmStart(0, &ftmParam1, 1);
    FTM_DRV_PwmStart(0, &ftmParam2, 2);
    FTM_DRV_PwmStart(0, &ftmParam3, 3);
    FTM_HAL_SetSoftwareTriggerCmd(g_ftmBaseAddr[0], true);

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
