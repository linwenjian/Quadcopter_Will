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
//    #define FXOS8700_ADDR       0x1DU //FXOS8700,SA0=1,SA1=0
//    #define FXOS8700_WHOAMI_VAL 0xC7U //FXOS8700 Who_am_I Value
//    #define FXOS8700_I2C_INSTANCE  BOARD_FXOS8700_I2C_INSTANCE
//#endif

/* The FXOS8700 i2c instance and slave address */
#define FXOS8700_I2C_INSTANCE  (0U)
#define FXOS8700_ADDR      0x69//(0x1DU)
#define FXOS8700_BAUDRATE  (100)

#define FXOS8700_STATUS        0x00
#define FXOS8700_WHOAMI        0x75//0x0D
#define FXOS8700_XYZ_DATA_CFG  0x0E
#define FXOS8700_HP_FILTER_CUTOFF   0x0F
#define FXOS8700_CTRL_REG1     0x2AU

#define FXOS8700_M_CTRL_REG1    0x5B
#define FXOS8700_M_CTRL_REG2    0x5C
#define FXOS8700_WHOAMI_VAL     0x68//0xC7

// number of bytes to be read from the FXOS8700
#define FXOS8700_READ_LEN       13     // status plus 6 channels = 13 bytes

#define FXOS8700_OUT_X_MSB     0x01U
#define FXOS8700_OUT_X_LSB     0x02U
#define FXOS8700_OUT_Y_MSB     0x03U
#define FXOS8700_OUT_Y_LSB     0x04U
#define FXOS8700_OUT_Z_MSB     0x05U
#define FXOS8700_OUT_Z_LSB     0x06U


/* The L3G4200D i2c instance and slave address */
#define GYRO_I2C_INSTANCE  (0U)
#define GYRO_ADDR          (0x68U)  //1101000b
#define GYRO_BAUDRATE      (400)
#define GYRO_ID            0xD3            /* Gyroscope ID (Who am I value)*/

//#define GYRO_I2C_ADDR   0x68            /* Gyroscope 7-bit I2C address        */
//#define GYRO_ID         0xD3            /* Gyroscope ID (Who am I value)      */
//
//#define GYRO_I2C_ADDR_2 0x6A            /* Gyroscope 7-bit I2C address 2      */
//#define GYRO_ID_2       0xD4            /* This driver also works with L3GD20 */

/* Register addresses */
#define GYRO_WHO_AM_I        0x0FU
#define GYRO_CTRL_REG1       0x20U
#define GYRO_CTRL_REG4       0x23U
#define GYRO_CTRL_REG5       0x24U
#define GYRO_OUT_X_LSB       0x28U
#define GYRO_OUT_X_MSB       0x29U
#define GYRO_OUT_Y_LSB       0x2AU
#define GYRO_OUT_Y_MSB       0x2BU
#define GYRO_OUT_Z_LSB       0x2CU
#define GYRO_OUT_Z_MSB       0x2DU

#define GYRO_READ_LEN       6

typedef struct _mems_date
{
  double accel_x;
  double accel_y;
  double accel_z;
  double magn_x;
  double magn_y;
  double magn_z;
  double gyro_x;
  double gyro_y;
  double gyro_z;
} mems_data_t;

#endif
/*******************************************************************************
* EOF
*******************************************************************************/
