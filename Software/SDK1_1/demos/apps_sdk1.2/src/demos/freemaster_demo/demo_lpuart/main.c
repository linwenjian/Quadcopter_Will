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

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#include "fsl_interrupt_manager.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_example.h"

//! This demo initializes LPUART module for communication. The FreeMASTER driver itself
//! supports multiple types of communication, but it requires the module to be initialized
//! before it starts. Refer to other freemaster_demo applications in the parent directory 
//! to see how other communication modules are initialized.
#if !(FMSTR_USE_LPUART) || !defined(BOARD_USE_LPUART)
#error This demo is prepared for LPUART communication, refer to other demo_xxx folders for other interfaces 
#endif

//! This demo uses the default LPUART module as it is defined in the board.h file.
//! You can choose a different LPUART instance by rewriting in the main() function below.
#ifndef BOARD_DEBUG_UART_INSTANCE
#error Define BOARD_USE_LPUART and BOARD_DEBUG_UART_INSTANCE to select what LPUART port is used with FreeMASTER
#endif

//! FreeMASTER supports two ways haw to select the communication module instance. Either 
//! statically via module base address defined as FMSTR_SCI_BASE in freemaster_cfg.h file, 
//! or dynamically through FMSTR_SetSciBaseAddress() function call. This demo uses the later
//! case. It makes no sense to define the FMSTR_SCI_BASE in this demo.
#if defined(FMSTR_SCI_BASE)
#error This application expects dynamically-assigned base address of the communication module.
#endif

//! FreeMASTER can't support CLOCK_SETUP=2 in frdmkl03z48m board.
#if defined(FRDM_KL03Z48M) && (CLOCK_SETUP==2)
#error This application can't support CLOCK_SETUP=2 in frdmkl03z48m board.
#endif

//! All compile-time checks passed, include LPUART HAL driver files
#include "fsl_lpuart_hal.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Table of base addresses for lpuart instances.
LPUART_Type * g_lpuartBase[LPUART_INSTANCE_COUNT] = LPUART_BASE_PTRS;

//! Table to save LPUART enum numbers defined in CMSIS files.
IRQn_Type g_lpuartRxTxIrqId[LPUART_INSTANCE_COUNT] = LPUART_RX_TX_IRQS;

//! Note: All global variables accessed by FreeMASTER are defined in a shared 
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_lpuart(uint32_t instance);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    // Board initialization
    hardware_init();

    // Initialize OSA to be able to install interrupt handlers generically
    OSA_Init();

    // FreeMASTER will communicate over default LPUART instance.
    init_freemaster_lpuart(BOARD_DEBUG_UART_INSTANCE);

    // Initialize FreeMASTER driver - shared code for all FreeMASTER demos
    FMSTR_Example_Init();

    while(1)
    {
        // FreeMASTER example increments several variables periodically,
        // use the FreeMASTER PC Host tool to visualize the variables
        FMSTR_Example_Poll();
    }
}

/*!
 * @brief LPUART Module initialization
 */
static void init_freemaster_lpuart(uint32_t instance)
{
    uint32_t lpuartSourceClock;
    LPUART_Type * lpuartBaseAddr;
    IRQn_Type lpuartIrqNum;

    // Get LPUART base address and vector
    lpuartBaseAddr = g_lpuartBase[instance];
    lpuartIrqNum = g_lpuartRxTxIrqId[instance];

    // Set the uart pin mux.
    configure_uart_pins(instance);

    // Set clock source for LPUART
#if (CLOCK_SETUP == 1)
    CLOCK_SYS_SetLpuartSrc(instance, kClockLpuartSrcIrc48M);
#else
    CLOCK_SYS_SetLpuartSrc(instance, kClockLpuartSrcMcgIrClk);
#endif        
    // LPUART clock source is either system clock or bus clock depending on the instance
    lpuartSourceClock = CLOCK_SYS_GetLpuartFreq(instance);

    // Un-gate LPUART module clock
    CLOCK_SYS_EnableLpuartClock(instance);

    // LPUART low-level initialization
    LPUART_HAL_Init(lpuartBaseAddr);

    // Set LPUART baudrate to standard speed and setup
    LPUART_HAL_SetBaudRate(lpuartBaseAddr, lpuartSourceClock, BOARD_DEBUG_UART_BAUD);
    LPUART_HAL_SetBitCountPerChar(lpuartBaseAddr, kLpuart8BitsPerChar);
    LPUART_HAL_SetParityMode(lpuartBaseAddr, kLpuartParityDisabled);
#if FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT
    LPUART_HAL_SetStopBitCount(lpuartBaseAddr, kLpuartOneStopBit);
#endif

    // This demo uses the vector table handler installation in ROM
    // -- see the LPUART0_IRQHandler() code below
    // Uncomment the following line when using RAM-based vector table 
    //  OSA_InstallIntHandler(lpuartIrqNum, FMSTR_Isr);
    
    // Enable LPUART interrupt on NVIC level.
    INT_SYS_EnableIRQ(lpuartIrqNum);

    // Base address is configured dynamically in this application, making slightly
    // bigger/slower code but more generic. See freemaster_cfg.h for more details
    FMSTR_SetSciBaseAddress((FMSTR_ADDR)lpuartBaseAddr);
}

/*!
 * @brief Example of direct interrupt vector installation - overriding the weak
 * symbol which is put in the vector table. Note that the function name
 * contains the LPUART instance number. Replace the LPUART1 in the name with the
 * proper instance number (for example a value of BOARD_DEBUG_LPUART_INSTANCE
 * macro defined in board.h)
 * 
 * This 'direct' approach may be used instead of OSA_InstallIntHandler
 * above, for example when ROM-based interrupt table is used.
 */

// void LPUART1_RX_TX_IRQHandler(void)
// {
    // FMSTR_Isr();
// }

void LPUART0_IRQHandler(void)
{
    FMSTR_Isr();
}

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
