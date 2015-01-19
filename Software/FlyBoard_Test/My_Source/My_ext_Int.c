/*
 * My_ext_Int.c
 *
 *  Created on: 2014-8-16
 *      Author: FGZ
 */
#include "Common.h"
#include "My_ext_Int.h"
#include "LED.h"


void ext_IntHandler(void)
{
#ifdef USE_MCU1
    uint32_t status;
    status = GPIOIntStatus(GPIO_PORTB_BASE, true);   // get the intStatus flag
    GPIOIntClear(GPIO_PORTB_BASE, status);           // clear intstatus flag

    if(status & GPIO_INT_PIN_4)                         // camera FRAME interrupt
    {
        GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_5); // enable camera LINE interrupt
        GPIOIntDisable(GPIO_PORTB_BASE, GPIO_INT_PIN_4);// disable camera FRAME interrupt
        //M1_LED_ON;
    }
    else if(status & GPIO_INT_PIN_5)                   // camera LINE interrupt
    {
        M1_LED_ON;
    }
#endif
#ifdef USE_MCU2
    uint32_t status;
    status = GPIOIntStatus(GPIO_PORTE_BASE, true);   // get the intStatus flag
    GPIOIntClear(GPIO_PORTE_BASE, status);           // clear int status flag

    if(status & GPIO_INT_PIN_3)
    {
        M2_LED3_ON;
        GPIOIntEnable(GPIO_PORTE_BASE,GPIO_INT_PIN_2);
    }
    else if(status & GPIO_INT_PIN_2)
    {
        M2_LED4_ON;
    }
#endif
}

void init_ext_Interrupt(void)
{
#ifdef USE_MCU1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_INT_PIN_4, GPIO_RISING_EDGE);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_INT_PIN_5, GPIO_RISING_EDGE);
    IntRegister(INT_GPIOB, ext_IntHandler);
    GPIOIntEnable(GPIO_PORTB_BASE,GPIO_INT_PIN_4);
    //GPIOIntEnable(GPIO_PORTB_BASE,GPIO_INT_PIN_5);
    IntEnable(INT_GPIOB);
    IntMasterEnable();
#endif
#ifdef USE_MCU2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x00);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);

    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_INT_PIN_3, GPIO_RISING_EDGE);
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_INT_PIN_2, GPIO_RISING_EDGE);
    IntRegister(INT_GPIOE, ext_IntHandler);
    GPIOIntEnable(GPIO_PORTE_BASE,GPIO_INT_PIN_3);
    //GPIOIntEnable(GPIO_PORTE_BASE,GPIO_INT_PIN_2);
    IntEnable(INT_GPIOE);
    IntMasterEnable();
    //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);
    //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x08);
#endif
}


