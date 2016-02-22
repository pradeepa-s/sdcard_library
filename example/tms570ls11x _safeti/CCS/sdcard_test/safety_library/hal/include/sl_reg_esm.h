/** @file reg_esm.h
*   @brief ESM Register Layer Header File
*   @date 7.January.2013
*   @version 03.04.00
*   
*   This file contains:
*   - Definitions
*   - Types
*   - Interface Prototypes
*   .
*   which are relevant for the ESM driver.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

#ifndef __SL_REG_ESM_H__
#define __SL_REG_ESM_H__

#include "sl_sys_common.h"

/* Esm Register Frame Definition */
/** @struct sl_esmBase
*   @brief Esm Register Frame Definition
*
*   This type is used to access the Esm Registers.
*/
/** @typedef sl_esmBASE_t
*   @brief Esm Register Frame Type Definition
*
*   This type is used to access the Esm Registers.
*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
typedef volatile struct sl_esmBase
{
    uint32 EEPAPR1;              /* 0x0000                 */
    uint32 DEPAPR1;              /* 0x0004                 */
    uint32 IESR1;             /* 0x0008                 */
    uint32 IECR1;             /* 0x000C                 */
    uint32 ILSR1;             /* 0x0010                 */
    uint32 ILCR1;             /* 0x0014                 */
    uint32 SR1[3U];           /* 0x0018, 0x001C, 0x0020 */
    uint32 EPSR;               /* 0x0024                 */
    uint32 IOFFHR;                /* 0x0028                 */
    uint32 IOFFLR;                /* 0x002C                 */
    uint32 LTCR;                    /* 0x0030                 */
    uint32 LTCPR;             /* 0x0034                 */
    uint32 EKR;                    /* 0x0038                 */
    uint32 SSR2;            /* 0x003C                 */
    uint32 IEPSR4;              /* 0x0040                 */
    uint32 IEPCR4;              /* 0x0044                 */
    uint32 IESR4;             /* 0x0048                 */
    uint32 IECR4;             /* 0x004C                 */
    uint32 ILSR4;             /* 0x0050                 */
    uint32 ILCR4;             /* 0x0054                 */
    uint32 SR4[3U];           /* 0x0058, 0x005C, 0x0060 */
    uint32 ESTATUS5EMU;            /* 0x0064                 */
} sl_esmBASE_t;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
typedef volatile struct sl_esmBase
{
    uint32 EEPAPR1;           /* 0x0000                 */
    uint32 DEPAPR1;           /* 0x0004                 */
    uint32 IESR1;             /* 0x0008                 */
    uint32 IECR1;             /* 0x000C                 */
    uint32 ILSR1;             /* 0x0010                 */
    uint32 ILCR1;             /* 0x0014                 */
    uint32 SR1[3U];           /* 0x0018, 0x001C, 0x0020 */
    uint32 EPSR;              /* 0x0024                 */
    uint32 IOFFHR;            /* 0x0028                 */
    uint32 IOFFLR;            /* 0x002C                 */
    uint32 LTCR;              /* 0x0030                 */
    uint32 LTCPR;             /* 0x0034                 */
    uint32 EKR;               /* 0x0038                 */
    uint32 SSR2;              /* 0x003C                 */
    uint32 IEPSR4;            /* 0x0040                 */
    uint32 IEPCR4;            /* 0x0044                 */
    uint32 IESR4;             /* 0x0048                 */
    uint32 IECR4;             /* 0x004C                 */
    uint32 ILSR4;             /* 0x0050                 */
    uint32 ILCR4;             /* 0x0054                 */
    uint32 SR4[3U];           /* 0x0058, 0x005C, 0x0060 */
    uint32 rsvd1[7U];         /* 0x0064 - 0x007C        */	
    uint32 IEPSR7;            /* 0x0080                 */
    uint32 IEPCR7;            /* 0x0084                 */
    uint32 IESR7;             /* 0x0088                 */
    uint32 IECR7;             /* 0x008C                 */
    uint32 ILSR7;             /* 0x0090                 */
    uint32 ILCR7;             /* 0x0094                 */
    uint32 SR7[3U];           /* 0x0098, 0x009C, 0x00A0 */
} sl_esmBASE_t;
#endif

/** @def sl_esmREG
*   @brief Esm Register Frame Pointer
*
*   This pointer is used by the Esm driver to access the Esm registers.
*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define sl_esmREG ((sl_esmBASE_t *)0xFFFFF500U)
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define sl_esmREG ((sl_esmBASE_t *)0xFFFFF500U)
#endif

#endif
