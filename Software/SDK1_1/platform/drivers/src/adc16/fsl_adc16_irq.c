/*
 * Copyright (c) 2013 -2014, Freescale Semiconductor, Inc.
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

#include "quad_common.h"

extern volatile uint32_t ADC_count_n ;
extern volatile uint32_t ADC_value[];
extern _ADC_Pin_Mux_Table_t kADCPinMuxTable[];
extern volatile bool g_AdcConvIntCompleted;
void ADC_IRQHandler_quadcopter(void)
{
  ADC_value[ADC_count_n] = ADC16_DRV_GetConvValueRAW(kADCPinMuxTable[ADC_count_n].instance,
                                                     kADCPinMuxTable[ADC_count_n].chnGroup);
  g_AdcConvIntCompleted = true;
}

/******************************************************************************
 * IRQ Handlers
 *****************************************************************************/
/* ADC16 IRQ handler that would cover the same name's APIs in startup code */
void ADC0_IRQHandler(void)
{
  ADC_IRQHandler_quadcopter();
}

#if (HW_ADC_INSTANCE_COUNT > 1U)
void ADC1_IRQHandler(void)
{
  ADC_IRQHandler_quadcopter();
}
#endif

#if (HW_ADC_INSTANCE_COUNT > 2U)
void ADC2_IRQHandler(void)
{
    /* Add user-defined ISR for ADC2. */
}
#endif

#if (HW_ADC_INSTANCE_COUNT > 3U)
void ADC3_IRQHandler(void)
{
    /* Add user-defined ISR for ADC3. */
}
#endif

/******************************************************************************
 * EOF
 *****************************************************************************/

