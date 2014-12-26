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

/* Setup the board as a spi master */
void main (void)
{
    uint8_t loopCount = 0;
    uint32_t j;
    uint32_t failCount = 0;
    uint32_t spiSourceClock;
    SPI_Type * spiBaseAddr = (SPI_Type*)SPI0_BASE;

    /* init the hardware, this also sets up up the SPI pins for each specific SoC */
    hardware_init();

    dbg_uart_init();

    OSA_Init();

    printf("\r\nSPI board to board polling example");
    printf("\r\nThis example run on instance 0 ");
    printf("\r\nBe sure master's SPI0 and slave's SPI0 are connected\r\n");
    //USER CONFIGURABLE OPTION FOR SPI INSTANCE
    printf("\r[Running Blocking SPI master ] \n ");

    // Configure SPI pin
    configure_spi_pins(SPI_MASTER_INSTANCE);

    printf("\rIMPORTANT, MAKE SURE TO FIRST SET UP THE SPI SLAVE BOARD!\r\n");

    // Initialize the source buffer
    for (j = 0; j < TRANSFER_SIZE; j++)
    {
        s_spiSourceBuffer[j] = j;
    }

    // Reset the sink buffer
    for (j = 0; j < TRANSFER_SIZE; j++)
    {
        s_spiSinkBuffer[j] = 0;
    }

    // Enable clock for SPI
    CLOCK_SYS_EnableSpiClock(SPI_MASTER_INSTANCE);

    // configure the run-time state struct with the source clock value
    spiSourceClock = CLOCK_SYS_GetSpiFreq(SPI_MASTER_INSTANCE);

    // Reset the SPI module to it's default state, which includes SPI disabled
    SPI_HAL_Init(spiBaseAddr);

    // Set SPI to master mode
    SPI_HAL_SetMasterSlave(spiBaseAddr, kSpiMaster);

    // Set slave select to automatic output mode
    SPI_HAL_SetSlaveSelectOutputMode(spiBaseAddr, kSpiSlaveSelect_AutomaticOutput);

    // Set the SPI pin mode to normal mode
    SPI_HAL_SetPinMode(spiBaseAddr, kSpiPinMode_Normal);

#if FSL_FEATURE_SPI_FIFO_SIZE
    if (FSL_FEATURE_SPI_FIFO_SIZEn(SPI_MASTER_INSTANCE) != 0)
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

    // Configure the bus to access the provided device.
    SPI_HAL_SetBaud(spiBaseAddr, TRANSFER_BAUDRATE, spiSourceClock);
    // Setup format as same as slave
    SPI_HAL_SetDataFormat(spiBaseAddr, kSpiClockPolarity_ActiveHigh, kSpiClockPhase_FirstEdge, kSpiMsbFirst);

    while(1)
    {
        // Initialize the source buffer
        for (j = 0; j < TRANSFER_SIZE; j++)
        {
            s_spiSourceBuffer[j] = j+loopCount;
        }

        // Reset the sink buffer
        for (j = 0; j < TRANSFER_SIZE; j++)
        {
            s_spiSinkBuffer[j] = 0;
        }

        // Disable module to clear the shift register
        SPI_HAL_Disable(spiBaseAddr);
        SPI_HAL_Enable(spiBaseAddr);

        for (j = 0; j < TRANSFER_SIZE; j++)
        {
#if FSL_FEATURE_SPI_16BIT_TRANSFERS
            SPI_HAL_WriteDataBlocking(spiBaseAddr, kSpi8BitMode, NULL, s_spiSourceBuffer[j]);
#else
            // Write data to slave
            SPI_HAL_WriteDataBlocking(spiBaseAddr, s_spiSourceBuffer[j]);
#endif
            OSA_TimeDelay(5);
        }

        // Delay sometime to wait slave prepare data
        OSA_TimeDelay(50);

        // Disable module to clear the shift register
        SPI_HAL_Disable(spiBaseAddr);
        SPI_HAL_Enable(spiBaseAddr);

        for (j = 0; j <TRANSFER_SIZE; j++)
        {
#if FSL_FEATURE_SPI_16BIT_TRANSFERS
            // Send dummy byte to receive data from slave
            SPI_HAL_WriteDataBlocking(spiBaseAddr, kSpi8BitMode, NULL, s_spiSourceBuffer[j]);
#else
            // Send dummy byte to receive data from slave
            SPI_HAL_WriteDataBlocking(spiBaseAddr, s_spiSourceBuffer[j]);
#endif
            // Wait for slave send data back
            while(SPI_HAL_IsReadBuffFullPending(spiBaseAddr)==0){}
            // Read data register
#if FSL_FEATURE_SPI_16BIT_TRANSFERS
            s_spiSinkBuffer[j] = SPI_HAL_ReadDataLow(spiBaseAddr);
#else
            s_spiSinkBuffer[j] = SPI_HAL_ReadData(spiBaseAddr);
#endif
            OSA_TimeDelay(5);
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

