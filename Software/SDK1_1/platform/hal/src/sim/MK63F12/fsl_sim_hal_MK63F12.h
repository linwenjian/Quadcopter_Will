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

#if !defined(__FSL_SIM_HAL_K63F12_H__)
#define __FSL_SIM_HAL_K63F12_H__

/*! @addtogroup sim_hal*/
/*! @{*/

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief SIM SDHC clock source */
typedef enum _sim_sdhc_clock_source
{
    kSimSdhcSrcCoreSysClk,  /* Core/system clock */
    kSimSdhcSrcPllFllSel,   /* clock as selected by SOPT2[PLLFLLSEL]. */
    kSimSdhcSrcOscerclk,    /* OSCERCLK clock */
    kSimSdhcSrcExt          /* External bypass clock (SDHC0_CLKIN) */
} sim_sdhc_clock_source_t;

/*! @brief SIM TIME clock source */
typedef enum _sim_time_clock_source
{
    kSimTimeSrcCoreSysClk,  /* Core/system clock */
    kSimTimeSrcPllFllSel,   /* clock as selected by SOPT2[PLLFLLSEL]. */
    kSimTimeSrcOscerclk,    /* OSCERCLK clock */
    kSimTimeSrcExt          /* ENET 1588 clock in (ENET_1588_CLKIN) */
} sim_time_clock_source_t; 

/*! @brief SIM RMII clock source */
typedef enum _sim_rmii_clock_source
{
    kSimRmiiSrcExtalClk,    /* EXTAL Clock */
    kSimRmiiSrcExt          /* ENET 1588 clock in (ENET_1588_CLKIN) */
} sim_rmii_clock_source_t;

/*! @brief SIM USB clock source */
typedef enum _sim_usb_clock_source
{
    kSimUsbSrcClkIn,       /* USB CLKIN Clock */
    kSimUsbSrcPllFllSel    /* clock as selected by SOPT2[PLLFLLSEL] */
} sim_usb_clock_source_t;

/*! @brief SIM PLLFLLSEL clock source select */
typedef enum _sim_pllfll_clock_sel
{
    kSimPllFllSelFll,       /* Fll clock */
    kSimPllFllSelPll        /* Pll0 clock */
} sim_pllfll_clock_sel_t;

/*! @brief SIM OSC32KSEL clock source select */
typedef enum _sim_osc32k_clock_sel
{
    kSimOsc32kSelOsc32k,      /* OSC 32k clock */
    kSimOsc32kSelReserved,    /* Reserved */
    kSimOsc32kSelRtc32k,      /* RTC 32k clock */
    kSimOsc32kSelLpo          /* LPO clock */
} sim_osc32k_clock_sel_t;

/*! @brief SIM TRACESEL clock source select */
typedef enum _sim_trace_clock_sel
{
    kSimTraceMcgoutClk,       /* MCG out clock */
    kSimTraceCoreClk          /* core clock */
} sim_trace_clock_sel_t;

/*! @brief SIM CLKOUT_SEL clock source select */
typedef enum _sim_clkout_clock_sel
{
    kSimClkoutFlexbusClk,       /* Flexbus clock */
    kSimClkoutReserved,         /* Reserved */
    kSimClkoutFlashClk,         /* Flash clock */
    kSimClkoutLpoClk,           /* LPO clock */
    kSimClkoutMcgIrcClk,        /* MCG out clock */
    kSimClkoutRtc32kClk,        /* RTC 32k clock */
    kSimClkoutReserved1         
} sim_clkout_clock_sel_t;

/*! @brief SIM RTCCLKOUTSEL clock source select */
typedef enum _sim_rtcclkout_clock_sel
{
    kSimRtcClkout1hzClk,       /* 1Hz clock */
    kSimRtcClkout32kClk        /* 32KHz clock */
} sim_rtcclkout_clock_sel_t;

/* TODO: Copied directly from fsl_sim_hal.h, will be modified during revisit.*/
/*! @brief SIM USB voltage regulator in standby mode setting during stop modes */
typedef enum _sim_usbsstby_stop
{
    kSimUsbsstbyNoRegulator,        /* regulator not in standby during Stop modes */
    kSimUsbsstbyWithRegulator       /* regulator in standby during Stop modes */
} sim_usbsstby_stop_t;

/*! @brief SIM USB voltage regulator in standby mode setting during VLPR and VLPW modes */
typedef enum _sim_usbvstby_stop
{
    kSimUsbvstbyNoRegulator,        /* regulator not in standby during VLPR and VLPW modes */
    kSimUsbvstbyWithRegulator       /* regulator in standby during VLPR and VLPW modes */
} sim_usbvstby_stop_t;

/*! @brief SIM CMT/UART pad drive strength */
typedef enum _sim_cmtuartpad_strengh
{
    kSimCmtuartSinglePad,           /* Single-pad drive strength for CMT IRO or UART0_TXD */
    kSimCmtuartDualPad              /* Dual-pad drive strength for CMT IRO or UART0_TXD */
} sim_cmtuartpad_strengh_t;

/*! @brief SIM PTD7 pad drive strength */
typedef enum _sim_ptd7pad_strengh
{
    kSimPtd7padSinglePad,           /* Single-pad drive strength for PTD7 */
    kSimPtd7padDualPad              /* Dual-pad drive strength for PTD7 */
} sim_ptd7pad_strengh_t;

/*! @brief SIM FlexBus security level */
typedef enum _sim_flexbus_security_level
{
    kSimFbslLevel0,                 /* All off-chip accesses (op code and data) via the FlexBus */
                                    /* and DDR controller are disallowed */
    kSimFbslLevel1,                 /* Undefined */
    kSimFbslLevel2,                 /* Off-chip op code accesses are disallowed. Data accesses */
                                    /* are allowed */
    kSimFbslLevel3                  /* Off-chip op code accesses and data accesses are allowed */
} sim_flexbus_security_level_t;

/*! @brief SIM ADCx pre-trigger select */
typedef enum _sim_pretrgsel
{
    kSimAdcPretrgselA,              /* Pre-trigger A selected for ADCx */
    kSimAdcPretrgselB               /* Pre-trigger B selected for ADCx */
} sim_pretrgsel_t;

/*! @brief SIM ADCx trigger select */
typedef enum _sim_trgsel
{
    kSimAdcTrgselExt,               /* External trigger */
    kSimAdcTrgSelHighSpeedComp0,    /* High speed comparator 0 asynchronous interrupt */
    kSimAdcTrgSelHighSpeedComp1,    /* High speed comparator 1 asynchronous interrupt */
    kSimAdcTrgSelHighSpeedComp2,    /* High speed comparator 2 asynchronous interrupt */
    kSimAdcTrgSelPit0,              /* PIT trigger 0 */
    kSimAdcTrgSelPit1,              /* PIT trigger 1 */
    kSimAdcTrgSelPit2,              /* PIT trigger 2 */
    kSimAdcTrgSelPit3,              /* PIT trigger 3 */
    kSimAdcTrgSelFtm0,              /* FTM0 trigger */
    kSimAdcTrgSelFtm1,              /* FTM1 trigger */
    kSimAdcTrgSelFtm2,              /* FTM2 trigger */
    kSimAdcTrgSelFtm3,              /* FTM3 trigger */
    kSimAdcTrgSelRtcAlarm,          /* RTC alarm */
    kSimAdcTrgSelRtcSec,            /* RTC seconds */
    kSimAdcTrgSelLptimer,           /* Low-power timer trigger */
    kSimAdcTrgSelHigSpeedComp3      /* High speed comparator 3 asynchronous interrupt */
} sim_trgsel_t;

/*! @brief SIM receive data source select */
typedef enum _sim_uart_rxsrc
{
    kSimUartRxsrcPin,               /* UARTx_RX Pin */
    kSimUartRxsrcCmp0,              /* CMP0 */
    kSimUartRxsrcCmp1,              /* CMP1 */
    kSimUartRxsrcReserved           /* Reserved */
} sim_uart_rxsrc_t;

/*! @brief SIM transmit data source select */
typedef enum _sim_uart_txsrc
{
    kSimUartTxsrcPin,               /* UARTx_TX Pin */
    kSimUartTxsrcCmp0,              /* UARTx_TX pin modulated with FTM1 channel 0 output */
    kSimUartTxsrcCmp1,              /* UARTx_TX pin modulated with FTM2 channel 0 output */
    kSimUartTxsrcReserved           /* Reserved */
} sim_uart_txsrc_t;

/*! @brief SIM FlexTimer x trigger y select */
typedef enum _sim_ftm_trg_src
{
    kSimFtmTrgSrc0,                 /* FlexTimer x trigger y select 0 */
    kSimFtmTrgSrc1                  /* FlexTimer x trigger y select 1 */
} sim_ftm_trg_src_t;

/*! @brief SIM FlexTimer external clock select */
typedef enum _sim_ftm_clk_sel
{
    kSimFtmClkSel0,                 /* FTM CLKIN0 pin. */
    kSimFtmClkSel1                  /* FTM CLKIN1 pin. */
} sim_ftm_clk_sel_t;

/*! @brief SIM FlexTimer x channel y input capture source select */
typedef enum _sim_ftm_ch_src
{
    kSimFtmChSrc0,                 /* See RM for details of each selection for each channel */
    kSimFtmChSrc1,                 /* See RM for details of each selection for each channel */
    kSimFtmChSrc2,                 /* See RM for details of each selection for each channel */
    kSimFtmChSrc3                  /* See RM for details of each selection for each channel */
} sim_ftm_ch_src_t;

/*! @brief SIM FlexTimer x Fault y select */
typedef enum _sim_ftm_flt_sel
{
    kSimFtmFltSel0,                 /* FlexTimer x fault y select 0 */
    kSimFtmFltSel1                  /* FlexTimer x fault y select 1 */
} sim_ftm_flt_sel_t;

/*! @brief SIM Timer/PWM external clock select */
typedef enum _sim_tpm_clk_sel
{
    kSimTpmClkSel0,                 /* Timer/PWM TPM_CLKIN0 pin. */
    kSimTpmClkSel1                  /* Timer/PWM TPM_CLKIN1 pin. */
} sim_tpm_clk_sel_t;

/*! @brief SIM Timer/PWM x channel y input capture source select */
typedef enum _sim_tpm_ch_src
{
    kSimTpmChSrc0,                 /* TPMx_CH0 signal */
    kSimTpmChSrc1                  /* CMP0 output */
} sim_tpm_ch_src_t;
// TODO: END

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name IP related clock feature APIs*/
/*@{*/

/*!
 * @brief Enable the clock for DMA module.
 *
 * This function enables the clock for DMA module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableDmaClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for DMA module.
 *
 * This function disables the clock for DMA module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableDmaClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for DMA module.
 *
 * This function  gets the clock gate state for DMA module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetDmaGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for DMAMUX module.
 *
 * This function enables the clock for DMAMUX module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableDmamuxClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for DMAMUX module.
 *
 * This function disables the clock for DMAMUX module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableDmamuxClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for DMAMUX module.
 *
 * This function will get the clock gate state for DMAMUX module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetDmamuxGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for PORT module.
 *
 * This function enables the clock for PORT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnablePortClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for PORT module.
 *
 * This function disables the clock for PORT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisablePortClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for PORT module.
 *
 * This function will get the clock gate state for PORT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetPortGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for MPU module.
 *
 * This function enables the clock for MPU module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableMpuClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for MPU module.
 *
 * This function disables the clock for MPU module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableMpuClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for MPU module.
 *
 * This function will get the clock gate state for MPU module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetMpuGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for EWM module.
 *
 * This function enables the clock for EWM module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableEwmClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for EWM module.
 *
 * This function disables the clock for EWM module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableEwmClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for EWM module.
 *
 * This function will get the clock gate state for EWM module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetEwmGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for FLEXBUS module.
 *
 * This function enables the clock for FLEXBUS module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableFlexbusClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for FLEXBUS module.
 *
 * This function disables the clock for FLEXBUS module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableFlexbusClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for FLEXBUS module.
 *
 * This function will get the clock gate state for FLEXBUS module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetFlexbusGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for FTF module.
 *
 * This function enables the clock for FTF module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableFtfClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for FTF module.
 *
 * This function disables the clock for FTF module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableFtfClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for FTF module.
 *
 * This function will get the clock gate state for FTF module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetFtfGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for CRC module.
 *
 * This function enables the clock for CRC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableCrcClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for CRC module.
 *
 * This function disables the clock for CRC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableCrcClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for CRC module.
 *
 * This function will get the clock gate state for CRC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetCrcGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for RNGA module.
 *
 * This function enables the clock for RNGA module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableRngaClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for RNGA module.
 *
 * This function disables the clock for RNGA module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableRngaClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for RNGA module.
 *
 * This function will get the clock gate state for RNGA module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetRngaGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for ADC module.
 *
 * This function enables the clock for ADC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableAdcClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for ADC module.
 *
 * This function disables the clock for ADC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableAdcClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for ADC module.
 *
 * This function will get the clock gate state for ADC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetAdcGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for CMP module.
 *
 * This function enables the clock for CMP module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableCmpClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for CMP module.
 *
 * This function disables the clock for CMP module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableCmpClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for CMP module.
 *
 * This function will get the clock gate state for CMP module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetCmpGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for DAC module.
 *
 * This function enables the clock for DAC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableDacClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for DAC module.
 *
 * This function disables the clock for DAC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableDacClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for DAC module.
 *
 * This function will get the clock gate state for DAC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetDacGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for VREF module.
 *
 * This function enables the clock for VREF module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableVrefClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for VREF module.
 *
 * This function disables the clock for VREF module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableVrefClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for VREF module.
 *
 * This function will get the clock gate state for VREF module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetVrefGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for SAI module.
 *
 * This function enables the clock for SAI module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableSaiClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for SAI module.
 *
 * This function disables the clock for SAI module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableSaiClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for SAI module.
 *
 * This function will get the clock gate state for SAI module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetSaiGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for PDB module.
 *
 * This function enables the clock for PDB module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnablePdbClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for PDB module.
 *
 * This function disables the clock for PDB module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisablePdbClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for PDB module.
 *
 * This function will get the clock gate state for PDB module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetPdbGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for FTM module.
 *
 * This function enables the clock for FTM module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableFtmClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for FTM module.
 *
 * This function disables the clock for FTM module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableFtmClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for FTM module.
 *
 * This function will get the clock gate state for FTM module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetFtmGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for PIT module.
 *
 * This function enables the clock for PIT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnablePitClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for PIT module.
 *
 * This function disables the clock for PIT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisablePitClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for PIT module.
 *
 * This function will get the clock gate state for PIT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetPitGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for LPTIMER module.
 *
 * This function enables the clock for LPTIMER module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableLptimerClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for LPTIMER module.
 *
 * This function disables the clock for LPTIMER module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableLptimerClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for LPTIMER module.
 *
 * This function will get the clock gate state for LPTIMER module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetLptimerGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for CMT module.
 *
 * This function enables the clock for CMT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableCmtClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for CMT module.
 *
 * This function disables the clock for CMT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableCmtClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for CMT module.
 *
 * This function will get the clock gate state for CMT module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetCmtGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for RTC module.
 *
 * This function enables the clock for RTC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableRtcClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for RTC module.
 *
 * This function disables the clock for RTC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableRtcClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for RTC module.
 *
 * This function will get the clock gate state for RTC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetRtcGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for ENET module.
 *
 * This function enables the clock for ENET module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableEnetClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for ENET module.
 *
 * This function disables the clock for ENET module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableEnetClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for ENET module.
 *
 * This function will get the clock gate state for ENET module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetEnetGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for USBFS module.
 *
 * This function enables the clock for USBFS module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableUsbClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for USBFS module.
 *
 * This function disables the clock for USBFS module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableUsbClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for USB module.
 *
 * This function will get the clock gate state for USB module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetUsbGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for USBDCD module.
 *
 * This function enables the clock for USBDCD module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableUsbdcdClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for USBDCD module.
 *
 * This function disables the clock for USBDCD module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableUsbdcdClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for USBDCD module.
 *
 * This function will get the clock gate state for USBDCD module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetUsbdcdGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for FLEXCAN module.
 *
 * This function enables the clock for FLEXCAN module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableFlexcanClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for FLEXCAN module.
 *
 * This function disables the clock for FLEXCAN module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableFlexcanClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for FLEXCAN module.
 *
 * This function will get the clock gate state for FLEXCAN module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetFlexcanGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for SPI module.
 *
 * This function enables the clock for SPI module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableSpiClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for SPI module.
 *
 * This function disables the clock for SPI module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableSpiClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for SPI module.
 *
 * This function will get the clock gate state for SPI module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetSpiGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for I2C module.
 *
 * This function enables the clock for I2C module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableI2cClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for I2C module.
 *
 * This function disables the clock for I2C module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableI2cClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for I2C module.
 *
 * This function will get the clock gate state for I2C module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetI2cGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for UART module.
 *
 * This function enables the clock for UART module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableUartClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for UART module.
 *
 * This function disables the clock for UART module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableUartClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for UART module.
 *
 * This function will get the clock gate state for UART module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetUartGateCmd(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Enable the clock for SDHC module.
 *
 * This function enables the clock for SDHC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_EnableSdhcClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Disable the clock for SDHC module.
 *
 * This function disables the clock for SDHC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 */
void SIM_HAL_DisableSdhcClock(uint32_t baseAddr, uint32_t instance);

/*!
 * @brief Get the the clock gate state for SDHC module.
 *
 * This function will get the clock gate state for SDHC module.
 *
 * @param baseAddr Base address for current SIM instance.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool SIM_HAL_GetSdhcGateCmd(uint32_t baseAddr, uint32_t instance);

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/


/*! @}*/

#endif /* __FSL_SIM_HAL_K63F12_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

