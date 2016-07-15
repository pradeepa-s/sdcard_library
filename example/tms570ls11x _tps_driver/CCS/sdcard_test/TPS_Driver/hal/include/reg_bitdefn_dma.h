/** @file rti.h
*   @brief RTI Driver Header File
*   @date 15.Mar.2012
*   @version 03.01.00
*
*   This file contains:
*   - Definitions
*   - Types
*   - Interface Prototypes
*   .
*   which are relevant for the RTI driver.
*/

/* (c) Texas Instruments 2009-2012, All rights reserved. */


#ifndef __DMA_BITDEFN_H__
#define __DMA_BITDEFN_H__

#include <sl_reg_dma.h>

#define DMA_PARCTRL_ECC_DETECT_EN_START 			(uint32)0u
#define DMA_PARCTRL_ECC_DETECT_EN_LENGTH 			(uint32)4u
#define DMA_PARCTRL_ECC_EN							(uint32)0xAu
#define DMA_PARCTRL_ECC_DIS							(uint32)0x5u
#define DMA_PARCTRL_ECC_EN_START					(uint32)0u
#define DMA_PARCTRL_ECC_EN_LENGTH					(uint32)4u
#define DMA_SBERRADDR								((uint32)0xFFFu << 0)

#define DMA_SRAM_ECC_DISABLED 						(uint32)0x5u

#define DMA_ECC_TST_EN			 					(uint32)((uint32)1u << 8u)
#define DMA_SBE_EVT_EN								(uint32)0xAu
#define DMA_SBE_EVT_DIS							    (uint32)0x5u
#define DMA_SBE_EVT_START							(uint32)8u
#define DMA_SBE_EVT_LENGTH							(uint32)4u

#define DMA_ECC_SBERR 								(uint32)16u
#define DMA_ECC_ERR 							(uint32)24u
#define DMA_ECC_UNCORERR_ADDR_START					(uint32)0u
#define DMA_ECC_UNCORERR_ADDR_LEN					(uint32)12u

#define DMA_FLIP_DATA_1BIT							(uint32)0x1u
#define DMA_FLIP_DATA_2BIT							(uint32)0x03u
#define DMA_SYN_1BIT_DATA_ECC						(uint32)0x1Cu	/* Introduce a one bit error in bit 23 */
#define DMA_SYN_2BIT_DATA_ECC						(uint32)0x1Du	/* Introduce a two bit error for 0 -> 5 */

#endif
