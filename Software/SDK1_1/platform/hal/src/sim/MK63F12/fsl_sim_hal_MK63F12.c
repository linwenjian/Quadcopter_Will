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

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fsl_device_registers.h"
#include "fsl_sim_hal_K63F12.h"
#include "fsl_sim_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief CLOCK name config table for K63*/
const clock_name_config_t kClockNameConfigTable [] =  {
    {false, kSystemClock,     kClockDividerOutdiv1},
    {false, kSystemClock,     kClockDividerOutdiv1},
    {false, kSystemClock,     kClockDividerOutdiv1},
    {false, kSystemClock,     kClockDividerOutdiv2},
    {false, kSystemClock,     kClockDividerOutdiv3},
    {false, kSystemClock,     kClockDividerOutdiv4}
};

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableDmaClock
 * Description   : Enable the clock for DMA module
 * This function enables the clock for DMA moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableDmaClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC7_DMA(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableDmaClock
 * Description   : Disable the clock for DMA module
 * This function disables the clock for DMA moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableDmaClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC7_DMA(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetDmaGateCmd
 * Description   : Get the the clock gate state for DMA module
 * This function will get the clock gate state for DMA moudle
 *
 *END**************************************************************************/
bool SIM_HAL_GetDmaGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC7_DMA(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableDmamuxClock
 * Description   : Enable the clock for DMAMUX module
 * This function enables the clock for DMAMUX moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableDmamuxClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_DMAMUX(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableDmamuxClock
 * Description   : Disable the clock for DMAMUX module
 * This function disables the clock for DMAMUX moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableDmamuxClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_DMAMUX(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetDmamuxGateCmd
 * Description   : Get the the clock gate state for DMAMUX module
 * This function will get the clock gate state for DMAMUX moudle
 *
 *END**************************************************************************/
bool SIM_HAL_GetDmamuxGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_DMAMUX(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnablePortClock
 * Description   : Enable the clock for PORT module
 * This function enables the clock for PORT moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnablePortClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC5_PORTA(baseAddr, 1);
        break;
    case 1:
        BW_SIM_SCGC5_PORTB(baseAddr, 1);
        break;
    case 2:
        BW_SIM_SCGC5_PORTC(baseAddr, 1);
        break;
    case 3:
        BW_SIM_SCGC5_PORTD(baseAddr, 1);
        break;
    case 4:
        BW_SIM_SCGC5_PORTE(baseAddr, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisablePortClock
 * Description   : Disable the clock for PORT module
 * This function disables the clock for PORT moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisablePortClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC5_PORTA(baseAddr, 0);
        break;
    case 1:
        BW_SIM_SCGC5_PORTB(baseAddr, 0);
        break;
    case 2:
        BW_SIM_SCGC5_PORTC(baseAddr, 0);
        break;
    case 3:
        BW_SIM_SCGC5_PORTD(baseAddr, 0);
        break;
    case 4:
        BW_SIM_SCGC5_PORTE(baseAddr, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetPortGateCmd
 * Description   : Get the the clock gate state for PORT module
 * This function will get the clock gate state for PORT moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetPortGateCmd(uint32_t baseAddr, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  BR_SIM_SCGC5_PORTA(baseAddr);
        break;
    case 1:
        retValue =  BR_SIM_SCGC5_PORTB(baseAddr);
        break;
    case 2:
        retValue =  BR_SIM_SCGC5_PORTC(baseAddr);
        break;
    case 3:
        retValue =  BR_SIM_SCGC5_PORTD(baseAddr);
        break;
    case 4:
        retValue =  BR_SIM_SCGC5_PORTE(baseAddr);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableMpuClock
 * Description   : Enable the clock for MPU module
 * This function enables the clock for MPU moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableMpuClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC7_MPU(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableMpuClock
 * Description   : Disable the clock for MPU module.
 * This function disables the clock for MPU moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableMpuClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC7_MPU(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetMpuGateCmd
 * Description   : Get the the clock gate state for MPU module
 * This function will get the clock gate state for MPU moudl.
 *
 *END**************************************************************************/
bool SIM_HAL_GetMpuGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC7_MPU(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableEwmClock
 * Description   : Enable the clock for EWM module
 * This function enables the clock for EWM moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableEwmClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_EWM(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableEwmClock
 * Description   : Disable the clock for EWM modul
 * This function disables the clock for EWM moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableEwmClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_EWM(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetEwmGateCmd
 * Description   : Get the the clock gate state for EWM module
 * This function will get the clock gate state for EWM moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetEwmGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC4_EWM(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableFlexbusClock
 * Description   : Enable the clock for FLEXBUS module
 * This function enables the clock for FLEXBUS moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableFlexbusClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC7_FLEXBUS(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableFlexbusClock
 * Description   : Disable the clock for FLEXBUS module
 * This function disables the clock for FLEXBUS moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableFlexbusClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC7_FLEXBUS(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFlexbusGateCmd
 * Description   : Get the the clock gate state for FLEXBUS module
 * This function will get the clock gate state for FLEXBUS moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetFlexbusGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC7_FLEXBUS(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableFtfClock
 * Description   : Enable the clock for FTF module
 * This function enables the clock for FTF moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableFtfClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_FTF(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableFtfClock
 * Description   : Disable the clock for FTF module
 * This function disables the clock for FTF moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableFtfClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_FTF(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtfGateCmd
 * Description   : Get the the clock gate state for FTF module
 * This function will get the clock gate state for FTF moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetFtfGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_FTF(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableCrcClock
 * Description   : Enable the clock for CRC module
 * This function enables the clock for CRC moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableCrcClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_CRC(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableCrcClock
 * Description   : Disable the clock for CRC module
 * This function disables the clock for CRC moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableCrcClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_CRC(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetCrcGateCmd
 * Description   : Get the the clock gate state for CRC module
 * This function will get the clock gate state for CRC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetCrcGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_CRC(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableRngaClock
 * Description   : Enable the clock for RNGA module
 * This function enables the clock for RNGA moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableRngaClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_RNGA(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableRngaClock
 * Description   : Disable the clock for RNGA module
 * This function disables the clock for RNGA moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableRngaClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_RNGA(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetRngaGateCmd
 * Description   : Get the the clock gate state for RNGA module
 * This function will get the clock gate state for RNGA moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetRngaGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_RNGA(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableAdcClock
 * Description   : Enable the clock for ADC module
 * This function enables the clock for ADC moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableAdcClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC6_ADC0(baseAddr, 1);
        break;
    case 1:
        BW_SIM_SCGC3_ADC1(baseAddr, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableAdcClock
 * Description   : Disable the clock for ADC module
 * This function disables the clock for ADC moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableAdcClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC6_ADC0(baseAddr, 0);
        break;
    case 1:
        BW_SIM_SCGC3_ADC1(baseAddr, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAdcGateCmd
 * Description   : Get the the clock gate state for ADC module
 * This function will get the clock gate state for ADC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetAdcGateCmd(uint32_t baseAddr, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  BR_SIM_SCGC6_ADC0(baseAddr);
        break;
    case 1:
        retValue =  BR_SIM_SCGC3_ADC1(baseAddr);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableCmpClock
 * Description   : Enable the clock for CMP module
 * This function enables the clock for CMP moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableCmpClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_CMP(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableCmpClock
 * Description   : Disable the clock for CMP module
 * This function disables the clock for CMP moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableCmpClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_CMP(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetCmpGateCmd
 * Description   : Get the the clock gate state for CMP module
 * This function will get the clock gate state for CMP moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetCmpGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC4_CMP(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableDacClock
 * Description   : Enable the clock for DAC module
 * This function enables the clock for DAC moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableDacClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC2_DAC0(baseAddr, 1);
        break;
    case 1:
        BW_SIM_SCGC2_DAC1(baseAddr, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableDacClock
 * Description   : Disable the clock for DAC module
 * This function disables the clock for DAC moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableDacClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC2_DAC0(baseAddr, 0);
        break;
    case 1:
        BW_SIM_SCGC2_DAC1(baseAddr, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetDacGateCmd
 * Description   : Get the the clock gate state for DAC module
 * This function will get the clock gate state for DAC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetDacGateCmd(uint32_t baseAddr, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  BR_SIM_SCGC2_DAC0(baseAddr);
        break;
    case 1:
        retValue =  BR_SIM_SCGC2_DAC1(baseAddr);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableVrefClock
 * Description   : Enable the clock for VREF module
 * This function enables the clock for VREF moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableVrefClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_VREF(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableVrefClock
 * Description   : Disable the clock for VREF module
 * This function disables the clock for VREF moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableVrefClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_VREF(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetVrefGateCmd
 * Description   : Get the the clock gate state for VREF module
 * This function will get the clock gate state for VREF moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetVrefGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC4_VREF(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSaiClock
 * Description   : Enable the clock for SAI module
 * This function enables the clock for SAI moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableSaiClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_I2S(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSaiClock
 * Description   : Disable the clock for SAI module
 * This function disables the clock for SAI moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableSaiClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_I2S(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSaiGateCmd
 * Description   : Get the the clock gate state for SAI module
 * This function will get the clock gate state for SAI moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetSaiGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_I2S(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnablePdbClock
 * Description   : Enable the clock for PDB module
 * This function enables the clock for PDB moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnablePdbClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_PDB(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisablePdbClock
 * Description   : Disable the clock for PDB module
 * This function disables the clock for PDB moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisablePdbClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_PDB(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetPdbGateCmd
 * Description   : Get the the clock gate state for PDB module
 * This function will get the clock gate state for PDB moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetPdbGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_PDB(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableFtmClock
 * Description   : Enable the clock for FTM module
 * This function enables the clock for FTM moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableFtmClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC6_FTM0(baseAddr, 1);
        break;
    case 1:
        BW_SIM_SCGC6_FTM1(baseAddr, 1);
        break;
    case 2:
        BW_SIM_SCGC6_FTM2(baseAddr, 1);
        break;
    case 3:
        BW_SIM_SCGC3_FTM3(baseAddr, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableFtmClock
 * Description   : Disable the clock for FTM module
 * This function disables the clock for FTM moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableFtmClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC6_FTM0(baseAddr, 0);
        break;
    case 1:
        BW_SIM_SCGC6_FTM1(baseAddr, 0);
        break;
    case 2:
        BW_SIM_SCGC6_FTM2(baseAddr, 0);
        break;
    case 3:
        BW_SIM_SCGC3_FTM3(baseAddr, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmGateCmd
 * Description   : Get the the clock gate state for FTM module
 * This function will get the clock gate state for FTM moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetFtmGateCmd(uint32_t baseAddr, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  BR_SIM_SCGC6_FTM0(baseAddr);
        break;
    case 1:
        retValue =  BR_SIM_SCGC6_FTM1(baseAddr);
        break;
    case 2:
        retValue =  BR_SIM_SCGC6_FTM2(baseAddr);
        break;
    case 3:
        retValue =  BR_SIM_SCGC3_FTM3(baseAddr);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnablePitClock
 * Description   : Enable the clock for PIT module
 * This function enables the clock for PIT moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnablePitClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_PIT(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisablePitClock
 * Description   : Disable the clock for PIT module
 * This function disables the clock for PIT moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisablePitClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_PIT(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetPitGateCmd
 * Description   : Get the the clock gate state for PIT module
 * This function will get the clock gate state for PIT moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetPitGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_PIT(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableLptimerClock
 * Description   : Enable the clock for LPTIMER module
 * This function enables the clock for LPTIMER moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableLptimerClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC5_LPTMR(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableLptimerClock
 * Description   : Disable the clock for LPTIMER module
 * This function disables the clock for LPTIMER moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableLptimerClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC5_LPTMR(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetLptimerGateCmd
 * Description   : Get the the clock gate state for LPTIMER module
 * This function will get the clock gate state for LPTIMER moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetLptimerGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC5_LPTMR(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableCmtClock
 * Description   : Enable the clock for CMT module
 * This function enables the clock for CMT moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableCmtClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_CMT(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableCmtClock
 * Description   : Disable the clock for CMT module
 * This function disables the clock for CMT moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableCmtClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_CMT(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetCmtGateCmd
 * Description   : Get the the clock gate state for CMT module
 * This function will get the clock gate state for CMT moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetCmtGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC4_CMT(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableRtcClock
 * Description   : Enable the clock for RTC module
 * This function enables the clock for RTC moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableRtcClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_RTC(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableRtcClock
 * Description   : Disable the clock for RTC module
 * This function disables the clock for RTC moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableRtcClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_RTC(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetRtcGateCmd
 * Description   : Get the the clock gate state for RTC module
 * This function will get the clock gate state for RTC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetRtcGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_RTC(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableEnetClock
 * Description   : Enable the clock for ENET module
 * This function enables the clock for ENET moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableEnetClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC2_ENET(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableEnetClock
 * Description   : Disable the clock for ENET module
 * This function disables the clock for ENET moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableEnetClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC2_ENET(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetEnetGateCmd
 * Description   : Get the the clock gate state for ENET module
 * This function will get the clock gate state for ENET moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetEnetGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC2_ENET(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableUsbClock
 * Description   : Enable the clock for USBFS module
 * This function enables the clock for USBFS moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableUsbClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_USBOTG(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableUsbClock
 * Description   : Disable the clock for USBFS module
 * This function disables the clock for USBFS moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableUsbClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC4_USBOTG(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetUsbGateCmd
 * Description   : Get the the clock gate state for USB module
 * This function will get the clock gate state for USB moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetUsbGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC4_USBOTG(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableUsbdcdClock
 * Description   : Enable the clock for USBDCD module
 * This function enables the clock for USBDCD moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableUsbdcdClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_USBDCD(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableUsbdcdClock
 * Description   : Disable the clock for USBDCD module
 * This function disables the clock for USBDCD moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableUsbdcdClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_USBDCD(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetUsbdcdGateCmd
 * Description   : Get the the clock gate state for USBDCD module
 * This function will get the clock gate state for USBDCD moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetUsbdcdGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_USBDCD(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableFlexcanClock
 * Description   : Enable the clock for FLEXCAN module
 * This function enables the clock for FLEXCAN moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableFlexcanClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_FLEXCAN0(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableFlexcanClock
 * Description   : Disable the clock for FLEXCAN module
 * This function disables the clock for FLEXCAN moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableFlexcanClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC6_FLEXCAN0(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFlexcanGateCmd
 * Description   : Get the the clock gate state for FLEXCAN module
 * This function will get the clock gate state for FLEXCAN moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetFlexcanGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC6_FLEXCAN0(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSpiClock
 * Description   : Enable the clock for SPI module
 * This function enables the clock for SPI moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableSpiClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC6_SPI0(baseAddr, 1);
        break;
    case 1:
        BW_SIM_SCGC6_SPI1(baseAddr, 1);
        break;
    case 2:
        BW_SIM_SCGC3_SPI2(baseAddr, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSpiClock
 * Description   : Disable the clock for SPI module
 * This function disables the clock for SPI moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableSpiClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC6_SPI0(baseAddr, 0);
        break;
    case 1:
        BW_SIM_SCGC6_SPI1(baseAddr, 0);
        break;
    case 2:
        BW_SIM_SCGC3_SPI2(baseAddr, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSpiGateCmd
 * Description   : Get the the clock gate state for SPI module
 * This function will get the clock gate state for SPI moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetSpiGateCmd(uint32_t baseAddr, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  BR_SIM_SCGC6_SPI0(baseAddr);
        break;
    case 1:
        retValue =  BR_SIM_SCGC6_SPI1(baseAddr);
        break;
    case 2:
        retValue =  BR_SIM_SCGC3_SPI2(baseAddr);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableI2cClock
 * Description   : Enable the clock for I2C module
 * This function enables the clock for I2C moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableI2cClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC4_I2C0(baseAddr, 1);
        break;
    case 1:
        BW_SIM_SCGC4_I2C1(baseAddr, 1);
        break;
    case 2:
        BW_SIM_SCGC1_I2C2(baseAddr, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableI2cClock
 * Description   : Disable the clock for I2C module
 * This function disables the clock for I2C moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableI2cClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC4_I2C0(baseAddr, 0);
        break;
    case 1:
        BW_SIM_SCGC4_I2C1(baseAddr, 0);
        break;
    case 2:
        BW_SIM_SCGC1_I2C2(baseAddr, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetI2cGateCmd
 * Description   : Get the the clock gate state for I2C module
 * This function will get the clock gate state for I2C moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetI2cGateCmd(uint32_t baseAddr, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  BR_SIM_SCGC4_I2C0(baseAddr);
        break;
    case 1:
        retValue =  BR_SIM_SCGC4_I2C1(baseAddr);
        break;
    case 2:
        retValue =  BR_SIM_SCGC1_I2C2(baseAddr);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableUartClock
 * Description   : Enable the clock for UART module
 * This function enables the clock for UART moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableUartClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC4_UART0(baseAddr, 1);
        break;
    case 1:
        BW_SIM_SCGC4_UART1(baseAddr, 1);
        break;
    case 2:
        BW_SIM_SCGC4_UART2(baseAddr, 1);
        break;
    case 3:
        BW_SIM_SCGC4_UART3(baseAddr, 1);
        break;
    case 4:
        BW_SIM_SCGC1_UART4(baseAddr, 1);
        break;
    case 5:
        BW_SIM_SCGC1_UART5(baseAddr, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableUartClock
 * Description   : Disable the clock for UART module
 * This function disables the clock for UART moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableUartClock(uint32_t baseAddr, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        BW_SIM_SCGC4_UART0(baseAddr, 0);
        break;
    case 1:
        BW_SIM_SCGC4_UART1(baseAddr, 0);
        break;
    case 2:
        BW_SIM_SCGC4_UART2(baseAddr, 0);
        break;
    case 3:
        BW_SIM_SCGC4_UART3(baseAddr, 0);
        break;
    case 4:
        BW_SIM_SCGC1_UART4(baseAddr, 0);
        break;
    case 5:
        BW_SIM_SCGC1_UART5(baseAddr, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetUartGateCmd
 * Description   : Get the the clock gate state for UART module
 * This function will get the clock gate state for UART moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetUartGateCmd(uint32_t baseAddr, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  BR_SIM_SCGC4_UART0(baseAddr);
        break;
    case 1:
        retValue =  BR_SIM_SCGC4_UART1(baseAddr);
        break;
    case 2:
        retValue =  BR_SIM_SCGC4_UART2(baseAddr);
        break;
    case 3:
        retValue =  BR_SIM_SCGC4_UART3(baseAddr);
        break;
    case 4:
        retValue =  BR_SIM_SCGC1_UART4(baseAddr);
        break;
    case 5:
        retValue =  BR_SIM_SCGC1_UART5(baseAddr);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSdhcClock
 * Description   : Enable the clock for SDHC module
 * This function enables the clock for SDHC moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableSdhcClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC3_SDHC(baseAddr, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSdhcClock
 * Description   : Disable the clock for SDHC module
 * This function disables the clock for SDHC moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableSdhcClock(uint32_t baseAddr, uint32_t instance)
{
    BW_SIM_SCGC3_SDHC(baseAddr, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSdhcGateCmd
 * Description   : Get the the clock gate state for SDHC module
 * This function will get the clock gate state for SDHC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetSdhcGateCmd(uint32_t baseAddr, uint32_t instance)
{
    return BR_SIM_SCGC3_SDHC(baseAddr);
}
/* TODO: The following code are copied from fsl_sim_hal.c directly, they are
 * placed here temporarily, after clock manager revist finished, they will be
 * moved to right place.
 */
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_HAL_SetSource
 * Description   : Set clock source setting 
 * This function will set the settings for specified clock source. Each clock 
 * source has its clock selection settings. Refer to reference manual for 
 * details of settings for each clock source. Refer to clock_source_names_t 
 * for clock sources.
 * 
 *END**************************************************************************/
sim_hal_status_t CLOCK_HAL_SetSource(uint32_t baseAddr,
                                     clock_source_names_t clockSource,
                                     uint8_t setting)
{
    sim_hal_status_t status = kSimHalSuccess;
    assert(clockSource < kClockSourceMax);

    switch (clockSource)
    {
#if FSL_FEATURE_SIM_OPT_HAS_NFCSRC
    case kClockNfcSrc:                   /* NFCSRC*/
        BW_SIM_SOPT2_NFCSRC(baseAddr, setting);
        break;
    case kClockNfcSel:                   /* NFC_CLKSEL*/
        BW_SIM_SOPT2_NFC_CLKSEL(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_ESDHCSRC
    case kClockEsdhcSrc:                 /* ESDHCSRC*/
        BW_SIM_SOPT2_ESDHCSRC(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_SDHCSRC
    case kClockSdhcSrc:                  /* SDHCSRC*/
        BW_SIM_SOPT2_SDHCSRC(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LCDCSRC
    case kClockLcdcSrc:                  /* LCDCSRC*/
        BW_SIM_SOPT2_LCDCSRC(baseAddr, setting);
        break;
    case kClockLcdcSel:                  /* LCDC_CLKSEL*/
        BW_SIM_SOPT2_LCDC_CLKSEL(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TIMESRC
    case kClockTimeSrc:                  /* TIMESRC*/
        BW_SIM_SOPT2_TIMESRC(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_RMIISRC
    case kClockRmiiSrc:                  /* RMIISRC*/
        BW_SIM_SOPT2_RMIISRC(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBSRC
    case kClockUsbSrc:                    /* USBSRC*/
        BW_SIM_SOPT2_USBSRC(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBFSRC
    case kClockUsbfSrc:                   /* USBFSRC*/
        BW_SIM_SOPT2_USBFSRC(baseAddr, setting);
        break;
    case kClockUsbfSel:                  /* USBF_CLKSEL*/
        BW_SIM_SOPT2_USBF_CLKSEL(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBHSRC
    case kClockUsbhSrc:                  /* USBHSRC*/
        BW_SIM_SOPT2_USBHSRC(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_UART0SRC
    case kClockUart0Src:                 /* UART0SRC*/
        BW_SIM_SOPT2_UART0SRC(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TPMSRC
    case kClockTpmSrc:                   /* TPMSRC*/
        BW_SIM_SOPT2_TPMSRC(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LPUARTSRC
    case kClockLpuartSrc:                /* LPUARTSRC*/
        BW_SIM_SOPT2_LPUARTSRC(baseAddr, setting);
        break;
#endif

    case kClockOsc32kSel:                /* OSC32KSEL*/
        BW_SIM_SOPT1_OSC32KSEL(baseAddr, setting);
        break;
        
#if FSL_FEATURE_SIM_OPT_HAS_PLL_FLL_SELECTION
    case kClockPllfllSel:                /* PLLFLLSEL*/
        BW_SIM_SOPT2_PLLFLLSEL(baseAddr, setting);
        break;
#endif
        
#if FSL_FEATURE_SIM_OPT_HAS_TRACE_CLKSEL
    case kClockTraceSel:                 /* TRACE_CLKSEL*/
        BW_SIM_SOPT2_TRACECLKSEL(baseAddr, setting);
        break;
#endif

    case kClockClkoutSel:                /* CLKOUTSEL*/
        BW_SIM_SOPT2_CLKOUTSEL(baseAddr, setting);
        break;

#if FSL_FEATURE_SIM_OPT_HAS_RTC_CLOCK_OUT_SELECTION
    case kClockRtcClkoutSel:                /* RTCCLKOUTSEL*/
        BW_SIM_SOPT2_RTCCLKOUTSEL(baseAddr, setting);
        break;
#endif
        
    default:
        status = kSimHalNoSuchClockSrc;
        break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_HAL_GetSource
 * Description   : Get clock source setting
 * This function will get the settings for specified clock source. Each clock 
 * source has its clock selection settings. Refer to reference manual for 
 * details of settings for each clock source. Refer to clock_source_names_t
 * for clock sources.
 * 
 *END**************************************************************************/
sim_hal_status_t CLOCK_HAL_GetSource(uint32_t baseAddr,
                                     clock_source_names_t clockSource,
                                     uint8_t *setting)
{
    sim_hal_status_t status = kSimHalSuccess;
    assert(clockSource < kClockSourceMax);

    switch (clockSource)
    {
#if FSL_FEATURE_SIM_OPT_HAS_NFCSRC
    case kClockNfcSrc:                   /* NFCSRC*/
        *setting = BR_SIM_SOPT2_NFCSRC(baseAddr);
        break;
    case kClockNfcSel:                   /* NFC_CLKSEL*/
        *setting = BR_SIM_SOPT2_NFC_CLKSEL(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_ESDHCSRC
    case kClockEsdhcSrc:                 /* ESDHCSRC*/
        *setting = BR_SIM_SOPT2_ESDHCSRC(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_SDHCSRC
    case kClockSdhcSrc:                  /* SDHCSRC*/
        *setting = BR_SIM_SOPT2_SDHCSRC(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LCDCSRC
    case kClockLcdcSrc:                  /* LCDCSRC*/
        *setting = BR_SIM_SOPT2_LCDCSRC(baseAddr);
        break;
    case kClockLcdcSel:                  /* LCDC_CLKSEL*/
        *setting = BR_SIM_SOPT2_LCDC_CLKSEL(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TIMESRC
    case kClockTimeSrc:                  /* TIMESRC*/
        *setting = BR_SIM_SOPT2_TIMESRC(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_RMIISRC
    case kClockRmiiSrc:                  /* RMIISRC*/
        *setting = BR_SIM_SOPT2_RMIISRC(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBSRC
    case kClockUsbSrc:                    /* USBSRC*/
        *setting = BR_SIM_SOPT2_USBSRC(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBFSRC
    case kClockUsbfSrc:                   /* USBFSRC*/
        *setting = BR_SIM_SOPT2_USBFSRC(baseAddr);
        break;
    case kClockUsbfSel:                  /* USBF_CLKSEL*/
        *setting = BR_SIM_SOPT2_USBF_CLKSEL(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBHSRC
    case kClockUsbhSrc:                  /* USBHSRC*/
        *setting = BR_SIM_SOPT2_USBHSRC(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_UART0SRC
    case kClockUart0Src:                 /* UART0SRC*/
        *setting = BR_SIM_SOPT2_UART0SRC(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TPMSRC
    case kClockTpmSrc:                   /* TPMSRC*/
        *setting = BR_SIM_SOPT2_TPMSRC(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LPUARTSRC
    case kClockLpuartSrc:                /* LPUARTSRC*/
        *setting = BR_SIM_SOPT2_LPUARTSRC(baseAddr);
        break;
#endif

    case kClockOsc32kSel:                /* OSC32KSEL*/
        *setting = BR_SIM_SOPT1_OSC32KSEL(baseAddr);
        break;

#if FSL_FEATURE_SIM_OPT_HAS_PLL_FLL_SELECTION
    case kClockPllfllSel:                /* PLLFLLSEL*/
        *setting = BR_SIM_SOPT2_PLLFLLSEL(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TRACE_CLKSEL
    case kClockTraceSel:                 /* TRACE_CLKSEL*/
        *setting = BR_SIM_SOPT2_TRACECLKSEL(baseAddr);
        break;
#endif

    case kClockClkoutSel:                /* CLKOUTSEL */
        *setting = BR_SIM_SOPT2_CLKOUTSEL(baseAddr);
        break;

#if FSL_FEATURE_SIM_OPT_HAS_RTC_CLOCK_OUT_SELECTION
    case kClockRtcClkoutSel:                /* RTCCLKOUTSEL */
        *setting = BR_SIM_SOPT2_RTCCLKOUTSEL(baseAddr);
        break;
#endif
        
    default:
        status = kSimHalNoSuchClockSrc;
        break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_HAL_SetDivider
 * Description   : Set clock divider setting
 * This function will set the setting for specified clock divider. Refer to 
 * reference manual for supported clock divider and value range. Refer to 
 * clock_divider_names_t for dividers.
 * 
 *END**************************************************************************/
sim_hal_status_t CLOCK_HAL_SetDivider(uint32_t baseAddr,
                                      clock_divider_names_t clockDivider, 
                                      uint32_t setting)
{
    sim_hal_status_t status = kSimHalSuccess;
    assert(clockDivider < kClockDividerMax);

    switch (clockDivider)
    {
    case kClockDividerOutdiv1:           /* OUTDIV1*/
        BW_SIM_CLKDIV1_OUTDIV1(baseAddr, setting);
        break;

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV2
    case kClockDividerOutdiv2:           /* OUTDIV2*/
        BW_SIM_CLKDIV1_OUTDIV2(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV3
    case kClockDividerOutdiv3:           /* OUTDIV3*/
        BW_SIM_CLKDIV1_OUTDIV3(baseAddr, setting);
        break;
#endif

    case kClockDividerOutdiv4:           /* OUTDIV4*/
        BW_SIM_CLKDIV1_OUTDIV4(baseAddr, setting);
        break;

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBDIV
    case kClockDividerUsbFrac:           /* USBFRAC*/
        BW_SIM_CLKDIV2_USBFRAC(baseAddr, setting);
        break;
    case kClockDividerUsbDiv:            /* USBDIV*/
        BW_SIM_CLKDIV2_USBDIV(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBFSDIV
    case kClockDividerUsbfsFrac:         /* USBFSFRAC*/
        BW_SIM_CLKDIV2_USBFSFRAC(baseAddr, setting);
        break;
    case kClockDividerUsbfsDiv:          /* USBFSDIV*/
        BW_SIM_CLKDIV2_USBFSDIV(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBHSDIV
    case kClockDividerUsbhsFrac:         /* USBHSFRAC*/
        BW_SIM_CLKDIV2_USBHSFRAC(baseAddr, setting);
        break;
    case kClockDividerUsbhsDiv:          /* USBHSDIV*/
        BW_SIM_CLKDIV2_USBHSDIV(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LCDCSRC
    case kClockDividerLcdcFrac:          /* LCDCFRAC*/
        BW_SIM_CLKDIV3_LCDCFRAC(baseAddr, setting);
        break;
    case kClockDividerLcdcDiv:           /* LCDCDIV*/
        BW_SIM_CLKDIV3_LCDCDIV(baseAddr, setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_NFCSRC
    case kClockDividerNfcFrac:           /* NFCFRAC*/
        BW_SIM_CLKDIV4_NFCFRAC(baseAddr, setting);
        break;
    case kClockDividerNfcDiv:            /* NFCDIV*/
        BW_SIM_CLKDIV4_NFCDIV(baseAddr, setting);
        break;
#endif

    case kClockDividerSpecial1:          /* special divider 1   */
        break;

    default:
        status = kSimHalNoSuchDivider;
        break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetOutDiv
 * Description   : Set all clock out dividers setting at the same time
 * This function will set the setting for all clock out dividers. Refer to 
 * reference manual for supported clock divider and value range. Refer to 
 * clock_divider_names_t for dividers.
 * 
 *END**************************************************************************/
void SIM_HAL_SetOutDiv(uint32_t baseAddr, uint32_t outdiv1, uint32_t outdiv2, 
                                 uint32_t outdiv3, uint32_t outdiv4)
{
    uint32_t clkdiv1 = 0;
    
    clkdiv1 |= BF_SIM_CLKDIV1_OUTDIV1(outdiv1);
#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV2
    clkdiv1 |= BF_SIM_CLKDIV1_OUTDIV2(outdiv2);
#endif
#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV3
    clkdiv1 |= BF_SIM_CLKDIV1_OUTDIV3(outdiv3);
#endif
    clkdiv1 |= BF_SIM_CLKDIV1_OUTDIV4(outdiv4);
    
    HW_SIM_CLKDIV1_WR(baseAddr, clkdiv1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_HAL_GetDivider
 * Description   : Get clock divider setting
 * This function will get the setting for specified clock divider. Refer to 
 * reference manual for supported clock divider and value range. Refer to 
 * clock_divider_names_t for dividers.
 * 
 *END**************************************************************************/
sim_hal_status_t CLOCK_HAL_GetDivider(uint32_t baseAddr,
                                      clock_divider_names_t clockDivider,
                                      uint32_t *setting)
{
    sim_hal_status_t status = kSimHalSuccess;
    assert(clockDivider < kClockDividerMax);

    *setting = 0;

    switch (clockDivider)
    {
    case kClockDividerOutdiv1:           /* OUTDIV1*/
        *setting = BR_SIM_CLKDIV1_OUTDIV1(baseAddr);
        break;

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV2
    case kClockDividerOutdiv2:           /* OUTDIV2*/
        *setting = BR_SIM_CLKDIV1_OUTDIV2(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV3
    case kClockDividerOutdiv3:           /* OUTDIV3*/
        *setting = BR_SIM_CLKDIV1_OUTDIV3(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV4
    case kClockDividerOutdiv4:           /* OUTDIV4*/
        *setting = BR_SIM_CLKDIV1_OUTDIV4(baseAddr);
        break;
#endif
        
#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV5
    case kClockDividerOutdiv5:           /* OUTDIV5*/
        *setting = BR_SIM_CLKDIV1_OUTDIV5(baseAddr);
        break;
#endif
        
#if FSL_FEATURE_SIM_DIVIDER_HAS_USBDIV
    case kClockDividerUsbFrac:           /* USBFRAC*/
        *setting = BR_SIM_CLKDIV2_USBFRAC(baseAddr);
        break;
    case kClockDividerUsbDiv:            /* USBDIV*/
        *setting = BR_SIM_CLKDIV2_USBDIV(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBFSDIV
    case kClockDividerUsbfsFrac:         /* USBFSFRAC*/
        *setting = BR_SIM_CLKDIV2_USBFSFRAC(baseAddr);
        break;
    case kClockDividerUsbfsDiv:          /* USBFSDIV*/
        *setting = BR_SIM_CLKDIV2_USBFSDIV(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBHSDIV
    case kClockDividerUsbhsFrac:         /* USBHSFRAC*/
        *setting = BR_SIM_CLKDIV2_USBHSFRAC(baseAddr);
        break;
    case kClockDividerUsbhsDiv:          /* USBHSDIV*/
        *setting = BR_SIM_CLKDIV2_USBHSDIV(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LCDCSRC
    case kClockDividerLcdcFrac:          /* LCDCFRAC*/
        *setting = BR_SIM_CLKDIV3_LCDCFRAC(baseAddr);
        break;
    case kClockDividerLcdcDiv:           /* LCDCDIV*/
        *setting = BR_SIM_CLKDIV3_LCDCDIV(baseAddr);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_NFCSRC
    case kClockDividerNfcFrac:           /* NFCFRAC*/
        *setting = BR_SIM_CLKDIV4_NFCFRAC(baseAddr);
        break;
    case kClockDividerNfcDiv:            /* NFCDIV*/
        *setting = BR_SIM_CLKDIV4_NFCDIV(baseAddr);
        break;
#endif

    case kClockDividerSpecial1:          /* special divider 1    */
        *setting = 1;                   
        break;

    default:
        status = kSimHalNoSuchDivider;
        break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetAdcAlternativeTriggerCmd
 * Description   : Set ADCx alternate trigger enable setting
 * This function will enable/disable alternative conversion triggers for ADCx. 
 * 
 *END**************************************************************************/
void SIM_HAL_SetAdcAlternativeTriggerCmd(uint32_t baseAddr, uint8_t instance, bool enable)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT7_ADC0ALTTRGEN(baseAddr, enable ? 1 : 0);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        BW_SIM_SOPT7_ADC1ALTTRGEN(baseAddr, enable ? 1 : 0);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        BW_SIM_SOPT7_ADC2ALTTRGEN(baseAddr, enable ? 1 : 0);
        break;
    case 3:
        BW_SIM_SOPT7_ADC3ALTTRGEN(baseAddr, enable ? 1 : 0);
        break;
#endif
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAdcAlternativeTriggerCmd
 * Description   : Get ADCx alternate trigger enable settingg
 * This function will get ADCx alternate trigger enable setting. 
 * 
 *END**************************************************************************/
bool SIM_HAL_GetAdcAlternativeTriggerCmd(uint32_t baseAddr, uint8_t instance)
{
    bool retValue = false;

    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        retValue = BR_SIM_SOPT7_ADC0ALTTRGEN(baseAddr);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        retValue = BR_SIM_SOPT7_ADC1ALTTRGEN(baseAddr);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        retValue = BR_SIM_SOPT7_ADC2ALTTRGEN(baseAddr);
        break;
    case 3:
        retValue = BR_SIM_SOPT7_ADC3ALTTRGEN(baseAddr);
        break;
#endif
#endif
    default:
        retValue = false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetAdcPreTriggerMode
 * Description   : Set ADCx pre-trigger select setting
 * This function will select the ADCx pre-trigger source when alternative
 * triggers are enabled through ADCxALTTRGEN
 * 
 *END**************************************************************************/
void SIM_HAL_SetAdcPreTriggerMode(uint32_t baseAddr, uint8_t instance, sim_pretrgsel_t select)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT7_ADC0PRETRGSEL(baseAddr, select);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        BW_SIM_SOPT7_ADC1PRETRGSEL(baseAddr, select);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        BW_SIM_SOPT7_ADC2PRETRGSEL(baseAddr, select);
        break;
    case 3:
        BW_SIM_SOPT7_ADC3PRETRGSEL(baseAddr, select);
        break;
#endif
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAdcPreTriggerMode
 * Description   : Get ADCx pre-trigger select setting
 * This function will get ADCx pre-trigger select setting.
 * 
 *END**************************************************************************/
sim_pretrgsel_t SIM_HAL_GetAdcPreTriggerMode(uint32_t baseAddr, uint8_t instance)
{
    sim_pretrgsel_t retValue = (sim_pretrgsel_t)0;

    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_pretrgsel_t)BR_SIM_SOPT7_ADC0PRETRGSEL(baseAddr);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        retValue = (sim_pretrgsel_t)BR_SIM_SOPT7_ADC1PRETRGSEL(baseAddr);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        retValue = (sim_pretrgsel_t)BR_SIM_SOPT7_ADC2PRETRGSEL(baseAddr);
        break;
    case 3:
        retValue = (sim_pretrgsel_t)BR_SIM_SOPT7_ADC3PRETRGSEL(baseAddr);
        break;
#endif
#endif
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetAdcTriggerMode
 * Description   : Set ADCx trigger select setting
 * This function will select the ADCx trigger source when alternative triggers
 * are enabled through ADCxALTTRGEN
 * 
 *END**************************************************************************/
void SIM_HAL_SetAdcTriggerMode(uint32_t baseAddr, uint8_t instance, sim_trgsel_t select)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT7_ADC0TRGSEL(baseAddr, select);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        BW_SIM_SOPT7_ADC1TRGSEL(baseAddr, select);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        BW_SIM_SOPT7_ADC2TRGSEL(baseAddr, select);
        break;
    case 3:
        BW_SIM_SOPT7_ADC3TRGSEL(baseAddr, select);
        break;
#endif
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAdcTriggerMode
 * Description   : Get ADCx trigger select setting 
 * This function will get ADCx trigger select setting.
 * 
 *END**************************************************************************/
sim_pretrgsel_t SIM_HAL_GetAdcTriggerMode(uint32_t baseAddr, uint8_t instance)
{
    sim_pretrgsel_t retValue =(sim_pretrgsel_t)0;

    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_pretrgsel_t)BR_SIM_SOPT7_ADC0TRGSEL(baseAddr);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        retValue = (sim_pretrgsel_t)BR_SIM_SOPT7_ADC1TRGSEL(baseAddr);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        retValue = (sim_pretrgsel_t)BR_SIM_SOPT7_ADC2TRGSEL(baseAddr);
        break;
    case 3:
        retValue = (sim_pretrgsel_t)BR_SIM_SOPT7_ADC3TRGSEL(baseAddr);
        break;
#endif
#endif
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetUartRxSrcMode
 * Description   : Set UARTx receive data source select setting 
 * This function will select the source for the UART1 receive data.
 * 
 *END**************************************************************************/
void SIM_HAL_SetUartRxSrcMode(uint32_t baseAddr, uint8_t instance, sim_uart_rxsrc_t select)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT5_UART0RXSRC(baseAddr, select);
        break;
    case 1:
        BW_SIM_SOPT5_UART1RXSRC(baseAddr, select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAdcPreTriggerMode
 * Description   : Get UARTx receive data source select setting 
 * This function will get UARTx receive data source select setting.
 * 
 *END**************************************************************************/
sim_uart_rxsrc_t SIM_HAL_GetUartRxSrcMode(uint32_t baseAddr, uint8_t instance)
{
    sim_uart_rxsrc_t retValue = (sim_uart_rxsrc_t)0;

    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_uart_rxsrc_t)BR_SIM_SOPT5_UART0RXSRC(baseAddr);
        break;
    case 1:
        retValue = (sim_uart_rxsrc_t)BR_SIM_SOPT5_UART1RXSRC(baseAddr);
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetUartTxSrcMode
 * Description   : Set UARTx transmit data source select setting 
 * This function will select the source for the UARTx transmit data.
 * 
 *END**************************************************************************/
void SIM_HAL_SetUartTxSrcMode(uint32_t baseAddr, uint8_t instance, sim_uart_txsrc_t select)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT5_UART0TXSRC(baseAddr, select);
        break;
    case 1:
        BW_SIM_SOPT5_UART1TXSRC(baseAddr, select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetUartTxSrcMode
 * Description   : Get UARTx transmit data source select setting 
 * This function will get UARTx transmit data source select setting.
 * 
 *END**************************************************************************/
sim_uart_txsrc_t SIM_HAL_GetUartTxSrcMode(uint32_t baseAddr, uint8_t instance)
{
    sim_uart_txsrc_t retValue =(sim_uart_txsrc_t)0;

    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_uart_txsrc_t)BR_SIM_SOPT5_UART0TXSRC(baseAddr);
        break;
    case 1:
        retValue = (sim_uart_txsrc_t)BR_SIM_SOPT5_UART1TXSRC(baseAddr);
        break;
    default:
        break;
    }

    return retValue;
}

#if FSL_FEATURE_SIM_OPT_HAS_ODE
/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetUartOpenDrainCmd
 * Description   : Set UARTx Open Drain Enable setting 
 * This function will enable/disable the UARTx Open Drain.
 * 
 *END**************************************************************************/
void SIM_HAL_SetUartOpenDrainCmd(uint32_t baseAddr, uint8_t instance, bool enable)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT5_UART0ODE(baseAddr, enable ? 1 : 0);
        break;
    case 1:
        BW_SIM_SOPT5_UART1ODE(baseAddr, enable ? 1 : 0);
        break;
#if FSL_FEATURE_SIM_OPT_HAS_UART2_ODE
    case 2:
        BW_SIM_SOPT5_UART2ODE(baseAddr, enable ? 1 : 0);
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetUartOpenDrainCmd
 * Description   : Get UARTx Open Drain Enable setting 
 * This function will get UARTx Open Drain Enable setting.
 * 
 *END**************************************************************************/
bool SIM_HAL_GetUartOpenDrainCmd(uint32_t baseAddr, uint8_t instance)
{
    bool retValue = false;

    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        retValue = BR_SIM_SOPT5_UART0ODE(baseAddr);
        break;
    case 1:
        retValue = BR_SIM_SOPT5_UART1ODE(baseAddr);
        break;
#if FSL_FEATURE_SIM_OPT_HAS_UART2_ODE
    case 2:
        retValue = BR_SIM_SOPT5_UART2ODE(baseAddr);
        break;
#endif
    default:
        break;
    }

    return retValue;
}
#endif

#if FSL_FEATURE_SIM_OPT_HAS_FTM
/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmTriggerSrcMode
 * Description   : Set FlexTimer x hardware trigger y source select setting 
 * This function will select the source of FTMx hardware trigger y.
 * 
 *END**************************************************************************/
void SIM_HAL_SetFtmTriggerSrcMode(uint32_t baseAddr,
                                  uint8_t  instance,
                                  uint8_t  trigger,
                                  sim_ftm_trg_src_t select)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);
    assert (trigger < FSL_FEATURE_SIM_OPT_FTM_TRIGGER_COUNT);

    switch (instance)
    {
#if FSL_FEATURE_SIM_OPT_HAS_FTM0_TRIGGER
    case 0:
        switch (trigger)
        {
        case 0:
            BW_SIM_SOPT4_FTM0TRG0SRC(baseAddr, select);
            break;
        case 1:
            BW_SIM_SOPT4_FTM0TRG1SRC(baseAddr, select);
            break;
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM3_TRIGGER
    case 3:
        switch (trigger)
        {
        case 0:
            BW_SIM_SOPT4_FTM3TRG0SRC(baseAddr, select);
            break;
        case 1:
            BW_SIM_SOPT4_FTM3TRG1SRC(baseAddr, select);
            break;
        default:
            break;
        }
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmTriggerSrcMode
 * Description   : Get FlexTimer x hardware trigger y source select setting
 * This function will get FlexTimer x hardware trigger y source select setting.
 * 
 *END**************************************************************************/
sim_ftm_trg_src_t SIM_HAL_GetFtmTriggerSrcMode(uint32_t baseAddr, uint8_t instance, uint8_t trigger)
{
    sim_ftm_trg_src_t retValue = (sim_ftm_trg_src_t)0;

    assert (instance < HW_FTM_INSTANCE_COUNT);
    assert (trigger < FSL_FEATURE_SIM_OPT_FTM_TRIGGER_COUNT);

    switch (instance)
    {
#if FSL_FEATURE_SIM_OPT_HAS_FTM0_TRIGGER
    case 0:
        switch (trigger)
        {
        case 0:
            retValue = (sim_ftm_trg_src_t)BR_SIM_SOPT4_FTM0TRG0SRC(baseAddr);
            break;
        case 1:
            retValue = (sim_ftm_trg_src_t)BR_SIM_SOPT4_FTM0TRG1SRC(baseAddr);
            break;
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM3_TRIGGER
    case 3:
        switch (trigger)
        {
        case 0:
            retValue = (sim_ftm_trg_src_t)BR_SIM_SOPT4_FTM3TRG0SRC(baseAddr);
            break;
        case 1:
            retValue = (sim_ftm_trg_src_t)BR_SIM_SOPT4_FTM3TRG1SRC(baseAddr);
            break;
        default:
            break;
        }
        break;
#endif
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmExternalClkPinMode
 * Description   : Set FlexTimer x external clock pin select setting 
 * This function will select the source of FTMx external clock pin select
 * 
 *END**************************************************************************/
void SIM_HAL_SetFtmExternalClkPinMode(uint32_t baseAddr, uint8_t instance, sim_ftm_clk_sel_t select)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT4_FTM0CLKSEL(baseAddr, select);
        break;
    case 1:
        BW_SIM_SOPT4_FTM1CLKSEL(baseAddr, select);
        break;
    case 2:
        BW_SIM_SOPT4_FTM2CLKSEL(baseAddr, select);
        break;
#if (HW_FTM_INSTANCE_COUNT > 3)
    case 3:
        BW_SIM_SOPT4_FTM3CLKSEL(baseAddr, select);
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmExternalClkPinMode
 * Description   : Get FlexTimer x external clock pin select setting
 * This function will get FlexTimer x external clock pin select setting.
 * 
 *END**************************************************************************/
sim_ftm_clk_sel_t SIM_HAL_GetFtmExternalClkPinMode(uint32_t baseAddr, uint8_t instance)
{
    sim_ftm_clk_sel_t retValue = (sim_ftm_clk_sel_t)0;

    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_ftm_clk_sel_t)BR_SIM_SOPT4_FTM0CLKSEL(baseAddr);
        break;
    case 1:
        retValue = (sim_ftm_clk_sel_t)BR_SIM_SOPT4_FTM1CLKSEL(baseAddr);
        break;
    case 2:
        retValue = (sim_ftm_clk_sel_t)BR_SIM_SOPT4_FTM2CLKSEL(baseAddr);
        break;
#if (HW_FTM_INSTANCE_COUNT > 3)
    case 3:
        retValue = (sim_ftm_clk_sel_t)BR_SIM_SOPT4_FTM3CLKSEL(baseAddr);
        break;
#endif
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmChSrcMode
 * Description   : FlexTimer x channel y input capture source select setting 
 * This function will select FlexTimer x channel y input capture source
 * 
 *END**************************************************************************/
void SIM_HAL_SetFtmChSrcMode(uint32_t baseAddr,
                             uint8_t  instance,
                             uint8_t  channel,
                             sim_ftm_ch_src_t select)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
#if FSL_FEATURE_SIM_OPT_HAS_FTM1_CHANNELS
    case 1:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_FTM1CH0SRC(baseAddr, select);
            break;
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM2_CHANNELS
    case 2:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_FTM2CH0SRC(baseAddr, select);
            break;
#if FSL_FEATURE_SIM_OPT_HAS_FTM2_CHANNEL1
        case 1:
            BW_SIM_SOPT4_FTM2CH1SRC(baseAddr, select);
            break;
#endif
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM3_CHANNELS
    case 3:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_FTM3CH0SRC(baseAddr, select);
            break;
        default:
            break;
        }
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmChSrcMode
 * Description   : Get FlexTimer x channel y input capture source select setting
 * This function will get FlexTimer x channel y input capture source select 
 * setting.
 * 
 *END**************************************************************************/
sim_ftm_ch_src_t SIM_HAL_GetFtmChSrcMode(uint32_t baseAddr, uint8_t instance, uint8_t channel)
{
    sim_ftm_ch_src_t retValue = (sim_ftm_ch_src_t)0;

    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
#if FSL_FEATURE_SIM_OPT_HAS_FTM1_CHANNELS
    case 1:
        switch (channel)
        {
        case 0:
            retValue = (sim_ftm_ch_src_t)BR_SIM_SOPT4_FTM1CH0SRC(baseAddr);
            break;
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM2_CHANNELS
    case 2:
        switch (channel)
        {
        case 0:
            retValue = (sim_ftm_ch_src_t)BR_SIM_SOPT4_FTM2CH0SRC(baseAddr);
            break;
#if FSL_FEATURE_SIM_OPT_HAS_FTM2_CHANNEL1
        case 1:
            retValue = (sim_ftm_ch_src_t)BR_SIM_SOPT4_FTM2CH1SRC(baseAddr);
            break;
#endif
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM3_CHANNELS
    case 3:
        switch (channel)
        {
        case 0:
            retValue = (sim_ftm_ch_src_t)BR_SIM_SOPT4_FTM3CH0SRC(baseAddr);
            break;
        default:
            break;
        }
        break;
#endif
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmFaultSelMode
 * Description   : Set FlexTimer x fault y select setting 
 * This function will set the FlexTimer x fault y select setting.
 * 
 *END**************************************************************************/
void SIM_HAL_SetFtmFaultSelMode(uint32_t baseAddr,
                                uint8_t  instance,
                                uint8_t  fault,
                                sim_ftm_flt_sel_t select)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        switch (fault)
        {
        case 0:
            BW_SIM_SOPT4_FTM0FLT0(baseAddr, select);
            break;
        case 1:
            BW_SIM_SOPT4_FTM0FLT1(baseAddr, select);
            break;
#if (FSL_FEATURE_SIM_OPT_FTM0_FAULT_COUNT > 2)
        case 2:
            BW_SIM_SOPT4_FTM0FLT2(baseAddr, select);
            break;
#if (FSL_FEATURE_SIM_OPT_FTM0_FAULT_COUNT > 3)
        case 3:
            BW_SIM_SOPT4_FTM0FLT3(baseAddr, select);
            break;
#endif
#endif
        default:
            break;
        }
        break;
    case 1:
        BW_SIM_SOPT4_FTM1FLT0(baseAddr, select);
        break;
    case 2:
        BW_SIM_SOPT4_FTM2FLT0(baseAddr, select);
        break;
#if (HW_FTM_INSTANCE_COUNT > 3)        
    case 3:
        BW_SIM_SOPT4_FTM3FLT0(baseAddr, select);
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmFaultSelMode
 * Description   : Get FlexTimer x fault y select setting
 * This function will get FlexTimer x fault y select setting.
 * 
 *END**************************************************************************/
sim_ftm_flt_sel_t SIM_HAL_GetFtmFaultSelMode(uint32_t baseAddr, uint8_t instance, uint8_t fault)
{
    sim_ftm_flt_sel_t retValue = (sim_ftm_flt_sel_t)0;

    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        switch (fault)
        {
        case 0:
            retValue = (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM0FLT0(baseAddr);
            break;
        case 1:
            retValue = (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM0FLT1(baseAddr);
            break;
#if (FSL_FEATURE_SIM_OPT_FTM0_FAULT_COUNT > 2)
        case 2:
            retValue = (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM0FLT2(baseAddr);
            break;
#if (FSL_FEATURE_SIM_OPT_FTM0_FAULT_COUNT > 3)
        case 3:
            retValue = (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM0FLT3(baseAddr);
            break;
#endif
#endif
        default:
            break;
        }
        break;
    case 1:
        retValue = (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM1FLT0(baseAddr);
        break;
    case 2:
        retValue = (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM2FLT0(baseAddr);
        break;
#if (HW_FTM_INSTANCE_COUNT > 3)        
    case 3:
        retValue = (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM3FLT0(baseAddr);
        break;
#endif
    default:
        break;
    }

    return retValue;
}
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TPM
/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetTpmExternalClkPinSelMode
 * Description   : Set Timer/PWM x external clock pin select setting 
 * This function will select the source of Timer/PWM x external clock pin select
 * 
 *END**************************************************************************/
void SIM_HAL_SetTpmExternalClkPinSelMode(uint32_t baseAddr,
                                         uint8_t instance,
                                         sim_tpm_clk_sel_t select)
{
    assert (instance < HW_TPM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT4_TPM0CLKSEL(baseAddr, select);
        break;
    case 1:
        BW_SIM_SOPT4_TPM1CLKSEL(baseAddr, select);
        break;
    case 2:
        BW_SIM_SOPT4_TPM2CLKSEL(baseAddr, select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetTpmExternalClkPinSelMode
 * Description   : Get Timer/PWM x external clock pin select setting
 * This function will get Timer/PWM x external clock pin select setting.
 * 
 *END**************************************************************************/
sim_tpm_clk_sel_t SIM_HAL_GetTpmExternalClkPinSelMode(uint32_t baseAddr, uint8_t instance)
{
    sim_tpm_clk_sel_t retValue = (sim_tpm_clk_sel_t)0;

    assert (instance < HW_TPM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_tpm_clk_sel_t)BR_SIM_SOPT4_TPM0CLKSEL(baseAddr);
        break;
    case 1:
        retValue = (sim_tpm_clk_sel_t)BR_SIM_SOPT4_TPM1CLKSEL(baseAddr);
        break;
    case 2:
        retValue = (sim_tpm_clk_sel_t)BR_SIM_SOPT4_TPM2CLKSEL(baseAddr);
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetTpmChSrcMode
 * Description   : Timer/PWM x channel y input capture source select setting 
 * This function will select Timer/PWM x channel y input capture source
 * 
 *END**************************************************************************/
void SIM_HAL_SetTpmChSrcMode(uint32_t baseAddr,
                             uint8_t instance,
                             uint8_t channel,
                             sim_tpm_ch_src_t select)
{
    assert (instance < HW_TPM_INSTANCE_COUNT);

    switch (instance)
    {
    case 1:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_TPM1CH0SRC(baseAddr, select);
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_TPM2CH0SRC(baseAddr, select);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetTpmChSrcMode
 * Description   : Get Timer/PWM x channel y input capture source select setting
 * This function will get Timer/PWM x channel y input capture source select 
 * setting.
 * 
 *END**************************************************************************/
sim_tpm_ch_src_t SIM_HAL_GetTpmChSrcMode(uint32_t baseAddr,
                                         uint8_t instance,
                                         uint8_t channel)
{
    sim_tpm_ch_src_t retValue = (sim_tpm_ch_src_t)0;

    assert (instance < HW_TPM_INSTANCE_COUNT);

    switch (instance)
    {
    case 1:
        switch (channel)
        {
        case 0:
            retValue = (sim_tpm_ch_src_t)BR_SIM_SOPT4_TPM1CH0SRC(baseAddr);
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (channel)
        {
        case 0:
            retValue = (sim_tpm_ch_src_t)BR_SIM_SOPT4_TPM2CH0SRC(baseAddr);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return retValue;
}
#endif
// TODO: end
/*******************************************************************************
 * EOF
 ******************************************************************************/

