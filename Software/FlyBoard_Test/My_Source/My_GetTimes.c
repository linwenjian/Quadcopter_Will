/*
 * My_GetTimes.c
 *
 *  Created on: 2014-8-17
 *      Author: FGZ
 */

#include "My_GetTimes.h"

void GetTimes_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC | TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER2_BASE, TIMER_A, 0xFFFFFFFF);          // MAX TIME 53s
    //IntMasterEnable();
    //TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    //IntEnable(TIMER2_BASE);
}

void enable_GetTimes(void)
{
    TimerEnable(TIMER2_BASE, TIMER_A);
}



