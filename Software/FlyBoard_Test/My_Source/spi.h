/*
 * spi.h
 *
 *  Created on: 2014-12-5
 *      Author: FGZ
 */

#ifndef SPI_H_
#define SPI_H_

#include "Common.h"
#include "NRF24L01.h"
#include "attitude_Control.h"
#include "math.h"

#define PI 3.14

//--------------------------------------------------------------
#define MISO   (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)!=0)?1:0

#define MOSI_H   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 ,0x02)
#define MOSI_L   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 ,0x00)

#define SCK_H    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 ,0x04)
#define SCK_L    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 ,0x00)

#define CSN_H    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3 ,0x08)
#define CSN_L    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3 ,0x00)

#define CE_H     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 ,0x40)
#define CE_L     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 ,0x00)

#define IRQ_READ (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)!=0)?1:0
//--------------------------------------------------------------

extern void spi_gpio_init(uint8_t int_flag);

#endif /* SPI_H_ */
