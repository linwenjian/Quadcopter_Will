/*
 * NRF24L01.h
 *
 *  Created on: 2014-12-5
 *      Author: FGZ
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_

#include "Common.h"
#include "spi.h"

typedef union
{
    int16_t dat_16;
    struct
    {
            uint8_t dat1;//低位
            uint8_t dat2;//高位
    }dat_8;
}int16_union,*int16_union_ptr;

//-----------------------------------------------------------------
#define TX_ADR_WIDTH      5  //发送地址校验码宽度(=SETUP_AW)
#define TX_PLOAD_WIDTH    16 //发送有效数据宽度  (=RX_PW_P0)

#define RX_ADR_WIDTH_P0   5  //接收地址校验码宽度(=SETUP_AW)
#define RX_PLOAD_WIDTH_P0 16 //接收有效数据宽度  (=RX_PW_P0)

#define TX_MODE 1
#define RX_MODE 0

#define RX_DR   (sta & 0x40) >> 6
#define TX_DS   (sta & 0x20) >> 5
#define MAX_RT (sta & 0x10) >> 4

#define DYNAMIC_PLOAD_LENGTH   1 //1:DYNAMIC packet length, 0:STATIC
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// nRF24L01) 命令 宏定义
#define READ_REG           0x00  // 1、读寄存器命令
#define WRITE_REG          0x20  // 1、写寄存器命令（只有在掉电和待机模式下可操作这条命令）

#define RD_RX_PLOAD_WID    0x60  // 2、读RX_FIFO数据的宽度(如果宽度>32，必须Flush RX_FIFO)（FEATURE、DYNPD相关功能要使能）
#define RD_RX_PLOAD        0x61  // 2、读RX_FIFO命令
#define WR_TX_PLOAD        0xA0  // 2、写TX_FIFO命令
#define WR_TX_PLOAD_NOACK  0xB0  // 2、写TX_FIFO命令（NO_ACK=1，接收端不发ACK信号）
#define WR_ACK_PLOAD       0xA8  //    RX端写ACK PAYLOAD 数据

#define FLUSH_TX           0xE1  // 3、清TX_FIFO命令
#define FLUSH_RX           0xE2  // 3、清RX_FIFO命令

#define REUSE_TX_PL        0xE3  // 重复装载数据命令

#define NOP                0xFF  // 空操作命令
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// nRF24L01) 寄存器地址 宏定义
#define NRF_CONFIG  0x00  //控制寄存器（中断开关，CRC使能，PWR_UP，PRIM_RX）

#define EN_AA       0x01  //6个接收机的 自动应答 功能 分别使能（ACK使能）

#define EN_RXADDR   0x02  //6个接收机的 接收功能 分别使能

#define SETUP_AW    0x03  //6个接收机的约定 地址校验码 宽度 共同设置（3-5）

#define SETUP_RETR  0x04  //发送机 自动重发 功能设置（重发延时，重发次数）

#define RF_CH       0x05  //发送机、6个接收机 工作频道 共同设置 frequency = 2400 + RF_CH [MHz]

#define RF_SETUP    0x06  //射频控制寄存器（传输速率，发射功率）

#define NRF_STATUS  0x07  //状态寄存器 （RX_DR,TX_DS,MAX_RT,RX_P_NO,TX_FULL）

#define OBSERVE_TX  0x08  //发送监测寄存器（数据包丢失计数器，重发计数器）

#define CD          0x09  //载波检测

#define RX_ADDR_P0  0x0A  //6个接收机的 地址校验码 设置(ADR_WIDTH个)（=TX_ADDR）
#define RX_ADDR_P1  0x0B  //
#define RX_ADDR_P2  0x0C  //
#define RX_ADDR_P3  0x0D  //
#define RX_ADDR_P4  0x0E  //
#define RX_ADDR_P5  0x0F  //

#define TX_ADDR     0x10  //目标接收机的 地址校验码 设置(ADR_WIDTH个)（=RX_ADDR_P0）

#define RX_PW_P0    0x11  //6个接收机的预定 有效数据 宽度 设置
#define RX_PW_P1    0x12  //
#define RX_PW_P2    0x13  //
#define RX_PW_P3    0x14  //
#define RX_PW_P4    0x15  //
#define RX_PW_P5    0x16  //

#define FIFO_STATUS 0x17  //FIFO状态寄存器 (TX_FULL,TX_EMPTY,RX_FULL,RX_EMPTY)

#define DYNPD       0x1C  //6个接收机的 动态负载长度功能 分别使能 （才可以通过 RD_RX_PL_WID 命令，读取到接收到的数据包中有效数据的宽度）
#define FEATURE     0x1D  //特征控制寄存器(动态负载长度功能总使能(RD_RX_PLOAD_WID命令可用)、使能应答信号负载功能(WR_ACK_PAYLOAD命令可用)、使能WR_TX_PAYLOAD_NOACK 命令)
//-----------------------------------------------------------------
#if 1
//外部函数
//-----------------------------------------------------------------
//--------------------------模式配置-----------------------------
extern void NRF_init(uint8_t mode);//NRF初始化
extern void TX_Mode(void);//转换发送模式
extern void RX_Mode(void);//转换接收模式
extern void TX_Mode_FAST(void);//快速转换为发送模式
extern void RX_Mode_FAST(void);//快速转换为接收模式

//----常规收、发独立函数（TX监测ACK信号，RX中断接收）（单工）----
extern int8_t Tx_Packet(const uint8_t* Buf, uint8_t num);
extern int8_t Rx_Packet(uint8_t* Buf);
extern int8_t Tx_Packet_16(const int16_t* Buf,uint8_t num);
extern int8_t Rx_Packet_16(int16_t* Buf,uint8_t num);

//---常规收、发一体函数（TX监测ACK信号，RX中断接收））（双工）---
extern int8_t Tx_Rx_Packet_TX_FAST(const uint8_t* TX_Buf, uint8_t TX_num, uint8_t* RX_Buf);
extern int8_t Tx_Rx_Packet_RX_FAST(const uint8_t* TX_Buf ,uint8_t TX_num, uint8_t* RX_Buf);
extern int8_t Tx_Rx_Packet_TX_FAST_16(const int16_t* TX_Buf,uint8_t TX_num, int16_t* RX_Buf, uint8_t RX_num);//int16
extern int8_t Tx_Rx_Packet_RX_FAST_16(const int16_t* TX_Buf,uint8_t TX_num, int16_t* RX_Buf, uint8_t RX_num);//int16;

//--------非常规发送函数（不检查Ack信号）(单、双工)--------------
extern void Tx_Packet_Noack(const uint8_t* Buf, uint8_t num);
extern void Tx_Packet_Noack_16(const int16_t* Buf, uint8_t num);

//非常规IQR中断处理函数(只在中断中处理三个中断TX_DS、RX_DR、MAX_RT)（单工）
extern int8_t NRF_IQR_hander_TX(void);
extern int8_t NRF_IQR_hander_RX(uint8_t* Buf);
extern int8_t NRF_IQR_hander_RX_16(int16_t* Buf,uint8_t num);

//非常规IQR中断处理函数(集中处理三个中断TX_DS、RX_DR、MAX_RT)（双工）
extern int8_t TX_RX_NRF_IQR_hander_TX(uint8_t* Buf);
extern int8_t TX_RX_NRF_IQR_hander_RX(uint8_t* Buf);
extern int8_t TX_RX_NRF_IQR_hander_TX_16(int16_t* Buf,uint8_t num);
extern int8_t TX_RX_NRF_IQR_hander_RX_16(int16_t* Buf,uint8_t num);

//-------------------IQR接收中断监测函数（双工）-----------------
extern int8_t MAX_RR(void);
//-----------------------------------------------------------------
#endif

#endif /* NRF24L01_H_ */
