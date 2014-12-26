/******************************************************************************
*
* (c) Copyright 2013, Freescale
*
******************************************************************************/
/*!
*
* @file       GFLIB_Lut2D.h
*
* @version    1.0.1.0
*
* @date       Oct-9-2013
*
* @brief      Header file for GFLIB_Lut2D function
*
******************************************************************************/
#ifndef GFLIB_LUT2D_H
#define GFLIB_LUT2D_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
*
* @section GFLIB_Lut2D_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires use of disallowed macro
* definition.
*
* @section GFLIB_Lut2D_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires the function-like macro
* definition.
*
* @section GFLIB_Lut2D_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires use of unparenthesized
* macro parameters.
*
* @section GFLIB_Lut2D_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators.
*/
#include "SWLIBS_Typedefs.h"
#include "SWLIBS_Defines.h"
#include "SWLIBS_MacroDisp.h"
#include "mlib.h"

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
  * macro.
  * @violates @ref GFLIB_Lut2D_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
  * @violates @ref GFLIB_Lut2D_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
  * parameter in definition of macro.
  */
  #define GFLIB_Lut2D(...)     macro_dispatcher(GFLIB_Lut2D, __VA_ARGS__)(__VA_ARGS__)     /*!< This function implements the two-dimensional look-up table. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
    * macro.
    */
    #define GFLIB_LUT2D_T                             GFLIB_LUT2D_T_F32                             /*!< Definition of GFLIB_LUT2D_T as alias for GFLIB_LUT2D_T_F32 datatype in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
    * macro.
    */
    #define GFLIB_LUT2D_DEFAULT                       GFLIB_LUT2D_DEFAULT_F32                       /*!< Definition of GFLIB_LUT2D_DEFAULT as alias for GFLIB_LUT2D_DEFAULT_F32 default value in case the 32-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
    * macro.
    * @violates @ref GFLIB_Lut2D_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
    * @violates @ref GFLIB_Lut2D_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
    * parameter in definition of macro.
    */
    /** @remarks Implements DGFLIB00156 */
    #define GFLIB_Lut2D_Dsptchr_3(In1,In2,pParam)     GFLIB_Lut2D_Dsptchr_4(In1,In2,pParam,F32)     /*!< Function dispatcher for GFLIB_Lut2D_Dsptchr_3, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
    * macro.
    */
    #define GFLIB_LUT2D_T                             GFLIB_LUT2D_T_F16                             /*!< Definition of GFLIB_LUT2D_T as alias for GFLIB_LUT2D_T_F16 datatype in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
    * macro.
    */
    #define GFLIB_LUT2D_DEFAULT                       GFLIB_LUT2D_DEFAULT_F16                       /*!< Definition of GFLIB_LUT2D_DEFAULT as alias for GFLIB_LUT2D_DEFAULT_F16 default value in case the 16-bit fractional implementation is selected. */
    /*
    * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
    * macro.
    * @violates @ref GFLIB_Lut2D_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
    * @violates @ref GFLIB_Lut2D_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
    * parameter in definition of macro.
    */
    /** @remarks Implements DGFLIB00156 */
    #define GFLIB_Lut2D_Dsptchr_3(In1,In2,pParam)     GFLIB_Lut2D_Dsptchr_4(In1,In2,pParam,F16)     /*!< Function dispatcher for GFLIB_Lut2D_Dsptchr_3, do not modify!!! */
  #endif

  /*
  * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
  * macro.
  * @violates @ref GFLIB_Lut2D_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
  * @violates @ref GFLIB_Lut2D_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
  * parameter in definition of macro.
  */
  #define GFLIB_Lut2D_Dsptchr_4(In1,In2,pParam,Impl)    GFLIB_Lut2D_Dsptchr_(In1,In2,pParam,Impl)   /*!< Function dispatcher for GFLIB_Lut2D_Dsptchr_4, do not modify!!! */

  /*
  * @violates @ref GFLIB_Lut2D_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
  * macro.
  * @violates @ref GFLIB_Lut2D_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
  * @violates @ref GFLIB_Lut2D_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
  * parameter in definition of macro.
  * @violates @ref GFLIB_Lut2D_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro.
  */
  /** @remarks Implements DGFLIB00158 */
  #define GFLIB_Lut2D_Dsptchr_(In1,In2,pParam,Impl)     GFLIB_Lut2D_##Impl(In1,In2,pParam)          /*!< Function dispatcher for GFLIB_Lut2D_Dsptchr_, do not modify!!! */
#endif /* _MATLAB_BAM_CREATE */

/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/




/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON)
  #define GFLIB_LUT2D_DEFAULT_F32 {0,0,0}     /*!< Default value for GFLIB_LUT2D_T_F32. */

  /************************************************************************//*!
  @struct GFLIB_LUT2D_T_F32 "\GFLIB_Lut2D.h"

  @brief  Structure containing 2D look-up table parameters.
  *//*************************************************************************/
  /** @remarks Implements DGFLIB00163, DGFLIB00159 */
  typedef struct{
    UWord32 u32ShamOffset1; /*!< Shift amount for extracting the fractional offset within an interpolated interval. */
    UWord32 u32ShamOffset2; /*!< Shift amount for extracting the fractional offset within an interpolated interval. */
    const Frac32 *pf32Table; /*!< Table holding ordinate values of interpolating intervals. */
  }GFLIB_LUT2D_T_F32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern Frac32 GFLIB_Lut2D_F32(Frac32 f32In1,Frac32 f32In2,const GFLIB_LUT2D_T_F32 * const pParam);
#endif /* SWLIBS_SUPPORT_F32 == SWLIBS_STD_ON */




/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
#if (SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON)
  #define GFLIB_LUT2D_DEFAULT_F16 {0,0,0}     /*!< Default value for GFLIB_LUT2D_T_F16. */

  /************************************************************************//*!
  @struct GFLIB_LUT2D_T_F16 "\GFLIB_Lut2D.h"

  @brief  Structure containing 2D look-up table parameters.
  *//*************************************************************************/
  /** @remarks Implements DGFLIB00163, DGFLIB00160 */
  typedef struct{
    UWord16 u16ShamOffset1; /*!< Shift amount for extracting the fractional offset within an interpolated interval. */
    UWord16 u16ShamOffset2; /*!< Shift amount for extracting the fractional offset within an interpolated interval. */
    const Frac16 *pf16Table; /*!< Table holding ordinate values of interpolating intervals. */
  }GFLIB_LUT2D_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
  extern Frac16 GFLIB_Lut2D_F16(Frac16 f16In1,Frac16 f16In2,const GFLIB_LUT2D_T_F16 * const pParam);
#endif /* SWLIBS_SUPPORT_F16 == SWLIBS_STD_ON */




#ifdef __cplusplus
}
#endif

#endif /* GFLIB_LUT2D_H */
