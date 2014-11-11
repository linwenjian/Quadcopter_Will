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
#include "quad_common.h"
#include "quad_i2c_config.h"


i2c_master_state_t master;
i2c_device_t device =
  {
    .address = ACCE_ADDR,
    .baudRate_kbps = ACCE_BAUDRATE
  };
i2c_status_t I2C_acceInit(void)
{
  uint8_t write_value = 0;
  uint8_t acce_reg = 0;
  uint8_t read_temp = 0;
  int16_t x = 0 ,y = 0, z = 0 ;
  uint8_t x_msb = 0 ,y_msb = 0, z_msb = 0 ;
  uint8_t x_lsb = 0 ,y_lsb = 0, z_lsb = 0 ;
  uint8_t show_time = 0;

//  i2c_status_t run_result = kStatus_I2C_Fail;

//  PRINTF("\r\n==I2C Accelerometer Interrupt_Example ==\r\n\r\n");
//  PRINTF("If full-scale is 2g , then 1g = 4096 \r\n");
//  PRINTF("If full-scale is 4g , then 1g = 2048 \r\n");
//  PRINTF("If full-scale is 8g , then 1g = 1024 \r\n");

  I2C_DRV_MasterInit(ACCE_I2C_INSTANCE, &master);

  /*Init Acce chip*/
  acce_reg = ACCE_CTRL_REG1;
  write_value = 0x00;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &write_value, 1, 1000) )
  {
    PRINTF("fail0");
    I2C_DRV_MasterDeinit(ACCE_I2C_INSTANCE);
    return kStatus_I2C_Fail;
  }

  acce_reg = ACCE_XYZ_DATA_CFG;
  if( kStatus_I2C_Success !=
     I2C_DRV_MasterReceiveDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &read_temp, 1, 1000) )
  {
    PRINTF("fail1");
    I2C_DRV_MasterDeinit(ACCE_I2C_INSTANCE);
    return kStatus_I2C_Fail;
  }
  write_value = (read_temp & (~RANGE_MASK)) | kRange8g  ;
  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &write_value, 1, 1000) )
  {
    PRINTF("fail2");
    I2C_DRV_MasterDeinit(ACCE_I2C_INSTANCE);
    return kStatus_I2C_Fail;
  }

  acce_reg = ACCE_CTRL_REG1;
  write_value =  0x01 ;
  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &write_value, 1, 1000) )
  {
    PRINTF("fail3");
    I2C_DRV_MasterDeinit(ACCE_I2C_INSTANCE);
    return kStatus_I2C_Fail;
  }
    return kStatus_I2C_Success ;
}


i2c_status_t I2C_acceInterrupt(void)
{
  uint8_t write_value = 0;
  uint8_t acce_reg = 0;
  uint8_t read_temp = 0;
  int16_t x = 0 ,y = 0, z = 0 ;
  uint8_t x_msb = 0 ,y_msb = 0, z_msb = 0 ;
  uint8_t x_lsb = 0 ,y_lsb = 0, z_lsb = 0 ;
  uint8_t show_time = 0;


    acce_reg = ACCE_STATUS ;
    if( kStatus_I2C_Success !=
       I2C_DRV_MasterReceiveDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &read_temp, 1, 1000))
    {
      PRINTF("fail4");
      I2C_DRV_MasterDeinit(ACCE_I2C_INSTANCE);
      return kStatus_I2C_Fail;
    }

    if( (read_temp & 0x0f )!= 0 )
    {
      x = 0 ;
      y = 0 ;
      z = 0 ;

      acce_reg = ACCE_OUT_X_MSB ;
      I2C_DRV_MasterReceiveDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &x_msb, 1, 1000);

      acce_reg = ACCE_OUT_X_LSB ;
      I2C_DRV_MasterReceiveDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &x_lsb, 1, 1000);

      acce_reg = ACCE_OUT_Y_MSB ;
      I2C_DRV_MasterReceiveDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &y_msb, 1, 1000);

      acce_reg = ACCE_OUT_Y_LSB ;
      I2C_DRV_MasterReceiveDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &y_lsb, 1, 1000);

      acce_reg = ACCE_OUT_Z_MSB ;
      I2C_DRV_MasterReceiveDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &z_msb, 1, 1000);

      acce_reg = ACCE_OUT_Z_LSB ;
      I2C_DRV_MasterReceiveDataBlocking(ACCE_I2C_INSTANCE, &device, &acce_reg, 1, &z_lsb, 1, 1000);

      x = acceDataCombine(x_msb,x_lsb);
      y = acceDataCombine(y_msb,y_lsb);
      z = acceDataCombine(z_msb,z_lsb);

      PRINTF("x = %d , y = %d , z = %d\r\n" ,x,y,z);
  }

  return kStatus_I2C_Success ;
}


int16_t acceDataCombine(uint8_t msb, uint8_t lsb)
{
  int16_t sign = 1;
  uint16_t value = 0;
  /*follow the rules of MISRA-2004*/
  sign = (msb & 0x80) ? (-1) : 1;
  value = msb & 0x7F;
  value = (value << 6U) + ((lsb) >> 2U);
  if (sign == -1)
  {
    value = ((uint16_t)(~value + 1)) & 0x1FFFU;
  }
  value *= sign;
  return value;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
