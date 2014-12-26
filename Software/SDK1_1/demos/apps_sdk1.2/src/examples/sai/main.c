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
///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
// Standard C Included Files
#include <stdio.h>
#include <string.h>
// Standard C Included Files
#include "fsl_sai_driver.h"
#include "board.h"
#include "fsl_os_abstraction.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#define BUFFER_SIZE                 (64U)
#define SAI_INSTANCE                (0U)

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
uint32_t rxFinish       = 0;
I2S_Type * saiBaseAddr    = (I2S_Type*)I2S0_BASE;

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////

/*!
* @brief Rx callback function
*/
void rx_callback(uint32_t *callParam)
{
    *callParam = 1;
    SAI_HAL_RxSetIntCmd(saiBaseAddr, kSaiIntrequestFIFORequest, false);
}

/*!
* @brief SAI in loopback mode.
*
* This function demostrates how to use SAI to transfer a buffer
* in loopback mode.
*/
int32_t main()
{
    uint32_t i = 0;
    uint8_t txBuffer[BUFFER_SIZE];
    uint8_t rxBuffer[BUFFER_SIZE];
    static sai_user_config_t txConfig;
    static sai_user_config_t rxConfig;
    static sai_state_t txState;
    static sai_state_t rxState;
    static sai_data_format_t format;
#if (FSL_FEATURE_SAI_FIFO_COUNT == 1)
    uint32_t data = (txBuffer[0] | (txBuffer[1] << 8));
#endif

    // Init hardware
    hardware_init();
    // Init pins for SAI module.
    configure_i2s_pins(SAI_INSTANCE);
    // Call this function to initialize the console UART.  This function
    // enables the use of STDIO functions (printf, scanf, etc.)
    dbg_uart_init();

    printf("SAI loopback example\n\r");
    // Initalize the buffer value.
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        txBuffer[i] = i;
    }
    // Get default setting of SAI.
    SAI_DRV_TxGetDefaultSetting(&txConfig);
    SAI_DRV_RxGetDefaultSetting(&rxConfig);

    while(1)
    {
        // Init SAI
        SAI_DRV_TxInit(SAI_INSTANCE,&txConfig, &txState);
        SAI_DRV_RxInit(SAI_INSTANCE,&rxConfig, &rxState);

        // Config the format, tx and rx use the same format.
        format.bits        = 16;
        format.mono_streo  = kSaiStreo;
        format.sample_rate = 44100;
        format.mclk        = format.sample_rate * 384;

        SAI_DRV_TxConfigDataFormat(SAI_INSTANCE, &format);
        SAI_DRV_RxConfigDataFormat(SAI_INSTANCE, &format);

        // Register callback function for rx
        SAI_DRV_RxRegisterCallback(SAI_INSTANCE, (sai_callback_t)rx_callback, (void*)&rxFinish);

    #if (FSL_FEATURE_SAI_FIFO_COUNT > 1)
        // Test interrupt way
        SAI_DRV_SendData(SAI_INSTANCE, txBuffer,BUFFER_SIZE);
        SAI_DRV_ReceiveData(SAI_INSTANCE, rxBuffer,BUFFER_SIZE);
        SAI_DRV_TxSetIntCmd(SAI_INSTANCE, true);
        SAI_DRV_RxSetIntCmd(SAI_INSTANCE, true);

        // Wait rx finished
        while(!rxFinish);
        rxFinish = 0;
    #else
        // Start tx and rx
        SAI_DRV_TxStartModule(SAI_INSTANCE);
        SAI_DRV_RxStartModule(SAI_INSTANCE);
        // First fill the TX FIFO, as FIFO length only 1.
        SAI_HAL_SendData(saiBaseAddr, txConfig.channel , data);
        for (i = 0; i < BUFFER_SIZE/2; i++)
        {
            SAI_DRV_SendDataBlocking(SAI_INSTANCE, &txBuffer[(i + 1) * 2], 2);
            SAI_DRV_ReceiveDataBlocking(SAI_INSTANCE, &rxBuffer[i * 2], 2);
        }
    #endif
        // Check if the data is transferred successfully.
        for ( i = 0; i < BUFFER_SIZE; i++)
        {
            if (txBuffer[i]  != rxBuffer[i])
            {
                printf("SAI loopback failed!\n\r");
                return -1;
            }
        }

        memset(rxBuffer, 0, sizeof(rxBuffer));

        // Print the successful message.
        printf("SAI loopback Succeed!\n\n\r");

        // Deinit module
        SAI_DRV_TxDeinit(SAI_INSTANCE);
        SAI_DRV_RxDeinit(SAI_INSTANCE);

        printf("Press any key to run again.\n\r");
        getchar();
    }
}

