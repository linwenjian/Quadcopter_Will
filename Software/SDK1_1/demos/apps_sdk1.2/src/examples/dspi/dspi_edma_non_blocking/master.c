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
//  Includes
///////////////////////////////////////////////////////////////////////////////

// Standard C Included Files
#include <stdio.h>
 // SDK Included Files
#include "board.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#include "fsl_edma_driver.h"
#include "fsl_dspi_edma_master_driver.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DSPI_MASTER_INSTANCE        (0)                 /*! User change define to choose DSPI instance */
#define TRANSFER_SIZE               (32)                /*! Transfer size */
#define TRANSFER_BAUDRATE           (500000U)           /*! Transfer baudrate - 500k */

/*!******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t receiveBuffer[TRANSFER_SIZE] = {0};
uint8_t sendBuffer[TRANSFER_SIZE] = {0};

/* Declare a 32-byte aligned software transfer control descriptor (DMA requirement) */
#if defined(__GNUC__) /* For toolchains like Atollic and KDS */
edma_software_tcd_t stcdDspiMasterTest __attribute__((aligned (32)));
#elif defined(__ARMCC_VERSION) /* For toolchains like Keil */
edma_software_tcd_t stcdDspiMasterTest __attribute__((aligned (32)));
#elif defined(__IAR_SYSTEMS_ICC__) /* For toolchains like IAR */
#pragma data_alignment = 32
edma_software_tcd_t stcdDspiMasterTest;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief DSPI master EDMA - non_blocking.
 *
 * Thid function uses DSPI master to send an array to slave
 * and receive the array back from slave,
 * thencompare whether the two buffers are the same.
 */

int main(void)
{
    uint32_t i;
    uint8_t loopCount = 1;
    uint32_t wordsTransfer = 0;
    uint32_t calculatedBaudRate;
    dspi_status_t dspiResult;

    edma_state_t dmaState;
    edma_user_config_t dmaUserConfig;
    dspi_edma_master_state_t edmaMasterState;
    dspi_edma_device_t edmaDevice;
    dspi_edma_master_user_config_t edmaMasterUserConfig = 
    {
        .isChipSelectContinuous     = false,
        .isSckContinuous            = false,
        .pcsPolarity                = kDspiPcs_ActiveLow,
        .whichCtar                  = kDspiCtar0,
        .whichPcs                   = kDspiPcs0
    };

    // Init hardware
    hardware_init();
    // Init OSA layer, used in DSPI_DRV_MasterTransferBlocking.
    OSA_Init();
    // Call this function to initialize the console UART.  This function
    // enables the use of STDIO functions (printf, scanf, etc.)
    dbg_uart_init();
    /* Initialze eDMA driver */
    dmaUserConfig.chnArbitration = kEDMAChnArbitrationRoundrobin;
    EDMA_DRV_Init(&dmaState, &dmaUserConfig);

    // Print a note.
    printf("\r\n DSPI board to board edma-non-blocking example");
    printf("\r\n This example run on instance 0 ");
    printf("\r\n Be sure DSPI0-DSPI0 are connected ");

    // Configure SPI pins.
    configure_spi_pins(DSPI_MASTER_INSTANCE);

    // Setup the configuration.
    edmaDevice.dataBusConfig.bitsPerFrame = 8;
    edmaDevice.dataBusConfig.clkPhase     = kDspiClockPhase_FirstEdge;
    edmaDevice.dataBusConfig.clkPolarity  = kDspiClockPolarity_ActiveHigh;
    edmaDevice.dataBusConfig.direction    = kDspiMsbFirst;

    //Init the dspi module for DMA operation
    dspiResult = DSPI_DRV_EdmaMasterInit(DSPI_MASTER_INSTANCE,
                                         &edmaMasterState,
                                         &edmaMasterUserConfig,
                                         &stcdDspiMasterTest);
    if (dspiResult != kStatus_DSPI_Success)
    {
        printf("\r\nERROR: Can not initialize master driver");
        return -1;
    }

    // Configure baudrate.
    edmaDevice.bitsPerSec = TRANSFER_BAUDRATE;
    dspiResult = DSPI_DRV_EdmaMasterConfigureBus(DSPI_MASTER_INSTANCE,
                                                 &edmaDevice,
                                                 &calculatedBaudRate);
    if (dspiResult != kStatus_DSPI_Success)
    {
        printf("\r\nERROR: failure in config bus ");
        return -1;
    }
    else
    {
        printf("\r\n Transfer at baudrate %lu", calculatedBaudRate);
    }

    while(1)
    {
        // Initialize the source buffer.
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            sendBuffer[i] = i + loopCount;
        }

        // Print out transmit buffer.
        printf("\r\n Master transmit:");
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            // Print 16 numbers in a line.
            if ((i & 0x0F) == 0)
            {
                printf("\r\n    ");
            }
            printf(" %02X", sendBuffer[i]);
        }

        // Reset the sink buffer.
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            receiveBuffer[i] = 0;
        }

        // Send the data.
        dspiResult = DSPI_DRV_EdmaMasterTransfer(DSPI_MASTER_INSTANCE,
                                             NULL,
                                             sendBuffer,
                                             NULL,
                                             TRANSFER_SIZE);
        if (dspiResult != kStatus_DSPI_Success)
        {
            printf("\r\nERROR: send data error ");
            return -1;
        }
        // Wait until the transfer is complete.
        while (DSPI_DRV_EdmaMasterGetTransferStatus(DSPI_MASTER_INSTANCE, &wordsTransfer) == kStatus_DSPI_Busy)
        {}
        // Wait until slave is ready to send.
        OSA_TimeDelay(100);

        // Receive the data.
        dspiResult = DSPI_DRV_EdmaMasterTransfer(DSPI_MASTER_INSTANCE,
                                             NULL,
                                             NULL,
                                             receiveBuffer,
                                             TRANSFER_SIZE);
        if (dspiResult != kStatus_DSPI_Success)
        {
            printf("\r\nERROR: receive data error ");
            return -1;
        }
        // Wait until the transfer is complete.
        while (DSPI_DRV_EdmaMasterGetTransferStatus(DSPI_MASTER_INSTANCE, &wordsTransfer) == kStatus_DSPI_Busy)
        {}

        // Print out receive buffer.
        printf("\r\n Master receive:");
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            // Print 16 numbers in a line.
            if ((i & 0x0F) == 0)
            {
                printf("\r\n    ");
            }
            printf(" %02X", receiveBuffer[i]);
        }

        // Check receiveBuffer.
        for (i = 0; i < TRANSFER_SIZE; ++i)
        {
            if (receiveBuffer[i] != receiveBuffer[i])
            {
                printf("\r\n ERROR: master received incorrect");
                return -1;
            }
        }

        printf("\r\n DSPI Master Sends/ Recevies Successfully");
        // Wait for press any key.
        printf("\r\n Press any key to run again");
        getchar();
        // Increase loop count to change transmit buffer.
        loopCount++;
    }
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
