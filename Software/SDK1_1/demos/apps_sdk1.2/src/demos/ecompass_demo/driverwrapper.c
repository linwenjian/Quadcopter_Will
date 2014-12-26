// -----------------------------------------------------------------------
// Copyright (c) 2011, 2012, 2013 Freescale Semiconductor, Inc.
// ----------------------------------------------------------------------------
// FILENAME:     sensordriver.c
// CONTENTS:     SENSOR DRIVERS
// DEPARTMENT:   ANALOG AND SENSORS DIVISION
// AUTHOR:       MARK PEDLEY
// ----------------------------------------------------------------------------
// VERSION             DATE           AUTHOR
// V1.0                20 FEB 2012    MARK PEDLEY
// V1.1                1 JUN 2012     MARK PEDLEY
// V1.2                15 JUN 2012    MARK PEDLEY
// V2.0                3 JUL 2012     MARK PEDLEY
// V3.0                18 MAR 2013    MARK PEDLEY
// V4.0                29 July 2014   Lionel Xu
// V5.0                25 Aug 2014    Lionel Xu
// ----------------------------------------------------------------------------
// PURPOSE:
// Tilt-compensated e-compass with magnetic calibration with support
// for Aerospace (NED), Android and Windows 8 coordinate systems.
// This code includes low level drivers for the accelerometer
// and magnetometer sensors.
// ----------------------------------------------------------------------------
// V4.0 ENHANCEMENTS:
// - Implement the low layer sensor driver for ecompass system
// ----------------------------------------------------------------------------

#include "include.h"
#include "board.h"
#include "stdio.h"

#include "fsl_fxos8700_driver.h"

//==============================================================
// I2C COMMUNICATION DATA
//--------------------------------------------------------------
volatile uint8 I2C2_Status = 0;         // volatile since it is changed by I2C callback functions

static fxos_handler_t fxos_handler;

uint8 SensorDrivers_Init()
{
    printf("\r\n== Accelerometer Test ==\r\n\r\n");

    fxos_handler.i2cInstance = 0;

    if (FXOS_Init(&fxos_handler, NULL) != kStatus_FXOS_Success)
    {
        printf("Failed to initialize the Accelerometer device!\r\n");
        return 1;
    }
    return 0;
}

uint8 fSixDOFSensorDrivers(struct AccelSensor *pthisAccel, struct MagSensor *pthisMag)
{
    // this function gets the real accelerometer and magnetometer sensor outputs
    // for random Euler angles (roll, pitch and yaw) under the NED (Aerospace),
    // Android and Win8 coordinate systems for the purpose of validating the eCompass
    // and magnetic calibration software independent of physical sensors.

    // it is vital that the x, y and z outputs from the real sensors are correctly
    // aligned with the NED, Android or Windows 8 coordinate system selected. the
    // raw x, y and z sensor data read from the accelerometer and magnetometer
    // sensors are in the sensor coordinate frame. the sensors frames of reference
    // will in general be rotated relative to each other and to the coordinate
    // system of the final product as a result of PCB layout decisions taken.

    // calculate the accelerometer output in units of g

    struct AccelSensor thisAccel;
    struct MagSensor thisMag;

    fxos_data_t fxos_data;

    /* Read acceleration data. */
    if (FXOS_ReadData(&fxos_handler, &fxos_data) != kStatus_FXOS_Success)
    {
         printf("Failed to read acceleration data!\r\n");
         return -1;
    }

    /* merge and align data */
    pthisAccel->iGpx = (int16_t)((fxos_data.accelXMSB << 8) | (fxos_data.accelXLSB));
    pthisAccel->iGpy = (int16_t)((fxos_data.accelYMSB << 8) | (fxos_data.accelYLSB));
    pthisAccel->iGpz = (int16_t)((fxos_data.accelZMSB << 8) | (fxos_data.accelZLSB));

    /* right shift 2 bits */
    pthisAccel->iGpx = pthisAccel->iGpx >> 2;
    pthisAccel->iGpy = pthisAccel->iGpy >> 2;
    pthisAccel->iGpz = pthisAccel->iGpz >> 2;

    /* apply the HAL to the raw accelerometer data */
    pthisAccel->iGpx = -pthisAccel->iGpx;

    /* calculate float point format accelerometer data */
    pthisAccel->fGpx = (float) (pthisAccel->iGpx / FCOUNTSPERG);
    pthisAccel->fGpy = (float) (pthisAccel->iGpy / FCOUNTSPERG);
    pthisAccel->fGpz = (float) (pthisAccel->iGpz / FCOUNTSPERG);

    /* get magnetometer sensor reading iBpxyz in bit counts */
    pthisMag->iBpx = ((int16_t)((fxos_data.magXMSB << 8) | (fxos_data.magXLSB)));
    pthisMag->iBpy = ((int16_t)((fxos_data.magYMSB << 8) | (fxos_data.magYLSB)));
    pthisMag->iBpz = ((int16_t)((fxos_data.magZMSB << 8) | (fxos_data.magZLSB)));

    /* apply the HAL to the raw magnetometer data */
    pthisMag->iBpy = -pthisMag->iBpy;
    pthisMag->iBpz = -pthisMag->iBpz;

    /* calculate float-point magnetometer data */
    pthisMag->fBpx = (float) (pthisMag->iBpx / FCOUNTSPERUT);
    pthisMag->fBpy = (float) (pthisMag->iBpy / FCOUNTSPERUT);
    pthisMag->fBpz = (float) (pthisMag->iBpz / FCOUNTSPERUT);

    return 0;
}
