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
 * @file        sl_sys_vim.h
 * @version     $$file_version$$
 * @brief       Vectored Interrupt Module Header File
 *
 * @details This file contains:
 *   - VIM Type Definitions
 *   - VIM General Definitions
 *   .
 *   which are relevant for Vectored Interrupt Controller.
 */

#ifndef __SL_SYS_VIM_H__
#define __SL_SYS_VIM_H__

#include "sl_reg_vim.h"

/* VIM Type Definitions */

/** @typedef t_isrFuncPTR
*   @brief ISR Function Pointer Type Definition
*
*   This type is used to access the ISR handler.
*/
#ifdef __TI_COMPILER_VERSION__
/*SAFETYMCUSW 298 S <APPROVED> "Not a MISRA-C:2004 violation"*/
typedef void (*sl_t_isrFuncPTR)(void);
#endif
#ifdef __IAR_SYSTEMS_ICC__
typedef void (__arm __fiq *sl_t_isrFuncPTR)(void);
#endif

/* VIM General Configuration */

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define SL_VIM_CHANNELS 96U
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define SL_VIM_CHANNELS 128U
#endif


/* Interrupt Handlers */
/*
extern void esmHighInterrupt(void);
extern void phantomInterrupt(void);
extern void esmLowInterrupt(void);
*/

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define SL_VIM_PARFLG		(*(volatile uint32 *)0xFFFFFDECU)
#define SL_VIM_PARCTL		(*(volatile uint32 *)0xFFFFFDF0U)
#define SL_VIM_ADDERR		(*(volatile uint32 *)0xFFFFFDF4U)
#define SL_VIM_FBPARERR		(*(volatile uint32 *)0xFFFFFDF8U)
#define SL_VIMRAMPARLOC		(*(volatile uint32 *)0xFFF82400U)
#define SL_VIMRAMLOC		(*(volatile uint32 *)0xFFF82000U)
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define SL_VIM_ECCSTAT		(*(volatile uint32 *)0xFFFFFDECU)
#define SL_VIM_ECCCTL		(*(volatile uint32 *)0xFFFFFDF0U)
#define SL_VIM_UERRADDR		(*(volatile uint32 *)0xFFFFFDF4U)
#define SL_VIM_FBVECADDR	(*(volatile uint32 *)0xFFFFFDF8U)
#define SL_VIM_SBERRADDR	(*(volatile uint32 *)0xFFFFFDFCU)

#define SL_VIMRAMECCLOC		(*(volatile uint32 *)0xFFF82400U)
#define SL_VIMRAMLOC		(*(volatile uint32 *)0xFFF82000U)
#endif

/* Configuration registers */
typedef struct sl_vim_config_reg
{
    uint32 CONFIG_FIRQPR0;
    uint32 CONFIG_FIRQPR1;
    uint32 CONFIG_FIRQPR2;
    uint32 CONFIG_FIRQPR3;
    uint32 CONFIG_REQMASKSET0;
    uint32 CONFIG_REQMASKSET1;
    uint32 CONFIG_REQMASKSET2;
    uint32 CONFIG_REQMASKSET3;
    uint32 CONFIG_WAKEMASKSET0;
    uint32 CONFIG_WAKEMASKSET1;
    uint32 CONFIG_WAKEMASKSET2;
    uint32 CONFIG_WAKEMASKSET3;
    uint32 CONFIG_CAPEVT;
    uint32 CONFIG_CHANCTRL[24U];
} sl_vim_config_reg_t;

#endif
