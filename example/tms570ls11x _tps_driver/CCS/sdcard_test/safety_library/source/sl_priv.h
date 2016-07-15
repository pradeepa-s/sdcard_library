/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
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
 * @file        sl_priv.h
 * @version     2.2.0
 * @brief       Private header file for SafeTI&trade; Hercules&trade; Diagnostic Library
 *
 * @details Contains interfaces/definitions that are private to SafeTI&trade; Hercules&trade; Diagnostic library & are not\n
 *  exposed to application.
 */
#ifndef __SL_PRIV_H__
#define __SL_PRIV_H__

#include <sl_types.h>
#include <sl_config.h>
#include <sl_api.h>

#include <sl_reg_system.h>




/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT(n)                  ((uint32)((uint32)1u <<(n)))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_SET(y, mask)        ((y) |=  (mask))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_CLEAR(y, mask)      ((y) &= ~(mask))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_FLIP(y, mask)       ((y) ^=  (mask))

/* Create a bitmask of length \a len.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_MASK(len)           (uint32)(BIT(len)-(uint32)1u)

/* Create a bitfield mask of length \a starting at bit \a start.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_MASK(start, len)     (BIT_MASK(len)<<(start))

/* Prepare a bitmask for insertion or combining.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_PREP(x, start, len)  (((x)&BIT_MASK(len)) << (start))

/*SAFETYMCUSW 79 S MR:19.4 <APPROVED> "Use of ternary operator is allowed" */
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define GET_ESM_BIT_NUM(x)		((x)<(uint32)32U? (BIT((x))): ((x)<(uint32)64u?(BIT(((x)-((uint32)32U)))):(BIT(((x)-((uint32)64u))))))
/* Extract a bitfield of length \a len starting at bit \a start from \a y.
 * y is the register
 * start is the starting bit of the field
 * len is the length of the field*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_GET(y, start, len)   ((uint32)(((uint32)((y)>>(start))) & BIT_MASK(len)))

/*Insert a new bitfield value \a x into \a y.
 * y is the register
 * x is the value of the field
 * start is the starting bit of the field
 * len is the length of the field*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_SET(y, x, start, len)    \
    ( (y) = ((y) & (~(BF_MASK((start),(len))))) | BF_PREP((x),(start),(len)))


#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define _SELFTEST_SRAMECC_MIN       ((SL_SelfTestType)SRAM_ECC_ERROR_FORCING_1BIT)
#define _SELFTEST_SRAMECC_MAX       ((SL_SelfTestType)SRAM_RADECODE_DIAGNOSTICS)

#define _SELFTEST_FLASHECC_MIN      ((SL_SelfTestType)FLASH_ECC_ADDR_TAG_REG_MODE)
#define _SELFTEST_FLASHECC_MAX      ((SL_SelfTestType)FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT)

#define _SELFTEST_FEEECC_MIN		((SL_SelfTestType)FEE_ECC_DATA_CORR_MODE)
#define _SELFTEST_FEEECC_MAX		((SL_SelfTestType)FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT)

#define _SELFTEST_PBIST_MIN         ((SL_SelfTestType)PBIST_EXECUTE)
#define _SELFTEST_PBIST_MAX         ((SL_SelfTestType)PBIST_EXECUTE_OVERRIDE)

#define _SELFTEST_STC_MIN           ((SL_SelfTestType)STC_RUN)
#define _SELFTEST_STC_MAX           ((SL_SelfTestType)STC_COMPARE_SELFCHECK)

#define _SELFTEST_PSCON_MIN         (SL_SelfTestType)(PSCON_SELF_TEST)
#define _SELFTEST_PSCON_MAX         ((SL_SelfTestType)PSCON_PMA_TEST)

#define _SELFTEST_EFUSE_MIN         ((SL_SelfTestType)EFUSE_SELF_TEST_AUTOLOAD)
#define _SELFTEST_EFUSE_MAX         ((SL_SelfTestType)EFUSE_SELF_TEST_STUCK_AT_ZERO)

#define _SELFTEST_CCMR4F_MIN        ((SL_SelfTestType)CCMR4F_SELF_TEST)
#define _SELFTEST_CCMR4F_MAX        ((SL_SelfTestType)CCMR4F_SELF_TEST_ERROR_FORCING)

#define _SELFTEST_L2L3INTERCONNECT_MIN        ((SL_SelfTestType)L2INTERCONNECT_FAULT_INJECT)
#define _SELFTEST_L2L3INTERCONNECT_MAX        ((SL_SelfTestType)L3INTERCONNECT_FAULT_INJECT)
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define _SELFTEST_SRAMECC_MIN       ((SL_SelfTestType)SRAM_ECC_ERROR_FORCING_1BIT)
#define _SELFTEST_SRAMECC_MAX       ((SL_SelfTestType)SRAM_ECC_ERROR_FORCING_2BIT)

#define _SELFTEST_FLASHECC_MIN      ((SL_SelfTestType)FLASH_ECC_TEST_MODE_1BIT)
#define _SELFTEST_FLASHECC_MAX      ((SL_SelfTestType)FLASH_ECC_TEST_MODE_2BIT)

#define _SELFTEST_PBIST_MIN         ((SL_SelfTestType)PBIST_EXECUTE)
#define _SELFTEST_PBIST_MAX         ((SL_SelfTestType)PBIST_EXECUTE_OVERRIDE)

#define _SELFTEST_PSCON_MIN         (SL_SelfTestType)(PSCON_SELF_TEST)
#define _SELFTEST_PSCON_MAX         ((SL_SelfTestType)PSCON_PMA_TEST)

#define _SELFTEST_EFUSE_MIN         ((SL_SelfTestType)EFUSE_SELF_TEST_AUTOLOAD)
#define _SELFTEST_EFUSE_MAX         ((SL_SelfTestType)EFUSE_SELF_TEST_STUCK_AT_ZERO)

#define _SELFTEST_CCMR5F_MIN        ((SL_SelfTestType)CCMR5F_CPUCOMP_SELF_TEST)
#define _SELFTEST_CCMR5F_MAX        ((SL_SelfTestType)CCMR5F_INMCOMP_SELF_TEST_ERROR_FORCING)

#define _SELFTEST_DMA_MIN        ((SL_SelfTestType)DMA_ECC_TEST_MODE_1BIT)
#define _SELFTEST_DMA_MAX        ((SL_SelfTestType)DMA_ECC_TEST_MODE_2BIT_FAULT_INJECT)

#define _SELFTEST_CAN_MIN        ((SL_SelfTestType)CAN_ECC_TEST_MODE_1BIT)
#define _SELFTEST_CAN_MAX        ((SL_SelfTestType)CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT)

#define DCAN_MIN                    1
#define DCAN_MAX                    4

#define _SELFTEST_STC_MIN           ((SL_SelfTestType)STC1_RUN)
#define _SELFTEST_STC_MAX           ((SL_SelfTestType)STC2_COMPARE_SELFCHECK)

#endif


#define ARM_MODE_USR    (0x10u)
#define ARM_MODE_FIQ    (0x11u)
#define ARM_MODE_IRQ    (0x12u)
#define ARM_MODE_SVC    (0x13u)
#define ARM_MODE_ABT    (0x17u)
#define ARM_MODE_UND    (0x1Bu)
#define ARM_MODE_SYS    (0x1Fu)

#define CCMR4F_EFT_EXEC 0u
#define CCMR4F_STEFT_EXEC 1u
#define CCMR4F_PSCON_PMA 2u

#define ADC_APPROXIMATION_THRESHOLD 5u
#define ADC_FIFO_SIZE 1u
#define ADC_POWERDOWN_MODE         (0x00000100U)
#define ADC_ENABLE                 (0x00000001U)

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define ALL_PBIST_RAM_GROUPS (PBIST_RAMGROUP_01_PBIST_ROM |\
                             PBIST_RAMGROUP_02_STC_ROM   |\
                             PBIST_RAMGROUP_03_DCAN1     |\
                             PBIST_RAMGROUP_04_DCAN2     |\
                             PBIST_RAMGROUP_05_DCAN3     |\
                             PBIST_RAMGROUP_06_ESRAM1    |\
                             PBIST_RAMGROUP_07_MIBSPI1   |\
                             PBIST_RAMGROUP_08_MIBSPI3   |\
                             PBIST_RAMGROUP_09_MIBSPI5   |\
                             PBIST_RAMGROUP_10_VIM       |\
                             PBIST_RAMGROUP_11_MIBADC1   |\
                             PBIST_RAMGROUP_12_DMA       |\
                             PBIST_RAMGROUP_13_N2HET1    |\
                             PBIST_RAMGROUP_14_HETTU1    |\
                             PBIST_RAMGROUP_15_RTP       |\
                             PBIST_RAMGROUP_16_FRAY      |\
                             PBIST_RAMGROUP_17_FRAY      |\
                             PBIST_RAMGROUP_18_MIBADC2   |\
                             PBIST_RAMGROUP_19_N2HET2    |\
                             PBIST_RAMGROUP_20_HETTU2    |\
                             PBIST_RAMGROUP_21_ESRAM5    |\
                             PBIST_RAMGROUP_22_ESRAM6    |\
                             PBIST_RAMGROUP_23_ETHERNET1 |\
                             PBIST_RAMGROUP_24_ETHERNET2 |\
                             PBIST_RAMGROUP_25_ETHERNET3 |\
                             PBIST_RAMGROUP_26_USB1      |\
                             PBIST_RAMGROUP_27_USB2      |\
                             PBIST_RAMGROUP_28_ESRAM8    |\
                             PBIST_RAMGROUP_29_RSVD)

#define ALL_PBISALGO (PBISTALGO_TRIPLE_READ_SLOW_READ |\
                     PBISTALGO_TRIPLE_READ_FAST_READ  |\
                     PBISTALGO_MARCH13N_RED_2PORT     |\
                     PBISTALGO_MARCH13N_RED_1PORT     |\
                     PBISTALGO_DOWN1A_RED_2PORT       |\
                     PBISTALGO_DOWN1A_RED_1PORT       |\
                     PBISTALGO_MAPCOLUMN_2PORT        |\
                     PBISTALGO_MAPCOLUMN_1PORT        |\
                     PBISTALGO_PRECHARGE_2PORT        |\
                     PBISTALGO_PRECHARGE_1PORT        |\
                     PBISTALGO_DTXN2_2PORT            |\
                     PBISTALGO_DTXN2_1PORT            |\
                     PBISTALGO_PMOS_OPEN_2PORT        |\
                     PBISTALGO_PMOS_OPEN_1PORT        |\
                     PBISTALGO_PMOS_OP_SLICE1_2PORT |\
                     PBISTALGO_PMOS_OP_SLICE2_2PORT |\
                     PBISTALGO_FLIP10_2PORT           |\
                     PBISTALGO_FLIP10_1PORT           |\
                     PBISTALGO_IDDQ_2PORT             |\
                     PBISTALGO_IDDQ_1PORT             |\
                     PBISTALGO_RETENTION_2PORT        |\
                     PBISTALGO_RETENTION_1PORT        |\
                     PBISTALGO_IDDQ2_2PORT            |\
                     PBISTALGO_IDDQ2_1PORT            |\
                     PBISTALGO_RETENTION2_2PORT       |\
                     PBISTALGO_RETENTION2_1PORT       |\
                     PBISTALGO_IDDQROWSTRIPE_2PORT    |\
                     PBISTALGO_IDDQROWSTRIPE_1PORT    |\
                     PBISTALGO_IDDQROWSTRIPE2_2PORT   |\
                     PBISTALGO_IDDQROWSTRIPE2_1PORT)
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define ALL_PBIST_RAM_GROUPS (PBIST_RAMGROUP_01_PBIST_ROM |\
					PBIST_RAMGROUP_02_STC1_1_ROM_R5		|\
					PBIST_RAMGROUP_03_STC1_2_ROM_R5		|\
					PBIST_RAMGROUP_04_STC2_ROM_NHET		|\
					PBIST_RAMGROUP_05_AWM1				|\
					PBIST_RAMGROUP_06_DCAN1				|\
					PBIST_RAMGROUP_07_DCAN2				|\
					PBIST_RAMGROUP_08_DMA				|\
					PBIST_RAMGROUP_09_HTU1				|\
					PBIST_RAMGROUP_10_MIBSPI1			|\
					PBIST_RAMGROUP_11_MIBSPI2			|\
					PBIST_RAMGROUP_12_MIBSPI3			|\
					PBIST_RAMGROUP_13_NHET1				|\
					PBIST_RAMGROUP_14_VIM				|\
					PBIST_RAMGROUP_15_RSVD				|\
					PBIST_RAMGROUP_16_RTP				|\
					PBIST_RAMGROUP_17_ATB				|\
					PBIST_RAMGROUP_18_AWM2				|\
					PBIST_RAMGROUP_19_DCAN3				|\
					PBIST_RAMGROUP_20_DCAN4				|\
					PBIST_RAMGROUP_21_HTU2				|\
					PBIST_RAMGROUP_22_MIBSPI4			|\
					PBIST_RAMGROUP_23_MIBSPI5			|\
					PBIST_RAMGROUP_24_NHET2				|\
					PBIST_RAMGROUP_25_FTU				|\
					PBIST_RAMGROUP_26_FRAY_INBUF_OUTBUF	|\
					PBIST_RAMGROUP_27_CPGMAC_STATE_RXADDR |\
					PBIST_RAMGROUP_28_CPGMAC_STAT_FIFO	|\
					PBIST_RAMGROUP_29_L2RAMW			|\
					PBIST_RAMGROUP_30_L2RAMW			|\
					PBIST_RAMGROUP_31_R5_ICACHE			|\
					PBIST_RAMGROUP_32_R5DCACHE			|\
					PBIST_RAMGROUP_33_RSVD				|\
					PBIST_RAMGROUP_34_RSVD				|\
					PBIST_RAMGROUP_35_FRAY_TRBUF_MSGRAM	|\
					PBIST_RAMGROUP_36_CPGMAC_CPPI		|\
					PBIST_RAMGROUP_37_R5_DCACHE_DIRTY	|\
					PBIST_RAMGROUP_38_RSVD)

#define ALL_PBISALGO (PBISTALGO_TRIPLE_READ_SLOW_READ    |\
					PBISTALGO_TRIPLE_READ_FAST_READ      |\
					PBISTALGO_MARCH13N_2PORT             |\
					PBISTALGO_MARCH13N_1PORT             |\
					PBISTALGO_DOWN2_1PORT                |\
					PBISTALGO_DOWN2_2PORT                |\
					PBISTALGO_MARCH_DIS_INC_1PORT    |\
					PBISTALGO_MARCH_DIS_INC_2PORT    |\
					PBISTALGO_MARCH_DIS_DEC_1PORT    |\
					PBISTALGO_MARCH_DIS_DEC_2PORT    |\
					PBISTALGO_MARCH_DIS_INC_1PORT_1  |\
					PBISTALGO_MARCH_DIS_INC_2PORT_1  |\
					PBISTALGO_MARCH_DIS_DEC_1PORT_1  |\
					PBISTALGO_MARCH_DIS_DEC_2PORT_1  |\
					PBISTALGO_MAP_COLUMN_1PORT    	     |\
					PBISTALGO_MAP_COLUMN_2PORT    	     |\
					PBISTALGO_PRECHARGE_1PORT            |\
					PBISTALGO_PRECHARGE_2PORT            |\
					PBISTALGO_FLIP10_1PORT               |\
					PBISTALGO_FLIP10_2PORT               |\
					PBISTALGO_DTXN2_1PORT                |\
					PBISTALGO_DTXN2_2PORT                |\
					PBISTALGO_PMOS_OP_SLICE1_1PORT     |\
					PBISTALGO_PMOS_OP_SLICE1_2PORT     |\
					PBISTALGO_PMOS_OP_SLICE2_2PORT)
#endif

#define ST_BOUNDARY_VALUE ((uint32)(EFC_BOUNDARY_SELF_TEST_ERR | EFC_BOUNDARY_SINGLE_BIT_ERR | \
		EFC_BOUNDARY_INSTR_ERR | EFC_BOUNDARY_AUTOLOAD_ERR))
#define ST_PIN_VALUE      ((uint32)(EFC_PINS_AUTOLOAD_ERR | EFC_PINS_INSTR_ERR | \
		EFC_PINS_SINGLE_BIT_ERR | EFC_PINS_SELF_TEST_ERROR))


#define ROM_CLK_SRC_HCKL_DIV_2  1u
#define ENABLE_OVERRIDE         1u
#define DISABLE_OVERRIDE        0u
#define ALG_RAM_FROM_ROM        3u
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define VBUS_CLK_CYCLES         64u
#endif
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define VBUS_CLK_CYCLES         32u
#endif

/*Bit Fields*/
#define BF_ROM_DIV_START        8u
#define BF_ROM_DIV_LENGTH       2u

/*Bit Fields*/
#define BF_MSTGENA_START              0u
#define BF_MSTGENA_LENGTH             4u
#define ENABLE_MEM_STEST_CTRL         0xau
#define DISABLE_MEM_STEST_CTRL        0x5u

/*Bit Fields*/
#define BF_MINITGENA_START            0u
#define BF_MINITGENA_LENGTH           4u
#define ENABLE_GLOB_MEM_HW_INIT       0xau
#define DISABLE_GLOB_MEM_HW_INIT      0x5u

/*Bit Fields*/
#define BF_MINIDONE_START            8u
#define BF_MINIDONE_LENGTH           1u
#define MEM_HW_INIT_COMPLETE         (uint32)1u
#define MEM_HW_INIT_INCOMPLETE       0u

/*Bit Fields*/
#define BF_EPSF_START   0u
#define BF_EPSF_LENGTH  1u

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
/*Bit Fields*/
#define BF_CCMR4_CMP_ERROR          2u
#define BF_CCMR4_CMP_ERROR_LENGTH   1u
#define BF_CCMR4_STEST_ERROR        31u
#define BF_CCMR4_STEST_ERROR_LENGTH 1u
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define BF_CCMR5_CPUCOMP_ERROR      2u
#define BF_CCMR5_VIMCOMP_ERROR      25u
#define BF_CCMR5_PDCOMP_ERROR       28u
#define BF_CCMR5_INMCOMP_ERROR      26u
#define BF_CCMR5_CMP_ERROR_LENGTH   1u
#define BF_CCMR5_STEST_ERROR        31u
#define BF_CCMR5_STEST_ERROR_LENGTH 1u
#endif

/*Bit Fields*/
#define BF_SERR_EN_START               0u
#define BF_SERR_EN_LEN                 1u
#define SERR_ENABLE                    1u
#define SERR_DISABLE                   0u

/*Bit Fields*/
#define BF_ECC_DETECT_EN_START         0u
#define BF_ECC_DETECT_EN_LEN           4u
#define ECC_DETECT_ENABLE              5u
#define ECC_DETECT_DISABLE             0u

/*Bit Fields*/
#define BF_ECC_WR_EN_START             8u
#define BF_ECC_WR_EN_LEN               1u
#define ECC_WR_ENABLE                  1u
#define ECC_WR_DISABLE                 0u

/*Bit Fields*/
#define BF_EDACEN_START             0u
#define BF_EDACEN_LEN               4u
#define EDAC_ENABLE_DEF             0xau
#define EDAC_DISABLE                5u

#define BF_EDACMODE_START             16u
#define BF_EDACMODE_LEN               4u
#define EDACMODE_DEF                  0xau

#define ESM_GRP1_MASK (1U<<16U)
#define ESM_GRP2_MASK (2U<<16U)
#define ESM_GRP3_MASK (3U<<16U)

/** @enum _SL_Adc_Groups
 *   @brief Alias name for ADC event group
 *
 *   @note These values should be used for API argument @a group
 */
typedef enum _SL_ASC_Groups {
	sl_adcEVTGROUP, sl_adcGROUP1, sl_adcGROUP2
} SL_ADC_Groups;

/** @struct SL_adcData
 *   @brief ADC Conversion data structure
 *
 *   This type is used to pass adc conversion data.
 */
/** @typedef SL_adcData
 *   @brief ADC Data Type SL_adcData
 */
typedef struct _SL_ADC_Data {
	uint32 id; /**< Channel/Pin Id        */
	uint16 value; /**< Conversion data value */
} SL_ADC_Data;



/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "This function is in asm file" */
void	_SL_Restore_IRQ(uint32 val);
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "This function is in asm file" */
uint32	_SL_Disable_IRQ(void);
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "This function is in asm file" */
uint32  _SL_Get_ARM_Mode(void);
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "This function is in asm file" */
void	_SL_Kickoff_STC_execution(void);
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "This function is in asm file" */
void	_SL_Barrier_Data_Access(void);

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "This function is in asm file" */
void _SL_Init_EnableEventExport(void);
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "This function is in asm file" */
void _SL_Init_ECCFlash(void);
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "This function is in asm file" */
void _SL_Init_ECCTCMRAM(void);

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
boolean _SL_SelfTest_adcStartConversion_selChn(sl_adcBASE_t * adc, uint8 channel, uint32 fifo_size, uint32 group);
boolean _SL_SelfTest_adcGetSingleData(sl_adcBASE_t * adc, uint32 group, SL_ADC_Data * data);
boolean _SL_SelfTest_adcIsConversionComplete(const sl_adcBASE_t * adc, uint32 group);
uint16  _SL_Approximate_value(uint16 value, uint16 reference, uint8 difference);
#endif

uint64 _SL_CRC_Calculate(uint64 * start_addr, uint32 count_dw);
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
void _SL_SelfTest_SRAM_RAD (sl_tcramBASE_t * ramBase, SL_SelfTest_Result * rad_stResult);
#endif
boolean SL_FLAG_GET(sint32 flag_id);
void SL_FLAG_CLEAR(sint32 flag_id);
boolean SL_FLAG_SET(sint32 flag_id);
void _SL_HoldNClear_nError(void);


#ifdef LOOP_BACK_ENABLE
#define	CAN_LOOP_BACK_MSG_SIZE 9
#define	CAN_LOOP_BACK_MSG_BOX 34U
void _SL_loadDataPattern(uint16 * pptr, uint16 seed_value, uint32 psize);
#endif

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define CHECK_RANGE_RAM_PTR(x)      ((((uint32)(x)) >= SRAM_START) && ((((uint32)(x)) <= ((SRAM_END-(uint32)sizeof((x)))))))

#endif /* __SL_PRIV_H__ */
