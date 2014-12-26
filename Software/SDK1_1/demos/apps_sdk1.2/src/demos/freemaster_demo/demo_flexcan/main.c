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

//! This demo initializes FlexCAN module for communication. The FreeMASTER driver itself
//! supports multiple types of communication, but it requires the module to be initialized
//! before it starts. Refer to other freemaster_demo applications in the parent directory 
//! to see how other communication modules are initialized.
#if !FMSTR_USE_FLEXCAN
#error This demo is prepared for FlexCAN communication, refer to other demo_xxx folders for other interfaces 
#endif

//! FreeMASTER supports two ways haw to select the communication module instance. Either 
//! statically via module base address defined as FMSTR_CAN_BASE in freemaster_cfg.h file, 
//! or dynamically through FMSTR_SetCanBaseAddress() function call. This demo uses the later
//! case. It makes no sense to define the FMSTR_CAN_BASE in this demo.
#if defined(FMSTR_CAN_BASE)
#error This application expects dynamically-assigned base address of the communication module.
#endif

//! All compile-time checks passed, include LPUART HAL driver files
#include "fsl_flexcan_hal.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
/////////////////////////////////////////////////////////////////////////////////

//! Table of base addresses for CAN instances.
CAN_Type * g_flexcanBase[] = CAN_BASE_PTRS;

//! CAN Message Buffer IRQ enum numbers defined in CMSIS header file.
const IRQn_Type g_flexcanOredMessageBufferIrqId[] = CAN_ORed_Message_buffer_IRQS;

//! Note: All global variables accessed by FreeMASTER are defined in a shared 
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_flexcan(uint32_t instance);

////////////////////////////////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    // Board initialization
    hardware_init();
    configure_can_pins(BOARD_CAN_INSTANCE);
    // Initialize OSA to be able to install interrupt handlers generically
    OSA_Init();

    // FreeMASTER will communicate over CAN.
    init_freemaster_flexcan(BOARD_CAN_INSTANCE);

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
 * @brief FlexCAN Module initialization
 */
static void init_freemaster_flexcan(uint32_t instance)
{
    flexcan_time_segment_t canTiming;
    CAN_Type * canBaseAddr;
    uint32_t canClk = 0;
    IRQn_Type canIrqNum;

    // Get CAN base address and Message Buffer interrupt number
    canBaseAddr = g_flexcanBase[instance];
    canIrqNum = g_flexcanOredMessageBufferIrqId[instance];

    // Configure the CAN pins
    configure_can_pins(instance);

    // Un-gate CAN module clock
    CLOCK_SYS_EnableFlexcanClock(instance);
    CLOCK_SYS_GetFreq(kOsc0ErClock, &canClk);

    // Select a source clock for FlexCAN - 8MHz oscillator
    FLEXCAN_HAL_SelectClock(canBaseAddr, kFlexCanClkSource_Osc);

    // Enable the CAN clock
    FLEXCAN_HAL_Enable(canBaseAddr);

    // CAN low-level initialization
    FLEXCAN_HAL_Init(canBaseAddr);
    FLEXCAN_HAL_SetMaxMbNumber(canBaseAddr, 16);
    
    // Set time segments to achieve 500kbps. This is different for different OSC clocks
    switch(canClk)
    {
        case 8000000UL: // 8MHz Tq clock, 1+3+4+8=16 Tqs in one bit making 8MHz/16=500kbps
            canTiming.pre_divider = 0;  // No prescaler
            canTiming.propseg = 3-1;    // Propagation segment
            canTiming.pseg1 = 4-1;      // Phase segment 1
            canTiming.pseg2 = 8-1;      // Phase segment 2
            canTiming.rjw = 4-1;        // Resync jump width
            break;
            
        case 50000000UL:    // 50MHz Tq clock, pre-scale to 10MHz and use 1+3+8+8=20 Tqs in one bit making 10MHz/20=500kbps
            canTiming.pre_divider = 5-1;    // 5x prescaler to achieve 10MHz
            canTiming.propseg = 3-1;        // Propagation segment
            canTiming.pseg1 = 8-1;          // Phase segment 1
            canTiming.pseg2 = 8-1;          // Phase segment 2
            canTiming.rjw = 4-1;            // Resync jump width
            break;
            
        default:
            assert(0);   // Add calculation for your OSC clock
            while(1);    // Do not continue
    };
    FLEXCAN_HAL_SetTimeSegments(canBaseAddr, &canTiming);

    // FreeMASTER uses exact ID matching, no advance filtering is needed.
    // Use global ID filtering which is simpler to set up. MB masks are
    // already configured for exact matching (done by HAL Init).
    FLEXCAN_HAL_SetMaskType(canBaseAddr, kFlexCanRxMask_Global);

    // Install the FreeMASTER CAN interrupt handler
    OSA_InstallIntHandler(canIrqNum, FMSTR_Isr);

    // Enable UART interrupt on NVIC level.
    INT_SYS_EnableIRQ(canIrqNum);

    // Set normal operation mode
    FLEXCAN_HAL_EnableOperationMode(canBaseAddr, kFlexCanNormalMode);

    // Base address is configured dynamically in this application, making slightly
    // bigger/slower code but more generic. See freemaster_cfg.h for more details
    FMSTR_SetCanBaseAddress((FMSTR_ADDR)canBaseAddr);
}

/*!
 * @brief Example of direct interrupt vector installation - overriding the weak
 * symbol which is put in the vector table. Note that the function name
 * contains the CAN instance number. Replace the CAN0 in the name with the
 * proper instance number.
 *
 * This 'direct' approach may be used instead of OSA_InstallIntHandler
 * above; for example when ROM-based interrupt table is used.
 */
// void CAN0_ORed_Message_buffer_IRQHandler(void)
// {
    // FMSTR_Isr();
// }

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
