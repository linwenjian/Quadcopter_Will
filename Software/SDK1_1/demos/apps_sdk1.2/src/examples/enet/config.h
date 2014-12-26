/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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
#ifndef __CONFIG_H__
#define __CONFIG_H__

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "fsl_enet_driver.h"

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

//Define parameter for configuration.
#define ENET_RXBD_NUM           (6)
#define ENET_TXBD_NUM           (4)
#define ENET_EXTRXBD_NUM           (4)
#define ENET_RXBUFF_SIZE        (kEnetMaxFrameSize)
#define ENET_TXBUFF_SIZE        (kEnetMaxFrameSize)
#define ENET_RXLARGEBUFF_NUM    (3)
#define ENET_RX_BUFFER_ALIGNMENT     (16)
#define ENET_TX_BUFFER_ALIGNMENT     (16)
#define ENET_BD_ALIGNMENT            (16)
#define ENET_MII_CLOCK               (2500000L)
#define ENET_RXBuffSizeAlign(n)      ENET_ALIGN(n, ENET_RX_BUFFER_ALIGNMENT)
#define ENET_TXBuffSizeAlign(n)      ENET_ALIGN(n, ENET_TX_BUFFER_ALIGNMENT)
#if FSL_FEATURE_ENET_SUPPORT_PTP
#define ENET_PTP_TXTS_RING_LEN           (25)
#define ENET_PTP_RXTS_RING_LEN           (25)
#define ENET_PTP_L2RING_LEN            (15)
#endif

#define BOARD_DEBUG_ENET_INSTANCE (0)
#define SEND_TASK_PRIO        (1)
#define ENET_TASK_STACK_SIZE     (2000)
#define SEND_TASK_STACK_SIZE     (2000)
#define RECEIVE_TASK_STACK_SIZE  (2000)
#define TIMESTAMP_TASK_STACK_SIZE  (512)
#define ENET_TRANSMIT_SIDE    (1)
#define ENET_RECEIVE_SIDE     (0)
#define ENET_DATA_LENGTH      (1300)

//Define the Macro to get variable aligned on 16-bytes except IAR compiler.
#if defined   (__GNUC__)        // GNU Compiler
  #ifndef __ALIGN_END
    #define __ALIGN_END    __attribute__ ((aligned (ENET_BD_ALIGNMENT)))
  #endif
  #ifndef __ALIGN_BEGIN
    #define __ALIGN_BEGIN
  #endif
#else
  #ifndef __ALIGN_END
    #define __ALIGN_END
  #endif
  #ifndef __ALIGN_BEGIN
    #if defined   (__CC_ARM)      // ARM Compiler
      #define __ALIGN_BEGIN    __align(ENET_BD_ALIGNMENT)
    #elif defined (__ICCARM__)    // IAR Compiler
      #define __ALIGN_BEGIN
    #endif
  #endif
#endif

#endif // __CONFIG_H__

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////