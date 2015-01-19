//-----------------------------------------------------------------
//文件名称：i2c.h
//功能概要：模拟i2c底层驱动程序文件
//版本更新：2013-10-17
//-----------------------------------------------------------------

//------------------------------ 编译预处理 ------------------------------//
#ifndef _I2C_H_
#define _I2C_H_  

//包含头文件
#include "Common.h"             //包含公共类型定义

//----------------------------------------------------------------------//
//条件编译
//----------------------------------------------------------------------//
#define TM4C123GH6PM_1
#define TM4C123GH6PM
//定义了MCS51芯片
#if defined MCS_51

sbit SCL = P2^0;                //IIC时钟引脚定义
sbit SDA = P2^1;                //IIC数据引脚定义

#define SCL_L (SCL=0)           //SCL置低
#define SCL_H (SCL=1)           //SCL置高
#define SDA_L (SDA=0)           //SDA置低
#define SDA_H (SDA=1)           //SDA置高

//定义了XS128芯片
#elif defined XS_128 

#define SCL PTH_PTH0  			//IIC时钟引脚定义
#define SDA PTH_PTH1		   	//IIC数据引脚定义

#define SCL_L (SCL=0)           //SCL置低
#define SCL_H (SCL=1)           //SCL置高
#define SDA_L (SDA=0)           //SDA置低
#define SDA_H (SDA=1)           //SDA置高

#define SDA_I (DDRH_DDRH1=0)    //设置SDA为输入模式
#define SDA_O (DDRH_DDRH1=1)    //设置SDA为输出模式

//定义了STM32芯片
#elif defined STM32_F4XX

#if 1 //X型传感器(PD2,PD3)
#define SCL_L    GPIOD->BSRRH=GPIO_Pin_2   //SCL置低     SCL:PD2
#define SCL_H    GPIOD->BSRRL=GPIO_Pin_2   //SCL置高
#define SDA_L    GPIOD->BSRRH=GPIO_Pin_3   //SDA置低     SDA:PD3
#define SDA_H    GPIOD->BSRRL=GPIO_Pin_3   //SDA置高

#define SDA_I	GPIOD->MODER|=GPIO_Mode_IN<<4; //设置SDA为输入模式（功能：将GPIOD->MODER的第4位置成GPIO_Mode_IN所代表的模式）
#define SDA_O   GPIOD->MODER|=GPIO_Mode_OUT<<4;//设置SDA为输出模式（功能：将GPIOD->MODER的第4位置成GPIO_Mode_OUT所代表的模式）

#define SDA    ((GPIOD->IDR&GPIO_Pin_3)!=0)?1:0 //读取SDA的值

#else //十字型传感器(PB11,PB13)
#define SCL_L    GPIOB->BSRRH=GPIO_Pin_11   //SCL置低     SCL:PB11
#define SCL_H    GPIOB->BSRRL=GPIO_Pin_11   //SCL置高
#define SDA_L    GPIOB->BSRRH=GPIO_Pin_13   //SDA置低     SDA:PB13
#define SDA_H    GPIOB->BSRRL=GPIO_Pin_13   //SDA置高

#define SDA_I	GPIOB->MODER|=GPIO_Mode_IN<<14; //设置SDA为输入模式（功能：将GPIOA->MODER的第14位置成GPIO_Mode_IN所代表的模式）
#define SDA_O   GPIOB->MODER|=GPIO_Mode_OUT<<14;//设置SDA为输出模式（功能：将GPIOA->MODER的第14位置成GPIO_Mode_OUT所代表的模式）

#define SDA    ((GPIOB->IDR&GPIO_Pin_13)!=0)?1:0 //读取SDA的值
#endif

//定义了TM4C123GH6PM芯片
#elif defined TM4C123GH6PM_1

#define SCL_L    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00)	//SCL置低     SCL:PE4
#define SCL_H    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x10)   	//SCL置高
#define SDA_L    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00)   	//SDA置低     SDA:PE5
#define SDA_H    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x20)   	//SDA置高

#define SDA_I	ROM_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5)	//设置SDA为输入模式
#define SDA_O   ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5)	//设置SDA为输出模式

#define SDA    (ROM_GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_5)!=0)?1:0 	//读取SDA的值

#endif

//------------------------------ 外部接口函数声明 ------------------------------//

extern void I2C_start(void);
extern void I2C_stop(void);
extern void I2C_ack(void);
extern void I2C_no_ack(void);
extern void I2C_check_ack(void);
extern void I2C_send_one_char(uint8_t c);
extern void I2C_recv_one_char(uint8_t *c);

//-----------------------------------------------------------------
//函数名称：i2c_init
//功能概要：i2c初始化，初始化模拟i2c的IO口
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
extern void i2c_init(void);

//-----------------------------------------------------------------
//函数名称：I2C_send_str
//功能概要：向有寄存器地址器件发送一个字符串。               
//          从启动总线到发送地址，寄存器地址,发送数据，结束总线的全过程。
//          从机地址slave，寄存器地址reg,发送的内容是s指向的内容，发送num个字节。
//函数返回：void
//参数说明：slave:从机地址
//          reg  :寄存器地址
//          s    :发送字符串首地址
//          num  :字节个数
//-----------------------------------------------------------------
extern void  I2C_send_str(uint8_t slave,uint8_t reg,uint8_t *s,uint8_t num);

//-----------------------------------------------------------------
//函数名称：I2C_recv_str
//功能概要：向有寄存器地址器件接收一个字符串。         
//          从启动总线到发送地址，寄存器地址,接收数据，结束总线的全过程。
//          从机地址slave，寄存器地址reg,接收的内容放入s指向的存储区，接收num个字节。
//函数返回：void
//参数说明：slave:从机地址
//          reg  :寄存器地址
//          s    :接收字符串首地址
//          num  :字节个数
//-----------------------------------------------------------------
extern void  I2C_recv_str(uint8_t slave,uint8_t reg,uint8_t *s,uint8_t num);

//交换参数输入顺序：
extern int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);
extern int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * buf);

#endif

