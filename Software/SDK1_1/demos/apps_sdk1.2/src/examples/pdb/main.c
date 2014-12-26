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
// SDK Included Files
#include "board.h"
#include "fsl_os_abstraction.h"
#include "fsl_pdb_driver.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#define PDB_INSTANCE   (0)

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
volatile uint32_t pdbIntCounter = 0;

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////
/*!
 * @brief PDB normal timer.
 *
 * This function uses PDB to generate a constant period of time.
 * Each time, PDB expires, interrupt occurres.
 */
int main(void)
{
    pdb_user_config_t pdbUserConfigStruct;

    uint32_t currentCounter = 0;

    // Init hardware
    hardware_init();

    // Call this function to initialize the console UART.  This function
    // enables the use of STDIO functions (printf, scanf, etc.)
    dbg_uart_init();

    printf("\r\nPDB example \r\n");

    // Prepare the configuration structure.
    PDB_DRV_StructInitUserConfigForSoftTrigger(&pdbUserConfigStruct);

    pdbUserConfigStruct.continuousModeEnable = true;
    // Interrupt occures when counter reaches this value.
    // Users change this value to change the timer for PDB.
    pdbUserConfigStruct.delayValue           = 0x0FFFU;
    // Counter reset when it reaches this value.
    pdbUserConfigStruct.pdbModulusValue      = 0x0FFFU;
    while(1)
    {
        // Clear counter
        currentCounter = 0;
        pdbIntCounter  = 0;

        // Initialize PDB instance.
        PDB_DRV_Init(PDB_INSTANCE, &pdbUserConfigStruct);

        // Trigger the PDB.
        PDB_DRV_SoftTriggerCmd(PDB_INSTANCE);

        while (pdbIntCounter <= 10)
        {
            // Check if interrupt occurred.
            if (pdbIntCounter > currentCounter)
            {
                currentCounter = pdbIntCounter;
                // PDB counter go 10 time before stop.
                printf("PDB ISR No.%d occured !\r\n", pdbIntCounter);
            }
        }

        // Deinitilize PDB instance
        PDB_DRV_Deinit(PDB_INSTANCE);

        printf("PDB example finished \r\n");

        // Print a note.
        printf(" Press any key to run example again \r\n");
        getchar();
    }

}

/*******************************************************************************
 * EOF
 ******************************************************************************/

