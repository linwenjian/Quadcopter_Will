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

#define HWTIMER_PERIOD          100000

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////

extern const hwtimer_devif_t kSystickDevif;
extern const hwtimer_devif_t kPitDevif;
hwtimer_t hwtimer;

/*!
 * @brief Main function
 */

void hwtimer_callback(void* data)
{static int i=0;
    PRINTF(".");
    I2C_acceInterrupt();
    if(i==0) {LED2_ON;i=1;}
    else    {LED2_OFF;i=0;}
}

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

    // Print the initial banner
    PRINTF("\r\nHello World!\n\n\r");

    LED2_EN;    LED3_EN;    LED4_EN;    LED5_EN;
    LED2_OFF;   LED3_OFF;   LED4_OFF;   LED5_OFF;


    I2C_acceInit();

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
