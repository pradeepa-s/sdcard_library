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


#ifndef __SL_MIBSPI_BITDEFN_H__
#define __SL_MIBSPI_BITDEFN_H__

#include <sl_reg_mibspi.h>

#define DEFAULT										(uint32)0x200u
#define EXTENDED									(uint32)0x400u

#define SPI_GCR0_RESET								(uint32)1u

#define SPI_ECC_MEMTST_EN							(uint32)((uint32)1u << 8u)

#define SPI_MIBSPIE_MSPIENA							(uint32)1u
#define SPI_MIBSPIE_EX_BUF_DIS						(uint32)5u
#define SPI_MIBSPIE_EX_BUF_START					(uint32)8u
#define SPI_MIBSPIE_EX_BUF_LENGTH					(uint32)4u
#define SPI_MIBSPI_RXRAM_ACCESS						(uint32)((uint32)1u << 16u)

#define SPI_DIAG_CTRL_ECC_EN						(uint32)0x5u
#define SPI_DIAG_CTRL_ECC_DIS						(uint32)0xAu
#define SPI_DIAG_CTRL_ECC_START 					(uint32)0u
#define SPI_DIAG_CTRL_ECC_LENGTH	 				(uint32)4u

#define SPI_FLIP_DATA_1BIT							(uint32)0x1u
#define SPI_FLIP_DATA_2BIT							(uint32)0x03u

#define SPI_PAR_CTRL_EX_BUF							(uint32)0xAu
#define SPI_PAR_CTRL_EX_BUF_START					(uint32)8u
#define SPI_PAR_CTRL_EX_BUF_LENGTH					(uint32)4u
#define SPI_PAR_CTRL_ECC_EN							(uint32)0xAu
#define SPI_PAR_CTRL_ECC_DIS						(uint32)0x5u
#define SPI_PAR_CTRL_ECC_START						(uint32)0u
#define SPI_PAR_CTRL_ECC_LENGTH						(uint32)4u
#define SPI_PAR_ECC_CTRL_SBE_EVT_EN					(uint32)0xAu
#define SPI_PAR_ECC_CTRL_SBE_EVT_START				(uint32)24u
#define SPI_PAR_ECC_CTRL_SBE_EVT_LENGTH				(uint32)4u

#define SPI_PAR_ECC_STAT_SBEFLGRX					(uint32)9u
#define SPI_PAR_ECC_STAT_SBEFLGTX					(uint32)8u
#define SPI_PAR_ECC_STAT_UERRFLGRX					(uint32)1u
#define SPI_PAR_ECC_STAT_UERRFLGTX					(uint32)0u

#define SPI_ECCDIAG_STAT_SBEFLGRX					(uint32)1u
#define SPI_ECCDIAG_STAT_SBEFLGTX					(uint32)0u
#define SPI_ECCDIAG_STAT_DEFLGRX					(uint32)17u
#define SPI_ECCDIAG_STAT_DEFLGTX					(uint32)16u

#define SPI_SBERR_ADDRRX_START						(uint32)0u
#define SPI_SBERR_ADDRRX_LENGTH						(uint32)10u
#define SPI_UERR_ADDRRX_START						(uint32)0u
#define SPI_UERR_ADDRRX_LENGTH						(uint32)10u

#define SPI_PAR_ECC_CTRL_EDACMODE_DIS				(uint32)0x5u
#define SPI_PAR_ECC_CTRL_EDACMODE_START				(uint32)16u
#define SPI_PAR_ECC_CTRL_EDACMODE_LENGTH			(uint32)4u

#endif /* __SL_MIBSPI_BITDEFN_H__ */
