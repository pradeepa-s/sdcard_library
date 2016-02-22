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
 * @file        sl_reg_dcc.h
 * @version     $$file_version$$
 * @brief       DCC Register Layer Header File
 *
 * @details This file contains:
 *   - Definitions
 *   - Types
 *   - Interface Prototypes
 *   .
 *   which are relevant for the DCC driver.
 */

#ifndef __SL_REG_DCC_H__
#define __SL_REG_DCC_H__

#include "sl_sys_common.h"






/* Dcc Register Frame Definition */
/** @struct sl_dccBase
*   @brief DCC Base Register Definition
*
*   This structure is used to access the DCC module registers.
*/
/** @typedef sl_dccBASE_t
*   @brief DCC Register Frame Type Definition
*
*   This type is used to access the DCC Registers.
*/
typedef volatile struct sl_dccBase
{
    uint32 GCTRL;      	    /**< 0x0000: DCC Control Register		*/
    uint32 REV;      		/**< 0x0004: DCC Revision Id Register 	*/
    uint32 CNT0SEED;    	/**< 0x0008: DCC Counter0 Seed Register	*/
    uint32 VALID0SEED;      /**< 0x000C: DCC Valid0 Seed Register 	*/
    uint32 CNT1SEED;		/**< 0x0010: DCC Counter1 Seed Register 	*/
    uint32 STAT; 			/**< 0x0014: DCC Status Register 		*/
    uint32 CNT0;    		/**< 0x0018: DCC Counter0 Value Register 	*/
    uint32 VALID0;    	    /**< 0x001C: DCC Valid0 Value Register 	*/
    uint32 CNT1;      	    /**< 0x0020: DCC Counter1 Value Register	*/
    uint32 CNT1CLKSRC;   	/**< 0x0024: DCC Counter1 Clock Source Selection Register 	*/
    uint32 CNT0CLKSRC;   	/**< 0x0028: DCC Counter0 Clock Source Selection Register 	*/
} sl_dccBASE_t;


/** @def sl_dccREG1
*   @brief DCC1 Register Frame Pointer
*
*   This pointer is used by the DCC driver to access the dcc1 module registers.
*/
#define sl_dccREG1   ((sl_dccBASE_t *)0xFFFFEC00U)


/** @def sl_dccREG2
*   @brief DCC2 Register Frame Pointer
*
*   This pointer is used by the DCC driver to access the dcc2 module registers.
*/
#define sl_dccREG2   ((sl_dccBASE_t *)0xFFFFF400U)





#endif
