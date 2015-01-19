/*
 * camera.h
 *
 *  Created on: 2014-9-14
 *      Author: FGZ
 */

#ifndef CAMERA_H_
#define CAMERA_H_
#include "Common.h"
#include "My_DMA.h"

#if 1
  #define USE_DMA_Camera
#endif

extern uint8_t cameraData[lineMax][rowsMax];
extern uint8_t sendFlag;
extern uint16_t line;

void cameraPortInit(void);

#endif /* CAMERA_H_ */
