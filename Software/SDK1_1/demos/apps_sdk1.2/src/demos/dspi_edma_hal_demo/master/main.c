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

#define SPI_BAUDRATE    (3*1000)
#define TEST_DATA_LEN   (64)
#define SPI0_TXCMD      (SPI_PUSHR_PCS(0x10) | SPI_PUSHR_CTAS(0x00))

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief DMA Buffers
uint32_t g_masterRxBuffer[TEST_DATA_LEN];
volatile uint32_t bReceivedFlag = false;
static uint32_t spiPush[TEST_DATA_LEN] = 
{
    0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210,
    0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210,
    0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210,
    0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210,
    0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210,
    0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210,
    0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210,
    0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

extern const IRQn_Type g_edmaIrqId[HW_DMA_INSTANCE_COUNT][FSL_FEATURE_EDMA_MODULE_CHANNEL];

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void dspi_master_setup(uint32_t instance, uint32_t baudrate)
{
    dspi_data_format_config_t config;
    uint32_t baseAddr = SPI0_BASE;
    
    // Enable clock
    CLOCK_SYS_EnableSpiClock(instance);
    
    // Enable HAL
    DSPI_HAL_Init(baseAddr);
    DSPI_HAL_SetMasterSlaveMode(baseAddr, kDspiMaster);
    
    DSPI_HAL_Enable(baseAddr);
    // Halt SPI first
    DSPI_HAL_StopTransfer(baseAddr);
    
    // PCS polarity
    DSPI_HAL_SetPcsPolarityMode(baseAddr, kDspiPcs4, kDspiPcs_ActiveLow);
    DSPI_HAL_SetContinuousSckCmd(baseAddr, false);
    
    // CTAR
    DSPI_HAL_SetBaudRate(baseAddr, kDspiCtar0, baudrate, 72000000);
    config.bitsPerFrame = 16;
    config.clkPhase = kDspiClockPhase_FirstEdge;
    config.clkPolarity = kDspiClockPolarity_ActiveHigh;
    config.direction = kDspiMsbFirst;
    DSPI_HAL_SetDataFormat(baseAddr, kDspiCtar0, &config);

    // DMA
    DSPI_HAL_SetTxFifoFillDmaIntMode(g_dspiBaseAddr[instance], kDspiGenerateDmaReq, true);
    DSPI_HAL_SetRxFifoDrainDmaIntMode(g_dspiBaseAddr[instance], kDspiGenerateDmaReq, true);
}

void edma_init(uint32_t instance)
{
    CLOCK_SYS_EnableDmaClock(instance);
    EDMA_HAL_Init(DMA_BASE);
    CLOCK_SYS_EnableDmamuxClock(instance);
    DMAMUX_HAL_Init(DMAMUX_BASE);
}


void edma_dspi_tx_setup(uint32_t dmaChl, uint32_t srcAddr)
{
    // eDMA Channel 0 Configuration for transmitting data from DSPI0.
    // Configure the DMAMUX for eDMA channel
    DMAMUX_HAL_SetChannelCmd(DMAMUX_BASE, dmaChl, false);
    DMAMUX_HAL_SetTriggerSource(DMAMUX_BASE, dmaChl, kDmaRequestMux0SPI0Tx&0xFF);
    DMAMUX_HAL_SetChannelCmd(DMAMUX_BASE, dmaChl, true);
    
    // Configure TCD
    EDMA_HAL_HTCDClearReg(DMA_BASE, dmaChl);
    edma_software_tcd_t stcd;
    edma_transfer_config_t config;
    config.destAddr = (uint32_t)&SPI0->PUSHR;
    config.destLastAddrAdjust = 0;
    config.destModulo = kEDMAModuloDisable;
    config.destOffset = 0;
    config.destTransferSize = kEDMATransferSize_4Bytes;
    config.srcAddr = srcAddr;
    config.srcLastAddrAdjust = 0;
    config.srcModulo = kEDMAModuloDisable;
    config.srcOffset = 4;
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


void edma_dspi_rx_setup(uint32_t dmaChl, uint32_t destAddr)
{
    // eDMA Channel 0 Configuration for transmitting data from DSPI0.
    // Configure the DMAMUX for eDMA channel
    DMAMUX_HAL_SetChannelCmd(DMAMUX_BASE, dmaChl, false);
    DMAMUX_HAL_SetTriggerSource(DMAMUX_BASE, dmaChl, kDmaRequestMux0SPI0Rx&0xFF);
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

int main(void)
{
    uint32_t i;
    volatile uint16_t count;

    OSA_Init();
    hardware_init();
    dbg_uart_init();
    configure_spi_pins(HW_SPI0);
    
    printf("dspi_edma_test_master\r\n");

    for(i = 0; i < TEST_DATA_LEN; i++)
    {
        spiPush[i] |= SPI0_TXCMD;
    }
    
    // Setup eDMA
    dspi_master_setup(HW_SPI0, SPI_BAUDRATE);
    edma_init(0);
    edma_dspi_tx_setup(kEDMAChannel0, (uint32_t)&spiPush[0]);
    edma_dspi_rx_setup(kEDMAChannel1, (uint32_t)&g_masterRxBuffer);

    printf("\r\nDemo started...\r\n");
    
    // Enable eDMA channels requests to initiate DSPI transfers.
    EDMA_HAL_SetDmaRequestCmd(DMA_BASE, kEDMAChannel0, true);
    EDMA_HAL_SetDmaRequestCmd(DMA_BASE, kEDMAChannel1, true);
    DSPI_HAL_StartTransfer(SPI0_BASE);
    
    
    printf("waiting transfer complete...\r\n");
    while(bReceivedFlag == false)
    {
        OSA_TimeDelay(300);
    }
    printf("transfer completed\r\n");
    
    // Print result
    printf("data received form slave:\r\n");
    for(i = 0; i < TEST_DATA_LEN; i++)
    {
        printf("%08X\t", (unsigned int)g_masterRxBuffer[i]);
        if((count + 1) % 4 == 0)
        {
            printf("\r\n");
        }
    }
    printf("\r\nEnd of demo.\r\n");
}

/* Interrupt handler */
void DMA0_IRQHandler(void)
{
    EDMA_HAL_SetDmaRequestCmd(DMA_BASE,kEDMAChannel0, false);
    EDMA_HAL_ClearDoneStatusFlag(DMA_BASE, kEDMAChannel0);
    EDMA_HAL_ClearIntStatusFlag(DMA_BASE, kEDMAChannel0);
    bReceivedFlag = true;
}

void DMA1_IRQHandler(void)
{
    EDMA_HAL_SetDmaRequestCmd(DMA_BASE,kEDMAChannel1, false);
    EDMA_HAL_ClearDoneStatusFlag(DMA_BASE, kEDMAChannel1);
    EDMA_HAL_ClearIntStatusFlag(DMA_BASE, kEDMAChannel1);
    bReceivedFlag = true;
}

void SPI0_IRQHandler(void)
{

}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
