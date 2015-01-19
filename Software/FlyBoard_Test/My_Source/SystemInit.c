/*
 * SystemInit.c
 *
 *  Created on: 2014-7-30
 *      Author: FGZ
 */
#include "MyHeader.h"

float pitch, roll, yaw;     // extern variable

void SystemInit(void)
{
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    McuClockSetPLL80MHz;

#ifdef UNLOCK_PF0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R = 0x01;               // PF0 UNLOCK
#endif

#ifdef UNLOCK_PD7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTD_CR_R = 0X80;               // PD7 UNLOCK
#endif
    init_Control();
    init_gpio();
    //init_ext_Interrupt();
#ifdef USE_MCU1
    //OLED_Init();
    //init_ADC();
    init_PWM();
    enable_PWM();
    //init_DMA();
#endif
    init_key_gpio();

    //init_Timer1B();
    //cameraPortInit();
    i2c_init();
    init_MPU6050();
    spi_gpio_init(1);
    NRF_init(RX_MODE);
    init_My_UART();
    init_Timer0A();
    enable_timerA();
}
