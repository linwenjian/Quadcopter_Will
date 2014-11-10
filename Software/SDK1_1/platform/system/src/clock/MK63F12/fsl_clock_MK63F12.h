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

#if !defined(__FSL_CLOCK_K63F12_H__)
#define __FSL_CLOCK_K63F12_H__

/*! @addtogroup clock_manager*/
/*! @{*/

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief Gets the clock frequency for DMA module.
 *
 * This function gets the clock frequence for DMA module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetDmaFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for DMAMUX module.
 *
 * This function gets the clock frequence for DMAMUX module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetDmamuxFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for PORT module.
 *
 * This function gets the clock frequence for PORT module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetPortFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for MPU module.
 *
 * This function gets the clock frequence for MPU module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetMpuFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for EWM module.
 *
 * This function gets the clock frequence for EWM module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetEwmFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for FLEXBUS module.
 *
 * This function gets the clock frequence for FLEXBUS module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetFlexbusFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for FTF module. (Flash Memory)
 *
 * This function gets the clock frequence for FTF module. (Flash Memory)
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetFtfFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for CRC module. 
 *
 * This function gets the clock frequence for CRC module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetCrcFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for RNGA module. 
 *
 * This function gets the clock frequence for RNGA module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetRngaFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for ADC module. 
 *
 * This function gets the clock frequence for ADC module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetAdcFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for CMP module. 
 *
 * This function gets the clock frequence for CMP module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetCmpFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for VREF module. 
 *
 * This function gets the clock frequence for VREF module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetVrefFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for PDB module. 
 *
 * This function gets the clock frequence for PDB module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetPdbFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for FTM module. (FlexTimer)
 *
 * This function gets the clock frequence for FTM module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetFtmFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for PIT module. 
 *
 * This function gets the clock frequence for PIT module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetPitFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for CMT module. 
 *
 * This function gets the clock frequence for CMT module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetCmtFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for ENET module RMII clock. 
 *
 * This function gets the clock frequence for ENET module RMII clock.. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetEnetRmiiFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for ENET module TIME clock. 
 *
 * This function gets the clock frequence for ENET module TIME clock.. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetEnetTimeStampFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for USB FS OTG module 
 *
 * This function gets the clock frequence for USB FS OTG module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetUsbFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for USB DCD module 
 *
 * This function gets the clock frequence for USB DCD module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetUsbdcdFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for SPI module 
 *
 * This function gets the clock frequence for SPI module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetSpiFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for I2C module 
 *
 * This function gets the clock frequence for I2C module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetI2cFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for UART module 
 *
 * This function gets the clock frequence for UART module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetUartFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for SDHC module.
 *
 * This function gets the clock frequence for SDHC module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetSdhcFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for I2S module. 
 *
 * This function gets the clock frequence for I2S module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetSaiFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for GPIO module 
 *
 * This function gets the clock frequence for GPIO module. 
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetGpioFreq(uint32_t instance);

/*!
 * @brief Enable the clock for DMA module.
 *
 * This function enables the clock for DMA module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableDmaClock(uint32_t instance)
{
    SIM_HAL_EnableDmaClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for DMA module.
 *
 * This function disables the clock for DMA module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableDmaClock(uint32_t instance)
{
    SIM_HAL_DisableDmaClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for DMA module.
 *
 * This function will get the clock gate state for DMA module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetDmaGateCmd(uint32_t instance)
{
    return SIM_HAL_GetDmaGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for DMAMUX module.
 *
 * This function enables the clock for DMAMUX module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableDmamuxClock(uint32_t instance)
{
    SIM_HAL_EnableDmamuxClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for DMAMUX module.
 *
 * This function disables the clock for DMAMUX module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableDmamuxClock(uint32_t instance)
{
    SIM_HAL_DisableDmamuxClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for DMAMUX module.
 *
 * This function will get the clock gate state for DMAMUX module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetDmamuxGateCmd(uint32_t instance)
{
    return SIM_HAL_GetDmamuxGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for PORT module.
 *
 * This function enables the clock for PORT module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnablePortClock(uint32_t instance)
{
    SIM_HAL_EnablePortClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for PORT module.
 *
 * This function disables the clock for PORT module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisablePortClock(uint32_t instance)
{
    SIM_HAL_DisablePortClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for PORT module.
 *
 * This function will get the clock gate state for PORT module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetPortGateCmd(uint32_t instance)
{
    return SIM_HAL_GetPortGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for MPU module.
 *
 * This function enables the clock for MPU module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableMpuClock(uint32_t instance)
{
    SIM_HAL_EnableMpuClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for MPU module.
 *
 * This function disables the clock for MPU module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableMpuClock(uint32_t instance)
{
    SIM_HAL_DisableMpuClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for MPU module.
 *
 * This function will get the clock gate state for MPU module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetMpuGateCmd(uint32_t instance)
{
    return SIM_HAL_GetMpuGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for EWM module.
 *
 * This function enables the clock for EWM module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableEwmClock(uint32_t instance)
{
    SIM_HAL_EnableEwmClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for EWM module.
 *
 * This function disables the clock for EWM module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableEwmClock(uint32_t instance)
{
    SIM_HAL_DisableEwmClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for EWM module.
 *
 * This function will get the clock gate state for EWM module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetEwmGateCmd(uint32_t instance)
{
    return SIM_HAL_GetEwmGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for FLEXBUS module.
 *
 * This function enables the clock for FLEXBUS module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableFlexbusClock(uint32_t instance)
{
    SIM_HAL_EnableFlexbusClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for FLEXBUS module.
 *
 * This function disables the clock for FLEXBUS module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableFlexbusClock(uint32_t instance)
{
    SIM_HAL_DisableFlexbusClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for FLEXBUS module.
 *
 * This function will get the clock gate state for FLEXBUS module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetFlexbusGateCmd(uint32_t instance)
{
    return SIM_HAL_GetFlexbusGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for FTF module.
 *
 * This function enables the clock for FTF module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableFtfClock(uint32_t instance)
{
    SIM_HAL_EnableFtfClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for FTF module.
 *
 * This function disables the clock for FTF module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableFtfClock(uint32_t instance)
{
    SIM_HAL_DisableFtfClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for FTF module.
 *
 * This function will get the clock gate state for FTF module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetFtfGateCmd(uint32_t instance)
{
    return SIM_HAL_GetFtfGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for CRC module.
 *
 * This function enables the clock for CRC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableCrcClock(uint32_t instance)
{
    SIM_HAL_EnableCrcClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for CRC module.
 *
 * This function disables the clock for CRC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableCrcClock(uint32_t instance)
{
    SIM_HAL_DisableCrcClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for CRC module.
 *
 * This function will get the clock gate state for CRC module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetCrcGateCmd(uint32_t instance)
{
    return SIM_HAL_GetCrcGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for RNGA module.
 *
 * This function enables the clock for RNGA module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableRngaClock(uint32_t instance)
{
    SIM_HAL_EnableRngaClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for RNGA module.
 *
 * This function disables the clock for RNGA module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableRngaClock(uint32_t instance)
{
    SIM_HAL_DisableRngaClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for RNGA module.
 *
 * This function will get the clock gate state for RNGA module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetRngaGateCmd(uint32_t instance)
{
    return SIM_HAL_GetRngaGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for ADC module.
 *
 * This function enables the clock for ADC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableAdcClock(uint32_t instance)
{
    SIM_HAL_EnableAdcClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for ADC module.
 *
 * This function disables the clock for ADC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableAdcClock(uint32_t instance)
{
    SIM_HAL_DisableAdcClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for ADC module.
 *
 * This function will get the clock gate state for ADC module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetAdcGateCmd(uint32_t instance)
{
    return SIM_HAL_GetAdcGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for CMP module.
 *
 * This function enables the clock for CMP module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableCmpClock(uint32_t instance)
{
    SIM_HAL_EnableCmpClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for CMP module.
 *
 * This function disables the clock for CMP module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableCmpClock(uint32_t instance)
{
    SIM_HAL_DisableCmpClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for CMP module.
 *
 * This function will get the clock gate state for CMP module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetCmpGateCmd(uint32_t instance)
{
    return SIM_HAL_GetCmpGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for DAC module.
 *
 * This function enables the clock for DAC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableDacClock(uint32_t instance)
{
    SIM_HAL_EnableDacClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for DAC module.
 *
 * This function disables the clock for DAC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableDacClock(uint32_t instance)
{
    SIM_HAL_DisableDacClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for DAC module.
 *
 * This function will get the clock gate state for DAC module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetDacGateCmd(uint32_t instance)
{
    return SIM_HAL_GetDacGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for VREF module.
 *
 * This function enables the clock for VREF module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableVrefClock(uint32_t instance)
{
    SIM_HAL_EnableVrefClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for VREF module.
 *
 * This function disables the clock for VREF module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableVrefClock(uint32_t instance)
{
    SIM_HAL_DisableVrefClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for VREF module.
 *
 * This function will get the clock gate state for VREF module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetVrefGateCmd(uint32_t instance)
{
    return SIM_HAL_GetVrefGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for SAI module.
 *
 * This function enables the clock for SAI module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableSaiClock(uint32_t instance)
{
    SIM_HAL_EnableSaiClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for SAI module.
 *
 * This function disables the clock for SAI module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableSaiClock(uint32_t instance)
{
    SIM_HAL_DisableSaiClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for SAI module.
 *
 * This function will get the clock gate state for SAI module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetSaiGateCmd(uint32_t instance)
{
    return SIM_HAL_GetSaiGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for PDB module.
 *
 * This function enables the clock for PDB module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnablePdbClock(uint32_t instance)
{
    SIM_HAL_EnablePdbClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for PDB module.
 *
 * This function disables the clock for PDB module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisablePdbClock(uint32_t instance)
{
    SIM_HAL_DisablePdbClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for PDB module.
 *
 * This function will get the clock gate state for PDB module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetPdbGateCmd(uint32_t instance)
{
    return SIM_HAL_GetPdbGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for FTM module.
 *
 * This function enables the clock for FTM module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableFtmClock(uint32_t instance)
{
    SIM_HAL_EnableFtmClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for FTM module.
 *
 * This function disables the clock for FTM module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableFtmClock(uint32_t instance)
{
    SIM_HAL_DisableFtmClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for FTM module.
 *
 * This function will get the clock gate state for FTM module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetFtmGateCmd(uint32_t instance)
{
    return SIM_HAL_GetFtmGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for PIT module.
 *
 * This function enables the clock for PIT module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnablePitClock(uint32_t instance)
{
    SIM_HAL_EnablePitClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for PIT module.
 *
 * This function disables the clock for PIT module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisablePitClock(uint32_t instance)
{
    SIM_HAL_DisablePitClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for PIT module.
 *
 * This function will get the clock gate state for PIT module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetPitGateCmd(uint32_t instance)
{
    return SIM_HAL_GetPitGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for LPTIMER module.
 *
 * This function enables the clock for LPTIMER module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableLptimerClock(uint32_t instance)
{
    SIM_HAL_EnableLptimerClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for LPTIMER module.
 *
 * This function disables the clock for LPTIMER module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableLptimerClock(uint32_t instance)
{
    SIM_HAL_DisableLptimerClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for LPTIMER module.
 *
 * This function will get the clock gate state for LPTIMER module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetLptimerGateCmd(uint32_t instance)
{
    return SIM_HAL_GetLptimerGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for CMT module.
 *
 * This function enables the clock for CMT module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableCmtClock(uint32_t instance)
{
    SIM_HAL_EnableCmtClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for CMT module.
 *
 * This function disables the clock for CMT module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableCmtClock(uint32_t instance)
{
    SIM_HAL_DisableCmtClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for CMT module.
 *
 * This function will get the clock gate state for CMT module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetCmtGateCmd(uint32_t instance)
{
    return SIM_HAL_GetCmtGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for RTC module.
 *
 * This function enables the clock for RTC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableRtcClock(uint32_t instance)
{
    SIM_HAL_EnableRtcClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for RTC module.
 *
 * This function disables the clock for RTC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableRtcClock(uint32_t instance)
{
    SIM_HAL_DisableRtcClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for RTC module.
 *
 * This function will get the clock gate state for RTC module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetRtcGateCmd(uint32_t instance)
{
    return SIM_HAL_GetRtcGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for ENET module.
 *
 * This function enables the clock for ENET module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableEnetClock(uint32_t instance)
{
    SIM_HAL_EnableEnetClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for ENET module.
 *
 * This function disables the clock for ENET module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableEnetClock(uint32_t instance)
{
    SIM_HAL_DisableEnetClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for ENET module.
 *
 * This function will get the clock gate state for ENET module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetEnetGateCmd(uint32_t instance)
{
    return SIM_HAL_GetEnetGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for USBFS module.
 *
 * This function enables the clock for USBFS module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableUsbClock(uint32_t instance)
{
    SIM_HAL_EnableUsbClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for USBFS module.
 *
 * This function disables the clock for USBFS module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableUsbClock(uint32_t instance)
{
    SIM_HAL_DisableUsbClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for USB module.
 *
 * This function will get the clock gate state for USB module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetUsbGateCmd(uint32_t instance)
{
    return SIM_HAL_GetUsbGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for USBDCD module.
 *
 * This function enables the clock for USBDCD module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableUsbdcdClock(uint32_t instance)
{
    SIM_HAL_EnableUsbdcdClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for USBDCD module.
 *
 * This function disables the clock for USBDCD module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableUsbdcdClock(uint32_t instance)
{
    SIM_HAL_DisableUsbdcdClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for USBDCD module.
 *
 * This function will get the clock gate state for USBDCD module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetUsbdcdGateCmd(uint32_t instance)
{
    return SIM_HAL_GetUsbdcdGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for FLEXCAN module.
 *
 * This function enables the clock for FLEXCAN module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableFlexcanClock(uint32_t instance)
{
    SIM_HAL_EnableFlexcanClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for FLEXCAN module.
 *
 * This function disables the clock for FLEXCAN module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableFlexcanClock(uint32_t instance)
{
    SIM_HAL_DisableFlexcanClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for FLEXCAN module.
 *
 * This function will get the clock gate state for FLEXCAN module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetFlexcanGateCmd(uint32_t instance)
{
    return SIM_HAL_GetFlexcanGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for SPI module.
 *
 * This function enables the clock for SPI module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableSpiClock(uint32_t instance)
{
    SIM_HAL_EnableSpiClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for SPI module.
 *
 * This function disables the clock for SPI module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableSpiClock(uint32_t instance)
{
    SIM_HAL_DisableSpiClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for SPI module.
 *
 * This function will get the clock gate state for SPI module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetSpiGateCmd(uint32_t instance)
{
    return SIM_HAL_GetSpiGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for I2C module.
 *
 * This function enables the clock for I2C module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableI2cClock(uint32_t instance)
{
    SIM_HAL_EnableI2cClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for I2C module.
 *
 * This function disables the clock for I2C module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableI2cClock(uint32_t instance)
{
    SIM_HAL_DisableI2cClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for I2C module.
 *
 * This function will get the clock gate state for I2C module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetI2cGateCmd(uint32_t instance)
{
    return SIM_HAL_GetI2cGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for UART module.
 *
 * This function enables the clock for UART module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableUartClock(uint32_t instance)
{
    SIM_HAL_EnableUartClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for UART module.
 *
 * This function disables the clock for UART module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableUartClock(uint32_t instance)
{
    SIM_HAL_DisableUartClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for UART module.
 *
 * This function will get the clock gate state for UART module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetUartGateCmd(uint32_t instance)
{
    return SIM_HAL_GetUartGateCmd(SIM_BASE, instance);
}

/*!
 * @brief Enable the clock for SDHC module.
 *
 * This function enables the clock for SDHC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableSdhcClock(uint32_t instance)
{
    SIM_HAL_EnableSdhcClock(SIM_BASE, instance);
}

/*!
 * @brief Disable the clock for SDHC module.
 *
 * This function disables the clock for SDHC module.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableSdhcClock(uint32_t instance)
{
    SIM_HAL_DisableSdhcClock(SIM_BASE, instance);
}

/*!
 * @brief Get the the clock gate state for SDHC module.
 *
 * This function will get the clock gate state for SDHC module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetSdhcGateCmd(uint32_t instance)
{
    return SIM_HAL_GetSdhcGateCmd(SIM_BASE, instance);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __FSL_CLOCK_K63F12_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

