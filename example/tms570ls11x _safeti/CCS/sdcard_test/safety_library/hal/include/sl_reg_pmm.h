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
 * @file        sl_reg_pmm.h
 * @version     $$file_version$$
 * @brief       PMM Register Layer Header File
 *
 * @details This file contains:
 *   - Definitions
 *   - Types
 *   .
 *   which are relevant for the PMM driver.
 */

#ifndef __SL_REG_PMM_H__
#define __SL_REG_PMM_H__

#include "sl_sys_common.h"

/* Pmm Register Frame Definition */
/** @struct sl_pmmBase
*   @brief Pmm Register Frame Definition
*
*   This type is used to access the Pmm Registers.
*/
/** @typedef sl_pmmBase_t
*   @brief Pmm Register Frame Type Definition
*
*   This type is used to access the Pmm Registers.
*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
typedef volatile struct sl_pmmBase
{
	uint32 LOGICPDPWRCTRL0;		/* 0x0000: Logic Power Domain Control Register 0 */	
    uint32   rsvd1[3U];			/* 0x0004 */
	uint32 MEMPDPWRCTRL0;			/* 0x0010: Memory Power Domain Control Register 0 */
    uint32   rsvd2[3U];			/* 0x0014 */
    uint32 PDCLKDISREG;			/* 0x0020: Power Domain Clock Disable Register */
    uint32 PDCLKDISSETREG;		/* 0x0024: Power Domain Clock Disable Set Register */
    uint32 PDCLKDISCLRREG;		/* 0x0028: Power Domain Clock Disable Clear Register */
    uint32   rsvd3[5U];			/* 0x002C */
    uint32 LOGICPDPWRSTAT[4U];   	/* 0x0040, 0x0044, 0x0048, 0x004C: Logic Power Domain PD2 Power Status Register 
										- 0: PD2
										- 1: PD3
										- 2: PD4
										- 3: PD5 */
    uint32   rsvd4[12U];			/* 0x0050 */
    uint32 MEMPDPWRSTAT[3U];		/* 0x0080, 0x0084, 0x0088: Memory Power Domain Power Status Register 
										- 0: RAM_PD1
										- 1: RAM_PD2
										- 2: RAM_PD3 */
    uint32   rsvd5[5U];			/* 0x008C */
    uint32 GLOBALCTRL1;			/* 0x00A0: Global Control Register 1 */
    uint32   rsvd6;					/* 0x00A4 */
    uint32 GLOBALSTAT;			/* 0x00A8: Global Status Register */
    uint32 PRCKEYREG;				/* 0x00AC: PSCON Diagnostic Compare Key Register */
    uint32 LPDDCSTAT1;			/* 0x00B0: LogicPD PSCON Diagnostic Compare Status Register 1 */
    uint32 LPDDCSTAT2;			/* 0x00B4: LogicPD PSCON Diagnostic Compare Status Register 2 */
    uint32 MPDDCSTAT1;			/* 0x00B8: Memory PD PSCON Diagnostic Compare Status Register 1 */
    uint32 MPDDCSTAT2;			/* 0x00BC: Memory PD PSCON Diagnostic Compare Status Register 2 */
    uint32 ISODIAGSTAT;			/* 0x00C0: Isolation Diagnostic Status Register */
}sl_pmmBase_t;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
typedef volatile struct sl_pmmBase
{
    uint32 LOGICPDPWRCTRL0;      /**< 0x0000: Logic Power Domain Control Register 0 */
    uint32 LOGICPDPWRCTRL1;      /**< 0x0000: Logic Power Domain Control Register 1 */	
    uint32 rsvd1[5U];            /**< 0x0008 - 0x001C: Reserved*/
    uint32 PDCLKDIS;             /**< 0x0020: Power Domain Clock Disable Register */
    uint32 PDCLKDISSET;          /**< 0x0024: Power Domain Clock Disable Set Register */
    uint32 PDCLKDISCLR;          /**< 0x0028: Power Domain Clock Disable Clear Register */
    uint32 rsvd2[5U];            /**< 0x002C - 0x003C: Reserved */
    uint32 LOGICPDPWRSTAT[5U];   /**< 0x0040 - 0x0050: Logic Power Domain Power Status Register 
                                        - 0: PD2
                                        - 1: PD3
                                        - 2: PD4
                                        - 3: PD5 
                                        - 4: PD6 */
    uint32 rsvd3[20U];           /**< 0x0050 - 0x009F: Reserved*/
    uint32 GLOBALCTRL1;          /**< 0x00A0: Global Control Register 1 */
    uint32 rsvd4;                /**< 0x00A4: Reserved */
    uint32 GLOBALSTAT;           /**< 0x00A8: Global Status Register */
    uint32 PRCKEYREG;            /**< 0x00AC: PSCON Diagnostic Compare Key Register */
    uint32 LPDDCSTAT1;           /**< 0x00B0: LogicPD PSCON Diagnostic Compare Status Register 1 */
    uint32 LPDDCSTAT2;           /**< 0x00B4: LogicPD PSCON Diagnostic Compare Status Register 2 */
    uint32 rsvd5;                /**< 0x00B8: Reserved */
    uint32 rsvd6;                /**< 0x00BC: Reserved */	
    uint32 ISODIAGSTAT;          /**< 0x00C0: Isolation Diagnostic Status Register */
}sl_pmmBase_t;
#endif

/** @def sl_pmmREG
*   @brief Pmm Register Frame Pointer
*
*   This pointer is used by the Pmm driver to access the Pmm registers.
*/
#define sl_pmmREG ((sl_pmmBase_t *)0xFFFF0000U)

#endif
