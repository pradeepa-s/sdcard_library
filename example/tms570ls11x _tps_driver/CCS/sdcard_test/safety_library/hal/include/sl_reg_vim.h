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
 * @file        sl_reg_vim.h
 * @version     $$file_version$$
 * @brief       VIM Register Layer Header File
 *
 * @details This file contains:
 *   - Definitions
 *   - Types
 *   .
 *   which are relevant for the System driver.
 */

#ifndef __SL_REG_VIM_H__
#define __SL_REG_VIM_H__

#include "sl_sys_common.h"




/* Vim Register Frame Definition */
/** @struct sl_vimBase
*   @brief Vim Register Frame Definition
*
*   This type is used to access the Vim Registers.
*/
/** @typedef sl_vimBASE_t
*   @brief VIM Register Frame Type Definition
*
*   This type is used to access the VIM Registers.
*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
typedef volatile struct sl_vimBase
{
    uint32      IRQINDEX;         /* 0x0000        */
    uint32      FIQINDEX;         /* 0x0004        */	
    uint32        rsvd1;          /* 0x0008        */
    uint32        rsvd2;          /* 0x000C        */
    uint32      FIRQPR0;          /* 0x0010        */
    uint32      FIRQPR1;          /* 0x0014        */
    uint32      FIRQPR2;          /* 0x0018        */
    uint32      FIRQPR3;          /* 0x001C        */
    uint32      INTREQ0;          /* 0x0020        */
    uint32      INTREQ1;          /* 0x0024        */
    uint32      INTREQ2;          /* 0x0028        */
    uint32      INTREQ3;          /* 0x002C        */
    uint32      REQMASKSET0;      /* 0x0030        */
    uint32      REQMASKSET1;      /* 0x0034        */
    uint32      REQMASKSET2;      /* 0x0038        */
    uint32      REQMASKSET3;      /* 0x003C        */
    uint32      REQMASKCLR0;      /* 0x0040        */
    uint32      REQMASKCLR1;      /* 0x0044        */
    uint32      REQMASKCLR2;      /* 0x0048        */
    uint32      REQMASKCLR3;      /* 0x004C        */
    uint32      WAKEMASKSET0;     /* 0x0050        */
    uint32      WAKEMASKSET1;     /* 0x0054        */
    uint32      WAKEMASKSET2;     /* 0x0058        */
    uint32      WAKEMASKSET3;     /* 0x005C        */
    uint32      WAKEMASKCLR0;     /* 0x0060        */
    uint32      WAKEMASKCLR1;     /* 0x0064        */
    uint32      WAKEMASKCLR2;     /* 0x0068        */
    uint32      WAKEMASKCLR3;     /* 0x006C        */
    uint32      IRQVECREG;        /* 0x0070        */
    uint32      FIQVECREG;        /* 0x0074        */
    uint32      CAPEVT;           /* 0x0078        */	
    uint32        rsvd3;          /* 0x007C        */
    uint32      CHANCTRL[24U];    /* 0x0080-0x017C */
} sl_vimBASE_t;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
typedef volatile struct sl_vimBase
{
    uint32      rsvd1[59U];       /* 0x0000 - 0x00E8 Reserved */
    uint32      ECCSTAT;          /* 0x00EC        */
    uint32      ECCCTL;           /* 0x00F0        */	
    uint32      UERRADDR;         /* 0x00F4        */
    uint32      FBVECADDR;        /* 0x00F8        */
    uint32      SBERRADDR;        /* 0x00FC        */	
    uint32      IRQINDEX;         /* 0x0100        */
    uint32      FIQINDEX;         /* 0x0104        */	
    uint32      rsvd2;            /* 0x0108        */
    uint32      rsvd3;            /* 0x010C        */
    uint32      FIRQPR0;          /* 0x0110        */
    uint32      FIRQPR1;          /* 0x0114        */
    uint32      FIRQPR2;          /* 0x0118        */
    uint32      FIRQPR3;          /* 0x011C        */
    uint32      INTREQ0;          /* 0x0120        */
    uint32      INTREQ1;          /* 0x0124        */
    uint32      INTREQ2;          /* 0x0128        */
    uint32      INTREQ3;          /* 0x012C        */
    uint32      REQMASKSET0;      /* 0x0130        */
    uint32      REQMASKSET1;      /* 0x0134        */
    uint32      REQMASKSET2;      /* 0x0138        */
    uint32      REQMASKSET3;      /* 0x013C        */
    uint32      REQMASKCLR0;      /* 0x0140        */
    uint32      REQMASKCLR1;      /* 0x0144        */
    uint32      REQMASKCLR2;      /* 0x0148        */
    uint32      REQMASKCLR3;      /* 0x014C        */
    uint32      WAKEMASKSET0;     /* 0x0150        */
    uint32      WAKEMASKSET1;     /* 0x0154        */
    uint32      WAKEMASKSET2;     /* 0x0158        */
    uint32      WAKEMASKSET3;     /* 0x015C        */
    uint32      WAKEMASKCLR0;     /* 0x0160        */
    uint32      WAKEMASKCLR1;     /* 0x0164        */
    uint32      WAKEMASKCLR2;     /* 0x0168        */
    uint32      WAKEMASKCLR3;     /* 0x016C        */
    uint32      IRQVECREG;        /* 0x0170        */
    uint32      FIQVECREG;        /* 0x0174        */
    uint32      CAPEVT;           /* 0x0178        */	
    uint32      rsvd4;            /* 0x017C        */
    uint32      CHANCTRL[24U];    /* 0x0180-0x027C */
} sl_vimBASE_t;
#endif

#define sl_vimREG ((sl_vimBASE_t *)0xFFFFFE00U)


#endif
