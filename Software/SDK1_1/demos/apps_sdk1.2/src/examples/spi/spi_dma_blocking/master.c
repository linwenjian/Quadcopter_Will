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

#include <string.h>
#include "fsl_spi_master_driver.h"

#if FSL_FEATURE_SPI_HAS_DMA_SUPPORT
#include "fsl_spi_dma_master_driver.h"
#endif

#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "board.h"
#include <stdio.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPI_MASTER_INSTANCE         (0)                 /*! User change define to choose SPI instan
ce */
#define TRANSFER_SIZE               (64)
#define TRANSFER_BAUDRATE           (500000U)           /*! Transfer baudrate - 500k */
#define MASTER_TRANSFER_TIMEOUT     (5000U)             /*! Transfer timeout of master - 5s */


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!
 * @brief Buffer for storing data received by the SPI slave driver.
 */
uint8_t s_spiSinkBuffer[TRANSFER_SIZE] = {0};

/*!
 * @brief Buffer that supplies data to be transmitted with the SPI slave driver.
 */
uint8_t s_spiSourceBuffer[TRANSFER_SIZE] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

// Setup the board as a spi master
int main (void)
{
    uint8_t loopCount = 0;
    uint32_t j;
    uint32_t failCount = 0;
    uint8_t * pRxBuff;
    uint32_t calculatedBaudRate;
    spi_master_state_t spiMasterState;
    spi_dma_master_state_t spiDmaMasterState;
    dma_state_t state;
    spi_dma_master_user_config_t userDmaConfig =
    {
    #if FSL_FEATURE_SPI_16BIT_TRANSFERS
        .bitCount       = kSpi8BitMode,
    #endif
        .polarity       = kSpiClockPolarity_ActiveHigh,
        .phase          = kSpiClockPhase_FirstEdge,
        .direction      = kSpiMsbFirst,
        .bitsPerSec     = TRANSFER_BAUDRATE
    };

    // init the hardware, this also sets up up the SPI pins for each specific SoC
    hardware_init();

    dbg_uart_init();

    OSA_Init();

    printf("\r\n SPI board to board dma-blocking example");
    printf("\r\n This example run on instance 0 ");
    printf("\r\n Be sure master's SPI0 and slave's SPI0 are connected ");

    //USER CONFIGURABLE OPTION FOR SPI INSTANCE

    // Configure SPI pin
    configure_spi_pins(SPI_MASTER_INSTANCE);

    printf("\rIMPORTANT, MAKE SURE TO FIRST SET UP THE SPI SLAVE BOARD!\r\n");

    //USER CONFIGURABLE OPTION FOR RXBUFF
    pRxBuff = s_spiSinkBuffer;
    // Set up and init the master
    DMA_DRV_Init(&state);
    // Init the dspi module for eDMA operation
    SPI_DRV_DmaMasterInit(SPI_MASTER_INSTANCE, &spiDmaMasterState);

    SPI_DRV_DmaMasterConfigureBus(SPI_MASTER_INSTANCE,
                                    &userDmaConfig,
                                    &calculatedBaudRate);


    if (calculatedBaudRate > userDmaConfig.bitsPerSec)
    {
        printf("\r**Something failed in the master bus config \r\n");
        return -1;
    }


    printf("\r\nSpi master SYNC test with various baud rates  \r\n");

    printf("\r\nBaudRate set to %dHz\r\n", calculatedBaudRate);
    while(1)
    {
        // Initialize the source buffer
        for (j = 0; j < TRANSFER_SIZE; j++)
        {
            s_spiSourceBuffer[j] = j + loopCount;
        }

        // Reset the sink buffer
        for (j = 0; j < TRANSFER_SIZE; j++)
        {
            s_spiSinkBuffer[j] = 0;
        }
        /* Start the transfer */
        if (SPI_DRV_DmaMasterTransferBlocking(SPI_MASTER_INSTANCE, NULL, s_spiSourceBuffer,
                            NULL, TRANSFER_SIZE, MASTER_TRANSFER_TIMEOUT) == kStatus_SPI_Timeout)
        {
            printf("\r**Sync transfer timed-out \r\n");
        }

        // Delay sometime to wait slave receive and send back data
        OSA_TimeDelay(500U);

        if (SPI_DRV_DmaMasterTransferBlocking(SPI_MASTER_INSTANCE, NULL, NULL,
                            s_spiSinkBuffer, TRANSFER_SIZE, MASTER_TRANSFER_TIMEOUT) == kStatus_SPI_Timeout)
        {
            printf("\r**Sync transfer timed-out \r\n");
        }
        // Verify the contents of the master sink buffer
        // refer to the slave driver for the expected data pattern
        failCount = 0; // reset failCount variable

        for (j = 0; j < TRANSFER_SIZE; j++)
        {
            if (s_spiSinkBuffer[j] != s_spiSourceBuffer[j])
            {
                 failCount++;
            }
        }

        if (failCount == 0)
        {
            printf("\r\nMaster transmit:");
            for (j = 0; j < TRANSFER_SIZE; j++)
            {
                if (j%16 == 0)
                {
                    printf("\r\n    ");
                }
                printf(" %02X", s_spiSourceBuffer[j]);
            }
            printf("\r\nMaster receive:");
            for (j = 0; j < TRANSFER_SIZE; j++)
            {
                if (j%16 == 0)
                {
                    printf("\r\n    ");
                }
                printf(" %02X", s_spiSinkBuffer[j]);
            }
            printf("\r\n");
            printf("\r Spi master blocking transfer succeed! \r\n");
        }
        else
        {
            printf("\r\nSource buffer:");
            for (j = 0; j < TRANSFER_SIZE; j++)
            {
                if (j%16 == 0)
                {
                    printf("\r\n    ");
                }
                printf(" %02X", s_spiSourceBuffer[j]);
            }
            printf("\r\nSink buffer:");
            for (j = 0; j < TRANSFER_SIZE; j++)
            {
                if (j%16 == 0)
                {
                    printf("\r\n    ");
                }
                printf(" %02X", s_spiSinkBuffer[j]);
            }
            printf("\r\n");
            printf("\r **failures detected in Spi master blocking transfer! \r\n");
        }

        // Wait for press any key.
        printf("\r\nPress any key to run again\r\n");
        getchar();
        loopCount++;
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

