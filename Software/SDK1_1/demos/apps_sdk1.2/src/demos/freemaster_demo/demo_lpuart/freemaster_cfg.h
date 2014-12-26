/***************************************************************************//*!
*
* @file   freemaster_cfg.h
*
* @brief  FreeMASTER Serial Communication Driver configuration file
*
*******************************************************************************/

#ifndef __FREEMASTER_CFG_H
#define __FREEMASTER_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! Select interrupt or poll-driven serial communication
#define FMSTR_LONG_INTR         0   //!< Complete message processing in interrupt
#define FMSTR_SHORT_INTR        1   //!< Queuing done in interrupt
#define FMSTR_POLL_DRIVEN       0   //!< No interrupt needed, polling only

//! Select communication interface (SCI, CAN, USB CDC or Packet Driven BDM)
//! Define communication interface base address or leave undefined for runtime setting
// #undef FMSTR_SCI_BASE   //!< SCI base will be assigned in runtime (when FMSTR_USE_SCI)
// #undef FMSTR_CAN_BASE   //!< CAN base will be assigned in runtime (when FMSTR_USE_FLEXCAN)

//! Select communication interface
#define FMSTR_DISABLE           0   //!< To disable all FreeMASTER functionalities
#define FMSTR_USE_SCI           0   //!< To select SCI/UART communication interface
#define FMSTR_USE_LPUART        1   //!< To select LPUART communication interface */
#define FMSTR_USE_FLEXCAN       0   //!< To select FlexCAN communication interface */
#define FMSTR_USE_USB_CDC       0   //!< To select USB CDC Class Driver - virtual serial

//! FlexCAN-specific, communication message buffers
#define FMSTR_FLEXCAN_TXMB      0
#define FMSTR_FLEXCAN_RXMB      1

//! Input/output communication buffer size
#define FMSTR_COMM_BUFFER_SIZE  0   //!< Set to 0 for "automatic"

//! Receive FIFO queue size (use with FMSTR_SHORT_INTR only)
#define FMSTR_COMM_RQUEUE_SIZE  32  //!< Set to 0 for "default"

//! Support for Application Commands 
#define FMSTR_USE_APPCMD        1   //!< Enable/disable App.Commands support
#define FMSTR_APPCMD_BUFF_SIZE  32  //!< App.Command data buffer size
#define FMSTR_MAX_APPCMD_CALLS  4   //!< How many app.cmd callbacks? (0=disable)

#ifdef FMSTR_SMALL_RAM_CONFIG
//! When RAM resources are limited, exclude some of the FreeMASTER features by default
#define FMSTR_USE_RECORDER      0   //!< Enable/disable recorder support
#define FMSTR_USE_SCOPE         0   //!< Enable/disable scope support
#define FMSTR_USE_TSA_SAFETY    0   //!< Enable access to TSA variables only
#else
//! When there is enough RAM, all FreeMASTER features can be enabled by default
#define FMSTR_USE_RECORDER      1
#define FMSTR_USE_SCOPE         1
#define FMSTR_USE_TSA_SAFETY    1
#endif

//! Oscilloscope support
#define FMSTR_MAX_SCOPE_VARS    8   //!< Max. number of scope variables (2..8)

//! Recorder support
#define FMSTR_MAX_REC_VARS      8   //! Max. number of recorder variables (2..8)
#define FMSTR_REC_OWNBUFF       0   //! Use user-allocated rec. buffer (1=yes)

//! Built-in recorder buffer (use when FMSTR_REC_OWNBUFF is 0)
#define FMSTR_REC_BUFF_SIZE     1024    //!< Built-in buffer size

//! Recorder time base, specifies how often the recorder is called in the user app.
#define FMSTR_REC_TIMEBASE      FMSTR_REC_BASE_MILLISEC(0)  //!< 0 = "unknown"

#define FMSTR_REC_FLOAT_TRIG    0   //!< Enable/disable floating point triggering

//!< Target-side address translation (TSA)
#define FMSTR_USE_TSA           1   //!< Enable TSA functionality
#define FMSTR_USE_TSA_INROM     1   //!< TSA tables declared as const (put to ROM)

//!< Enable/Disable read/write memory commands
#define FMSTR_USE_READMEM       1   //!< Enable read memory commands
#define FMSTR_USE_WRITEMEM      1   //!< Enable write memory commands
#define FMSTR_USE_WRITEMEMMASK  1   //!< Enable write memory bits commands

//! Enable/Disable read/write variable commands (a bit faster than Read Mem)
#define FMSTR_USE_READVAR       1   //!< Enable read variable fast commands
#define FMSTR_USE_WRITEVAR      1   //!< Enable write variable fast commands
#define FMSTR_USE_WRITEVARMASK  1   //!< Enable write variable bits fast commands

#endif /* __FREEMASTER_CFG_H */

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
