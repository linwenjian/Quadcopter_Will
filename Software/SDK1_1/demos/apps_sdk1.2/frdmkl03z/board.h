/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
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

#if !defined(__BOARD_H__)
#define __BOARD_H__

#include <stdint.h>

#include "gpio_pins.h"
#include "pin_mux.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/* The board name */
#define BOARD_NAME                      "FRDM-KL03Z"

/*! The LPUART to use for debug messages. */
#define BOARD_DEBUG_UART_INSTANCE (0)
#define BOARD_DEBUG_UART_BAUD      9600
#define BOARD_DEBUG_UART_BASEADDR  LPUART0

/* Define the port interrupt number for the board switches */
#define BOARD_SW_IRQ_NUM            PORTB_IRQn
#define BOARD_SW_IRQ_HANDLER        PORTB_IRQHandler
/* Define print statement to inform user which switch to press for
 * low_power_demo
 */

#define PRINT_LLWU_SW_NUM \
  printf("SW2")


 /* Defines the llwu pin number for board switch which is used in power_manager_demo. */
#define BOARD_SW_HAS_LLWU_PIN        1
#define BOARD_SW_LLWU_EXT_PIN        4
/* Switch port base address and IRQ handler name. Used by power_manager_demo */
#define BOARD_SW_LLWU_PIN            0
#define BOARD_SW_LLWU_BASE           PORTB
#define BOARD_SW_LLWU_IRQ_HANDLER    PORTB_IRQHandler
#define BOARD_SW_LLWU_IRQ_NUM        PORTB_IRQn

#define BOARD_USE_LPUART

#define BOARD_MMA8451_I2C_INSTANCE 0

#define BOARD_DAC_I2C_INSTANCE      1

/*! The MMA8451 i2c slave address */
#define BOARD_ACCEL_ADDR_ACTUAL    (0x1D)
#define BOARD_ACCEL_I2C_ADDR       (BOARD_ACCEL_ADDR_ACTUAL << 1)

#define BOARD_I2C_GPIO_SCL          GPIO_MAKE_PIN(GPIOB_IDX, 3)
#define BOARD_I2C_GPIO_SDA          GPIO_MAKE_PIN(GPIOB_IDX, 4)
#define BOARD_I2C_DELAY \
    do \
    { \
        int32_t i; \
        for (i = 0; i < 500; i++) \
        { \
            __asm("nop"); \
        } \
    } while (0)



#define BOARD_I2C_COMM_INSTANCE     0

/* ADC channels definition for thermistors using differential input */
#define BOARD_ADC_INSTANCE              0         /*!< ADC instance number*/
#define ADC0_CHANNEL0                   3         /*!< PTA8 - ADC0_SE3 */
#define ADC0_CHANNEL1                   2         /*!< PTA9 - ADC0_SE2 */

#define BOARD_I2C_COMM_INSTANCE     0

/* The TPM instance/channel used for board */
#define BOARD_TPM_INSTANCE              0
#define BOARD_TPM_CHANNEL               0

/* board led color mapping */
#define BOARD_GPIO_LED_RED              kGpioLED1
#define BOARD_GPIO_LED_GREEN            kGpioLED2
#define BOARD_GPIO_LED_BLUE             kGpioLED3

#define LED1_EN  {PORT_HAL_SetMuxMode(PORTB, 10, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[0]);}  /*!< Enable target LED1 */
#define LED2_EN  {PORT_HAL_SetMuxMode(PORTB, 11, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[1]);}  /*!< Enable target LED2 */
#define LED3_EN  {PORT_HAL_SetMuxMode(PORTB, 13, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[2]);}  /*!< Enable target LED3 */

#define LED1_DIS (PORT_HAL_SetMuxMode(PORTB, 10, kPortPinDisabled))  /*!< Disable target LED1 */
#define LED2_DIS (PORT_HAL_SetMuxMode(PORTB, 11, kPortPinDisabled))  /*!< Disable target LED2 */
#define LED3_DIS (PORT_HAL_SetMuxMode(PORTB, 13, kPortPinDisabled))  /*!< Disable target LED3 */

#define LED1_OFF (GPIO_DRV_WritePinOutput(ledPins[0].pinName, 1))         /*!< Turn off target LED1 */
#define LED2_OFF (GPIO_DRV_WritePinOutput(ledPins[1].pinName, 1))         /*!< Turn off target LED2 */
#define LED3_OFF (GPIO_DRV_WritePinOutput(ledPins[2].pinName, 1))         /*!< Turn off target LED3 */

#define LED1_ON (GPIO_DRV_WritePinOutput(ledPins[0].pinName, 0))          /*!< Turn on target LED1 */
#define LED2_ON (GPIO_DRV_WritePinOutput(ledPins[1].pinName, 0))          /*!< Turn on target LED2 */
#define LED3_ON (GPIO_DRV_WritePinOutput(ledPins[2].pinName, 0))          /*!< Turn on target LED3 */

#define LED1_TOGGLE (GPIO_DRV_TogglePinOutput(ledPins[0].pinName))        /*!< Toggle on target LED1 */
#define LED2_TOGGLE (GPIO_DRV_TogglePinOutput(ledPins[1].pinName))        /*!< Toggle on target LED2 */
#define LED3_TOGGLE (GPIO_DRV_TogglePinOutput(ledPins[2].pinName))        /*!< Toggle on target LED3 */

#define OFF_ALL_LEDS  \
                           LED1_OFF;\
                           LED2_OFF;\
                           LED3_OFF;

/* deafult ADC channel for hw trigger demo */
#define BOARD_ADC_HW_TRIGGER_CHAN 3

/* The rtc instance used for rtc_func */
#define BOARD_RTC_FUNC_INSTANCE         0

/******************************************************************************
 * Externs
 ******************************************************************************/

/******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void hardware_init(void);
void dbg_uart_init(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __BOARD_H__ */
/******************************************************************************
 * EOF
 ******************************************************************************/
