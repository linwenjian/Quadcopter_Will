/*
 * Common.h
 *
 *  Created on: 2014-7-30
 *      Author: FGZ
 */
#ifndef COMMON_H_
#define COMMON_H_
#define TARGET_IS_BLIZZARD_RB1
#define PART_TM4C123GH6PM
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>
#include <utils/uartstdio.h>
#include <utils/ustdlib.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/debug.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin_map.h>
#include <driverlib/rom.h>
#include <driverlib/uart.h>
#include <driverlib/fpu.h>
#include <driverlib/adc.h>
#include <driverlib/pwm.h>
#include <driverlib/udma.h>
#include <driverlib/interrupt.h>

#define MCU1 1
#define MCU2 2

#define lineMax  70
#define rowsMax  360

#define SIDE_DOWN

#if 1

  #define USE_MCU1

#else

  #define USE_MCU2

#endif

#if 1

  #define UNLOCK_PF0

#endif

#if 1

  #define UNLOCK_PD7

#endif

#ifdef USE_MCU1

  #define M1_LED_ON  ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,0X00)
  #define M1_LED_OFF ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,0x10)

#endif

#ifdef USE_MCU2

  #define M2_LED_ALL_ON ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,0X00)

  #define M2_LED1_ON ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4,0X00)
  #define M2_LED2_ON ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0X00)
  #define M2_LED3_ON ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0X00)
  #define M2_LED4_ON ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,0X00)

  #define M2_LED_ALL_OFF ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,0Xf0)

  #define M2_LED1_OFF ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4,0X10)
  #define M2_LED2_OFF ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0X20)
  #define M2_LED3_OFF ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0X40)
  #define M2_LED4_OFF ROM_GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,0X80)

#endif

#define McuClockSetPLL80MHz SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ)

#endif /* COMMON_H_ */
