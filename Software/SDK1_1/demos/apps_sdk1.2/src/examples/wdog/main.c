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
// Includes
///////////////////////////////////////////////////////////////////////////////
// Standard C Included Files
#include <stdio.h>
// Standard C Included Files
#include "board.h"
#include "fsl_wdog_driver.h"
#include "fsl_os_abstraction.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#define BLINK_TIME         400   // The blink time is 400ms

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////
/*!
 * @brief Check if button is pressed.
 *
 * This function gets the state of button.
 *
 * @return 0 if botton is not pressed.
 *         1 if botton is pressed
 */
static uint32_t is_key_pressed(void)
{
    return (!GPIO_DRV_ReadPinInput(kGpioSW1));
}

/*!
 * @brief Watchdog main routine
 * Run a simple application which enables watchdog, then
 * continuously refreshes the watchdog to prevent CPU reset
 * Upon SW1 button push, the watchdog will expire after
 * approximately 2 seconds and issue reset
 */
void main(void)
{
    // Configure watchdog.
    const wdog_user_config_t wdogConfig =
    {
        .timeoutValue          = 2048U,// Watchdog overflow time is about 2s
        .windowValue           = 0,    // Watchdog window value, 0-disable window function
        .clockPrescalerValue   = kWdogClockPrescalerValueDevide1, // Watchdog clock prescaler
        .updateRegisterEnable  = true, // Update register enabled
        .clockSource           = kClockWdogSrcLpoClk, // Watchdog clock source is LPO 1KHz
        .workInWaitModeEnable  = true, // Enable watchdog in wait mode
        .workInStopModeEnable  = true, // Enable watchdog in stop mode
        .workInDebugModeEnable = false,// Disable watchdog in debug mode
    };

    // Init hardware.
    hardware_init();

    // Init OSA layer.
    OSA_Init();

    // Call this function to initialize the console UART.  This function
    // enables the use of STDIO functions (printf, scanf, etc.)
    dbg_uart_init();
    // Init pinsfor switch and led.

    GPIO_DRV_Init(switchPins, ledPins);
    // Initialize wdog before the WDOG timer has a chance
    //to reset the device

    // Turn LED1 on;
    LED1_ON;

    WDOG_DRV_Init(&wdogConfig);

    // If not wdog reset, clear reset count
    if (!(RCM->SRS0 & RCM_SRS0_WDOG_MASK))
    {
        WDOG_DRV_ClearResetCount();
        printf("\r\n WDOG example \r\n");
    }

    // Check if WDOG reset occurred , disable WDOG and turn off LED1.
    if (WDOG_DRV_GetResetCount())
    {
        printf("\r\n WDOG reset count %ld",WDOG_DRV_GetResetCount());
    }

    printf("\r\n Press SW1 to expire watchdog ");

    // Continue to run in loop to refresh watchdog until SW1 is pushed
    while (1)
    {
        // Check for SW1 button push.Pin is grounded when button is pushed.
        if (0 != is_key_pressed())
        {
            while (1)
            {
                // Button has been pushed,blink LED
                // showing that the watchdog is about to expire.
                LED1_TOGGLE;
                OSA_TimeDelay(BLINK_TIME);
            }
        }

        // Restart the watchdog so it doesn't reset.
        WDOG_DRV_Refresh();
        OSA_TimeDelay(100u);
    }

}

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////

