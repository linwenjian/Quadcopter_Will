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
#include <stdio.h>
#include <string.h>

// SDK Included Files
#include "board.h"
#include "fsl_uart_driver.h"
#include "fsl_os_abstraction.h"
#include "fsl_interrupt_manager.h"
#include "fsl_gpio_driver.h"
#include "fsl_llwu_hal.h"
#include "fsl_smc_hal.h"
#include "fsl_power_manager.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "fsl_sim_hal.h"
#include "fsl_misc_utilities.h"

// Project Included Files
#include "task_lpm.h"

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * function
 ******************************************************************************/
/*!
 * @brief set alarm command.
 *
 * This function set the alarm which will be
 * trigerred x secs later. The alarm trigger
 * will print a notification on the console.
 */
void cmdAlarm(wakeUpSource_t wus, uint8_t offsetSec)
{
    if((wus & wakeUpSourceRtc) != 0)
    {
#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
        rtcSetAlarm(PM_RTOS_DEMO_RTC_FUNC_INSTANCE, offsetSec);
#endif
    }
    else
    {
        lptmrSetAlarm(offsetSec);
    }
}

/*!
 * @brief get wake up source from given character.
 */
wakeUpSource_t getWakeupSource(uint8_t val)
{
#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
    if(val == 'R' || val == 'r')
    {
        return wakeUpSourceRtc;
    }
#endif
    if(val == 'L' || val == 'l')
    {
        return wakeUpSourceLptmr;
    }
    if(val == 'S' || val == 's')
    {
        return wakeUpSourceSwBtn;
    }
    return wakeUpSourceErr;
}

/* LLW_IRQHandler that would cover the same name's APIs in startup code */
#if (defined FRDM_KL03Z48M)
void MODULE_IRQ_HANDLER(LLWU)(void)
#else
void MODULE_IRQ_HANDLER(LLW)(void)
#endif
{
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE
#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
    if (RTC_DRV_IsAlarmPending(PM_RTOS_DEMO_RTC_FUNC_INSTANCE))
    {
        RTC_DRV_SetAlarmIntCmd(PM_RTOS_DEMO_RTC_FUNC_INSTANCE, false);
    }
#endif
#endif
#if BOARD_SW_HAS_LLWU_PIN
    LLWU_HAL_ClearExternalPinWakeupFlag(LLWU, (llwu_wakeup_pin_t)BOARD_SW_LLWU_EXT_PIN);
#endif
}

/* IRQ handler for switch/button. */
void BOARD_SW_LLWU_IRQ_HANDLER(void)
{
    PORT_HAL_ClearPortIntFlag(BOARD_SW_LLWU_BASE);
}

uint8_t setWakeUpTimeOut(wakeUpSource_t wus)
{
    uint8_t val0;
    uint8_t val1;

    while(1)
    {
        printf("Select the wake up timeout in format DD. Possible decimal value is from range 01 - 60 seconds. Eg. 05 means 5 seconds delay");
        printf("\n\rWaiting for key press..\n\r\n\r");
        val0 = getInput();
        printf("You pressed: '%c", val0);
        if( (val0 >= '0') && (val0 <= '6') )
        {
            val1 = getInput();
            printf("%c'\r\n", val1);
            if( (val1 >= '0') && (val1 <= '9') )
            {
                val0 = (val0-'0')*10 + (val1-'0');
                if( (val0!=0) && (val0<=60) )
                {
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE
                    OSA_EnterCritical(kCriticalDisableInt);
#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
                    if((wus & wakeUpSourceRtc) != 0)
                    {
                        LLWU_HAL_SetInternalModuleCmd(LLWU,PM_RTOS_DEMO_RTC_LLWU_WAKEUP_MODULE,true);
                    }
#endif
                    if((wus & wakeUpSourceLptmr) != 0)
                    {
                        LLWU_HAL_SetInternalModuleCmd(LLWU,PM_RTOS_DEMO_LPTMR_LLWU_WAKEUP_MODULE,true);
                    }
                    OSA_ExitCritical(kCriticalDisableInt);
#endif
                    cmdAlarm(wus, val0);
                    return val0;
                }
            }
        }

        printf("Wrong value!\n\r");
    }
}

void printWakeUpSourceText(wakeUpSource_t wus)
{
    printf("Select the wake up source:\n\r");
    if((wus & wakeUpSourceRtc) != 0)
    {
        printf("Press R for RTC - RTC Timer\n\r");
    }
    if((wus & wakeUpSourceLptmr) != 0)
    {
        printf("Press L for LPTMR - LPTMR Timer\n\r");
    }
    if((wus & wakeUpSourceSwBtn) != 0)
    {
        printf("Press S for switch/button ");
        PRINT_LLWU_SW_NUM;
        printf("\n\r");
    }
}

/* TWR_K60D100M doesn't have switch pins connected to LLWU.
 * It's not possible to wake up by SWx buttons from some modes.
 * returns 0 when RTC is selected as wake up source
 * returns 1 when sw pins are selected as wake up source
 */
wakeUpSource_t selectWakeUpSource(demo_power_modes_t mode)
{
    uint8_t val = 0;
    wakeUpSource_t retval = wakeUpSourceErr;
    wakeUpSource_t wus = (wakeUpSource_t)(wakeUpSourceRtc | wakeUpSourceSwBtn | wakeUpSourceLptmr);
    while(retval == wakeUpSourceErr)
    {
        switch(mode)
        {
            case kDemoWait:
            case kDemoVlpr:
            case kDemoVlpw:

            case kDemoStop:
            case kDemoVlps:
#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
            case kDemoLls:
#endif
            case kDemoVlls1:
#if FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
            case kDemoVlls2:
#endif
            case kDemoVlls3:
#if BOARD_SW_HAS_LLWU_PIN
                break;
#else
                // switch is not connected to LLWU, we can use only RTC wake up
                printf("The board does not support wake up from this mode by switch/button. Timer is used as wake up source:\n\r");
                wus &= ~wakeUpSourceSwBtn;
                break;
#endif
#if FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 & BOARD_SW_HAS_LLWU_PIN
            case kDemoVlls0:
                // in VLLS0 LPO oscillator is disabled.
                printf("The board does not support wake up from this mode by Timer due to disabled LPO.\n\r");
                wus &= ~(wakeUpSourceRtc | wakeUpSourceLptmr);
                break;
#endif
            default:
                break;
        }
        printf("\n\rWaiting for key press..\n\r\n\r");
#if (!defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
        // Clear RTC option.
        wus &= (~wakeUpSourceRtc);
#endif
        if(wus == wakeUpSourceRtc || wus == wakeUpSourceLptmr || wus == wakeUpSourceSwBtn)
        {
            return wus;
        }
        printWakeUpSourceText(wus);
        // Wait for user response
        val = getInput();
        printf("You pressed: '%c'\r\n", val);
        retval = getWakeupSource(val);
    }

    return retval;
}

#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE
void llwuDisableWakeUp(void)
{
#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
    LLWU_HAL_SetInternalModuleCmd(LLWU, PM_RTOS_DEMO_RTC_LLWU_WAKEUP_MODULE, false);
#endif
    LLWU_HAL_SetInternalModuleCmd(LLWU, PM_RTOS_DEMO_LPTMR_LLWU_WAKEUP_MODULE, false);
}
#endif

void gpioDisableWakeUp(void)
{
    // disables interrupt
    PORT_HAL_SetPinIntMode(BOARD_SW_LLWU_BASE, BOARD_SW_LLWU_PIN, kPortIntDisabled);
    INT_SYS_DisableIRQ(BOARD_SW_LLWU_IRQ_NUM);

#if BOARD_SW_HAS_LLWU_PIN
    LLWU_HAL_ClearExternalPinWakeupFlag(LLWU, (llwu_wakeup_pin_t)BOARD_SW_LLWU_EXT_PIN);
    LLWU_HAL_SetExternalInputPinMode(LLWU,kLlwuExternalPinDisabled, (llwu_wakeup_pin_t)BOARD_SW_LLWU_EXT_PIN);
#endif
}

void gpioEnableWakeUp(void)
{
    // enables falling edge interrupt for switch SWx
    PORT_HAL_SetPinIntMode(BOARD_SW_LLWU_BASE, BOARD_SW_LLWU_PIN, kPortIntFallingEdge);
    INT_SYS_EnableIRQ(BOARD_SW_LLWU_IRQ_NUM);

#if BOARD_SW_HAS_LLWU_PIN
    LLWU_HAL_ClearExternalPinWakeupFlag(LLWU, (llwu_wakeup_pin_t)BOARD_SW_LLWU_EXT_PIN);
    LLWU_HAL_SetExternalInputPinMode(LLWU,kLlwuExternalPinFallingEdge, (llwu_wakeup_pin_t)BOARD_SW_LLWU_EXT_PIN);
#endif
}

void setWakeUpSource(wakeUpSource_t wus,char *textMode)
{
    uint8_t timeout;

    if((wus & (wakeUpSourceRtc | wakeUpSourceLptmr)) != 0)
    {
        gpioDisableWakeUp();
        // Wake up on timer's interrupt
        timeout = setWakeUpTimeOut(wus);
        printf("Entering %s, will wake up after %u seconds\n\r",textMode,timeout);
    }
    else if((wus & wakeUpSourceSwBtn) != 0)
    {
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE
        llwuDisableWakeUp();
#endif
        // Wake up on gpio interrupt from button(s)
        gpioEnableWakeUp();
        printf("Entering %s, press the ",textMode);
        PRINT_LLWU_SW_NUM;
        printf(" button to wake up.\n\r");
    }
    else
    {
        printf("Unknown error.\n\r");
    }
}

void displayPowerMode(void)
{
    uint32_t smcMode = SMC_HAL_GetStat(SMC);

    switch (smcMode)
    {
    case kStatRun:
        printf("    SMC mode = kPowerModeRun\n\r");
        break;
    case kStatVlpr:
        printf("    SMC mode = kPowerModeVlpr\n\r");
        break;
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
    case kStatHsrun:
        printf("    SMC mode = kPowerModeHsrun\n\r");
        break;
#endif
    default:
        printf("    SMC mode = Unknown!\n\r");
        break;
    }
}

void updateClockManagerToRunMode(uint8_t cmConfigMode)
{
    // if current config mode is RUN but CM is not, need to re-config it to RUN
    if ((cmConfigMode == CLOCK_CONFIG_INDEX_FOR_RUN) &&
        (CLOCK_SYS_GetCurrentConfiguration() != CLOCK_CONFIG_INDEX_FOR_RUN))
    {
        CLOCK_SYS_UpdateConfiguration(CLOCK_CONFIG_INDEX_FOR_RUN, kClockManagerPolicyForcible);
    }
}

void updateClockManagerToVlprMode(uint8_t cmConfigMode)
{
    // if current config mode and CM are both RUN, need to re-config it to VLPR
    if ((cmConfigMode == CLOCK_CONFIG_INDEX_FOR_RUN) &&
        (CLOCK_SYS_GetCurrentConfiguration() == CLOCK_CONFIG_INDEX_FOR_RUN))
    {
        CLOCK_SYS_UpdateConfiguration(CLOCK_CONFIG_INDEX_FOR_VLPR, kClockManagerPolicyForcible);
    }
}

#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
void updateClockManagerToHsRunMode(uint8_t cmConfigMode)
{
    // if current config mode and CM are both RUN, need to re-config it to HSRUN
    if ((cmConfigMode == CLOCK_CONFIG_INDEX_FOR_RUN) &&
        (CLOCK_SYS_GetCurrentConfiguration() == CLOCK_CONFIG_INDEX_FOR_RUN))
    {
        CLOCK_SYS_UpdateConfiguration(CLOCK_CONFIG_INDEX_FOR_HSRUN, kClockManagerPolicyForcible);
    }
}
#endif

/* Update clock to compatible with RUN mode. */
void updateClockMode(uint8_t cmConfigMode)
{
#if (defined(FSL_FEATURE_MCGLITE_MCGLITE))
    if (g_defaultClockConfigurations[cmConfigMode].mcgliteConfig.mcglite_mode == kMcgliteModeHirc48M)
#else
    if (g_defaultClockConfigurations[cmConfigMode].mcgConfig.mcg_mode == kMcgModePEE)
#endif
    {
        CLOCK_SYS_UpdateConfiguration(CLOCK_CONFIG_INDEX_FOR_RUN, kClockManagerPolicyForcible);
    }
    else
    {
        updateClockManagerToRunMode(cmConfigMode);
    }
}

/******************************************************************************
 * Golobal function
 ******************************************************************************/
/* little delay for uart flush */
void delay(uint32_t delay_time)
{
    uint32_t wait;
    for(wait=0;wait<delay_time;wait++)
    {
        __asm("NOP");
    }
}

void task_lpm(task_param_t param)
{
    demo_power_modes_t testVal;
    uint8_t cmConfigMode = CLOCK_CONFIG_INDEX_FOR_RUN;
    uint8_t mode;
    power_manager_error_code_t ret;
    uint32_t freq = 0;

#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
    rtc_datetime_t date;
    memset(&date, 0, sizeof(rtc_datetime_t));
#endif
    memset(&cmCallbackData, 0, sizeof(lptmrStructure_t));
    cmCallbackData.instance = PM_RTOS_DEMO_LPTMR_FUNC_INSTANCE;
    lptmr_user_config_t *lptmrUserConfig = &(cmCallbackData.lptmrUserConfig);
    lptmr_state_t *lptmrState = &(cmCallbackData.lptmrState);

    CLOCK_SYS_Init(g_defaultClockConfigurations,
                   CLOCK_CONFIG_NUM,
                   &cm_callback_tbl,
                   cm_callback_tbl_size);

    CLOCK_SYS_UpdateConfiguration(cmConfigMode, kClockManagerPolicyForcible);

#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
    // Set a start date time and start RTC
    date.year = 2014U;
    date.month = 4U;
    date.day = 30U;
    date.hour = 14U;
    date.minute = 0U;
    date.second = 0U;
    rtcInit(PM_RTOS_DEMO_RTC_FUNC_INSTANCE, &date);
#endif
    lptmrUserConfig->timerMode = kLptmrTimerModeTimeCounter; // Use LPTMR in Time Counter mode
    lptmrUserConfig->freeRunningEnable = false; // When hit compare value, set counter back to zero
    lptmrUserConfig->prescalerEnable = false; // bypass prescaler
    lptmrUserConfig->prescalerClockSource = kClockLptmrSrcLpoClk; // use 1kHz Low Power Clock
    lptmrUserConfig->isInterruptEnabled = true;
    lptmrInit(lptmrUserConfig, lptmrState);

    // Enables LLWU interrupt
#if (defined FRDM_KL03Z48M)
    INT_SYS_EnableIRQ(LLWU_IRQn);
#else
    INT_SYS_EnableIRQ(LLW_IRQn);
#endif

    POWER_SYS_Init(&powerConfigs,
                   powerConfigsSize,
                   &pm_callback_tbl,
                   pm_callback_tbl_size);

#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
    mode = kDemoRun - kDemoMin - 1;
    ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
    if (ret != kPowerManagerSuccess)
    {
        printf("POWER_SYS_SetMode(%u) returned unexpected status : %u\n\r",mode,ret);
    }
    else
    {
        updateClockManagerToRunMode(cmConfigMode);
    }
#endif
#if (defined FSL_RTOS_BM)
    printf("\n\r####################  Power Manager BM Demo ####################\n\n\r");
#elif (defined FSL_RTOS_FREE_RTOS)
    printf("\n\r####################  Power Manager FreeRTOS Demo ####################\n\n\r");
#elif (defined FSL_RTOS_MQX)
    printf("\n\r####################  Power Manager MQX Demo ####################\n\n\r");
#elif (defined FSL_RTOS_UCOSII)
    printf("\n\r####################  Power Manager Ucos2 Demo ####################\n\n\r");
#elif (defined FSL_RTOS_UCOSIII)
    printf("\n\r####################  Power Manager Ucos3 Demo ####################\n\n\r");
#else
    printf("\n\rUnknown RTOS\n\n\r");
#endif

    while (1)
    {
        mode = 0;
        CLOCK_SYS_GetFreq(kCoreClock, &freq);
        printf("    Core Clock = %luHz \n\r", freq);
        displayPowerMode();
        printf("\n\rSelect the desired operation \n\n\r");
        printf("Press  %c for enter: Wait  - Wait mode\n\r",kDemoWait);
        printf("Press  %c for enter: Stop  - Stop mode\n\r",kDemoStop);
        printf("Press  %c for enter: VLPR  - Very Low Power Run mode\n\r",kDemoVlpr);
        printf("Press  %c for enter: VLPW  - Very Low Power Wait mode\n\r",kDemoVlpw);
        printf("Press  %c for enter: VLPS  - Very Low Power Stop mode\n\r",kDemoVlps);

#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
#if FSL_FEATURE_SMC_HAS_LLS_SUBMODE
        printf("Press  %c for enter: LLS3  - Low Leakage Stop mode\n\r",kDemoLls);
#else
        printf("Press  %c for enter: LLS   - Low Leakage Stop mode\n\r",kDemoLls);
#endif
#endif

#if FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 & BOARD_SW_HAS_LLWU_PIN
        printf("Press  %c for enter: VLLS0 - Very Low Leakage Stop 0 mode\n\r",kDemoVlls0);
#endif

        printf("Press  %c for enter: VLLS1 - Very Low Leakage Stop 1 mode\n\r",kDemoVlls1);

#if FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
        printf("Press  %c for enter: VLLS2 - Very Low Leakage Stop 2 mode\n\r",kDemoVlls2);
#endif

        printf("Press  %c for enter: VLLS3 - Very Low Leakage Stop 3 mode\n\r",kDemoVlls3);

        printf("Press  %c for enter: RUN   - Normal RUN mode\n\r",kDemoRun);
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
        printf("Press  %c for enter: HSRUN   - High Speed RUN mode\n\r",kDemoHsRun);
#endif
        printf("Press  %c to get current chip temperature\n\r",KDemoADC);

        printf("------------------------------------------------------------\n\r");
        printf("\n\rWaiting for key press..\n\r\n\r");

        // Wait for user response
        testVal = (demo_power_modes_t)getInput();
        printf("You pressed: '%c'\r\n", testVal);

        // convert lower to upper character.
        if(testVal > kDemoMax)
        {
            testVal = (demo_power_modes_t)(testVal + 'A' - 'a');
        }

        mode = testVal - kDemoMin - 1;

        switch (testVal)
        {
            case kDemoWait:
                if (POWER_SYS_GetCurrentMode() == kPowerManagerVlpr)
                {
                    printf("Can not go from VLPR to WAIT directly\n\r");
                    break;
                }
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to WAIT directly\n\r");
                    break;
                }
#endif
                setWakeUpSource(selectWakeUpSource(testVal),"Wait mode");

                delay(1000); // wait for uart tx fifo complete
                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
                delay(1000);
                CHECK_RET_VAL(ret, mode);

                // update Clock Mode
                updateClockManagerToRunMode(cmConfigMode);
                break;

            case kDemoStop:
                if (POWER_SYS_GetCurrentMode() == kPowerManagerVlpr)
                {
                    printf("Can not go from VLPR to STOP directly\n\r");
                    break;
                }
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to STOP directly\n\r");
                    break;
                }
#endif
                setWakeUpSource(selectWakeUpSource(testVal),"Stop mode");

                delay(100000); // wait for uart tx fifo complete
                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
                CHECK_RET_VAL(ret, mode);

                // update Clock Mode
                updateClockMode(cmConfigMode);
                break;

            case kDemoVlpr:
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to VLPR directly\n\r");
                    break;
                }
#endif
                if(kPowerManagerVlpr != POWER_SYS_GetCurrentMode())
                {
                    /*
                     If apps default CM config mode is not VLPR, but needs to enter VLPR, and real CM config
                     is not VLPR, then we need to update it to VLPR mode here. Otherwise pass through.
                     */
                    updateClockManagerToVlprMode(cmConfigMode);
                    printf("Entering Very Low Power Run mode\n\r");

                    delay(1000); // wait for uart tx fifo complete
                    ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);

                    CHECK_RET_VAL(ret, mode);
                }
                else
                {
                    printf("Very Low Power Run mode already active\n\r");
                }
                break;

            case kDemoVlpw:
                if (POWER_SYS_GetCurrentMode() == kPowerManagerRun)
                {
                    printf("Can not go from RUN to VLPW directly\n\r");
                    break;
                }

#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to VLPW directly\n\r");
                    break;
                }
#endif

                setWakeUpSource(selectWakeUpSource(testVal),"Very Low Wait mode");

                delay(1000); // wait for uart tx fifo complete

                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);

                if (POWER_SYS_GetCurrentMode() == kPowerManagerRun)
                {
                    // update Clock Mode to Run
                    updateClockManagerToRunMode(cmConfigMode);
                }

                CHECK_RET_VAL(ret, mode);
                break;

            case kDemoVlps:
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to VLPS directly\n\r");
                    break;
                }
#endif
                setWakeUpSource(selectWakeUpSource(testVal),"Very Low Power Stop mode");

                delay(100000); // wait for uart tx fifo complete

                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);

                if (POWER_SYS_GetCurrentMode() == kPowerManagerRun)
                {
                    // update Clock Mode to Run
                    updateClockMode(cmConfigMode);
                }

                CHECK_RET_VAL(ret, mode);
                break;
#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
            case kDemoLls:
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to LLSx directly\n\r");
                    break;
                }
#endif
#if FSL_FEATURE_SMC_HAS_LLS_SUBMODE
                setWakeUpSource(selectWakeUpSource(testVal),"Low Leakage Stop mode 3");
#else
                setWakeUpSource(selectWakeUpSource(testVal),"Low Leakage Stop mode");
#endif

                delay(100000); // wait for uart tx fifo complete

                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);

                // Check the mode LLS was entered
                if(kPowerManagerVlpr != POWER_SYS_GetCurrentMode())
                {
                    updateClockMode(cmConfigMode);
                }

                CHECK_RET_VAL(ret, mode);
                break;
#endif
#if FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 & BOARD_SW_HAS_LLWU_PIN
            case kDemoVlls0:
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to VLLS0 directly\n\r");
                    break;
                }
#endif
                setWakeUpSource(selectWakeUpSource(testVal),"Very Low Leakage Stop 0 mode");
                printf("Wake up goes through Reset sequence.\n\r");

                delay(100000); // wait for uart tx fifo complete

                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
                CHECK_RET_VAL(ret, mode);

                break;
#endif
            case kDemoVlls1:
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to VLLS1 directly\n\r");
                    break;
                }
#endif
                setWakeUpSource(selectWakeUpSource(testVal),"Very Low Leakage Stop 1 mode");
                printf("Wake up goes through Reset sequence.\n\r");

                delay(100000); // wait for uart tx fifo complete

                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
                CHECK_RET_VAL(ret, mode);


                break;

#if FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
            case kDemoVlls2:
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to VLLS2 directly\n\r");
                    break;
                }
#endif
                setWakeUpSource(selectWakeUpSource(testVal),"Very Low Leakage Stop 2 mode");
                printf("Wake up goes through Reset sequence.\n\r");

                delay(100000); // wait for uart tx fifo complete

                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
                CHECK_RET_VAL(ret, mode);

                break;
#endif
            case kDemoVlls3:
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
                if (POWER_SYS_GetCurrentMode() == kPowerManagerHsrun)
                {
                    printf("Can not go from HSRUN to VLLS3 directly\n\r");
                    break;
                }
#endif
                setWakeUpSource(selectWakeUpSource(testVal),"Very Low Leakage Stop 3 mode");
                printf("Wake up goes through Reset sequence.\n\r");

                delay(100000); // wait for uart tx fifo complete

                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
                CHECK_RET_VAL(ret, mode);

                break;

            case kDemoRun:

                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
                if (ret != kPowerManagerSuccess)
                {
                    printf("POWER_SYS_SetMode(%u) returned unexpected status : %u\n\r",mode,ret);
                }
                else
                {
                    updateClockManagerToRunMode(cmConfigMode);
                }
                break;
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
            case kDemoHsRun:
                if (POWER_SYS_GetCurrentMode() == kPowerManagerVlpr)
                {
                    printf("Can not go from HSRUN to VLPR directly\n\r");
                    break;
                }
                ret = POWER_SYS_SetMode(mode, kPowerManagerPolicyAgreement);
                if (ret != kPowerManagerSuccess)
                {
                    printf("POWER_SYS_SetMode(%u) returned unexpected status : %u\n\r",mode,ret);
                }
                else
                {
                    updateClockManagerToHsRunMode(cmConfigMode);
                }
                break;
#endif
            case KDemoADC:
                adc16PrintTemperature();
            break;

            default:
                break;
        }
        printf("\n\rNext loop\n\r\n\r");
    }
}

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
