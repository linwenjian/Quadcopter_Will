/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : pin_mux.c
**     Project     : twrk64f120m-pin
**     Processor   : MK64FN1M0VMD12
**     Component   : PinSettings
**     Version     : Component 01.007, Driver 1.2, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-06-08, 20:07, # CodeGen: 9
**     Abstract    :
**
**     Settings    :
**
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc.
**     All Rights Reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file pin_mux.c
** @version 1.2
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
#include "fsl_sim_hal.h"
#include "pin_mux.h"

void configure_ftm_pins(uint32_t instance)
{
    switch(instance) {
        case 0:                       /* FTM0 */
            /* Affects PORTC_PCR1 register */
            PORT_HAL_SetMuxMode(PORTC,1u,kPortMuxAlt4);
            /* Affects PORTE_PCR25 register */
            PORT_HAL_SetMuxMode(PORTE,25u,kPortMuxAlt3);
            /* Affects PORTC_PCR3 register */
            PORT_HAL_SetMuxMode(PORTC,3u,kPortMuxAlt4);
            /* Affects PORTC_PCR4 register */
            PORT_HAL_SetMuxMode(PORTC,4u,kPortMuxAlt4);
            /* Affects PORTD_PCR4 register */
            PORT_HAL_SetMuxMode(PORTD,4u,kPortMuxAlt4);
            /* Affects PORTD_PCR5 register */
            PORT_HAL_SetMuxMode(PORTD,5u,kPortMuxAlt4);
          break;
          case 1:                       /* FTM1 */
              /* Affects PORTD_PCR6 register */
              PORT_HAL_SetMuxMode(PORTD,6u,kPortMuxAlt5);
              /* Affects PORTD_PCR7 register */
              PORT_HAL_SetMuxMode(PORTD,7u,kPortMuxAlt5);
          break;
          case 2:                       /* FTM2 */
              /* Affects PORTA_PCR1 register */
              PORT_HAL_SetMuxMode(PORTA,1u,kPortMuxAlt3);
              /* Affects PORTA_PCR2 register */
              PORT_HAL_SetMuxMode(PORTA,2u,kPortMuxAlt3);
          break;
    default:
      break;
  }
}

void configure_gpio_pins(uint32_t instance)
{
  switch(instance) {
    case PORTA_IDX:                      /* PORTA_IDX */
      /* Affects PORTA_PCR2 register */
      PORT_HAL_SetMuxMode(PORTA,2u,kPortMuxAsGpio);
      /* Affects PORTA_PCR4 register */
      PORT_HAL_SetMuxMode(PORTA,4u,kPortMuxAsGpio);
      break;
    case PORTB_IDX:                      /* PORTB_IDX */
      /* Affects PORTB_PCR0 register */
      PORT_HAL_SetMuxMode(PORTB,0u,kPortMuxAsGpio);
      /* Affects PORTB_PCR3 register */
      PORT_HAL_SetMuxMode(PORTB,3u,kPortMuxAsGpio);
      break;
    case PORTC_IDX:                      /* PORTC_IDX */
      /* Affects PORTC_PCR1 register */
      PORT_HAL_SetDriveStrengthMode(PORTC,1u,kPortLowDriveStrength);
      PORT_HAL_SetMuxMode(PORTC,1u,kPortMuxAsGpio);
      PORT_HAL_SetSlewRateMode(PORTC,1u,kPortSlowSlewRate);
      /* Affects PORTC_PCR3 register */
      PORT_HAL_SetDriveStrengthMode(PORTC,3u,kPortLowDriveStrength);
      PORT_HAL_SetMuxMode(PORTC,3u,kPortMuxAsGpio);
      PORT_HAL_SetSlewRateMode(PORTC,3u,kPortSlowSlewRate);
      /* Affects PORTC_PCR4 register */
      PORT_HAL_SetDriveStrengthMode(PORTC,4u,kPortLowDriveStrength);
      PORT_HAL_SetMuxMode(PORTC,4u,kPortMuxAsGpio);
      PORT_HAL_SetSlewRateMode(PORTC,4u,kPortSlowSlewRate);
      break;
    case PORTD_IDX:                      /* PORTD_IDX */
      /* Affects PORTD_PCR4 register */
      PORT_HAL_SetDriveStrengthMode(PORTD,4u,kPortLowDriveStrength);
      PORT_HAL_SetMuxMode(PORTD,4u,kPortMuxAsGpio);
      PORT_HAL_SetSlewRateMode(PORTD,4u,kPortSlowSlewRate);
      /* Affects PORTD_PCR5 register */
      PORT_HAL_SetDriveStrengthMode(PORTD,5u,kPortLowDriveStrength);
      PORT_HAL_SetMuxMode(PORTD,5u,kPortMuxAsGpio);
      PORT_HAL_SetSlewRateMode(PORTD,5u,kPortSlowSlewRate);
      /* Affects PORTD_PCR6 register */
      PORT_HAL_SetDriveStrengthMode(PORTD,6u,kPortLowDriveStrength);
      PORT_HAL_SetMuxMode(PORTD,6u,kPortMuxAsGpio);
      PORT_HAL_SetSlewRateMode(PORTD,6u,kPortSlowSlewRate);
      /* Affects PORTD_PCR7 register */
      PORT_HAL_SetDriveStrengthMode(PORTD,7u,kPortLowDriveStrength);
      PORT_HAL_SetMuxMode(PORTD,7u,kPortMuxAsGpio);
      PORT_HAL_SetSlewRateMode(PORTD,7u,kPortSlowSlewRate);
    case PORTE_IDX:                      /* PORTE_IDX */
      /* Affects PORTE_PCR25 register */
      PORT_HAL_SetDriveStrengthMode(PORTE,25u,kPortLowDriveStrength);
      PORT_HAL_SetMuxMode(PORTE,25u,kPortMuxAsGpio);
      PORT_HAL_SetSlewRateMode(PORTE,25u,kPortSlowSlewRate);
      break;
    default:
      break;
  }
}

void configure_i2c_pins(uint32_t instance)
{
  switch(instance) {
    case I2C0_IDX:                       /* I2C0 */
      /* Affects PORTC_PCR6 register */
      PORT_HAL_SetMuxMode(PORTC,6u,kPortMuxAlt7);
      /* Affects PORTC_PCR7 register */
      PORT_HAL_SetMuxMode(PORTC,7u,kPortMuxAlt7);
      break;
    default:
      break;
  }
}

void configure_spi_pins(uint32_t instance)
{
  switch(instance) {
    case SPI0_IDX:                       /* SPI0 */
      /* Affects PORTD_PCR2 register */
      PORT_HAL_SetMuxMode(PORTD,2u,kPortMuxAlt2);
      /* Affects PORTD_PCR3 register */
      PORT_HAL_SetMuxMode(PORTD,3u,kPortMuxAlt2);
      /* Affects PORTC_PCR5 register */
      PORT_HAL_SetMuxMode(PORTC,5u,kPortMuxAlt2);
      /* Affects PORTC_PCR0 register */
      PORT_HAL_SetMuxMode(PORTC,0u,kPortMuxAlt7);

      break;
    default:
      break;
  }
}

void configure_uart_pins(uint32_t instance)
{
  switch(instance) {
    case UART0_IDX:                      /* UART0 */
      /* Affects PORTB_PCR16 register */
      PORT_HAL_SetMuxMode(PORTB,16u,kPortMuxAlt3);
      /* Affects PORTB_PCR17 register */
      PORT_HAL_SetMuxMode(PORTB,17u,kPortMuxAlt3);
      break;
    case UART1_IDX:                      /* UART1 */
      /* Affects PORTD_PCR0 register */
      PORT_HAL_SetMuxMode(PORTD,0u,kPortMuxAlt5);
      /* Affects PORTD_PCR1 register */
      PORT_HAL_SetMuxMode(PORTD,1u,kPortMuxAlt5);
      break;
    default:
      break;
  }
}

void configure_gpio_i2c_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTC,6u,kPortMuxAsGpio);
  PORT_HAL_SetMuxMode(PORTC,7u,kPortMuxAsGpio);
}


void configure_cmp_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                      /* CMP0 */
      /* Affects PORTC_PCR5 register -- CMP0_OUT*/
      PORT_HAL_SetMuxMode(PORTC,5u,kPortMuxAlt6);
      /* Affects PORTC_PCR6 register -- CMP0_IN0, KV10 J9-2*/
      PORT_HAL_SetMuxMode(PORTC,6u,kPortPinDisabled);
      /* Affects PORTC_PCR7 register -- CMP0_IN1*/
      PORT_HAL_SetMuxMode(PORTC,7u,kPortPinDisabled);
      break;
    case 1:                      /* CMP1 */
      /* Affects PORTC_PCR2 register -- CMP1_IN0, J6-15*/
      PORT_HAL_SetMuxMode(PORTC,2u,kPortPinDisabled);
      /* Affects PORTC_PCR3 register -- CMP1_IN1*/
      PORT_HAL_SetMuxMode(PORTC,3u,kPortPinDisabled);
      /* Affects PORTC_PCR4 register -- CMP1_OUT*/
      PORT_HAL_SetMuxMode(PORTC,4u,kPortMuxAlt6);
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
**     This file was created by Processor Expert 10.4 [05.10]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
