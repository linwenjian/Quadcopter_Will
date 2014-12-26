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

/**
 * Usage:
 * - Open one side with ENET_RECEIVE_SIDE macro and another side with ENET_TRANSMIT_SIDE macro.
 */

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "fsl_clock_manager.h"
#include "board.h"
#include "config.h"
#include "fsl_enet_hal.h"
#include "fsl_device_registers.h"
#include "fsl_phy_driver.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define ENET_PACKETS_NUM   (1000)
#define ENET_PHY_REGISTER_NUM    (0x20)

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

uint32_t enet_receive(void *param, enet_mac_packet_buffer_t *packetBuffer);

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////

#if (FSL_RTOS_MQX) && (MQX_COMMON_CONFIG != MQX_LITE_CONFIG)
void Main_Task(uint32_t param);
TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { 1L, Main_Task, ENET_TASK_STACK_SIZE, MQX_MAIN_TASK_PRIORITY, "Main", MQX_AUTO_START_TASK},
   { 0L, 0L,        0L,   0L,                     0L,     0L }
};
#endif

// Ethernet Receive buffer descriptors
#if defined ( __ICCARM__ ) // IAR Compiler
  #pragma data_alignment=ENET_BD_ALIGNMENT
#endif
__ALIGN_BEGIN enet_bd_struct_t RxBuffDescrip[ENET_RXBD_NUM] __ALIGN_END;
// Ethernet Transmit buffer descriptors
#if defined ( __ICCARM__ ) // IAR Compiler
  #pragma data_alignment=ENET_BD_ALIGNMENT
#endif
__ALIGN_BEGIN enet_bd_struct_t TxBuffDescrip[ENET_TXBD_NUM] __ALIGN_END;

// Ethernet Receive data Buffer
#if defined ( __ICCARM__ ) // IAR Compiler
  #pragma data_alignment=ENET_RX_BUFFER_ALIGNMENT
#endif
__ALIGN_BEGIN uint8_t RxDataBuff[ENET_RXBD_NUM][ENET_RXBuffSizeAlign(ENET_RXBUFF_SIZE)] __ALIGN_END;
#if !ENET_RECEIVE_ALL_INTERRUPT
// Ethernet Receive data Buffer
#if defined ( __ICCARM__ ) // IAR Compiler
  #pragma data_alignment=ENET_RX_BUFFER_ALIGNMENT
#endif
__ALIGN_BEGIN uint8_t ExtRxDataBuff[ENET_EXTRXBD_NUM][ENET_RXBuffSizeAlign(ENET_RXBUFF_SIZE)] __ALIGN_END;
#endif
// Ethernet Transmit data Buffer
#if defined ( __ICCARM__ ) // IAR Compiler
  #pragma data_alignment=ENET_TX_BUFFER_ALIGNMENT
#endif
__ALIGN_BEGIN uint8_t TxDataBuff[ENET_TXBD_NUM][ENET_TXBuffSizeAlign(ENET_TXBUFF_SIZE)] __ALIGN_END;
// Ethernet Receive large data Buffer
#if defined ( __ICCARM__ ) // IAR Compiler
  #pragma data_alignment=ENET_RX_BUFFER_ALIGNMENT
#endif
__ALIGN_BEGIN uint8_t RxLargeDataBuff[ENET_RXLARGEBUFF_NUM][ENET_RXBuffSizeAlign(kEnetMaxFrameSize)] __ALIGN_END;

#if defined (FSL_RTOS_FREE_RTOS) || defined (FSL_RTOS_MQX)
// Two-dimentional array for enet interrupt vector number.
extern const IRQn_Type g_enetTxIrqId[];
extern const IRQn_Type g_enetRxIrqId[];
extern const IRQn_Type g_enetTsIrqId[];
#if FSL_RTOS_MQX
extern void MQX_ENET_Receive_IRQHandler(void);
extern void MQX_ENET_Transmit_IRQHandler(void);
extern void MQX_ENET_1588_Timer_IRQHandler(void);
#endif
#endif

void *handle;
static enet_dev_if_t enetDevIf;

uint8_t frame[ENET_DATA_LENGTH] = {0x99,0xAA, 0xBB, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd};
uint8_t Mgframe[ENET_DATA_LENGTH] = {0xff,0xff, 0xff, 0xff, 0xff, 0xff, 0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};
uint8_t *dataBuffQue;
uint8_t  multiGroupAddr[kEnetMacAddrLen] = {0x01,0x00,0x5e,0x00,0x01,0x81};
#if ENET_RECEIVE_SIDE
uint8_t dest[kEnetMacAddrLen] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};
uint8_t source[kEnetMacAddrLen] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x7e};
uint32_t receiveOkNumber = 0;
#endif
#if ENET_TRANSMIT_SIDE
uint8_t source[kEnetMacAddrLen] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};
uint8_t dest[kEnetMacAddrLen] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x7e};
OSA_TASK_DEFINE(enet_send, SEND_TASK_STACK_SIZE);
uint32_t sendNumber = 0;
#endif

uint8_t rxLargeBuffer[kEnetMaxFrameSize];

uint8_t *txBdPtr, *rxBdPtr;
uint8_t *txBuffer, *rxBuffer;

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////

/*!
 * @brief Waiting for link up
 *
 * Indicate status by toggling LED1.
 *
 * @param enetIfPtr The enet context structure
 */
void wait_for_link_up(void *enetIfPtr)
{
    bool status = false;
    uint32_t device, phyAddr;
    device = ((enet_dev_if_t *)enetIfPtr)->deviceNumber;
    phyAddr = ((enet_dev_if_t *)enetIfPtr)->phyAddr;
    while(!status)
    {
        PHY_DRV_GetLinkStatus(device, phyAddr, &status);
        LED1_TOGGLE;
        OSA_TimeDelay(500);
    }
    LED1_ON;
}

/*!
 * @brief Send enet frame
 *
 * This send task will send 1000 frame once the link is up.
 *
 * @param param Task parameter
 */
#if ENET_TRANSMIT_SIDE
void enet_send_task(task_param_t param)
{
    bool status;
    enet_dev_if_t *enetIfPtr = (enet_dev_if_t *)param;
    uint16_t size = 0, dataOffset = 0, bdNumUsed = 0;
    uint8_t *buffer;
    volatile enet_bd_struct_t *curBd;
    uint8_t rx_char;

    // Check out
    if(!param)
    {
        return;
    }

#if USE_RTOS
    while(1)
    {
#endif
        // Add data
        size = ENET_DATA_LENGTH + kEnetEthernetHeadLen;

        // Get the current transmit data buffer in buffer descriptor
        curBd = enetIfPtr->bdContext.txBdCurPtr;
        buffer = ENET_HAL_GetBuffDescripData(curBd);

        // Add Ethernet header
        memcpy(((enet_ethernet_header_t *)buffer)->destAddr, dest, kEnetMacAddrLen);
        memcpy(((enet_ethernet_header_t *)buffer)->sourceAddr, enetIfPtr->macAddr, kEnetMacAddrLen);
       ((enet_ethernet_header_t *)buffer)->type = ENET_HTONS(kEnetProtocolIpv4);

        // Send a whole frame with a signal buffer
        if(size <= enetIfPtr->bdContext.txBuffSizeAlign)
        {
            bdNumUsed = 1;
            // Add Data
            memcpy(buffer + kEnetEthernetHeadLen, frame, ENET_DATA_LENGTH);
            ENET_DRV_SendData(enetIfPtr, size, bdNumUsed);
        }
        else
        {
            // Send a whole frame with multiple buffer descriptors
            while((size - bdNumUsed* enetIfPtr->bdContext.txBuffSizeAlign)> enetIfPtr->bdContext.txBuffSizeAlign)
            {
                if(bdNumUsed == 0)
                {
                    // Add Data
                    memcpy(buffer + kEnetEthernetHeadLen, frame, enetIfPtr->bdContext.txBuffSizeAlign - kEnetEthernetHeadLen);
                    dataOffset += (enetIfPtr->bdContext.txBuffSizeAlign - kEnetEthernetHeadLen);
                }
                else
                {
                    memcpy((void *)buffer, (void *)(frame + dataOffset), enetIfPtr->bdContext.txBuffSizeAlign);
                    // Increment the index and parameters
                    dataOffset += enetIfPtr->bdContext.txBuffSizeAlign;
                }

                // Incremenet the buffer descriptor
                curBd = ENET_DRV_IncrTxBuffDescripIndex(enetIfPtr, curBd);
                buffer = ENET_HAL_GetBuffDescripData(curBd);
                bdNumUsed ++;
            }
             memcpy((void *)buffer, (void *)(frame + dataOffset), size - bdNumUsed* enetIfPtr->bdContext.txBuffSizeAlign);
             bdNumUsed ++;
            // Send packet to the device
            ENET_DRV_SendData(enetIfPtr, size, bdNumUsed);
        }
        sendNumber ++;
        if (sendNumber >= ENET_PACKETS_NUM)
        {
            // Wait for input character then start sending again.
            rx_char = getchar();
            sendNumber = 0;
        }
        LED2_TOGGLE;
        OSA_TimeDelay(100);
#if USE_RTOS
    }
#endif
}
#endif

/*!
 * @brief Initialize enet mac buffer
 *
 * Return Value: The execution status.
 *
 * @param enetIfPtr The enet context structure
 * @param buffCfgPtr Buffer configuration
 */
static uint32_t enet_buffer_init(enet_dev_if_t * enetIfPtr, enet_buff_config_t *buffCfgPtr)
{
    uint32_t rxBufferSizeAlign, txBufferSizeAlign;
    uint8_t  *txBufferAlign, *rxBufferAlign;
    volatile enet_bd_struct_t *txBdPtrAlign, *rxBdPtrAlign;
#if FSL_FEATURE_ENET_SUPPORT_PTP
    enet_mac_ptp_ts_data_t ptpTsRxData[ENET_PTP_RXTS_RING_LEN];
    enet_mac_ptp_ts_data_t ptpTsTxData[ENET_PTP_TXTS_RING_LEN];
#endif

    // Check input parameter
    if((!enetIfPtr) || (!buffCfgPtr))
    {
        return kStatus_ENET_InvalidInput;
    }

    // Allocate ENET receive buffer descriptors
    txBdPtr = (uint8_t *)OSA_MemAllocZero(ENET_TXBD_NUM * sizeof(enet_bd_struct_t) + ENET_BD_ALIGNMENT);
    if (!txBdPtr)
    {
        return kStatus_ENET_MemoryAllocateFail;
    }
   txBdPtrAlign = (volatile enet_bd_struct_t *)ENET_ALIGN((uint32_t)txBdPtr, ENET_BD_ALIGNMENT);

    rxBdPtr = (uint8_t *)OSA_MemAllocZero(ENET_RXBD_NUM * sizeof(enet_bd_struct_t) + ENET_BD_ALIGNMENT);
    if(!rxBdPtr)
    {
         OSA_MemFree(txBdPtr);
         return kStatus_ENET_MemoryAllocateFail;
    }
    rxBdPtrAlign = (volatile enet_bd_struct_t *)ENET_ALIGN((uint32_t)rxBdPtr, ENET_BD_ALIGNMENT);

    // Allocate the transmit and receive date buffers
    rxBufferSizeAlign = ENET_RXBuffSizeAlign(ENET_RXBUFF_SIZE);
    rxBuffer = (uint8_t *)OSA_MemAllocZero(ENET_RXBD_NUM * rxBufferSizeAlign  + ENET_RX_BUFFER_ALIGNMENT);
    if (!rxBuffer)
    {
        OSA_MemFree(txBdPtr);
        OSA_MemFree(rxBdPtr);
        return kStatus_ENET_MemoryAllocateFail;
    }
    rxBufferAlign = (uint8_t *)ENET_ALIGN((uint32_t)rxBuffer, ENET_RX_BUFFER_ALIGNMENT);

    txBufferSizeAlign = ENET_RXBuffSizeAlign(ENET_TXBUFF_SIZE);
    txBuffer = OSA_MemAllocZero(ENET_TXBD_NUM * txBufferSizeAlign + ENET_TX_BUFFER_ALIGNMENT);
    if (!txBuffer)
    {
        OSA_MemFree(txBdPtr);
        OSA_MemFree(rxBdPtr);
        OSA_MemFree(rxBuffer);
        return kStatus_ENET_MemoryAllocateFail;
    }
    txBufferAlign = (uint8_t *)ENET_ALIGN((uint32_t)txBuffer, ENET_TX_BUFFER_ALIGNMENT);

#if FSL_FEATURE_ENET_SUPPORT_PTP
    buffCfgPtr->ptpTsRxDataPtr = &ptpTsRxData[0];
    buffCfgPtr->ptpTsRxBuffNum = ENET_PTP_RXTS_RING_LEN;
    buffCfgPtr->ptpTsTxDataPtr = &ptpTsTxData[0];
    buffCfgPtr->ptpTsTxBuffNum = ENET_PTP_TXTS_RING_LEN;
#endif
    buffCfgPtr->extRxBuffQue = NULL;
    buffCfgPtr->extRxBuffNum = 0;

    buffCfgPtr->rxBdNumber = ENET_RXBD_NUM;
    buffCfgPtr->rxBdPtrAlign = rxBdPtrAlign;
    buffCfgPtr->rxBufferAlign = rxBufferAlign;
    buffCfgPtr->rxBuffSizeAlign = rxBufferSizeAlign;
    buffCfgPtr->txBdNumber = ENET_TXBD_NUM;
    buffCfgPtr->txBdPtrAlign = txBdPtrAlign;
    buffCfgPtr->txBufferAlign = txBufferAlign;
    buffCfgPtr->txBuffSizeAlign = txBufferSizeAlign;

    return kStatus_ENET_Success;
}

/*!
 * @brief De-initialize enet mac buffer
 *
 * Return Value: The execution status.
 *
 * @param enetIfPtr The enet context structure
 */
static uint32_t enet_buffer_deinit(enet_dev_if_t * enetIfPtr)
{
    // Check input parameter
    if(!enetIfPtr)
    {
        return kStatus_ENET_InvalidInput;
    }

    // Free allocated memory
    if(txBdPtr)
    {
        OSA_MemFree(txBdPtr);
    }
    if(rxBdPtr)
    {
        OSA_MemFree(rxBdPtr);
    }
    if(txBuffer)
    {
        OSA_MemFree(txBuffer);
    }
    if(rxBuffer)
    {
        OSA_MemFree(rxBuffer);
    }
#if !ENET_RECEIVE_ALL_INTERRUPT
    if(rxExtBuffer[enetIfPtr->deviceNumber])
    {
        OSA_MemFree(rxExtBuffer[enetIfPtr->deviceNumber]);
    }
#endif
    return kStatus_ENET_Success;
}

/*!
 * @brief Initialize enet driver
 */
uint32_t enet_init(void)
{
    uint32_t result;
    enet_dev_if_t * enetIfPtr;
    uint32_t device = BOARD_DEBUG_ENET_INSTANCE;
    uint32_t phyAddr;
    // MAC configuration
    enet_mac_config_t configMac;
    enet_buff_config_t bufferCfg;
    enet_config_rmii_t rmiiCfg;
    rmiiCfg.duplex = kEnetCfgFullDuplex;
    rmiiCfg.speed = kEnetCfgSpeed100M;
    rmiiCfg.mode = kEnetCfgRmii;
    rmiiCfg.isLoopEnabled = false;
    rmiiCfg.isRxOnTxDisabled = false;
    memset(&configMac, 0,sizeof(enet_mac_config_t));
    configMac.macMode = kEnetMacNormalMode;
    configMac.macAddr = source;
    configMac.rmiiCfgPtr = &rmiiCfg;
    configMac.macCtlConfigure = kEnetRxCrcFwdEnable | kEnetTxCrcBdEnable | kEnetMacEnhancedEnable;

    // Check the device status
    if (enetDevIf.isInitialized)
    {
        return kStatus_ENET_Initialized;
    }

    // Initialize device
    enetIfPtr = (enet_dev_if_t *)&enetDevIf;
    enetIfPtr->deviceNumber = device;
#if ENET_RECEIVE_SIDE
    enetIfPtr->enetNetifcall = enet_receive;
#endif
    OSA_MutexCreate(&enetIfPtr->enetContextSync);

#if defined (FSL_RTOS_FREE_RTOS)
    NVIC_SetPriority(g_enetRxIrqId[enetIfPtr->deviceNumber], 4);
    NVIC_SetPriority(g_enetTxIrqId[enetIfPtr->deviceNumber], 4);
#endif

    // Initialize enet buffers
    result = enet_buffer_init(enetIfPtr, &bufferCfg);
    if(result != kStatus_ENET_Success)
    {
        return result;
    }
    // Initialize enet module
    result = ENET_DRV_Init(enetIfPtr, &configMac, &bufferCfg);
    if (result == kStatus_ENET_Success)
    {
        // Initialize PHY
       if(PHY_DRV_Autodiscover(device, &phyAddr) != kStatus_ENET_Success)
       {
            phyAddr = 0;
       }
       PHY_DRV_Init(device, phyAddr, false);

        handle = enetIfPtr;
        enetIfPtr->isInitialized = true;
    }
    else
    {
        handle = NULL;
        ENET_DRV_Deinit(enetIfPtr);
        enet_buffer_deinit(enetIfPtr);
        OSA_MutexDestroy(&enetIfPtr->enetContextSync);
    }
    return result;
}

/*!
 * @brief enet receive-interrupt callback handler.
 */
#if ENET_RECEIVE_SIDE
uint32_t enet_receive(void *param, enet_mac_packet_buffer_t *packetBuffer)
{
    uint16_t length = 0;
    uint8_t *packet;
    uint16_t result, type;
    uint8_t *data;

    // Check input param
    if((!param) || (!packetBuffer))
    {
        return kStatus_ENET_InvalidInput;
    }

    LED3_TOGGLE;

    // Collect the frame first
    if (!packetBuffer->next)
    {
        packet = packetBuffer->data;  // the frame with only one bd
        length = packetBuffer->length;
    }
    else
    {
        // Dequeue a large  buffer
        packet = rxLargeBuffer;
        do
        {
            if (packetBuffer->length != 0)
            {
                memcpy(packet + length, packetBuffer->data, packetBuffer->length);
                length += packetBuffer->length;
            }
            packetBuffer = packetBuffer->next;

        }while (packetBuffer != NULL);

    }

    // Process the received frame
    type = *(uint16_t *)&((enet_ethernet_header_t *)packet)->type;

    if (ENET_NTOHS(type) == kEnetProtocolIpv4)
    {
        data = (uint8_t *)((uint8_t *)&(((enet_ethernet_header_t *)packet)->type) + 2 );
        length = length - kEnetEthernetHeadLen;
        if (length == ENET_DATA_LENGTH)
        {
            // Compare the received data with send data
            result = memcmp(&frame[0], data, ENET_DATA_LENGTH);
            if (result != 0)
            {
                 printf(" receive error with wrong data! \r\n");
            }
            else
            {
                 receiveOkNumber ++;
                 printf(" receive %d success!\r\n", receiveOkNumber);
            }
       }
    }
    else
    {
         printf(" receive error with wrong ip protocol\r\n");
    }
    return kStatus_ENET_Success;
}
#endif

/*!
 * @brief Initalize hardware.
 */
void app_init_hardware(void)
{
#ifndef FSL_RTOS_MQX
    uint32_t i;

    // Open uart module for debug
    hardware_init();
    configure_enet_pins(BOARD_DEBUG_ENET_INSTANCE);
    dbg_uart_init();
#endif
    // Select PTP timer outclk
    CLOCK_SYS_SetEnetTimeStampSrc(0, kClockTimeSrcOsc0erClk);

    // Disable the mpu
    BW_MPU_CESR_VLD(MPU_BASE, 0);

    // Enable LED indicator
    GPIO_DRV_Init(NULL, ledPins);
}

#if (FSL_RTOS_MQX) && (MQX_COMMON_CONFIG != MQX_LITE_CONFIG)
void Main_Task(uint32_t param)
#else
void main(void)
#endif
{
    uint32_t result;
    app_init_hardware();
    OSA_Init();

    // Initialize enet
    result = enet_init();
    if(result == kStatus_ENET_Success)
    {
        printf(" enet initialize success!\r\n");
    }
    else
    {
        printf(" enet initialize failed!\r\n");
    }

    wait_for_link_up(handle);

#if ENET_TRANSMIT_SIDE
    // Create send task
    OSA_TaskCreate(enet_send_task, "send", SEND_TASK_STACK_SIZE, enet_send_stack,(SEND_TASK_PRIO), handle, false, &enet_send_task_handler);
#endif
    OSA_Start();
}

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
