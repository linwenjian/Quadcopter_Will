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

// Standard C Included Files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// SDK Included Files
#include "fsl_crc_driver.h"
#include "board.h"
///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#define CRC_INSTANCE         (0)
#define BUFF_SIZE            (100u)

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////
/*!
 * @brief Calculates checksum number of an array, gererated randomly.
 */
int main(void)
{
    uint32_t i,crcValue;
    uint8_t buffer[BUFF_SIZE];
    // Configure CRC.
    crc_user_config_t userConfigPtr;

    // Init hardware
    hardware_init();

    // Call this function to initialize the console UART.  This function
    // enables the use of STDIO functions (printf, scanf, etc.)
    dbg_uart_init();

    // Initialize randomization.
    srand (time(NULL));
    // Init CRC-16-CCITT
    // seed disable
    // polynomial 1000000100001: x^12+x^5+1
    userConfigPtr.crcWidth       = kCrc16Bits;
    userConfigPtr.seed           = 0x0000U;
    userConfigPtr.polynomial     = 0x1021U;
    userConfigPtr.writeTranspose = kCrcTransposeBits;
    userConfigPtr.readTranspose  = kCrcTransposeBits;
    userConfigPtr.complementRead = false;

    // Init CRC.
    CRC_DRV_Init(CRC_INSTANCE, &userConfigPtr);

    printf("\r\nCRC EXAMPLE \r\n");
    while(1)
    {

        printf("Generate a random buffer: \r\n");
        // Randomize buffer.
        for(i = 0; i < BUFF_SIZE; i++)
        {
            buffer[i] = (uint8_t)rand();
            // print buffer.
            if (0 == i & 0x0Fu)
            {
                printf("\r\n");
            }
            printf("%5d ", buffer[i]);
        }

        // Calculates CRC value.
        crcValue = CRC_DRV_GetCrcBlock(CRC_INSTANCE, buffer, BUFF_SIZE);

        // print calculated checksum number.
        printf("\r\n Checksum value of buffer array 0x%0X",crcValue);

        // Print a note.
        printf(" print any key to continue \r\n");
        getchar();
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

