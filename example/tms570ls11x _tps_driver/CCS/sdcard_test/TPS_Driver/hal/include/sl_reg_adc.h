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
 * @file        sl_reg_adc.h
 * @version     $$file_version$$
 * @brief       ADC Register Layer Header File
 *
 * @details This file contains:
 *   - Definitions
 *   - Types
 *   - Interface Prototypes
 *   .
 *   which are relevant for the ADC driver.
 */

#ifndef __SL_REG_ADC_H__
#define __SL_REG_ADC_H__

#include "sl_sys_common.h"

/* Adc Register Frame Definition */
/** @struct sl_adcBase
*   @brief ADC Register Frame Definition
*
*   This type is used to access the ADC Registers.
*/
/** @typedef sl_adcBASE_t
*   @brief ADC Register Frame Type Definition
*
*   This type is used to access the ADC Registers.
*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
typedef volatile struct sl_adcBase
{
    uint32 RSTCR;              /**< 0x0000: Reset control register                            */
    uint32 OPMODECR;           /**< 0x0004: Operating mode control register                   */
    uint32 CLOCKCR;            /**< 0x0008: Clock control register                            */
    uint32 CALCR;              /**< 0x000C: Calibration control register                      */
    uint32 GxMODECR[3U];       /**< 0x0010,0x0014,0x0018: Group 0-2 mode control register     */
    uint32 EVSRC;              /**< 0x001C: Group 0 trigger source control register           */
    uint32 G1SRC;              /**< 0x0020: Group 1 trigger source control register           */
    uint32 G2SRC;              /**< 0x0024: Group 2 trigger source control register           */
    uint32 GxINTENA[3U];       /**< 0x0028,0x002C,0x0030: Group 0-2 interrupt enable register */
    uint32 GxINTFLG[3U];       /**< 0x0034,0x0038,0x003C: Group 0-2 interrupt flag register   */
    uint32 GxINTCR[3U];        /**< 0x0040-0x0048: Group 0-2 interrupt threshold register     */
    uint32 EVDMACR;            /**< 0x004C: Group 0 DMA control register                      */
    uint32 G1DMACR;            /**< 0x0050: Group 1 DMA control register                      */
    uint32 G2DMACR;            /**< 0x0054: Group 2 DMA control register                      */
    uint32 BNDCR;              /**< 0x0058: Buffer boundary control register                  */
    uint32 BNDEND;             /**< 0x005C: Buffer boundary end register                      */
    uint32 EVSAMP;             /**< 0x0060: Group 0 sample window register                    */
    uint32 G1SAMP;             /**< 0x0064: Group 1 sample window register                    */
    uint32 G2SAMP;             /**< 0x0068: Group 2 sample window register                    */
    uint32 EVSR;               /**< 0x006C: Group 0 status register                           */
    uint32 G1SR;               /**< 0x0070: Group 1 status register                           */
    uint32 G2SR;               /**< 0x0074: Group 2 status register                           */
    uint32 GxSEL[3U];          /**< 0x0078-0x007C: Group 0-2 channel select register          */
    uint32 CALR;               /**< 0x0084: Calibration register                              */
    uint32 SMSTATE;            /**< 0x0088: State machine state register                      */
    uint32 LASTCONV;           /**< 0x008C: Last conversion register                          */
    struct
    {
        uint32 BUF0;           /**< 0x0090,0x00B0,0x00D0: Group 0-2 result buffer 1 register  */
        uint32 BUF1;           /**< 0x0094,0x00B4,0x00D4: Group 0-2 result buffer 1 register  */
        uint32 BUF2;           /**< 0x0098,0x00B8,0x00D8: Group 0-2 result buffer 2 register  */
        uint32 BUF3;           /**< 0x009C,0x00BC,0x00DC: Group 0-2 result buffer 3 register  */
        uint32 BUF4;           /**< 0x00A0,0x00C0,0x00E0: Group 0-2 result buffer 4 register  */
        uint32 BUF5;           /**< 0x00A4,0x00C4,0x00E4: Group 0-2 result buffer 5 register  */
        uint32 BUF6;           /**< 0x00A8,0x00C8,0x00E8: Group 0-2 result buffer 6 register  */
        uint32 BUF7;           /**< 0x00AC,0x00CC,0x00EC: Group 0-2 result buffer 7 register  */
    } GxBUF[3U];
    uint32 EVEMUBUFFER;        /**< 0x00F0: Group 0 emulation result buffer                   */
    uint32 G1EMUBUFFER;        /**< 0x00F4: Group 1 emulation result buffer                   */
    uint32 G2EMUBUFFER;        /**< 0x00F8: Group 2 emulation result buffer                   */
    uint32 EVTDIR;             /**< 0x00FC: Event pin direction register                      */
    uint32 EVTOUT;             /**< 0x0100: Event pin digital output register                 */
    uint32 EVTIN;              /**< 0x0104: Event pin digital input register                  */
    uint32 EVTSET;             /**< 0x0108: Event pin set register                            */
    uint32 EVTCLR;             /**< 0x010C: Event pin clear register                          */
    uint32 EVTPDR;             /**< 0x0110: Event pin open drain register                     */
    uint32 EVTDIS;             /**< 0x0114: Event pin pull disable register                   */
    uint32 EVTPSEL;            /**< 0x0118: Event pin pull select register                    */
    uint32 EVSAMPDISEN;        /**< 0x011C: Group 0 sample discharge register                 */
    uint32 G1SAMPDISEN;        /**< 0x0120: Group 1 sample discharge register                 */
    uint32 G2SAMPDISEN;        /**< 0x0124: Group 2 sample discharge register                 */
    uint32 MAGINTCR1;          /**< 0x0128: Magnitude interrupt control register 1            */
    uint32 MAGINT1MASK;        /**< 0x012C: Magnitude interrupt mask register 1               */
    uint32 MAGINTCR2;          /**< 0x0130: Magnitude interrupt control register 2            */
    uint32 MAGINT2MASK;        /**< 0x0134: Magnitude interrupt mask register 2               */
    uint32 MAGINTCR3;          /**< 0x0138: Magnitude interrupt control register 3            */
    uint32 MAGINT3MASK;        /**< 0x013C: Magnitude interrupt mask register 3               */
    uint32   rsvd1;            /**< 0x0140: Reserved                                          */
    uint32   rsvd2;            /**< 0x0144: Reserved                                          */
    uint32   rsvd3;            /**< 0x0148: Reserved                                          */
    uint32   rsvd4;            /**< 0x014C: Reserved                                          */
    uint32   rsvd5;            /**< 0x0150: Reserved                                          */
    uint32   rsvd6;            /**< 0x0154: Reserved                                          */
    uint32 MAGTHRINTENASET;    /**< 0x0158: Magnitude interrupt set register                  */
    uint32 MAGTHRINTENACLR;    /**< 0x015C: Magnitude interrupt clear register                */
    uint32 MAGTHRINTFLG;       /**< 0x0160: Magnitude interrupt flag register                 */
    uint32 MAGTHRINTOFFSET;    /**< 0x0164: Magnitude interrupt offset register               */
    uint32 GxFIFORESETCR[3U];  /**< 0x0168,0x016C,0x0170: Group 0-2 fifo reset register       */
    uint32 EVRAMADDR;          /**< 0x0174: Group 0 RAM pointer register                      */
    uint32 G1RAMADDR;          /**< 0x0178: Group 1 RAM pointer register                      */
    uint32 G2RAMADDR;          /**< 0x017C: Group 2 RAM pointer register                      */
    uint32 PARCR;              /**< 0x0180: Parity control register                           */
    uint32 PARADDR;            /**< 0x0184: Parity error address register                     */
    uint32 PWRUPDLYCTRL;       /**< 0x0188: Power-Up delay control register                   */
} sl_adcBASE_t;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
typedef volatile struct sl_adcBase
{
    uint32 RSTCR;              /**< 0x0000: Reset control register                            */
    uint32 OPMODECR;           /**< 0x0004: Operating mode control register                   */
    uint32 CLOCKCR;            /**< 0x0008: Clock control register                            */
    uint32 CALCR;              /**< 0x000C: Calibration control register                      */
    uint32 GxMODECR[3U];       /**< 0x0010,0x0014,0x0018: Group 0-2 mode control register     */
    uint32 EVSRC;              /**< 0x001C: Group 0 trigger source control register           */
    uint32 G1SRC;              /**< 0x0020: Group 1 trigger source control register           */
    uint32 G2SRC;              /**< 0x0024: Group 2 trigger source control register           */
    uint32 GxINTENA[3U];       /**< 0x0028,0x002C,0x0030: Group 0-2 interrupt enable register */
    uint32 GxINTFLG[3U];       /**< 0x0034,0x0038,0x003C: Group 0-2 interrupt flag register   */
    uint32 GxINTCR[3U];        /**< 0x0040-0x0048: Group 0-2 interrupt threshold register     */
    uint32 EVDMACR;            /**< 0x004C: Group 0 DMA control register                      */
    uint32 G1DMACR;            /**< 0x0050: Group 1 DMA control register                      */
    uint32 G2DMACR;            /**< 0x0054: Group 2 DMA control register                      */
    uint32 BNDCR;              /**< 0x0058: Buffer boundary control register                  */
    uint32 BNDEND;             /**< 0x005C: Buffer boundary end register                      */
    uint32 EVSAMP;             /**< 0x0060: Group 0 sample window register                    */
    uint32 G1SAMP;             /**< 0x0064: Group 1 sample window register                    */
    uint32 G2SAMP;             /**< 0x0068: Group 2 sample window register                    */
    uint32 EVSR;               /**< 0x006C: Group 0 status register                           */
    uint32 G1SR;               /**< 0x0070: Group 1 status register                           */
    uint32 G2SR;               /**< 0x0074: Group 2 status register                           */
    uint32 GxSEL[3U];          /**< 0x0078-0x007C: Group 0-2 channel select register          */
    uint32 CALR;               /**< 0x0084: Calibration register                              */
    uint32 SMSTATE;            /**< 0x0088: State machine state register                      */
    uint32 LASTCONV;           /**< 0x008C: Last conversion register                          */
    struct
    {
        uint32 BUF0;           /**< 0x0090,0x00B0,0x00D0: Group 0-2 result buffer 1 register  */
        uint32 BUF1;           /**< 0x0094,0x00B4,0x00D4: Group 0-2 result buffer 1 register  */
        uint32 BUF2;           /**< 0x0098,0x00B8,0x00D8: Group 0-2 result buffer 2 register  */
        uint32 BUF3;           /**< 0x009C,0x00BC,0x00DC: Group 0-2 result buffer 3 register  */
        uint32 BUF4;           /**< 0x00A0,0x00C0,0x00E0: Group 0-2 result buffer 4 register  */
        uint32 BUF5;           /**< 0x00A4,0x00C4,0x00E4: Group 0-2 result buffer 5 register  */
        uint32 BUF6;           /**< 0x00A8,0x00C8,0x00E8: Group 0-2 result buffer 6 register  */
        uint32 BUF7;           /**< 0x00AC,0x00CC,0x00EC: Group 0-2 result buffer 7 register  */
    } GxBUF[3U];
    uint32 EVEMUBUFFER;        /**< 0x00F0: Group 0 emulation result buffer                   */
    uint32 G1EMUBUFFER;        /**< 0x00F4: Group 1 emulation result buffer                   */
    uint32 G2EMUBUFFER;        /**< 0x00F8: Group 2 emulation result buffer                   */
    uint32 EVTDIR;             /**< 0x00FC: Event pin direction register                      */
    uint32 EVTOUT;             /**< 0x0100: Event pin digital output register                 */
    uint32 EVTIN;              /**< 0x0104: Event pin digital input register                  */
    uint32 EVTSET;             /**< 0x0108: Event pin set register                            */
    uint32 EVTCLR;             /**< 0x010C: Event pin clear register                          */
    uint32 EVTPDR;             /**< 0x0110: Event pin open drain register                     */
    uint32 EVTDIS;             /**< 0x0114: Event pin pull disable register                   */
    uint32 EVTPSEL;            /**< 0x0118: Event pin pull select register                    */
    uint32 EVSAMPDISEN;        /**< 0x011C: Group 0 sample discharge register                 */
    uint32 G1SAMPDISEN;        /**< 0x0120: Group 1 sample discharge register                 */
    uint32 G2SAMPDISEN;        /**< 0x0124: Group 2 sample discharge register                 */
    uint32 MAGINTCR1;          /**< 0x0128: Magnitude interrupt control register 1            */
    uint32 MAGINT1MASK;        /**< 0x012C: Magnitude interrupt mask register 1               */
    uint32 MAGINTCR2;          /**< 0x0130: Magnitude interrupt control register 2            */
    uint32 MAGINT2MASK;        /**< 0x0134: Magnitude interrupt mask register 2               */
    uint32 MAGINTCR3;          /**< 0x0138: Magnitude interrupt control register 3            */
    uint32 MAGINT3MASK;        /**< 0x013C: Magnitude interrupt mask register 3               */
    uint32   rsvd1;            /**< 0x0140: Reserved                                          */
    uint32   rsvd2;            /**< 0x0144: Reserved                                          */
    uint32   rsvd3;            /**< 0x0148: Reserved                                          */
    uint32   rsvd4;            /**< 0x014C: Reserved                                          */
    uint32   rsvd5;            /**< 0x0150: Reserved                                          */
    uint32   rsvd6;            /**< 0x0154: Reserved                                          */
    uint32 MAGTHRINTENASET;    /**< 0x0158: Magnitude interrupt set register                  */
    uint32 MAGTHRINTENACLR;    /**< 0x015C: Magnitude interrupt clear register                */
    uint32 MAGTHRINTFLG;       /**< 0x0160: Magnitude interrupt flag register                 */
    uint32 MAGTHRINTOFFSET;    /**< 0x0164: Magnitude interrupt offset register               */
    uint32 GxFIFORESETCR[3U];  /**< 0x0168,0x016C,0x0170: Group 0-2 fifo reset register       */
    uint32 EVRAMADDR;          /**< 0x0174: Group 0 RAM pointer register                      */
    uint32 G1RAMADDR;          /**< 0x0178: Group 1 RAM pointer register                      */
    uint32 G2RAMADDR;          /**< 0x017C: Group 2 RAM pointer register                      */
    uint32 PARCR;              /**< 0x0180: Parity control register                           */
    uint32 PARADDR;            /**< 0x0184: Parity error address register                     */
    uint32 PWRUPDLYCTRL;       /**< 0x0188: Power-Up delay control register                   */
    uint32   rsvd7;            /**< 0x018C: Reserved                                            */
	uint32 ADEVCHNSELMODECTRL; /**< 0x0190: Event Group Channel Selection Mode Control Register */
    uint32 ADG1CHNSELMODECTRL; /**< 0x0194: Group1 Channel Selection Mode Control Register      */
    uint32 ADG2CHNSELMODECTRL; /**< 0x0198: Group2 Channel Selection Mode Control Register      */
    uint32 ADEVCURRCOUNT;      /**< 0x019C: Event Group Current Count Register                  */
    uint32 ADEVMAXCOUNT;       /**< 0x01A0: Event Group Max Count Register                      */
    uint32 ADG1CURRCOUNT;      /**< 0x01A4: Group1 Current Count Register                       */
    uint32 ADG1MAXCOUNT;       /**< 0x01A8: Group1 Max Count Register                           */
    uint32 ADG2CURRCOUNT;      /**< 0x01AC: Group2 Current Count Register                       */
    uint32 ADG2MAXCOUNT;       /**< 0x01B0: Group2 Max Count Register                           */
} sl_adcBASE_t;
#endif


/** @struct adcLUTEntry
*   @brief ADC Look-Up Table Entry
*
*   This type is used to access ADC Look-Up Table Entry
*/
/** @typedef adcLUTEntry_t
*   @brief ADC Look-Up Table Entry
*
*   This type is used to access the Look-Up Table Entry.
*/
typedef struct sl_adcLUTEntry
{
#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
    uint8 EV_INT_CHN_MUX_SEL;
    uint8 EV_EXT_CHN_MUX_SEL;
    uint16 rsvd;
#else
    uint16 rsvd;
    uint8 EV_EXT_CHN_MUX_SEL;
    uint8 EV_INT_CHN_MUX_SEL;
#endif
}sl_adcLUTEntry_t;


/** @struct adcLUT
*   @brief ADC Look-Up Table
*
*   This type is used to access ADC Look-Up Table
*/
/** @typedef adcLUT_t
*   @brief ADC Look-Up Table
*
*   This type is used to access the ADC Look-Up Table.
*/
typedef volatile struct sl_adcLUT
{
    sl_adcLUTEntry_t eventGroup[32];
    sl_adcLUTEntry_t Group1[32];
    sl_adcLUTEntry_t Group2[32];
} sl_adcLUT_t;


/** @def sl_adcREG1
*   @brief ADC1 Register Frame Pointer
*
*   This pointer is used by the ADC driver to access the ADC1 registers.
*/
#define sl_adcREG1 ((sl_adcBASE_t *)0xFFF7C000U)

/** @def sl_adcREG2
*   @brief ADC2 Register Frame Pointer
*
*   This pointer is used by the ADC driver to access the ADC2 registers.
*/
#define sl_adcREG2 ((sl_adcBASE_t *)0xFFF7C200U)

/** @def sl_adcRAM1
*   @brief ADC1 RAM Pointer
*
*   This pointer is used by the ADC driver to access the ADC1 RAM.
*/
#define sl_adcRAM1 (*(volatile uint32 *)0xFF3E0000U)

/** @def sl_adcRAM2
*   @brief ADC2 RAM Pointer
*
*   This pointer is used by the ADC driver to access the ADC2 RAM.
*/
#define sl_adcRAM2 (*(volatile uint32 *)0xFF3A0000U)

/** @def sl_adcPARRAM1
*   @brief ADC1 Parity RAM Pointer
*
*   This pointer is used by the ADC driver to access the ADC1 Parity RAM.
*/
#define sl_adcPARRAM1 (*(volatile uint32 *)(0xFF3E0000U + 0x1000U))

/** @def sl_adcPARRAM2
*   @brief ADC2 Parity RAM Pointer
*
*   This pointer is used by the ADC driver to access the ADC2 Parity RAM.
*/
#define sl_adcPARRAM2 (*(volatile uint32 *)(0xFF3A0000U + 0x1000U))

/** @def adcLUT1
*   @brief ADC1 Look-Up Table
*
*   This pointer is used by the ADC driver to access the ADC1 Look-Up Table.
*/
#define sl_adcLUT1 ((sl_adcLUT_t *) 0xFF3E2000U)

/** @def adcLUT2
*   @brief ADC2 Look-Up Table
*
*   This pointer is used by the ADC driver to access the ADC2 Look-Up Table.
*/
#define sl_adcLUT2 ((sl_adcLUT_t *) 0xFF3A2000U)


#endif
