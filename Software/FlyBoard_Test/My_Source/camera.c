/*
 * camera.c
 *
 *  Created on: 2014-9-14
 *      Author: FGZ
 */
#include "Common.h"
#include "camera.h"
#include "My_DMA.h"

extern uint8_t cameraData[lineMax][rowsMax];
extern uint8_t sendFlag;
//uint16_t line;
extern uint16_t line;
uint16_t rows;
uint8_t frameCount = 0;
uint8_t *imageTmp, *end;
void camera_IntHandler(void)
{
#ifdef USE_MCU1
    uint32_t status;
    status = GPIOIntStatus(GPIO_PORTB_BASE, true);      // get the intStatus flag
    GPIOIntClear(GPIO_PORTB_BASE, status);              // clear intstatus flag

    if((status & GPIO_INT_PIN_4) == GPIO_INT_PIN_4)                         // camera FRAME interrupt
    {
        if(frameCount > 4)
        {
            GPIOIntDisable(GPIO_PORTB_BASE, GPIO_INT_PIN_4);// disable camera FRAME interrupt
            rows = 0;
            line = 0;
            frameCount = 0;
            GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_5); // enable camera LINE interrupt
        }
        frameCount++;
    }
    else if((status & GPIO_INT_PIN_5) == GPIO_INT_PIN_5)                    // camera LINE interrupt
    {

#ifndef USE_DMA_Camera
#if 1
        for(rows = 0; rows < rowsMax; rows++)
          {
            cameraData[line][rows] = GPIO_PORTD_DATA_R;
          }
#else
        imageTmp = cameraData[line];
        end = cameraData[line] + rowsMax;
        for(; imageTmp < end; imageTmp++)
          {
            *imageTmp = (uint8_t)GPIOPinRead(GPIO_PORTD_BASE, 0xff);
          }
#endif
#else
        getCameraData();
        uDMAChannelEnable(UDMA_CHANNEL_SW);
        uDMAChannelRequest(UDMA_CHANNEL_SW);
#endif
        line++;
        if(line > lineMax)
          {
            GPIOIntDisable(GPIO_PORTB_BASE, GPIO_INT_PIN_5);
            sendFlag = 1;
            M1_LED_ON;
          }
    }
#endif
}

void cameraPortInit(void)
{
#ifdef USE_MCU1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
        | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);                                 //SET CAMERA DATA BITS

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);                           //SET CAMERA INT BITS
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_INT_PIN_4, GPIO_RISING_EDGE);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_INT_PIN_5, GPIO_RISING_EDGE);
    IntRegister(INT_GPIOB, camera_IntHandler);
    //GPIOIntEnable(GPIO_PORTB_BASE,GPIO_INT_PIN_4);
    //GPIOIntEnable(GPIO_PORTB_BASE,GPIO_INT_PIN_5);
    IntEnable(INT_GPIOB);
    IntMasterEnable();
#endif
}
