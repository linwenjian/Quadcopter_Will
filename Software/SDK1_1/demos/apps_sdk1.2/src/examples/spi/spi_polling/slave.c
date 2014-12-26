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
#include "fsl_spi_slave_driver.h"

#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "board.h"
#include <stdio.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPI_SLAVE_INSTANCE          (0)                 /*! User change define to choose SPI instance */
#define TRANSFER_SIZE               (64)
#define SLAVE_TRANSFER_TIMEOUT      (OSA_WAIT_FOREVER)             /*! Transfer timeout of slave - 5s */
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


/*******************************************************************************
 * Code
 ******************************************************************************/

int main (void)
{
    uint32_t j;
    SPI_Type * spiBaseAddr = (SPI_Type*)SPI0_BASE;

    // init the hardware, this also sets up up the SPI pins for each specific SoC
    hardware_init();

    OSA_Init();

    dbg_uart_init();

    printf("\r\nSPI board to board polling example");
    printf("\r\nThis example run on instance 0 ");
    printf("\r\nBe sure master's SPI0 and slave's SPI0 are connected ");

    //USER CONFIGURABLE OPTION FOR SPI INSTANCE (if applicable)

    // Configure SPI pin
    configure_spi_pins(SPI_SLAVE_INSTANCE);

    // Enable clock for SPI
    CLOCK_SYS_EnableSpiClock(SPI_SLAVE_INSTANCE);

    // Reset the SPI module to its default settings including disabling SPI
    SPI_HAL_Init(spiBaseAddr);

    // Set SPI to slave mode
    SPI_HAL_SetMasterSlave(spiBaseAddr, kSpiSlave);

    // Set the SPI pin mode to normal mode
    SPI_HAL_SetPinMode(spiBaseAddr, kSpiPinMode_Normal);

#if FSL_FEATURE_SPI_FIFO_SIZE
    if (FSL_FEATURE_SPI_FIFO_SIZEn(SPI_SLAVE_INSTANCE) != 0)
    {
        // If SPI module contains a FIFO, disable it and set watermarks to half full/empty
        SPI_HAL_SetFifoMode(spiBaseAddr, false, kSpiTxFifoOneHalfEmpty, kSpiRxFifoOneHalfFull);
    }
#endif

    //USER CONFIGURABLE OPTION FOR 8 or 16-BIT MODE (if applicable)
#if FSL_FEATURE_SPI_16BIT_TRANSFERS
    SPI_HAL_Set8or16BitMode(spiBaseAddr, kSpi8BitMode);
#endif
    // SPI system Enable
    SPI_HAL_Enable(spiBaseAddr);
    // Setup format as same as master
    SPI_HAL_SetDataFormat(spiBaseAddr, kSpiClockPolarity_ActiveHigh, kSpiClockPhase_FirstEdge, kSpiMsbFirst);
    while(1)
    {
        printf("\r\nSlave example is running...");

        // Reset the sink buffer
        for (j = 0; j < TRANSFER_SIZE; j++)
        {
            s_spiSinkBuffer[j] = 0;
        }

        // Receive data from master
        printf("\r\nSPI is waiting to receive data \r\n");

        SPI_HAL_Disable(spiBaseAddr);
        SPI_HAL_Enable(spiBaseAddr);

        // Disable module to clear shift register
        for (j = 0; j <TRANSFER_SIZE; j++)
        {
            // Check if data received
            while(SPI_HAL_IsReadBuffFullPending(spiBaseAddr)==0){}

#if FSL_FEATURE_SPI_16BIT_TRANSFERS
            // Read data from data register
            s_spiSinkBuffer[j] = SPI_HAL_ReadDataLow(spiBaseAddr);
#else
            s_spiSinkBuffer[j] = SPI_HAL_ReadData(spiBaseAddr);
#endif
        }

        // Disable module to clear shift register
        SPI_HAL_Disable(spiBaseAddr);
        SPI_HAL_Enable(spiBaseAddr);

        // Send data back to master
        for (j = 0; j <TRANSFER_SIZE; j++)
        {
#if FSL_FEATURE_SPI_16BIT_TRANSFERS
         SPI_HAL_WriteDataBlocking(spiBaseAddr, kSpi8BitMode, NULL, s_spiSinkBuffer[j]);
#else
         SPI_HAL_WriteDataBlocking(spiBaseAddr, s_spiSinkBuffer[j]);
#endif
        }

        OSA_TimeDelay(50);

        // Print out receive buffer
        printf("\r\nSlave receive:\r\n    ");
        for (j = 0; j < TRANSFER_SIZE; j++)
        {
            if ((j>0) && (j%16 == 0))
            {
                printf("\r\n    ");
            }
            printf(" %02X", s_spiSinkBuffer[j]);
        }
        printf("\r\n");
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

