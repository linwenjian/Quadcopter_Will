/*
 * OLED.h
 *
 *  Created on: May 23, 2013
 *      Author: LiuSX
 */

#ifndef OLED_H_
#define OLED_H_

#include "Common.h"


//typedef unsigned char uint8_t;
//typedef unsigned int uint16_t;
typedef unsigned long int32;

//extern uint8_t guiji[8][128];


//OLED mode control
#define DisplayON_OFF		DisplayON				//set OLED display ON/OFF
#define DisplayMode			NormalDisplay			//set display mode(normal/inverse)
#define ScanRowDirection	ScanLeftRight			//set scan from left to right (right to left)
#define ScanColumnDirection	ScanTopButtom			//set scan from top to buttom (buttom to top)

//mode control set parameter
#define ScanLeftRight		0xa1
#define ScanRightLeft		0xa0
#define ScanTopButtom		0xc8
#define ScanButtomTop		0xc0
#define DisplayON		0xa4
#define DisplayOFF		0xa5
#define NormalDisplay		0xa6
#define InverseDisplay          0xa7

//OLED control pin select
#define OLED_SCL_INIT		// nothing to be done
#define OLED_SDA_INIT		// nothing to be done
#define OLED_RST_INIT		// nothing to be done
#define OLED_DC_INIT		// nothing to be done

#define OLED_SCL_SET		ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0,0X01)
#define OLED_SCL_CLR		ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0,0X00)
#define OLED_SDA_SET	        ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1,0X02)
#define OLED_SDA_CLR		ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1,0X00)
#define OLED_RST_SET	        ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0X04)
#define OLED_RST_CLR		ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0X00)
#define OLED_DC_SET	        ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,0X08)
#define OLED_DC_CLR		ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,0X00)


#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF 
#define X_WIDTH 128
#define Y_WIDTH 64


//declare function
void OLED_WrDat(uint8_t dat);
void OLED_WrCmd(uint8_t cmd);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Fill(uint8_t bmp_dat);
void OLED_CLS(void);
void OLED_Init(void);
void OLED_P6x8Str(uint8_t x, uint8_t y,char *ch);
void OLED_P8x16Str(uint8_t x, uint8_t y,char *ch);
void Draw_BMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
void OLED_P6x8Num(uint8_t x, uint8_t y, uint16_t num);
void OLED_P8x16Num(uint8_t x, uint8_t y, uint16_t num);
void OLED_CLS_RECT(uint8_t x, uint8_t y, uint8_t row, uint8_t column);
void OLED_Draw_Ver_Line(uint8_t cloumn);
//void OLED_Draw_point(uint8_t column,uint8_t i);

#endif /* OLED_H_ */
