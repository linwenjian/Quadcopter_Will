/*
 * LED.c
 *
 *  Created on: 2014-7-29
 *      Author: FGZ
 */
#include "LED.h"
#include "Common.h"
#include "Delay.h"
#include "OLED.h"
void init_gpio(void)
{

#ifdef USE_MCU1
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_4);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  M1_LED_OFF;
#endif

#ifdef USE_MCU2
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
  M2_LED_ALL_OFF;
#endif
}

void blink_led(void)
{

  for(;;)
    {
#ifdef USE_MCU1
      M1_LED_OFF;
      simple_Dealy(200);
      M1_LED_ON;
      simple_Dealy(200);
#endif

#ifdef USE_MCU2
      M2_LED_ALL_OFF;
      simple_Dealy(200);
      M2_LED_ALL_ON;
      simple_Dealy(200);
#endif
    }
}


