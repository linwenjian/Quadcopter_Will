/*
 * OLED.c
 *
 *  Created on: May 23, 2013
 *      Author: LiuSX
 */

//#include "Inc\Delay.h"
#include "OLED_DATA.h"
#include "OLED.h"


//common functions
#define DATABIT_LENGTH 8
void getDataBit(uint16_t num);
uint16_t DataBit[DATABIT_LENGTH];
uint16_t DataCount;

//Bit table
const uint8_t zero_bit[8] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
const uint8_t one_bit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
//const uint8_t d_bit[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}; //0~7
const uint8_t d_bit[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

//void Delay_us(uint16_t cnt);
//void Delay_ms(uint16_t cnt);

/*
 * details:
 * 			Delay_us(1)		--> 	5.3us
 * 			Delay_us(10)	-->		19.1us
 * 			Delay_us(100)	-->		156.4us
 * 			Delay_us(1000)	-->		1.528ms
 */
/*void Delay_us(uint16_t cnt)
{
	uint16_t us;
	while(cnt--)
	{
		for(us = 1; us > 0; us--);
	}
}*/

/*
 * details:
 * 			Delay_ms(1)		--> 	1.07ms
 * 			Delay_ms(10)	-->		10.7us
 * 			Delay_ms(100)	-->		106.9us
 * 			Delay_ms(1000)	-->		1.048s
 */
/*void Delay_ms(uint16_t cnt)
{
	while(cnt--)
	{
		Delay_us(700);
	}
}*/

/*
 * description:
 * 			write one byte data to OLED
 */
void OLED_WrDat(uint8_t dat)
{
	uint8_t i=8;
	OLED_DC_SET;
	for(i = 0; i < 8; i++)
	{
		//Delay_us(1);
		OLED_SCL_CLR;
		//Delay_us(1);
		if(dat & 0x80)
		{
			OLED_SDA_SET;
		}
		else
		{
			OLED_SDA_CLR;
		}
		//Delay_us(1);
		OLED_SCL_SET;
		//Delay_us(1);
		dat <<= 1;
	}
}

/*
 * description:
 * 			write one byte command to OLED
 */
void OLED_WrCmd(uint8_t cmd)
{
	uint8_t i=8;
	OLED_DC_CLR;
	for(i = 0; i < 8; i++)
	{
		//Delay_us(1);
		OLED_SCL_CLR;
		//Delay_us(1);
		if(cmd & 0x80)
		{
			OLED_SDA_SET;
		}else
		{
			OLED_SDA_CLR;
		}
		//Delay_us(1);
		OLED_SCL_SET;
		//Delay_us(1);
		cmd <<= 1;
	}
}

/*
 * description:
 * 			 set the current position in OLED
 * 	parameter:
 * 			x			|			column position
 * 			---------------------------------------
 * 			0-128		|			column
 *  ---------------------------------------------------------
 *  		y			|			row position
 *  		------------------------------------
 *  		0-7			|			row
 */
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WrCmd(0xb0 + y);
	OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
	OLED_WrCmd((x & 0x0f) | 0x01);
}

/*
 * description:
 * 			set the OLED all light
 */
void OLED_Fill(uint8_t bmp_dat)
{
	uint8_t y, x;
	for(y = 0; y < 8; y++)
	{
		OLED_WrCmd(0xb0 + y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x = 0; x < X_WIDTH; x++)
			OLED_WrDat(bmp_dat);
	}
}

/*
 * description:
 * 			clear the OLED scream
 */
void OLED_CLS(void)
{
	uint8_t y, x;
	for(y = 0; y < 8; y++)
	{
		OLED_WrCmd(0xb0 + y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x = 0; x < X_WIDTH; x++)
			OLED_WrDat(0);
	}
}

/*
 * description:
 * 			clear OLED in a particular region
 * 	parameter:
 * 			(x, y) 			-----> set clear position start
 * 			(column, row) 	-----> set clear region
 * 	notice:
 * 			column  : 	0-128
 * 			row 	: 	0-7
 */
void OLED_CLS_RECT(uint8_t x, uint8_t y, uint8_t column, uint8_t row)
{
	uint8_t i, j;
	for(i = x; i < (x+column); i++)
	{
		for(j = y; j < (y+row); j++)
		{
			OLED_Set_Pos(i, j);
			OLED_WrDat(0x00);
			OLED_WrDat(0x00);
		}
	}
}


/*
 * description:
 * 			OLED draw a vertical line in particular column
 * 			column is from 0 to 127
 */
void OLED_Draw_Ver_Line(uint8_t column)
{
	uint8_t i;
	for(i = 0; i <= 7; i++)
	{
		OLED_Set_Pos(column,i);
		OLED_WrDat(0xff);
	}
}
void delayyy() 
{
  uint8_t x,u;
  for(x=100;x>0;x--)
    for(u=200;u>0;u--);
}
#if 0
/*可以显示像素点的函数*/
void OLED_Draw_point(uint8_t column,uint8_t i)
{
	static uint8_t dh=0,xh=0;//,yuu = 0;
	//yuu = 0;
	dh = i / 8;
	xh = i % 8;
	guiji[dh][column] |= d_bit[xh];
	//yuu = yuu | d_bit[xh];
	OLED_Set_Pos(column,dh);
	OLED_WrDat(guiji[dh][column]);
}
#endif
 
/*
 * description:
 * 				OLED init
 */
void OLED_Init(void)
{
	//gpio init
	OLED_SCL_INIT;
	OLED_SDA_INIT;
	OLED_RST_INIT;
	OLED_DC_INIT;
	//set OLED mode
	OLED_SCL_SET;
	OLED_RST_CLR;
	//Delay_us(10);				//After VDD become stable, set RES# pin LOW (logic low) for at least 3us (t1) (4) and then HIGH (logic high).
	OLED_RST_SET;       		//start initial OLED
	//Delay_ms(100);				//After VCC become stable, send command AFh for display ON. SEG/COM will be ON after 100ms
	OLED_WrCmd(0xae);			//--turn off oled panel
	OLED_WrCmd(0x00);			//---set low column address
	OLED_WrCmd(0x10);			//---set high column address
	OLED_WrCmd(0x40);			//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);			//--set contrast control register. default:0x81
	OLED_WrCmd(0xcf); 			// Set SEG Output Current Brightness
	OLED_WrCmd(ScanRowDirection);			//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(ScanColumnDirection);		//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);			//--set normal display(A6) / Inverse display(A7)
	OLED_WrCmd(0xa8);			//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);			//--1/64 duty
	OLED_WrCmd(0xd3);			//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);			//-not offset
	OLED_WrCmd(0xd5);			//--set display clock divide ratio/oscillator frequency   default : 0xd5
	OLED_WrCmd(0x80);			//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);			//--set pre-charge period
	OLED_WrCmd(0xf1);			//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);			//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);			//--set vcomh
	OLED_WrCmd(0x40);			//Set VCOM Deselect Level
	OLED_WrCmd(0x20);			//-Set Page Addressing Mode (0x00/0x01/0x02) 0x20
	OLED_WrCmd(0x02);			//
	OLED_WrCmd(0x8d);			//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);			//--set(0x10) disable
	OLED_WrCmd(DisplayON_OFF);	// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(DisplayMode);	// Disable Inverse Display On (0xa6/a7)
	OLED_WrCmd(0xaf);			//--turn on oled panel
	OLED_Fill(0x00);  			//end initial OLED
	OLED_Set_Pos(0,0);
}


/*
 * description:
 * 			display string ch in specific position
 * 			which determined by x(column) and y(row)
 * 			the font size is: 1 row and 6 column
 * 	example:
 * 			1)	OLED_P8x16Str(45,0,"HuNan");
 * 			2) 	uint8_t txtbuf[32];
 * 				uint8_t TempDataInte = 32;
 * 				uint8_t TempDataDeci = 54;
 * 				sprintf((char *)(txtbuf),"temperature: %d.%d",TempDataInte, TempDataDeci);
 * 				OLED_P6x8Str(0, 5, (char *)txtbuf);
 */
void OLED_P6x8Str(uint8_t x, uint8_t y,char *ch)
{
	uint8_t c=0,i=0,j=0;
	uint8_t *ch1;
	
	ch1 = (uint8_t*)(ch);
	while (ch1[j]!='\0')
	{
		c = ch1[j] - 32;
		if(x > 122)
		{
			x = 0;
			y++;
		}
		OLED_Set_Pos(x,y);
		for(i = 0; i < 6; i++)
			OLED_WrDat(F6x8[c][i]);
		x += 6;
		j++;
	}
}


/*
 * description:
 * 			display string ch in specific position
 * 			which determined by x(column) and y(row)
 * 			the font size is: 2 row and 12 column
 * 	example:
 * 			1)	OLED_P8x16Str(45,0,"HuNan");
 * 			2) 	uint8_t txtbuf[32];
 * 				uint8_t TempDataInte = 32;
 * 				uint8_t TempDataDeci = 54;
 * 				sprintf((char *)(txtbuf),"temperature: %d.%d",TempDataInte, TempDataDeci);
 * 				OLED_P8x16Str(0, 5, (char *)txtbuf);
 */
void OLED_P8x16Str(uint8_t x, uint8_t y,char *ch)
{
	uint8_t c=0,i=0,j=0;
	uint8_t *ch1;
	
	ch1 = (uint8_t*)(ch);
	while (ch1[j] != '\0')
	{
		c =ch1[j] - 32;
		if(x > 116)
		{
			x = 0;
			y += 2;
		}
		OLED_Set_Pos(x,y);
		for(i = 0; i < 8; i++)
		{
			OLED_WrDat(F8X16[c * 16 + i]);
		}
		OLED_Set_Pos(x,y+1);
		for(i = 0; i < 8; i++)
		{
			OLED_WrDat(F8X16[c * 16 + i + 8]);
		}
		x += 8;
		j++;
	}
}


/*
 * description:
 * 			display a unsigned 16 bit number (0-65535) in specific
 * 			position which determined by x(column) and y(row)
 * 			the font size is : 1 row and 6 column
 * 	example:
 * 			OLED_P6x8Num(90, 4, 65123);
 */
void OLED_P6x8Num(uint8_t x, uint8_t y, uint16_t num)
{
	uint8_t c, i, j;
	
	getDataBit(num);
	for(j = DataCount; j > 0; j--)
	{
		c = DataBit[j-1] + 16;
		if(x > 122)
		{
			x = 0;
			y++;
		}
		OLED_Set_Pos(x, y);
		for(i = 0; i < 8; i++)
		{
			OLED_WrDat(F6x8[c][i]);
		}
		x += 6;
	}

	for(j = DataCount + 1; j <= 6; j++)
	{
		OLED_Set_Pos(x, y);
		for(i = 0; i < 8; i++)
		{
			OLED_WrDat(0x00);
		}
		x += 6;
	}
}



/*
 * description:
 * 			display a unsigned 16 bit number (0-65535) in specific
 * 			position which determined by x(column) and y(row)
 * 			the font size is : 2 row and 12 column
 * 	example:
 * 			OLED_P8x16Num(90, 4, 65123);
 */
void OLED_P8x16Num(uint8_t x, uint8_t y, uint16_t num)
{
	uint8_t c, i, j;
	
	getDataBit(num);
	for(j = DataCount; j > 0; j--)
	{
		c = DataBit[j-1] + 16;
		if(x > 116)
		{
			x = 0;
			y += 2;
		}
		OLED_Set_Pos(x,y);
		for(i = 0; i < 8; i++)
		{
			OLED_WrDat(F8X16[c * 16 + i]);
		}
		OLED_Set_Pos(x, y+1);
		for(i = 0; i < 8; i++)
		{
			OLED_WrDat(F8X16[c * 16 + i + 8]);
		}
		x += 8;
	}
	for(j = DataCount + 1; j <= 6; j++)
	{
		OLED_Set_Pos(x, y);
		for(i = 0; i < 8; i++)
		{
			OLED_WrDat(0x00);
		}
		OLED_Set_Pos(x, y + 1);
		for(i = 0; i < 8; i++)
		{
			OLED_WrDat(0x00);
		}
		x += 8;
	}
}


/*
 * description:
 * 			display a picture
 */
void Draw_BMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	unsigned int j=0;
	uint8_t x,y;
	if(y1 % 8 == 0)
	{
		y = y1 / 8;
	}
	else
	{
		y = y1 / 8 + 1;
	}
	for(y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0,y);
		for(x = x0; x < x1; x++)
	    {
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}

/*
 * description:
 * 			get what the input data every bit and how many is...
 * 			the DataBits[5] is for save the bits
 * 			the DataCount is for count the bits
 * 			both DataBits[5] and DataCount are global variables
 *  example:
 *  		getDataBit(3569);
 *  	the result of this commnd is:
 *  		DataBits[5] = {0,3,5,6,9};
 *  		DataCount = 4;
 *  notice:
 *  		the runtime of this function is less than use % method...
 */
void getDataBit(uint16_t num)
{
	uint16_t tmp;
	uint8_t i;
	
	// get every digit
	tmp = num;
	for(i = 0; i < DATABIT_LENGTH; i++)
	{
		DataBit[i] = tmp % 10;
		tmp /= 10;
		if(tmp == 0)
		{
			break;
		}
	}
	// get length
	DataCount = i + 1;
	
	// BISHI XIAMIAN DE DAIMA
	/*
	uint16_t num1, num2, num3, num4;
	//get the DataBit[4]...
	if(num >= 60000)
	{
		DataBit[4] = 6; num1 = num - 60000;
	}else if(num >= 50000)
	{
		DataBit[4] = 5; num1 = num - 50000;
	}else if(num >= 40000)
	{
		DataBit[4] = 4; num1 = num - 40000;
	}else if(num >= 30000)
	{
		DataBit[4] = 3; num1 = num - 30000;
	}else if(num >= 20000)
	{
		DataBit[4] = 2; num1 = num - 20000;
	}else if(num >= 10000)
	{
		DataBit[4] = 1; num1 = num - 10000;
	}else
	{
		DataBit[4] = 0;
		num1 = num;
	}
	//get the DataBit[3]...
	if(num1 >= 9000)
	{
		DataBit[3] = 9; num2 = num1 - 9000;
	}else if(num1 >= 8000)
	{
		DataBit[3] = 8; num2 = num1 - 8000;
	}else if(num1 >= 7000)
	{
		DataBit[3] = 7; num2 = num1 - 7000;
	}else if(num1 >= 6000)
	{
		DataBit[3] = 6; num2 = num1 - 6000;
	}else if(num1 >= 5000)
	{
		DataBit[3] = 5; num2 = num1 - 5000;
	}else if(num1 >= 4000)
	{
		DataBit[3] = 4; num2 = num1 - 4000;
	}else if(num1 >= 3000)
	{
		DataBit[3] = 3; num2 = num1 - 3000;
	}else if(num1 >= 2000)
	{
		DataBit[3] = 2; num2 = num1 - 2000;
	}else if(num1 >= 1000)
	{
		DataBit[3] = 1; num2 = num1 - 1000;
	}else
	{
		DataBit[3] = 0;
		num2 = num1;
	}
	//get the DataBit[2]...
	if(num2 >= 900)
	{
		DataBit[2] = 9; num3 = num2 - 900;
	}else if(num2 >= 800)
	{
		DataBit[2] = 8; num3 = num2 - 800;
	}else if(num2 >= 700)
	{
		DataBit[2] = 7; num3 = num2 - 700;
	}else if(num2 >= 600)
	{
		DataBit[2] = 6; num3 = num2 - 600;
	}else if(num2 >= 500)
	{
		DataBit[2] = 5; num3 = num2 - 500;
	}else if(num2 >= 400)
	{
		DataBit[2] = 4; num3 = num2 - 400;
	}else if(num2 >= 300)
	{
		DataBit[2] = 3; num3 = num2 - 300;
	}else if(num2 >= 200)
	{
		DataBit[2] = 2; num3 = num2 - 200;
	}else if(num2 >= 100)
	{
		DataBit[2] = 1; num3 = num2 - 100;
	}else
	{
		DataBit[2] = 0;
		num3 = num2;
	}
	//get the DataBit[1]...
	if(num3 >= 90)
	{
		DataBit[1] = 9; num4 = num3 - 90;
	}else if(num3 >= 80)
	{
		DataBit[1] = 8; num4 = num3 - 80;
	}else if(num3 >= 70)
	{
		DataBit[1] = 7; num4 = num3 - 70;
	}else if(num3 >= 60)
	{
		DataBit[1] = 6; num4 = num3 - 60;
	}else if(num3 >= 50)
	{
		DataBit[1] = 5; num4 = num3 - 50;
	}else if(num3 >= 40)
	{
		DataBit[1] = 4; num4 = num3 - 40;
	}else if(num3 >= 30)
	{
		DataBit[1] = 3; num4 = num3 - 30;
	}else if(num3 >= 20)
	{
		DataBit[1] = 2; num4 = num3 - 20;
	}else if(num3 >= 10)
	{
		DataBit[1] = 1; num4 = num3 - 10;
	}else
	{
		DataBit[1] = 0;
	}
	//get DataBit[0]...
	DataBit[0] = num4;

	if(DataBit[4] != 0)
	{
		DataCount = 5;
	}else if(DataBit[3] != 0)
	{
		DataCount = 4;
	}else if(DataBit[2] != 0)
	{
		DataCount = 3;
	}else if(DataBit[1] != 0)
	{
		DataCount = 2;
	}else
	{
		DataCount = 1;
	}*/

}








