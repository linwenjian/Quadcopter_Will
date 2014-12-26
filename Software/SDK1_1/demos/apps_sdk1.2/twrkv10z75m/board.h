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

#if !defined(__BOARD_H__)
#define __BOARD_H__

#include <stdint.h>
#include "pin_mux.h"
#include "gpio_pins.h"

/* The board name */
#define BOARD_NAME                      "TWR-KV10Z32"

/* The UART to use for debug messages. */
#ifndef BOARD_DEBUG_UART_INSTANCE
    #define BOARD_DEBUG_UART_INSTANCE   0
    #define BOARD_DEBUG_UART_BASEADDR   UART0
#endif
#ifndef BOARD_DEBUG_UART_BAUD
    #define BOARD_DEBUG_UART_BAUD       115200
#endif

#define BOARD_USE_UART

/* Define feature for the low_power_demo */
#define FSL_FEATURE_HAS_VLLS2 (0)

/* Define the port interrupt number for the board switches */
#define BOARD_SW_IRQ_NUM            PORTBCDE_IRQn
#define BOARD_SW_IRQ_HANDLER        PORTBCDE_IRQHandler
/* Define print statement to inform user which switch to press for
 * low_power_demo
 */
#define PRINT_INT_SW_NUM \
  printf("SW1")

#define PRINT_LLWU_SW_NUM \
  printf("SW2")

/* The MMA8451 i2c instance and slave address */
#define BOARD_MMA8451_I2C_INSTANCE      0
#define BOARD_ACCEL_ADDR_ACTUAL         (0x1D)
#define BOARD_ACCEL_I2C_ADDR            (BOARD_ACCEL_ADDR_ACTUAL << 1)

#define BOARD_I2C_GPIO_SCL              GPIO_MAKE_PIN(GPIOC_IDX, 6)
#define BOARD_I2C_GPIO_SDA              GPIO_MAKE_PIN(GPIOC_IDX, 7)
#define BOARD_I2C_DELAY \
    do \
    { \
        int32_t i; \
        for (i = 0; i < 500; i++) \
        { \
            __asm("nop"); \
        } \
    } while (0)

/* The instances of peripherals used for dac_adc_demo */
#define BOARD_DAC_DEMO_DAC_INSTANCE     0U
#define BOARD_DAC_DEMO_ADC_INSTANCE     0U
#define BOARD_DAC_DEMO_ADC_CHANNEL      5U

/* The i2c instance used for i2c communication demo */
#define BOARD_I2C_COMM_INSTANCE         0

/* The Flextimer instance/channel used for board */
#define BOARD_FTM_INSTANCE              0
#define BOARD_FTM_CHANNEL               1

/* ADC0 input channel */
#define BOARD_ADC0_INPUT_CHAN           0
/* ADC0 input channel for HW trigger test*/
#define BOARD_ADC_HW_TRIGGER_CHAN       BOARD_ADC0_INPUT_CHAN

/* board led color mapping */
#define BOARD_GPIO_LED_RED              kGpioLED8
#define BOARD_GPIO_LED_GREEN            kGpioLED1
#define BOARD_GPIO_LED_ORANGE           kGpioLED7
#define BOARD_GPIO_LED_YELLOW           kGpioLED2
/* for KV10 twr, we don't have BLUE LED, just use ORANGE instead*/
#define BOARD_GPIO_LED_BLUE             BOARD_GPIO_LED_ORANGE

#define DISABLE_DEBUG_CONSOLE_TX    PORT_HAL_SetMuxMode(PORTB, 17, kPortPinDisabled)
#define DISABLE_DEBUG_CONSOLE_RX    PORT_HAL_SetMuxMode(PORTB, 16, kPortPinDisabled)

#define DISABLE_SW_INTERRUPT        PORT_HAL_SetPinIntMode(PORTA, 4, kPortIntDisabled)
#define DISABLE_SW_PIN              PORT_HAL_SetMuxMode(PORTA, 4, kPortPinDisabled)
#define ENABLE_SW_PIN               PORT_HAL_SetMuxMode(PORTA, 4, kPortMuxAsGpio)

#define LED1_EN  {PORT_HAL_SetMuxMode(PORTC, 1, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[0]);}  /*!< Enable target LED1 */
#define LED2_EN  {PORT_HAL_SetMuxMode(PORTE, 25, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[1]);} /*!< Enable target LED2 */
#define LED3_EN  {PORT_HAL_SetMuxMode(PORTC, 3, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[2]);}  /*!< Enable target LED3 */
#define LED4_EN  {PORT_HAL_SetMuxMode(PORTC, 4, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[3]);}  /*!< Enable target LED4 */
#define LED5_EN  {PORT_HAL_SetMuxMode(PORTD, 4, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[4]);}  /*!< Enable target LED5 */
#define LED6_EN  {PORT_HAL_SetMuxMode(PORTD, 5, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[5]);}  /*!< Enable target LED6 */
#define LED7_EN  {PORT_HAL_SetMuxMode(PORTD, 6, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[6]);}  /*!< Enable target LED7 */
#define LED8_EN  {PORT_HAL_SetMuxMode(PORTD, 7, kPortMuxAsGpio);GPIO_DRV_OutputPinInit(&ledPins[7]);}  /*!< Enable target LED8 */

#define LED1_DIS (PORT_HAL_SetMuxMode(PORTC, 1, kPortPinDisabled)) /*!< Disable target LED1 */
#define LED2_DIS (PORT_HAL_SetMuxMode(PORTE, 25, kPortPinDisabled))/*!< Disable target LED2 */
#define LED3_DIS (PORT_HAL_SetMuxMode(PORTC, 3, kPortPinDisabled)) /*!< Disable target LED3 */
#define LED4_DIS (PORT_HAL_SetMuxMode(PORTC, 4, kPortPinDisabled)) /*!< Disable target LED4 */
#define LED5_DIS (PORT_HAL_SetMuxMode(PORTD, 4, kPortPinDisabled)) /*!< Disable target LED5 */
#define LED6_DIS (PORT_HAL_SetMuxMode(PORTD, 5, kPortPinDisabled)) /*!< Disable target LED6 */
#define LED7_DIS (PORT_HAL_SetMuxMode(PORTD, 6, kPortPinDisabled)) /*!< Disable target LED7 */
#define LED8_DIS (PORT_HAL_SetMuxMode(PORTD, 7, kPortPinDisabled)) /*!< Disable target LED8 */

#define LED1_OFF (GPIO_DRV_WritePinOutput(ledPins[0].pinName, 1))       /*!< Turn off target LED1 */
#define LED2_OFF (GPIO_DRV_WritePinOutput(ledPins[1].pinName, 1))       /*!< Turn off target LED2 */
#define LED3_OFF (GPIO_DRV_WritePinOutput(ledPins[2].pinName, 1))       /*!< Turn off target LED3 */
#define LED4_OFF (GPIO_DRV_WritePinOutput(ledPins[3].pinName, 1))       /*!< Turn off target LED4 */
#define LED5_OFF (GPIO_DRV_WritePinOutput(ledPins[4].pinName, 1))       /*!< Turn off target LED5 */
#define LED6_OFF (GPIO_DRV_WritePinOutput(ledPins[5].pinName, 1))       /*!< Turn off target LED6 */
#define LED7_OFF (GPIO_DRV_WritePinOutput(ledPins[6].pinName, 1))       /*!< Turn off target LED7 */
#define LED8_OFF (GPIO_DRV_WritePinOutput(ledPins[7].pinName, 1))       /*!< Turn off target LED8 */

#define LED1_ON (GPIO_DRV_WritePinOutput(ledPins[0].pinName, 0))        /*!< Turn on target LED1 */
#define LED2_ON (GPIO_DRV_WritePinOutput(ledPins[1].pinName, 0))        /*!< Turn on target LED2 */
#define LED3_ON (GPIO_DRV_WritePinOutput(ledPins[2].pinName, 0))        /*!< Turn on target LED3 */
#define LED4_ON (GPIO_DRV_WritePinOutput(ledPins[3].pinName, 0))        /*!< Turn on target LED4 */
#define LED5_ON (GPIO_DRV_WritePinOutput(ledPins[4].pinName, 0))        /*!< Turn on target LED5 */
#define LED6_ON (GPIO_DRV_WritePinOutput(ledPins[5].pinName, 0))        /*!< Turn on target LED6 */
#define LED7_ON (GPIO_DRV_WritePinOutput(ledPins[6].pinName, 0))        /*!< Turn on target LED7 */
#define LED8_ON (GPIO_DRV_WritePinOutput(ledPins[7].pinName, 0))        /*!< Turn on target LED8 */

#define OFF_ALL_LEDS  \
                           LED1_OFF;\
                           LED2_OFF;\
                           LED3_OFF;\
                           LED4_OFF;\
                           LED5_OFF;\
                           LED6_OFF;\
                           LED7_OFF;\
                           LED8_OFF;
/* The CMP instance used for board. */
#define BOARD_CMP_INSTANCE              0
/* The CMP channel used for board. */
#define BOARD_CMP_CHANNEL               0

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void hardware_init(void);
void dbg_uart_init(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __BOARD_H__ */
