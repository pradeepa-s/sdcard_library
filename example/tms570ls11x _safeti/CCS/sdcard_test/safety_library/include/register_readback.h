/*
 * register_readback.h
 *
 *  Created on: Apr 23, 2013
 *      Author: a0131910
 */

#ifndef REGISTER_READBACK_H_
#define REGISTER_READBACK_H_

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#include <system.h>
#include <esm.h>
#include <sys_vim.h>
#include <dcc.h>
#include <rti.h>
#if defined(_TMS570LS31x_) || defined(_RM48x_) || defined(_TMS570LS12x_) || defined(_RM46x_)
#include <sys_pmm.h>
#endif

#include <sys_selftest.h>

#include <reg_bitdefn_esm.h>
#include <reg_bitdefn_tcmram.h>
#include <reg_bitdefn_vim.h>
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#include <HL_system.h>
#include <HL_esm.h>
#include <HL_sys_vim.h>
#include <HL_dcc.h>
#include <HL_rti.h>

#include <HL_sys_pmm.h>
#include <HL_sys_selftest.h>

#include <reg_bitdefn_esm.h>
#include <reg_bitdefn_tcmram.h>
#include <reg_bitdefn_vim.h>
#endif

 /** \defgroup grp_module_ids Module IDs for various register sets in the SOC.
 *
 *  Following macros are to be used for by the register compare API to compare
 *  the register configuration against a golder value
 *
 *  NOTE: This feature is not available in this release
 */
/**@{*/
typedef enum _ModuleId {
    INVALID             = 0,    /**< Reserved, do not use                               */
    _NOT_SUPPORTED_1    = 1,    /**< Reserved, do not use                               */
    PMM					= 2,    /**< Power Management Module - Power State Controller   */
    DCC1		        = 3,    /**< DCC1												                        */
    DCC2		        = 4,    /**< DCC2                                               */
    SYSTEM              = 5,    /**< System (SYS) Module                                */
    ESM                 = 6,    /**< Error Signaling Module                             */
    CCMR4               = 7,    /**< Cortex-R4 Compare Module                            */
    TCMFLASH            = 8,    /**< On-Chip Flash TCM (ATCM)                           */
    FEE                 = 9,    /**< Flash EEPROM Emulation Module                      */
    SRAM                = 10,   /**< SRAM Modules (BTCM & Peripheral SRAMs)             */
    L2L3                = 11,   /**< Level2 & Level3 Interconnect Subsystem             */
    EFC               	= 12,   /**< EFuse Module                                       */
    OTP                 = 13,   /**< OTP Module                                         */
    IOMM                = 14,   /**< IO Multiplex Module                                */
    VIM                 = 15,   /**< Vectored Interrupt Module                          */
    RTI                 = 16,   /**< Real Time Interrupt Module (Timer)                 */
    PBIST				= 17,   /**< PBIST                                              */
    STC			        = 18    /**< Self Test Controller                               */
} ModuleId;
/**@}*/

typedef enum _CfgRegRdBk_Mode {
	BASELINE 	= 0xAB,
	COMPARE 	= 0xCD,
	REBASELINE 	= 0xEF
}CfgRegRdBk_Mode;

/** \defgroup grp_enhancements Future enhancements - DO NOT USE THESE MACROS
 *
 *  Following macros are reserved for enhancements.
 *
 */
/**@{*/
#if defined(_TMS570LS31x_) || defined(_RM48x_) || defined(_TMS570LS12x_) || defined(_RM46x_)
#define PMM_ENABLED          1u
#endif
#if defined(_RM42x_) || defined(TMS570LS04x)
#define PMM_ENABLED          0u
#endif

#define DCC1_ENABLED         1u

#if defined(_TMS570LS31x_) || defined(_RM48x_) || defined(_TMS570LS12x_) || defined(_RM46x_)
#define DCC2_ENABLED         1u
#endif
#if defined(_RM42x_) || defined(TMS570LS04x)
#define DCC2_ENABLED         0u
#endif

#define SYSTEM_ENABLED       1u
#define ESM_ENABLED          1u
#define CCMR4_ENABLED        1u
#define TCMFLASH_ENABLED     1u
#define FEE_ENABLED          0u
#define SRAM_ENABLED         1u
#define L2L3_ENABLED         0u
#define EFC_ENABLED          1u
#define OTP_ENABLED          0u
#define IOMM_ENABLED         0u
#define VIM_ENABLED          1u
#define RTI_ENABLED          1u
#define PBIST_ENABLED        1u
#define STC_ENABLED          1u
/**@}*/

/*SAFETYMCUSW 74 S MR: 18.4 <INSPECTED> "Union declared only for allocating maximum space, no union operations made" */
typedef union _CfgReg{
#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)
#if PMM_ENABLED
	pmm_config_reg_t 		pmmCfgReg;
#endif
#endif
#if DCC1_ENABLED
	dcc_config_reg_t		dccCfgReg;
#endif
#if DCC2_ENABLED
	system_config_reg_t		systemCfgReg;
#endif
#if ESM_ENABLED
	esm_config_reg_t		esmCfgReg;
#endif
#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)
#if CCMR4_ENABLED
	ccmr4_config_reg_t		cpur4CfgReg;
#endif
#endif
#if TCMFLASH_ENABLED
	tcmflash_config_reg_t	tcmflashCfgReg;
#endif
#if FEE_ENABLED
	fee_config_reg_t		feeCfgReg;
#endif
#if SRAM_ENABLED
	sram_config_reg_t		sramCfgReg;
#endif
#if L2L3_ENABLED
	l2l3_config_reg_t		l2l3CfgReg;
#endif
#if EFC_ENABLED
	efc_config_reg_t		efuseCfgReg;
#endif
#if OTP_ENABLED
	otp_config_reg_t		otpCfgReg;
#endif
#if IOMM_ENABLED
	iomm_config_reg_t		iommCfgReg;
#endif
#if VIM_ENABLED
	vim_config_reg_t		vimCfgReg;
#endif
#if RTI_ENABLED
	rti_config_reg_t		rtiCfgReg;
#endif
} CfgReg;


/*SAFETYMCUSW 298 S MR:- <INSPECTED> "Reason -  This changes during runtime, for different modules"*/
typedef void (*getConfiReg_t)(CfgReg* cfgReg, config_value_type_t mode);

/*Common structure for all modules where the details for the module are saved in*/
typedef struct _CfgRegDetails {
	void 	*baseline;
	void 	*current;
	uint32 	size;
	getConfiReg_t getConfiReg;
}CfgRegDetails;




#define NOT_BASELINED 0xDEADBEEFu





void 	Reg_getCfgRegDetails(ModuleId module, CfgRegDetails *details);
boolean Reg_isBaselined(void *baseline);
/**
 * @brief       Function is used to for the configuration register read back and compare functionalities. This function can be
 * used in 3 modes : BASELINE, REBASELINE & COMPARE. The COMPARE mode is used for read back compare and the BASELINE, REBASELINE values
 * are used to update the values which are used for future comparisons. For each module, there are a set of values called 'baseline'
 * values which are used for comparison.  During comparison (using COMPARE mode) the values in the 'baseline' is compared against
 * the current set of configuration registers for that module. The other 2 modes are provided so that the user can update the
 * configuration registers with the default values(BASELINE) or the current values(REBASELINE).
 *
 * In the BASELINE mode, the function selects the value from HALCoGen as the baseline value. In the REBASELINE mode, the current
 * set of values of the configuration register is selected as the baseline value. In the COMPARE mode, the baseline values are
 * compared against the current value of the configuration register. In case the function is called with COMPARE without prior
 * baselining, a REBASELINE is done before comparison.
 *
 * @image html  SL_Read_Compare.jpg
 * @image rtf  SL_Read_Compare.jpg
 * @image latex  SL_Read_Compare.eps
 *
 *
 * @param [in]  module - Name of the module used for the test
 * @param [in] mode -	The mode of the read back test.BASELINE/COMPARE/REBASELINE are used to baseline with the configuration register
 * with the HalCoGen GUI values, compare the current values with the baselined ones and to rebasline with the current values.
 *
 * The current implementation only supports the following modules:
 *
 * PMM
 * DCC1
 * DCC2
 * SYSTEM
 * ESM
 * CCMR4
 * TCMFLASH
 * SRAM
 * EFC
 * VIM
 * RTI
 * PBIST
 * STC
 *
 * @return      TRUE if function executed successfully for BASELINE/REBASLINE, and comparison passes in COMPARE else FALSE.
 *
 * Example Usage:
 * @code
 *      boolean retVal = SL_Read_Compare(PMM,COMPARE);
 * @endcode
 *
 * @entrymode   Any privilege mode
 * @exitmode    Same as at entry
 *
 * Note: Calling this function with COMPARE before baselining, baselines first and then compares
 *
 * @seealso     Types : @ref SL_ModuleId, @ref SL_CfgRegRdBk_Mode
 *
 * @destag		DES_28
 * @reqtag		SL_SR5,SL_SR6,SL_SR14,SL_SR15,SL_SR22,SL_SR23,SL_SR25,SL_SR26,SLSR27,SL_SR31,SL_SR39,SL_SR48,SL_SR49,SL_SR55,SL_SR56,SL_SR67,SL_SR68,SL_SR75,SL_SR78,SL_SR84,SL_SR87,SL_SR94,SL_SR95,SL_SR103,SL_SR112,SL_SR126,SL_SR136,SL_SR146,SL_SR150,SL_SR154,SL_SR158,SL_SR167,SL_SR179,SL_SR183,SL_SR192,SL_SR205
 */
boolean Reg_Read_Compare(ModuleId module, CfgRegRdBk_Mode mode);



#endif /* REGISTER_READBACK_H_ */
