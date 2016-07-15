/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                                                                          |**
**|         Copyright (c) 2012 Texas Instruments Incorporated                |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/
/**
 * @file        sl_sys_common.h
 * @version     $$file_version$$
 * @brief       Common Header File
 *
 * @details This file contains:
 *   - General Definitions
 *   .
 *   which are relevant for all drivers.
 */

#ifndef __SL_SYS_COMMON_H__
#define __SL_SYS_COMMON_H__

#include "sl_hal_stdtypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************/
/* Type Definitions                                         */
/************************************************************/

#ifndef _TBOOLEAN_DECLARED
typedef boolean tBoolean;
#define _TBOOLEAN_DECLARED
#endif

/** @enum sl_loopBackType
*   @brief Loopback type definition
*/
/** @typedef sl_loopBackType_t
*   @brief Loopback type Type Definition
*
*   This type is used to select the module Loopback type Digital or SL_Analog loopback.
*/
typedef enum sl_loopBackType 
{
    SL_Digital_Lbk = 0U, 
    SL_Analog_Lbk = 1U
}sl_loopBackType_t;

/** @enum sl_config_value_type
*   @brief config type definition
*/
/** @typedef sl_config_value_type_t
*   @brief config type Type Definition
*
*   This type is used to specify the Initial and Current value.
*/
typedef enum sl_config_value_type
{
	Sl_InitialValue,
	Sl_CurrentValue
}sl_config_value_type_t;


/********************************************************************************/
/* The ASSERT macro, which does the actual assertion checking.  Typically, this */
/* will be for procedure arguments.                                             */
/********************************************************************************/
#ifdef DEBUG
#define ASSERT(expr) {                                      \
                         if(!(expr))                        \
                         {                                  \
                             __error__(__FILE__, __LINE__); \
                         }                                  \
                     }
#else
#define ASSERT(expr)
#endif

/**@}*/
#ifdef __cplusplus
}
#endif


#endif
