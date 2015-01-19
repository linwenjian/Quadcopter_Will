/*
 * My_DMA.c
 *
 *  Created on: 2014-8-12
 *      Author: FGZ
 */
#include "Common.h"
#include "My_DMA.h"
#include "OLED.h"
#include "camera.h"

#ifdef memory_to_memory
uint8_t rxVal[10] = {0,0,0,0,0,0,0,0,0,0};
uint8_t txVal[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
#endif

uint8_t DMAControlTable[1024] __attribute__ ((aligned(1024)));
uint16_t line;           // extern define in camera.c
void init_DMA(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    uDMAEnable();
    uDMAControlBaseSet(DMAControlTable);
    uDMAChannelAttributeDisable(UDMA_CHANNEL_SW, UDMA_ATTR_ALL);
    uDMAChannelAttributeEnable(UDMA_CHANNEL_SW, UDMA_ATTR_USEBURST);
#ifdef memory_to_memory
    uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_8
        | UDMA_DST_INC_8 | UDMA_ARB_512 | UDMA_NEXT_USEBURST);
    uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_BASIC, txVal, rxVal, TransferNums);
    uDMAChannelEnable(UDMA_CHANNEL_SW);
    uDMAChannelRequest(UDMA_CHANNEL_SW);

    OLED_P6x8Num(2,4,rxVal[0]);
    OLED_P6x8Num(14,4,rxVal[1]);
    OLED_P6x8Num(26,4,rxVal[2]);
    OLED_P6x8Num(38,4,rxVal[3]);
    OLED_P6x8Num(2,5,rxVal[4]);
    OLED_P6x8Num(14,5,rxVal[5]);
    OLED_P6x8Num(26,5,rxVal[6]);
    OLED_P6x8Num(38,5,rxVal[7]);
    OLED_P6x8Num(50,5,rxVal[8]);
    OLED_P6x8Num(62,5,rxVal[9]);
#else
    uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_NONE
        | UDMA_DST_INC_8 | UDMA_ARB_512 | UDMA_NEXT_USEBURST);
#endif
    //uDMAIntRegister(UDMA_INT_SW, getCameraData);
}
void getCameraData(void)
{
    //uDMAIntClear(uDMAIntStatus());
    uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
        (void*)(0x400073FC), cameraData[line], rowsMax);
}
