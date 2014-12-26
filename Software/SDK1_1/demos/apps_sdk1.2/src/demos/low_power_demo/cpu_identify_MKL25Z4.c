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

#include "fsl_uart_hal.h"
#include "low_power_demo.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! 
 * @brief cpu identification function for KL25Z48M
 */
void cpu_identify(void)
{
    /* Determine the Kinetis family */
    switch((SIM->SDID & SIM_SDID_FAMID(0x7))>>SIM_SDID_FAMID_SHIFT) 
    {  
        case 0x0:
            printf_demo("\n\rKL0");
            break;
        case 0x1:
            printf_demo("\n\rKL1");
            break;
        case 0x2:
            printf_demo("\n\rKL2");
            break;
        case 0x3:
            printf_demo("\n\rKL3");
            break;
        case 0x4:
            printf_demo("\n\rKL4");
            break;
        default:
            printf_demo("\n\rUnrecognized Kinetis family device.\n\r");  
            break;  	
    }
    /* Determine Sub-Family ID */
    switch((SIM->SDID & SIM_SDID_SUBFAMID(0x7))>>SIM_SDID_SUBFAMID_SHIFT) 
    {  
        case 0x4:
            printf_demo("4");
            break;
        case 0x5:
            printf_demo("5");
            break;
        default:
            printf_demo("\n\rUnrecognized Kinetis sub-family device.\n\r");  
            break;  	
    }
    /* Determine the package size */
    switch((SIM->SDID & SIM_SDID_PINID(0xF))>>SIM_SDID_PINID_SHIFT) 
    {  
        case 0x0:
            printf_demo("  16pin       ");
            break;
        case 0x1:
            printf_demo("  24pin       ");
            break;
        case 0x2:
            printf_demo("  32pin      ");
            break;
        case 0x4:
            printf_demo("  48pin      ");
            break;
        case 0x5:
            printf_demo("  64pin      ");
            break;
        case 0x6:
            printf_demo("  80pin      ");
            break;
        case 0x8:
            printf_demo("  100pin      ");        
            break;
        default:
            printf_demo("\n\rUnrecognized Kinetis package code.      ");  
            break;  	
    }
    /* Determine Attribute ID */
    switch((SIM->SDID & SIM_SDID_SERIESID(0x7))>>SIM_SDID_SERIESID_SHIFT) 
    {  
        case 0x1:
            printf_demo("\n\rLow Power Line with Cortex M0+\n\r\n\r");
            break;
        default:
            printf_demo("\n\rUnrecognized Kinetis family attribute.\n\r");  
            break;  	
    }
    /* Determine the System SRAM Size */
    switch((SIM->SDID & SIM_SDID_SRAMSIZE(0x7))>>SIM_SDID_SRAMSIZE_SHIFT) 
    {  
        case 0x0:
            printf_demo("SRAM Size: 0.5 KB\n\r");
            break;
        case 0x1:
            printf_demo("SRAM Size:  1 KB\n\r");
            break;
        case 0x2:
            printf_demo("SRAM Size:  2 KB\n\r");
            break;
        case 0x3:
            printf_demo("SRAM Size:  4 KB\n\r");
            break;
        case 0x4:
            printf_demo("SRAM Size:  8 KB\n\r");
            break;
        case 0x5:
            printf_demo("SRAM Size:  16 KB\n\r");
            break;
        case 0x6:
            printf_demo("SRAM Size:  32 KB\n\r");
            break;
        case 0x7:
            printf_demo("SRAM Size:  64 KB\n\r");
            break;
        default:
            printf_demo("\n\rUnrecognized SRAM Size.\n\r");  
            break;  	
    }                
    /* Determine the P-flash size */
    switch((SIM->FCFG1 & SIM_FCFG1_PFSIZE(0xF))>>SIM_FCFG1_PFSIZE_SHIFT)
    {
        case 0x0:
            printf_demo("Flash size:  8 KB program flash, 0.25 KB protection region");
            break;
        case 0x1:
            printf_demo("Flash size:  16 KB program flash, 0.5 KB protection region	");
            break;
        case 0x3:
            printf_demo("Flash size:  32 KB program flash, 1 KB protection region	");
            break;
        case 0x5:
            printf_demo("Flash size:  64 KB program flash, 2 KB protection region	");
            break;
        case 0x7:
            printf_demo("Flash size:  128 KB program flash, 4 KB protection region	");
            break;
        case 0x9:
            printf_demo("Flash size:  256 KB program flash, 4 KB protection region	");
            break;
        case 0xF:
            printf_demo("Flash size:  128 KB program flash, 4 KB protection region	");
            break;
        default:
            printf_demo("ERR!! Undefined flash size\n\r");  
            break;  	  		
    }
    printf_demo("\r\n");
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
