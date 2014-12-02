#ifndef __QUAD_SEND_DATA_H__
#define __QUAD_SEND_DATA_H__

#include "fsl_device_registers.h"

#pragma pack(1)
typedef struct _trans_user_data
{
    int16_t trans_accel[3];
    int16_t trans_gyro[3];
    int16_t trans_mag[3];
    int16_t trans_roll;
    int16_t trans_pitch;
    int16_t trans_yaw;
}trans_user_data_t;
#pragma pack()

#pragma pack(1)
typedef struct _trans_packet
{
    uint8_t trans_header[3];
    trans_user_data_t user_data;
    uint8_t trans_reserved[4];
    uint8_t sum;
}trans_packet_t;
#pragma pack()

#endif