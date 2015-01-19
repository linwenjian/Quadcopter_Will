/*
 * Keys.h
 *
 *  Created on: 2014-7-29
 *      Author: FGZ
 */

#ifndef KEYS_H_
#define KEYS_H_
#include "Common.h"

void init_key_gpio(void);
uint8_t get_pin_Val(uint32_t ui32Port,uint8_t pin);

#define MCU1_key1_Val get_pin_Val(GPIO_PORTE_BASE,GPIO_PIN_0)
#define MCU1_key2_Val get_pin_Val(GPIO_PORTE_BASE,GPIO_PIN_1)

#define MCU2_key1_Val get_pin_Val(GPIO_PORTB_BASE,GPIO_PIN_7)
#define MCU2_key2_Val get_pin_Val(GPIO_PORTF_BASE,GPIO_PIN_4)

uint8_t KeyScan(void);

#endif /* KEYS_H_ */
