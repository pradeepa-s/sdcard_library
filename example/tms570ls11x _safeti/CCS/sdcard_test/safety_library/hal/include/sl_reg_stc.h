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
 * @file        sl_reg_stc.h
 * @version     $$file_version$$
 * @brief       STC Register Layer Header File
 *
 * @details This file contains:
 *   - Definitions
 *   - Types
 *   .
 *   which are relevant for the System driver.
 */

#ifndef __SL_REG_STC_H__
#define __SL_REG_STC_H__

#include "sl_sys_common.h"

/* Stc Register Frame Definition */
/** @struct sl_stcBase
*   @brief STC Base Register Definition
*
*   This structure is used to access the STC module registers.
*/
/** @typedef sl_stcBASE_t
*   @brief STC Register Frame Type Definition
*
*   This type is used to access the STC Registers.
*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
typedef volatile struct sl_stcBase
{
    uint32 STCGCR0;		/**< 0x0000: STC Control Register 0	*/
    uint32 STCGCR1;		/**< 0x0004: STC Control Register 1 */
    uint32 STCTPR;		/**< 0x0008: STC Self-Test Run Timeout Counter Preload Register	*/
    uint32 STCCADDR;		/**< 0x000C: STC Self-Test Current ROM Address Register */
    uint32 STCCICR;		/**< 0x0010: STC Self-Test Current Interval Count Register */
    uint32 STCGSTAT;		/**< 0x0014: STC Self-Test Global Status Register */
	uint32 STCFSTAT;		/**< 0x0018: STC Self-Test Fail Status Register */
    uint32 CPU1_CURMISR3;	/**< 0x001C: STC CPU1 Current MISR Register */
    uint32 CPU1_CURMISR2;	/**< 0x0020: STC CPU1 Current MISR Register */
    uint32 CPU1_CURMISR1;	/**< 0x0024: STC CPU1 Current MISR Register */
    uint32 CPU1_CURMISR0;	/**< 0x0028: STC CPU1 Current MISR Register */
    uint32 CPU2_CURMISR3;	/**< 0x002C: STC CPU1 Current MISR Register */
    uint32 CPU2_CURMISR2;	/**< 0x0030: STC CPU1 Current MISR Register */
    uint32 CPU2_CURMISR1;	/**< 0x0034: STC CPU1 Current MISR Register */
    uint32 CPU2_CURMISR0;	/**< 0x0038: STC CPU1 Current MISR Register */
    uint32 STCSCSCR;		/**< 0x003C: STC Signature Compare Self-Check Register */
} sl_stcBASE_t;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
typedef volatile struct sl_stcBase
{
    uint32 STCGCR0;         /**< 0x0000: STC Control Register 0    */
    uint32 STCGCR1;         /**< 0x0004: STC Control Register 1 */
    uint32 STCTPR;          /**< 0x0008: STC Self-Test Run Timeout Counter Preload Register    */
    uint32 STCCADDR;        /**< 0x000C: STC Self-Test Current ROM Address Register */
    uint32 STCCICR;         /**< 0x0010: STC Self-Test Current Interval Count Register */
    uint32 STCGSTAT;        /**< 0x0014: STC Self-Test Global Status Register */
    uint32 STCFSTAT;        /**< 0x0018: STC Self-Test Fail Status Register */
    uint32 CPU1_CURMISR3;   /**< 0x001C: STC CPU1 Current MISR Register */
    uint32 CPU1_CURMISR2;   /**< 0x0020: STC CPU1 Current MISR Register */
    uint32 CPU1_CURMISR1;   /**< 0x0024: STC CPU1 Current MISR Register */
    uint32 CPU1_CURMISR0;   /**< 0x0028: STC CPU1 Current MISR Register */
    uint32 CPU2_CURMISR3;   /**< 0x002C: STC CPU1 Current MISR Register */
    uint32 CPU2_CURMISR2;   /**< 0x0030: STC CPU1 Current MISR Register */
    uint32 CPU2_CURMISR1;   /**< 0x0034: STC CPU1 Current MISR Register */
    uint32 CPU2_CURMISR0;   /**< 0x0038: STC CPU1 Current MISR Register */
    uint32 STCSCSCR;        /**< 0x003C: STC Signature Compare Self-Check Register */
    uint32 STCCADDR1;       /**< 0x0040: STC Current ROM Address Register - CORE2 */
    uint32 STCCLKDIV;       /**< 0x0044: STC Clock Divider Register */
    uint32 STCSEGPLR;       /**< 0x0048: STC Segment First Preload Register */
} sl_stcBASE_t;
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define sl_stcREG   ((sl_stcBASE_t *)0xFFFFE600U)
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define sl_stcREG1   ((sl_stcBASE_t *)0xFFFFE600U)
#define sl_stcREG2   ((sl_stcBASE_t *)0xFFFF0800U)
#endif

#endif
