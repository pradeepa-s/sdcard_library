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
 * @file        sl_reg_pbist.h
 * @version     $$file_version$$
 * @brief       PBIST Register Layer Header File
 *
 * @details This file contains:
 *   - Definitions
 *   - Types
 *   .
 *   which are relevant for the System driver.
 */

#ifndef __SL_REG_PBIST_H__
#define __SL_REG_PBIST_H__

#include "sl_sys_common.h"

/* PBIST Register Frame Definition */
/** @struct sl_pbistBase
*   @brief PBIST Base Register Definition
*
*   This structure is used to access the PBIST module registers.
*/
/** @typedef sl_pbistBASE_t
*   @brief PBIST Register Frame Type Definition
*
*   This type is used to access the PBIST Registers.
*/
typedef volatile struct sl_pbistBase
{
    uint32 RAMT;			/* 0x0160: RAM Configuration Register */
    uint32 DLR;			/* 0x0164: Datalogger Register */
    uint32   rsvd1[6U];	/* 0x0168 */
    uint32 PACT;			/* 0x0180: PBIST Activate Register */
    uint32 PBISTID;		/* 0x0184: PBIST ID Register */
    uint32 OVER;			/* 0x0188: Override Register */
    uint32   rsvd2;		/* 0x018C */
    uint32 FSRF0;			/* 0x0190: Fail Status Fail Register 0 */
	uint32 FSRF1;			/* 0x0194: Fail Status Fail Register 1 */
    uint32 FSRC0;			/* 0x0198: Fail Status Count Register 0 */
    uint32 FSRC1;			/* 0x019C: Fail Status Count Register 1 */
    uint32 FSRA0;			/* 0x01A0: Fail Status Address 0 Register */
    uint32 FSRA1;			/* 0x01A4: Fail Status Address 1 Register */
    uint32 FSRDL0;		/* 0x01A8: Fail Status Data Register 0 */
    uint32   rsvd3;		/* 0x01AC */
    uint32 FSRDL1;		/* 0x01B0: Fail Status Data Register 1 */
    uint32   rsvd4[3U];	/* 0x01B4 */
    uint32 ROM;			/* 0x01C0: ROM Mask Register */
    uint32 ALGO;			/* 0x01C4: Algorithm Mask Register */
    uint32 RINFOL;		/* 0x01C8: RAM Info Mask Lower Register */
    uint32 RINFOU;		/* 0x01CC: RAM Info Mask Upper Register */
} sl_pbistBASE_t;

#define sl_pbistREG   ((sl_pbistBASE_t *)0xFFFFE560U)

#endif
