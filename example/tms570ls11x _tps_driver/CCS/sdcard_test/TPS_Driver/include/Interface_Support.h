/**********************************************************************************************************************
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
 *         @File       :  Interface_Support.c
 *         @version    :  $file_version$
 *         Component   :  application
 *         Module      :  application
 *         Generator   :  -
 *
 *         @brief      : Provides Interfacing API for TPS driver to communicate to TPS device.These API's are used by
 *                       the Tps_Interface.c
 *         @detail     :  todo
 *
 *---------------------------------------------------------------------------------------------------------------------
 * Author:  Manoj R
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
 * Version        Date         Author               Change ID        Description
 *---------------------------------------------------------------------------------------------------------------------
 * 0.0.1          20Dec2012    Manoj R              00000000000      Initial version created
 ********************************************************************************************************************/
#ifndef __INTERFACE_SUPPORT__
#define __INTERFACE_SUPPORT__
/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "Board_Config.h"
#ifndef SPI_COMPATIBILITY_MODE
#include "mibspi.h"
#else
#include "spi.h"
#endif
/*********************************************************
 Defines / data types / structs / unions /enums
 **********************************************************/

/*********************************************************
 Function Prototypes
 **********************************************************/
boolean SendandRecieveDataTPS(const uint16* const u16TransmitData,const uint32 u32TransmitSize,volatile uint16* u16RecieveData,uint32 *u32RecieveSize);
boolean SendandRecieveDataTPS_ERR(const uint16* const u16TransmitData,const uint32 u32TransmitSize,volatile uint16* u16RecieveData,uint32 *u32RecieveSize);

boolean TPSDiagosticPinSampledData(uint16* u16Data);
boolean TPSDiagosticPinSampledData_ERR(uint16* u16Data);

boolean SendDebugTextTPS(const uint8 *u8textbuf,uint32 u32textlength);
boolean SendDebugTextTPS_ERR(const uint8 *u8textbuf,uint32 u32textlength);

#endif
