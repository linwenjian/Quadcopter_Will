/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Standard C Included Files
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "fsl_cadc_driver.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define CADC_INSTANCE       0U

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////

// Four kinds of structures to configure Cyclic ADC with conversion sequence.
cadc_user_config_t g_AdcUserConfigStruct;
cadc_chn_config_t g_AdcChnConfigStruct;
cadc_conv_config_t g_AdcConvConfigStruct;
cadc_slot_config_t g_AdcSlotConfigStruct;

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////

int main(void)
{
    volatile int tmp;
    uint8_t rx_char = 0x0;
    uint32_t cadc_instance = CADC_INSTANCE;
    uint32_t i = 0, slot_id = 0;

    hardware_init();
    dbg_uart_init();

    CADC_DRV_StructInitUserConfigDefault(&g_AdcUserConfigStruct);
    CADC_DRV_Init(cadc_instance, &g_AdcUserConfigStruct);

    // Configure ADC sample input channel.
    g_AdcChnConfigStruct.diffEnable = true;
    g_AdcChnConfigStruct.diffSelMode = kCAdcChnSelBoth;
    g_AdcChnConfigStruct.gainMode = kCAdcSGainBy1;
    for (i = kCAdcDiffChnANA0_1; i <= kCAdcDiffChnANB6_7; i++)
        CADC_DRV_ConfigSampleChn(cadc_instance, kCAdcDiffChnANA0_1, &g_AdcChnConfigStruct);

    // Configure ADC converters.
    g_AdcConvConfigStruct.dmaEnable = false;
    g_AdcConvConfigStruct.stopEnable = false; // Ungate the converter.
    g_AdcConvConfigStruct.syncEnable = false; // Software trigger only.
    g_AdcConvConfigStruct.endOfScanIntEnable = false; // No interrupt.
    g_AdcConvConfigStruct.convIRQEnable = false;
    g_AdcConvConfigStruct.clkDivValue = 0x4U;
    g_AdcConvConfigStruct.powerOnEnable = true; // Power on the converter.
    g_AdcConvConfigStruct.useChnInputAsVrefH = false;
    g_AdcConvConfigStruct.useChnInputAsVrefL = false;
    g_AdcConvConfigStruct.speedMode = kCAdcConvClkLimitBy25MHz;
    g_AdcConvConfigStruct.sampleWindowCount = 0U;
    CADC_DRV_ConfigConverter(cadc_instance, kCAdcConvA, &g_AdcConvConfigStruct);
    CADC_DRV_ConfigConverter(cadc_instance, kCAdcConvB, &g_AdcConvConfigStruct);
    while (!CADC_DRV_GetConvFlag(cadc_instance, kCAdcConvA, kCAdcConvPowerUp) ) {}
    while (!CADC_DRV_GetConvFlag(cadc_instance, kCAdcConvB, kCAdcConvPowerUp) ) {}


    // Configure slot in conversion sequence.
    // Common setting.
    g_AdcSlotConfigStruct.zeroCrossingMode = kCAdcZeroCrossingDisable;
    g_AdcSlotConfigStruct.lowLimitValue = 0U;
    g_AdcSlotConfigStruct.highLimitValue = 0xFFFU;
    g_AdcSlotConfigStruct.offsetValue = 0U;
    g_AdcSlotConfigStruct.syncPointEnable = false;
    g_AdcSlotConfigStruct.scanIntEnable = false;

    for (i = kCAdcDiffChnANA0_1; i <= kCAdcDiffChnANB6_7; i++)
    {
        // For each slot in conversion sequence.
        g_AdcSlotConfigStruct.diffChns = (cadc_diff_chn_mode_t)i;
        g_AdcSlotConfigStruct.diffSel = kCAdcChnSelBoth;
        g_AdcSlotConfigStruct.slotEnable = true;
        CADC_DRV_ConfigSeqSlot(cadc_instance, slot_id++, &g_AdcSlotConfigStruct);
    }

    // Trigger the conversion sequence.
    CADC_DRV_SoftTriggerConv(cadc_instance, kCAdcConvA);

    for(;;)
    {
        printf("Press any key to start conversion...\r\n");
        // Wait until UART get 1 character, then print out conversion's result.
        rx_char = getchar();
        for (i = 0; i < slot_id; i++)
        {
           // Wait the data to be ready.
           while (!CADC_DRV_GetSlotFlag(cadc_instance, i, kCAdcSlotReady)) { }
           // Get result.
           tmp = CADC_DRV_GetSeqSlotConvValueSigned(cadc_instance, i);
           printf("ADC Slot %2d value: %d\r\n", i, tmp);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
