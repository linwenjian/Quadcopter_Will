#ifndef _LED_H_
#define _LED_H_
#include "stm32f10x.h"
#include "tim3.h"		

#define LED1_OFF  		GPIO_SetBits(GPIOA, GPIO_Pin_15);
#define LED1_ON 			GPIO_ResetBits(GPIOA, GPIO_Pin_15);
//#define LED2_OFF  		GPIO_SetBits(GPIOB, GPIO_Pin_13);
//#define LED2_ON 			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
//#define LED3_OFF  		GPIO_SetBits(GPIOB, GPIO_Pin_14);
//#define LED3_ON 			GPIO_ResetBits(GPIOB, GPIO_Pin_14);

void LED_INIT(void);
void LED_FLASH(void);
void LED1_ONOFF(void);
//void LED2_ONOFF(void);
//void LED3_ONOFF(void);

#endif
