/*
 * NRF24L01.c
 *
 *  Created on: 2014-12-5
 *      Author: FGZ
 */
#include "NRF24L01.h"
//----------------------------------本文件全局变量-------------------------------//
static uint8_t TX_ADR_VAL[TX_ADR_WIDTH]       = {0x34,0x43,0x28,0x37,0x51};//发送地址值
static uint8_t RX_ADR_VAL_P0[RX_ADR_WIDTH_P0] = {0x34,0x43,0x28,0x37,0x51};//接收地址值

static uint8_t sta;
//static int8_t rx_fail_count=0;

//-------------------------------------内部函数---------------------------------//
static void Flush_TX(void);
static void Flush_RX(void);
static void Clear_Status(void);
static void Clear_All(void);
static uint8_t SPI_Read_FIFO_Width(void);

/*
 *   功能：GPIO模拟SPI协议，主机通过MOSI线，将byte字节输出给从机，同时通过MISO线，读取从机传回的数据。
 *   参数：byte:要发送的字节和发送的同时收到的字节 (WRITE_REG+reg / COMMAND)
 */
static uint8_t SPI_RW(uint8_t byte)//模拟SPI
{
        uint8_t i;
        for(i=0; i<8; i++)   // 循环8次
        {
                if(byte & 0x80)
                        MOSI_H;      //输出字节的MSB=1
                else
                        MOSI_L;      //输出字节的MSB=0 ，输出字节通过主机的MOSI线从MSB循环输出
                byte <<= 1;      //byte的次高位移到MSB等待发送
                SCK_H;           //MSB从主机和从机同时流出
                byte |= MISO;    //读取从机的MOSI状态，输入字节通过MISO从LSB循环移入主机
                SCK_L;           //MISO移入移位寄存器
        }
    return(byte);        //返回读出的字节
}

/*
 * 1、写寄存器，同时返回寄存器初始值
 *    将value写入reg寄存器
 *
 */
static uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value)//写寄存器
{
        uint8_t status;

        CSN_L;                //CNS=0：SPI通信使能

        status = SPI_RW(reg); //发送寄存器地址（WRITE_REG+reg）
        SPI_RW(value);        //将value写入reg寄存器中

        CSN_H;                //CSN=1：SPI通信失能

        return status;       //返回状态字
}

/*
 * 1、读寄存器
 *    读reg寄存器的值
 */
static uint8_t SPI_Read_Reg(uint8_t reg)//读寄存器
{
        uint8_t reg_val;

        CSN_L;                 //CNS=0：SPI通信使能

        reg_val = SPI_RW(reg); //发送寄存器地址（READ_REG+reg）
        reg_val = SPI_RW(0);   //读寄存器

        CSN_H;                 //CSN=1：SPI通信失能

        return reg_val;       //返回寄存器数据
}

/*
 * 2、写FIFO，向首地址为reg的缓冲区写入宽度为width的数组Buf
 */
static uint8_t SPI_Write_Buf(uint8_t reg, const uint8_t* Buf, uint8_t width)//写FIFO(数据/地址)
{
        uint8_t status, i;

        CSN_L;                       //CNS=0：SPI通信使能

        status = SPI_RW(reg);        //发送寄存器地址或命令（WRITE_REG+TX_ADDR、RX_ADDR_P0 / WR_TX_PLOAD）

        for(i=0; i < width; i++)     //width:TX_PLOAD_WIDTH（=RX_PLOAD_WIDTH_P0）
        {
                status = SPI_RW(Buf[i]); //将Fuf[i]逐个写入reg缓冲区
        }

        CSN_H;                       //CSN=1：SPI通信失能
        return(status);             //返回状态字
}

/*
 * 2、读FIFO，从首地址为reg的缓冲区读出宽度为width的数组Buf
 */
static uint8_t SPI_Read_Buf(uint8_t reg, uint8_t* Buf, uint8_t width)//读FIFO(数据/地址)
{
        uint8_t status, i;

        CSN_L;                       //CNS=0：SPI通信使能

        status = SPI_RW(reg);        //发送寄存器地址或命令（READ_REG+TX_ADDR、RX_ADDR_P0 / RD_RX_PLOAD）

        for(i=0; i < width; i++)     //width:RX_PLOAD_WIDTH_P0（=TX_PLOAD_WIDTH)
        {
                Buf[i] = SPI_RW(0);      //逐个字节读取reg缓冲区
        }

        CSN_H;                       //CSN=1：SPI通信失能

        return(status);             //返回状态字
}

/*
 * 2、使用动态负载长度功能，读取数据FIFO：
 *    从缓冲区读出数组Buf
 */
static uint8_t SPI_Read_PLOAD_DYNAMIC(uint8_t* Buf)//读数据FIFO
{
        uint8_t status, i;
        uint8_t width;

        width = SPI_Read_FIFO_Width();//读取本次数据宽度

        CSN_L;                       //CNS=0：SPI通信使能

        status = SPI_RW(RD_RX_PLOAD);//发送RD_RX_PLOAD命令

        for(i=0; i<width; i++)       //width:RX_PLOAD_WIDTH_P0（=TX_PLOAD_WIDTH)
        {
                Buf[i] = SPI_RW(0);      //逐个字节读取reg缓冲区
        }

        CSN_H;                       //CSN=1：SPI通信失能

        return status;              //返回状态字
}

/*
 * 读RX_FIFO数据的宽度
 */
static uint8_t SPI_Read_FIFO_Width(void)
{
        uint8_t width;

        CSN_L;

        SPI_RW(RD_RX_PLOAD_WID);//发送RD_RX_PLOAD_WID命令

        width = SPI_RW(0);      //读取宽度

        CSN_H;

        return width;
}

/*
 * 清空TX_FIFO
 */
static void Flush_TX(void)
{
        CSN_L;
        SPI_RW(FLUSH_TX);      //清空TX_FIFO
        CSN_H;
}

/*
 * 清空RX_FIFO
 */
static void Flush_RX(void)
{
        CSN_L;
        SPI_RW(FLUSH_RX);      //清空RX_FIFO
        CSN_H;
}

/*
 * 清除所有状态标志
 */
static void Clear_Status(void)
{
        SPI_RW_Reg(WRITE_REG + NRF_STATUS, 0xff);
}

/*
 * 清空FIFO和状态标志
 */
static void Clear_All(void)
{
        Flush_TX();
        Flush_RX();
        Clear_Status();
}

/*
 * 进入掉电模式
 */
static void Power_Down(void)
{
        CE_L;
        SPI_RW_Reg(WRITE_REG + NRF_CONFIG,0x0d);  //PWR_UP=0
        CE_H;

        //delay_us(20);  //等待20us
}

//-------------------------------------外部函数---------------------------------//

//--------------------------模式配置-----------------------------
//---------------------------------------------------------------
/*
 * NRF24L01初始化
 */
void NRF_init(uint8_t mode)
{
        CE_L; //chip enable
        CSN_H;//spi disable
        SCK_L;//初始化时钟线

        if(mode == TX_MODE)
        {
                TX_Mode();
        }
        else
        {
                RX_Mode();
        }
}

/*
 * 配置NRF为发送模式
 * 进入待机模式2，只要TX_FIFO非空即进入TX_Mode自动进行发射
 */
void TX_Mode(void)
{
        //1、模式转换
        CE_L;                  //CE=0,使芯片进入待机模式1（进行WRITE_REG操作）

        //2、寄存器配置
        //----------------------------------------
        Clear_All();           //清空FIFO和状态标志
        //----------------------------------------
        SPI_RW_Reg(WRITE_REG + EN_AA,     0x01);              //接收机P0的自动应答功能使能（Ack）
        SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);              //接收机P0的接收功能使能
        SPI_RW_Reg(WRITE_REG + SETUP_RETR,0x25);              //发送机自动重发延时750us，自动重发5次 (250Kbps传输速率时，发送40个字节需要320*1000/250000=1.28ms,自动重发延时必须>250us，否则重发速率太快导致发送端接收不到Ack信号)
        SPI_RW_Reg(WRITE_REG + RF_CH,       10);              //发送机、6个接收机 工作频道 设置 frequency = 2400 + RF_CH [MHz]
        SPI_RW_Reg(WRITE_REG + RF_SETUP,  0x26);              //数据传输率250kbps，发射功率0dBm
        if(DYNAMIC_PLOAD_LENGTH)  //使用动态负载长度功能
        {
                SPI_RW_Reg(WRITE_REG + FEATURE,   0x04);              //使能动态负载长度
                SPI_RW_Reg(WRITE_REG + DYNPD,     0x01);                          //开启DPL_P0
        }
        else                      //使用静态负载长度
        {
                SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH_P0);  //设定本芯片RX_P0机的 有效数据宽度=RX_PLOAD_WIDTH_P0
        }
        SPI_RW_Reg(WRITE_REG + SETUP_AW, RX_ADR_WIDTH_P0-2);  //设定本芯片6个RX机的 地址宽度=RX_ADR_WIDTH_P0
        SPI_Write_Buf(WRITE_REG + TX_ADDR,    TX_ADR_VAL,    TX_ADR_WIDTH);   //设定本芯片   TX机的  地址值（宽度=TX_ADR_WIDTH）
        SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADR_VAL_P0, RX_ADR_WIDTH_P0);//设定本芯片RX_P0机的  地址值（宽度=RX_ADR_WIDTH_P0）
        //RX_ADDR_P0主要是为了使能EN_AA
        SPI_RW_Reg(WRITE_REG + NRF_CONFIG,0x0e);              //打开所有中断，使能CRC，16位CRC校验，PWR_UP=1，PRIM_RX=0发送模式
        //----------------------------------------

        //3、模式转换
        CE_H;//待机模式1 -> PRIM_RX=0、TX_FIFO空、CE=1 -> 待机模式2

        //CE=1：待机模式2 -> TX_FIFO非空 -> delay>130us -> TX_Mode  ( -> TX_FIFO空 -> 待机模式2)
}

/*
 * 配置NRF为接收模式
 */
void RX_Mode(void)
{
        //1、模式转换
        CE_L;                  //CE=0,使芯片进入待机模式1（进行WRITE_REG操作）

        //2、寄存器配置
        //----------------------------------------
        Clear_All();           //清空FIFO和状态标志
        //----------------------------------------
        SPI_RW_Reg(WRITE_REG + EN_AA,     0x01);              //接收机P0的自动应答功能使能（Ack）
        SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);              //接收机P0的接收功能使能
        SPI_RW_Reg(WRITE_REG + SETUP_RETR,0x25);              //发送机自动重发延时750us，自动重发5次 (250Kbps传输速率时，发送40个字节需要320*1000/250000=1.28ms,自动重发延时必须>250us，否则重发速率太快导致发送端接收不到Ack信号)
        SPI_RW_Reg(WRITE_REG + RF_CH,       10);              //发送机、6个接收机 工作频道 设置 frequency = 2400 + RF_CH [MHz]
        SPI_RW_Reg(WRITE_REG + RF_SETUP,  0x26);              //数据传输率250kbps，发射功率0dBm
        if(DYNAMIC_PLOAD_LENGTH)  //使用动态负载长度功能
        {
                SPI_RW_Reg(WRITE_REG + FEATURE,   0x04);              //使能动态负载长度
                SPI_RW_Reg(WRITE_REG + DYNPD,     0x01);                          //开启DPL_P0
        }
        else                      //使用静态负载长度
        {
                SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH_P0);  //设定本芯片RX_P0机的 有效数据宽度=RX_PLOAD_WIDTH_P0
        }
        SPI_RW_Reg(WRITE_REG + SETUP_AW, RX_ADR_WIDTH_P0-2);  //设定本芯片6个RX机的 地址宽度=RX_ADR_WIDTH_P0
        SPI_Write_Buf(WRITE_REG + TX_ADDR,    TX_ADR_VAL,    TX_ADR_WIDTH);   //设定本芯片   TX机的  地址值（宽度=TX_ADR_WIDTH）
        SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADR_VAL_P0, RX_ADR_WIDTH_P0);//设定本芯片RX_P0机的  地址值（宽度=RX_ADR_WIDTH_P0）
        //RX_ADDR_P0主要是为了使能EN_AA
        SPI_RW_Reg(WRITE_REG + NRF_CONFIG,0x0f);              //打开所有中断，使能CRC，16位CRC校验，PWR_UP=1，PRIM_RX=1接收模式
        //----------------------------------------

        //3、模式转换
        CE_H; //待机模式1 -> PRIM_RX=1、CE=1 -> delay>130us -> RX_Mode
}

/*
 *快速配置NRF为发送模式进入待机模式2
 */
void TX_Mode_FAST(void)
{
        //1、模式转换
        CE_L;                  //CE=0,使芯片进入待机模式1（进行WRITE_REG操作）

        //2、寄存器配置
        //----------------------------------------
        Clear_All();           //清空FIFO和状态标志
        SPI_RW_Reg(WRITE_REG + NRF_CONFIG, 0x0e); //打开所有中断，使能CRC，16位CRC校验，PWR_UP=1，PRIM_RX=0发送模式
        //----------------------------------------
        //3、模式转换
        CE_H;//待机模式1 -> PRIM_RX=0、TX_FIFO空、CE=1 -> 待机模式2

        //CE=1：待机模式2 -> TX_FIFO非空 -> delay>130us -> TX_Mode  ( -> TX_FIFO空 -> 待机模式2)
}

/*
 * 快速配置NRF为接收模式，进入RX_Mode
 */
void RX_Mode_FAST(void)
{
        //1、模式转换
        CE_L;                  //CE=0,使芯片进入待机模式1（进行WRITE_REG操作）

        //2、寄存器配置
        //----------------------------------------
        Clear_All();           //清空FIFO和状态标志
        SPI_RW_Reg(WRITE_REG + NRF_CONFIG, 0x0f);//打开所有中断，使能CRC，16位CRC校验，PWR_UP=1，PRIM_RX=1接收模式
        //----------------------------------------
        //3、模式转换
        CE_H; //待机模式1 -> PRIM_RX=1、CE=1 -> delay>130us -> RX_Mode
}
//---------------------------------------------------------------
//---------------------------------------------------------------

//----常规收、发独立函数（TX监测ACK信号，RX中断接收）（单工）----
//---------------------------------------------------------------
/*
 * 填充TX_FIFO，启动发送，等待Ack信号
 * 将数组Buf写入TX_FIFO中，并启动发射，
 * 填充TX_FIFO启动发送 -> 等待Ack中断 -> 读取状态寄存器 -> 中断处理 -> 清空FIFO和状态寄存器 -> 再次进入待机模式2 等待发送
 */
int8_t Tx_Packet(const uint8_t* Buf, uint8_t num)
{
        int8_t result=0;//收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        //num = _constrain(num,1,32);//num限幅

        //TX端已经是 待机模式2 （可进行WRITE_REG操作）
        if(DYNAMIC_PLOAD_LENGTH)
        {
                SPI_Write_Buf(WR_TX_PLOAD, Buf, num);//填充TX_FIFO（WR_TX_PLOAD -> TX_FIFO有效数据缓冲区：写FIFO）
        }
        else
        {
                SPI_Write_Buf(WR_TX_PLOAD, Buf, TX_PLOAD_WIDTH);//填充TX_FIFO（WR_TX_PLOAD -> TX_FIFO有效数据缓冲区：写FIFO）
        }

        CE_H;             //待机模式2 -> TX_FIFO非空、CE=1 -> 130us -> TX_Mode

        while(IRQ_READ);  //等待TX_DS中断 (收到接收端的Ack信号)或者 MAX_RT中断 (未收到接收端的Ack信号)

        sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

        if(TX_DS) //自动清空TX_FIFO，由于CE=1，再次 进入待机模式2 （可进行WRITE_REG操作）
        {
                Clear_Status();  //清空状态寄存器，置位IRQ

                result = 1;      //发送成功返回1
        }
        else if(MAX_RT) //最多次重发中断
        {

                Flush_TX(); //清空TX_FIFO，由于CE=1，进入待机模式2（才可进行WRITE_REG操作）

                Clear_Status();    //清空状态寄存器(MAX_RT)，置位IRQ

                result = -1;       //发送失败返回-1
        }
        else
        {
                Clear_All();      //清空FIFO和状态标志，置位IRQ

                result =  3;     //其他返回3
        }

        return result;
}

/*
 * 等待IRQ接收中断，将RX_FIFO中的数据读入数组Buf中
 * 查询IRQ_READ中断 -> 进入待机模式1 -> 读取状态寄存器 -> 判断中断类型 -> 读取RX_FIFO -> 清空FIFO和状态寄存器 -> CE_H系统再次进入RX_Mode
 */
int8_t Rx_Packet(uint8_t* Buf)
{
        int8_t result=0;//收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        while(IRQ_READ); //确认接收到有效数据包后，产生RX_DR中断，并自动发送Ack信号
                           //NRF_STATUS 的 RX_P_NO 显示本次接收中断由哪一个接收机产生

        CE_L;  //CE=0 进入待机模式1

        sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

        if(RX_DR) //判断是否是接收中断
        {
                //读写操作
                if(DYNAMIC_PLOAD_LENGTH)
                {
                        SPI_Read_PLOAD_DYNAMIC(Buf);
                }
                else
                {
                        SPI_Read_Buf(RD_RX_PLOAD, Buf, RX_PLOAD_WIDTH_P0);  //SPI读取RX_FIFO中的有效数据
                }

                Clear_Status(); //清空状态寄存器，置位IRQ

                result = 2;     //接收成功返回2
        }
        else
        {
                Clear_All();     //清空FIFO和状态标志，置位IRQ

                result =  3;     //其他返回3
        }

        CE_H;  //再次进入RX_Mode

        return result;
}

/*
 * 填充TX_FIFO，启动发送，检查Ack信号
 */
int8_t Tx_Packet_16(const int16_t* Buf,uint8_t num)//(int16)
{
        static int16_union dat;
        static uint8_t dat_8[32];
        uint8_t i=0;
        int8_t result; //收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        for(i=0; i<num; i++) //字节拆解
        {
                dat.dat_16 = Buf[i];
                dat_8[i*2] = dat.dat_8.dat1;
                dat_8[i*2+1] = dat.dat_8.dat2;
        }

        result = Tx_Packet(dat_8 ,2*num);  //发送数据包（2*num<=TX_PLOAD_WIDTH）

        return result;
}

/*
 * 等待IRQ接收中断，将RX_FIFO中的数据读入数组Buf中
 */
int8_t Rx_Packet_16(int16_t* Buf,uint8_t num)//(int16)
{
        static int16_union dat;
        static uint8_t dat_8[32];
        uint8_t i=0;
        int8_t result; //收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        result = Rx_Packet(dat_8); //接收数据包（<=RX_PLOAD_WIDTH_P0）

        for(i=0; i<num; i++) //字节拆解
        {
                dat.dat_8.dat1 = dat_8[i*2];
                dat.dat_8.dat2 = dat_8[i*2+1];
                Buf[i] = dat.dat_16;
        }

        return result;
}
//---------------------------------------------------------------
//---------------------------------------------------------------

//---常规收、发一体函数（TX监测ACK信号，RX中断接收））（双工）---
//---------------------------------------------------------------
/*
 * 快速转换收发模式
 * 双工通信的TX端收发函数，交替发送TX_Buf和接收BX_Buf
 */
int8_t Tx_Rx_Packet_TX_FAST(const uint8_t* TX_Buf, uint8_t TX_num, uint8_t* RX_Buf)
{
        int8_t result=0;//收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）
        static uint8_t status = TX_MODE;//初始化为TX端

        //TX_num = _constrain(TX_num,1,32);

        if(status == TX_MODE)
        {
                //TX端已经是 待机模式2 （可进行WRITE_REG操作）
                if(DYNAMIC_PLOAD_LENGTH)
                {
                        SPI_Write_Buf(WR_TX_PLOAD, TX_Buf, TX_num);//填充TX_FIFO（WR_TX_PLOAD -> TX_FIFO有效数据缓冲区：写FIFO）
                }
                else
                {
                        SPI_Write_Buf(WR_TX_PLOAD, TX_Buf, TX_PLOAD_WIDTH);//填充TX_FIFO（WR_TX_PLOAD -> TX_FIFO有效数据缓冲区：写FIFO）
                }

                CE_H;             //待机模式2 -> TX_FIFO非空、CE=1 -> 130us -> TX_Mode

                //TX完成后，P0被自动设置为接收模式来接收Ack信号，约定：RX_ADDR_P0=TX_ADDR
                while(IRQ_READ);  //等待TX_DS中断 (收到接收端的Ack信号)或者 MAX_RT中断 (未收到接收端的Ack信号)

                sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

                if(TX_DS) //自动清空TX_FIFO，由于CE=1，再次 进入待机模式2 （可进行WRITE_REG操作）
                {
                        Clear_Status(); //清空状态寄存器，置位IRQ
                        //--------------------------------------
                        RX_Mode_FAST();
                        status = RX_MODE; //发送成功，开始接收
                        //--------------------------------------
                        result = 1;      //发送成功返回1
                }
                else if(MAX_RT) //最多次重发中断
                {
                        Flush_TX();; //清空TX_FIFO，由于CE=1，进入待机模式2（才可进行WRITE_REG操作）

                        Clear_Status();                          //清空状态寄存器(MAX_RT)，置位IRQ
                        //--------------------------------------
                        RX_Mode_FAST();
                        status = RX_MODE;//1、TX端发送失败，转换为接收！！！
                        //--------------------------------------
                        result = -1;       //发送失败返回-1
                }
                else
                {
                        Clear_All();      //清空FIFO和状态标志，置位IRQ

                        result =  3;      //其他返回3
                }
        }
        else  //RX_MODE
        {
                if(!IRQ_READ) //查询接收
                {

                        CE_L;  //CE=0 进入待机模式1

                        sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

                        if(RX_DR) //判断是否是接收中断
                        {
                                //读写操作
                                if(DYNAMIC_PLOAD_LENGTH)
                                {
                                        SPI_Read_PLOAD_DYNAMIC(RX_Buf);
                                }
                                else
                                {
                                        SPI_Read_Buf(RD_RX_PLOAD, RX_Buf, RX_PLOAD_WIDTH_P0);  //SPI读取RX_FIFO中的有效数据
                                }

                                Clear_Status();                          //清空状态寄存器，置位IRQ
                                //--------------------------------------
                                TX_Mode_FAST();
                                status = TX_MODE;//接收成功，开始发送 （待机模式2）
                                //--------------------------------------
                                result = 2;   //接收成功返回2
                        }
                        else
                        {
                                Clear_All();                             //清空FIFO和状态标志，置位IRQ

                                CE_H;  //再次进入RX_Mode

                                result =  3;     //其他返回3
                        }
                }
                else
                {
                        status = status;  //2、TX端接收失败，继续接收！！！

                        result =  -2;     //接收失败返回-2
                }
        }
        return result;
}

/*
 * 快速转换收发模式
 * 双工通信的RX端收发函数，交替发送TX_Buf和接收BX_Buf
 */
int8_t Tx_Rx_Packet_RX_FAST(const uint8_t* TX_Buf ,uint8_t TX_num, uint8_t* RX_Buf)
{
        int8_t result=0;//收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）
        static uint8_t status = RX_MODE;//初始化为RX端
        static uint8_t rx_failed=0;//接收失败计数（发送失败有中断状态提示，所以不用设变量来监测了）

        //TX_num = _constrain(TX_num,1,32);

        rx_failed++;
        //rx_failed = _constrain(rx_failed,1,50);

        if(status == TX_MODE)
        {
                //TX端已经是 待机模式2 （可进行WRITE_REG操作）
                if(DYNAMIC_PLOAD_LENGTH)
                {
                        SPI_Write_Buf(WR_TX_PLOAD, TX_Buf, TX_num);//填充TX_FIFO（WR_TX_PLOAD -> TX_FIFO有效数据缓冲区：写FIFO）
                }
                else
                {
                        SPI_Write_Buf(WR_TX_PLOAD, TX_Buf, TX_PLOAD_WIDTH);//填充TX_FIFO（WR_TX_PLOAD -> TX_FIFO有效数据缓冲区：写FIFO）
                }

                CE_H;             //待机模式2 -> TX_FIFO非空、CE=1 -> 130us -> TX_Mode

                //TX完成后，P0被自动设置为接收模式来接收Ack信号，约定：RX_ADDR_P0=TX_ADDR
                while(IRQ_READ);  //等待TX_DS中断 (收到接收端的Ack信号)或者 MAX_RT中断 (未收到接收端的Ack信号)

                sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

                if(TX_DS) //自动清空TX_FIFO，由于CE=1，再次 进入待机模式2 （可进行WRITE_REG操作）
                {
                        Clear_Status();                          //清空状态寄存器，置位IRQ
                        //--------------------------------------
                        RX_Mode_FAST();
                        status = RX_MODE;
                        rx_failed = 0;  //发送成功，开始接收
                        //--------------------------------------
                        result = 1;      //发送成功返回1
                }
                else if(MAX_RT) //最多次重发中断
                {
                        Flush_TX();; //清空TX_FIFO，由于CE=1，进入待机模式2（才可进行WRITE_REG操作）

                        Clear_Status();      //清空状态寄存器(MAX_RT)，置位IRQ

                        status = status;     //3、RX端发送失败，继续发送！！！

                        result = -1;       //发送失败返回-1
                }
                else
                {
                        Clear_All();      //清空FIFO和状态标志，置位IRQ

                        result =  3;      //其他返回3
                }
        }
        else  //RX_MODE
        {
                if(!IRQ_READ) //查询接收
                {

                        CE_L;  //CE=0 进入待机模式1（可进行WRITE_REG等SPI操作）

                        sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

                        if(RX_DR) //判断是否是接收中断
                        {
                                //读写操作
                                if(DYNAMIC_PLOAD_LENGTH)
                                {
                                        SPI_Read_PLOAD_DYNAMIC(RX_Buf);
                                }
                                else
                                {
                                        SPI_Read_Buf(RD_RX_PLOAD, RX_Buf, RX_PLOAD_WIDTH_P0);  //SPI读取RX_FIFO中的有效数据
                                }

                                Clear_Status();                          //清空状态寄存器，置位IRQ
                                //--------------------------------------
                                TX_Mode_FAST();
                                status = TX_MODE;
                                rx_failed=0;  //接收成功，开始发送（待机模式2）
                                //--------------------------------------
                                result = 2;   //接收成功返回2
                        }
                        else
                        {
                                Clear_All();  //清空FIFO和状态标志，置位IRQ

                                CE_H;  //再次进入RX_Mode

                                result =  3;     //其他返回3
                        }
                }
                //--------------------------------------
                else if(rx_failed > 10)
                {
                        TX_Mode_FAST();
                        status = TX_MODE;  //4、RX端接收失败，转换为发送！！！（待机模式2）
                        rx_failed=0;

                        result =  -2;     //接收失败返回-2
                }
                else
                {
                        status = status;  //状态不变，继续接收

                        result =  3;     //其他返回3
                }
                //--------------------------------------
        }
        return result;
}

/*
 * 双工通信的TX端收发函数，快速转换收发模式
 */
int8_t Tx_Rx_Packet_TX_FAST_16(const int16_t* TX_Buf,uint8_t TX_num, int16_t* RX_Buf, uint8_t RX_num)//int16
{
        static uint8_t TX_dat_8[32];
        static uint8_t RX_dat_8[32];
        static int16_union dat;
        uint8_t i=0;
        int8_t result; //收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        for(i=0; i<TX_num; i++) //字节拆解
        {
                dat.dat_16 = TX_Buf[i];
                TX_dat_8[i*2] = dat.dat_8.dat1;
                TX_dat_8[i*2+1] = dat.dat_8.dat2;
        }

        result = Tx_Rx_Packet_TX_FAST(TX_dat_8, 2*TX_num, RX_dat_8); //收发数据包（2*num<=TX_PLOAD_WIDTH）

        for(i=0; i<RX_num; i++) //字节拆解
        {
                dat.dat_8.dat1 = RX_dat_8[i*2];
                dat.dat_8.dat2 = RX_dat_8[i*2+1];
                RX_Buf[i] = dat.dat_16;
        }

        return result;
}

/*
 * 双工通信的RX端收发函数，快速转换收发模式
 */
int8_t Tx_Rx_Packet_RX_FAST_16(const int16_t* TX_Buf,uint8_t TX_num, int16_t* RX_Buf, uint8_t RX_num)//int16
{
        static uint8_t TX_dat_8[32];
        static uint8_t RX_dat_8[32];
        static int16_union dat;
        uint8_t i=0;
        int8_t result; //收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        for(i=0; i<TX_num; i++) //字节拆解
        {
                dat.dat_16 = TX_Buf[i];
                TX_dat_8[i*2] = dat.dat_8.dat1;
                TX_dat_8[i*2+1] = dat.dat_8.dat2;
        }

        result = Tx_Rx_Packet_RX_FAST(TX_dat_8, 2*TX_num, RX_dat_8); //收发数据包（2*num<=TX_PLOAD_WIDTH）

        for(i=0; i<RX_num; i++) //字节拆解
        {
                dat.dat_8.dat1 = RX_dat_8[i*2];
                dat.dat_8.dat2 = RX_dat_8[i*2+1];
                RX_Buf[i] = dat.dat_16;
        }

        return result;
}

//--------非常规发送函数（不检查Ack信号）(单、双工)--------------
//---------------------------------------------------------------
/*
 * 填充TX_FIFO，启动发送，不检查Ack信号
 * 将数组Buf(uint8_t)写入TX_FIFO中，并启动发射
 */
void Tx_Packet_Noack(const uint8_t* Buf, uint8_t num)
{
        //num = _constrain(num,1,32);

        //TX端已经是 待机模式2 （可进行WRITE_REG操作）
        if(DYNAMIC_PLOAD_LENGTH)
        {
                SPI_Write_Buf(WR_TX_PLOAD, Buf, num);//填充TX_FIFO（WR_TX_PLOAD -> TX_FIFO有效数据缓冲区：写FIFO）
        }
        else
        {
                SPI_Write_Buf(WR_TX_PLOAD, Buf, TX_PLOAD_WIDTH);//填充TX_FIFO（WR_TX_PLOAD -> TX_FIFO有效数据缓冲区：写FIFO）
        }

        CE_H;             //待机模式2 -> TX_FIFO非空、CE=1 -> 130us -> TX_Mode
}

/*
 * 填充TX_FIFO，启动发送，不检查Ack信号
 * 将数组Buf(int16)写入TX_FIFO中，并启动发射
 */
void Tx_Packet_Noack_16(const int16_t* Buf, uint8_t num)
{
        static int16_union dat;
        static uint8_t dat_8[32];
        uint8_t i=0;

        for(i=0; i<num; i++) //字节拆解
        {
                dat.dat_16 = Buf[i];
                dat_8[i*2] = dat.dat_8.dat1;
                dat_8[i*2+1] = dat.dat_8.dat2;
        }

        Tx_Packet_Noack(dat_8 ,2*num);  //发送数据包（2*num<=TX_PLOAD_WIDTH）
}
//---------------------------------------------------------------
//---------------------------------------------------------------


//非常规IQR中断处理函数(只在中断中处理三个中断TX_DS、RX_DR、MAX_RT)（单工）
//---------------------------------------------------------------

/*
 * NRF中断信号处理(TX端)
 */
int8_t NRF_IQR_hander_TX(void)
{
        int8_t result;//收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

        if(TX_DS) //发送成功中断，自动清空TX_FIFO，由于CE=1，再次 进入待机模式2 （可进行WRITE_REG操作）
        {
                CE_L;            //进入待机模式1
                Clear_All();     //清空FIFO和状态标志，置位IRQ
                CE_H;            //进入待机模式2(待机模式1 -> PRIM_RX=0，CE=1 -> 待机模式2 （可进行WRITE_REG操作）)

                //--------------------------------------
                //...
                //--------------------------------------

                //g_ctrl_signal_check = 0;//发送成功，计数清零

                result = 1;      //发送成功返回1
        }
        else if(MAX_RT)  //多次重发中断
        {
                //Flush_TX();; //清空TX_FIFO，由于CE=1，进入待机模式2（才可进行WRITE_REG操作）

                CE_L;            //进入待机模式1
                Clear_All();     //清空FIFO和状态标志，置位IRQ
                CE_H;            //进入待机模式2(待机模式1 -> PRIM_RX=0，CE=1 -> 待机模式2 （可进行WRITE_REG操作）)

                //--------------------------------------
                //...
                //--------------------------------------

                result = -1;       //发送失败返回-1
        } //发送成功，转换为接收模式
        else
        {
                CE_L;            //进入待机模式1
                Clear_All();     //清空FIFO和状态标志，置位IRQ
                CE_H;            //进入待机模式2(待机模式1 -> PRIM_RX=0，CE=1 -> 待机模式2 （可进行WRITE_REG操作）)


                result = 3;      //其他情况返回3
        }
        return result;
}

/*
 * NRF中断信号处理(RX端)，中断接收函数
 */
int8_t NRF_IQR_hander_RX(uint8_t* Buf)
{
        int8_t result;//收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

        if(RX_DR) //接收中断
        {
                if(DYNAMIC_PLOAD_LENGTH)//读RX_FIFO
                {
                        SPI_Read_PLOAD_DYNAMIC(Buf);
                }
                else
                {
                        SPI_Read_Buf(RD_RX_PLOAD, Buf, RX_PLOAD_WIDTH_P0);  //SPI读取RX_FIFO中的有效数据
                }

                //Ack信号发送完成后，PRIM_RX=1 CE=1,此时系统还是处于接收模式

                CE_L;            //进入待机模式1
                Clear_All();     //清空FIFO和状态标志，置位IRQ
                CE_H;            //130us后，进入接收模式

                //--------------------------------------
                //rx_fail_count=0;  //接收监控变量清零
                //--------------------------------------
                result = 2;   //接收成功返回2
        }
        else
        {
                CE_L;            //进入待机模式1
                Clear_All();     //清空FIFO和状态标志，置位IRQ
                CE_H;            //130us后，进入接收模式

                result = 3;     //其他情况返回3
        }
        return result;
}

/*
 * NRF中断信号处理(RX端)，中断接收
 */
int8_t NRF_IQR_hander_RX_16(int16_t* Buf,uint8_t num)//(int16)
{
        static int16_union dat;
        static uint8_t dat_8[32];
        uint8_t i=0;
        int8_t result; //收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        result = NRF_IQR_hander_RX(dat_8); //接收数据包（2*num<=RX_PLOAD_WIDTH_P0）

        for(i=0; i<num; i++) //字节拆解
        {
                dat.dat_8.dat1 = dat_8[i*2];
                dat.dat_8.dat2 = dat_8[i*2+1];
                Buf[i] = dat.dat_16;
        }
        return result;
}
//---------------------------------------------------------------
//---------------------------------------------------------------


//非常规IQR中断处理函数(集中处理三个中断TX_DS、RX_DR、MAX_RT)（双工）
//---------------------------------------------------------------
/*
 * NRF中断信号处理(TX端)
 * 中断接收，通信成功，则转换模式
 */
int8_t TX_RX_NRF_IQR_hander_TX(uint8_t* Buf)
{
        int8_t result;//收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

        if(TX_DS) //发送成功中断，自动清空TX_FIFO，由于CE=1，再次 进入待机模式2 （可进行WRITE_REG操作）
        {
                Power_Down();  //从 待机模式2 进入 掉电模式

                //--------------------------------------
                RX_Mode_FAST();//从 掉电模式 进入 待机模式1 再进入 接收模式(发送成功，开始接收)
                //--------------------------------------

                result = 1;      //发送成功返回1
        }
        else if(MAX_RT)  //多次重发中断
        {
                Flush_TX();; //清空TX_FIFO，由于CE=1，进入待机模式2（才可进行WRITE_REG操作）

                Clear_Status();                          //清空状态寄存器(MAX_RT)，置位IRQ

                //--------------------------------------
                              //1、TX端发送失败，继续发送！！！
                //--------------------------------------

                result = -1;       //发送失败返回-1
                //g_global[24]++;
        } //发送成功，转换为接收模式
        else if(RX_DR) //接收中断
        {
                if(DYNAMIC_PLOAD_LENGTH) //读RX_FIFO
                {
                        SPI_Read_PLOAD_DYNAMIC(Buf);
                }
                else
                {
                        SPI_Read_Buf(RD_RX_PLOAD, Buf, RX_PLOAD_WIDTH_P0);  //SPI读取RX_FIFO中的有效数据
                }

                //--------------------------------------
                //delay_us(500); //等待Ack信号发送完成
                //Ack信号发送完成后，PRIM_RX=1 CE=1,此时系统还是处于接收模式

                //rx_fail_count=0;  //接收监控变量清零
                //--------------------------------------

                result = 2;   //接收成功返回2
        }
        else
        {
                Clear_All();                             //清空FIFO和状态标志，置位IRQ

                result =  3;     //其他情况返回3
        }

        return result;
}

/*
 * NRF中断信号处理(RX端)
 */
int8_t TX_RX_NRF_IQR_hander_RX(uint8_t* Buf)
{
        int8_t result;//收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        sta = SPI_Read_Reg(READ_REG+NRF_STATUS); //读取状态寄存器

        if(TX_DS) //发送成功中断，自动清空TX_FIFO，由于CE=1，再次 进入待机模式2 （可进行WRITE_REG操作）
        {
                Power_Down();  //从 待机模式2 进入 掉电模式

                //--------------------------------------
                RX_Mode_FAST();//从 掉电模式 进入 待机模式1 再进入 接收模式(发送成功，开始接收)
                //--------------------------------------

                result = 1;      //发送成功返回1
        }
        else if(MAX_RT)  //多次重发中断
        {
                //Flush_TX();; //清空TX_FIFO，由于CE=1，进入待机模式2（才可进行WRITE_REG操作）

                Power_Down(); //从 待机模式2 进入 掉电模式

                //--------------------------------------
                RX_Mode_FAST();//从 掉电模式 进入 待机模式1 再进入 接收模式(2、RX端发送失败，转接收模式！！！)
                //--------------------------------------

                result = -1;       //发送失败返回-1
        }//发送成功，转换为接收模式
        else if(RX_DR) //接收中断
        {
                if(DYNAMIC_PLOAD_LENGTH)//读RX_FIFO
                {
                        SPI_Read_PLOAD_DYNAMIC(Buf);
                }
                else
                {
                        SPI_Read_Buf(RD_RX_PLOAD, Buf, RX_PLOAD_WIDTH_P0);  //SPI读取RX_FIFO中的有效数据
                }

                //--------------------------------------
                //delay_us(500); //等待Ack信号发送完成


                TX_Mode_FAST();//进入待机模式2 (接收成功，开始发送)
                //Tx_Packet_Noack_16(g_RC_Back, 8); //RX端接到数据发送Ack信号后，马上发送一个数据包过去给TX端接收(回馈飞行器的重要数据给遥控器)
                //delay_us(130);    //等待130us进入发送模式

                //rx_fail_count=0;  //接收监控变量清零
                //--------------------------------------

                result = 2;   //接收成功返回2
        }
        else
        {
                Power_Down();  //从 待机模式2 进入 掉电模式

                //--------------------------------------
                RX_Mode_FAST();//从 掉电模式 进入 待机模式1 再进入 接收模式(发送成功，开始接收)
                //--------------------------------------

                result = 3;     //其他情况返回3
        }
        return result;
}

/*
 * NRF中断信号处理(TX端)
 * 中断接收，通信成功，则转换模式
 */
int8_t TX_RX_NRF_IQR_hander_TX_16(int16_t* Buf,uint8_t num)//(int16)
{
        static int16_union dat;
        static uint8_t dat_8[32];
        uint8_t i=0;
        int8_t result; //收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        result = TX_RX_NRF_IQR_hander_TX(dat_8); //接收数据包（2*num<=RX_PLOAD_WIDTH_P0）

        for(i=0; i<num; i++) //字节拆解
        {
                dat.dat_8.dat1 = dat_8[i*2];
                dat.dat_8.dat2 = dat_8[i*2+1];
                Buf[i] = dat.dat_16;
        }
        return result;
}

/*
 * NRF中断信号处理(RX端)
 * 中断接收(int16)，通信成功，则转换模式
 */
int8_t TX_RX_NRF_IQR_hander_RX_16(int16_t* Buf,uint8_t num)//(int16)
{
        static int16_union dat;
        static uint8_t dat_8[32];
        uint8_t i=0;
        int8_t result; //收发结果（发送成功：1，发送失败：-1 接收成功：2 接收失败：-2  数据包过大：0  其他：3）

        result = TX_RX_NRF_IQR_hander_RX(dat_8); //接收数据包（2*num<=RX_PLOAD_WIDTH_P0）

        for(i=0; i<num; i++) //字节拆解
        {
                dat.dat_8.dat1 = dat_8[i*2];
                dat.dat_8.dat2 = dat_8[i*2+1];
                Buf[i] = dat.dat_16;
        }
        return result;
}
//---------------------------------------------------------------
//---------------------------------------------------------------

#if 0
//-------------------IQR接收中断监测函数（双工）-----------------
//---------------------------------------------------------------
/*
 * 多次重接失败检测（每个控制周期对NRF接收中断进行一次监测，在IRQ中断中将监测变量清零）
 * 失败：返回1
 * 成功：返回0
 */
int8_t MAX_RR(void)
{
        const int8_t ARC = 50; //最大重接次数

        if(g_period_count == 1)
                rx_fail_count = ARC;//初始化rx_fail_count变量，如果赋值为0，就认为此时已经接到一个数据了，显然不对

        rx_fail_count++;
        rx_fail_count = _constrain(rx_fail_count,1,100);

        if(rx_fail_count > ARC) //达到最大接收次数，确认接收失败了
                return 1;
        else
                return 0;
}
#endif
