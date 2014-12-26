/******************************************************************************
*
* (c) Copyright 2013, Freescale
*
******************************************************************************/
/*!
*
* @file       MLIB_MulSat.h
*
* @version    1.0.1.0
*
* @date       Oct-9-2013
*
* @brief      Header file for MLIB_MulSat function
*
******************************************************************************/
#ifndef MLIB_MULSAT_H
#define MLIB_MULSAT_H
/*!
@if MLIB_GROUP
    @addtogroup MLIB_GROUP
@else
    @defgroup MLIB_GROUP   MLIB
@endif
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
* @page misra_violations MISRA-C:2004 violations
*
* @section MLIB_MulSat_h_REF_1
* Violates MISRA 2004 Required Rule 19.4, Disallowed definition for macro.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires use of disallowed macro
* definition.
*
* @section MLIB_MulSat_h_REF_2
* Violates MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires the function-like macro
* definition.
*
* @section MLIB_MulSat_h_REF_3
* Violates MISRA 2004 Required Rule 19.10, Unparenthesized macro parameter in definition of macro.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires use of unparenthesized
* macro parameters.
*
* @section MLIB_MulSat_h_REF_4
* Violates MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro.
* To allow the user utilize the benefits of using all three supported implementation of each
* function in user application, the macro dispatcher inevitably requires use of '#/##' operators.
*
* @section MLIB_MulSat_h_REF_5
* Violates MISRA 2004 Required Rule 8.5, Object/function definition in header file.
* Allowing the inline functions significantly increase the speed of the library thus the lowest
* layer (MLIB) is implemented as inline. This approach removes the overhead caused by standard
* function calling.
*/
#include "SWLIBS_Defines.h"
#ifndef AMMCLIB_TESTING_ENV
  #include "MLIB_Mul.h"
#else
  /* Following include serves for Freescale internal testing purposes only.
  *  This header is not part of the release. */
  #include "CCOV_MLIB_Mul.h"
#endif

/****************************************************************************
* Defines and macros            (scope: module-local)
****************************************************************************/
#ifndef  _MATLAB_BAM_CREATE
  /*
  * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
  * macro.
  * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
  * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
  * parameter in definition of macro.
  */
  #define MLIB_MulSat(...)     macro_dispatcher(MLIB_MulSat, __VA_ARGS__)(__VA_ARGS__)     /*!< This function multiply two input parameters and saturate if necessary. */

  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F32)
    /*
    * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
    * macro.
    * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
    * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
    * parameter in definition of macro.
    */
    /** @remarks Implements DMLIB00101 */
    #define MLIB_MulSat_Dsptchr_2(In1,In2)     MLIB_MulSat_Dsptchr_3(In1,In2,F32)     /*!< Function dispatcher for MLIB_MulSat_Dsptchr_2, do not modify!!! */
  #endif
  #if (SWLIBS_DEFAULT_IMPLEMENTATION == SWLIBS_DEFAULT_IMPLEMENTATION_F16)
    /*
    * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
    * macro.
    * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
    * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
    * parameter in definition of macro.
    */
    /** @remarks Implements DMLIB00101 */
    #define MLIB_MulSat_Dsptchr_2(In1,In2)     MLIB_MulSat_Dsptchr_3(In1,In2,F16)     /*!< Function dispatcher for MLIB_MulSat_Dsptchr_2, do not modify!!! */
  #endif

  /*
  * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
  * macro.
  * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
  * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
  * parameter in definition of macro.
  */
  #define MLIB_MulSat_Dsptchr_3(In1,In2,Impl)    MLIB_MulSat_Dsptchr_(In1,In2,Impl)     /*!< Function dispatcher for MLIB_MulSat_Dsptchr_3, do not modify!!! */

  /*
  * @violates @ref MLIB_MulSat_h_REF_1 MISRA 2004 Required Rule 19.4, Disallowed definition for
  * macro.
  * @violates @ref MLIB_MulSat_h_REF_2 MISRA 2004 Advisory Rule 19.7, Function-like macro defined.
  * @violates @ref MLIB_MulSat_h_REF_3 MISRA 2004 Required Rule 19.10, Unparenthesized macro
  * parameter in definition of macro.
  * @violates @ref MLIB_MulSat_h_REF_4 MISRA 2004 Advisory Rule 19.13, #/##' operator used in macro.
  */
  /** @remarks Implements DMLIB00103 */
  #define MLIB_MulSat_Dsptchr_(In1,In2,Impl)     MLIB_MulSat_##Impl(In1,In2)            /*!< Function dispatcher for MLIB_MulSat_Dsptchr_, do not modify!!! */
#endif
/****************************************************************************
* Typedefs and structures       (scope: module-local)
****************************************************************************/

/****************************************************************************
* Exported function prototypes
****************************************************************************/

/****************************************************************************
* static inline functions
****************************************************************************/





/****************************************************************************
* Implementation variant: 32-bit fractional
****************************************************************************/
/***************************************************************************/
/*!
@ingroup    MLIB_GROUP

@brief        This function multiplies two input parameters and saturate if necessary.

@param[in]    f32In1     Operand is a 32-bit number normalized between [-1,1).

@param[in]    f32In2     Operand is a 32-bit number normalized between [-1,1).

@return       Fractional multiplication of the input arguments.

@details      Fractional multiplication of two fractional 32-bit values. The input values as well as output value
              are considered as 32-bit fractional data type. The output saturation is implemented in this function,
              thus in case the multiplication of input values is outside the (-1, 1) interval, the output value
              is limited to the boundary value.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_MulSat_F32
              \image rtf mulsatEq1_f32.math "MLIB_MulSat_Eq1"

@note         Overflow is detected. The functions saturates the return value if it cannot fit into the return type.

@par Re-entrancy
              The function is re-entrant.

@par Code Example
\code
#include "mlib.h"

Frac32 f32In1;
Frac32 f32In2;
Frac32 f32Out;

void main(void)
{
  // first input = 0.8
  f32In1  = FRAC32(0.8);

  // second input = 0.75
  f32In2  = FRAC32(0.75);

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat_F32(f32In1,f32In2);

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat(f32In1,f32In2,F32);

  // ##############################################################
  // Available only if 32-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat(f32In1,f32In2);
}
\endcode
****************************************************************************/
/** @remarks Implements DMLIB00102, DMLIB00095, DMLIB00097, DMLIB00100, DMLIB00105 */
static inline Frac32 MLIB_MulSat_F32(register Frac32 f32In1,register Frac32 f32In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register Word32 w32Temp;

  w32Temp = F32TOINT32(MLIB_Mul_F32(f32In1, f32In2));
  w32Temp = (w32Temp == INT32_MIN) ? INT32_MAX : w32Temp;
  /** @remarks Implements DMLIB00098 */
  return(INT32TOF32(w32Temp));
}





/***************************************************************************/
/*!
@ingroup    MLIB_GROUP

@brief        This function multiplies two input parameters and saturate if necessary.

@param[in]    f16In1     Operand is a 16-bit number normalized between [-1,1).

@param[in]    f16In2     Operand is a 16-bit number normalized between [-1,1).

@return       Fractional multiplication of the input arguments.

@details      Fractional multiplication of two fractional 16-bit values. The input values are considered as 16-bit
              fractional data type and the output value is considered as 32-bit fractional data type. The output
              saturation is implemented in this function, thus in case the multiplication of input values is outside the
              (-1, 1) interval, the output value is limited to the boundary value.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_MulSat_F32F16F16
              \image rtf mulsatEq1_f32f16f16.math "MLIB_MulSat_Eq1"

@note         Overflow is detected. The functions saturates the return value if it cannot fit into the return type.

@par Re-entrancy
              The function is re-entrant.

@par Code Example
\code
#include "mlib.h"

Frac16 f16In1;
Frac16 f16In2;
Frac32 f32Out;

void main(void)
{
  // first input = 0.8
  f16In1  = FRAC16(0.8);

  // second input = 0.75
  f16In2  = FRAC16(0.75);

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat_F32F16F16(f16In1,f16In2);

  // output should be 0x4ccccccc = FRAC32(0.6)
  f32Out = MLIB_MulSat(f32In1,f32In2,F32F16f16);
}
\endcode
****************************************************************************/
/** @remarks Implements DMLIB00102, DMLIB00095, DMLIB00099, DMLIB00100, DMLIB00105 */
static inline Frac32 MLIB_MulSat_F32F16F16(register Frac16 f16In1,register Frac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register Word32 w32Temp;

  w32Temp = F32TOINT32(MLIB_Mul_F32F16F16(f16In1, f16In2));
  w32Temp = (w32Temp == INT32_MIN) ? INT32_MAX : w32Temp;
  /** @remarks Implements DMLIB00098 */
  return(INT32TOF32(w32Temp));
}





/****************************************************************************
* Implementation variant: 16-bit fractional
****************************************************************************/
/***************************************************************************/
/*!
@ingroup    MLIB_GROUP

@brief        This function multiplies two input parameters and saturate if necessary.

@param[in]    f16In1     Operand is a 16-bit number normalized between [-1,1).

@param[in]    f16In2     Operand is a 16-bit number normalized between [-1,1).

@return       Fractional multiplication of the input arguments.

@details      Fractional multiplication of two fractional 16-bit values. The input values as well as output value
              are considered as 16-bit fractional data type. The output saturation is implemented in this function,
              thus in case the multiplication of input values is outside the (-1, 1) interval, the output value
              is limited to the boundary value.

              \par

              The output of the function is defined by the following simple equation:
              \anchor eq1_MulSat_F16
              \image rtf mulsatEq1_f16.math "MLIB_MulSat_Eq1"

@note         Overflow is detected. The functions saturates the return value if it cannot fit into the return type.

@par Re-entrancy
              The function is re-entrant.

@par Code Example
\code
#include "mlib.h"

Frac16 f16In1;
Frac16 f16In2;
Frac16 f16Out;

void main(void)
{
  // first input = 0.8
  f16In1  = FRAC16(0.8);

  // second input = 0.75
  f16In2  = FRAC16(0.75);

  // output should be 0x4ccc = FRAC16(0.6)
  f16Out = MLIB_MulSat_F16(f16In1,f16In2);

  // output should be 0x4ccc = FRAC16(0.6)
  f16Out = MLIB_MulSat(f16In1,f16In2,F16);

  // ##############################################################
  // Available only if 16-bit fractional implementation selected
  // as default
  // ##############################################################

  // output should be 0x4ccc = FRAC32(0.6)
  f16Out = MLIB_MulSat(f16In1,f16In2);
}
\endcode
****************************************************************************/
/** @remarks Implements DMLIB00102, DMLIB00095, DMLIB00096, DMLIB00100, DMLIB00106 */
static inline Frac16 MLIB_MulSat_F16(register Frac16 f16In1,register Frac16 f16In2)
/*
* @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
* header file.
*/
{
  /*
  * @violates @ref MLIB_MulSat_h_REF_5 MISRA 2004 Required Rule 8.5, Object/function definition in
  * header file.
  */
  register Word16 w16Temp;

  w16Temp = F16TOINT16(MLIB_Mul_F16(f16In1, f16In2));
  w16Temp = (w16Temp == INT16_MIN) ? INT16_MAX : w16Temp;
  /** @remarks Implements DMLIB00098 */
  return(INT16TOF16(w16Temp));
}


#ifdef __cplusplus
}
#endif

#endif /* MLIB_MULSAT_H */
