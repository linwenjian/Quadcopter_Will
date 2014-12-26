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

//! This demo initializes SCI/UART module for communication. The FreeMASTER driver itself
//! supports multiple types of communication, but it requires the module to be initialized
//! before it starts. Refer to other freemaster_demo applications in the parent directory 
//! to see how other communication modules are initialized.
#ifndef BOARD_USE_LPUART
#define BOARD_USE_LPUART 0
#endif
#if !(FMSTR_USE_SCI) || (BOARD_USE_LPUART)
#error This demo is prepared for UART communication, refer to other demo_xxx folders for other interfaces 
#endif

//! This demo uses the default UART module as it is defined in the board.h file.
//! You can choose a different UART instance by rewriting in the main() function below.
#ifndef BOARD_DEBUG_UART_INSTANCE
#error Define BOARD_DEBUG_UART_INSTANCE to select what UART port is used with FreeMASTER
#endif

//! FreeMASTER supports two ways haw to select the communication module instance. Either 
//! statically via module base address defined as FMSTR_SCI_BASE in freemaster_cfg.h file, 
//! or dynamically through FMSTR_SetSciBaseAddress() function call. This demo uses the later
//! case. It makes no sense to define the FMSTR_SCI_BASE in this demo.
#if defined(FMSTR_SCI_BASE)
#error This application expects dynamically-assigned base address of the communication module.
#endif

//! All compile-time checks passed, include LPUART HAL driver files
#include "fsl_uart_hal.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Table of base addresses for uart instances.
UART_Type * g_uartBase[UART_INSTANCE_COUNT] = UART_BASE_PTRS;

//! Table to save UART IRQ numbers defined in CMSIS files.
IRQn_Type g_uartRxTxIrqId[UART_INSTANCE_COUNT] = UART_RX_TX_IRQS;

//! Note: All global variables accessed by FreeMASTER are defined in a shared 
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_uart(uint32_t instance);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    // Board initialization
    hardware_init();

    // Initialize OSA to be able to install interrupt handlers generically
    OSA_Init();

    // FreeMASTER will communicate over default UART instance.
    init_freemaster_uart(BOARD_DEBUG_UART_INSTANCE);

    // Initialize FreeMASTER driver
    FMSTR_Example_Init();

    while(1)
    {
        // FreeMASTER example increments several variables periodically,
        // use the FreeMASTER PC Host tool to visualize the variables
        FMSTR_Example_Poll();
    }
}

/*!
 * @brief UART Module initialization
 */
static void init_freemaster_uart(uint32_t instance)
{
    uint32_t uartSourceClock;
    UART_Type * uartBaseAddr;
    IRQn_Type uartIrqNum;

    // Get UART base address and vector
    uartBaseAddr = g_uartBase[instance];
    uartIrqNum = g_uartRxTxIrqId[instance];

    // Configure the UART TX/RX pins
    configure_uart_pins(instance);

    // UART clock source is either system clock or bus clock depending on the instance
    uartSourceClock = CLOCK_SYS_GetUartFreq(instance);

    // Un-gate UART module clock
    CLOCK_SYS_EnableUartClock(instance);

    // UART low-level initialization
    UART_HAL_Init(uartBaseAddr);

    // Set UART baudrate to standard speed and setup
    UART_HAL_SetBaudRate(uartBaseAddr, uartSourceClock, 115200);
    UART_HAL_SetBitCountPerChar(uartBaseAddr, kUart8BitsPerChar);
    UART_HAL_SetParityMode(uartBaseAddr, kUartParityDisabled);
#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
    UART_HAL_SetStopBitCount(uartBaseAddr, kUartOneStopBit);
#endif

    // Install the FreeMASTER UART interrupt handler
    OSA_InstallIntHandler(uartIrqNum, FMSTR_Isr);

    // Enable UART interrupt on NVIC level.
    INT_SYS_EnableIRQ(uartIrqNum);

    // Base address is configured dynamically in this application, making slightly
    // bigger/slower code but more generic. See freemaster_cfg.h for more details
    FMSTR_SetSciBaseAddress((FMSTR_ADDR)uartBaseAddr);
}

/*!
 * @brief Example of direct interrupt vector installation - overriding the weak
 * symbol which is put in the vector table. Note that the function name
 * contains the UART instance number. Replace the UART1 in the name with the
 * proper instance number (for example a value of BOARD_DEBUG_UART_INSTANCE
 * macro defined in board.h)
 * 
 * This 'direct' approach may be used instead of OSA_InstallIntHandler
 * above, for example when ROM-based interrupt table is used.
 */
// void UART1_RX_TX_IRQHandler(void)
// {
    // FMSTR_Isr();
// }

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
