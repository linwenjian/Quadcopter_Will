/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
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
#ifndef __ACCEL_H__
#define __ACCEL_H__

#include "fsl_i2c_master_driver.h"
#include <stdio.h>
#include <stdint.h>

/*!
 * @addtogroup fxos_common
 * @{
 */

/*!
 * @file
 */
     
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ACCEL_MMA8451   1
#define ACCEL_FXOS      2

typedef union
{
    struct
    {
        uint32_t accelX;
        uint32_t accelY;
        uint32_t accelZ;
    } mma8451_data;
    struct
    {
        uint8_t accelXMSB;                      	
        uint8_t accelXLSB;                      	
        uint8_t accelYMSB;                      	
        uint8_t accelYLSB;                      	
        uint8_t accelZMSB;                      	
        uint8_t accelZLSB;
        uint8_t magXMSB;                        	
        uint8_t magXLSB;                        	
        uint8_t magYMSB;                        	
        uint8_t magYLSB;                        	
        uint8_t magZMSB;                        	
        uint8_t magZLSB;                        	
    } fxos_data;
} accel_sensor_data_t;

/*
typedef struct
{
    void    (*i2c_init)(int32_t speed, int32_t slaveaddr);
    uint8_t (*i2c_probe)(uint8_t addr);
    uint8_t (*i2c_read) (uint8_t chip, uint8_t addr, int32_t alen, uint8_t *buf, int32_t len);
    uint8_t (*i2c_write)(uint8_t chip, uint8_t addr, int32_t alen, uint8_t *buf, int32_t len);
} fxos_i2c_interface_t;
*/
typedef struct
{
    void    (*i2c_init)(int32_t speed, int32_t slaveaddr);
    uint8_t (*i2c_probe)(uint8_t addr);
    i2c_status_t (*i2c_read) (uint32_t instance, i2c_device_t const *chip, uint8_t * addr,
                                uint32_t alen, uint8_t *buf, uint32_t len, uint32_t timeout);
    i2c_status_t (*i2c_write)(uint32_t instance, i2c_device_t const *chip, uint8_t * addr,
                                uint32_t alen, uint8_t *buf, uint32_t len, uint32_t timeout);
} accel_i2c_interface_t;

struct accel_dev_interface;
struct accel_device;

struct accel_dev_interface
{
    int32_t (* accel_init)(struct accel_device *dev);
    int32_t (* accel_read_sensor_data)(const struct accel_device *dev, accel_sensor_data_t *accel);
};

struct accel_device
{
    uint8_t bus; 
    struct accel_dev_interface *accel;
    accel_i2c_interface_t *i2c;
    i2c_device_t slave;
};

typedef struct accel_dev_interface accel_dev_interface_t;
typedef struct accel_device accel_dev_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the accel driver instance.
 *
 * @param device Device driver state structure that will be filled in by this function.
 *      It is the responsibility of the caller to provide storage for this structure, and
 *      to free that storage when the driver is no longer needed.
 * @param isSaoSet
 * @param baud_kbps
 *
 * @return kStatus_Success or error code.
 * @retval kStatus_Success
 * @retval kAccel_Invalid_I2C_Address_Error
 */
int32_t accel_init(accel_dev_t *device);

/*!
 * @brief Read the current acceleration values.
 *
 * @param device Pointer to a valid device instance structure.
 * @param accel Pointer to a structure that will be filled in with the current acceleration
 *      values for all three axes.
 *
 * @return kStatus_Success or error code.
 * @retval kStatus_Success
 */
int32_t accel_read_sensor_data(const accel_dev_t *device, accel_sensor_data_t *accel);

/*!
 * @brief Read a single register.
 *
 * @param device Pointer to a valid device instance structure.
 * @param reg_addr Address of the internal accel register to read.
 *
 * @return The register's value.
 */
uint8_t accel_read_reg(const accel_dev_t * device, uint8_t reg_addr);

/*!
 * @brief Write a single register of the accel.
 *
 * @param device Pointer to a valid device instance structure.
 * @param reg_addrAddress of the internal accel register to change the value of.
 * @param reg_val New value for the register.
 *
 * @return kStatus_Success or error code.
 * @retval kStatus_Success
 */
int32_t accel_write_reg(const accel_dev_t * device, uint8_t reg_addr, uint8_t reg_val);

#if defined(__cplusplus)
}
#endif

#endif /* _ACCEL_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
