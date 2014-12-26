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
#include "fsl_rtc_driver.h"
#include "fsl_os_abstraction.h"
///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#define RTC_INSTANCE      BOARD_RTC_FUNC_INSTANCE

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
volatile bool busyWait = true;

/*!
* @brief ISR for Alarm interrupt
*
* This function change state of busyWait.
*/
void RTC_IRQHandler(void)
{
    busyWait = false;
}
/*!
* @brief RTC in alarm mode.
*
* This function demostrates how to use RTC as an alarm clock.
*/
void main(void)
{
    uint32_t sec;
    uint32_t currSeconds;
    rtc_datetime_t date;

    // Init hardware.
    hardware_init();

    // Call this function to initialize the console UART.  This function
    // enables the use of STDIO functions (printf, scanf, etc.)
    dbg_uart_init();

    printf("RTC example: set up time to wake up an alarm\r\n");

    // Init RTC
    RTC_DRV_Init(RTC_INSTANCE);
    // Set a start date time and start RT.
    date.year   = 2014U;
    date.month  = 12U;
    date.day    = 25U;
    date.hour   = 19U;
    date.minute = 0;
    date.second = 0;

    // Set RTC time to default
    RTC_DRV_SetDatetime(RTC_INSTANCE, &date);

    while (1)
    {
        // Get date time.
        RTC_DRV_GetDatetime(RTC_INSTANCE, &date);

        // print default time
        printf("Current datetime: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n",
                        date.year, date.month, date.day,
                        date.hour, date.minute, date.second);

        // Convert current date time to seconds
        RTC_HAL_ConvertDatetimeToSecs(&date, &currSeconds);

        // Get alarm time from user
        sec = 0;
        printf("Please input the number of second to wait for alarm \r\n");
        printf("The second must be positive value\r\n");
        while (sec < 1)
        {
            scanf("%d",&sec);
        }

        // Add curr_seconds
        sec += currSeconds;
        // Convert sec to date type
        RTC_HAL_ConvertSecsToDatetime(&sec, &date);

        // Set alarm time
        if(!RTC_DRV_SetAlarm(RTC_INSTANCE, &date, true))
        {
            printf("Failed to set alarm. \r\n");
            continue;
        }

        // Print alarm time
        printf("Alarm will be occured at: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n",
                        date.year, date.month, date.day,
                        date.hour, date.minute, date.second);

        // Wait until alarm occures
        while(busyWait)
        {}

        printf("\r\n Alarm occured !!!! ");
    }

}

/*******************************************************************************
 * EOF
 *******************************************************************************/
