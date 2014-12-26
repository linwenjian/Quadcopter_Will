/*
 * Copyright (c) 2013, Freescale Semiconductor, Inc.
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
#include "fsl_clock_manager.h"
#include "fsl_llwu_hal.h"
#include "fsl_lptmr_hal.h"
#include "fsl_interrupt_manager.h"

#include "gpio_pins.h"
#include "low_power_demo.h"

#if defined(TWR_KV10Z75M)
#undef  LLW_IRQn
#define LLW_IRQn        LLWU_IRQn
#undef  LPTimer_IRQn
#define LPTimer_IRQn    LPTMR0_IRQn
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint8_t uartEnabled; /*!< indicates whether the uart port is currently enabled or not */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
* @brief ISR Routine for Low Power Timer
*/
void demo_lptmr_isr(void)
{
   volatile uint32_t lptmrCsrTemp;
   CLOCK_SYS_EnableLptimerClock(0);
       
   LPTMR_HAL_ClearIntFlag(LPTMR0_BASE);      /* write 1 to TCF to clear the LPT timer compare flag */
   LPTMR_HAL_Enable(LPTMR0_BASE);            /* enable timer */
   LPTMR_HAL_SetIntCmd(LPTMR0_BASE, true);   /* enable interrupts */
   LPTMR_HAL_ClearIntFlag(LPTMR0_BASE);      /* clear the flag */

   /*wait for write to complete to  before returning */
   while(!(LPTMR_HAL_IsEnabled(LPTMR0_BASE) && LPTMR_HAL_GetIntCmd(LPTMR0_BASE)));
}

/*!
* @brief initializes LLWU module
*/
void llwu_init(void)
{
    /* Configure the interrupt vector for the interrupt handler */
    INT_SYS_InstallHandler(LLW_IRQn, &llwu_isr);
    
    /* Enable the IRQs */
    INT_SYS_EnableIRQ(LLW_IRQn);
    
    /* Configure the interrupt vector for the interrupt handler */
    INT_SYS_InstallHandler(LPTimer_IRQn, &demo_lptmr_isr);

    /* 
    * TWR_K22F120M and TWR_K64F120M: SW1,PTC6 LLWU_P10
    * FRDM_K64F120M: SW2,PTC6 LLWU_P10
    * FRDM_K22F120M: SW2,PTC1 LLWU_P6
    */
    llwu_configure(LLWU_PIN_NUM, kLlwuExternalPinFallingEdge, kLlwuWakeupModule0);
}

/*!
 * @brief Set up the LLWU for wakeup the MCU from LLS and VLLSx modes 
 * from the selected pin or module.
 *  
 * @param
 * pin_en - unsigned integer, bit position indicates the pin is enabled.  
 *          More than one bit can be set to enable more than one pin at a time.  
 * @param 
 * rise_fall - 0x00 = External input disabled as wakeup
 *             0x01 - External input enabled as rising edge detection
 *             0x02 - External input enabled as falling edge detection
 *             0x03 - External input enablge as any edge detection
 * @param
 * module_en - unsigned char, bit position indicates the module is enabled.  
 *             More than one bit can be set to enabled more than one module                   
 *  
 * for example:  if bit 0 and 1 need to be enabled as rising edge detect call this  routine with
 * pin_en = 0x0003 and rise_fall = 0x02
   
 * Note: to set up one set of pins for rising and another for falling, 2 calls to this 
 *       function are required, 1st for rising then the second for falling.
 */
void llwu_configure(llwu_wakeup_pin_t pinEn, llwu_external_pin_modes_t riseFall, 
                    llwu_wakeup_module_t moduleEn ) 
{
    /****************************************************************
     * LLWU pin initialization
     *
     * First, clear the associated flag just to be sure the device
     * doesn't immediately enter the LLWU interrupt service routine 
     * (ISR). Then enable the interrupt
     ****************************************************************/
    LLWU_HAL_ClearExternalPinWakeupFlag(LLWU_BASE, pinEn);
    
    LLWU_HAL_SetExternalInputPinMode(LLWU_BASE, riseFall, pinEn);
    
    /******************************************************************
     * LLWU module initialization
     *
     * Now enable an internal peripheral module as an LLWU source if
     * desired
     ****************************************************************/
    if (moduleEn < NULL_LLWU_SRC_VAL)
    {
      LLWU_HAL_SetInternalModuleCmd(LLWU_BASE, moduleEn, true);
    }
    
} /* End LLWU Configuration */


/*!
* @brief LLWU ISR function
*/
void llwu_isr(void)
{
    uint8_t pinEn;
      
    NVIC_ClearPendingIRQ(LLW_IRQn);
    
    /* Print LLWU acknowledgement only if UART is enabled */
    for(pinEn = 0; pinEn < FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN; pinEn++)
    {
      if (LLWU_HAL_GetExternalPinWakeupFlag(LLWU_BASE, (llwu_wakeup_pin_t)pinEn)) {
          LLWU_HAL_ClearExternalPinWakeupFlag(LLWU_BASE, (llwu_wakeup_pin_t)pinEn);   /* write one to clear the flag */
      }
    }
                       
    /*
    * Note: This ISR does not write to the LLWU_F3 register because these
    * are peripheral module wakeups.  The flags contained in the LLWU_F3
    * register should be cleared through the associated module interrupt
    * and not through the LLWU_F3 per the Kinetis L Family Reference
    * Manual (LLWU Chapter)
    */
    if (LLWU_HAL_GetInternalModuleWakeupFlag(LLWU_BASE, kLlwuWakeupModule0)) {
        CLOCK_SYS_EnableLptimerClock(0);
        LPTMR_HAL_ClearIntFlag(LPTMR0_BASE);   /* write 1 to TCF to clear the LPT timer compare flag */
        LPTMR_HAL_IsEnabled(LPTMR0_BASE);
        LPTMR_HAL_SetIntCmd(LPTMR0_BASE, 1);
        LPTMR_HAL_IsIntPending(LPTMR0_BASE);
    }
    if(LLWU_HAL_GetFilterDetectFlag(LLWU_BASE, 0)){
       LLWU_HAL_ClearFilterDetectFlag(LLWU_BASE, 0);
    }
    if(LLWU_HAL_GetFilterDetectFlag(LLWU_BASE, 1)){
       LLWU_HAL_ClearFilterDetectFlag(LLWU_BASE, 1);
    }
}
                   
/*******************************************************************************
 * EOF
 ******************************************************************************/
