/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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
#ifndef LPM_RTOS_H
#define LPM_RTOS_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
// Standard C Included Files
#include <stdio.h>
// SDK Included Files
#include "board.h"
#include "fsl_clock_manager.h"
#include "fsl_power_manager.h"
#include "fsl_pmc_hal.h"
#include "fsl_smc_hal.h"
#include "fsl_os_abstraction.h"

#if (defined BOARD_USE_LPSCI)
#include "fsl_lpsci_driver.h"
#elif (defined BOARD_USE_LPUART)
#include "fsl_lpuart_driver.h"
#else
#include "fsl_uart_driver.h"
#endif // BOARD_USE_LPSCI
// Project Included Files

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#if (defined(FRDM_KL46Z48M) || defined(CPU_MK02FN128VLH10) || defined(CPU_MK02FN64VLH10) || defined(CPU_MK02FN12810))
#define HWADC_INSTANCE            0   /*! ADC instance 0. */
#define ADC_IRQ_N                 ADC0_IRQn   /*! ADC irq handler number. */
#else
#define HWADC_INSTANCE            1   /*! ADC instance 1. */
#define ADC_IRQ_N                 ADC1_IRQn   /*! ADC irq handler number. */
#endif

#if (defined(FSL_FEATURE_SIM_OPT_HAS_RTC_CLOCK_OUT_SELECTION) && FSL_FEATURE_SIM_OPT_HAS_RTC_CLOCK_OUT_SELECTION == 1)
/* The RTC instance used for RTC */
#define PM_RTOS_DEMO_RTC_FUNC_INSTANCE                  0
#endif

/* The LPTMR instance used for LPTMR */
#define PM_RTOS_DEMO_LPTMR_FUNC_INSTANCE                0

/* The LLWU wakeup module for RTC */
#define PM_RTOS_DEMO_RTC_LLWU_WAKEUP_MODULE             kLlwuWakeupModule5

/* The LLWU wakeup module for LPTMR */
#define PM_RTOS_DEMO_LPTMR_LLWU_WAKEUP_MODULE           kLlwuWakeupModule0

/************************* Configure for MQX **********************************/
#if (defined FSL_RTOS_MQX)
#define _MODULE_IRQ_HANDLER(module)  MQX_##module##IRQHandler
#else
#define _MODULE_IRQ_HANDLER(module)  module##IRQHandler
#endif /* LPM_RTOS_H */
#define MODULE_IRQ_HANDLER(module)  _MODULE_IRQ_HANDLER(module##_)
/******************************************************************************/

/************************* Configure for UART *********************************/
#if defined (KL25Z4_SERIES) || defined (KL46Z4_SERIES) || defined (KV10Z7_SERIES)
#define __PM_DBG_UART(instance) UART##instance##_
#elif defined (K64F12_SERIES) || defined (K24F12_SERIES) || defined (K63F12_SERIES) || \
      defined (K22F51212_SERIES) || defined (K22F25612_SERIES) || defined (K22F12810_SERIES) || \
      defined (KV31F51212_SERIES) || defined (KV31F25612_SERIES) || defined (KV31F12810_SERIES) || \
      defined (K70F12_SERIES) || defined(K60D10_SERIES) || defined(K24F25612_SERIES) || \
      defined (KV30F12810_SERIES) || defined (K02F12810_SERIES)
#define __PM_DBG_UART(instance) UART##instance##_RX_TX_
#elif defined (KL27Z644_SERIES) || defined (KL17Z644_SERIES)
#define __PM_DBG_UART(instance) UART##instance##_FLEXIO_
#elif defined (KL27Z644_SERIES) || defined (KL17Z644_SERIES)
#else
    #error "No valid CPU defined!"
#endif

#define __PM_DBG_UART_IRQn(instance)    instance##IRQn

#define _PM_DBG_UART(instance)          __PM_DBG_UART(instance)
#define _PM_DBG_UART_IRQn(instance)     __PM_DBG_UART_IRQn(instance)

//This macro generates UARTxxx_
#define PM_DBG_UART                     _PM_DBG_UART(BOARD_DEBUG_UART_INSTANCE)
// This macro generates UARTxxx_IRQn
#define PM_DBG_UART_IRQn                _PM_DBG_UART_IRQn(PM_DBG_UART)

#define _PM_DBG_UART_IRQ_HANDLER(instance)   _MODULE_IRQ_HANDLER(instance)
// This macro generates UARTxxx_IRQHandler
#define PM_DBG_UART_IRQ_HANDLER         _PM_DBG_UART_IRQ_HANDLER(PM_DBG_UART)
/******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
// Structures & enumerations
///////////////////////////////////////////////////////////////////////////////
enum LPM_DEMO_RETURN
{
    LPM_DEMO_RETURN_OK,
    LPM_DEMO_RETURN_NULL,
};

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
clock_manager_error_code_t rtos_cm_callback(clock_notify_struct_t *notify, void* callbackData);
clock_manager_error_code_t dbg_console_cm_callback(clock_notify_struct_t *notify, void* callbackData);
power_manager_error_code_t rtos_pm_callback(power_manager_notify_struct_t * notify,
 power_manager_callback_data_t * dataPtr);
power_manager_error_code_t adc16_pm_callback(power_manager_notify_struct_t * notify, power_manager_callback_data_t * dataPtr);

/*!
 * @Brief get character from uart0 in blocking mode.
 */
char getInput(void);

/*!
 * @Brief IRQ handler.
 */
/*******************************************************************************
 * ADC
 ******************************************************************************/
#if (ADC_INSTANCE_COUNT > 0)
void MODULE_IRQ_HANDLER(ADC0)(void);
#endif
#if (ADC_INSTANCE_COUNT > 1U)
void MODULE_IRQ_HANDLER(ADC1)(void);
#endif
#if (ADC_INSTANCE_COUNT > 2U)
void MODULE_IRQ_HANDLER(ADC2)(void);
#endif
#if (ADC_INSTANCE_COUNT > 3U)
void MODULE_IRQ_HANDLER(ADC3)(void);
#endif

/*******************************************************************************
 * UART
 ******************************************************************************/
void PM_DBG_UART_IRQ_HANDLER(void);

/*******************************************************************************
 * RTC
 ******************************************************************************/
#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
void MODULE_IRQ_HANDLER(RTC)(void);
#endif
void MODULE_IRQ_HANDLER(LPTMR0)(void);
/*******************************************************************************
 * LLW
 ******************************************************************************/
#if (defined FRDM_KL03Z48M)
void MODULE_IRQ_HANDLER(LLWU)(void);
#else
void MODULE_IRQ_HANDLER(LLW)(void);
#endif

#endif

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////

