/*
 * My_UART.c
 *
 *  Created on: 2014-7-31
 *      Author: FGZ
 */
#include "Common.h"
#include "OLED.h"
#include "My_UART.h"
#include "attitude_Control.h"

union
{
    float f32;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
    uint8_t send_2val[2];
    uint8_t send_4val[4];
}send_muxByte_Tmp;

uint8_t serialFIFO_RX[8];

float radioVal = 0;
uint16_t timess = 0;

extern void rxDataHaHandler(void)
{
    uint8_t rxFlag = 0;
    uint32_t uartState;
    uartState = UARTIntStatus(UART3_BASE, true);
    UARTIntClear(UART3_BASE, uartState);



    if((uartState == UART_INT_RX) || (uartState == UART_INT_RT))
      {
          serialFIFO_RX[0] = UARTCharGet(UART3_BASE);
          serialFIFO_RX[1] = UARTCharGet(UART3_BASE);
          serialFIFO_RX[2] = UARTCharGet(UART3_BASE);
          serialFIFO_RX[3] = UARTCharGet(UART3_BASE);
          serialFIFO_RX[4] = UARTCharGet(UART3_BASE);
          serialFIFO_RX[5] = UARTCharGet(UART3_BASE);
          serialFIFO_RX[6] = UARTCharGet(UART3_BASE);
          serialFIFO_RX[7] = UARTCharGet(UART3_BASE);
#if 1
          rxFlag = get_Radio_Val(serialFIFO_RX, &radioVal);
          PID_Val_Set(rxFlag, radioVal);
          if(timess % 2 == 0)
            {
              M1_LED_ON;
            }
          else
            {
              M1_LED_OFF;
            }
          timess++;
#endif
      }
}

// return 0 failed
// 1 -> P1; 2 -> I1; 3 -> D1;
// 4 -> P2; 5 -> I2; 6 -> D2;
// 11 -> ideal_pit
// 12 -> ideal_rol
// 13 -> ideal_yaw
// 14 -> power
uint8_t get_Radio_Val(uint8_t *rxFlag, float *val)
{
    union
    {
      float Rf32;
      int32_t Ri32;
      uint32_t Ru32;
      uint8_t send_4val[4];
    }Rx_float_Tmp;

    if(rxFlag[0] == 0XAA && rxFlag[1] == 0XAB && rxFlag[2] == 0XAC && rxFlag[3] == 0XAD)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = Rx_float_Tmp.Rf32;
          return 1;
      }
    else if(rxFlag[0] == 0XBA && rxFlag[1] == 0XBB && rxFlag[2] == 0XBC && rxFlag[3] == 0XBD)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = Rx_float_Tmp.Rf32;
          return 2;
      }
    else if(rxFlag[0] == 0XCA && rxFlag[1] == 0XCB && rxFlag[2] == 0XCC && rxFlag[3] == 0XCD)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = Rx_float_Tmp.Rf32;
          return 3;
      }
    else if(rxFlag[0] == 0XDA && rxFlag[1] == 0XDB && rxFlag[2] == 0XDC && rxFlag[3] == 0XDD)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = Rx_float_Tmp.Rf32;
          return 4;
      }
    else if(rxFlag[0] == 0XEA && rxFlag[1] == 0XEB && rxFlag[2] == 0XEC && rxFlag[3] == 0XED)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = Rx_float_Tmp.Rf32;
          return 5;
      }
    else if(rxFlag[0] == 0XFA && rxFlag[1] == 0XFB && rxFlag[2] == 0XFC && rxFlag[3] == 0XFD)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = Rx_float_Tmp.Rf32;
          return 6;
      }
    else if(rxFlag[0] == 0XA0 && rxFlag[1] == 0XA1 && rxFlag[2] == 0XA2 && rxFlag[3] == 0XA3)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = (float)Rx_float_Tmp.Ri32;
          return 11;
      }
    else if(rxFlag[0] == 0XB0 && rxFlag[1] == 0XB1 && rxFlag[2] == 0XB2 && rxFlag[3] == 0XB3)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = (float)Rx_float_Tmp.Ri32;
          return 12;
      }
    else if(rxFlag[0] == 0XC0 && rxFlag[1] == 0XC1 && rxFlag[2] == 0XC2 && rxFlag[3] == 0XC3)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = (float)Rx_float_Tmp.Ri32;
          return 13;
      }
    else if(rxFlag[0] == 0XD0 && rxFlag[1] == 0XD1 && rxFlag[2] == 0XD2 && rxFlag[3] == 0XD3)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = (float)Rx_float_Tmp.Ri32;
          return 14;
      }
    else if(rxFlag[0] == 0X00 && rxFlag[1] == 0X11 && rxFlag[2] == 0X22 && rxFlag[3] == 0X33)
      {
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = (float)Rx_float_Tmp.Rf32;
          return 15;
      }
    else if(rxFlag[0] == 0X33 && rxFlag[1] == 0X22 && rxFlag[2] == 0X11 && rxFlag[3] == 0X00)
      {
#if 0
          Rx_float_Tmp.send_4val[0] = rxFlag[4];
          Rx_float_Tmp.send_4val[1] = rxFlag[5];
          Rx_float_Tmp.send_4val[2] = rxFlag[6];
          Rx_float_Tmp.send_4val[3] = rxFlag[7];
          *val = (float)Rx_float_Tmp.Rf32;
#endif
          *val = 1;
          return 16;
      }
    else
      {
          return 0;
      }
}


void init_My_UART(void)
{
#ifdef USE_MCU1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);

    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    UARTConfigSetExpClk(UART3_BASE, ROM_SysCtlClockGet(), SerialBaudrate, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);

    UARTFIFOLevelSet(UART3_BASE, UART_FIFO_TX2_8, UART_FIFO_RX4_8);

    UARTFIFOEnable(UART3_BASE);

    //IntPrioritySet(UART_INT_RX, 0);

    UARTIntEnable(UART3_BASE, UART_INT_RX);

    UARTIntRegister(UART3_BASE, rxDataHaHandler);

#endif
#ifdef USE_MCU2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);

    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART1_BASE,ROM_SysCtlClockGet(),SerialBaudrate,UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
#endif
}

void MCU1_Send_uin8(uint8_t val)
{
    UARTCharPut(UART3_BASE,val);
}

void MCU2_Send_uin8(uint8_t val)
{
    UARTCharPut(UART1_BASE,val);
}

void Send_i16(uint8_t MCU_NUM , int16_t val)
{
  if(MCU_NUM == MCU1)
    {
      send_muxByte_Tmp.i16 = val;
      MCU1_Send_uin8(send_muxByte_Tmp.send_2val[0]);
      ROM_SysCtlDelay(delayTime);
      MCU1_Send_uin8(send_muxByte_Tmp.send_2val[1]);
      ROM_SysCtlDelay(delayTime);
    }
  else if(MCU_NUM == MCU2)
    {
      send_muxByte_Tmp.i16 = val;
      MCU2_Send_uin8(send_muxByte_Tmp.send_2val[0]);
      ROM_SysCtlDelay(delayTime);
      MCU2_Send_uin8(send_muxByte_Tmp.send_2val[1]);
      ROM_SysCtlDelay(delayTime);
    }
}

void Send_u16(uint8_t MCU_NUM , uint16_t val)
{
  if(MCU_NUM == MCU1)
    {
        send_muxByte_Tmp.u16 = val;
        MCU1_Send_uin8(send_muxByte_Tmp.send_2val[0]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_2val[1]);
        ROM_SysCtlDelay(delayTime);
    }
  else if(MCU_NUM == MCU2)
    {
        send_muxByte_Tmp.u16 = val;
        MCU2_Send_uin8(send_muxByte_Tmp.send_2val[0]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_2val[1]);
        ROM_SysCtlDelay(delayTime);
    }
}

void Send_i32(uint8_t MCU_NUM , int32_t val)
{
  if(MCU_NUM == MCU1)
    {
        send_muxByte_Tmp.i32= val;
        MCU1_Send_uin8(0x55);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(0x0f);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[0]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[1]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[2]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[3]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(0xaa);
        ROM_SysCtlDelay(delayTime);
    }
  else if(MCU_NUM == MCU2)
    {
        send_muxByte_Tmp.i32 = val;
        MCU2_Send_uin8(0x55);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(0x0f);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[0]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[1]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[2]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[3]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(0xaa);
        ROM_SysCtlDelay(delayTime);
    }
}

void Send_u32(uint8_t MCU_NUM , uint32_t val)
{
  if(MCU_NUM == MCU1)
    {
        send_muxByte_Tmp.u32= val;
        MCU1_Send_uin8(0x55);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(0x0f);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[0]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[1]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[2]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[3]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(0xaa);
        ROM_SysCtlDelay(delayTime);
    }
  else if(MCU_NUM == MCU2)
    {
        send_muxByte_Tmp.u32 = val;
        MCU2_Send_uin8(0x55);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(0x0f);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[0]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[1]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[2]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[3]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(0xaa);
        ROM_SysCtlDelay(delayTime);
    }
}

void Send_f32(uint8_t MCU_NUM, float val)
{
  if(MCU_NUM == MCU1)
    {
        send_muxByte_Tmp.f32= val;
        MCU1_Send_uin8(0x55);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(0x0f);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[0]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[1]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[2]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(send_muxByte_Tmp.send_4val[3]);
        ROM_SysCtlDelay(delayTime);
        MCU1_Send_uin8(0xaa);
        ROM_SysCtlDelay(delayTime);
    }
  else if(MCU_NUM == MCU2)
    {
        send_muxByte_Tmp.f32 = val;
        MCU2_Send_uin8(0x55);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(0x0f);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[0]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[1]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[2]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(send_muxByte_Tmp.send_4val[3]);
        ROM_SysCtlDelay(delayTime);
        MCU2_Send_uin8(0xaa);
        ROM_SysCtlDelay(delayTime);
    }
}

// flag = 0x1f ~ 0x6f 上位机上面代表的六条曲线
void sendLineX(uint8_t MCU_NUM, uint8_t flag, float val)
{
    if(MCU_NUM == MCU1)
      {
          send_muxByte_Tmp.f32= val;
          MCU1_Send_uin8(0x55);
          ROM_SysCtlDelay(delayTime);
          MCU1_Send_uin8(flag);
          ROM_SysCtlDelay(delayTime);
          MCU1_Send_uin8(send_muxByte_Tmp.send_4val[0]);
          ROM_SysCtlDelay(delayTime);
          MCU1_Send_uin8(send_muxByte_Tmp.send_4val[1]);
          ROM_SysCtlDelay(delayTime);
          MCU1_Send_uin8(send_muxByte_Tmp.send_4val[2]);
          ROM_SysCtlDelay(delayTime);
          MCU1_Send_uin8(send_muxByte_Tmp.send_4val[3]);
          ROM_SysCtlDelay(delayTime);
          MCU1_Send_uin8(0xaa);
          ROM_SysCtlDelay(delayTime);
      }
    else if(MCU_NUM == MCU2)
      {
          send_muxByte_Tmp.f32 = val;
          MCU2_Send_uin8(0x55);
          ROM_SysCtlDelay(delayTime);
          MCU2_Send_uin8(flag);
          ROM_SysCtlDelay(delayTime);
          MCU2_Send_uin8(send_muxByte_Tmp.send_4val[0]);
          ROM_SysCtlDelay(delayTime);
          MCU2_Send_uin8(send_muxByte_Tmp.send_4val[1]);
          ROM_SysCtlDelay(delayTime);
          MCU2_Send_uin8(send_muxByte_Tmp.send_4val[2]);
          ROM_SysCtlDelay(delayTime);
          MCU2_Send_uin8(send_muxByte_Tmp.send_4val[3]);
          ROM_SysCtlDelay(delayTime);
          MCU2_Send_uin8(0xaa);
          ROM_SysCtlDelay(delayTime);
      }
}
