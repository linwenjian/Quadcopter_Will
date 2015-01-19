/*
 * My_PeriodTimer.c
 *
 *  Created on: 2014-7-30
 *      Author: FGZ
 */
#include "MyHeader.h"

//*****************************************************************************
//
// Counter to count the number of interrupts that have been called.
//
//*****************************************************************************
extern volatile uint32_t timeCounter;
extern uint8_t keyStatus;
extern uint8_t iss;
void init_Timer1B(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);

    ROM_TimerLoadSet(TIMER1_BASE, TIMER_B, ROM_SysCtlClockGet() / 2000);

    ROM_IntMasterEnable();

    ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMB_TIMEOUT);

    ROM_IntEnable(INT_TIMER1B);

    timeCounter = 0;

    ROM_TimerEnable(TIMER1_BASE, TIMER_B);
}

void stop_Timer0B(void)
{
    ROM_IntDisable(INT_TIMER0B);

    ROM_TimerIntDisable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
}

volatile uint32_t disVal = 0;
uint16_t pduty = 2500;
void Timer1BIntHandler(void)
{
    uint32_t intStatus;
    intStatus = TimerIntStatus(TIMER1_BASE, true);
    TimerIntClear(TIMER1_BASE, intStatus);

    //g_ui32Counter++;
    timeCounter++;
    keyStatus = KeyScan();
if(keyStatus == 1)
  {
      iss = 1;
      M1_LED_ON;
      setMotorPowerMax();
      pduty += 50;
  }
else if(keyStatus == 2)
  {
      iss = 2;
      M1_LED_OFF;
      setMotorPowerMin();
  }
#ifdef USE_MCU2
    switch(keyStatus)
    {
    case 0:
      M2_LED1_OFF;
      M2_LED2_OFF;
      break;
    case 1:
      M2_LED1_ON;
      //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x01);
      //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x02);
      break;
    case 2:
      M2_LED2_ON;
      //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x00);
      //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);
      M2_LED4_OFF;
      break;
    case 3:
      disVal++;
      if(disVal % 2 == 0)
        {
          M2_LED3_ON;
          M2_LED4_ON;
        }
      else
        {
          M2_LED3_OFF;
          M2_LED4_OFF;
        }
      break;
    }
#endif
    if(timeCounter == NUMBER_OF_INTS)
    {
        //g_ui32Counter = 0;
        timeCounter = 0;
#if 0
        disVal++;
        if(disVal % 2 == 0)
        M1_LED_ON;
        else
        M1_LED_OFF;
#endif
        //OLED_P6x8Num(2,4,disVal++);
        //stop_Timer0B();
    }
}

void init_Timer0A(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC | TIMER_CFG_A_PERIODIC);
#if 0
    TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() / 80);          // 8ms
#else
    TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() / 100);         // 10ms
#endif

    IntMasterEnable();

    IntPrioritySet(INT_TIMER0A, 0XE0);

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    IntEnable(INT_TIMER0A);
}

void enable_timerA(void)
{
    TimerEnable(TIMER0_BASE, TIMER_A);
}

short gtmp[3];
void Timer0AIntHandler(void)
{
    uint32_t intStatus;
    intStatus = TimerIntStatus(TIMER0_BASE, true);
    TimerIntClear(TIMER0_BASE, intStatus);
    /***********************************************/

    get_AttitudeVal(gtmp);
    control_Attitudes(gtmp);

#if 0
    sendLineX(MCU1, 0X1F, gtmp[0]);
    sendLineX(MCU1, 0X2F, gtmp[1]);
    sendLineX(MCU1, 0X3F, gtmp[2]);

    sendLineX(MCU1, 0X4F, pitch);
    sendLineX(MCU1, 0X5F, roll);
    sendLineX(MCU1, 0X6F, yaw);
#endif
    /***********************************************/
}
