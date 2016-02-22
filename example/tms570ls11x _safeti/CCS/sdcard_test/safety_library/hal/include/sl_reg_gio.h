/** @file HL_reg_gio.h
*   @brief GIO Register Layer Header File
*   @date 04.Oct.2013
*   @version 03.07.00
*   
*   This file contains:
*   - Definitions
*   - Types
*   - Interface Prototypes
*   .
*   which are relevant for the GIO driver.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

#ifndef __SL_REG_GIO_H__
#define __SL_REG_GIO_H__

#include "sl_sys_common.h"

/* Gio Register Frame Definition */
/** @struct gioBase
*   @brief GIO Base Register Definition
*
*   This structure is used to access the GIO module registers.
*/
/** @typedef gioBASE_t
*   @brief GIO Register Frame Type Definition
*
*   This type is used to access the GIO Registers.
*/
typedef volatile struct sl_gioBase
{
    uint32 GCR0;      /**< 0x0000: Global Control Register */
    uint32 rsvd;      /**< 0x0004: Reserved*/
    uint32 INTDET;    /**< 0x0008: Interrupt Detect Register*/
    uint32 POL;       /**< 0x000C: Interrupt Polarity Register */
    uint32 ENASET;    /**< 0x0010: Interrupt Enable Set Register */
    uint32 ENACLR;    /**< 0x0014: Interrupt Enable Clear Register */
    uint32 LVLSET;    /**< 0x0018: Interrupt Priority Set Register */
    uint32 LVLCLR;    /**< 0x001C: Interrupt Priority Clear Register */
    uint32 FLG;       /**< 0x0020: Interrupt Flag Register */
    uint32 OFF1;      /**< 0x0024: Interrupt Offset A Register */
    uint32 OFF2;      /**< 0x0028: Interrupt Offset B Register */
} sl_gioBASE_t;


/** @struct gioPort
*   @brief GIO Port Register Definition
*/
/** @typedef gioPORT_t
*   @brief GIO Port Register Type Definition
*
*   This type is used to access the GIO Port Registers.
*/
typedef volatile struct sl_gioPort
{
    uint32 DIR;    /**< 0x0000: Data Direction Register */
    uint32 DIN;    /**< 0x0004: Data Input Register */
    uint32 DOUT;   /**< 0x0008: Data Output Register */
    uint32 DSET;   /**< 0x000C: Data Output Set Register */
    uint32 DCLR;   /**< 0x0010: Data Output Clear Register */
    uint32 PDR;    /**< 0x0014: Open Drain Register */
    uint32 PULDIS; /**< 0x0018: Pullup Disable Register */
    uint32 PSL;    /**< 0x001C: Pull Up/Down Selection Register */
} sl_gioPORT_t;


/** @def gioREG
*   @brief GIO Register Frame Pointer
*
*   This pointer is used by the GIO driver to access the gio module registers.
*/
#define sl_gioREG   ((sl_gioBASE_t *)0xFFF7BC00U)

/** @def gioPORTA
*   @brief GIO Port (A) Register Pointer
*
*   Pointer used by the GIO driver to access PORTA
*/
#define sl_gioPORTA ((sl_gioPORT_t *)0xFFF7BC34U)

/** @def gioPORTB
*   @brief GIO Port (B) Register Pointer
*
*   Pointer used by the GIO driver to access PORTB
*/
#define sl_gioPORTB ((sl_gioPORT_t *)0xFFF7BC54U)

#endif
