/*
 * Keys.c
 *
 *  Created on: 2014-7-29
 *      Author: FGZ
 */
#include "Keys.h"
#include "Common.h"

void init_key_gpio(void)
{
#ifdef USE_MCU1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
#endif
#ifdef USE_MCU2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
#endif
}

// pin include -> GPIO_PIN_0 ~ GPIO_PIN_7
uint8_t get_pin_Val(uint32_t ui32Port, uint8_t pin)
{
    uint8_t pinVal;
    pinVal = 0;
    pinVal |= pin;
    pinVal &= GPIOPinRead(ui32Port,pin);
    pinVal = (pinVal > 0 ? 1 : 0);
    return pinVal;
}

#define LS1 1    // 按键没有按下
#define LS2 2    // 按键按下
#define LS3 3    // 按键松开

uint8_t KeyState = LS1;  // 按键的初始状态 为 S1
uint8_t lastIO = 0x03;
uint8_t nowIO = 0x03;   // 0B00000011
uint8_t KeyVal = 0;
uint8_t tmpp = 0;
uint16_t times = 0;
uint8_t two_flg = 0;
// return 0 NO_KEY ; return 1 KEY1 ; return 2 KEY2 ; return 3 KEY1 and KEY2
uint8_t KeyScan(void)
{
    lastIO = nowIO;
#ifdef USE_MCU1
    nowIO = (MCU1_key1_Val | MCU1_key2_Val << 1);
#endif
#ifdef USE_MCU2
    nowIO = (MCU2_key1_Val | MCU2_key2_Val << 1);
#endif
    switch(KeyState)
    {
    case LS1:
      if(nowIO == 0x03)
        {
            KeyState = LS1;
        }
      else
        {
            KeyState = LS2;
        }
      KeyVal = 0;
      break;
    case LS2:
      if(nowIO != 0x03 && lastIO == nowIO)
        {
            KeyState = LS2;
            if(lastIO == 0)
              {
                times++;
              }
            else
              {
                times = 0;
              }
            if(times > 1)
              {
                two_flg = 1;
              }
            KeyVal = 0;
        }
      else if(nowIO == 0x03 && lastIO != nowIO)
        {
            KeyState = LS3;
            tmpp = lastIO;
            KeyVal = 0;
        }
      else
        {
            KeyState = LS1;
            KeyVal = 0;
        }
      break;
    case LS3:
      KeyState = LS1;
      times = 0;
      if(nowIO == 0x03)
        {
          if(two_flg)
            {
                KeyVal = 3;
                two_flg = 0;
            }
          else
            {
                switch(tmpp)
                {
                case 0:
                  KeyVal = 3;
                  break;
                case 1:
                  KeyVal = 1;
                  break;
                case 2:
                  KeyVal = 2;
                  break;
                }
            }
        }
      else
        {
            KeyVal = 0;
        }
      break;
    }
    return KeyVal;
}
