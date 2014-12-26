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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "device/fsl_device_registers.h"
#include "fsl_uart_driver.h"
#include "fsl_uart_hal.h"
#include "fsl_smc_hal.h"
#include "fsl_gpio_driver.h"
#include "fsl_interrupt_manager.h"
#include "fsl_clock_manager.h"
#include "fsl_port_hal.h"
#include "fsl_sim_hal.h"
#include "fsl_mcg_hal.h"
#include "fsl_osc_hal.h"
#include "fsl_lptmr_hal.h"
#include "fsl_lptmr_driver.h"
#include "fsl_power_manager.h"
#include "fsl_hwtimer.h"
#include "fsl_rcm_hal.h"

#include "board.h"
#include "low_power_demo.h"
#include "fsl_pmc_hal.h"
#include "clock_mode_functions.h"

/* Structure for callback data */
typedef struct {
    uint32_t counter;
    uint32_t status;
    uint32_t err;
} callback_data_t;

/* Callback structure definition to be used by the Power Manager variables */
typedef struct {
    callback_data_t none;
    callback_data_t before;
    callback_data_t after;
    power_manager_callback_type_t lastType;
    uint32_t err;
} user_callback_data_t;

/******************************************************************************
 * Definitions
 *****************************************************************************/
#if !(defined(TWR_KV10Z75M)) 
#define HWTIMER_LL_DEVIF        kPitDevif
#define HWTIMER_LL_SRCCLK       kBusClock
#define HWTIMER_LL_ID           3
#define HWTIMER_PERIOD          1000000
#else
#define HWTIMER_LL_DEVIF        kSystickDevif
#define HWTIMER_LL_SRCCLK       kCoreClock
#define HWTIMER_LL_ID           0
#define HWTIMER_PERIOD          100000
#endif
#define HWTIMER_ISR_PRI         5

/*******************************************************************************
* Prototypes
******************************************************************************/

extern void cpu_identify(void);
extern void de_init_pins(void);
void hwtimer_callback(void* data);
void myTimeDelay(void);
/*******************************************************************************
* Variables
******************************************************************************/
uint8_t uartEnabled = 1; /* Variable used to indicate whether or not the UART is active */

extern const hwtimer_devif_t kPitDevif; /* HWTIMER Timer Init variable */
extern const hwtimer_devif_t kSystickDevif;
hwtimer_t hwtimer; /* HWTIMER Initialization structure */
volatile bool gTimeElapsed = false; /* Variable for HWTIMER usage */

int32_t mcgClkHz; /*!< MCG out clock frequecy in Hz */
uint32_t uartClkHz; /*!<Other uart clock frequcy in Hz */
uint8_t debug = 'n'; /* Variable used to signal whether or not debug pins should be de-activated before low power entry */

/*******************************************************************************
* Code
******************************************************************************/

/*!
* @brief main function.
*/
int main(void)
{   
    /* initialize the hardware */
    hardware_init();
    
    /* Initialize the UART module */
    dbg_uart_init();
    
    /* Hwtimer initialization */
    if (kHwtimerSuccess != HWTIMER_SYS_Init(&hwtimer, &HWTIMER_LL_DEVIF, \
        HWTIMER_LL_ID, HWTIMER_ISR_PRI, NULL))
    {
        printf("\r\nError: hwtimer initialization.\r\n");
    }
    
    /* Need to setup a callback for the Hardware timer to use. */
    if (kHwtimerSuccess != HWTIMER_SYS_RegisterCallback(&hwtimer, hwtimer_callback, NULL))
    {
        printf("\r\nError: hwtimer callback registration.\r\n");
    }
    
    /* Set the hardware timer period */
    if (kHwtimerSuccess != HWTIMER_SYS_SetPeriod(&hwtimer, HWTIMER_LL_SRCCLK, HWTIMER_PERIOD))
    {
        printf("\r\nError: hwtimer set period.\r\n");
    }

    /* check and print the reason of reset */
    out_srs();
    
    /* identify the cpu */
    cpu_identify();
    
    /* initialize the low-leakage wakeup unit */
    llwu_init();
    
    /* configure tower hardware port pins for the application */
    port_init();
    
    /* configure the LED pins in the hardware */
    led_init();
    
    /* start test */
    low_power_modes_test();
}

/*!
* @brief configure tower/freedom hardware port pins for the application
*/
void port_init(void){
    volatile uint32_t buttonRead = 1;
    uint32_t i;
    
    /* De-initialize all unused pins */
    de_init_pins();
    
    /* Configure both pushbuttons for falling edge interrupts */
    switchPins[0].config.interrupt = kPortIntFallingEdge;
    switchPins[1].config.interrupt = kPortIntFallingEdge;
    
    /* Initialize the pins used for switches and LEDs only */
    GPIO_DRV_Init(switchPins, ledPins);
    
    /* Configure the interrupt vector for the interrupt handler of the 
     * Pushbutton IRQs */
    INT_SYS_EnableIRQ(BOARD_SW_IRQ_NUM);
    
    /* Give a little delay here */
    for(i=0; i<1000; i++)
    {}
    
    /* read interrupt switch level */
    buttonRead = GPIO_DRV_ReadPinInput(kGpioSW1);
    
    /* Check to see if the button is being pressed.  If the button is not
     * being pressed, then the pin will be a logic high and buttonRead will be
     * a 1 and the user does not want to leave the debug pins active. 
     * If the button is being pressed, then the pin will be low and buttonRead
     * will be 0 and the user does want to deactivate the debug pins when 
     * entering low power modes. 
     */
    if (buttonRead)
    {
        debug = 'n';
    }
    else
    {
        debug = 'y';
    }
    if((debug == 'n') || (debug == 'N'))
    {   
        printf("\n\r*--------------D E B U G    D I S A B L E D------------------*");
        printf("\n\r*-------Press ");
        PRINT_INT_SW_NUM;
        printf(" then press Reset to re-enable debug---------*");
    } else
    {
        printf("\n\r*--------------D E B U G    E N A B L E D--------------------*");
    }
}

/*!
* @brief configure LED pins for the tower/freedom hardware
*/
void led_init(void)
{
    /* Turn on the LEDs to signal the start of the program */
    LED1_ON;
    LED2_ON;
    LED3_ON;
#if defined(TOWER)
    LED4_ON;
#endif
}


/*!
* @brief main low power mode test routine
*/
void low_power_modes_test(void)
{
    int32_t i;
    uint8_t mcgMode, powerMode;
    int8_t testNum = UNDEF_VALUE;
    uint8_t testVal;
    uint32_t settings;
    
    /* LPTMR configuration structures to use with timed low power wakeup demos */
    lptmr_user_config_t lptmrConfig = {
        .timerMode = kLptmrTimerModeTimeCounter,
        .freeRunningEnable = false,
        .prescalerEnable = false,
        .prescalerClockSource = kClockLptmrSrcLpoClk,
        .isInterruptEnabled = true
    };
    
    lptmr_state_t lptmrState;
    
    /* Power manager error code return variable */
    power_manager_error_code_t ret;

    /* Power manager configuration variables that configure 
     * the named low power mode
     */
    power_manager_user_config_t vlprConfig;
    power_manager_user_config_t vlpwConfig;
    power_manager_user_config_t vlls0Config;
    power_manager_user_config_t vlls1Config;
    power_manager_user_config_t vlls2Config;
    power_manager_user_config_t vlls3Config;
    power_manager_user_config_t llsConfig;
    power_manager_user_config_t vlpsConfig;
    power_manager_user_config_t waitConfig;
    power_manager_user_config_t stopConfig;
    power_manager_user_config_t runConfig; 
    power_manager_user_config_t pstop1Config; 
    power_manager_user_config_t pstop2Config;
    
    /* This is the power manager configuration variable.  The order of this
     * list determines the index that should be passed to the Setmode 
     * function to enter the desired power mode.  
     */
    power_manager_user_config_t const *powerConfigs[] = { &waitConfig, &stopConfig,
            &vlprConfig, &vlpwConfig, &vlpsConfig, &llsConfig, &vlls3Config,
            &vlls2Config, &vlls1Config, &vlls0Config, &runConfig, &pstop1Config, &pstop2Config };

    /* This is the declaration of the callback function that can be used
     * in power mode transitions 
     */
    user_callback_data_t callbackData0;

    power_manager_static_callback_user_config_t callbackCfg0 = { callback0,
            kPowerManagerCallbackBeforeAfter,
            (power_manager_callback_data_t*) &callbackData0 };

    power_manager_static_callback_user_config_t const * callbacks[] =
            { &callbackCfg0 };
    
    /* Allocate memory for the callback */
    memset(&callbackData0, 0, sizeof(user_callback_data_t));

    /* Define the power mode configurations */
    /* Define all of the parameters for the VLPR configuration */
    vlprConfig.mode = kPowerManagerVlpr;
    vlprConfig.policy = kPowerManagerPolicyAgreement;
#if FSL_FEATURE_SMC_HAS_LPWUI
    vlprConfig.lowPowerWakeUpOnInterruptOption = true;
    vlprConfig.lowPowerWakeUpOnInterruptValue = kSmcLpwuiEnabled;
#endif
    vlprConfig.sleepOnExitValue = false;
    vlprConfig.sleepOnExitOption = false;

    /* Copy the VLPR config but set the mode to VLPW */
    vlpwConfig = vlprConfig;
    vlpwConfig.mode = kPowerManagerVlpw;

    /* Copy the VLPR config but set the mode to VLLS0 */
    vlls0Config = vlprConfig;
    vlls0Config.mode = kPowerManagerVlls0;

    /* Copy the VLPR config but set the mode to VLLS1 */
    vlls1Config = vlprConfig;
    vlls1Config.mode = kPowerManagerVlls1;

    /* Copy the VLPR config but set the mode to VLLS2 */
    vlls2Config = vlprConfig;
    vlls2Config.mode = kPowerManagerVlls2;

    /* Copy the VLPR config but set the mode to VLLS3 */
    vlls3Config = vlprConfig;
    vlls3Config.mode = kPowerManagerVlls3;

    /* Copy the VLPR config but set the mode to LLS (LLS3 for devices with 
     * LLS sub-mode functionality.
     */
    llsConfig = vlprConfig;
#if FSL_FEATURE_SMC_HAS_LLS_SUBMODE
    llsConfig.mode = kPowerManagerLls3;
#else
    llsConfig.mode = kPowerManagerLls;
#endif

    /* Copy the VLPR config but set the mode to VLPS */
    vlpsConfig = vlprConfig;
    vlpsConfig.mode = kPowerManagerVlps;

    /* Copy the VLPR config but set the mode to WAIT */
    waitConfig = vlprConfig;
    waitConfig.mode = kPowerManagerWait;

    /* Copy the VLPR config but set the mode to STOP */
    stopConfig = vlprConfig;
    stopConfig.mode = kPowerManagerStop;
    
    /* Copy the VLPR config but set the mode to RUN */
    runConfig = vlprConfig;
    runConfig.mode = kPowerManagerRun;

#if FSL_FEATURE_SMC_HAS_PSTOPO	
    /* Copy the VLPR config but set the mode to Partial stop modes1 */
    pstop1Config = vlprConfig;
    pstop1Config.mode = kPowerManagerPstop1;

    /* Copy the VLPR config but set the mode to Partial stop modes2 */
    pstop2Config = vlprConfig;
    pstop2Config.mode = kPowerManagerPstop2;
#endif

    /* Initialize the power manager module */
    POWER_SYS_Init(&powerConfigs, numPowerModeConfigs, &callbacks, numCallBacksUsed);
    
    /* Print banner */
    printf("\n\r*------------------------------------------------------------*");
    printf("\n\r*                    Kinetis Low Power DEMO                  *");
    printf("\n\r*                    %s %s                    *", __DATE__, __TIME__);
    printf("\n\r*------------------------------------------------------------*\n\r");
    
    /* Enter the main loop which should never be exited (except for VLLS 
     * mode recoveries
     */
    while(1)
    {
        while ((testNum > 22) | (testNum < 0)){
            LED1_ON;
            LED2_ON;
            LED3_ON;
#if defined(TOWER)
            LED4_ON;
#endif
            /* Check the power mode to see if the device is in RUN or VLPR */
            powerMode = POWER_SYS_GetRunningMode();
            if (powerMode == kPowerManagerRun) 
            {
                printf("  in Run Mode !  ");
            }
            else if (powerMode == kPowerManagerVlpr)
            {
                printf("  in VLPR Mode !  ");
            }
            else
            {
                printf("\n\n\rUnknown power mode: \n\r");
                printf("powerMode = %d\n\n\r",powerMode);
            }
    
            /* Check the clock mode */
            mcgMode = what_mcg_mode();
            
            /* Set the mcgClkHz variable with, initially, the mcgoutclk 
             * Note that this is the raw system clock before the dividers
             * have divided the frequency down
             */
            mcgClkHz = CLOCK_HAL_GetOutClk(MCG_BASE);
            
            /* Get the clock divider values and adjust the mcgClkHz variable */
            settings = CLOCK_SYS_GetOutDiv1();
            mcgClkHz =  mcgClkHz / (settings + 1);
            
            /* Acknowledge which clock mode the device is in */
            if (mcgMode ==1)
            {
                printf("in BLPI mode now at %d Hz\r\n",(int)mcgClkHz );
            }
            if (mcgMode ==2)
            {
                printf(" in FBI mode now at %d Hz\r\n",(int)mcgClkHz );
            }
            if (mcgMode ==3)
            {
                printf(" in FEI mode now at %d Hz\r\n",(int)mcgClkHz );
            }
            if (mcgMode ==4)
            {
                printf(" in FEE mode now at %d Hz\r\n",(int)mcgClkHz );
            }
            if (mcgMode ==5)
            {
                printf(" in FBE mode now at %d Hz\r\n",(int)mcgClkHz );
            }
            if (mcgMode ==6)
            {
                printf(" in BLPE mode now at %d Hz\r\n",(int)mcgClkHz );
            }
            if (mcgMode ==7)
            {
                printf(" in PBE mode now at %d Hz\r\n",(int)mcgClkHz );
            }
            if (mcgMode ==8)
            {
                printf(" in PEE mode now at %d Hz\r\n",(int)mcgClkHz );
            }
            
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
            /* Check the PMSTAT register to see if device is in HS Run */
            if((SMC_HAL_GetStat(SMC_BASE)&&kStatHsrun) == kStatHsrun)
            {
                printf("Currently in HS Run mode. HS Run not supported.\n\r");
                printf("Moving to Normal Run...");
                
                /* Currently in HS Run.  Exit HS Run */
                SMC_HAL_SetRunMode(SMC_BASE, kSmcRun);
                
                while(SMC_HAL_GetStat(SMC_BASE)&&kStatRun != kStatRun)
                {}
            }
#endif
            /*****************************************************************
             * Print the menu 
             ****************************************************************/
            printf("\n\rSelect the desired operation \n\r");
            printf("0 for CASE 0: Enter VLLS0 with POR disabled (Very Low Leakage STOP 0) NO POR\n\r");
            printf("1 for CASE 1: Enter VLLS0 with POR enabled (Very Low Leakage STOP 0) with POR\n\r");
            printf("2 for CASE 2: Enter VLLS1 (Very Low Leakage STOP 1)\n\r");
#if FSL_FEATURE_HAS_VLLS2 /* This macro is currently defined in board.h. */
            printf("3 for CASE 3: Enter VLLS2 (Very Low Leakage STOP 2)\n\r");
#endif
            printf("4 for CASE 4: Enter VLLS3 (Very Low Leakage STOP 3)\n\r");
#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
            printf("5 for CASE 5: Enter LLS(Low Leakage Stop)\n\r");
#endif
            printf("6 for CASE 6: Enter VLPS(Very Low Power Stop)\n\r");
	    printf("7 for CASE 7: VLPR from BLPE (Not supported in this demo yet) \n\r");
            printf("8 for CASE 8: Exit VLPR(Very Low Power RUN)\n\r");
            printf("9 for CASE 9: Enter VLPW(Very Low Power WAIT)\n\r");
            printf("A for CASE 10: Enter WAIT from RUN \n\r");
            printf("B for CASE 11: Enter Normal STOP from RUN or VLPS from VLPR\n\r");
#if FSL_FEATURE_SMC_HAS_PSTOPO
            printf("C for CASE 12: Enter PARTIAL STOP 1 with both system and bus clocks disabled\n\r");
            printf("D for CASE 13: Enter PARTIAL STOP 2 with system clock disabled and bus clock enabled\n\r");
#endif
#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
            printf("F for CASE 15: Enter LLS with LPTMR 1 second wakeup loop (Low Leakage Stop)\n\r");
#endif
            printf("G for CASE 16: Enable LPTMR to wakeup every 5 seconds from any mode except VLLS0\n\r");
            printf("H for CASE 17: Disable LPTMR wakeup\n\r");
            printf("I for CASE 18: Enter VLPR in BLPI at Core Frequency of 4 MHz\n\r");
            printf("J for CASE 19: Enter VLPR in BLPI at Core Frequency of 2 MHz\n\r");
            printf("L for CASE 21: To enable DEBUG");
            
            printf("\n\r > ");
            
            /* Get the user's desired action */
            testVal = getchar();
            
            /* Echo the user's choice */
            printf("%c\n\n\r", testVal);
                        
            /* Normalize the ASCII character to a number to be used in the 
             * Case statement that performs the actions desired.
             */
            if((testVal>='0') && (testVal<='9'))
            {
                testNum = testVal - '0';
            }
            if((testVal>='A') && (testVal<='L'))
            {
                testNum = ((testVal - 'A') + 10);
            }
            if((testVal>='a') && (testVal<='l'))
            {
                testNum = ((testVal - 'a') + 10);
            }
        }
        
        /* Turn off the LEDs to indicate that the command has been received */
        LED1_OFF;
        LED2_OFF;
        LED3_OFF;
#if defined(TOWER)
        LED4_OFF;
#endif
        
        /* Select the proper case and perform the desired actions */
        switch(testNum){
        case 0:/*VLLS0 no POR */
            printf("Press any key to enter VLLS0 with POR disable\n\r ");
            testVal = getchar();
            
            /* Print SW number for exit */
            printf("Press ");
            PRINT_LLWU_SW_NUM;
            printf(" to wake up from VLLS0\n\r ");

            /* Delay for some time to let the print complete */
            myTimeDelay();
            
            /* Setup the device for entry to a low power mode. */
            set_for_lp(0);
            
            /* Move to VLLS0 using the Power Manager */
            ret = POWER_SYS_SetMode(powerModeVlls0Index);
            
            /* The code execution should not get here on successful entries.
             * However, unsuccessful entries should make it to this code and 
             * report errors
             */
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeVlls0Index) returns : %u\n\r", ret);
            }
            break;
            
        case 1:/*VLLS0 */
            printf("Press any key to enter VLLS0 with POR enabled\n\r ");
            getchar();

            /* Print SW number for exit */
            printf("Press ");
            PRINT_LLWU_SW_NUM;
            printf(" to wake up from VLLS0\n\r ");

            /* Delay used to allow the print statement to fully complete before
             * entering the low power mode. 
             */
            myTimeDelay();
            
            /* Setup the device for entry to a low power mode. */
            set_for_lp(0);
            
            /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModeVlls0Index);
            
            /* The code execution should not get here on successful entries.
             * However, unsuccessful entries should make it to this code and 
             * report errors
             */
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeVlls0Index) returns : %u\n\r", ret);
            }
            break;
            
        case 2:/*VLLS1 */
            printf("Press any key to enter VLLS1\n\r ");
            getchar();

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_LLWU_SW_NUM;
            printf(" to wake up from VLLS1\n\r ");

            /* Delay to allow print to complete */
            myTimeDelay();
            
            /* Setup the device for entry to a low power mode. */
            set_for_lp(0);
            
            /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModeVlls1Index);
            
            /* The code execution should not get here on successful entries.
             * However, unsuccessful entries should make it to this code and 
             * report errors
             */
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeVlls1Index) returns : %u\n\r", ret);
            }
            break;
            
#if FSL_FEATURE_HAS_VLLS2
        case 3:/*VLLS2 */
            printf("Press any key to enter VLLS2\n\r ");
            getchar();

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_LLWU_SW_NUM;
            printf(" to wake up from VLLS2\n\r ");

            /* Delay to allow print to complete */
            myTimeDelay();
            
            /* Setup the device for entry to a low power mode. */
            set_for_lp(0);
            
            /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModeVlls2Index);
            
            /* The code execution should not get here on successful entries.
             * However, unsuccessful entries should make it to this code and 
             * report errors
             */
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeVlls2Index) returns : %u\n\r", ret);
            }
            break;
#endif

        case 4:/*VLLS3 */
            printf("Press any key to enter VLLS3\n\r ");
            getchar();

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_LLWU_SW_NUM;
            printf(" to wake up from VLLS3\n\r ");
            
            /* Delay to allow print to complete */
            myTimeDelay();
            
            /* Setup the device for entry to a low power mode. */
            set_for_lp(0);
            
            /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModeVlls3Index);
             
            /* The code execution should not get here on successful entries.
             * However, unsuccessful entries should make it to this code and 
             * report errors
             */
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeVlls3Index) returns : %u\n\r", ret);
            }
            break;

#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE            
        case 5:/*LLS */
            printf("Press any key to enter LLS\n\r ");
            getchar();

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_LLWU_SW_NUM;
            printf(" to wake up from LLS\n\r ");
            
            /* Delay to allow print to complete */
            myTimeDelay();
            
            /* Setup the device for entry to a low power mode. */
            set_for_lp(0);
            
            /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModeLlsIndex);
            
            /* Check for a successful power mode entry */
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeLlsIndex) returns : %u\n\r", ret);
            }
            
            /* Reconfigure the UART and switch pins after exiting low power */
            set_for_lp(1);
            
            /* Check to see what clock mode we are in */
            mcgMode = what_mcg_mode();
#if FSL_FEATURE_MCG_HAS_PLL
            /*
            * After LLS wake up that was enter from PEE the exit will be on PBE
            * for this reason after wake up make sure to get back to desired
            * clock mode
            */
            if(mcgMode==PBE)
            {
                mcgClkHz = pbe_pee(CPU_XTAL_CLK_HZ);
                uartClkHz = mcgClkHz; /* UART clock frequency will equal system frequency */
                UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkHz, BOARD_DEBUG_UART_BAUD);
            }
#endif
            break;
#endif            

        case 6:/*VLPS */
            printf("Press any key to enter VLPS\n\r ");
            getchar();

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_INT_SW_NUM;
            printf(" to wake up from VLPS\n\r ");
            
            /* Delay to allow print to complete */
            myTimeDelay();
            
            /* Setup the device for entry to a low power mode. */
            set_for_lp(2);
            
            /*Go to VLPS*/
            /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModeVlpsIndex);
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeVlpsIndex) returns : %u\n\r", ret);
            }
            
            /* Re-enable pins */
            set_for_lp(1);
            
            /* Check the power mode */
            /* Check the power mode to see if the device is in RUN or VLPR */
            powerMode = POWER_SYS_GetRunningMode();
            if (powerMode == kPowerManagerVlpr) 
            {
                printf("  in VLPR Mode !  ");
            }
            else if (powerMode == kPowerManagerRun)
            {
                mcgMode = what_mcg_mode(); /* check if in PBE mode and if so, switch to PEE */
#if FSL_FEATURE_MCG_HAS_PLL
                if(mcgMode==PBE)
                {
                    mcgClkHz = pbe_pee(CPU_XTAL_CLK_HZ); /* Move back to PEE using the external crystal as reference */
                    uartClkHz = mcgClkHz; /* UART clock frequency will equal system frequency */
                    UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkHz, BOARD_DEBUG_UART_BAUD);
                }
                else{}
#endif
                printf("  VLPS back to Run Mode  !\n\r ");
            }
            else{}
            break;
            
        case 7:/*VLPR */
            printf("VLPR from BLPE not yet supported! \n\n\r");
            break;
            
        case 8:/* Exit VLPR */
            printf("Press any key to exit VLPR\n\r ");
            getchar();
            
            /*Exit VLPR*/
            /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModeRunIndex);
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeRunIndex) returns : %u\n\r", ret);
            }
            
            /* Determine operating clock mode of the device */
            mcgMode = what_mcg_mode();
            if(mcgMode==BLPE)
            {
                /* If the device is in BLPE, the clock mode was PEE (if the 
                 * device has a PLL) or FEE (if no PLL is present), so move 
                 * back to that mode 
                 */
#if FSL_FEATURE_MCG_HAS_PLL
                vlp_clock_config(PEE);
#else
                vlp_clock_config(FEE);
#endif
            }
            /* Else, if the device is in BLPI, return to FEI as the operating
             * clock mode 
             */
            else if (mcgMode==BLPI)
            {
                vlp_clock_config(FEI);
            }
            else{}
            
            /* Check the power mode of the device and provide feedback to user */
            powerMode = POWER_SYS_GetRunningMode();
            
            if(powerMode == kPowerManagerVlpr){
                printf(" in VLPR Mode!\n\r");
            }
            else if (powerMode == kPowerManagerRun){
                printf(" in Run Mode!\n\r");
            }
            else{
                printf(" Unknown power mode!\n\r");
                
                printf("\n\rPower Mode Index = %d\n\r", powerMode);
                printf("SMC_PMSTAT = 0x%X\n\n\r", SMC_HAL_GetStat(SMC_BASE));
            }
            break;
            
        case 9:/* VLPW */
          /* Check the powerMode variable which is set upon every entry into 
           * this loop.  Cannot enter VLPW mode from Normal RUN or HS Run
           */
            if (powerMode == kPowerManagerRun){
                printf("\n\r ERROR - cannot enter VLPW from Run Mode  !\n\r ");
            } else if (powerMode == kPowerManagerVlpr){
                printf("Press any key to enter VLPW\n\r ");
                getchar();

                /* Print SW Number for exit */
                printf("Press ");
                PRINT_INT_SW_NUM;
                printf(" to wake up from VLPW\n\r ");
                
                printf("  in VLPR Mode moving to VLPW!\n\r ");
                CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
                
                 /* Now Issue power mode change */
                ret = POWER_SYS_SetMode(powerModeVlpwIndex);
                if (ret != kPowerManagerSuccess) 
                {
                    printf("POWER_SYS_SetMode(powerModeVlpwIndex) returns : %u\n\r", ret);
                }
                
                /* Now update the powerMode variable to ensure that the mode
                 * change happened successfully
                 */
                powerMode = POWER_SYS_GetRunningMode();
                
                if(powerMode == kPowerManagerVlpr){
                    printf(" VLPW back to VLPR Mode!\n\r");
                }
                else if (powerMode == kPowerManagerRun){
                    printf(" VLPW back to Run Mode!\n\r");
                }
                else{
                    printf(" Unknown power mode!\n\r");
                }
            }
            else{}
            break;

        case 0xa:/*WAIT*/
          /* Check the powerMode variable which is updated upon entry into this
           * loop.  Cannot enter wait mode from VLPR mode. 
           */
            if (powerMode == kPowerManagerVlpr){
                printf("  in VLPR Mode !\n\r ");
                printf("  Cannot enter Wait from VLPR!!\n\r ");
                
                break;
            }
            
            printf("Press any key to enter Wait\n\r ");
            getchar();

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_INT_SW_NUM;
            printf(" to wake up from Wait\n\r ");
            
            
             /* Now Issue power mode change to enter wait */
            ret = POWER_SYS_SetMode(powerModeWaitIndex);
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeWaitIndex) returns : %u\n\r", ret);
            }
            
            /* Get power mode and check it */
            powerMode = POWER_SYS_GetRunningMode();
            
            if(powerMode == kPowerManagerVlpr){
                printf(" in VLPR Mode!\n\r");
            }
            else if (powerMode == kPowerManagerRun){
                printf(" in Run Mode!\n\r");
            }
            else{
                printf(" Unknown power mode!\n\r");
                
                printf("\n\rPower Mode Index = %d\n\r", powerMode);
                printf("SMC_PMSTAT = 0x%X\n\n\r", SMC_HAL_GetStat(SMC_BASE));
            }
            
            break;
            
        case 0xb:/*STOP*/
          /* Check the power mode and change the printout based on power mode */
            if(powerMode == kPowerManagerRun)
            {
                printf("Press any key to enter Normal Stop\n\r ");
                getchar();
            }
            else if(powerMode == kPowerManagerVlpr)
            {
                printf("Currently in VLPR, so press any key to enter VLPS\n\r ");
                getchar();
            }
            else
            {
                printf("\n\n\rUnknown power mode:  Exiting case.\n\n\r");
                break;
            }

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_INT_SW_NUM;
            printf(" to wake up from Normal Stop\n\r ");

            /* Delay for some time to allow print out to complete */
            myTimeDelay();
            
            /* Turn off the clock monitor to be safe; Entering STOP with the 
             * external oscillator running and the clock monitor on will cause
             * a clock monitor reset. 
             */
            CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
            
            /* Setup the device for entry to a low power mode. */
            set_for_lp(2);
            
             /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModeStopIndex);
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeStopIndex) returns : %u\n\r", ret);
            }
            
            /* Re-enable pins */
            set_for_lp(1);
            
            /*
            * After LLS wake up that was enter from PEE the exit will be on PBE
            * for this reason after wake up make sure to get back to desired
            * clock mode
            */
            LED2_OFF;
            LED1_ON;
            /* Check the MCG mode.  */
            mcgMode = what_mcg_mode();
#if FSL_FEATURE_MCG_HAS_PLL
            if(mcgMode==PBE)
            {
                mcgClkHz = pbe_pee(CPU_XTAL_CLK_HZ);
                uartClkHz = mcgClkHz; /* UART clock frequency will equal system frequency */
                UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkHz, BOARD_DEBUG_UART_BAUD);
            }
#endif
            
            /* Get power mode and check it */
            powerMode = POWER_SYS_GetRunningMode();
            
            if(powerMode == kPowerManagerVlpr){
                printf(" Stop back to VLPR Mode!\n\r");
            }
            else if (powerMode == kPowerManagerRun){
                printf(" Stop back to Run Mode!\n\r");
            }
            else{
                printf(" Unknown power mode!\n\r");
                
                printf("\n\rPower Mode Index = %d\n\r", powerMode);
                printf("SMC_PMSTAT = 0x%X\n\n\r", SMC_HAL_GetStat(SMC_BASE));
            }
            
            break;

#if FSL_FEATURE_SMC_HAS_PSTOPO            
        case 0xc:/*PSTOP1 */
            printf("Press any key to enter Stop PSTOP1\n\r ");
            getchar();

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_INT_SW_NUM;
            printf(" to wake up from Stop PSTOP1 \n\r ");
            
            /* Delay for some time to allow print out to complete */
            myTimeDelay();
            CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
            
            /*
            * Enter stop mode
            * Partial Stop Option:
            *  0x00 = STOP - Normal Stop Mode
            *  0x40 = PSTOP1 - Partial Stop with both system and bus clocks disabled
            *  0x80 = PSTOP2 - Partial Stop with system clock disabled and
            *                  bus clock enabled
            */
#if FSL_FEATURE_MCG_HAS_PLL
            /* The next line is to resolve an errata where the LOCK bit is cleared in CPO */
            CLOCK_HAL_SetPllClk0Cmd(MCG_BASE, true); /* set PLLSTEN to keep PLL LOCK bit from clearing */
            
             /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModePstop1Index);
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModePstop1Index) returns : %u\n\r", ret);
            }
            
            /* The next line is to clear PLLSTEN associated with PLL LOCK bit errata */
            CLOCK_HAL_SetPllClk0Cmd(MCG_BASE, false); /* clear PLLSTEN now that CPO mode is finished with */
#endif
            /*
            * After LLS wake up that was enter from PEE the exit will be on PBE
            * for this reason after wake up make sure to get back to desired clock mode
            */
            mcgMode = what_mcg_mode();
#if FSL_FEATURE_MCG_HAS_PLL
            if(mcgMode==PBE)
            {
                mcgClkHz = pbe_pee(CPU_XTAL_CLK_HZ);
                uartClkHz = mcgClkHz; /* UART0 clock frequency will equal system frequency */
                UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkHz, BOARD_DEBUG_UART_BAUD);
            }
#endif
            
            /* Get power mode and check it */
            powerMode = POWER_SYS_GetRunningMode();
            
            if(powerMode == kPowerManagerVlpr){
                printf(" Stop back to VLPR Mode!\n\r");
            }
            else if (powerMode == kPowerManagerRun){
                printf(" Stop back to Run Mode!\n\r");
            }
            else{
                printf(" Unknown power mode!\n\r");
                
                printf("\n\rPower Mode Index = %d\n\r", powerMode);
                printf("SMC_PMSTAT = 0x%X\n\n\r", SMC_HAL_GetStat(SMC_BASE));
            }
            
            break;
            
        case 0xd:/*STOP PSTOP2*/
            printf("Press any key to enter Stop PSTOP2\n\r ");
            getchar();

            /* Print SW Number for exit */
            printf("Press ");
            PRINT_INT_SW_NUM;
            printf(" to wake up from Stop PSTOP2\n\r ");
            
            /* Delay for some time to allow print out to complete */
            myTimeDelay();
            CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
            /*
            * Enter stop mode
            * Partial Stop Option:
            *  0x00 = STOP - Normal Stop Mode
            *  0x40 = PSTOP1 - Partial Stop with both system and bus clocks disabled
            *  0x80 = PSTOP2 - Partial Stop with system clock disabled and bus clock enabled
            */
             /* Now Issue power mode change */
            ret = POWER_SYS_SetMode(powerModePstop2Index);
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModePstop2Index) returns : %u\n\r", ret);
            }
            
            /*
            * After LLS wake up that was enter from PEE the exit will be on PBE
            * for this reason after wake up make sure to get back to desired
            * clock mode
            */
            mcgMode = what_mcg_mode();
#if FSL_FEATURE_MCG_HAS_PLL
            if(mcgMode==PBE)
            {
                mcgClkHz = pbe_pee(CPU_XTAL_CLK_HZ);
                uartClkHz = mcgClkHz; /* UART clock frequency will equal system frequency */
                UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkHz, BOARD_DEBUG_UART_BAUD);
            }
#endif 
            /* Get power mode and check it */
            powerMode = POWER_SYS_GetRunningMode();
            
            if(powerMode == kPowerManagerVlpr){
                printf(" Stop back to VLPR Mode!\n\r");
            }
            else if (powerMode == kPowerManagerRun){
                printf(" Stop back to Run Mode!\n\r");
            }
            else{
                printf(" Unknown power mode!\n\r");
                
                printf("\n\rPower Mode Index = %d\n\r", powerMode);
                printf("SMC_PMSTAT = 0x%X\n\n\r", SMC_HAL_GetStat(SMC_BASE));
            }
            
            break;
#endif

#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE            
        case 0xf:/*LLS loop wake up every 1000 ms */
            printf("Press any key to enter LLS with LPTMR 1 Second wakeup" 
                   "loop\n\r ");
            getchar();
            
            /* First configure the LLWU */
            llwu_configure(LLWU_PIN_NUM, kLlwuExternalPinFallingEdge, LLWU_MOD_WKUP_SRC);
            
            /* Enable and configure the LPTMR */
            CLOCK_SYS_EnableLptimerClock(0);
            LPTMR_DRV_Init(LPTMR_INST, &lptmrConfig, &lptmrState);
            
            /* Set the timer period of the LPTMR to one second.  
             * The time argument provided to this function is in units of us.
             */
            LPTMR_DRV_SetTimerPeriodUs(LPTMR_INST, LPTMR_WKUP_INT_1S);
            
            printf("Entering LLS mode with LPTMR enabled for 1 second " 
                   "interrupts\n\r ");
            
            /* Loop through wakeup entry and exit 10 times */
            for (i=0;i<10;i++)
            {
                printf("Entering LLS mode\n\r");
                CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
                printf("Set for LP ... ");
                
                /* Start the timer */
                LPTMR_DRV_Start(LPTMR_INST);
                
                /* Delay for some time to allow print to complete.  Then 
                 * setup device for low power entry 
                 */
                myTimeDelay();
                set_for_lp(0);
                
                 /* Now Issue power mode change */
                ret = POWER_SYS_SetMode(powerModeLlsIndex);
                if (ret != kPowerManagerSuccess) 
                {
                    printf("POWER_SYS_SetMode(powerModeLlsIndex) returns : %u\n\r", ret);
                }
            
                /* Reconfigure for printing after exiting low power mode */
                set_for_lp(1);
                
                /*
                * After LLS wake up that was enter from PEE the exit will be on PBE
                * for this reason after wake up make sure to get back to desired
                * clock mode
                */
                mcgMode = what_mcg_mode();
#if FSL_FEATURE_MCG_HAS_PLL
                if(mcgMode==PBE)
                {
                    mcgClkHz = pbe_pee(CPU_XTAL_CLK_HZ);
                    uartClkHz = mcgClkHz; /* UART clock frequency will equal system frequency */
                    UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkHz, BOARD_DEBUG_UART_BAUD);
                }
#endif 
                printf("\n Loop Count 0x%02X ",(int)i) ;
                printf("  Back in RUN mode \n\r");
            }
            
            /* Now stop the timer */
            LPTMR_DRV_Stop(LPTMR_INST);
            
            break;
#endif
            
        case 0x10:  /*set up LPTMR alarm to wakeup every 5 secondes */
            /*enable LPTMR to cause LLWU wakeup */
            llwu_configure(LLWU_PIN_NUM, kLlwuExternalPinFallingEdge, LLWU_MOD_WKUP_SRC);
            
            /* Enable the clock gate to the LPTMR control registers, then 
             * configure the LPTMR
             */
            CLOCK_SYS_EnableLptimerClock(0);
            LPTMR_DRV_Init(LPTMR_INST, &lptmrConfig, &lptmrState);
            
            LPTMR_DRV_SetTimerPeriodUs(LPTMR_INST, LPTMR_WKUP_INT_5S);
            
            LPTMR_DRV_Start(LPTMR_INST);
            
            break;
            
        case 0x11:  /* Disable LPTMR alarm */
            /*disable LPTMR to cause LLWU wakeup */
            CLOCK_SYS_EnableLptimerClock(0);
            LPTMR_DRV_Stop(LPTMR_INST);
            
            printf("\n\r  Disabled LPTMR  \n\r");
            break;
            
        case 0x12:/*VLPR in BLPI 4 MHZ */
            /*Maximum clock frequency for this mode is core 4MHz and 
            Flash 800kHz*/
            printf("Press any key to enter VLPR in BLPI mode \n\r ");
            getchar();
            /*Maximum clock frequency for this mode is core 4MHz and Flash 800Khz*/
            printf("Configure clock frequency to 4MHz core clock and 800KHz flash clock\n\r ");
            
            /*If necessary, get out of VLPR to change clock dividers*/
            /* get out of VLPR back to RUN */
            if(powerMode == kPowerManagerVlpr)
            {
                ret = POWER_SYS_SetMode(powerModeRunIndex);
                if (ret != kPowerManagerSuccess) 
                {
                    printf("POWER_SYS_SetMode(powerModeRunIndex) returns : %u\n\r", ret);
                }
            }
            
            /* Reconfigure the clocks for BLPI */
            vlp_clock_config(BLPI);
                   
            /*
            * default clock divider is set to divide by 1 of Fast IRC
            * and Div by 5 for Flash clock
            */
            CLOCK_SYS_SetOutDiv(0,0,0,4);
                   
            /* Get the dividers from the registers to ensure we have the 
             * correct settings and then update mcgClkHz
             */
            settings = CLOCK_SYS_GetOutDiv1();
            mcgClkHz =  CPU_INT_FAST_CLK_HZ / (settings + 1);
                   
            /* Now update the uartClkHz variable and set the baud rate */
            uartClkHz = mcgClkHz; /* UART clock frequency will equal system frequency */
            UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkHz, BOARD_DEBUG_UART_BAUD);
            
            /*Go to VLPR*/
            ret = POWER_SYS_SetMode(powerModeVlprIndex);
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeVlprIndex) returns : %u\n\r", ret);
            }
            
            /* Get power mode and check it */
            powerMode = POWER_SYS_GetRunningMode();
            
            if(powerMode == kPowerManagerVlpr){
                printf(" Stop back to VLPR Mode!\n\r");
            }
            else if (powerMode == kPowerManagerRun){
                printf(" Stop back to Run Mode!\n\r");
            }
            else{
                printf(" Unknown power mode!\n\r");
                
                printf("\n\rPower Mode Index = %d\n\r", powerMode);
                printf("SMC_PMSTAT = 0x%X\n\n\r", SMC_HAL_GetStat(SMC_BASE));
            }
                   
            break;       

        case 0x13:/*VLPR in BLPI 2 MHZ */
            printf("Press any key to enter VLPR\n\r ");
            getchar();

            /*Maximum clock frequency for this mode is core 4MHz and Flash 800KHz*/
            printf("Configure clock frequency to 2MHz core clock and 800KHz flash clock\n\r ");

            /*Get out of VLPR to change clock dividers*/
            /* get out of VLPR back to RUN */
            if(powerMode == kPowerManagerVlpr)
            {
                ret = POWER_SYS_SetMode(powerModeRunIndex);
                if (ret != kPowerManagerSuccess) 
                {
                    printf("POWER_SYS_SetMode(powerModeRunIndex) returns : %u\n\r", ret);
                }
            }
            
            /* Now reconfigure the clocks for BLPI */
            vlp_clock_config(BLPI);
            
            /*
            * default clock divider is set to divide by 2 of Fast IRC
            * and Div by 5 for Bus/Flash
            */
            CLOCK_SYS_SetOutDiv(1,1,1,4);
            
            /* Get the dividers from the registers to ensure we have the 
             * correct settings and then update mcgClkHz
             */
            settings = CLOCK_SYS_GetOutDiv1();
            mcgClkHz =  CPU_INT_FAST_CLK_HZ / (settings + 1);
            
            /* Now update the uartClkHz variable and set the baud rate */
            uartClkHz = mcgClkHz; /* UART clock frequency will equal system frequency */
            UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkHz, BOARD_DEBUG_UART_BAUD);
            
            /* Enter VLPR*/
            ret = POWER_SYS_SetMode(powerModeVlprIndex);
            if (ret != kPowerManagerSuccess) 
            {
                printf("POWER_SYS_SetMode(powerModeVlprIndex) returns : %u\n\r", ret);
            }
            
            /* Get power mode and check it */
            powerMode = POWER_SYS_GetRunningMode();
            
            if(powerMode == kPowerManagerVlpr){
                printf(" Stop back to VLPR Mode!\n\r");
            }
            else if (powerMode == kPowerManagerRun){
                printf(" Stop back to Run Mode!\n\r");
            }
            else{
                printf(" Unknown power mode!\n\r");
                
                printf("\n\rPower Mode Index = %d\n\r", powerMode);
                printf("SMC_PMSTAT = 0x%X\n\n\r", SMC_HAL_GetStat(SMC_BASE));
            }
            
            break;
            
        case 0x15:  /* enable debug mode */
            printf("\n\r*--------------D E B U G    E N A B L E D--------------------*\n\r ");
            SWD_CLK_ENABLE;
            SWD_DIO_ENABLE;
            break;
            
        default:
            break;
        }
        
        testNum = UNDEF_VALUE ;           
    }
}

/*!
* @brief Settings done before swithing in low power mode
* @param outOfLp - indicate whether to do the setting for going in/coming out of low power mode
*
*              = 0 for going into low power mode (low-leakage mode)
*              = 2 for going into low power mode (NOT a low-leakage mode )
*              = 1 for exiting low power mode
*/
void set_for_lp(uint8_t outOfLp)
{
    if(outOfLp == 1) /*outOfLp = 1 for exiting a low power mode. */ 
    {
        CLOCK_SYS_EnableUartClock(BOARD_DEBUG_UART_INSTANCE);
        /* Enable the pins for the selected SCI */
        configure_uart_pins(BOARD_DEBUG_UART_INSTANCE);
                           
        /* Set variable to let other functions know that the UART is now enabled */
        uartEnabled = 1;
                           
        /* Enable Debug */
        SWD_CLK_ENABLE;
        SWD_DIO_ENABLE;
                           
        /* configure normal interrupt switch pin */
        ENABLE_SW_PIN;
                           
        switchPins[1].config.pullSelect = kPortPullUp;
        switchPins[1].config.isPullEnable = true;
        switchPins[1].config.isPassiveFilterEnabled = true;
        switchPins[1].config.interrupt = kPortIntFallingEdge;
                           
                           
        /* Enable the IRQs */
        INT_SYS_EnableIRQ(BOARD_SW_IRQ_NUM);
                           
        /* configure LLWU pin */
        switchPins[0].config.pullSelect = kPortPullUp;
        switchPins[0].config.isPullEnable = true;
        switchPins[0].config.isPassiveFilterEnabled = true;
                          
        /* Call the GPIO driver initialization to put the new settings into effect */
        GPIO_DRV_Init(switchPins, ledPins);
                           
    }
    else  if(outOfLp == 2)/* outOfLp = 2 for going into low power mode (not 
                            * a low-leakage low power mode 
                            */
    {
        /* clock gate off the UART */
        CLOCK_SYS_DisableUartClock(BOARD_DEBUG_UART_INSTANCE);
                           
        /* Disable UART pins */
        DISABLE_DEBUG_CONSOLE_TX;
        DISABLE_DEBUG_CONSOLE_RX;
        
        
                           
        /* Set variable so other functions know the UART is disabled */
        uartEnabled = 0;
                  
        if ((debug == 'n') || (debug == 'N'))
        {
            /* Disable Debug */
            SWD_CLK_DISABLE;
            SWD_DIO_DISABLE;
            
            PORT_HAL_SetPullCmd(PORTA_BASE,3u,true);
            PORT_HAL_SetPullMode(PORTA_BASE,3u,kPortPullUp);
        }
        
        /* Finally, disable the clock monitor */
        CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
    }
    else  /* outOfLp = 0 for going into a low leakage low power mode */
    {
        /* First disable any pins that are being used that are a source of
         * leakage
         */
        DISABLE_SW_INTERRUPT;
        DISABLE_SW_PIN;
                           
        /* clock gate off the UART */
        CLOCK_SYS_DisableUartClock(BOARD_DEBUG_UART_INSTANCE);
                           
        /* Disable UART pins */
        DISABLE_DEBUG_CONSOLE_TX;
        DISABLE_DEBUG_CONSOLE_RX;
                           
        /* Set variable so other functions know the UART is disabled */
        uartEnabled = 0;
           
        if ((debug == 'n') || (debug == 'N'))
        {
            /* Disable Debug */
            SWD_CLK_DISABLE;
            SWD_DIO_DISABLE;
            
            PORT_HAL_SetPullCmd(PORTA_BASE,3u,true);
            PORT_HAL_SetPullMode(PORTA_BASE,3u,kPortPullUp);
        }
        
        /* Disable clock monitor before entering low power mode */
        CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
    }
}                  
                   
/*!
* @brief clock configuration for very low power
* @param nextMode = BLPI or BLPE
*/
void vlp_clock_config(int8_t nextMode)
{
    int32_t currentMcgMode, uartClkFreqHz;
    uint8_t returnCode, l_powerMode;
    uint32_t settings;
    
    
    power_manager_error_code_t ret;
                       
    /* Get the current MCG mode */
    currentMcgMode = what_mcg_mode();
    
    /* Get power mode and check it */
    l_powerMode = POWER_SYS_GetRunningMode();
            
    if (l_powerMode == kPowerManagerVlpr)
    {
        printf("\n\rIn VLPR Mode ! Must go to RUN to change Clock Modes --> ");
        /* Now Issue power mode change */
        ret = POWER_SYS_SetMode(powerModeRunIndex);
        if (ret != kPowerManagerSuccess) 
        {
            printf("POWER_SYS_SetMode(powerModeRunIndex) returns : %u\n\r", ret);
        }
    }
    else if (l_powerMode == kPowerManagerRun)
    {
        printf("In RUN Mode !\n\r");
    }
    else
    {
        printf("\n\n\rUnsupported or unknown power mode!!");
        printf("Aborting!! \n\n\r");
        return;
    }
    
    switch(currentMcgMode)
    {
        /*current_mcg_mode ==BLPI */
        case 1:
          if(nextMode == FEI)
          {
              /* BLPI --> FEI
               * For necessary mode transitions, refer to the MCG chapter of
               * your device specific reference manual. 
               */
              mcgClkHz = blpi_fbi(CPU_INT_SLOW_CLK_HZ, 0);
              mcgClkHz = fbi_fei(CPU_INT_SLOW_CLK_HZ);
              
#ifdef CPU_MK22FN128VDC10
              CLOCK_SYS_SetOutDiv(0,1,1,2);
#else
              CLOCK_SYS_SetOutDiv(0,1,1,3);
#endif
              
#ifndef CPU_MKV10Z32VLF7              
              /* clear PLLFLLSEL to select the FLL for this clock source */
              CLOCK_SYS_SetPllfllSel(kClockPllFllSelFll);
#endif
              
              /* the uart0 clock frequency will equal the FLL frequency */
              uartClkFreqHz = mcgClkHz;
              UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
          }
          break;
        /* current_mcg_mode ==FBI */
        case 2:
          printf("\n\r in FBI mode now \n\r");
          break;
                           
        /* current_mcg_mode ==FEI */
        case 3:
          printf("\n\r in FEI mode ");
          if (nextMode == BLPE)
          {
              /* FEI --> BLPE
               * For necessary mode transitions, refer to the MCG chapter of
               * your device specific reference manual. 
               */
              mcgClkHz =  fei_fbe( CPU_XTAL_CLK_HZ,  kMcgHighGainOscSelHigh, kMcgExternalRefClkSelOsc);
              mcgClkHz = fbe_blpe(CPU_XTAL_CLK_HZ);
              OSC_HAL_SetExternalRefClkCmd(OSC_BASE, true);
              uartClkFreqHz = CPU_XTAL_CLK_HZ; /* UART0 clock frequency will equal ERCLK */

              UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
                               
              printf("\n\r moved to BLPE mode \n\r");
                               
              /* The purpose of moving to BLPE is to setup to move to 
               * VLPR.  Therefore, the clock dividers need to be setup first
               */
              CLOCK_SYS_SetOutDiv(0,0,0,7);
                               
          }
          else if (nextMode == BLPI)
          {
              /* FEI --> BLPI
               * For necessary mode transitions, refer to the MCG chapter of
               * your device specific reference manual. 
               */
              /* next_mode is BLPI */
              CLOCK_HAL_SetFastClkInternalRefDiv(MCG_BASE, 0);	/*set to div by 1 */
              mcgClkHz = fei_fbi(CPU_INT_FAST_CLK_HZ,FAST_IRC);
              mcgClkHz = fbi_blpi(CPU_INT_FAST_CLK_HZ,FAST_IRC);
              /* Internal Fast IRC is 4 MHz so div by 1 for sysclk and div 4 for flash clock */
              /* div 5 for flash clk margin */
              CLOCK_SYS_SetOutDiv(0,0,0,4);
              /* the uart0 clock frequency will equal the FAST IRC frequency */
              uartClkFreqHz = mcgClkHz;
                               
              CLOCK_HAL_SetInternalClkCmd(MCG_BASE, true);
              CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelFast);
                               
              UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
              
              printf("\n\r moved to BLPI mode \n\r");
          }
          else
          {}
                           
          break;
                           
          /* current_mcg_mode ==FEE) */
          case 4:
            printf("\n\r in FEE mode \n\r");
            if (nextMode == BLPI){
                /* FEE --> BLPI
               * For necessary mode transitions, refer to the MCG chapter of
               * your device specific reference manual. 
               */
                CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelSlow);
                CLOCK_HAL_SetInternalClkCmd(MCG_BASE, true);
                CLOCK_HAL_SetFastClkInternalRefDiv(MCG_BASE, 0);	/*set to div by 1 */
                mcgClkHz = fee_fbi(CPU_INT_FAST_CLK_HZ, FAST_IRC);
                mcgClkHz = fbi_blpi(CPU_INT_FAST_CLK_HZ,FAST_IRC);
                
                /* Internal Fast IRC is 4 MHz so div by 1 for sysclk and div 4 for flash clock */
                /* div 5 for flash clk margin */
                CLOCK_SYS_SetOutDiv(0,0,0,4);
                /* the uart0 clock frequency will equal the FAST IRC frequency */
                uartClkFreqHz = mcgClkHz;

                UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
                printf("\n moved to BLPI mode \n");
            }
            else if (nextMode == BLPE){
                /* FEE --> BLPE
               * For necessary mode transitions, refer to the MCG chapter of
               * your device specific reference manual. 
               */
                mcgClkHz = fee_fbe(CPU_XTAL_CLK_HZ);
                mcgClkHz = fbe_blpe(CPU_XTAL_CLK_HZ);
                OSC_HAL_SetExternalRefClkCmd(OSC_BASE, true);
                               
                /* External clock needs to be divided down to 
                * acceptable levels for VLPR mode */
                CLOCK_SYS_SetOutDiv(1,1,1,7);
                               
                settings = CLOCK_SYS_GetOutDiv1();
                mcgClkHz =  mcgClkHz / (settings + 1);
                               
                uartClkFreqHz = CPU_XTAL_CLK_HZ; /* UART0 clock frequency will equal ERCLK   */

                UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
                printf("\n moved to BLPE mode \n");
            }
            else{}
              break;
            /* current_mcg_mode ==FBE) */
            case 5:
              printf("\n\r in FBE mode \n\r");
              break;
            /* current_mcg_mode ==BLPE) */
            case 6:
              printf("\n\r in BLPE mode \n\r");
              if (nextMode == BLPI){
                  
                  /* BLPE --> BLPI
                  * For necessary mode transitions, refer to the MCG chapter of
                  * your device specific reference manual. 
                  */
                  
                  /* Setup the internal clock first */
                  CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelSlow);
                  CLOCK_HAL_SetInternalClkCmd(MCG_BASE, true);
                  CLOCK_HAL_SetFastClkInternalRefDiv(MCG_BASE, 0);	/*set to div by 1 */
                  
                  /* Now perform the appropriate switches */
                  mcgClkHz = blpe_fbe(CPU_XTAL_CLK_HZ);
                  mcgClkHz = fbe_fbi(CPU_INT_FAST_CLK_HZ, FAST_IRC);
                  mcgClkHz = fbi_blpi(CPU_INT_FAST_CLK_HZ,FAST_IRC);
                  
                  /* Internal Fast IRC is 4 MHz so div by 1 for sysclk and div 4 for flash clock */
                  /* div 5 for flash clk margin */
                  CLOCK_SYS_SetOutDiv(0,0,0,4);
                  /* the uart0 clock frequency will equal the FAST IRC frequency */
                  uartClkFreqHz = mcgClkHz;

                  UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
                  printf("\n\r moved to BLPI mode \n\r");
              }
#if FSL_FEATURE_MCG_HAS_PLL
              else if (nextMode == PEE){
                  CLOCK_SYS_SetOutDiv(0,0,0,1);
                  /*After wake up back to the original clock frequency */
                  mcgClkHz = blpe_pbe(CPU_XTAL_CLK_HZ, PLL0_PRDIV,PLL0_VDIV);
                  mcgClkHz = pbe_pee(CPU_XTAL_CLK_HZ);
                               
                  uartClkFreqHz = mcgClkHz;
                  UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
                               
                  printf("moved to PEE clock mode \n\r");
              }
#endif
              else 
              {
                  /* Unsupported transition case */
                  printf("\n\n\rUnsupported transition!\n\n\r");
              }
              break;
                           
#if FSL_FEATURE_MCG_HAS_PLL
            /* current_mcg_mode == PBE */
            case 7:
              /* PBE --> PEE
               * For necessary mode transitions, refer to the MCG chapter of
               * your device specific reference manual. 
               */
              printf("\n\r in PBE mode \n\r");
              if (nextMode == PEE){
                  /* Prepare the clock dividers */
                  CLOCK_SYS_SetOutDiv(0,0,0,1);
                  /* After wake up back to the original clock mode */
                  mcgClkHz = pbe_pee(CPU_XTAL_CLK_HZ);
                  UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, mcgClkHz, BOARD_DEBUG_UART_BAUD);
              }
              printf("moved to PEE clock mode \n\r");
                           
              break; 
            /* current_mcg_mode ==8) PEE */
            case 8:
              if (nextMode == BLPI)
              {
                  /* PEE --> BLPI
                  * For necessary mode transitions, refer to the MCG chapter of
                  * your device specific reference manual. 
                  */
#if (defined(TWR_K22F120M)||defined(FRDM_K22F120M)||defined(TWR_KV31F120M))  /* Use 8Mhz crystal*/
                   /* run ATC test to determine irc trim */
                   returnCode = atc(FAST_IRC, CPU_INT_FAST_CLK_HZ, mcgClkHz);
                   if (returnCode != 0){
                       printf("\n\rAutotrim Failed\n\r");
                   }
                   
                   /* After ATC test, switch modes */
                   mcgClkHz =  pee_pbe(CRYSTAL);
                   mcgClkHz = pbe_fbe(CRYSTAL);
#elif (defined(FRDM_K64F120M)||defined(TWR_K64F120M))  /* Use 50Mhz external oscillator*/
                   mcgClkHz =  pee_pbe(CPU_XTAL_CLK_HZ);
                   mcgClkHz = pbe_fbe(CPU_XTAL_CLK_HZ);
#endif
                   /* Prepare the internal clock first */
                   CLOCK_HAL_SetFastClkInternalRefDiv(MCG_BASE, 0);	/*set to div by 1 */
                   
                   /* Continue mode switch */
                   mcgClkHz = fbe_fbi(CPU_INT_FAST_CLK_HZ, FAST_IRC);
                   mcgClkHz = fbi_blpi(CPU_INT_FAST_CLK_HZ, FAST_IRC);
                   uartClkFreqHz = mcgClkHz; /* UART0 clock frequency will equal ERCLK */
                   
                   /* div 5 for flash clk margin */
                   CLOCK_SYS_SetOutDiv(0,0,0,4);
                   CLOCK_HAL_SetInternalClkCmd(MCG_BASE, true);
                   CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelFast);
                   CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);

                   UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
                   printf("\n\r Now moved to BLPI clock mode \n\r");
               }
               else /*nextMode == blpe*/
               {
                   /* PEE --> BLPE
                  * For necessary mode transitions, refer to the MCG chapter of
                  * your device specific reference manual. 
                  */
                   printf("\n\r in PEE clock mode moving to BLPE clock mode \n\r");
                   returnCode = atc(FAST_IRC, CPU_INT_FAST_CLK_HZ, mcgClkHz);
                   if (returnCode != 0){
                       printf("\n\rAutotrim Failed\n\r");
                   }
                   else {}
#if FSL_FEATURE_MCG_HAS_PLL
                   mcgClkHz = pee_to_blpe();
#endif
                   /* Configure the external oscillator */
                   OSC_HAL_SetExternalRefClkCmd(OSC_BASE, true);

                   /*
                   * External Reference is 8 MHz so div by 2 for sysclk, bus clock,
                   * flexbus clock and div 8 for flash clock
                   */
                   CLOCK_SYS_SetOutDiv(1,1,1,7);

                   /* Get the divider settings from the register to ensure we
                    * have the correct values and adjust the mcgClkHz variable */
                   settings = CLOCK_SYS_GetOutDiv1();
                   mcgClkHz =  mcgClkHz / (settings + 1);
                   uartClkFreqHz = mcgClkHz; /* UART clock frequency will equal system clock */
                       
                   /* Prepare the internal clock */ 
                   CLOCK_HAL_SetInternalClkCmd(MCG_BASE, true);	/* enable irc */
                   CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelFast);	/* select fast irc */
                   CLOCK_HAL_SetFastClkInternalRefDiv(MCG_BASE, 0);	/*set to div by 1 */

                   UART_HAL_SetBaudRate(BOARD_DEBUG_UART_BASEADDR, uartClkFreqHz, BOARD_DEBUG_UART_BAUD);
                   printf("\n\r Now moved to BLPE clock mode \n\r");
               }
               break;
#endif
             default:
               break;
             /* end of case statement*/
    }
}

/*!
* @brief checks the value in the SRS registers and sends
* messages to the terminal announcing the status at the start of the
* code.
*/
void out_srs(void)
{
    /* Check for a stop mode acknowledge reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmStopModeAckErr))
    {
        printf("\n\rStop Mode Acknowledge Error Reset");
    }
    //if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmStopModeAckErr))
    //{
    //    printf("\n\rMDM-AP Reset");
    //}
    /* Check for a software reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmSoftware))
    {
        printf("\n\rSoftware Reset");
    }
    /* Check for Core Lockup */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmCoreLockup))
    {
        printf("\n\rCore Lockup Event Reset");
    }
    /* Check for Power-on Reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmPowerOn))
    {
        printf("\n\rPower-on Reset");
    }
    /* Check for Reset pin reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmExternalPin))
    {
        printf("\n\rExternal Pin Reset");
    }
    /* Check for Watchdog reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmWatchDog))
    {
        printf("\n\rWatchdog(COP) Reset");
    }
    /* Check for Loss of external clock reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmLossOfClk))
    {
        printf("\n\rLoss of External Clock Reset");
    }
#if FSL_FEATURE_RCM_HAS_LOL
    /* Check for Loss of lock reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmLossOfLock))
    {
        printf("\n\rLoss of Lock in PLL Reset");
    }
#endif
    /* Check for low-voltage reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmLowVoltDetect))
    {
        printf("\n\rLow-voltage Detect Reset");
    }
    /* Check for low-leakage power mode recovery reset */
    if (RCM_HAL_GetSrcStatusCmd(RCM_BASE, kRcmWakeup))
    {
        /* If we had a low-leakage stop mode recovery, determine the specifc
         * low-leakage stop mode recovery
         */
        printf("\n\r[outSRS]Wakeup bit set from low power mode ");
#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE           
        if (SMC_HAL_GetStopMode(SMC_BASE) == kSmcLls)
        {
            printf("LLS exit ") ;
        }
#endif        
        if ((SMC_HAL_GetStopMode(SMC_BASE)== kSmcVlls) && (SMC_HAL_GetStopSubMode(SMC_BASE)== kSmcStopSub0))
        {
            printf("VLLS0 exit ") ;
        }
        if ((SMC_HAL_GetStopMode(SMC_BASE)== kSmcVlls) && (SMC_HAL_GetStopSubMode(SMC_BASE)== kSmcStopSub1))
        {
            printf("VLLS1 exit ") ;
        }
        if ((SMC_HAL_GetStopMode(SMC_BASE)== kSmcVlls) && (SMC_HAL_GetStopSubMode(SMC_BASE)== kSmcStopSub2))
        {
            printf("VLLS2 exit") ;
        }
        if ((SMC_HAL_GetStopMode(SMC_BASE)== kSmcVlls) && (SMC_HAL_GetStopSubMode(SMC_BASE)== kSmcStopSub3))
        {
            printf("VLLS3 exit ") ;
        }
    }
    printf("\n\r");
}
   
/*
 * Power manager callback implementation code 
 * 
 * This section defines what the power manager functions do before 
 * and after the power manager mode transitions occur.  It also defines
 * what to do if no callback is provided. 
 *
 */
power_manager_error_code_t callback0(power_manager_callback_type_t type,
        power_manager_user_config_t * configPtr,
        power_manager_callback_data_t * dataPtr) {

    user_callback_data_t * userData = (user_callback_data_t*) dataPtr;
    power_manager_error_code_t ret = kPowerManagerError;

    switch (type) {

    case kPowerManagerCallbackNone:

        userData->none.counter++;
        ret = kPowerManagerSuccess;
        break;
    case kPowerManagerCallbackBefore:

        userData->before.counter++;
        ret = kPowerManagerSuccess;
        break;
    case kPowerManagerCallbackAfter:

        userData->after.counter++;
        ret = kPowerManagerSuccess;
        break;
    default:
        userData->err++;
        break;
    }

    userData->lastType = type;

    return ret;
}

/************************************************************
 * hwtimer_callback
 * 
 * This function performs the application specific interrupt
 * actions of the hwtimer.  For this application, it is only 
 * necessary to set the global boolean variable, gTimeElapsed
 * to true.  
 ****************************************************************/
void hwtimer_callback(void* data)
{
    gTimeElapsed = true;
}

/***************************************************************
 * myTimeDelay
 *
 * This function implements a blocking time delay using the PIT
 * as the timer source. This function accepts no arguments and has 
 * no return values. 
 *****************************************************************/
void myTimeDelay(void)
{
    gTimeElapsed = false;
    /* Set the hardware timer period every time since the bus clock can change
     * in this demo application 
     */
    if (kHwtimerSuccess != HWTIMER_SYS_SetPeriod(&hwtimer, HWTIMER_LL_SRCCLK, HWTIMER_PERIOD))
    {
        printf("\r\nError: hwtimer set period.\r\n");
    }
    
    /* Now start the timer */
    if (kHwtimerSuccess != HWTIMER_SYS_Start(&hwtimer))
    {
        printf("\r\nError: hwtimer start.\r\n");
    }
    
    while(!gTimeElapsed)
    {}
    
    /* Now stop the timer */
    if (kHwtimerSuccess != HWTIMER_SYS_Stop(&hwtimer))
    {
        printf("\r\nError: hwtimer start.\r\n");
    }
}
/*****************************************************************************
*
* Hard Fault handler - This handler is used to provide some debugging in the
* event that the hard fault handler is reached.  
****************************************************************************/
                   
void HardFault_Handler(void)
{
    LED1_DIS;
    LED2_DIS;
    LED3_DIS;
    
    LED4_EN;
              
    LED4_ON;
    while(1)
    {}                   
}

                   
/*******************************************************************************
* EOF
******************************************************************************/
                   
