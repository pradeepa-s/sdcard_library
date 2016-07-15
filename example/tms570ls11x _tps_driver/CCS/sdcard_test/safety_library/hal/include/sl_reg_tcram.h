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
 * @file        sl_reg_tcram.h
 * @version     $$file_version$$
 * @brief       TCRAM Register Layer Header File
 *
 * @details This file contains:
 *   - Definitions
 *   - Types
 *   .
 *   which are relevant for the System driver.
 */

#ifndef __SL_REG_TCRAM_H__
#define __SL_REG_TCRAM_H__


#include "sl_sys_common.h"

/* Tcram Register Frame Definition */
/** @struct sl_tcramBase
*   @brief TCRAM Wrapper Register Frame Definition
*
*   This type is used to access the TCRAM Wrapper Registers.
*/
/** @typedef sl_tcramBASE_t
*   @brief TCRAM Wrapper Register Frame Type Definition
*
*   This type is used to access the TCRAM Wrapper Registers.
*/

typedef volatile struct sl_tcramBase
{
    uint32 RAMCTRL;		    /* 0x0000 */
    uint32 RAMTHRESHOLD;      /* 0x0004 */
    uint32 RAMOCCUR;			/* 0x0008 */
    uint32 RAMINTCTRL;		/* 0x000C */
    uint32 RAMERRSTATUS;		/* 0x0010 */
    uint32 RAMSERRADDR;		/* 0x0014 */
    uint32   rsvd1;			/* 0x0018 */
    uint32 RAMUERRADDR;		/* 0x001C */
    uint32   rsvd2[4U];		/* 0x0020 */
    uint32 RAMTEST;			/* 0x0030 */
    uint32   rsvd3;			/* 0x0034 */
    uint32 RAMADDRDECVECT;	/* 0x0038 */
    uint32 RAMPERADDR;        /* 0x003C */
} sl_tcramBASE_t;


#define sl_tcram1REG ((sl_tcramBASE_t *)(0xFFFFF800U))
#define sl_tcram2REG ((sl_tcramBASE_t *)(0xFFFFF900U))


#endif
