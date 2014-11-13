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
i2c_device_t acce_device =
  {
    .address = FXOS8700_ADDR,
    .baudRate_kbps = FXOS8700_BAUDRATE
  };
i2c_device_t gyro_device =
  {
    .address = GYRO_ADDR,
    .baudRate_kbps = GYRO_BAUDRATE
  };

int16_t acce_x = 0 ,acce_y = 0, acce_z = 0 ;
int16_t gyro_x = 0 ,gyro_y = 0, gyro_z = 0 ;

uint8_t fxo8700_buffer[FXOS8700_READ_LEN];
uint8_t gyro_buffer[GYRO_READ_LEN];



i2c_status_t I2C_acceInit(void)
{
  uint8_t write_value = 0;
  uint8_t acce_reg = 0;
  uint8_t read_temp = 0;
  uint8_t x_msb = 0 ,y_msb = 0, z_msb = 0 ;
  uint8_t x_lsb = 0 ,y_lsb = 0, z_lsb = 0 ;
  uint8_t show_time = 0;

//  i2c_status_t run_result = kStatus_I2C_Fail;

//  PRINTF("\r\n==I2C Accelerometer Interrupt_Example ==\r\n\r\n");
//  PRINTF("If full-scale is 2g , then 1g = 4096 \r\n");
//  PRINTF("If full-scale is 4g , then 1g = 2048 \r\n");
//  PRINTF("If full-scale is 8g , then 1g = 1024 \r\n");

  I2C_DRV_MasterInit(FXOS8700_I2C_INSTANCE, &master);

  //who am I
  acce_reg = FXOS8700_WHOAMI;
  I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &read_temp, 1, 100);
  if (read_temp != FXOS8700_WHOAMI_VAL )
  {
    PRINTF("no a FXO8700 chip");
    return kStatus_I2C_Fail;
  }

  // write 0000 0000 = 0x00 to accelerometer control register 1 to place FXOS8700 into standby mode ,
  // then you can do other settings.
  // [7-1] = 0000 000
  // [0]: active=0
  acce_reg = FXOS8700_CTRL_REG1;
  write_value = 0x00;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }


  // write 0001 1111 = 0x1F to magnetometer control register 1
  // [7]: m_acal=0: auto calibration disabled
  // [6]: m_rst=0: no one-shot magnetic reset
  // [5]: m_ost=0: no one-shot magnetic measurement
  // [4-2]: m_os=111=7: 8x oversampling (for 200Hz) to reduce magnetometer noise
  // [1-0]: m_hms=11=3: select hybrid mode with accel and magnetometer active
  acce_reg = FXOS8700_M_CTRL_REG1;
  write_value = 0x1F;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }

  // write 0010 0000 = 0x20 to magnetometer control register 2
  // [7]: reserved
  // [6]: reserved
  // [5]: hyb_autoinc_mode=1 to map the magnetometer registers to follow the accelerometer registers
  //the user may do a burst read of 12 bytes starting from OUT_X_MSB (address 0x1) to read out both
  //the current accelerometer and magnetometer data in one contiguous operation.
  // [4]: m_maxmin_dis=0 to retain default min/max latching even though not used
  // [3]: m_maxmin_dis_ths=0
  // [2]: m_maxmin_rst=0
  // [1-0]: m_rst_cnt=00 to enable magnetic reset each cycle
  acce_reg = FXOS8700_M_CTRL_REG2;
  write_value = 0x20;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }

  // write 0000 0001= 0x01 to XYZ_DATA_CFG register
  // [7]: reserved
  // [6]: reserved
  // [5]: reserved
  // [4]: hpf_out=0
  // [3]: reserved
  // [2]: reserved
  // [1-0]: fs=02 for accelerometer range of +/-8g range with 0.976mg/LSB
  acce_reg = FXOS8700_XYZ_DATA_CFG;
  write_value = 0x02;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }


  // write 0000 1101b = 0x0D to accelerometer control register 1
  // [7-6]: aslp_rate=00
  // [5-3]: dr=001=1 for 200Hz data rate (when in hybrid mode)
  // [2]: lnoise=1 for low noise mode
  // [1]: f_read=0 for normal 16 bit reads
  // [0]: active=1 to take the part out of standby and enable sampling

  acce_reg = FXOS8700_CTRL_REG1;
  write_value = 0x0D;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }

//  acce_reg = FXOS8700_XYZ_DATA_CFG;
//  if( kStatus_I2C_Success !=
//     I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &read_temp, 1, 1000) )
//  {
//    PRINTF("fail1");
//    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
//    return kStatus_I2C_Fail;
//  }
//  write_value = (read_temp & (~RANGE_MASK)) | kRange8g  ;
//  if( kStatus_I2C_Success !=
//     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &write_value, 1, 1000) )
//  {
//    PRINTF("fail2");
//    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
//    return kStatus_I2C_Fail;
//  }
//
//  acce_reg = FXOS8700_CTRL_REG1;
//  write_value =  0x01 ;
//  if( kStatus_I2C_Success !=
//     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &write_value, 1, 1000) )
//  {
//    PRINTF("fail3");
//    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
//    return kStatus_I2C_Fail;
//  }
    return kStatus_I2C_Success ;
}


i2c_status_t I2C_gyroInit(void)
{
  uint8_t write_value = 0;
  uint8_t gyro_reg = 0;
  uint8_t read_temp = 0;
  uint8_t x_msb = 0 ,y_msb = 0, z_msb = 0 ;
  uint8_t x_lsb = 0 ,y_lsb = 0, z_lsb = 0 ;
  uint8_t show_time = 0;

//  I2C_DRV_MasterInit(GYRO_I2C_INSTANCE, &master);


  uint8_t who = 0;

  /*Init gyro chip*/
  gyro_reg = GYRO_WHO_AM_I;




  if( kStatus_I2C_Success !=
     I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &read_temp, 1, 1000) )
  {
    PRINTF("fail0");
  //  I2C_DRV_MasterDeinit(GYRO_I2C_INSTANCE);
    return kStatus_I2C_Fail;
  }
  if(GYRO_ID != read_temp)
  {
    PRINTF("no GYRO chip");
  }

  /* ODR: 100Hz, Cut-Off: 12.5,         */
  gyro_reg = GYRO_CTRL_REG1;
  write_value = 0x0f;
  I2C_DRV_MasterSendDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &write_value, 1, 1000);

  /* Continous update,full scale 250dps */
  gyro_reg = GYRO_CTRL_REG4;
  write_value = 0x00;
  I2C_DRV_MasterSendDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &write_value, 1, 1000);

  /* FIFO Enable                        */
  gyro_reg = GYRO_CTRL_REG5;
  write_value = 0x40;
  I2C_DRV_MasterSendDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &write_value, 1, 1000);

   return kStatus_I2C_Success ;
}





i2c_status_t I2C_acceInterrupt(void)
{
  uint8_t write_value = 0;
  uint8_t acce_reg = 0;
  uint8_t read_temp = 0;

  uint8_t x_msb = 0 ,y_msb = 0, z_msb = 0 ;
  uint8_t x_lsb = 0 ,y_lsb = 0, z_lsb = 0 ;
  uint8_t show_time = 0;


//    acce_reg = FXOS8700_STATUS ;
//    if( kStatus_I2C_Success !=
//       I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &read_temp, 1, 1000))
//    {
//      PRINTF("fail4");
//      I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
//      return kStatus_I2C_Fail;
//    }

 //   if( (read_temp & 0x0f )!= 0 )
    {
//      x = 0 ;
//      y = 0 ;
//      z = 0 ;

      acce_reg = FXOS8700_STATUS ;
      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1,
                                        fxo8700_buffer, FXOS8700_READ_LEN, 100);

//
//      acce_reg = FXOS8700_OUT_X_MSB ;
//      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &x_msb, 1, 1000);
//
//      acce_reg = FXOS8700_OUT_X_LSB ;
//      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &x_lsb, 1, 1000);
//
//      acce_reg = FXOS8700_OUT_Y_MSB ;
//      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &y_msb, 1, 1000);
//
//      acce_reg = FXOS8700_OUT_Y_LSB ;
//      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &y_lsb, 1, 1000);
//
//      acce_reg = FXOS8700_OUT_Z_MSB ;
//      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &z_msb, 1, 1000);
//
//      acce_reg = FXOS8700_OUT_Z_LSB ;
//      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &acce_device, &acce_reg, 1, &z_lsb, 1, 1000);

      acce_x = DataCombine(fxo8700_buffer[1],fxo8700_buffer[2]);
      acce_y = DataCombine(fxo8700_buffer[3],fxo8700_buffer[4]);
      acce_z = DataCombine(fxo8700_buffer[5],fxo8700_buffer[6]);

      PRINTF("acce_x = %5d , acce_y = %5d , acce_z = %5d\r\n" ,acce_x,acce_y,acce_z);
  }

  return kStatus_I2C_Success ;
}


i2c_status_t I2C_gyroInterrupt(void)
{
//  uint8_t write_value = 0;
  uint8_t gyro_reg = 0;
//  uint8_t read_temp = 0;

//  uint8_t x_msb = 0 ,y_msb = 0, z_msb = 0 ;
//  uint8_t x_lsb = 0 ,y_lsb = 0, z_lsb = 0 ;
//  uint8_t show_time = 0;


//    gyro_reg = gyro_STATUS ;
//    if( kStatus_I2C_Success !=
//       I2C_DRV_MasterReceiveDataBlocking(gyro_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &read_temp, 1, 1000))
//    {
//      PRINTF("fail4");
//      I2C_DRV_MasterDeinit(gyro_I2C_INSTANCE);
//      return kStatus_I2C_Fail;
//    }

//    if( (read_temp & 0x0f )!= 0 )
    {
//      x = 0 ;
//      y = 0 ;
//      z = 0 ;

      /*******************
      In order to read multiple bytes, it is necessary to assert the most significant bit of the sub-
      address field. In other words, SUB(7) must be equal to 1, while SUB(6-0) represents the
      address of the first register to be read.
      *******************/
      gyro_reg = (GYRO_OUT_X_LSB | 0x80) ;
      I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1,
                                        gyro_buffer, GYRO_READ_LEN, 100);

//      gyro_reg = GYRO_OUT_X_MSB ;
//      I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &x_msb, 1, 1000);
//
//      gyro_reg = GYRO_OUT_X_LSB ;
//      I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &x_lsb, 1, 1000);
//
//      gyro_reg = GYRO_OUT_Y_MSB ;
//      I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &y_msb, 1, 1000);
//
//      gyro_reg = GYRO_OUT_Y_LSB ;
//      I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &y_lsb, 1, 1000);
//
//      gyro_reg = GYRO_OUT_Z_MSB ;
//      I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &z_msb, 1, 1000);
//
//      gyro_reg = GYRO_OUT_Z_LSB ;
//      I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &z_lsb, 1, 1000);

//      gyro_x = DataCombine(x_msb,x_lsb);
//      gyro_y = DataCombine(y_msb,y_lsb);
//      gyro_z = DataCombine(z_msb,z_lsb);

      gyro_x = DataCombine(gyro_buffer[1],gyro_buffer[0]);
      gyro_y = DataCombine(gyro_buffer[3],gyro_buffer[2]);
      gyro_z = DataCombine(gyro_buffer[5],gyro_buffer[4]);

      PRINTF("gyro_x = %5d , gyro_y = %5d , gyro_z = %5d\r\n\r\n" ,gyro_x,gyro_y,gyro_z);
  }

  return kStatus_I2C_Success ;
}






int16_t DataCombine(uint8_t msb, uint8_t lsb)
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
