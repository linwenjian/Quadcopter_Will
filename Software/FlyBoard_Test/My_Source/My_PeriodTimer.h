/*
 * My_PeriodTimer.h
 *
 *  Created on: 2014-7-30
 *      Author: FGZ
 */

#ifndef MY_PERIODTIMER_H_
#define MY_PERIODTIMER_H_
#include "Common.h"
extern volatile uint32_t timeCounter;
extern uint8_t keyStatus;
extern uint8_t iss;
//*****************************************************************************
//
// Number of interrupts before the timer gets turned off.
//
//*****************************************************************************
#define NUMBER_OF_INTS          100

void init_Timer0A(void);
void init_Timer1B(void);
void stop_Timer0B(void);

void enable_timerA(void);



#endif /* MY_PERIODTIMER_H_ */
