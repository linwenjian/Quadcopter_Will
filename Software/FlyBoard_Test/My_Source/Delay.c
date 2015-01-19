/*
 * Delay.c
 *
 *  Created on: 2014-7-30
 *      Author: FGZ
 */
#include "Common.h"
#include "Delay.h"

void simple_Dealy(uint32_t times)
{
  for( ; times > 0 ; times--)
    {
      ROM_SysCtlDelay(5000);
    }
}

void delay_ms(uint32_t delayVal)
{
    volatile uint32_t x, y;
    for(x = ROM_SysCtlClockGet() / 12000 * delayVal; x > 0; x--){}
}

void delay_us(uint32_t delayVal)
{
    volatile uint32_t x;
    for(x = ROM_SysCtlClockGet() / 12000000 * delayVal; x > 0; x--){}
}

void delay_1us(void)
{
    volatile uint32_t x;
    for(x = ROM_SysCtlClockGet() / 12000000; x > 0; x--){}
}
void delay_2us(void)
{
    volatile uint32_t x;
    for(x = ROM_SysCtlClockGet() / 12000000 * 2; x > 0; x--){}
}
void delay_3us(void)
{
    volatile uint32_t x;
    for(x = ROM_SysCtlClockGet() / 12000000 * 3; x > 0; x--){}
}
void delay_4us(void)
{
    volatile uint32_t x;
    for(x = ROM_SysCtlClockGet() / 12000000 * 4; x > 0; x--){}
}
void delay_5us(void)
{
    volatile uint32_t x;
    for(x = ROM_SysCtlClockGet() / 12000000 * 5; x > 0; x--){}
}
