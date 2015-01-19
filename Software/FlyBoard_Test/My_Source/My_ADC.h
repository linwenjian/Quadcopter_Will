/*
 * My_ADC.h
 *
 *  Created on: 2014-8-10
 *      Author: FGZ
 */

#ifndef MY_ADC_H_
#define MY_ADC_H_

void init_ADC(void);
void get_ADC_Val(uint32_t *val);
void battary_V_dis(uint8_t x, uint8_t y,uint32_t batV);

#endif /* MY_ADC_H_ */
