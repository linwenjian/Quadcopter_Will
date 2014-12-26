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

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Standard C Included Files
#include <string.h>

// SDK Included Files

// Project Included Files
#include "adc16_temperature.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Structures & enumerations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
static uint32_t sAdcValue = 0;               /*! ADC value */
static uint32_t sAdcrTemp25 = 0;             /*! Calibrated ADCR_TEMP25 */
static uint32_t sAdcr100m = 0;
volatile static uint8_t sPdbInitialized = 0;

SIM_Type * gSimBase[] = SIM_BASE_PTRS;
volatile uint8_t gConversionCompleteFlag;

/* Structure of initialize PIT channel 0 */
static pit_user_config_t pitUserConf;
///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////
/*!
 * @brief calibrate param for adc.
 */
void adc1IrqHandler(void)
{
    /* Get current ADC value */
    sAdcValue = ADC_GetConvValueRAWInt (HWADC_INSTANCE, CHANNEL_0);
    /* Set gConversionCompleteFlag */
    gConversionCompleteFlag = 1;
}

/*!
 * @brief calibrate param for adc.
 */
void adc16CalibrateParams(void)
{
#if FSL_FEATURE_ADC16_HAS_CALIBRATION
    adc16_calibration_param_t adcCalibraitionParam;
#endif
    adc16_user_config_t adcUserConfig;
    adc16_chn_config_t adcChnConfig;
    uint32_t bandgapValue = 0;  /*! ADC value of BANDGAP */
    uint32_t vdd = 0;           /*! VDD in mV */

#if FSL_FEATURE_ADC16_HAS_CALIBRATION
    // Auto calibration
    ADC16_DRV_GetAutoCalibrationParam(HWADC_INSTANCE, &adcCalibraitionParam);
    ADC16_DRV_SetCalibrationParam(HWADC_INSTANCE, &adcCalibraitionParam);
#endif

    // Enable BANDGAP reference voltage
    PMC_HAL_SetBandgapBufferCmd(PMC, true);

    // Initialization ADC for
    // 16bit resolution, interrupt mode, hw trigger disabled.
    // normal convert speed, VREFH/L as reference,
    // disable continuous convert mode.
    ADC16_DRV_StructInitUserConfigDefault(&adcUserConfig);
    adcUserConfig.resolutionMode = kAdcResolutionBitOf16;
    adcUserConfig.continuousConvEnable = false;
    adcUserConfig.clkSrcMode = kAdcClkSrcOfAsynClk;
    ADC16_DRV_Init(HWADC_INSTANCE, &adcUserConfig);

#if FSL_FEATURE_ADC16_HAS_HW_AVERAGE
    ADC16_DRV_EnableHwAverage(HWADC_INSTANCE, kAdcHwAverageCountOf32);
#endif // FSL_FEATURE_ADC16_HAS_HW_AVERAGE

    adcChnConfig.chnNum = kAdcChannelBandgap;
    adcChnConfig.diffEnable = false;
    adcChnConfig.intEnable = false;
    adcChnConfig.chnMux = kAdcChnMuxOfA;
    ADC16_DRV_ConfigConvChn(HWADC_INSTANCE, CHANNEL_0, &adcChnConfig);

    // Wait for the conversion to be done
    ADC16_DRV_WaitConvDone(HWADC_INSTANCE, CHANNEL_0);

    // Get current ADC BANDGAP value
    bandgapValue = ADC16_DRV_GetConvValueRAW(HWADC_INSTANCE, CHANNEL_0);
    bandgapValue = ADC16_DRV_ConvRAWData(bandgapValue, false, adcUserConfig.resolutionMode);

    // ADC stop conversion
    ADC16_DRV_PauseConv(HWADC_INSTANCE, CHANNEL_0);

    // Get VDD value measured in mV: VDD = (ADCR_VDD x V_BG) / ADCR_BG
    vdd = ADCR_VDD * V_BG / bandgapValue;
    // Calibrate ADCR_TEMP25: ADCR_TEMP25 = ADCR_VDD x V_TEMP25 / VDD
    sAdcrTemp25 = ADCR_VDD * V_TEMP25 / vdd;
    // ADCR_100M = ADCR_VDD x M x 100 / VDD
    sAdcr100m = (ADCR_VDD * M) / (vdd * 10);

#if FSL_FEATURE_ADC16_HAS_HW_AVERAGE
    ADC16_DRV_DisableHwAverage(HWADC_INSTANCE);
#endif // FSL_FEATURE_ADC16_HAS_HW_AVERAGE

    // Disable BANDGAP reference voltage
    PMC_HAL_SetBandgapBufferCmd(PMC, false);
}

/*!
 * @brief Getting current temperature value.
 * @return Current temperature.
 */
int32_t adc16GetCurrentTempValue(void)
{
    int32_t currentTemperature = 0;

    currentTemperature = (int32_t)(STANDARD_TEMP - ((int32_t)sAdcValue - (int32_t)sAdcrTemp25) * 100 / (int32_t)sAdcr100m);
    return currentTemperature;
}

/*!
 * @brief Initialize the ADCx for HW trigger.
 * @return Error code.
 */
int adc16Init(adc16_user_config_t *adcUserConfig, adc16_chn_config_t *adcChnConfig, adc16_calibration_param_t *adcCalibraitionParam)
{
    if(!adcUserConfig || !adcChnConfig)
    {
        return LPM_DEMO_RETURN_NULL;
    }

    memset(adcUserConfig, 0, sizeof(adc16_user_config_t));
    memset(adcChnConfig, 0, sizeof(adc16_chn_config_t));
    memset(adcCalibraitionParam, 0, sizeof(adc16_calibration_param_t));

    adc16CalibrateParams();

    // Auto calibration
    if(adcCalibraitionParam)
    {
        ADC16_DRV_GetAutoCalibrationParam(HWADC_INSTANCE, adcCalibraitionParam);
        ADC16_DRV_SetCalibrationParam(HWADC_INSTANCE, adcCalibraitionParam);
    }

    // Initialization ADC for
    // 16bit resolution, interrupt mode, hw trigger enabled.
    // normal convert speed, VREFH/L as reference,
    // disable continuous convert mode.
    ADC16_DRV_StructInitUserConfigDefault(adcUserConfig);
    adcUserConfig->intEnable = true;
    adcUserConfig->resolutionMode = kAdcResolutionBitOf16;
    adcUserConfig->hwTriggerEnable = true;
    adcUserConfig->continuousConvEnable = false;
    adcUserConfig->clkSrcMode = kAdcClkSrcOfAsynClk;
    ADC16_DRV_Init(HWADC_INSTANCE, adcUserConfig);

    // Install Callback function into ISR
    ADC_InstallCallback(HWADC_INSTANCE, CHANNEL_0, adc1IrqHandler);

    adcChnConfig->chnNum = kAdcChannelTemperature;
    adcChnConfig->diffEnable = false;
    adcChnConfig->intEnable = true;
    adcChnConfig->chnMux = kAdcChnMuxOfA;

    // Configure channel0
    ADC16_DRV_ConfigConvChn(HWADC_INSTANCE, CHANNEL_0, adcChnConfig);

    adc16InitPitTriggerSource(HWADC_INSTANCE);

    gConversionCompleteFlag = 0;

    return LPM_DEMO_RETURN_OK;
}

/*!
 * @brief Print temperature got from adc.
 */
void adc16PrintTemperature(void)
{
    if(0 == gConversionCompleteFlag)
    {
        printf("\n\rWarning: Conversion has not completed");
    }
    else
    {
        printf("\n\rTemperature: %d C ", (int)adc16GetCurrentTempValue());
        gConversionCompleteFlag = 0;
    }
}

/*!
 * @Brief enable the trigger source
 *
 * @param instance The ADC instance number
 */
void adc16InitPitTriggerSource(uint32_t adcInstance)
{
    /* Change the timer period here in unit of microseconds.*/
    uint32_t timerPeriod = 500;
    uint32_t pitInstance = PIT_IDX;
    uint32_t pitChannel = 0;

    if(sPdbInitialized)
    {
        return;
    }
    sPdbInitialized = 1;

    pitUserConf.isInterruptEnabled = false;
    pitUserConf.isTimerChained = false;
    pitUserConf.periodUs = timerPeriod;

    /* Init pit module and enable run in debug */
    PIT_DRV_Init(pitInstance, false);

    /* Initialize PIT timer 0 */
    PIT_DRV_InitChannel(pitInstance, pitChannel, &pitUserConf);

    /* Set timer0 period and start it.*/
    PIT_DRV_SetTimerPeriodByUs(pitInstance, pitChannel, timerPeriod * 1000);

    // Configure SIM for ADC hw trigger source PIT
    SIM_HAL_SetAdcAlternativeTriggerCmd(gSimBase[0], adcInstance, true);

    SIM_HAL_SetAdcTriggerMode(gSimBase[0], adcInstance, kSimAdcTrgSelPit0);

    PIT_DRV_StartTimer(pitInstance, pitChannel);
}

/*!
 * @Brief disable the trigger source
 */
void adc16DeinitPitTriggerSource(uint32_t adcInstance)
{
    if(sPdbInitialized == 1)
    {
        PIT_DRV_Deinit(PIT_IDX);
        sPdbInitialized = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
