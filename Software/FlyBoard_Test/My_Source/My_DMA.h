/*
 * My_DMA.h
 *
 *  Created on: 2014-8-12
 *      Author: FGZ
 */

#ifndef MY_DMA_H_
#define MY_DMA_H_

#define TransferNums 10
#if 0
  #define memory_to_memory
#else
  #define peripheral_to_memory
#endif

void getCameraData(void);
void init_DMA(void);

#endif /* MY_DMA_H_ */
