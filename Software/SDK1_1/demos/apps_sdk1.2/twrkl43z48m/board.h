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
#define BOARD_NAME                  "TWR-KL43Z"
/*! The UART to use for debug messages. */
#define BOARD_DEBUG_UART_INSTANCE   0
#define BOARD_DEBUG_UART_BASEADDR   UART2
#ifndef BOARD_DEBUG_UART_BAUD
#define BOARD_DEBUG_UART_BAUD       115200
#endif

#define BOARD_USE_LPUART

#define BOARD_DAC_DEMO_DAC_INSTANCE     0U
#define BOARD_DAC_DEMO_ADC_INSTANCE     0U
#define BOARD_DAC_DEMO_ADC_CHANNEL      23U

/* Define print statement to inform user which switch to press for
 * low_power_demo
 */
#define PRINT_INT_SW_NUM \
  printf("SW2")

#define PRINT_LLWU_SW_NUM \
  printf("SW1")

 /* Defines the llwu pin number for board switch which is used in power_manager_demo. */
#define BOARD_SW_HAS_LLWU_PIN        1
#define BOARD_SW_LLWU_EXT_PIN        7
/* Switch port base address and IRQ handler name. Used by power_manager_demo */
#define BOARD_SW_LLWU_PIN            3
#define BOARD_SW_LLWU_BASE           PORTC
//#define BOARD_SW_LLWU_IRQ_HANDLER    PORTCD_IRQHandler
#define BOARD_SW_LLWU_IRQ_NUM        PORTCD_IRQn

#define BOARD_TPM_INSTANCE 0
#define BOARD_TPM_CHANNEL 0

/* Isn't supported now to use DAC as sine generator in adc_hw_trigger demo */
//#define USE_DAC_OUT_AS_SOURCE

#define BOARD_CMP_INSTANCE          0
#define BOARD_CMP_CHANNEL           0

#define BOARD_I2C_COMM_INSTANCE     1

/* The rtc instance used for rtc_func */
#define BOARD_RTC_FUNC_INSTANCE         0

/*! The FXOS8700 i2c slave address */
#define BOARD_ACCEL_I2C_INSTANCE    0
#define BOARD_ACCEL_I2C_ADDR        (0x1f << 1)

/*! Tower I2C instance connected to Accelerometer */
#define BOARD_I2C_GPIO_SCL          GPIO_MAKE_PIN(GPIOE_IDX, 1) // PTE1
#define BOARD_I2C_GPIO_SDA          GPIO_MAKE_PIN(GPIOE_IDX, 0) // PTE0
#define BOARD_I2C_DELAY \
    do \
    { \
        int32_t i; \
        for (i = 0; i < 500; i++) \
        { \
            __asm("nop"); \
        } \
    } while (0)

/* board led color mapping */
#define BOARD_GPIO_LED_GREEN           kGpioLED1
#define BOARD_GPIO_LED_RED             kGpioLED2
#define BOARD_GPIO_LED_BLUE            kGpioLED3
#define BOARD_GPIO_LED2_RED            kGpioLED4
      
/* board button mapping */
#define BOARD_GPIO_BUTTON              kGpioSW3
#define BOARD_GPIO_BUTTON_IRQ          PORTA_IRQn /*!< PTA5 */
      
#define BOARD_GPIO_BUTTON2             kGpioSW2
#define BOARD_GPIO_BUTTON2_IRQ         PORTA_IRQn /*!< PTA4 */
        
#define LED1_EN (PORT_HAL_SetMuxMode(PORTA, 12, kPortMuxAsGpio)) 		/*!< Enable target LED1 */
#define LED2_EN (PORT_HAL_SetMuxMode(PORTA, 13, kPortMuxAsGpio)) 		/*!< Enable target LED2 */
#define LED3_EN (PORT_HAL_SetMuxMode(PORTB,  0, kPortMuxAsGpio)) 		/*!< Enable target LED3 */
#define LED4_EN (PORT_HAL_SetMuxMode(PORTB, 19, kPortMuxAsGpio)) 		/*!< Enable target LED4 */
            
#define LED1_DIS (PORT_HAL_SetMuxMode(PORTA, 12, kPortPinDisabled)) 	/*!< Enable target LED1 */
#define LED2_DIS (PORT_HAL_SetMuxMode(PORTA, 13, kPortPinDisabled)) 	/*!< Enable target LED2 */
#define LED3_DIS (PORT_HAL_SetMuxMode(PORTB,  0, kPortPinDisabled)) 	/*!< Enable target LED3 */
#define LED4_DIS (PORT_HAL_SetMuxMode(PORTB, 19, kPortPinDisabled)) 	/*!< Enable target LED4 */

#define LED1_OFF (GPIO_DRV_WritePinOutput(ledPins[0].pinName, 1))       	/*!< Turn off target LED1 */
#define LED2_OFF (GPIO_DRV_WritePinOutput(ledPins[1].pinName, 1))       	/*!< Turn off target LED2 */
#define LED3_OFF (GPIO_DRV_WritePinOutput(ledPins[2].pinName, 1))       	/*!< Turn off target LED3 */
#define LED4_OFF (GPIO_DRV_WritePinOutput(ledPins[3].pinName, 1))       	/*!< Turn off target LED4 */

#define LED1_ON (GPIO_DRV_WritePinOutput(ledPins[0].pinName, 0))        	/*!< Turn on target LED1 */
#define LED2_ON (GPIO_DRV_WritePinOutput(ledPins[1].pinName, 0))        	/*!< Turn on target LED2 */
#define LED3_ON (GPIO_DRV_WritePinOutput(ledPins[2].pinName, 0))        	/*!< Turn on target LED3 */
#define LED4_ON (GPIO_DRV_WritePinOutput(ledPins[3].pinName, 0))        	/*!< Turn on target LED4 */

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
