/*
 * My_UART.h
 *
 *  Created on: 2014-7-31
 *      Author: FGZ
 */

#ifndef MY_UART_H_
#define MY_UART_H_

#define S_i16 1
#define S_u16 2
#define S_i32 3
#define S_u32 4
#define S_f32 5
#define delayTime 1
#define SerialBaudrate 115200

// return 0 failed
// 1 -> P1; 2 -> I1; 3 -> D1;
// 4 -> P2; 5 -> I2; 6 -> D2;
// 11 -> ideal_pit
// 12 -> ideal_rol
// 13 -> ideal_yaw
// 14 -> power
uint8_t get_Radio_Val(uint8_t *rxFlag, float *val);


void init_My_UART(void);
void MCU1_Send_uin8(uint8_t val);
void MCU2_Send_uin8(uint8_t val);

extern void rxDataHaHandler(void);
void Send_i16(uint8_t MCU_NUM , int16_t val);
void Send_u16(uint8_t MCU_NUM , uint16_t val);
void Send_i32(uint8_t MCU_NUM , int32_t val);
void Send_u32(uint8_t MCU_NUM , uint32_t val);
void Send_f32(uint8_t MCU_NUM , float val);
void sendLineX(uint8_t MCU_NUM, uint8_t flag, float val);

#endif /* MY_UART_H_ */
