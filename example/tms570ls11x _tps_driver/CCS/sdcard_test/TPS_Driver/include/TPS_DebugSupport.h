/**
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION
 *
 *                 Property of Texas Instruments, Unauthorized reproduction and/or distribution
 *                 is strictly prohibited.  This product  is  protected  under  copyright  law
 *                 and  trade  secret law as an  unpublished work.
 *                 (C) Copyright Texas Instruments.  All rights reserved.
 *
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *  ------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         @file       Tps_DebugSupport.h
 *         @version    2.2.0
 *         @component  TPS
 *         @module     TPS Driver
 *         @generator  No Configuration
 *
 *         @brief      This files contains the Debug Support API prototypes.
 *         @details    The Tps_DebugSupport.h file provides the debug support API
 *                     prototypes.These API's should help the application\n
 *                     debug the end application /driver.
 *
 *---------------------------------------------------------------------------------------------------------------------
 * @author  Manoj
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
 |Version       | Date         | Author           |    Change ID       | Description                 |
 |------------: |:------------:|:----------------:|:------------------:|:----------------------------|
 |2.2.0         | 20Dec2012    | Manoj R          |    00000000000     | Initial version             |
 */
#ifndef __TPS_DEBUGSUPPORT__
#define __TPS_DEBUGSUPPORT__
/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "TPS_Types.h"
#include "TPS_Config.h"
#include <stdio.h>
#include <string.h>
/*********************************************************
 Defines / data types / structs / unions /enums
 **********************************************************/
/*********************************************************
 Function Prototypes
 **********************************************************/
/**
 * @brief  API to initialize the debug support in the TPS driver.
 *
 *
 * @description The API is used to set up the TPS driver for the debug support.
 *
 * @image html  TPS_DriverInit.jpg
 * @image rtf   TPS_DriverInit.jpg
 * @image latex todo
 *
 * @param [in]   SendTPSDebugText  pointer to the Application supported API\n
 *               which receives the debug text from the TPS driver.
 *
 * Example Usage:
 * @code
 * todo
 * @endcode
 *
 * @entrymode   any,The debu support interface such as SCI has to be\n
 * initialized before the debug init is called.
 * @exitmode    any.
 *
 * @seealso     todo
 *
 *
 * @destag      todo
 * @reqtag      todo
 */
void TPS_DebugInit(SendDebugText SendTPSDebugText);
/**
 * @brief  The API is used flag the errors/debug messages for debug purposes.
 *
 *
 * @description The API is used flag the errors/debug messages for debug purposes.
 *
 * @image html  TPS_DriverInit.jpg
 * @image rtf   TPS_DriverInit.jpg
 * @image latex todo
 *
 * @param [in]   severity  Severity ERROR or DEBUG of the debug message.
 * @param [in]   u8buftext NULL terminated text buffer containing user\n
 *               information.
 * @param [in]   u32value  Value printed along with the debug text.This\n
 *               arguement is ignored if severity is INFO
 * Example Usage:
 * @code
 * todo
 * @endcode
 *
 * @entrymode   any,The debug support interface such as SCI has to be\n
 * initialized before the debug init is called.
 * @exitmode    any.
 *
 * @seealso     todo
 *
 *
 * @destag      todo
 * @reqtag      todo
 */
boolean TPS_SendDebugText(TPS_DebugSeverity severity, const uint8* const u8buftext,
        uint32 u32value);
#endif

