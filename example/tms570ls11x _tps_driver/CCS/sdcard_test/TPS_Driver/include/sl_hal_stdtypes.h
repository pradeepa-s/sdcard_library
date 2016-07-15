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
 * @file        sl_hal_stdtypes.h
 * @version     $$file_version$$
 * @brief       Standard types header File
 *
 * @details This file contains:
 *   - Type and Global definitions which are relevant for all drivers.
 */

#ifndef __SL_HAL_STDTYPES_H__
#define __SL_HAL_STDTYPES_H__

#include <stdint.h>
#include <stdbool.h>



/************************************************************/
/* Type Definitions                                         */
/************************************************************/
#ifndef _UINT64_DECLARED
typedef uint64_t uint64;
#define	_UINT64_DECLARED
#endif

#ifndef _UINT32_DECLARED
typedef uint32_t uint32;
#define	_UINT32_DECLARED
#endif

#ifndef _UINT16_DECLARED
typedef uint16_t uint16;
#define	_UINT16_DECLARED
#endif

#ifndef _UINT8_DECLARED
typedef uint8_t uint8;
#define	_UINT8_DECLARED
#endif

#ifndef _BOOLEAN_DECLARED
typedef _Bool boolean;
#define	_BOOLEAN_DECLARED
#endif

#ifndef _SINT64_DECLARED
typedef int64_t sint64;
#define	_SINT64_DECLARED
#endif

#ifndef _SINT32_DECLARED
typedef int32_t sint32;
#define	_SINT32_DECLARED
#endif

#ifndef _SINT16_DECLARED
typedef int16_t sint16;
#define	_SINT16_DECLARED
#endif

#ifndef _SINT8_DECLARED
typedef int8_t sint8;
#define	_SINT8_DECLARED
#endif

#ifndef _FLOAT32_DECLARED
typedef float float32;
#define	_FLOAT32_DECLARED
#endif

#ifndef _FLOAT64_DECLARED
typedef double float64;
#define	_FLOAT64_DECLARED
#endif

/************************************************************/
/* Global Definitions                                       */
/************************************************************/
/** @def NULL
*   @brief NULL definition
*/
#ifndef NULL
    #define NULL ((void *) 0U)
#endif

/** @def TRUE
*   @brief definition for TRUE
*/
#ifndef TRUE
    #define TRUE (true)
#endif

/** @def FALSE
*   @brief BOOLEAN definition for FALSE
*/
#ifndef FALSE
    #define FALSE (false)
#endif




#endif /* __SL_HAL_STDTYPES_H__ */
