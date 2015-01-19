/*
 * My_PWM.c
 *
 *  Created on: 2014-8-10
 *      Author: FGZ
 */
#include "Common.h"
#include "My_PWM.h"

void init_PWM(void)
{
    SysCtlPWMClockSet(SYSCTL_PWMDIV_32);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinConfigure(GPIO_PB6_M0PWM0);
    GPIOPinConfigure(GPIO_PB7_M0PWM1);
    GPIOPinConfigure(GPIO_PC4_M0PWM6);
    GPIOPinConfigure(GPIO_PC5_M0PWM7);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);

    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

#ifdef ESC_50_Hz
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 50000);   // 50Hz
#else
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 6250);   // 400Hz
#endif

    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

#ifdef ESC_50_Hz
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 50000);   // 50Hz
#else
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 6250);   // 400Hz
#endif

#ifdef ESC_50_Hz
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 20);  //start duty cycle 5%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 20);  //start duty cycle 5%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 20);  //start duty cycle 5%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 20);  //start duty cycle 5%
#else
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) * 7 / 16);  //start duty cycle 43.75%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) * 7 / 16);  //start duty cycle 43.75%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) * 7 / 16);  //start duty cycle 43.75%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) * 7 / 16);  //start duty cycle 43.75%
#endif
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
}

void enable_PWM(void)
{
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

uint8_t PWM_duty_Set(uint8_t PWM_n,uint16_t duty)
{
    if(duty < Motor_Closed)
      {
          duty = Motor_Closed;
      }
    else if(duty > Throttle_MAX)
      {
          duty = Throttle_MAX;
      }
    switch(PWM_n)
    {
      case PWM1:
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, duty);
        break;
      case PWM2:
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, duty);
        break;
      case PWM3:
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, duty);
        break;
      case PWM4:
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, duty);
        break;
      default:
        return 0;
    }
    return 1;
}

void setMotorPowerMax(void)
{
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 10);  //start duty cycle 10%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 10);  //start duty cycle 10%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 10);  //start duty cycle 10%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 10);  //start duty cycle 10%
}

void setMotorPowerMin(void)
{
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 20);  //start duty cycle 5%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 20);  //start duty cycle 5%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 20);  //start duty cycle 5%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM0_BASE, PWM_OUT_0) / 20);  //start duty cycle 5%
}
