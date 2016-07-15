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
 * @file        sl_reg_efc.h
 * @version     $$file_version$$
 * @brief       EFC Register Layer Header File
 *
 * @details This file contains:
 *   - Definitions
 *   - Types
 *   .
 *   which are relevant for the System driver.
 */

#ifndef __SL_REG_EFC_H__
#define __SL_REG_EFC_H__

#include "sl_sys_common.h"




/* Efc Register Frame Definition */
/** @struct sl_efcBase
*   @brief Efc Register Frame Definition
*
*   This type is used to access the Efc Registers.
*/
/** @typedef sl_efcBASE_t
*   @brief Efc Register Frame Type Definition
*
*   This type is used to access the Efc Registers.
*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
typedef volatile struct sl_efcBase
{
	uint32 INSTRUCTION;    			/* 0x0	INSTRUCTION AN DUMPWORD REGISTER	*/
	uint32 ADDRESS;			    	/* 0x4 ADDRESS REGISTER    					*/
	uint32 DATA_UPPER;			    /* 0x8 DATA UPPER REGISTER    				*/
	uint32 DATA_LOWER;				/* 0xc DATA LOWER REGISTER    				*/
	uint32 SYSTEM_CONFIG;				/* 0x10 SYSTEM CONFIG REGISTER    			*/
	uint32 SYSTEM_STATUS;				/* 0x14 SYSTEM STATUS REGISTER    			*/
	uint32 ACCUMULATOR;				/* 0x18 ACCUMULATOR REGISTER    			*/
	uint32 BOUNDARY;			    	/* 0x1C BOUNDARY REGISTER 					*/
	uint32 KEY_FLAG;			    	/* 0x20 KEY FLAG REGISTER    				*/
	uint32 KEY;			        	/* 0x24	KEY REGISTER 						*/
	uint32   rsvd1;			        /* 0x28	RESERVED 							*/
	uint32 PINS;				        /* 0x2C  PINS REGISTER						*/
	uint32 CRA;				        /* 0x30  CRA								*/
	uint32 READ;				        /* 0x34 READ REGISTER						*/
	uint32 PROGRAMME;				    /* 0x38 PROGRAMME REGISTER					*/
	uint32 ERROR;				        /* 0x3C	ERROR STATUS REGISTER				*/
	uint32 SINGLE_BIT;				/* 0x40	SINGLE BIT ERROR 					*/
	uint32 TWO_BIT_ERROR;				/* 0x44	DOUBLE BIT ERROR 					*/
	uint32 SELF_TEST_CYCLES;			/* 0x48	SELF TEST CYCLEX					*/
	uint32 SELF_TEST_SIGN;			/* 0x4C SELF TEST SIGNATURE					*/
} sl_efcBASE_t;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
typedef volatile struct sl_efcBase
{
    uint32 rsvd1;             /* 0x00 RESERVED                             */
    uint32 rsvd2;             /* 0x04 RESERVED                             */
    uint32 rsvd3;             /* 0x08 RESERVED                             */
    uint32 rsvd4;             /* 0x0C RESERVED                             */	
    uint32 rsvd5;             /* 0x10 RESERVED                             */
    uint32 rsvd6;             /* 0x14 RESERVED                             */
    uint32 rsvd7;             /* 0x18 RESERVED                             */
    uint32 BOUND;             /* 0x1C RESERVED                             */
    uint32 rsvd8;             /* 0x20 RESERVED                             */
    uint32 rsvd9;             /* 0x24 RESERVED                             */
    uint32 rsvd10;            /* 0x28 RESERVED                             */
    uint32 PINS;              /* 0x2C RESERVED                             */
    uint32 rsvd11;            /* 0x30 RESERVED                             */
    uint32 rsvd12;            /* 0x34 RESERVED                             */
    uint32 rsvd13;            /* 0x38 RESERVED                             */
    uint32 ERR_STAT;          /* 0x3C RESERVED                             */
    uint32 rsvd14;            /* 0x40 RESERVED                             */
    uint32 rsvd15;            /* 0x44 RESERVED                             */
    uint32 ST_CY;             /* 0x48 RESERVED                             */
    uint32 ST_SIG;            /* 0x4C RESERVED                             */
} sl_efcBASE_t;
#endif

#define sl_efcREG   ((sl_efcBASE_t *)0xFFF8C000U)


#endif
