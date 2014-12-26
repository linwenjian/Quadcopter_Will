
#include <stdint.h>

#include "device/fsl_device_registers.h"
#include "fsl_mcg_hal.h"
#include "fsl_clock_manager.h"
#include "low_power_demo.h"

/* MCG Mode defines */
#define BLPI 1  /*!< BLPI MCG mode */
#define FBI  2  /*!< FBI MCG mode */
#define FEI  3  /*!< FEI MCG mode */
#define FEE  4  /*!< FEE MCG mode */
#define FBE  5  /*!< FBE MCG mode */
#define BLPE 6  /*!< BLPE MCG mode */
#define PBE  7  /*!< PBE MCG mode */
#define PEE  8  /*!< PEE MCG mode */

int32_t fll_freq(int32_t fllRef);
uint8_t what_mcg_mode(void);


int32_t fee_fei(int32_t slowIrcFreqInner);
int32_t fei_fbe(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal);
int32_t fei_fbi(int32_t ircFreq, uint8_t ircSelect);
int32_t fbi_fei(int32_t slowIrcFreqInner);
int32_t fbe_pbe(int32_t crystalVal, int8_t prDivVal, int8_t vDivVal);

#if FSL_FEATURE_MCG_HAS_PLL
int32_t pee_to_blpe(void);
int32_t blpe_to_pee(void);
int32_t pbe_pee(int32_t crystalVal);
int32_t pee_pbe(int32_t crystalVal);
int32_t pbe_fbe(int32_t crystalVal);
#endif
int32_t fbe_fbi(int32_t ircFreq, uint8_t ircSelect);
int32_t fbi_fbe(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal);
int32_t fbi_fee(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal);
int32_t fbe_fee(int32_t crystalVal);
int32_t fee_fbe(int32_t crystalVal);
int32_t pbe_blpe(int32_t crystalVal);
int32_t blpe_pbe(int32_t crystalVal, int8_t prDivVal, int8_t vDivVal);
int32_t blpe_fbe(int32_t crystalVal);
int32_t fbi_blpi(int32_t ircFreq, uint8_t ircSelect);
int32_t blpi_fbi(int32_t ircFreq, uint8_t ircSelect);
int32_t fei_fee(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal);
int32_t fee_fbi(int32_t ircFreq, uint8_t ircSelect);
int32_t fbe_blpe(int32_t crystalVal);
int32_t pll_init(int32_t crystalVal, mcg_high_gain_osc_select_t hgoVal, mcg_external_ref_clock_select_t erefsVal, int8_t prDivVal, int8_t vDivVal, uint8_t mcgOutSelect);
