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
 *
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "board.h"
#include "fsl_os_abstraction.h"
#include "fsl_interrupt_manager.h"
#include "fsl_clock_manager.h"
#include "fsl_dspi_master_driver.h"
#include "fsl_dspi_slave_driver.h"
#include "fsl_edma_driver.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define SPI_BAUDRATE    (3000U)
#define TEST_DATA_LEN   (64)
#define SPI0_TXCMD      (SPI_PUSHR_PCS(0x00) | SPI_PUSHR_CTAS(0x00))

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

uint32_t g_slaveRxBuffer[TEST_DATA_LEN];
volatile uint32_t bReceivedFlag = false;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

extern const IRQn_Type g_edmaIrqId[HW_DMA_INSTANCE_COUNT][FSL_FEATURE_EDMA_MODULE_CHANNEL];
extern const IRQn_Type g_dspiIrqId[HW_SPI_INSTANCE_COUNT];

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void dspi_slave_setup(uint32_t instance, uint32_t baudrate)
{
    dspi_data_format_config_t config;
    uint32_t baseAddr = SPI0_BASE;
    
    // Enable clock
    CLOCK_SYS_EnableSpiClock(instance);
    
    // Clear flags
    DSPI_HAL_ClearStatusFlag(g_dspiBaseAddr[instance], kDspiTxComplete);
    DSPI_HAL_ClearStatusFlag(g_dspiBaseAddr[instance], kDspiTxAndRxStatus);
    DSPI_HAL_ClearStatusFlag(g_dspiBaseAddr[instance], kDspiEndOfQueue);
    DSPI_HAL_ClearStatusFlag(g_dspiBaseAddr[instance], kDspiTxFifoUnderflow);
    DSPI_HAL_ClearStatusFlag(g_dspiBaseAddr[instance], kDspiTxFifoFillRequest);
    DSPI_HAL_ClearStatusFlag(g_dspiBaseAddr[instance], kDspiRxFifoOverflow);
    DSPI_HAL_ClearStatusFlag(g_dspiBaseAddr[instance], kDspiRxFifoDrainRequest);
    
    // Enable HAL
    DSPI_HAL_Init(baseAddr);
    DSPI_HAL_SetMasterSlaveMode(baseAddr, kDspiSlave);
    DSPI_HAL_Enable(baseAddr);

    // Disable transfer
    DSPI_HAL_StopTransfer(baseAddr);
    
    // PCS popularity
    DSPI_HAL_SetPcsPolarityMode(baseAddr, kDspiPcs0, kDspiPcs_ActiveLow);

    // CTAR
    DSPI_HAL_SetBaudRate(baseAddr, kDspiCtar0, baudrate, 72000000);
    config.bitsPerFrame = 16;
    config.clkPhase = kDspiClockPhase_FirstEdge;
    config.clkPolarity = kDspiClockPolarity_ActiveHigh;
    config.direction = kDspiMsbFirst;
    DSPI_HAL_SetDataFormat(baseAddr, kDspiCtar0, &config);

    // Interrupt
    INT_SYS_EnableIRQ(g_dspiIrqId[instance]);
    DSPI_HAL_SetIntMode(g_dspiBaseAddr[instance], kDspiTxFifoUnderflow, false);
    DSPI_HAL_SetIntMode(g_dspiBaseAddr[instance], kDspiTxFifoFillRequest, false);
    // DMA
    DSPI_HAL_SetTxFifoFillDmaIntMode(g_dspiBaseAddr[instance], kDspiGenerateIntReq, true);
    DSPI_HAL_SetRxFifoDrainDmaIntMode(g_dspiBaseAddr[instance], kDspiGenerateDmaReq, true);
}

void edma_init(void)
{
    CLOCK_SYS_EnableDmaClock(0);
    EDMA_HAL_Init(DMA_BASE);
    CLOCK_SYS_EnableDmamuxClock(0);
    DMAMUX_HAL_Init(DMAMUX_BASE);
}

void edma_dspi_rx_setup(uint32_t dmaChl, uint32_t destAddr)
{
    // Configure the DMAMUX for eDMA channel
    DMAMUX_HAL_SetChannelCmd(DMAMUX_BASE, dmaChl, false);
    DMAMUX_HAL_SetTriggerSource(DMAMUX_BASE, dmaChl, kDmaRequestMux0SPI0Rx & 0xFF);
    DMAMUX_HAL_SetChannelCmd(DMAMUX_BASE, dmaChl, true);
    
    // Configure TCD
    EDMA_HAL_HTCDClearReg(DMA_BASE, dmaChl);
    edma_software_tcd_t stcd;
    edma_transfer_config_t config;
    config.destAddr = destAddr;
    config.destLastAddrAdjust = 0;
    config.destModulo = kEDMAModuloDisable;
    config.destOffset = 4;
    config.destTransferSize = kEDMATransferSize_4Bytes;
    config.srcAddr = (uint32_t)&SPI0->POPR;
    config.srcLastAddrAdjust = 0;
    config.srcModulo = kEDMAModuloDisable;
    config.srcOffset = 0;
    config.srcTransferSize = kEDMATransferSize_4Bytes;
    config.majorLoopCount = TEST_DATA_LEN;
    config.minorLoopCount = 4;
    
    EDMA_HAL_STCDSetBasicTransfer(DMA_BASE, &stcd, &config, true, true);
    EDMA_HAL_PushSTCDToHTCD(DMA_BASE, dmaChl, &stcd);
    
    // Configure interrupt
    IRQn_Type irqNumber;
    irqNumber = g_edmaIrqId[0][dmaChl];
    INT_SYS_EnableIRQ(irqNumber);
}

int main (void)
{
    uint8_t msg;
    uint32_t timeout = 0;
    uint32_t var;
    volatile uint16_t count;

    OSA_Init();

    hardware_init();
    dbg_uart_init();
    configure_spi_pins(HW_SPI0);
    
    printf("dspi_edma_test_slave\r\n");
    printf("\r\nDemo started...\r\n");

    // Initialize configuration
    edma_init();
    edma_dspi_rx_setup(kEDMAChannel2, (uint32_t)&g_slaveRxBuffer);
    dspi_slave_setup(HW_SPI0, SPI_BAUDRATE);
    
    printf("Press space bar to begin.\r\n");
    msg = 'A';
    while(msg != ' ')
    {
        msg = getchar();
    }

    printf("\r\nDemo started...\r\n");
    
    // Slave only have PCS0
    PORT_HAL_SetMuxMode(PORTC_BASE, 0u, kPortMuxAlt7);
    
    // Enable eDMA channels requests to initiate DSPI transfers.
    EDMA_HAL_SetDmaRequestCmd(DMA_BASE, kEDMAChannel2, true);
    DSPI_HAL_StartTransfer(SPI0_BASE);
    
    // Waiting transfer complete
    printf("waiting transfer complete...\r\n");
    while((bReceivedFlag == false) & (timeout < 50))
    {
        OSA_TimeDelay(100);
        timeout++;
    }
    
    if(bReceivedFlag == false)
    {
        printf("No date received, please check connections\r\n");
    }
    
    printf("received data:\r\n");
    
    for(count = 0; count < TEST_DATA_LEN; count++)
    {
        var = g_slaveRxBuffer[count];
        printf("%08X\t", (unsigned int)var);
        if((count + 1) % 4 == 0)
        {
            printf("\r\n");
        }
    }
    printf("\r\nEnd of demo.\r\n");
}

/*! @brief SPI0 IRQ handler with the same name in the startup code*/
void SPI0_IRQHandler(void)
{
    static uint32_t cnt;
    DSPI_HAL_WriteDataSlavemode(SPI0_BASE, cnt++);
    DSPI_HAL_ClearStatusFlag(SPI0_BASE, kDspiTxComplete);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA2_IRQHandler(void)
{
    EDMA_HAL_SetDmaRequestCmd(DMA_BASE, kEDMAChannel2, false);
    EDMA_HAL_ClearDoneStatusFlag(DMA_BASE, kEDMAChannel2);
    EDMA_HAL_ClearIntStatusFlag(DMA_BASE, kEDMAChannel2);
    bReceivedFlag = true;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
