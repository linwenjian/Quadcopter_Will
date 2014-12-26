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
#ifndef __LOW_POWER_DEMO_H__
#define __LOW_POWER_DEMO_H__

#include <stdint.h>
#include "fsl_uart_driver.h"
#include "fsl_smc_hal.h"
#include "fsl_mcg_hal.h"
#include "fsl_llwu_hal.h"
#include "fsl_power_manager.h"

/*******************************************************************************
* Defination
******************************************************************************/
#define LOW_POWER 0                     /*!< Low power indication during PLL initialization */
#define HIGH_GAIN 1                     /*!< High power indication during PLL initialization */

#define CLK0_TYPE           CRYSTAL     /*!< Clock type indication */

#define MCGOUT 1                        /*!< MCG out indication during PLL initialization */         

#define SWD_CLK_DISABLE PORT_HAL_SetMuxMode(PORTA_BASE, 0, kPortPinDisabled)        /*!< Disable SWD port clock */
#define SWD_DIO_DISABLE	PORT_HAL_SetMuxMode(PORTA_BASE, 3, kPortPinDisabled)        /*!< Disable SWD port DIO */

#define SWD_CLK_ENABLE PORT_HAL_SetMuxMode(PORTA_BASE, 0, kPortMuxAlt7)         /*!< Enable SWD port clock */
#define SWD_DIO_ENABLE PORT_HAL_SetMuxMode(PORTA_BASE, 3, kPortMuxAlt7)         /*!< Enable SWD port DIO */

/* Power mode index defines */
#define powerModeWaitIndex      0U
#define powerModeStopIndex      1U
#define powerModeVlprIndex      2U
#define powerModeVlpwIndex      3U
#define powerModeVlpsIndex      4U
#define powerModeLlsIndex       5U
#define powerModeVlls3Index     6U
#define powerModeVlls2Index     7U
#define powerModeVlls1Index     8U
#define powerModeVlls0Index     9U
#define powerModeRunIndex       10U
#define powerModePstop1Index    11U
#define powerModePstop2Index    12U

/* Define the total number of power configs to be used and callbacks */
/* Necessary for the PowerSysInit function */
#define numPowerModeConfigs     13U
#define numCallBacksUsed        1U


#define NULL_LLWU_SRC_VAL       8U
#if defined(FRDM_K22F120M)    
#define LLWU_PIN_NUM            kLlwuWakeupPin6
#elif defined(TWR_KV10Z75M)
#define LLWU_PIN_NUM            kLlwuWakeupPin5
#else
#define LLWU_PIN_NUM            kLlwuWakeupPin10
#endif

#define	LLWU_MOD_WKUP_SRC		kLlwuWakeupModule1

#define UNDEF_VALUE  0xFF

#ifdef  ON
#undef  ON
#endif
#define ON      (1)

#ifdef  OFF
#undef  OFF
#endif
#define OFF     (0)

/* LP Tiemr specific macros */   
#define LPTMR_INST      0U
#define LPTMR_USE_IRCLK 0U       /*!< LP Timer modules uses Internal Reference Clock */
#define LPTMR_USE_LPOCLK 1U      /*!< LP Timer modules uses 1KHz clock */
#define LPTMR_USE_ERCLK32 2U     /*!< LP Timer modules uses 32KHz External Reference Clock */
#define LPTMR_USE_OSCERCLK 3U    /*!< LP Timer modules uses External Reference Clock */
#define LPTMR_WKUP_INT_1S	1000000U
#define LPTMR_WKUP_INT_5S	5000000U

#define CRYSTAL 1                       /*!< Crystal */
#define CANNED_OSC 0                    /*!< CANNED_OSC */

/* IRC defines */
#define SLOW_IRC 0                      /*!< Slow Internal Reference Clock */
#define FAST_IRC 1                      /*!< Fast Internal Reference Clock */

#define MCG_OUT_FREQ 48000000           /*!< Given MCG OUT Clock frequency*/

#if defined(CPU_MK22FN512VDC12)
#define OSC0_CR                         OSC_CR
#define SMC_STOPCTRL_VLLSM_MASK         SMC_STOPCTRL_LLSM_MASK
#endif

#if defined(CPU_MKV10Z32VLF7)
#define	OSC_BASE			OSC0_BASE
#endif

/* 
* The expected PLL output frequency is:
* PLL out = ((CLKIN/PRDIV) x VDIV)
* where the CLKIN is CLK0_FREQ_HZ.
*
*/

#define PLL0_PRDIV      4       /*!<divide reference by 4 = 2 MHz */
#define PLL0_VDIV       24      /*!<multiply reference by 24 = 48 MHz */

#define LED4_TOGGLE (GPIO_DRV_TogglePinOutput(ledPins[3].pinName))        /*!< Turn on target LED3 */

/*******************************************************************************
* Prototypes
******************************************************************************/
void clock_monitor(uint8_t enable);
void init_hardware(void);   
void low_power_modes_test(void);
void llwu_init(void);
void llwu_configure(llwu_wakeup_pin_t pinEn, llwu_external_pin_modes_t riseFall, llwu_wakeup_module_t moduleEn );
void llwu_isr(void);

void led_init(void);

void port_isr(void);
void port_init(void);
void set_for_lp(uint8_t outOfLp);

void vlp_clock_config(int8_t nextMode);

void vlp_clock_config(int8_t nextMode);
uint8_t atc(uint8_t ircSelect, int32_t ircFreq, int32_t mcgOutFreq);

void out_srs(void);
void get_uart_data(uart_state_t * uartState, uint8_t * rxBuffer, uint32_t requestedByteCount);
power_manager_error_code_t callback0(power_manager_callback_type_t type,
        power_manager_user_config_t * configPtr,
        power_manager_callback_data_t * dataPtr);

#endif /* __LOW_POWER_DEMO_H__*/

/*******************************************************************************
* EOF
******************************************************************************/
