/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __QUAD_I2C_COFIG_H__
#define __QUAD_I2C_COFIG_H__

//#if defined (FRDM_K64F120M)
//    #define ACCE_ADDR       0x1DU //FXOS8700,SA0=1,SA1=0
//    #define ACCE_WHOAMI_VAL 0xC7U //FXOS8700 Who_am_I Value
//    #define ACCE_I2C_INSTANCE  BOARD_FXOS8700_I2C_INSTANCE
//#endif

/* The FXOS8700 i2c instance and slave address */
#define ACCE_I2C_INSTANCE  (0U)
#define ACCE_ADDR             (0x1DU)
#define ACCE_BAUDRATE (100)

#define ACCE_STATUS        0x00U
#define ACCE_XYZ_DATA_CFG  0x0EU
#define ACCE_CTRL_REG1     0x2AU
#define ACCE_WHOAMI        0x0DU

#define ACCE_OUT_X_MSB     0x01U
#define ACCE_OUT_X_LSB     0x02U
#define ACCE_OUT_Y_MSB     0x03U
#define ACCE_OUT_Y_LSB     0x04U
#define ACCE_OUT_Z_MSB     0x05U
#define ACCE_OUT_Z_LSB     0x06U


/* The L3G4200D i2c instance and slave address */
#define GYRO_I2C_INSTANCE  (0U)
#define GYRO_ADDR              (0x68U)  //1101000b
#define GYRO_BAUDRATE (100)
#define GYRO_ID         0xD3            /* Gyroscope ID (Who am I value)*/

//#define GYRO_I2C_ADDR   0x68            /* Gyroscope 7-bit I2C address        */
//#define GYRO_ID         0xD3            /* Gyroscope ID (Who am I value)      */
//
//#define GYRO_I2C_ADDR_2 0x6A            /* Gyroscope 7-bit I2C address 2      */
//#define GYRO_ID_2       0xD4            /* This driver also works with L3GD20 */

/* Register addresses */
#define GYRO_WHO_AM_I        0x0F
#define GYRO_CTRL_REG1       0x20
#define GYRO_CTRL_REG4       0x23
#define GYRO_CTRL_REG5       0x24
#define GYRO_OUT_X_L         0x28
#define GYRO_OUT_X_H         0x29
#define GYRO_OUT_Y_L         0x2A
#define GYRO_OUT_Y_H         0x2B
#define GYRO_OUT_Z_L         0x2C
#define GYRO_OUT_Z_H         0x2D





#endif
/*******************************************************************************
* EOF
*******************************************************************************/
