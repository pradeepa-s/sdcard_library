/*
 * register_readback.c
 *
 *  Created on: Apr 23, 2013
 *      Author: a0131910
 */

#include "register_readback.h"

#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)
#if PMM_ENABLED
static pmm_config_reg_t		pmmBaseline={NOT_BASELINED};
#endif
#endif
#if DCC1_ENABLED
static dcc_config_reg_t		dcc1Baseline={NOT_BASELINED};
#endif
#if DCC2_ENABLED
static dcc_config_reg_t		dcc2Baseline={NOT_BASELINED};
#endif
#if SYSTEM_ENABLED
static system_config_reg_t	systemBaseline={NOT_BASELINED};
#endif
#if ESM_ENABLED
static esm_config_reg_t		esmBaseline={NOT_BASELINED};
#endif

#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)
#if CCMR4_ENABLED
static ccmr4_config_reg_t	ccmr4Baseline={NOT_BASELINED};
#endif
#endif
#if TCMFLASH_ENABLED
static tcmflash_config_reg_t	tcmflashBaseline={NOT_BASELINED};
#endif
#if FEE_ENABLED
static fee_config_reg_t		feeBaseline={NOT_BASELINED};
#endif
#if SRAM_ENABLED
static sram_config_reg_t	sramBaseline={NOT_BASELINED};
#endif
#if L2L3_ENABLED
static l2l3_config_reg_t	l2l3Baseline={NOT_BASELINED};
#endif
#if EFC_ENABLED
static efc_config_reg_t		efcBaseline={NOT_BASELINED};
#endif
#if OTP_ENABLED
static otp_config_reg_t		otpBaseline={NOT_BASELINED};
#endif
#if IOMM_ENABLED
static iomm_config_reg_t	iommBaseline={NOT_BASELINED};
#endif
#if VIM_ENABLED
static vim_config_reg_t		vimBaseline={NOT_BASELINED};
#endif
#if RTI_ENABLED
static rti_config_reg_t		rtiBaseline={NOT_BASELINED};
#endif
#if PBIST_ENABLED
static pbist_config_reg_t	pbistBaseline={NOT_BASELINED};
#endif
#if STC_ENABLED
static stc_config_reg_t		stcBaseline={NOT_BASELINED};
#endif
/* Variable to store the temporary current values */
static CfgReg curRegDetails;



/** @fn void getCntlRegDetails(modules module,CNTLREGDETAILS *details)
*   @brief 	A common point for all modules. Gets all the details for the corresponding Control register
*   including the function pointer used to fill up the details, the baselined values and the current values
*   Module is the module for which the details are filled up
*/
void Reg_getCfgRegDetails(ModuleId module, CfgRegDetails *details)
{
	details->current=&curRegDetails;
	switch(module)
	{
#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)
#if PMM_ENABLED
		case PMM:		details->baseline=&pmmBaseline;
						details->size = sizeof(pmm_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)pmmGetConfigValue;
						break;
#endif
#endif
#if DCC1_ENABLED
		case DCC1:		details->baseline=&dcc1Baseline;
						details->size = sizeof(dcc_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)dcc1GetConfigValue;
						break;
#endif
#if DCC2_ENABLED
		case DCC2:		details->baseline=&dcc2Baseline;
						details->size = sizeof(dcc_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)dcc2GetConfigValue;
						break;
#endif
/*Depends on HALCoGen function call - Not implemented in the current version for these devices*/
#if (SYSTEM_ENABLED  && !(defined(_RM42x_) || defined(_TMS570LS04x_)))
		case SYSTEM:
						details->baseline=&systemBaseline;
						details->size = sizeof(system_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)systemGetConfigValue;
						break;
#endif
#if ESM_ENABLED
		case ESM:		details->baseline=&esmBaseline;
						details->size = sizeof(esm_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)esmGetConfigValue;
						break;
#endif
#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)
#if CCMR4_ENABLED
		case CCMR4:		details->baseline=&ccmr4Baseline;
						details->size = sizeof(ccmr4_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)ccmr4GetConfigValue;
						break;
#endif
#endif
/*Depends on HALCoGen function call - Not implemented in the current version for these devices*/
#if (TCMFLASH_ENABLED && !(defined(_RM42x_) || defined(_TMS570LS04x_)))
		case TCMFLASH:	details->baseline=&tcmflashBaseline;
						details->size = sizeof(tcmflash_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)tcmflashGetConfigValue;
						break;
#endif
#if FEE_ENABLED
		case FEE:		details->baseline=&feeBaseline;
						details->size = sizeof(fee_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)feeGetConfigValue;
						break;
#endif
/*Depends on HALCoGen function call - Not implemented in the current version for these devices*/
#if (SRAM_ENABLED && !(defined(_RM42x_) || defined(_TMS570LS04x_)))
		case SRAM:		details->baseline=&sramBaseline;
						details->size = sizeof(sram_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)sramGetConfigValue;
						break;
#endif
#if L2L3_ENABLED
		case L2L3:		details->baseline=&l2l3Baseline;
						details->size = sizeof(l2l3_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)l2l3GetConfigValue;
						break;
#endif
#if EFC_ENABLED
		case EFC:		details->baseline=&efcBaseline;
						details->size = sizeof(efc_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)efcGetConfigValue;
						break;
#endif
#if OTP_ENABLED
		case OTP:		details->baseline=&otpBaseline;
						details->size = sizeof(otp_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)otpGetConfigValue;
						break;
#endif
#if IOMM_ENABLED
		case IOMM:		details->baseline=&iommBaseline;
						details->size = sizeof(iomm_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)iommGetConfigValue;
						break;
#endif
#if VIM_ENABLED
		case VIM:		details->baseline=&vimBaseline;
						details->size = sizeof(vim_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)vimGetConfigValue;
						break;
#endif

#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)
#if RTI_ENABLED
		case RTI:		details->baseline=&rtiBaseline;
						details->size = sizeof(rti_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)rtiGetConfigValue;
						break;
#endif
#endif
#if PBIST_ENABLED
		case PBIST:		details->baseline=&pbistBaseline;
						details->size = sizeof(pbist_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)pbistGetConfigValue;
						break;
#endif
#if STC_ENABLED
		case STC:		details->baseline=&stcBaseline;
						details->size = sizeof(stc_config_reg_t);
						/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
						details->getConfiReg=(getConfiReg_t)stcGetConfigValue;
						break;
#endif
		default:		details->current=NULL;
						details->getConfiReg=NULL;
						break;
	}
}

/* Function to check if values are baseline atleast once */
boolean Reg_isBaselined(void * baseline)
{
	/*SAFETYMCUSW 94 S MR:11.1,11.2,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
	/*SAFETYMCUSW 95 S MR:11.1,11.4- <INSPECTED> "Reason -  Implementation constraint"*/
	if( NOT_BASELINED == *((uint32 *)baseline)){
		return FALSE;
	}else{
		return TRUE;
	}
}

boolean Reg_Read_Compare(ModuleId module, CfgRegRdBk_Mode mode)
{
/* Making sure the API is not run for disabled modules */
#if (!PMM_ENABLED)
	if(module == PMM){
		return FALSE;
	}
#endif
#if (!DCC1_ENABLED)
	if(module == DCC1){
		return FALSE;
	}
#endif
#if (!DCC2_ENABLED)
	if(module == DCC2){
		return FALSE;
	}
#endif
#if (!SYSTEM_ENABLED)
	if(module == SYSTEM){
		return FALSE;
	}
#endif
#if (!ESM_ENABLED)
	if(module == ESM){
		return FALSE;
	}
#endif
#if (!CCMR4_ENABLED)
	if(module == CCMR4){
		return FALSE;
	}
#endif
#if (!TCMFLASH_ENABLED)
	if(module == TCMFLASH){
		return FALSE;
	}
#endif
#if (!FEE_ENABLED)
	if(module == FEE){
		return FALSE;
	}
#endif
#if (!SRAM_ENABLED)
	if(module == SRAM){
		return FALSE;
	}
#endif
#if (!L2L3_ENABLED)
	if(module == L2L3){
		return FALSE;
	}
#endif
#if (!EFC_ENABLED)
	if(module == EFC){
		return FALSE;
	}
#endif
#if (!OTP_ENABLED)
	if(module == OTP){
		return FALSE;
	}
#endif
#if (!IOMM_ENABLED)
	if(module == IOMM){
		return FALSE;
	}
#endif
#if (!VIM_ENABLED)
	if(module == VIM){
		return FALSE;
	}
#endif
#if (!RTI_ENABLED)
	if(module == RTI){
		return FALSE;
	}
#endif
#if (!PBIST_ENABLED)
	if(module == PBIST){
		return FALSE;
	}
#endif
#if (!STC_ENABLED)
	if(module == STC){
		return FALSE;
	}
#endif
	CfgRegDetails details;
	Reg_getCfgRegDetails(module,&details);
	/* if getCfgRegDetails did not succeed, return FALSE, else proceed */
	if((details.getConfiReg==NULL) || (details.baseline==NULL) || (details.current==NULL))
	{
		return FALSE;
	}else{
		if(BASELINE == mode)
		{
			/*SAFETYMCUSW 45 D MR: 21.1 <INSPECTED> "Reason - Checking done above" */
			details.getConfiReg(details.baseline, InitialValue);
			return TRUE;
		}
		else if(COMPARE == mode)
		{
			/* If the module is set for comparison before the initial baselining, it is baselined */
			if(!Reg_isBaselined(details.baseline))
			{
				details.getConfiReg(details.baseline, CurrentValue);
			}
			/*SAFETYMCUSW 45 D MR: 21.1 <INSPECTED> "Reason - Checking done above" */
			memset(details.current,0,sizeof(CfgReg));
			details.getConfiReg(details.current,CurrentValue);
			/*SAFETYMCUSW 45 D MR: 21.1 <INSPECTED> "Reason - Checking done above" */
			if(memcmp(details.baseline,details.current,details.size)==0){
				return TRUE;
			}
			else{
				return FALSE;
			}
		}
		else
		{
			/*Rebaseline*/
			details.getConfiReg(details.baseline,CurrentValue);
			return TRUE;
		}
	}
}



