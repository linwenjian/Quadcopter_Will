/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : pin_mux.c
**     Project     : frdmkl02z48m
**     Processor   : MKL02Z48M
**     Component   : PinSettings
**     Version     : Component 01.002, Driver 1.1, CPU db: 3.00.000
**     Compiler    : IAR ARM C Compiler
**     Date/Time   : 2013-12-10, 14:58, # CodeGen: 7
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         GPIO                - void pin_mux_GPIO(uint32_t instance);
**         I2C                 - void pin_mux_I2C(uint32_t instance);
**         SPI                 - void pin_mux_SPI(uint32_t instance);
**         UART                - void pin_mux_UART(uint32_t instance);
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file pin_mux.c
** @version 1.1
** @brief
**
*/         
/*!
**  @addtogroup pin_mux_module pin_mux module documentation
**  @{
*/         

/* MODULE pin_mux. */

#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "pin_mux.h"


void configure_gpio_pins(uint32_t instance)
{
  switch(instance) {    
    case 0:
	  /* PORTA_PCR10  MMA8451 - INT2 */
      PORT_HAL_SetMuxMode(PORTA,10u,kPortMuxAsGpio);
      break;
    case 1:                             /* PTB */
	  /* PORTB_PCR7 LED1 - Green */
      PORT_HAL_SetMuxMode(PORTB, 7u,kPortMuxAsGpio);
	  /* PORTD_PCR6 LED2 - Red */
      PORT_HAL_SetMuxMode(PORTB, 6u,kPortMuxAsGpio);
	  /* PORTB_PCR10 LED2 - Blue */
      PORT_HAL_SetMuxMode(PORTB,10u,kPortMuxAsGpio);
      break;
    default:
      break;
  }
}

void configure_i2c_pins(uint32_t instance)
{
  switch(instance) {    
    case 0:                          /* I2C0 */
      /* PORTB_PCR3 */
      PORT_HAL_SetMuxMode(PORTB,3u,kPortMuxAlt2);
      PORT_HAL_SetPullCmd(PORTB, 3u, true);
      PORT_HAL_SetPullMode(PORTB, 3u, kPortPullUp);
      PORT_HAL_SetPassiveFilterCmd(PORTB, 3u, false);
      /* PORTB_PCR4 */
      PORT_HAL_SetMuxMode(PORTB,4u,kPortMuxAlt2);
      PORT_HAL_SetPullCmd(PORTB, 4u, true);
      PORT_HAL_SetPullMode(PORTB, 4u, kPortPullUp);
      PORT_HAL_SetPassiveFilterCmd(PORTB, 4u, false);


      break;
    case 1:                          /* I2C1 */
      /* PORTA_PCR8 */
      PORT_HAL_SetMuxMode(PORTA,8u,kPortMuxAlt2);
      PORT_HAL_SetPullCmd(PORTA, 8u, true);
      PORT_HAL_SetPullMode(PORTA, 8u, kPortPullUp);
      PORT_HAL_SetPassiveFilterCmd(PORTA, 8u, false);
      /* PORTA_PCR9 */
      PORT_HAL_SetMuxMode(PORTA,9u,kPortMuxAlt2);
      PORT_HAL_SetPullCmd(PORTA, 9u, true);
      PORT_HAL_SetPullMode(PORTA, 9u, kPortPullUp);
      PORT_HAL_SetPassiveFilterCmd(PORTA, 9u, false);
      break;

    default:
      break;
  }
}

void configure_lpsci_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* LPSCI0 */
      /* PORTB_PCR1 */
      PORT_HAL_SetMuxMode(PORTB,1u,kPortMuxAlt2);
      /* PORTB_PCR2 */
      PORT_HAL_SetMuxMode(PORTB,2u,kPortMuxAlt2);
      break;
    default:
      break;
  }
}

void configure_uart_pins(uint32_t instance)
{
  switch(instance) {    
    case 0:                             /* UART0 */
      /* PORTB_PCR1 */
      PORT_HAL_SetMuxMode(PORTB,1u,kPortMuxAlt2);
      /* PORTB_PCR2 */
      PORT_HAL_SetMuxMode(PORTB,2u,kPortMuxAlt2);
      break;
    default:
      break;
  }
}

void configure_gpio_i2c_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTB,3u,kPortMuxAsGpio);
  PORT_HAL_SetMuxMode(PORTB,4u,kPortMuxAsGpio);
}

void configure_spi_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                          /* SPI0 */
      /* PORTA_PCR6  */
      PORT_HAL_SetMuxMode(PORTA, 6u,kPortMuxAlt3); /* MISO */
      /* PORTA_PCR7  */
      PORT_HAL_SetMuxMode(PORTA, 7u,kPortMuxAlt3); /* MOSI */
      /* PORTB_PCR0  */
      PORT_HAL_SetMuxMode(PORTB, 0u,kPortMuxAlt3); /* SCK */
      /* PORTA_PCR5  */
      PORT_HAL_SetMuxMode(PORTA, 5u,kPortMuxAlt3); /* PCS0 */
      break;
    default:
      break;
  }
}

void configure_tpm_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* TPM0 */
        /* PTB11 TPM0 channel 0 -- GRENN LED*/
        PORT_HAL_SetMuxMode(PORTB,11u,kPortMuxAlt2);
        /* PTB10 TPM0 channel 1 -- RED  LED*/
        PORT_HAL_SetMuxMode(PORTB,10u,kPortMuxAlt2);
      break;
    case 1:                             /* TPM1 */
        /* PTB7  TPM1 channel 0 -- No LED*/
        PORT_HAL_SetMuxMode(PORTB,7u,kPortMuxAlt2);
        /* PTB13 TPM1 channel 1 --BLUE LED */
        PORT_HAL_SetMuxMode(PORTB,13u,kPortMuxAlt2);
      break;
    default:
      break;
  }
}

void configure_cmp_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* CMP0 */
        /* PTA12 CMP0 input channel 0 */
        PORT_HAL_SetMuxMode(PORTA,12u,kPortPinDisabled);
        /* PTA2 CMP0 output */
        PORT_HAL_SetMuxMode(PORTA,2u,kPortMuxAlt2);
        break;
    default:
        break;
    }
}

/* END pin_mux. */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
