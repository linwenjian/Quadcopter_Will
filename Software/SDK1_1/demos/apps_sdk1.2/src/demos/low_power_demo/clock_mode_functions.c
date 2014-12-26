

#include "clock_mode_functions.h"


/*!
 * @brief returns the Frequency Locked Loop frequency
 *
 * @param fllRef - reference FLL frequency
 */
 int32_t fll_freq(int32_t fllRef)
 {
     int32_t fllFreqHz = 0;
                       
     /* if DMX32 set */
     if (CLOCK_HAL_GetDmx32(MCG_BASE))
     {
         /* determine multiplier based on DRS */
         switch (CLOCK_HAL_GetDcoRangeMode(MCG_BASE))
         {
         case 0:
           fllFreqHz = (fllRef * 732);
           if (fllFreqHz < 20000000) {return 0x33;}
           else if (fllFreqHz > 25000000) {return 0x34;}
           else{}
           break;
         case 1:
           fllFreqHz = (fllRef * 1464);
           if (fllFreqHz < 40000000) {return 0x35;}
           else if (fllFreqHz > 50000000) {return 0x36;}
           else{}
           break;
         case 2:
           fllFreqHz = (fllRef * 2197);
           if (fllFreqHz < 60000000) {return 0x37;}
           else if (fllFreqHz > 75000000) {return 0x38;}
           else{}
           break;
         case 3:
           fllFreqHz = (fllRef * 2929);
           if (fllFreqHz < 80000000) {return 0x39;}
           else if (fllFreqHz > 100000000) {return 0x3A;}
           else{}
           break;
         default:
           break;
         }
       }
       /* if DMX32 = 0 */
       else
       {
         /* determine multiplier based on DRS */
         switch (CLOCK_HAL_GetDcoRangeMode(MCG_BASE))
         {
         case 0:
           fllFreqHz = (fllRef * 640);
           if (fllFreqHz < 20000000) {return 0x33;}
           else if (fllFreqHz > 25000000) {return 0x34;}
           else{}
           break;
         case 1:
           fllFreqHz = (fllRef * 1280);
           if (fllFreqHz < 40000000) {return 0x35;}
           else if (fllFreqHz > 50000000) {return 0x36;}
           else{}
           break;
         case 2:
           fllFreqHz = (fllRef * 1920);
           if (fllFreqHz < 60000000) {return 0x37;}
           else if (fllFreqHz > 75000000) {return 0x38;}
           else{}
           break;
         case 3:
           fllFreqHz = (fllRef * 2560);
           if (fllFreqHz < 80000000) {return 0x39;}
           else if (fllFreqHz > 100000000) {return 0x3A;}
           else{}
           break;
         default:
           break;
       }
     }
       
     return fllFreqHz;
} /* fll_freq */
                   
/*!
* @brief returns the the current MCG mode
*/
uint8_t what_mcg_mode(void)
{
    /* check if in FEI mode */
    if (((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) && /* check CLKS mux has selcted FLL output */
        (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal)   /* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
          && (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel)
#endif
          ))  /* check PLLS mux has selected FLL */
        {
            /* return FEI code */
            return FEI;
        }
    
#if FSL_FEATURE_MCG_HAS_PLL
    /* Check MCG is in PEE mode */
    else if (((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatPll) && /* check CLKS mux has selcted PLL output */
               (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) /* check FLL ref is external ref clk */
                  &&(CLOCK_HAL_GetPllStatMode(MCG_BASE) == kMcgPllStatPllClkSel)
                  ))	 /* check PLLS mux has selected PLL */
        {
            /* return PEE code */
            return PEE;
        }
                       /* Check MCG is in PBE mode */
    else if (((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) && /* check CLKS mux has selcted external reference */
              (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	  /* check FLL ref is external ref clk */
                (CLOCK_HAL_GetPllStatMode(MCG_BASE) == kMcgPllStatPllClkSel) &&	  /* check PLLS mux has selected PLL */
                  (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))	  /* check MCG_C2[LP] bit is not set */
        
        {
            /* return PBE code */
            return PBE;
        }
#endif
    /* Check MCG is in FBE mode */
    else if (((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) &&   /* check CLKS mux has selcted external reference */
               (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                  (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif 
                    (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is not set */
        {
            /* return FBE code */
            return FBE;
        }
                       /* Check MCG is in BLPE mode */
    else if (((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) && 	/* check CLKS mux has selcted external reference */
               (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
                 (CLOCK_HAL_GetLowPowerMode(MCG_BASE) == kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is set */
        {
            /* return BLPE code */
            return BLPE;
        }
    /* check if in BLPI mode */
    else if (((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef) && 	/* check CLKS mux has selcted int ref clk */
               (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) &&	/* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif
                   (CLOCK_HAL_GetLowPowerMode(MCG_BASE) == kMcgLowPowerSelLowPower)))	/* check LP bit is set */
        {
            return BLPI;    /* return BLPI code */
        }
    /* check if in FBI mode */
    else if (((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef) && 	/* check CLKS mux has selcted int ref clk */
               (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) &&	/* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif 
                   (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))	/* check LP bit is clear */
    {
          /* return FBI code */
          return FBI;
    }
    /* Check MCG is in FEE mode */
    else if (((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) &&    /* check CLKS mux has selcted FLL */
               (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal)/* check FLL ref is external ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                 &&(CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel)
#endif
                     ))	/* check PLLS mux has selected FLL */
        {
            /* return FEE code */
            return FEE;
        }
    else
        {
          return 0;    /* error condition */
        }
} /* what_mcg_mode */
                   
                   
                   
#if FSL_FEATURE_MCG_HAS_PLL
/*!
* @brief switch from PEE mode to BLPE mode
*/
int32_t pee_to_blpe(void)
{
    int32_t mcgClockHz;
                       
    mcgClockHz = atc(FAST_IRC, CPU_INT_FAST_CLK_HZ, MCG_OUT_FREQ);
                       
    mcgClockHz = pee_pbe(CPU_XTAL_CLK_HZ);
    mcgClockHz = pbe_blpe(CPU_XTAL_CLK_HZ);
                       
    return mcgClockHz;
}
                   
/*!
* @brief switch from BLPE mode to PEE mode
*/
int32_t blpe_to_pee(void)
{
    int32_t mcgClockHz;
                       
    CLOCK_SYS_SetOutDiv(0,0,0,1);
                       
    /* After wake up back to the original clock frequency */
    mcgClockHz = blpe_pbe(CPU_XTAL_CLK_HZ, PLL0_PRDIV,PLL0_VDIV);
    mcgClockHz = pbe_pee(CPU_XTAL_CLK_HZ);
                       
    return mcgClockHz;
 }
#endif
                   
                                    
#if FSL_FEATURE_MCG_HAS_PLL
/*!
* @brief switches from PEE to PBE mode
* @param crystalVal - crystal value
*/
int32_t pee_pbe(int32_t crystalVal)
{
    int16_t i;
                       
    /* Check MCG is in PEE mode */
    if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatPll) &&        /* check CLKS mux has selcted PLL output */
      (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
        (CLOCK_HAL_GetPllStatMode(MCG_BASE) == kMcgPllStatPllClkSel)))	/* check PLLS mux has selected PLL */
    {
        return 0x8;                                                       /* return error code */
    }
                       
    /*
    * As we are running from the PLL by default the PLL and external clock settings are valid
    * To move to PBE from PEE simply requires the switching of the CLKS mux to select the ext clock
    */
    /* As CLKS is already 0 the CLKS value can simply be OR'ed into the register  */
    CLOCK_HAL_SetClkSrcMode(MCG_BASE, kMcgClkSelExternal);	/* switch CLKS mux to select external reference clock as MCG_OUT */
                       
                       
    /* Wait for clock status bits to update */
    for (i = 0 ; i < 2000 ; i++)
    {
        if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef)
        {
            break; /* jump out early if CLKST shows EXT CLK slected before loop finishes */
        }
    }
    if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatExternalRef)
    {
        return 0x1A; /* check EXT CLK is really selected and return with error if not */
    }
                       
    /* Now in PBE mode */
    return crystalVal; /* MCGOUT frequency equals external clock frequency */
} /* pee_pbe */
                   
/*!
* @brief switches from PBE to PEE mode
* @param crystalVal - crystal value
*/
int32_t pbe_pee(int32_t crystalVal)
{
    uint8_t prDiv, vDiv;
    int16_t i;
                       
    /* Check MCG is in PBE mode */
    if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) &&       /* check CLKS mux has selcted external reference */
      (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
        (CLOCK_HAL_GetPllStatMode(MCG_BASE) == kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected PLL */
          (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is not set */
    {
        return 0x7;                                                       /* return error code */
    }
                       
    /* As the PLL settings have already been checked when PBE mode was enterred they are not checked here */
                       
    /* Check the PLL state before transitioning to PEE mode */
                       
    /* Check LOCK bit is set before transitioning MCG to PLL output (already checked in fbe_pbe but good practice */
    /* to re-check before switch to use PLL) */
    for (i = 0 ; i < 2000 ; i++)
    {
        if (CLOCK_HAL_GetLock0Mode(MCG_BASE))
        {
            break; /* jump out early if LOCK sets before loop finishes */
        }
    }
    if (!CLOCK_HAL_GetLock0Mode(MCG_BASE))
    {
        return 0x44; /* check bit is really set and return with error if not set */
    }
    /* Use actual PLL settings to calculate PLL frequency */
    prDiv = ((MCG_C5 & MCG_C5_PRDIV0_MASK) + 1);
    vDiv = ((MCG_C6 & MCG_C6_VDIV0_MASK) + 24);
                       
    CLOCK_HAL_SetClkSrcMode(MCG_BASE, kMcgClkSelOut);	/* clear CLKS to switch CLKS mux to select PLL as MCG_OUT */
                       
    /* Wait for clock status bits to update */
    for (i = 0 ; i < 2000 ; i++)
    {
        if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatPll)
        {
            break; /* jump out early if CLKST = 3 before loop finishes */
        }
    }
    if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatPll)
    {
        return 0x1B; /* check CLKST is set correctly and return with error if not */
    }
                       
    /* Now in PEE */
    return ((crystalVal / prDiv) * vDiv); /*MCGOUT equals PLL output frequency */
                       
}  /* pbe_pee */
                   
                   
                   /*!
                   * @brief switches from PBE to FBE mode
                   * @param crystalVal - crystal value
                   */
                   int32_t pbe_fbe(int32_t crystalVal)
                   {
                       int16_t i;
                       
                       /* Check MCG is in PBE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) &&       /* check CLKS mux has selcted external reference */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) == kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected PLL */
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is not set */
                       {
                           return 0x7;                                                       /* return error code */
                       }
                       
                       /*
                       * As we are running from the ext clock, by default the external clock settings are valid
                       * To move to FBE from PBE simply requires the switching of the PLLS mux to disable the PLL
                       */
                       
                       CLOCK_HAL_SetPllSelMode(MCG_BASE, kMcgPllSelFll);	/* clear PLLS to disable PLL, still clocked from ext ref clk */
                       
                       /* wait for PLLST status bit to set */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (!CLOCK_HAL_GetPllStatMode(MCG_BASE))
                           {
                               break; /* jump out early if PLLST clears before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetPllStatMode(MCG_BASE))
                       {
                           return 0x15; /* check bit is really clear and return with error if not clear */
                       }
                       
                       /* Now in FBE mode */
                       return crystalVal; /* MCGOUT frequency equals external clock frequency */
                   } /* pbe_fbe */
                   
                   /*!
                   * @brief switches from FBE to PBE mode
                   * This function transitions the MCG from FBE mode to PBE mode.
                   * This function presently only supports OSC0 and PLL0. Support for OSC1 and PLL1 will be added soon
                   * The function requires the desired OSC and PLL be passed in to it for compatibility with the
                   * future support of OSC/PLL selection
                   *
                   * @param crystalVal - external clock frequency in Hz
                   * @param prDivVal - value to divide the external clock source by to create the desired
                   *                           PLL reference clock frequency
                   * @param vDivVal - value to multiply the PLL reference clock frequency by
                   * Return value : MCGCLKOUT frequency (Hz) or error code
                   */
                   int32_t fbe_pbe(int32_t crystalVal, int8_t prDivVal, int8_t vDivVal)
                   {
                       int16_t i;
                       int32_t pllFreq;
                       
                       /* Check MCG is in FBE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) &&       /* check CLKS mux has selcted external reference */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is not set */
                       {
                           return 0x4;                                                       /* return error code */
                       }
                       
                       /* As the external frequency has already been checked when FBE mode was enterred it is not checked here */
                       
                       /* Check PLL divider settings are within spec. */
                       if ((prDivVal < 1) || (prDivVal > 25)) {return 0x41;}
                       if ((vDivVal < 24) || (vDivVal > 50)) {return 0x42;}
                       
                       /* Check PLL reference clock frequency is within spec. */
                       if (((crystalVal / prDivVal) < 2000000) || ((crystalVal / prDivVal) > 4000000)) {return 0x43;}
                       
                       /* Check PLL output frequency is within spec. */
                       pllFreq = (crystalVal / prDivVal) * vDivVal;
                       if ((pllFreq < 48000000) || (pllFreq > 100000000)) {return 0x45;}
                       
                       /* Configure MCG_C5 */
                       /* If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code. */
                       CLOCK_HAL_SetPllExternalRefDiv0(MCG_BASE, prDivVal - 1);	/*set PLL ref divider */
                       
                       /* Configure MCG_C6 */
                       /*
                       * The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
                       * The clock monitor is not enabled here as it has likely been enabled previously and so the value of CME
                       * is not altered here.
                       *
                       * The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
                       */
                       /* write new VDIV and enable PLL */
                       CLOCK_HAL_SetVoltCtrlOscDiv0(MCG_BASE, vDivVal - 24);
                       CLOCK_HAL_SetPllSelMode(MCG_BASE, kMcgPllSelPllClkSel);
                       
                       /* wait for PLLST status bit to set */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetPllStatMode(MCG_BASE))
                           {
                               break; /* jump out early if PLLST sets before loop finishes */
                           }
                       }
                       if (!CLOCK_HAL_GetPllStatMode(MCG_BASE))
                       {
                           return 0x16; /* check bit is really set and return with error if not set */
                       }
                       
                       /* Wait for LOCK bit to set */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetLock0Mode(MCG_BASE))
                           {
                               break; /* jump out early if LOCK sets before loop finishes */
                           }
                       }
                       if (!CLOCK_HAL_GetLock0Mode(MCG_BASE))
                       {
                           return 0x44; /* check bit is really set and return with error if not set */
                       }
                       
                       /* now in PBE */
                       return crystalVal; /* MCGOUT frequency equals external clock frequency */
                   } /* fbe_pbe */
                   
                   /*!
                   * @brief switches from PBE to BLPE mode
                   * @param crystalVal - crystal value
                   */
                   int32_t pbe_blpe(int32_t crystalVal)
                   {
                       /* Check MCG is in PBE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) &&       /* check CLKS mux has selcted external reference */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) == kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected PLL */
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is not set */
                       {
                           return 0x7;                                                       /* return error code */
                       }
                       
                       /* To enter BLPE mode the LP bit must be set, disabling the PLL */
                       CLOCK_HAL_SetLowPowerMode(MCG_BASE, kMcgLowPowerSelLowPower);
                       
                       
                       /* Now in BLPE mode */
                       return crystalVal;
                   } /* pbe_blpe */
                   
                   /*!
                   * @brief switches from BLPE to PBE mode
                   * Since PBE mode can be enterred via FBE -> BLPE modes, it cannot be assumed that the PLL has been
                   * previously configured correctly. That is why this general purpose driver has the PLL settings as
                   * passed parameters.
                   * @param crystalVal - external clock frequency in Hz
                   * @param prDivVal - value to divide the external clock source by to create the desired
                   *                           PLL reference clock frequency
                   * @param vDivVal - value to multiply the PLL reference clock frequency by
                   * Return value : MCGCLKOUT frequency (Hz) or error code
                   */
                   int32_t blpe_pbe(int32_t crystalVal, int8_t prDivVal, int8_t vDivVal)
                   {
                       int16_t i;
                       
                       /* Check MCG is in BLPE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) && 	/* check CLKS mux has selcted external reference */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
                                 (CLOCK_HAL_GetLowPowerMode(MCG_BASE) == kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is set */
                       {
                           return 0x6;                                                       /* return error code */
                       }
                       
                       /* As the external frequency has already been checked when FBE mode was enterred it is not checked here */
                       
                       /* Check PLL divider settings are within spec. */
                       if ((prDivVal < 1) || (prDivVal > 25)) {return 0x41;}
                       if ((vDivVal < 24) || (vDivVal > 50)) {return 0x42;}
                       
                       /* Check PLL reference clock frequency is within spec. */
                       if (((crystalVal / prDivVal) < 2000000) || ((crystalVal / prDivVal) > 4000000)) {return 0x43;}
                       
                       /* If PRDIV, VDIV and the PLL ref clock are in spec. then the PLL frequency is within spec. */
                       
                       /* Configure MCG_C5 */
                       /* If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code. */
                       CLOCK_HAL_SetPllExternalRefDiv0(MCG_BASE, prDivVal - 1);	/*set PLL ref divider */
                       
                       /* Configure MCG_C6 */
                       /*
                       * The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
                       * The clock monitor is not enabled here as it has likely been enabled previously and so the value of CME
                       * is not altered here.
                       */
                       /* The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6 */
                       /* write new VDIV and enable PLL */
                       CLOCK_HAL_SetVoltCtrlOscDiv0(MCG_BASE, vDivVal - 24);
                       CLOCK_HAL_SetPllSelMode(MCG_BASE, kMcgPllSelPllClkSel);
                       
                       /* Now that PLL is configured, LP is cleared to enable the PLL */
                       CLOCK_HAL_SetLowPowerMode(MCG_BASE, kMcgLowPowerSelNormal);
                       
                       /* wait for PLLST status bit to set */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetPllStatMode(MCG_BASE))
                           {
                               break; /* jump out early if PLLST sets before loop finishes */
                           }
                       }
                       if (!CLOCK_HAL_GetPllStatMode(MCG_BASE))
                       {
                           return 0x16; /* check bit is really set and return with error if not set */
                       }
                       
                       /* Wait for LOCK bit to set */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetLock0Mode(MCG_BASE))
                           {
                               break; /* jump out early if LOCK sets before loop finishes */
                           }
                       }
                       if (!CLOCK_HAL_GetLock0Mode(MCG_BASE))
                       {
                           return 0x44; /* check bit is really set and return with error if not set */
                       }
                       
                       /* now in PBE */
                       return crystalVal; /* MCGOUT frequency equals external clock frequency */
                   } /* blpe_pbe */
#endif
                   
                   /*!
                   * @brief switches from BLPE to FBE mode
                   * @param crystalVal - external clock frequency in Hz
                   */
                   int32_t blpe_fbe(int32_t crystalVal)
                   {
                       int16_t i;
                       
                       /* Check MCG is in BLPE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) &&       /* check CLKS mux has selcted external reference */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
                                 (CLOCK_HAL_GetLowPowerMode(MCG_BASE) == kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is set */
                       {
                           return 0x6;                                                       /* return error code */
                       }
                       
#if FSL_FEATURE_MCG_HAS_PLL
                       /* To move from BLPE to FBE the PLLS mux be set to select the FLL output and the LP bit must be cleared */
                       CLOCK_HAL_SetPllSelMode(MCG_BASE, kMcgPllSelFll);	/* clear PLLS to select the FLL */
#endif
                       CLOCK_HAL_SetLowPowerMode(MCG_BASE, kMcgLowPowerSelNormal);	/* clear LP bit */
                       
#if FSL_FEATURE_MCG_HAS_PLL
                       /* wait for PLLST status bit to clear */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (!CLOCK_HAL_GetPllStatMode(MCG_BASE))
                           {
                               break; /* jump out early if PLLST clears before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetPllStatMode(MCG_BASE))
                       {
                           return 0x15; /* check bit is really clear and return with error if not clear  */
                       }
#endif
                       
                       /* now in FBE mode */
                       return crystalVal; /* MCGOUT frequency equals external clock frequency */
                   } /* blpe_fbe */
                   
                   /*!
                   * @brief switches from FBE to PLPE mode
                   * @param crystalVal - external clock frequency in Hz
                   */
                   int32_t fbe_blpe(int32_t crystalVal)
                   {
                       /* Check MCG is in FBE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) && 	/* check CLKS mux has selcted external reference */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))	/* check MCG_C2[LP] bit is not set */
                       {
                           return 0x4;                                                       /* return error code */
                       }
                       
                       /* To move from FBE to BLPE the LP bit must be set */
                       CLOCK_HAL_SetLowPowerMode(MCG_BASE, kMcgLowPowerSelLowPower);	/* set LP bit */
                       
                       /* now in FBE mode */
                       return crystalVal; /* MCGOUT frequency equals external clock frequency */
                   } /* fbe_blpe */
                   
                   /*!
                   * @brief switches from FEI to FBE mode
                   *
                   * Mode transition: FEI to FBE mode
                   *
                   * This function transitions the MCG from FEI mode to FBE mode. This is
                   * achieved by setting the MCG_C2[LP] bit. There is no status bit to
                   * check so 0 is always returned if the function was called with the MCG
                   * in FBI mode. The MCGCLKOUT frequency does not change
                   *
                   * @param crystalVal - external clock frequency in Hz
                   * @param hgoVal - selects whether low power or high gain mode is selected
                   *                           for the crystal oscillator. This has no meaning if an
                   *                           external clock is used.
                   * @param erefsVal - selects external clock (=kMcgErefClockSelectExt) or
                   *                                    crystal osc (=kMcgExternalRefClkSelOsc)
                   * Return value : MCGCLKOUT frequency (Hz) or error code
                   */
                   int32_t fei_fbe(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal)
                   {
                       uint8_t frDivVal;
                       int16_t i;
                       
                       /* check if in FEI mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) &&        /* check CLKS mux has selcted FLL output */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) 	/* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 && (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel)
#endif 
                           ))	/* check PLLS mux has selected FLL */
                       {
                           return 0x1;                                                     /* return error code */
                       }
                       
                       /* check external frequency is less than the maximum frequency */
                       if  (crystalVal > 50000000) {return 0x21;}
                       
                       /* check crystal frequency is within spec. if crystal osc is being used */
                       if (erefsVal)
                       {
                           if ((crystalVal < 30000) ||
                               ((crystalVal > 40000) && (crystalVal < 3000000)) ||
                                   (crystalVal > 32000000)) {return 0x22;} /* return error if one of the available crystal options is not available */
                       }
                       
                       /* make sure HGO will never be greater than 1. Could return an error instead if desired. */
                       if (hgoVal > 0)
                       {
                           hgoVal = kMcgHighGainOscSelHigh; /* force hgo_val to 1 if > 0 */
                       }
                       
                       /* configure the MCG_C2 register */
                       /* the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value */
                       /* it still needs to be set correctly even if the oscillator is not being used */
                       if (crystalVal <= 40000)
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelLow);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       else if (crystalVal <= 8000000)
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelHigh);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       else
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelVeryHigh);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       
                       
                       /* determine FRDIV based on reference clock frequency */
                       /* since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here. */
                       if (crystalVal <= 1250000) {frDivVal = 0;}
                       else if (crystalVal <= 2500000) {frDivVal = 1;}
                       else if (crystalVal <= 5000000) {frDivVal = 2;}
                       else if (crystalVal <= 10000000) {frDivVal = 3;}
                       else if (crystalVal <= 20000000) {frDivVal = 4;}
                       else {frDivVal = 5;}
                       
                       /*
                       * Select external oscilator and Reference Divider and clear IREFS to start ext osc
                       * If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
                       * CLKS=2, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
                       */
                       /* Set the required CLKS and FRDIV values */
                       CLOCK_HAL_SetClksFrdivInternalRefSelect(MCG_BASE, kMcgClkSelExternal, frDivVal, kMcgInternalRefClkSrcExternal);
                       
                       /* if the external oscillator is used need to wait for OSCINIT to set */
                       if (erefsVal)
                       {
                           for (i = 0 ; i < 10000 ; i++)
                           {
                               if (CLOCK_HAL_GetOscInit0(MCG_BASE))
                               {
                                   break; /* jump out early if OSCINIT sets before loop finishes */
                               }
                           }
                           if (!CLOCK_HAL_GetOscInit0(MCG_BASE))
                           {
                               return 0x23; /* check bit is really set and return with error if not set */
                           }
                       }
                       
                       /* wait for Reference clock Status bit to clear */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (!CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                           {
                               break; /* jump out early if IREFST clears before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                       {
                           return 0x11; /* check bit is really clear and return with error if not set */
                       }
                       
                       /* Wait for clock status bits to show clock source is ext ref clk */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef)
                           {
                               break; /* jump out early if CLKST shows EXT CLK slected before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatExternalRef)
                       {
                           return 0x1A; /* check EXT CLK is really selected and return with error if not */
                       }
                       
                       return crystalVal; /* MCGOUT frequency equals external clock frequency */
                   } /* fei_fbe */
                   
                   /*!
                   * @brief switches from FEE to FBE mode
                   * @param crystalVal - external clock frequency in Hz
                   */
                   int32_t fee_fbe(int32_t crystalVal)
                   {
                       int16_t i;
                       
                       /* Check MCG is in FEE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) &&        /* check CLKS mux has selcted FLL */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) /* check FLL ref is external ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 &&(CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel)
#endif 
                           ))	/* check PLLS mux has selected FLL */
                       {
                           return 0x2;                                                       /* return error code */
                       }
                       
                       /* Set CLKS field to 2 to switch CLKS mux to select ext ref clock */
                       /* MCG is current in FEE mode so CLKS field = 0 so can just OR in new value */
                       CLOCK_HAL_SetClkSrcMode(MCG_BASE, kMcgClkSelExternal); /* set CLKS to select ext ref clock */
                       
                       /* Wait for clock status bits to show clock source is ext ref clk */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef)
                           {
                               break; /* jump out early if CLKST shows EXT CLK slected before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatExternalRef)
                       {
                           return 0x1A; /* check EXT CLK is really selected and return with error if not */
                       }
                       
                       /* Now in FBE mode */
                       return crystalVal;
                   } /* fee_fbe */
                   
                   /*!
                   * @brief switches from FBE to PLPE mode
                   * @param ircFreq - IRC clock frequency
                   * @param ircSelect - 0 if slow irc, 1 if fast irc
                   */
                   int32_t fbe_fbi(int32_t ircFreq, uint8_t ircSelect)
                   {
                       uint8_t fcrDivVal;
                       int16_t i;
                       
                       /* Check MCG is in FBE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef) && 	/* check CLKS mux has selcted external reference */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) &&	/* check FLL ref is external ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))/* check MCG_C2[LP] bit is not set */
                       {
                           return 0x4;                                                       /* return error code */
                       }
                       
                       /* Check that the irc frequency matches the selected IRC */
                       if (!(ircSelect))
                       {
                           if ((ircFreq < 31250) || (ircFreq > 39063)) {return 0x31;}
                       }
                       else
                       {
                           if ((ircFreq < 3000000) || (ircFreq > 5000000)) {return 0x32;} /* Fast IRC freq */
                           else{}
                       }
                       
                       /* Select the required IRC */
                       if (ircSelect)
                       {
                           CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelFast); /* select fast IRC by setting IRCS */
                       }
                       else
                       {
                           CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelSlow); /* select slow IRC by clearing IRCS */
                       }
                       
                       /* Make sure the clock monitor is disabled before switching modes otherwise it will trigger */
                       CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
                       
                       /* Select the IRC as the CLKS mux selection */
                       /* select IRC as MCGOUT and enable IREFS */
                       CLOCK_HAL_SetClksFrdivInternalRefSelect(MCG_BASE, kMcgClkSelInternal, 
                                                               CLOCK_HAL_GetFllExternalRefDiv(MCG_BASE), kMcgInternalRefClkSrcSlow);
                       
                       /* wait until internal reference switches to requested irc. */
                       if (!(ircSelect))
                       {
                           for (i = 0 ; i < 2000 ; i++)
                           {
                               if (!(CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE)))
                               {
                                   break; /* jump out early if IRCST clears before loop finishes */
                               }
                           }
                           if (CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE))
                           {
                               return 0x13; /* check bit is really clear and return with error if set */
                           }
                       }
                       else
                       {
                           for (i = 0 ; i < 2000 ; i++)
                           {
                               if (CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE))
                               {
                                   break; /* jump out early if IRCST sets before loop finishes */
                               }
                               else{}
                           }
                           if (!(CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE)))
                           {
                               return 0x14; /* check bit is really set and return with error if not set */
                           }
                           else{}
                       }
                       
                       /* Wait for clock status bits to update */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef)
                           {
                               break; /* jump out early if CLKST shows IRC slected before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatInternalRef)
                       {
                           return 0x19; /* check IRC is really selected and return with error if not */
                       }
                       
                       /* wait for Reference clock Status bit to set */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                           {
                               break; /* jump out early if IREFST sets before loop finishes */
                           }
                       }
                       if (!CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                       {
                           return 0x12; /* check bit is really set and return with error if not set */
                       }
                       
                       /* Now in FBI mode */
                       
                       if (ircSelect)
                       {
                           fcrDivVal = (1 << CLOCK_HAL_GetFastClkInternalRefDiv(MCG_BASE)); /* calculate the fast IRC divder factor */
                           return (ircFreq / fcrDivVal); /* MCGOUT frequency equals fast IRC frequency divided by FCRDIV factor */
                       }
                       else
                       {
                           return ircFreq; /* MCGOUT frequency equals slow IRC frequency */
                       }
                   } /*fbe_fbi*/
                   
                   /*!
                   * @brief switches from FBI to FBE mode
                   *
                   * @param crystalVal - external clock frequency in Hz
                   * @param hgoVal - selects whether low power or high gain mode is selected
                   *                           for the crystal oscillator. This has no meaning if an
                   *                           external clock is used.
                   * @param erefsVal - selects external clock (=kMcgErefClockSelectExt) or
                   *                                    crystal osc (=kMcgExternalRefClkSelOsc)
                   * Return value : MCGCLKOUT frequency (Hz) or error code
                   */
                   int32_t fbi_fbe(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal)
                   {
                       uint8_t frDivVal;
                       int16_t i;
                       
                       /* check if in FBI mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef) && 	/* check CLKS mux has selcted int ref clk */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) &&	/* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))/* check LP bit is clear */
                       {
                           return 0x3;                                                       /* MCG not in correct mode return fail code */
                       }
                       
                       /* check external frequency is less than the maximum frequency */
                       if  (crystalVal > 50000000) {return 0x21;}
                       
                       /* check crystal frequency is within spec. if crystal osc is being used */
                       if (erefsVal)
                       {
                           if ((crystalVal < 30000) ||
                               ((crystalVal > 40000) && (crystalVal < 3000000)) ||
                                   (crystalVal > 32000000)) {return 0x22;} /* return error if one of the available crystal options is not available */
                       }
                       
                       /* make sure HGO will never be greater than 1. Could return an error instead if desired. */
                       if (hgoVal > 0)
                       {
                           hgoVal = kMcgHighGainOscSelHigh; /* force hgo_val to 1 if > 0 */
                       }
                       
                       /* configure the MCG_C2 register */
                       /* the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value */
                       /* it still needs to be set correctly even if the oscillator is not being used */
                       if (crystalVal <= 40000)
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelLow);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       else if (crystalVal <= 8000000)
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelHigh);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       else
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelVeryHigh);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       
                       /* determine FRDIV based on reference clock frequency */
                       /* since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here. */
                       if (crystalVal <= 1250000) {frDivVal = 0;}
                       else if (crystalVal <= 2500000) {frDivVal = 1;}
                       else if (crystalVal <= 5000000) {frDivVal = 2;}
                       else if (crystalVal <= 10000000) {frDivVal = 3;}
                       else if (crystalVal <= 20000000) {frDivVal = 4;}
                       else {frDivVal = 5;}
                       
                       /*
                       * Select external oscilator and Reference Divider and clear IREFS to start ext osc
                       * If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
                       * CLKS=2, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
                       */
                       /* Set the required CLKS and FRDIV values */
                       CLOCK_HAL_SetClksFrdivInternalRefSelect(MCG_BASE, kMcgClkSelExternal, frDivVal, kMcgInternalRefClkSrcExternal);
                       
                       /* if the external oscillator is used need to wait for OSCINIT to set */
                       if (erefsVal)
                       {
                           for (i = 0 ; i < 10000 ; i++)
                           {
                               if (CLOCK_HAL_GetOscInit0(MCG_BASE))
                               {
                                   break; /* jump out early if OSCINIT sets before loop finishes */
                               }
                           }
                           if (!CLOCK_HAL_GetOscInit0(MCG_BASE))
                           {
                               return 0x23; /* check bit is really set and return with error if not set */
                           }
                       }
                       
                       /* wait for Reference clock Status bit to clear */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (!CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                           {
                               break; /* jump out early if IREFST clears before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                       {
                           return 0x11; /* check bit is really clear and return with error if not set */
                       }
                       
                       /* Wait for clock status bits to show clock source is ext ref clk */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef)
                           {
                               break; /* jump out early if CLKST shows EXT CLK slected before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatExternalRef)
                       {
                           return 0x1A; /* check EXT CLK is really selected and return with error if not */
                       }
                       
                       return crystalVal; /* MCGOUT frequency equals external clock frequency */
                   } /* fbi_fbe*/
                   
                   /*!
                   * @brief switches from FBI to BLPI mode
                   * This function transitions the MCG from FBI mode to BLPI mode. This is
                   * achieved by setting the MCG_C2[LP] bit. There is no status bit to
                   * check so 0 is always returned if the function was called with the MCG
                   * in FBI mode.
                   *
                   * @param ircFreq - IRC clock frequency
                   * @param ircSelect - 0 if slow irc, 1 if fast irc
                   * Return value : MCGOUT frequency or error code
                   */
                   int32_t fbi_blpi(int32_t ircFreq, uint8_t ircSelect)
                   {
                       uint8_t fcrDivVal;
                       
                       /* check if in FBI mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef) && 	/* check CLKS mux has selcted int ref clk */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) &&	/* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))/* check LP bit is clear */
                       {
                           return 0x3;                                                       /* MCG not in correct mode return fail code */
                       }
                       
                       /* Set LP bit to disable the FLL and enter BLPI */
                       CLOCK_HAL_SetLowPowerMode(MCG_BASE, kMcgLowPowerSelLowPower);
                       
                       /* Now in BLPI */
                       if (ircSelect)
                       {
                           fcrDivVal = (1 << CLOCK_HAL_GetFastClkInternalRefDiv(MCG_BASE)); /* calculate the fast IRC divder factor */
                           return (ircFreq / fcrDivVal); /* MCGOUT frequency equals fast IRC frequency divided by 2 */
                       }
                       else
                       {
                           return ircFreq; /* MCGOUT frequency equals slow IRC frequency */
                       }
                   } /* fbi_blpi*/
                   
                   
                   
                   /*!
                   * @brief switches from BLPI to FBI mode
                   *
                   * This function transitions the MCG from BLPI mode to FBI mode. This is
                   * achieved by clearing the MCG_C2[LP] bit. There is no status bit to
                   * check so 0 is always returned if the function was called with the MCG
                   * in BLPI mode.
                   *
                   * @param ircFreq - IRC clock frequency
                   * @param ircSelect - 0 if slow irc, 1 if fast irc
                   * Return value : MCGOUT frequency or error code
                   */
                   int32_t blpi_fbi(int32_t ircFreq, uint8_t ircSelect)
                   {
                       uint8_t fcrDivVal;
                       /* check if in BLPI mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef) && 	/* check CLKS mux has selcted int ref clk */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) &&	/* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) == kMcgLowPowerSelLowPower)))/* check LP bit is set */
                       {
                           return 0x5;                                                       /* MCG not in correct mode return fail code */
                       }
                       
                       /* Clear LP bit to enable the FLL and enter FBI mode */
                       CLOCK_HAL_SetLowPowerMode(MCG_BASE, kMcgLowPowerSelNormal);
                       
                       /* Now in FBI mode */
                       if (ircSelect)
                       {
                           fcrDivVal = (1 << CLOCK_HAL_GetFastClkInternalRefDiv(MCG_BASE)); /* calculate the fast IRC divder factor */
                           return (ircFreq / fcrDivVal); /* MCGOUT frequency equals fast IRC frequency divided by 2 */
                       }
                       else
                       {
                           return ircFreq; /* MCGOUT frequency equals slow IRC frequency */
                       }
                   } /* blpi_fbi*/
                   
                   /*!
                   * @brief switches from FEE to FBI mode
                   * @param ircFreq - IRC clock frequency
                   * @param ircSelect - 0 if slow irc, 1 if fast irc
                   * Return value : MCGOUT frequency or error code
                   */
                   int32_t fee_fbi(int32_t ircFreq, uint8_t ircSelect)
                   {
                       uint8_t fcrDivVal;
                       int16_t i;
                       
                       /* Check MCG is in FEE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) && 	/* check CLKS mux has selcted FLL output */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal) /* check FLL ref is external ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 &&(CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel)
#endif
                           ))	/* check PLLS mux has selected FLL */
                       {
                           return 0x2;                                                     /* return error code */
                       }
                       
                       /* Check that the irc frequency matches the selected IRC */
                       if (!(ircSelect))
                       {
                           if ((ircFreq < 31250) || (ircFreq > 39063)) {return 0x31;}
                           else{}
                       }
                       else
                       {
                           if ((ircFreq < 3000000) || (ircFreq > 5000000)) {return 0x32;} /* Fast IRC freq */
                           else{}
                       }
                       
                       /* Select the required IRC */
                       if (ircSelect)
                       {
                           CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelFast); /* select fast IRC by setting IRCS */
                       }
                       else
                       {
                           CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelSlow); /* select slow IRC by clearing IRCS */
                       }
                       
                       /* Make sure the clock monitor is disabled before switching modes otherwise it will trigger */
                       CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
                       
                       /* Select the IRC as the CLKS mux selection */
                       /* set IREFS and select IRC as MCGOUT */
                       CLOCK_HAL_SetClksFrdivInternalRefSelect(MCG_BASE, kMcgClkSelInternal, CLOCK_HAL_GetFllExternalRefDiv(MCG_BASE),kMcgInternalRefClkSrcSlow);
                       
                       /* wait until internal reference switches to requested irc. */
                       if (!(ircSelect))
                       {
                           for (i = 0 ; i < 2000 ; i++)
                           {
                               if (!(CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE)))
                               {
                                   break; /* jump out early if IRCST clears before loop finishes */
                               }
                           }
                           if (CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE))
                           {
                               return 0x13; /* check bit is really clear and return with error if set */
                           }
                       }
                       else
                       {
                           for (i = 0 ; i < 2000 ; i++)
                           {
                               if (CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE))
                               {
                                   break; /* jump out early if IRCST sets before loop finishes */
                               }
                               else{}
                           }
                           if (!(CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE)))
                           {
                               return 0x14; /* check bit is really set and return with error if not set */
                           }
                           else{}
                       }
                       
                       /* Wait for clock status bits to update */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef)
                           {
                               break; /* jump out early if CLKST shows IRC slected before loop finishes */
                           }
                           else{}
                       }
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatInternalRef)
                       {
                           return 0x19; /* check IRC is really selected and return with error if not */
                       }
                       else{}
                       
                       /* wait for Reference clock Status bit to set */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                           {
                               break; /* jump out early if IREFST sets before loop finishes */
                           }
                           else{}
                       }
                       if (!CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                       {
                           return 0x12; /* check bit is really set and return with error if not set */
                       }
                       else{}
                       
                       /* Now in FBI mode */
                       if (ircSelect)
                       {
                           fcrDivVal = (1 << CLOCK_HAL_GetFastClkInternalRefDiv(MCG_BASE)); /* calculate the fast IRC divder factor */
                           return (ircFreq / fcrDivVal); /* MCGOUT frequency equals fast IRC frequency divided by 2 */
                       }
                       else
                       {
                           return ircFreq; /* MCGOUT frequency equals slow IRC frequency */
                       }
                   } /* fee_fbi */
                   
                   /*!
                   * @brief switches from FBI to FEI mode
                   * @param slowIrcFreqInner - slow IRD frequency
                   */
                   int32_t fbi_fei(int32_t slowIrcFreqInner)
                   {
                       int16_t i;
                       int32_t mcgOut;
                       
                       /* check if in FBI mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef) && 	/* check CLKS mux has selcted int ref clk */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) &&	/* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel) &&	/* check PLLS mux has selected FLL */
#endif 
                                     (CLOCK_HAL_GetLowPowerMode(MCG_BASE) != kMcgLowPowerSelLowPower)))/* check LP bit is clear */
                       {
                           return 0x3;                                                       /* MCG not in correct mode return fail code */
                       }
                       else{}
                       
                       /* Check IRC frequency is within spec. */
                       if ((slowIrcFreqInner < 31250) || (slowIrcFreqInner > 39063))
                       {
                           return 0x31;
                       }
                       else{}
                       
                       /* Check resulting FLL frequency */
                       mcgOut = fll_freq(slowIrcFreqInner);
                       if (mcgOut < 0x3C) {return mcgOut;} /* If error code returned, return the code to calling function */
                       
                       /* Change the CLKS mux to select the FLL output as MCGOUT */
                       
                       /* select FLL as MCGOUT */
                       /* make sure IRC is FLL reference */
                       CLOCK_HAL_SetClksFrdivInternalRefSelect(MCG_BASE, kMcgClkSelOut, CLOCK_HAL_GetFllExternalRefDiv(MCG_BASE), kMcgInternalRefClkSrcSlow);
                       
                       /* wait for Reference clock Status bit to clear */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                           {
                               break; /* jump out early if IREFST clears before loop finishes */
                           }
                           else{}
                       }
                       if (!CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                       {
                           return 0x12; /* check bit is really set and return with error if not set */
                       }
                       
                       /* Wait for clock status bits to show clock source is ext ref clk */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll)
                           {
                               break; /* jump out early if CLKST shows FLL slected before loop finishes */
                           }
                           else{}
                       }
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatFll)
                       {
                           return 0x18; /* check FLL is really selected and return with error if not */
                       }
                       else{}
                       
                       /* Now in FEI mode */
                       return mcgOut;
                   } /* fbi_fei*/
                   
                   /*!
                   * @brief switches from FEI to FBI mode
                   * @param ircFreq - IRC clock frequency
                   * @param ircSelect - 0 if slow irc, 1 if fast irc
                   */
                   int32_t fei_fbi(int32_t ircFreq, uint8_t ircSelect)
                   {
                       uint8_t fcrDivVal;
                       int16_t i;
                       
                       /* Check MCG is in FEI mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) && 	/* check CLKS mux has selcted FLL output */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) /* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 &&(CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel)
#endif
                           ))	/* check PLLS mux has selected FLL */
                       {
                           return 0x1;                                                       /* return error code */
                       }
                       else{}
                       
                       /* Check that the irc frequency matches the selected IRC  */
                       if (!(ircSelect))
                       {
                           if ((ircFreq < 31250) || (ircFreq > 39063)) {return 0x31;}
                       }
                       else
                       {
                           if ((ircFreq < 3000000) || (ircFreq > 5000000)) {return 0x32;} /* Fast IRC freq */
                           else{}
                       }
                       
                       /* Select the desired IRC */
                       if (ircSelect)
                       {
                           CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelFast); /* select fast IRCS */
                       }
                       else
                       {
                           CLOCK_HAL_SetInternalRefClkSelMode(MCG_BASE, kMcgInternalRefClkSelSlow); /* select slow IRCS */
                       }
                       
                       /* Change the CLKS mux to select the IRC as the MCGOUT */
                       CLOCK_HAL_SetClkSrcMode(MCG_BASE, kMcgClkSelInternal); /* select IRC as the MCG clock sourse */
                       
                       /* wait until internal reference switches to requested irc. */
                       if (!(ircSelect))
                       {
                           for (i = 0 ; i < 2000 ; i++)
                           {
                               if (!(CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE)))
                               {
                                   break; /* jump out early if IRCST clears before loop finishes */
                               }
                               else{}
                           }
                           if (CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE))
                           {
                               return 0x13; /* check bit is really clear and return with error if set */
                           }
                           else{}
                       }
                       else
                       {
                           for (i = 0 ; i < 2000 ; i++)
                           {
                               if (CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE))
                               {
                                   break; /* jump out early if IRCST sets before loop finishes */
                               }
                               else{}
                           }
                           if (!(CLOCK_HAL_GetInternalRefClkStatMode(MCG_BASE)))
                           {
                               return 0x14; /* check bit is really set and return with error if not set */
                           }
                           else{}
                       }
                       
                       /* Wait for clock status bits to update */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatInternalRef)
                           {
                               break; /* jump out early if CLKST shows IRC slected before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatInternalRef)
                       {
                           return 0x19; /* check IRC is really selected and return with error if not */
                       }
                       
                       /* Now in FBI mode */
                       if (ircSelect)
                       {
                           fcrDivVal = (1 << CLOCK_HAL_GetFastClkInternalRefDiv(MCG_BASE)); /* calculate the fast IRC divder factor */
                           return (ircFreq / fcrDivVal); /* MCGOUT frequency equals fast IRC frequency divided by 2 */
                       }
                       else
                       {
                           return ircFreq; /* MCGOUT frequency equals slow IRC frequency */
                       }
                   } /* fei_fbi*/
                   
                   /*!
                   * @brief switches from FEI to FEE mode
                   *
                   * This function transitions the MCG from FEI mode to FEE mode. This is
                   * achieved by setting the MCG_C2[LP] bit. There is no status bit to
                   * check so 0 is always returned if the function was called with the MCG
                   * in FBI mode. The MCGCLKOUT frequency does not change
                   *
                   * @param crystalVal - external clock frequency in Hz
                   * @param hgoVal - selects whether low power or high gain mode is selected
                   *                           for the crystal oscillator. This has no meaning if an
                   *                           external clock is used.
                   * @param erefsVal - selects external clock (=kMcgErefClockSelectExt) or
                   *                                    crystal osc (=kMcgExternalRefClkSelOsc)
                   * Return value : MCGCLKOUT frequency (Hz) or error code
                   */
                   int32_t fei_fee(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal)
                   {
                       uint8_t frDivVal;
                       int32_t mcgOut, fllRefFreq, i;
                       
                       /* check if in FEI mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) && 	/* check CLKS mux has selcted FLL output */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) /* check FLL ref is internal ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 &&(CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel)
#endif
                           ))	/* check PLLS mux has selected FLL */
                       {
                           return 0x1;                                                     /* return error code */
                       }
                       
                       /* check external frequency is less than the maximum frequency */
                       if  (crystalVal > 50000000) {return 0x21;}
                       
                       /* check crystal frequency is within spec. if crystal osc is being used */
                       if (erefsVal)
                       {
                           if ((crystalVal < 30000) ||
                               ((crystalVal > 40000) && (crystalVal < 3000000)) ||
                                   (crystalVal > 32000000)) {return 0x22;} /* return error if one of the available crystal options is not available */
                       }
                       
                       /* make sure HGO will never be greater than 1. Could return an error instead if desired. */
                       if (hgoVal > 0)
                       {
                           hgoVal = kMcgHighGainOscSelHigh; /* force hgo_val to 1 if > 0 */
                       }
                       
                       /*
                       * configure the MCG_C2 register
                       * the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
                       * it still needs to be set correctly even if the oscillator is not being used
                       */
                       if (crystalVal <= 40000)
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelLow);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       else if (crystalVal <= 8000000)
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelHigh);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       else
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelVeryHigh);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       
                       /* determine FRDIV based on reference clock frequency */
                       /* since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here. */
                       if (crystalVal <= 1250000) {frDivVal = 0;}
                       else if (crystalVal <= 2500000) {frDivVal = 1;}
                       else if (crystalVal <= 5000000) {frDivVal = 2;}
                       else if (crystalVal <= 10000000) {frDivVal = 3;}
                       else if (crystalVal <= 20000000) {frDivVal = 4;}
                       else {frDivVal = 5;}
                       
                       /* The FLL ref clk divide value depends on FRDIV and the RANGE value */
                       //if (((MCG_C2 & MCG_C2_RANGE0_MASK) >> MCG_C2_RANGE0_SHIFT) > 0)
                       if(CLOCK_HAL_GetRange0Mode(MCG_BASE) != kMcgFreqRangeSelLow)
                       {
                           fllRefFreq = ((crystalVal) / (32 << frDivVal));
                       }
                       else
                       {
                           fllRefFreq = ((crystalVal) / (1 << frDivVal));
                       }
                       
                       /* Check resulting FLL frequency  */
                       mcgOut = fll_freq(fllRefFreq); /* FLL reference frequency calculated from ext ref freq and FRDIV */
                       if (mcgOut < 0x3C) {return mcgOut;} /* If error code returned, return the code to calling function */
                       
                       /*
                       * Select external oscilator and Reference Divider and clear IREFS to start ext osc
                       * If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
                       * CLKS=0, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
                       */
                       /* Set the required CLKS and FRDIV values */
                       CLOCK_HAL_SetClksFrdivInternalRefSelect(MCG_BASE, kMcgClkSelOut, frDivVal, kMcgInternalRefClkSrcExternal);
                       
                       /* if the external oscillator is used need to wait for OSCINIT to set */
                       if (erefsVal)
                       {
                           for (i = 0 ; i < 20000000 ; i++)
                           {
                               if (CLOCK_HAL_GetOscInit0(MCG_BASE))
                               {
                                   break; /* jump out early if OSCINIT sets before loop finishes */
                               }
                           }
                           if (!CLOCK_HAL_GetOscInit0(MCG_BASE))
                           {
                               return 0x23; /* check bit is really set and return with error if not set */
                           }
                       }
                       
                       /* wait for Reference clock Status bit to clear */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (!CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                           {
                               break; /* jump out early if IREFST clears before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                       {
                           return 0x11; /* check bit is really clear and return with error if not set */
                       }
                       
                       return mcgOut; /* MCGOUT frequency equals FLL frequency */
                   } /* fei_fee*/
                   
                   /*!
                   * @brief switches from
                   * @param slowIrcFreqInner - slow IRC frequency
                   */
                   int32_t fee_fei(int32_t slowIrcFreqInner)
                   {
                       int16_t i;
                       uint32_t mcgOut;
                       clock_manager_error_code_t clockFreqError;
                       
                       /* Check MCG is in FEE mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) && 	/* check CLKS mux has selcted FLL */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) != kMcgInternalRefStatInternal)/* check FLL ref is external ref clk */
#if FSL_FEATURE_MCG_HAS_PLL
                                 &&(CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel)
#endif
                           ))	/* check PLLS mux has selected FLL */
                       {
                           return 0x2;                                                     /* return error code */
                       }
                       
                       /* Check IRC frequency is within spec. */
                       if ((slowIrcFreqInner < 31250) || (slowIrcFreqInner > 39063))
                       {
                           return 0x31;
                       }
                       
                       /* Check resulting FLL frequency  */
                       clockFreqError = CLOCK_SYS_GetFreq(kMcgFllClock, &mcgOut);
                       /* If error code returned, return the code to calling function */
                       if(clockFreqError != kClockManagerSuccess)
                       {
                           return mcgOut;
                       }
                       
                       /* Ensure clock monitor is disabled before switching to FEI otherwise a loss of clock will trigger */
                       CLOCK_HAL_SetClkMonitor0Cmd(MCG_BASE, false);
                       
                       /* Change FLL reference clock from external to internal by setting IREFS bit */
                       CLOCK_HAL_SetInternalRefSelMode(MCG_BASE, kMcgInternalRefClkSrcSlow); /* select internal reference */
                       
                       /* wait for Reference clock to switch to internal reference */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                           {
                               break; /* jump out early if IREFST sets before loop finishes */
                           }
                       }
                       if (!CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                       {
                           return 0x12; /* check bit is really set and return with error if not set */
                       }
                       
                       /* Now in FEI mode */
                       return mcgOut;
                   } /* fee_fei*/
                   
                   /*!
                   * @brief configure for MCG auto trimming
                   * @param ircSelect - 0 if slow irc, 1 if fast irc
                   * @param ircFreq - IRC frequency
                   * @param mcgOutFreq - MCG output frequency
                   */
                   uint8_t atc(uint8_t ircSelect, int32_t ircFreq, int32_t mcgOutFreq)
                   {
                       uint8_t mcgModeTemp;
                       uint16_t atcv;
                       uint32_t busClockFreq;
                       uint8_t outdiv1, outdiv2, outdiv3, outdiv4;
                       int32_t  busClkDivVal;
                                             
                       busClkDivVal = mcgOutFreq / 16000000; /* calculate bus clock divider to generate fastest allowed bus clock for autotrim */
                       
                       if (ircSelect > 0) /* force irc to 1 if greater than 0 */
                       {
                           ircSelect = 1;
                       }
                       
                       mcgModeTemp = what_mcg_mode(); /* get present MCG mode */
                       if ((mcgModeTemp != PEE) && (mcgModeTemp != PBE) && (mcgModeTemp != FBE))
                       {
                           return 1; /* return error code if not in PEE, PBE or FBE modes */
                       }
                       
                       /*store present clock divider values */
                       CLOCK_SYS_GetOutDiv(&outdiv1, &outdiv2, &outdiv3, &outdiv4);
                       
                       /* set all bus and flash dividers to same value to ensure clocking restrictions are met */
#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV2
                       CLOCK_SYS_SetOutDiv2(busClkDivVal);
#endif                       
#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV3
                       CLOCK_SYS_SetOutDiv3(busClkDivVal);
#endif                       
                       CLOCK_SYS_SetOutDiv4(busClkDivVal);

                       
                       CLOCK_SYS_GetFreq(kBusClock, &busClockFreq);
                       
                       if ((busClockFreq < 8000000) || (busClockFreq > 16000000))
                       {
                           CLOCK_SYS_SetOutDiv(outdiv1, outdiv2, outdiv3, outdiv4);   /* set SIM_CLKDIV1 back to original value */
                           return 3; /* error, bus clock frequency is not within 8MHz to 16MHz */
                       }
                       
                       if(!ircSelect) /*determine if slow or fast IRC to be trimmed */
                       {
                           if (ircFreq < 31250) /* check frequency is above min spec. */
                           {
                               CLOCK_SYS_SetOutDiv(outdiv1, outdiv2, outdiv3, outdiv4);   /* set SIM_CLKDIV1 back to original value */
                               return 4;
                           }
                           if (ircFreq > 39062) /* check frequency is below max spec. */
                           {
                               CLOCK_SYS_SetOutDiv(outdiv1, outdiv2, outdiv3, outdiv4);   /* set SIM_CLKDIV1 back to original value */
                               return 5;
                           }
                       }
                       else
                       {
                           if (ircFreq < 3000000) /* check frequency is above min spec. */
                           {
                               CLOCK_SYS_SetOutDiv(outdiv1, outdiv2, outdiv3, outdiv4);   /* set SIM_CLKDIV1 back to original value */
                               return 6;
                           }
                           else{}
                           if (ircFreq > 5000000) /* check frequency is below max spec. */
                           {
                               CLOCK_SYS_SetOutDiv(outdiv1, outdiv2, outdiv3, outdiv4);   /* set SIM_CLKDIV1 back to original value */
                               return 7;
                           }
                           else{}
                       } /* if */
                       
                       /* Set up autocal registers, must use floating point calculation */
                       if (ircSelect)
                       {
                           atcv = (uint16_t)(128.0f * (21.0f * (busClockFreq / (float)ircFreq)));
                       }
                       else
                       {
                           atcv = (uint16_t)(21.0f * (busClockFreq / (float)ircFreq));
                       }
                       
                       CLOCK_HAL_SetAutoTrimMachineCmpVal(MCG_BASE, atcv);
                       
                       /* Enable autocal */
                       /*Select IRC to trim and enable trim machine */
                       CLOCK_HAL_SetAutoTrimMachineSelMode(MCG_BASE, (mcg_atm_select_t)ircSelect);
                       CLOCK_HAL_SetAutoTrimMachineCmd(MCG_BASE, true);
                       
                       while (CLOCK_HAL_GetAutoTrimMachineCmd(MCG_BASE))
                       {} /*poll for ATME bit to clear */
                       
                       if (CLOCK_HAL_GetAutoTrimMachineFailStatus(MCG_BASE))	/* check if error flag set */
                       {
                           CLOCK_SYS_SetOutDiv(outdiv1, outdiv2, outdiv3, outdiv4);   /* set SIM_CLKDIV1 back to original value */
                           return 8;
                       }
                       else
                       {
                           if (!ircSelect)
                           {
                               /* Might want to establish a MAX and MIN definition here  */
                               if ((CLOCK_HAL_GetSlowInternalRefClkTrim(MCG_BASE) == 0xFF) || (CLOCK_HAL_GetSlowInternalRefClkTrim(MCG_BASE) == 0))
                               {
                                   CLOCK_SYS_SetOutDiv(outdiv1, outdiv2, outdiv3, outdiv4);   /* set SIM_CLKDIV1 back to original value */
                                   return 9;
                               }
                               else{}
                           }
                           else
                           {}
                       }
                       CLOCK_SYS_SetOutDiv(outdiv1, outdiv2, outdiv3, outdiv4);   /* set SIM_CLKDIV1 back to original value */
                       return 0;
                   }/* atc*/
                   
#if FSL_FEATURE_MCG_HAS_PLL
                   /*!
                   * @brief Option to move from FEI to PEE mode or to just initialize the PLL
                   *
                   * @param crystalVal - external clock frequency in Hz either from a crystal or square
                   *                           wave clock source
                   * @param hgoVal     - selects whether low power or high gain mode is selected
                   *                           for the crystal oscillator. This has no meaning if an
                   *                           external clock is used.
                   * @param erefsVal   - selects external clock (=0) or crystal osc (=1)
                   * @param prdivVal   - value to divide the external clock source by to create the desired
                   *                           PLL reference clock frequency
                   * @param vdivVal    - value to multiply the PLL reference clock frequency by
                   * @param mcgOutSelect  - 0 if the PLL is just to be enabled, non-zero if the PLL is used
                   *                              to provide the MCGOUT clock for the system.
                   *
                   * Return value : PLL frequency (Hz) or error code
                   */
                   int32_t pll_init(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal, int8_t prDivVal, int8_t vDivVal, uint8_t mcgOutSelect)
                   {
                       uint8_t frDivVal;
                       uint8_t prDiv, vDiv;
                       int16_t i;
                       int32_t refFreq;
                       int32_t pllFreq;
                       
                       /* check if in FEI mode */
                       if (!((CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatFll) && 	/* check CLKS mux has selcted FLL output */
                             (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE) == kMcgInternalRefStatInternal) &&	/* check FLL ref is internal ref clk */
                                 (CLOCK_HAL_GetPllStatMode(MCG_BASE) != kMcgPllStatPllClkSel))
                           )	/* check PLLS mux has selected FLL */
                       {
                           return 0x1;                                                     /* return error code */
                       }
                       
                       /* check external frequency is less than the maximum frequency */
                       if  (crystalVal > 50000000) {return 0x21;}
                       
                       /* check crystal frequency is within spec. if crystal osc is being used as PLL ref */
                       if (crystalVal)
                       {
                           if ((crystalVal < 3000000) || (crystalVal > 32000000)) {return 0x22;} /* return 1 if one of the available crystal options is not available */
                       }
                       
                       /* make sure HGO will never be greater than 1. Could return an error instead if desired. */
                       if (hgoVal > 0)
                       {
                           hgoVal = kMcgHighGainOscSelHigh; /* force hgo_val to 1 if > 0 */
                       }
                       
                       /* Check PLL divider settings are within spec. */
                       if ((prDivVal < 1) || (prDivVal > 25)) {return 0x41;}
                       if ((vDivVal < 24) || (vDivVal > 50)) {return 0x42;}
                       
                       /* Check PLL reference clock frequency is within spec. */
                       refFreq = crystalVal / prDivVal;
                       if ((refFreq < 2000000) || (refFreq > 4000000)) {return 0x43;}
                       
                       /* Check PLL output frequency is within spec. */
                       pllFreq = (crystalVal / prDivVal) * vDivVal;
                       if ((pllFreq < 48000000) || (pllFreq > 100000000)) {return 0x45;}
                       
                       /*
                       * configure the MCG_C2 register
                       * the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
                       */
                       /* it still needs to be set correctly even if the oscillator is not being used */
                       
                       
                       if (crystalVal <= 8000000)
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelHigh);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       else
                       {
                           CLOCK_HAL_SetRange0Mode(MCG_BASE, kMcgFreqRangeSelVeryHigh);
                           CLOCK_HAL_SetHighGainOsc0Mode(MCG_BASE, hgoVal);
                           CLOCK_HAL_SetExternalRefSel0Mode(MCG_BASE, erefsVal);
                       }
                       
                       /* determine FRDIV based on reference clock frequency */
                       /* since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here. */
                       if (crystalVal <= 1250000) {frDivVal = 0;}
                       else if (crystalVal <= 2500000) {frDivVal = 1;}
                       else if (crystalVal <= 5000000) {frDivVal = 2;}
                       else if (crystalVal <= 10000000) {frDivVal = 3;}
                       else if (crystalVal <= 20000000) {frDivVal = 4;}
                       else {frDivVal = 5;}
                       
                       /*
                       * Select external oscillator and Reference Divider and clear IREFS to start ext osc
                       * If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
                       * CLKS=2, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
                       */
                       /* Set the required CLKS and FRDIV values */
                       CLOCK_HAL_SetClksFrdivInternalRefSelect(MCG_BASE, kMcgClkSelExternal, frDivVal, kMcgInternalRefClkSrcExternal);
                       
                       
                       
                       /* if the external oscillator is used need to wait for OSCINIT to set */
                       if (erefsVal)
                       {
                           for (i = 0 ; i < 20000 ; i++)
                           {
                               if (CLOCK_HAL_GetOscInit0(MCG_BASE))
                               {
                                   break; /* jump out early if OSCINIT sets before loop finishes */
                               }
                           }
                           if (!(CLOCK_HAL_GetOscInit0(MCG_BASE)))
                           {
                               return 0x23; /* check bit is really set and return with error if not set */
                           }
                       }
                       
                       /* wait for Reference clock Status bit to clear */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (!(CLOCK_HAL_GetInternalRefStatMode(MCG_BASE)))
                           {
                               break; /* jump out early if IREFST clears before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetInternalRefStatMode(MCG_BASE))
                       {
                           return 0x11; /* check bit is really clear and return with error if not set */
                       }
                       
                       /* Wait for clock status bits to show clock source is ext ref clk */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatExternalRef)
                           {
                               break; /* jump out early if CLKST shows EXT CLK slected before loop finishes */
                           }
                       }
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatExternalRef)
                       {
                           return 0x1A; /* check EXT CLK is really selected and return with error if not */
                       }
                       
                       
                       /*
                       * Configure PLL
                       * Configure MCG_C5
                       * If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.
                       */
                       CLOCK_HAL_SetPllExternalRefDiv0(MCG_BASE, prDivVal - 1); /*set PLL ref divider */
                       
                       /*
                       * Configure MCG_C6
                       * The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
                       * The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
                       */
                       /* write new VDIV and enable PLL */
                       CLOCK_HAL_SetVoltCtrlOscDiv0(MCG_BASE, vDivVal - 24);
                       CLOCK_HAL_SetPllSelMode(MCG_BASE, kMcgPllSelPllClkSel);
                       
                       /* wait for PLLST status bit to set */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           if (CLOCK_HAL_GetPllStatMode(MCG_BASE))
                           {
                               break; /* jump out early if PLLST sets before loop finishes */
                           }
                       }
                       if (!(CLOCK_HAL_GetPllStatMode(MCG_BASE)))
                       {
                           return 0x16; /* check bit is really set and return with error if not set */
                       }
                       
                       /* Wait for LOCK bit to set */
                       for (i = 0 ; i < 4000 ; i++)
                       {
                           if (CLOCK_HAL_GetLock0Mode(MCG_BASE))
                           {
                               break; /* jump out early if LOCK sets before loop finishes */
                           }
                       }
                       if (!(CLOCK_HAL_GetLock0Mode(MCG_BASE)))
                       {
                           return 0x44; /* check bit is really set and return with error if not set */
                       }
                       
                       /* Use actual PLL settings to calculate PLL frequency */
                       
                       prDiv = CLOCK_HAL_GetPllExternalRefDiv0(MCG_BASE);
                       vDiv = CLOCK_HAL_GetVoltCtrlOscDiv0(MCG_BASE);
                       
                       /* now in PBE */
                       
                       CLOCK_HAL_SetClkSrcMode(MCG_BASE, kMcgClkSelOut); /* clear CLKS to switch CLKS mux to select PLL as MCG_OUT */
                       
                       /* Wait for clock status bits to update */
                       for (i = 0 ; i < 2000 ; i++)
                       {
                           
                           if (CLOCK_HAL_GetClkStatMode(MCG_BASE) == kMcgClkStatPll)
                           {
                               break; /* jump out early if CLKST = 3 before loop finishes */
                           }
                       }
                       
                       if (CLOCK_HAL_GetClkStatMode(MCG_BASE) != kMcgClkStatPll)
                       {
                           return 0x1B; /* check CLKST is set correctly and return with error if not */
                       }
                       
                       /* Now in PEE */
                       
                       return ((crystalVal / prDiv) * vDiv); /*MCGOUT equals PLL output frequency */
                   } /* pll_init*/
#endif 
				   
