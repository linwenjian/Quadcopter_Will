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

#include <stdio.h>

#include "fsl_uart_hal.h"
#include "fsl_sim_hal.h"
#include "low_power_demo.h"

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
* Code
******************************************************************************/

/*!
* @brief cpu identification function for KV10Z75M
*/
void cpu_identify(void)
{
    /* Determine the Kinetis family */
    switch(SIM_HAL_GetFamId(SIM_BASE)) 
    {  
    case 0x1:
        printf("KV10Z\r\n");
        break;
    default:
        printf("\r\nUnrecognized Kinetis family device.\r\n");  
        break;  	
    }
    /* Determine the package size */
    switch(SIM_HAL_GetPinCntId(SIM_BASE))
    {  
    case 0x2:
        printf("32pin\r\n");
        break;
    case 0x3:
        printf("44pin\r\n");
        break;
    case 0x4:
        printf("48pin\r\n");
        break;
    default:
        printf("\r\nUnrecognized Kinetis package code.\r\n");  
        break;  	
    }                
    /* Determine the P-flash size */
    switch(SIM_HAL_GetProgramFlashSize(SIM_BASE))
    {
    case 0x0:
        printf("8 kBytes of P-flash, 0.25 kBytes protection region\r\n");
        break;
    case 0x1:
        printf("16 kBytes of P-flash, 0.5 kBytes protection region\r\n");
        break;
    case 0x3:
        printf("32 kBytes of P-flash, 1 kBytes protection region\r\n");
        break;
    case 0x5:
        printf("64 kBytes of P-flash, 2 kBytes protection region\r\n");
        break;
    case 0x7:
        printf("128 kBytes of P-flash, 4 kBytes protection region\r\n");
        break;
    case 0x9:
        printf("256 kBytes of P-flash, 4 kBytes protection region\r\n");
        break;
     case 0xF:
        printf("32 kBytes of P-flash, 1 kBytes protection region\r\n");
        break;
    default:
        printf("ERR!! Undefined P-flash size\r\n");  
        break;  		
    }
    /* Determine the SRAM size */
    switch(SIM_HAL_GetSramSizeId(SIM_BASE))
    {
    case 0x3:
        printf("4 kBytes of SRAM\r\n");
        break;
    case 0x4:
        printf("8 kBytes of SRAM\r\n");
        break;
    default:
        printf("ERR!! Undefined SRAM size\r\n");  
        break;  		
    }
    printf("\r\n");
}

/*******************************************************************************
* EOF
******************************************************************************/

