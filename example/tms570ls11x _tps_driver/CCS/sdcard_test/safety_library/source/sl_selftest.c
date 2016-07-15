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
 * @file        sl_selftest.c
 * @version     2.2.0
 * @brief       File contains the Self Test APIs
 *
 * @details The file implements the  api's for carrying out selftests and fault injection tests on/n
 * all the peripherals (includes Safety island peripherals also).The utility functions or/n
 * support functions which are used by this file are defined in either sl_misc.c or/n
 * sl_priv.c
 */

/* Comments regarding various justificatios for deviation from MISRA-C coding guidelines
 * are provided at the bottom of the file */

#include <sl_api.h>
#include <sl_priv.h>
#define FLASH_DIAG_MODE_ENABLED 0x5U


/*SAFETYMCUSW 69 S MR:3.4 <APPROVED> Comment_1*/
#ifdef __TI_COMPILER_VERSION__
/* 16 byte alignment is required to ensure that the starting address is always an even bank */
#pragma DATA_ALIGN(sramEccTestBuff, 16);
#endif
#ifdef __IAR_SYSTEMS_ICC__
#pragma data_alignment=16
#endif
volatile uint64 sramEccTestBuff[4] = {0x0u};

/*SAFETYMCUSW 69 S MR:3.4 <APPROVED> Comment_2*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_SRAM(SL_SelfTestType testType, boolean bMode, SL_SelfTest_Result * sram_stResult)
{
    volatile boolean _sl_fault_injection = FALSE;
    boolean retVal = FALSE;
    volatile uint32 ramRead, regBkupIntEnaSet, regBckupErrInfulence;

	volatile uint64* eccB1;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
	volatile uint32 regBkupRamTh1, regBkupRamTh2;
	volatile uint64* eccB2;
#endif
	volatile uint32 irqStatus;
	register uint32 tempVal;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    register uint64 ramread64;
    uint32 ram1uerraddr,ram2uerraddr;
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif


#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    /* If fault inject set global variable to flag to the ESM handler that it is a fault injection */
    if((SRAM_ECC_2BIT_FAULT_INJECT == testType)||
        (SRAM_ECC_1BIT_FAULT_INJECTION == testType)||
        (SRAM_ECC_ERROR_PROFILING_FAULT_INJECT == testType)){
        _sl_fault_injection=TRUE;
    }
#endif
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in range */
    /*LDRA_INSPECTWINDOW 50 */
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if ((_SELFTEST_SRAMECC_MIN > testType) || (_SELFTEST_SRAMECC_MAX < testType) ||
            !CHECK_RANGE_RAM_PTR(sram_stResult)) {
        SL_Log_Error(FUNC_ID_ST_SRAM, ERR_TYPE_PARAM, 0U);
        return(FALSE);
    }
#endif
#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /* Error if not in privilege mode */
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_SRAM, ERR_TYPE_ENTRY_CON, 2U);
        return(FALSE);
    }
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    /* For MISRA C fix for LDRA - 408S*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    uint32 tmpRAMCTRL1=sl_tcram1REG->RAMCTRL;
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    uint32 tmpRAMCTRL2=sl_tcram2REG->RAMCTRL;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        uint32 tmpRAMCTRL=sl_l2ramwREG->RAMCTRL;
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    /* Error if ECC is not enabled */
    if ((SRAM_PAR_ADDR_CTRL_SELF_TEST == testType)
            && (((SRAM_ADDR_PARITY_DISABLED) == BF_GET(tmpRAMCTRL1, TCRAM_RAMCTRL_ADDRPARDIS_START, TCRAM_RAMCTRL_ADDRPARDIS_LEN))
                || ((SRAM_ADDR_PARITY_DISABLED) == BF_GET(tmpRAMCTRL2, TCRAM_RAMCTRL_ADDRPARDIS_START, TCRAM_RAMCTRL_ADDRPARDIS_LEN)))) {
        SL_Log_Error(FUNC_ID_ST_SRAM, ERR_TYPE_ENTRY_CON, 1U);
        return(FALSE);
    }
    if(((SRAM_ECC_ERROR_FORCING_1BIT == testType)||
            (SRAM_ECC_ERROR_FORCING_2BIT == testType)||
            (SRAM_RADECODE_DIAGNOSTICS == testType)) &&
             (((SRAM_ECC_DISABLED) == BF_GET(tmpRAMCTRL1, TCRAM_RAMCTRL_ECC_DET_EN_START, TCRAM_RAMCTRL_ECC_DET_EN_LEN))
                 ||((SRAM_ECC_DISABLED) == BF_GET(tmpRAMCTRL2, TCRAM_RAMCTRL_ECC_DET_EN_START, TCRAM_RAMCTRL_ECC_DET_EN_LEN)))) {
        SL_Log_Error(FUNC_ID_ST_SRAM, ERR_TYPE_ENTRY_CON, 1U);
        return(FALSE);
    }

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_SRAM, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
    /* Error if ECC is not enabled */
    if(((SRAM_ECC_ERROR_FORCING_1BIT == testType)||
            (SRAM_ECC_ERROR_FORCING_2BIT == testType)) &&
             ((SRAM_ECC_DISABLED) == BF_GET(tmpRAMCTRL, L2RAM_RAMCTRL_ECC_DETECT_EN_START, L2RAM_RAMCTRL_ECC_DETECT_EN_LENGTH)))
    {
        SL_Log_Error(FUNC_ID_ST_SRAM, ERR_TYPE_ENTRY_CON, 1U);
        return(FALSE);
    }

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active())
    {
        SL_Log_Error(FUNC_ID_ST_SRAM, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }
#endif

    retVal = TRUE;      /* Function executed successfully */

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    regBkupRamTh1 = sl_tcram1REG->RAMTHRESHOLD;
    regBkupRamTh2 = sl_tcram2REG->RAMTHRESHOLD;
#endif

    /* False warning */
    switch(testType) {

case SRAM_ECC_ERROR_FORCING_2BIT:
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    case SRAM_ECC_2BIT_FAULT_INJECT:
        /* Read-unlock the error addresses for subsequent captures */
        ramRead= sl_tcram1REG->RAMUERRADDR;
        ramRead= sl_tcram2REG->RAMUERRADDR;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /* Clear any previous diagnostic errors status bits [22,21,20,19,12,11,10, 4] - required before trigger */
        BIT_SET(sl_l2ramwREG->RAMERRSTATUS, L2RAM_RAMERRSTATUS_CLRALL);
#endif

        /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_6*/
        sramEccTestBuff[2] = 0U;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_6*/
        sramEccTestBuff[3] = 0U;
#endif
        /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_7*/
        /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> Comment_8*/
        /*SAFETYMCUSW 344 S MR:11.5 <APPROVED> Comment_9*/
        eccB1 = &sramEccTestBuff[2];
        eccB1 += (0x00400000u/sizeof(uint64));
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_7*/
        /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> Comment_8*/
        /*SAFETYMCUSW 344 S MR:11.5 <APPROVED> Comment_9*/
        eccB2 = &sramEccTestBuff[3];
        eccB2 += (0x00400000u/sizeof(uint64));

        /* Enable Writes to ECC RAM */
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        BIT_SET(sl_tcram1REG->RAMCTRL,TCRAM_RAMCTRL_ECCWREN);

        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        BIT_SET(sl_tcram2REG->RAMCTRL,TCRAM_RAMCTRL_ECCWREN);

        /* cause a 2-bit ECC error */
        /* Flip 2 bits, this flip in ECC works as 2 bit error when the data part used for ECC is zero*/
        /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_7*/
        *eccB1 ^= TCRAM_SYN_2BIT_DATA_ECC;
        _SL_Barrier_Data_Access();
        /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_7*/
        *eccB2 ^= TCRAM_SYN_2BIT_DATA_ECC;
        _SL_Barrier_Data_Access();
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)

        /* Write test vectors */

		sl_l2ramwREG->DIAGDATAVECTOR_H = BF_GET((sramEccTestBuff[2]), 32, 32);
		sl_l2ramwREG->DIAGDATAVECTOR_L = BF_GET((sramEccTestBuff[2]), 0, 32);
        sl_l2ramwREG->DIAG_ECC = (*eccB1) ^ L2RAM_SYN_2BIT_DATA_ECC;

#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        ramRead = sl_tcram1REG->RAMCTRL;
        /*flag is set to indicate the current test which is ongoing and
        These flags are used in the sl_esm.c so as to mask the esm callback*/
        if(SRAM_ECC_2BIT_FAULT_INJECT == testType)
        {
        	SL_FLAG_SET(SRAM_ECC_2BIT_FAULT_INJECT);
        }
        else
#endif
        {
        	SL_FLAG_SET(SRAM_ECC_ERROR_FORCING_2BIT);
        }

#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        _SL_Barrier_Data_Access();

        /* Set the self test flag for a self test to indicate the esm handler that this is done as a part of selftest */
        /* read from location with 2-bit ECC error this will cause a data abort to be generated */
        /*SAFETYMCUSW 446 S MR:10.1 <APPROVED> Comment_11*/
        ramread64 = sramEccTestBuff[2];
        _SL_Barrier_Data_Access();
        /* Restore ctrl registers */
        sl_tcram1REG->RAMCTRL &= ~TCRAM_RAMCTRL_ECCWREN;

        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        ramRead = sl_tcram2REG->RAMCTRL;
        /* Set the self test flag for a self test to indicate the esm handler that this is done as a part of selftest */
        /*SAFETYMCUSW 446 S MR:10.1 <APPROVED> Comment_11*/
        ramread64 = sramEccTestBuff[3];
        _SL_Barrier_Data_Access();
        /* Restore ctrl registers */
        sl_tcram2REG->RAMCTRL &= ~TCRAM_RAMCTRL_ECCWREN;

        if(SRAM_ECC_ERROR_FORCING_2BIT == testType){
            ram1uerraddr=sl_tcram1REG->RAMUERRADDR;
            ram2uerraddr=sl_tcram2REG->RAMUERRADDR;
            /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
            status functions to get the pass/failure information*/
            /* Check the error status on both banks */
    		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
            /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> Comment_8*/
            /*SAFETYMCUSW 344 S MR:11.5 <APPROVED> Comment_9*/
    		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_1"*/
            /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> Comment_8*/
            /*SAFETYMCUSW 344 S MR:11.5 <APPROVED> Comment_9*/
            /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
            /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
            if ((((uint32)&sramEccTestBuff[2] & TCRAM_RAMUERRADDR_UNC_ERRADD) == ram1uerraddr)
            		&& (((uint32)&sramEccTestBuff[3] & TCRAM_RAMUERRADDR_UNC_ERRADD) == (ram2uerraddr)) &&
                    ((1 << ESM_G3ERR_B1TCM_ECC_UNCORR) == (sl_esmREG->SR1[2] & (1u << ESM_G3ERR_B1TCM_ECC_UNCORR)))&&
                    ((1 << ESM_G3ERR_B0TCM_ECC_UNCORR) == (sl_esmREG->SR1[2] & (1u << ESM_G3ERR_B0TCM_ECC_UNCORR)))) {
                *sram_stResult = ST_PASS;
            } else {
                *sram_stResult = ST_FAIL;
            }
            /* Clear nError */
            _SL_HoldNClear_nError();
            /* Clear the ESM Status */
            sl_esmREG->SR1[2] = ((1u << ESM_G3ERR_B0TCM_ECC_UNCORR) | (1u << ESM_G3ERR_B1TCM_ECC_UNCORR));
            /* Clear double bit error anyways */
            sl_tcram1REG->RAMERRSTATUS |= TCRAM_RAMERRSTATUS_DER;
            sl_tcram2REG->RAMERRSTATUS |= TCRAM_RAMERRSTATUS_DER;
            /* Compute uncorrupted ECC */
            sramEccTestBuff[2] = 0u;
            sramEccTestBuff[3] = 0u;
        }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)

        /* Equality check test */
        sl_l2ramwREG->RAMTEST = (uint32) 0x0000008Au; /* 1->Trigger, 8->Eq., test mode, A->Test enable */
        BIT_SET(sl_l2ramwREG->RAMTEST, L2RAM_RAMTEST_TST_TRIGGER);

        /* Wait for the test to complete */
        /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
        while (L2RAM_RAMTEST_TST_TRIGGER == (sl_l2ramwREG->RAMTEST & L2RAM_RAMTEST_TST_TRIGGER));

        if(SRAM_ECC_ERROR_FORCING_2BIT == testType)
        {
            /* Check if the diagnostic error status bits have been set */
            if((1 << ESM_G2ERR_L2RAMW_UNCORR_B) == ((sl_esmREG->SSR2 & (1u << ESM_G2ERR_L2RAMW_UNCORR_B))) &&
                    ((sl_l2ramwREG->RAMERRSTATUS & (1 << L2RAM_RAMERRSTATUS_DRDE)) == (1 << L2RAM_RAMERRSTATUS_DRDE)) &&
                    ((sl_l2ramwREG->RAMERRSTATUS & (1 << L2RAM_RAMERRSTATUS_DWDE)) == (1 << L2RAM_RAMERRSTATUS_DWDE)))
            {
                *sram_stResult = ST_PASS;
            }
            else
            {
                *sram_stResult = ST_FAIL;
            }
            /* Clear any previous diagnostic errors status bits [22,21,20,19,12,11,10, 4] - required before trigger */
            BIT_SET(sl_l2ramwREG->RAMERRSTATUS, L2RAM_RAMERRSTATUS_CLRALL);
            _SL_HoldNClear_nError(); /* Clear nError */
        }

#endif
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*clear the flags which indicate tests ongoing*/
		if(SRAM_ECC_2BIT_FAULT_INJECT == testType)
		{
		   /*clear the flags which indicate tests ongoing*/
			SL_FLAG_CLEAR(SRAM_ECC_2BIT_FAULT_INJECT);
		}
		else
#endif
		{
		   /*clear the flags which indicate tests ongoing*/
			SL_FLAG_CLEAR(SRAM_ECC_ERROR_FORCING_2BIT);
		}
        break;

    case SRAM_ECC_ERROR_FORCING_1BIT:
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    case SRAM_ECC_1BIT_FAULT_INJECTION:
    case SRAM_ECC_ERROR_PROFILING:
    case SRAM_ECC_ERROR_PROFILING_FAULT_INJECT:
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /* Clear any previous diagnostic errors status bits [22,21,20,19,12,11,10, 4] - required before trigger */
        BIT_SET(sl_l2ramwREG->RAMERRSTATUS, L2RAM_RAMERRSTATUS_CLRALL);
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
        status functions to get the pass/failure information*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBkupIntEnaSet = sl_esmREG->IESR1;
        regBckupErrInfulence = sl_esmREG->EEPAPR1;
         if((SRAM_ECC_ERROR_FORCING_1BIT == testType)||(SRAM_ECC_ERROR_PROFILING == testType)){
             sl_esmREG->IECR1 = GET_ESM_BIT_NUM(ESM_G1ERR_B0TCM_CORRERR)|
                                 GET_ESM_BIT_NUM(ESM_G1ERR_B1TCM_CORRERR);
             sl_esmREG->DEPAPR1 = GET_ESM_BIT_NUM(ESM_G1ERR_B0TCM_CORRERR)|
                                 GET_ESM_BIT_NUM(ESM_G1ERR_B1TCM_CORRERR);

         }
#endif

        sramEccTestBuff[0] = 0U;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        sramEccTestBuff[1] = 0U;
        uint8 testIterations=1u;
#endif
        /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> Comment_8*/
        /*SAFETYMCUSW 344 S MR:11.5 <APPROVED> Comment_9*/
		eccB1 = &sramEccTestBuff[0];
        eccB1 += (0x00400000u/sizeof(uint64));

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> Comment_8*/
        /*SAFETYMCUSW 344 S MR:11.5 <APPROVED> Comment_9*/
		eccB2 = &sramEccTestBuff[1];
        eccB2 += (0x00400000u/sizeof(uint64));

        /* Enable Writes to ECC RAM */
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        BIT_SET(sl_tcram1REG->RAMCTRL,TCRAM_RAMCTRL_ECCWREN);

        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        BIT_SET(sl_tcram2REG->RAMCTRL,TCRAM_RAMCTRL_ECCWREN);

        regBkupRamTh1 = sl_tcram1REG->RAMTHRESHOLD;
        regBkupRamTh2 = sl_tcram2REG->RAMTHRESHOLD;
        /* Clear RAMOCCUR before setting value */
        sl_tcram1REG->RAMOCCUR=0u;
        sl_tcram2REG->RAMOCCUR=0u;
        if((SRAM_ECC_ERROR_PROFILING_FAULT_INJECT == testType)||(SRAM_ECC_ERROR_PROFILING == testType)){
            sl_tcram1REG->RAMTHRESHOLD = TCRAM_RAMTHRESHOLD_FOR_TEST;
            sl_tcram2REG->RAMTHRESHOLD = TCRAM_RAMTHRESHOLD_FOR_TEST;
            testIterations=(uint8)TCRAM_RAMTHRESHOLD_FOR_TEST;
        }else{
            sl_tcram1REG->RAMTHRESHOLD = 1u;
            sl_tcram2REG->RAMTHRESHOLD = 1u;
            testIterations=1u;
        }

        uint8 iTestCycle=0u;
        *sram_stResult = ST_PASS;
        for(iTestCycle=1u;iTestCycle<=testIterations;iTestCycle++){
            _SL_Barrier_Data_Access();
            /* cause a 1-bit ECC error */
            /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_7*/
            *eccB1 ^= TCRAM_SYN_1BIT_DATA_ECC;
            /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_7*/
            *eccB2 ^= TCRAM_SYN_1BIT_DATA_ECC;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)

        /* Write test vectors */
		sl_l2ramwREG->DIAGDATAVECTOR_H = BF_GET((sramEccTestBuff[0]), 32, 32);
		sl_l2ramwREG->DIAGDATAVECTOR_L = BF_GET((sramEccTestBuff[0]), 0, 32);
        sl_l2ramwREG->DIAG_ECC = (*eccB1) ^ L2RAM_SYN_1BIT_DATA_ECC;

#endif

#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
            /* read from location with to be generate error */

            _SL_Barrier_Data_Access();
            /*SAFETYMCUSW 446 S MR:10.1 <APPROVED> Comment_11*/
            ramread64 = sramEccTestBuff[0];
            /*SAFETYMCUSW 446 S MR:10.1 <APPROVED> Comment_11*/
            _SL_Barrier_Data_Access();
            ramread64 = sramEccTestBuff[1];
            _SL_Barrier_Data_Access();
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
            if((sl_tcram1REG->RAMOCCUR == (iTestCycle % testIterations))
               && (sl_tcram2REG->RAMOCCUR == (iTestCycle % testIterations))){
                /*Testcase is still in ST_PASS as long as RAMOCCUR gets incremented*/
                *sram_stResult = ST_PASS;
            }else{
                /*Testcase fails if RAMOCCUR does not get incremented*/
                *sram_stResult = ST_FAIL;
                break;
            }
            sramEccTestBuff[0] = 0u;
            sramEccTestBuff[1] = 0u;
        }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)

        /* Equality check in compare */
        sl_l2ramwREG->RAMTEST = (uint32) 0x0000008Au; /* 1->Trigger, 8->Eq., test mode, A->Test enable */
        sl_l2ramwREG->RAMTEST = (uint32) 0x00000100u;

        /* Wait for the test to complete */
        /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
        while (L2RAM_RAMTEST_TST_TRIGGER == (sl_l2ramwREG->RAMTEST & L2RAM_RAMTEST_TST_TRIGGER));
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /* If in fault injection mode, this will not clear the status and ESM status */
        if(!((SRAM_ECC_1BIT_FAULT_INJECTION== testType) || (SRAM_ECC_ERROR_PROFILING_FAULT_INJECT == testType))){
            if(ST_FAIL != *sram_stResult){
                /* Check if the single bit errors have been generated for both banks */
            	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
            	/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                if ((TCRAM_RAMERRSTATUS_ADDR_SERR == ((uint32)sl_tcram1REG->RAMERRSTATUS & TCRAM_RAMERRSTATUS_ADDR_SERR))
                		/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                        && ((TCRAM_RAMERRSTATUS_ADDR_SERR == ((uint32)sl_tcram2REG->RAMERRSTATUS & TCRAM_RAMERRSTATUS_ADDR_SERR)))
                        && (((1u << ESM_G1ERR_B0TCM_CORRERR) | (1u << ESM_G1ERR_B1TCM_CORRERR))
                                == (sl_esmREG->SR1[0] & ((1u << ESM_G1ERR_B0TCM_CORRERR) | (1u << ESM_G1ERR_B1TCM_CORRERR))))){
                    /*LDRA_INSPECTWINDOW 50 */
            		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                    /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> Comment_8*/
                    /*SAFETYMCUSW 344 S MR:11.5 <APPROVED> Comment_9*/
                	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                	/*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
                    if( ((SRAM_ECC_ERROR_FORCING_1BIT==testType) &&
                    		(sl_tcram2REG->RAMSERRADDR == ((uint32)&sramEccTestBuff[1] & ((uint32)0x0003FFFFU)))) || (SRAM_ECC_ERROR_PROFILING==testType)) {
                        *sram_stResult = ST_PASS;
                    }else{
                        *sram_stResult = ST_FAIL;
                    }
                    sl_tcram1REG->RAMOCCUR = 0x00000000u;
                    sl_tcram2REG->RAMOCCUR = 0x00000000u;
                } else {
                    *sram_stResult = ST_FAIL;
                }
            }
            /* Clear the ESM Status */
            sl_esmREG->SR1[0] = ((1u << ESM_G1ERR_B0TCM_CORRERR) | (1u << ESM_G1ERR_B1TCM_CORRERR));
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            BIT_SET(sl_tcram1REG->RAMERRSTATUS,TCRAM_RAMERRSTATUS_ADDR_SERR);/* Clear for subsequent operation */
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            BIT_SET(sl_tcram2REG->RAMERRSTATUS,TCRAM_RAMERRSTATUS_ADDR_SERR);
            /* Compute uncorrupted ECC */
            sramEccTestBuff[0] = 0u;
            sramEccTestBuff[1] = 0u;
        }
        sl_tcram1REG->RAMTHRESHOLD = regBkupRamTh1;
        sl_tcram2REG->RAMTHRESHOLD = regBkupRamTh2;
        /* Restore ctrl registers */
        sl_tcram1REG->RAMCTRL &= ~TCRAM_RAMCTRL_ECCWREN;
        sl_tcram2REG->RAMCTRL &= ~TCRAM_RAMCTRL_ECCWREN;

        /*Restore grp1 esm interrupt enable*/
        sl_esmREG->IESR1 = regBkupIntEnaSet;
        sl_esmREG->EEPAPR1 = regBckupErrInfulence;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /*if(SRAM_ECC_ERROR_FORCING_1BIT == testType){*/
        /* Check if the diagnostic error status bits have been set */
        if(((sl_esmREG->SSR2 & (1u << ESM_G2ERR_L2RAMW_UNCORR_B)) == (1 << ESM_G2ERR_L2RAMW_UNCORR_B)) &&
            ((sl_l2ramwREG->RAMERRSTATUS & (1 << L2RAM_RAMERRSTATUS_DRSE)) == (1 << L2RAM_RAMERRSTATUS_DRSE)) &&
            ((sl_l2ramwREG->RAMERRSTATUS & (1 << L2RAM_RAMERRSTATUS_DWSE)) == (1 << L2RAM_RAMERRSTATUS_DWSE)))
        {
            *sram_stResult = ST_PASS;
        }
        else
        {
            *sram_stResult = ST_FAIL;
        }
        /* Clear diagnostic errors status bits [22,21,20,19,12,11,10, 4] */
        BIT_SET(sl_l2ramwREG->RAMERRSTATUS, L2RAM_RAMERRSTATUS_CLRALL);
        _SL_HoldNClear_nError(); /* Clear nError */
        /*}*/
#endif

        break;

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    case SRAM_PAR_ADDR_CTRL_SELF_TEST:
        /* Once the tests are started, any access to RAM will generate parity errors. So block interrupts */
        irqStatus = _SL_Disable_IRQ();
        /* Should the interrupt routine be backed up and restored before this test is run or
         * should we use the Shadow register only. Because the ISR reads IOFFHR and IOFFLR
         * which in turn clears the status register, which will still be there in Shadow register
         *
         * Pointing ISR to a dummy ISR which does not read IOFFHR/IOFFLR can resolve this
         *
         */

        /* Added for bug fix*/
        tempVal = sl_tcram1REG->RAMPERADDR; /* RAMPERRADDR register must be read for subsequent updates */
        tempVal = sl_tcram2REG->RAMPERADDR;
        tempVal = tempVal; /* avoid compiler warning */
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
        /* Override parity (actually flip).. NO RAM ACCESS from this point (except intentional errors) */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        sl_tcram1REG->RAMCTRL = (sl_tcram1REG->RAMCTRL & (~TCRAM_RAMCTRL_ADDRPAR_OVR_MASK)) | TCRAM_RAMCTRL_ADDR_PARITY_OVER;
        /* Access any RAM location */
        _SL_Barrier_Data_Access();
        ramread64 = sramEccTestBuff[0]; /* Generate read parity error on B0 */
        _SL_Barrier_Data_Access();
        /* Restore parity, so that we can use the stack */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        sl_tcram1REG->RAMCTRL = (sl_tcram1REG->RAMCTRL & (~TCRAM_RAMCTRL_ADDRPAR_OVR_MASK));


        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        sl_tcram2REG->RAMCTRL = (sl_tcram2REG->RAMCTRL & (~TCRAM_RAMCTRL_ADDRPAR_OVR_MASK)) | TCRAM_RAMCTRL_ADDR_PARITY_OVER;
        ramread64 = sramEccTestBuff[1]; /* Generate read parity error on B1 */
        _SL_Barrier_Data_Access();
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        sl_tcram2REG->RAMCTRL = (sl_tcram2REG->RAMCTRL & (~TCRAM_RAMCTRL_ADDRPAR_OVR_MASK));


        /* Have to check the Shadow register as the normal ESM status register is cleared during the interrupt handling */
        /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if ((TCRAM_RAMERRSTAT_RADDRPAR_FAIL == (uint32)(sl_tcram1REG->RAMERRSTATUS &
        		TCRAM_RAMERRSTAT_RADDRPAR_FAIL)) &&  /* Read parity error on B1 */
                (TCRAM_RAMERRSTAT_RADDRPAR_FAIL ==
                		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                		(uint32)(sl_tcram2REG->RAMERRSTATUS &
                		TCRAM_RAMERRSTAT_RADDRPAR_FAIL)) &&  /* Read parity error on B2 */
                (GET_ESM_BIT_NUM(ESM_G2ERR_B0TCM_ADDPAR) ==
                		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                		(sl_esmREG->SSR2 & GET_ESM_BIT_NUM(ESM_G2ERR_B0TCM_ADDPAR))) &&   /* B1 Parity error */
                (GET_ESM_BIT_NUM(ESM_G2ERR_B1TCM_ADDPAR) ==
                		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                		(sl_esmREG->SSR2 & GET_ESM_BIT_NUM(ESM_G2ERR_B1TCM_ADDPAR)))) {    /* B2 parity error */
            *sram_stResult = ST_PASS;
        } else {
            *sram_stResult = ST_FAIL;
        }
        /* Anyways Clear the interrupt bits */
        sl_esmREG->SSR2 = (1u << ESM_G2ERR_B0TCM_ADDPAR);
        sl_esmREG->SSR2 = (1u << ESM_G2ERR_B1TCM_ADDPAR);
        sl_esmREG->SR1[1] = (1u << ESM_G2ERR_B0TCM_ADDPAR);
        sl_esmREG->SR1[1] = (1u << ESM_G2ERR_B1TCM_ADDPAR);
        sl_tcram1REG->RAMERRSTATUS = TCRAM_RAMERRSTAT_RADDRPAR_FAIL;
        sl_tcram2REG->RAMERRSTATUS = TCRAM_RAMERRSTAT_RADDRPAR_FAIL;
        tempVal = sl_tcram1REG->RAMPERADDR; /* RAMPERRADDR register must be read for subsequent updates */
        tempVal = sl_tcram2REG->RAMPERADDR;
        tempVal = tempVal; /* avoid compiler warning */

        if (ST_PASS == *sram_stResult) {
            /* Override parity (actually flip).. NO RAM ACCESS from this point (except intentional errors) */
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            sl_tcram1REG->RAMCTRL = (sl_tcram1REG->RAMCTRL & 0xF0FFFFFFU) | TCRAM_RAMCTRL_ADDR_PARITY_OVER;
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            sl_tcram2REG->RAMCTRL = (sl_tcram2REG->RAMCTRL & 0xF0FFFFFFU) | TCRAM_RAMCTRL_ADDR_PARITY_OVER;
            sramEccTestBuff[0] = 0xAAAAAAAABBBBBBBBu; /* Generate write parity error on B1 & B2 */
            sramEccTestBuff[1] = 0xBBBBBBBBAAAAAAAAu;
            /* Restore parity, so that we can use the stack */
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            sl_tcram1REG->RAMCTRL = (sl_tcram1REG->RAMCTRL & 0xF0FFFFFFU);
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            sl_tcram2REG->RAMCTRL = (sl_tcram2REG->RAMCTRL & 0xF0FFFFFFU);
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
            /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
            /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
            if ((TCRAM_RAMERRSTAT_WADDRPAR_FAIL == (uint32)(sl_tcram1REG->RAMERRSTATUS & TCRAM_RAMERRSTAT_WADDRPAR_FAIL)) &&  /* Write parity error on B1 */
                    (TCRAM_RAMERRSTAT_WADDRPAR_FAIL == (uint32)(sl_tcram2REG->RAMERRSTATUS & TCRAM_RAMERRSTAT_WADDRPAR_FAIL)) &&  /* Write parity error on B2 */
                    (0 != (sl_esmREG->SSR2 & (1u << ESM_G2ERR_B0TCM_ADDPAR))) &&    /* B1 Parity error */
                    (0 != (sl_esmREG->SSR2 & (1u << ESM_G2ERR_B1TCM_ADDPAR)))) {    /* B2 parity error */
                *sram_stResult = ST_PASS;
            } else {
                *sram_stResult = ST_FAIL;
            }

            /* Restore parity */
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            sl_tcram1REG->RAMCTRL = (sl_tcram1REG->RAMCTRL & 0xF0FFFFFFU);
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            sl_tcram2REG->RAMCTRL = (sl_tcram2REG->RAMCTRL & 0xF0FFFFFFU);

            /* Anyways Clear the interrupt bits */
            sl_esmREG->SSR2 = (1u << ESM_G2ERR_B0TCM_ADDPAR);
            sl_esmREG->SSR2 = (1u << ESM_G2ERR_B1TCM_ADDPAR);
            sl_esmREG->SR1[1] = (1u << ESM_G2ERR_B0TCM_ADDPAR);
            sl_esmREG->SR1[1] = (1u << ESM_G2ERR_B1TCM_ADDPAR);
            sl_tcram1REG->RAMERRSTATUS = TCRAM_RAMERRSTAT_WADDRPAR_FAIL;
            sl_tcram2REG->RAMERRSTATUS = TCRAM_RAMERRSTAT_WADDRPAR_FAIL;
            tempVal = sl_tcram1REG->RAMPERADDR; /* RAMPERRADDR register must be read for subsequent updates */
            tempVal = sl_tcram2REG->RAMPERADDR;
            tempVal = tempVal; /* avoid compiler warning */
        }
        _SL_HoldNClear_nError(); /* Clear nError */
        /* Restore interrupts */
        _SL_Restore_IRQ(irqStatus);
        ramread64 = ramread64; /* avoid compiler warning */

        break;
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    case SRAM_RADECODE_DIAGNOSTICS:
        retVal = TRUE;

#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif

        /*flag is set to indicate the current test which is ongoing and
        These flags are used in the sl_esm.c so as to mask the esm callback*/
        SL_FLAG_SET(SRAM_RADECODE_DIAGNOSTICS);
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        _SL_SelfTest_SRAM_RAD(sl_tcram1REG, sram_stResult);
        _SL_HoldNClear_nError(); /* Clear nError */
#endif
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        if (ST_PASS == *sram_stResult) {
            sl_esmREG->SR1[1] = (1u << ESM_G2ERR_B0TCM_UNCORR);
            sl_esmREG->SSR2 = (1u << ESM_G2ERR_B0TCM_UNCORR);
            /*flag is set to indicate the current test which is ongoing and
            These flags are used in the sl_esm.c so as to mask the esm callback*/
            SL_FLAG_SET(SRAM_RADECODE_DIAGNOSTICS);
            _SL_SelfTest_SRAM_RAD(sl_tcram2REG, sram_stResult);
            _SL_HoldNClear_nError(); /* Clear nError */
            if (ST_PASS == *sram_stResult) {
                sl_esmREG->SR1[1] = (1u << ESM_G2ERR_B1TCM_UNCORR);
                sl_esmREG->SSR2 = (1u << ESM_G2ERR_B1TCM_UNCORR);
            }
        }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        _SL_SelfTest_SRAM_RAD(sl_l2ramwREG, sram_stResult);
        /* Clear any previous diagnostic errors status bits [22,21,20,19,12,11,10, 4] - required before trigger */
        BIT_SET(sl_l2ramwREG->RAMERRSTATUS, L2RAM_RAMERRSTATUS_CLRALL);
        _SL_HoldNClear_nError(); /* Clear nError */
#endif
        /*clear the flags which indicate tests ongoing*/
        SL_FLAG_CLEAR(SRAM_RADECODE_DIAGNOSTICS);
        break;
#endif

    default:
        retVal = FALSE;
        break;
    }
    /* If the test was fault injection and not self test, then do not mask the call back & clear the *
    *  FAULT_INJECTION flag for subsequent runs*/
    if((TRUE) == _sl_fault_injection){
            _sl_fault_injection=FALSE;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_14*/
    SL_Log_Result(FUNC_ID_ST_SRAM,testType, *sram_stResult , 0U);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define flashBadECC1    0x20000000u
#define flashBadECC2    0x20000008u
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define flashBadECC1    0x00000008u
#define flashBadECC2    0x000000016u
/* Flash diagnostics must be run from SRAM  */
#ifdef __TI_COMPILER_VERSION__
#pragma CODE_SECTION (SL_SelfTest_Flash, ".sl_stflash_SRAM")
#endif
#ifdef __IAR_SYSTEMS_ICC__
#pragma location = ".sl_stflash_SRAM"
#endif
#endif
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_Flash (SL_SelfTestType testType, boolean bMode, SL_SelfTest_Result * flash_stResult)
{
    boolean retVal = FALSE;
    uint32 fdiagCtrl = F021F_FDIAGCTRL_DIAG_EN_KEY, regBkupIntEnaSet;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    uint32 regBkupFparOvr, regBckupErrInfulence, regBckupFdiagctrl;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
    uint32 content, index, regBkupCamAvail, regBckupFdiagctrl, regBckupErrctrl, regBckupErrInfulence, vimCleared = 0;
#endif
    volatile boolean _sl_fault_injection = FALSE;
    volatile uint32 flashread = 0u;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    /* If fault inject set global variable to flag to the ESM handler that it is a fault injection */
    if((FLASH_ADDRESS_ECC_FAULT_INJECT == testType)||
        (FLASH_ADDRESS_PARITY_FAULT_INJECT == testType)||
        (FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT == testType)){
        _sl_fault_injection=TRUE;
    }
#endif
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in rage */
    /*LDRA_INSPECTWINDOW 50 */
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if ((_SELFTEST_FLASHECC_MIN > testType) || (_SELFTEST_FLASHECC_MAX < testType) || !CHECK_RANGE_RAM_PTR(flash_stResult)) {
        SL_Log_Error(FUNC_ID_ST_FLASH, ERR_TYPE_PARAM, 0U);
        return(FALSE);
    }
#endif
#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /* Error if not in privilege mode */
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_FLASH, ERR_TYPE_ENTRY_CON, 2U);
        return(FALSE);
    }

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_FLASH, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif

    switch(testType) {
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    case FLASH_ECC_ADDR_TAG_REG_MODE:
        regBckupFdiagctrl = sl_flashWREG->FDIAGCTRL;
        /* Read to unfreeze the error address registers */
        flashread = sl_flashWREG->FUNCERRADD;
        fdiagCtrl |= F021F_FDIAGCTRL_DMODE_ADDR_TAG;
        /* clear flash status flags */
        sl_flashWREG->FEDACSTATUS = (F021F_FEDACSTATUS_ADD_TAG_ERR |  F021F_EE_STATUS_D_UNC_ERR_FLG);
        sl_flashWREG->FDIAGCTRL = fdiagCtrl;
        /* address for test in FRAWDATAL*/
        sl_flashWREG->FRAWDATAL = sl_flashWREG->FPRIMADDTAG;

        sl_flashWREG->FREDUADDTAG^=(uint32)~0;
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
        sl_flashWREG->FDIAGCTRL |=F021F_FDIAGCTRL_DIAG_TRIG;
        _SL_HoldNClear_nError();
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        /*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
        if ((F021F_FEDACSTATUS_ADD_TAG_ERR == ((uint32)sl_flashWREG->FEDACSTATUS & F021F_FEDACSTATUS_ADD_TAG_ERR)) &&
            (GET_ESM_BIT_NUM(ESM_G3ERR_FMC_UNCORR) == (sl_esmREG->SR1[2] & GET_ESM_BIT_NUM(ESM_G3ERR_FMC_UNCORR))))
        {
            /*Clear ESM status */
            sl_esmREG->SR1[2]=GET_ESM_BIT_NUM(ESM_G3ERR_FMC_UNCORR);
            *flash_stResult = ST_PASS;
            /* clear flash status flags */
            sl_flashWREG->FEDACSTATUS = (F021F_FEDACSTATUS_ADD_TAG_ERR |  F021F_EE_STATUS_D_UNC_ERR_FLG);
            /* Read to unfreeze the error address registers */
            flashread = sl_flashWREG->FUNCERRADD;

            sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;
            sl_flashWREG->FDIAGCTRL = fdiagCtrl;
            /*Load primary address tag(with a different value address in FRAW_DATAL != FPRIMADDTAG )*/
            sl_flashWREG->FPRIMADDTAG^=(uint32)~0;
            /*Load duplicate address tag*/
            sl_flashWREG->FRAWDATAL =sl_flashWREG->FREDUADDTAG;
            sl_flashWREG->FDIAGCTRL |=F021F_FDIAGCTRL_DIAG_TRIG;
            _SL_HoldNClear_nError();
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
            if ((F021F_FEDACSTATUS_ADD_TAG_ERR == ((uint32)sl_flashWREG->FEDACSTATUS & F021F_FEDACSTATUS_ADD_TAG_ERR))&&
                (GET_ESM_BIT_NUM(ESM_G3ERR_FMC_UNCORR) == (sl_esmREG->SR1[2] & GET_ESM_BIT_NUM(ESM_G3ERR_FMC_UNCORR))))
            {
                *flash_stResult = ST_PASS;
            }
            else
            {
                *flash_stResult = ST_FAIL;
            }
        }
        else
        {
            *flash_stResult = ST_FAIL;
            /*if any of the test fails break the loop as the test case has failed*/
        }
        /*Clear ESM status */
        sl_esmREG->SR1[2]=GET_ESM_BIT_NUM(ESM_G3ERR_FMC_UNCORR);
        /* clear flash status flags */
        sl_flashWREG->FEDACSTATUS = (F021F_FEDACSTATUS_ADD_TAG_ERR |  F021F_EE_STATUS_D_UNC_ERR_FLG);
        /* Read to unfreeze the error address registers */
        flashread = sl_flashWREG->FUNCERRADD;
        sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;
        retVal = TRUE;
        break;
#endif
    case FLASH_ECC_TEST_MODE_1BIT:

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /* Backup parity over ride register */
        regBkupFparOvr = sl_flashWREG->FPAROVR;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /* Backup the EPC error control register*/
        regBckupErrctrl = sl_epcREG1->EPCCNTRL;
#endif
        /* Backup the FLASH diag control register */
        regBckupFdiagctrl = sl_flashWREG->FDIAGCTRL;

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /* Read to unfreeze the error address registers */
        flashread = sl_flashWREG->FCORERRADD;
        sl_flashWREG->FEDACSTATUS= F021F_FEDACSTATUS_ERR_ZERO_FLG;
#endif
        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        regBkupIntEnaSet = sl_esmREG->IESR1;

        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBckupErrInfulence = sl_esmREG->EEPAPR1;

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /* Disable interrupt on CAM/FIFO full */
        if((sl_vimREG->REQMASKCLR3 & VIM_CHANNEL_EPC_FIFO_CAM_FULL) == VIM_CHANNEL_EPC_FIFO_CAM_FULL) {
            BIT_SET(sl_vimREG->REQMASKCLR3, VIM_CHANNEL_EPC_FIFO_CAM_FULL);
            vimCleared = 1;
        }
#endif
        if((FALSE) == _sl_fault_injection){
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
            sl_esmREG->IECR1 = GET_ESM_BIT_NUM(ESM_G1ERR_FMC_CORR);
            sl_esmREG->DEPAPR1 = GET_ESM_BIT_NUM(ESM_G1ERR_FMC_CORR);
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
            sl_esmREG->IECR1 = GET_ESM_BIT_NUM(ESM_G1ERR_EPC_SERR);
            sl_esmREG->DEPAPR1 = GET_ESM_BIT_NUM(ESM_G1ERR_EPC_SERR);
#endif
        }
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /* Clear any pending CAM full/overflow events */
        BIT_SET(sl_epcREG1->EPCERRSTAT, (EPC_ERR_CAM_FULL | EPC_ERR_CAM_OVRFLW));

        /* Clear any pending FIFO full/overflow events */
        BIT_SET(sl_epcREG1->FIFOFULLSTAT, EPC_FIFO_FULL);
        BIT_SET(sl_epcREG1->OVRFLWSTAT, EPC_FIFO_OVRFLW);

        /* Enable EPC CAM/FIFO full events */
        BIT_SET(sl_epcREG1->EPCCNTRL, EPC_CTRL_FULL_EN);
        /* Enable EPC CAM/FIFO overflow events (also enables correctable error event) */
        BF_SET(sl_epcREG1->EPCCNTRL, EPC_CTRL_SERR_EN, EPC_CTRL_SERR_START, EPC_CTRL_SERR_LENGTH);
#endif

        /* Setup the diag mode for 1 bit error */
        fdiagCtrl |= F021F_FDCTRL_DMODE_TEST_MODE;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        sl_flashWREG->FPAROVR = F021F_FPAROVR_PAR_OVR_ENABLE|F021F_FPAROVR_BUS_PAR_ENABLE;
        sl_flashWREG->FPAROVR |= F021F_FPAROVR_SYN_1BIT_DATA_ECC;
#endif
        sl_flashWREG->FDIAGCTRL = fdiagCtrl;
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /* Prepare the values of data and ecc to be XORed */
        sl_flashWREG->FEMU_DMSW = 0U;
        sl_flashWREG->FEMU_DLSW = 0U;
        sl_flashWREG->FEMU_ECC = F021F_FEMU_ECC_SYN_1BIT_DATA_ECC;
        /* Select the right port for reading from */
        BF_SET(sl_flashWREG->FDIAGCTRL, F021F_FDIAGCTRL_DIAG_BUF_SEL_A, F021F_FDIAGCTRL_DIAG_EN_START, F021F_FDIAGCTRL_DIAG_EN_LENGTH);


        regBkupCamAvail = sl_epcREG1->CAMAVAILSTAT;
#endif

#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        regBkupCamAvail = sl_epcREG1->CAMAVAILSTAT;
#endif
        /* Enable the trigger bit of the diagnostic */
        sl_flashWREG->FDIAGCTRL |= F021F_FDIAGCTRL_DIAG_TRIG;

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 58 S MR:14.3 <APPROVED> Comment_16*/
        flashread = *(volatile uint32 *)flashBadECC2;
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
        /*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
        if ((F021F_FEDACSTATUS_ERR_ZERO_FLG == ((uint32)sl_flashWREG->FEDACSTATUS & F021F_FEDACSTATUS_ERR_ZERO_FLG))
                && (sl_flashWREG->FCORERRADD == 0x8u)
                && ((sl_esmREG->SR1[0] & BIT(ESM_G1ERR_FMC_CORR)) == BIT(ESM_G1ERR_FMC_CORR))) {
            /* Clear flash & ESM status registers */
            sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_ERR_ZERO_FLG;
            sl_esmREG->SR1[0] = BIT(ESM_G1ERR_FMC_CORR);
            flashread = sl_flashWREG->FCORERRADD;
            /* Repeat the test for a different address to ensure that the FCORERRADD register is updated correctly */
            /*SAFETYMCUSW 58 S MR:14.3 <APPROVED> Comment_16*/
            sl_flashWREG->FDIAGCTRL |= F021F_FDIAGCTRL_DIAG_TRIG;
            flashread = *(volatile uint32 *)flashBadECC1;
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
            /*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
            if ((F021F_FEDACSTATUS_ERR_ZERO_FLG == ((uint32)sl_flashWREG->FEDACSTATUS & F021F_FEDACSTATUS_ERR_ZERO_FLG))
                    && (sl_flashWREG->FCORERRADD == 0x0u)
                    && ((sl_esmREG->SR1[0] & BIT(ESM_G1ERR_FMC_CORR)) == BIT(ESM_G1ERR_FMC_CORR))) {
                *flash_stResult = ST_PASS;
            } else {
                *flash_stResult = ST_FAIL;
            }
        } else {
            *flash_stResult = ST_FAIL;
        }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)

        /*SAFETYMCUSW 58 S MR:14.3 <APPROVED> Comment_16*/
        flashread = *(volatile uint32 *)flashBadECC1;

        if (((((sl_epcREG1->EPCERRSTAT & EPC_ERR_CAM_FULL) == EPC_ERR_CAM_FULL) ||
                ((sl_epcREG1->EPCERRSTAT & EPC_ERR_CAM_OVRFLW) == EPC_ERR_CAM_OVRFLW) ||
                (sl_epcREG1->CAMAVAILSTAT != regBkupCamAvail))) &&
                ((((sl_esmREG->SR1[0]) & BIT(ESM_G1ERR_EPC_SERR))) == (BIT(ESM_G1ERR_EPC_SERR)))) {
                    /* Clear ESM status registers */
                    sl_esmREG->SR1[0] = BIT(ESM_G1ERR_EPC_SERR);

            if(sl_epcREG1->CAMAVAILSTAT != regBkupCamAvail) {
                /* Clear the CAM entries */
                /* Set CAM indices to available */
                for(content = 0U; content<32U; content++) {
                    if((volatile uint32 *)(sl_epcREG1->CAM_CONTENT[content] & EPC_CAM_CONTENT_ADDR) == (volatile uint32 *)(flashBadECC1 & EPC_CAM_CONTENT_ADDR)) {
                        break;
                    }
                }
                /* Calculate the index location */
                index = content/4;
                index = content - index*4;
                content = content/4;
                BF_SET(sl_epcREG1->CAM_INDEX[content], EPC_CAM_INDEX_CLEAR, EPC_CAM_INDEX_START(index), EPC_CAM_INDEX_LENGTH);
            }

            *flash_stResult = ST_PASS;

        } else {
            *flash_stResult = ST_FAIL;
        }
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /* Anyways clear flash & ESM status registers */
        sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_ERR_ZERO_FLG;
        sl_esmREG->SR1[0] = BIT(ESM_G1ERR_FMC_CORR);
        flashread = sl_flashWREG->FCORERRADD;
#endif
        /* Clear the diag mode settings */
        sl_flashWREG->FDIAGCTRL &=~(F021F_FDCTRL_DMODE_TEST_MODE|F021F_FDIAGCTRL_DIAG_EN_MASK); /* Clear Diagnostics enable key */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        sl_flashWREG->FPAROVR = regBkupFparOvr;
#endif
        sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;

        /*Restore grp1 esm interrupt enable*/
        sl_esmREG->IESR1 = regBkupIntEnaSet;
        sl_esmREG->EEPAPR1 = regBckupErrInfulence;
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)

        /* Restore EPC registers*/
        sl_epcREG1->EPCCNTRL = regBckupErrctrl;
		sl_epcREG1->CAMAVAILSTAT = regBkupCamAvail;

        if(vimCleared == 1) {
            BIT_SET(sl_vimREG->REQMASKSET3, VIM_CHANNEL_EPC_FIFO_CAM_FULL);
        }

#endif

        retVal = TRUE; /* Tests were run */
        break;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    case FLASH_ADDRESS_PARITY_SELF_TEST:
    case FLASH_ADDRESS_PARITY_FAULT_INJECT:
        /* Backup registers */
        regBkupFparOvr = sl_flashWREG->FPAROVR;
        regBckupFdiagctrl = sl_flashWREG->FDIAGCTRL;
        /* Read to unfreeze the error address registers */
        flashread = sl_flashWREG->FUNCERRADD;
        sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_ADD_PAR_ERR;
        fdiagCtrl |= F021F_FDCTRL_DMODE_TEST_MODE;
        sl_flashWREG->FPAROVR = F021F_FPAROVR_PAR_OVR_ENABLE|F021F_FPAROVR_BUS_PAR_ENABLE;
        sl_flashWREG->FDIAGCTRL = fdiagCtrl;
        sl_flashWREG->FPAROVR |= F021F_FPAROVR_ADD_INV_PAR;
        /*flag is set to indicate the current test which is ongoing and
        These flags are used in the sl_esm.c so as to mask the esm callback*/
        if(testType == FLASH_ADDRESS_PARITY_SELF_TEST)
        {
        	SL_FLAG_SET(FLASH_ADDRESS_PARITY_SELF_TEST);
        }
        else
        {
        	SL_FLAG_SET(FLASH_ADDRESS_PARITY_FAULT_INJECT);
        }
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
        flashread = *(volatile uint32 *)flashBadECC1;

        if(FLASH_ADDRESS_PARITY_FAULT_INJECT != testType)
        {
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        	/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
            if ((F021F_FEDACSTATUS_ADD_PAR_ERR==((uint32)sl_flashWREG->FEDACSTATUS & F021F_FEDACSTATUS_ADD_PAR_ERR))) {
                sl_flashWREG->FPAROVR = regBkupFparOvr;
                _SL_HoldNClear_nError(); /* Clear nError */
                /* Clear flash & ESM status registers */
                sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_ADD_PAR_ERR;
                sl_esmREG->SR1[1] = GET_ESM_BIT_NUM(ESM_G2ERR_FMC_UNCORR);
                flashread = sl_flashWREG->FUNCERRADD;
                *flash_stResult = ST_PASS;
            }else {
                *flash_stResult = ST_FAIL;
            }
            /* Anyways clear flash & ESM status registers */
            sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_ADD_PAR_ERR;
            sl_esmREG->SR1[1] =  GET_ESM_BIT_NUM(ESM_G2ERR_FMC_UNCORR);
            sl_esmREG->SSR2 = GET_ESM_BIT_NUM(ESM_G2ERR_FMC_UNCORR);
            flashread = sl_flashWREG->FUNCERRADD;

            /* Clear the diag mode settings */
            sl_flashWREG->FPAROVR = regBkupFparOvr;
            sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;
        }
        retVal = TRUE; /* Tests were run */
        /*clear the flags which indicate tests ongoing*/
        if(testType == FLASH_ADDRESS_PARITY_SELF_TEST)
        {
        	SL_FLAG_CLEAR(FLASH_ADDRESS_PARITY_SELF_TEST);
        }
        else
        {
        	SL_FLAG_CLEAR(FLASH_ADDRESS_PARITY_FAULT_INJECT);
        }
        break;
#endif
    case FLASH_ECC_TEST_MODE_2BIT:
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    case FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT:
    case FLASH_ADDRESS_ECC_SELF_TEST:
    case FLASH_ADDRESS_ECC_FAULT_INJECT:

        /* Backup parity over ride register */
        regBkupFparOvr = sl_flashWREG->FPAROVR;
#endif
        regBckupFdiagctrl = sl_flashWREG->FDIAGCTRL;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /* Read to unfreeze the error address registers */
        flashread = sl_flashWREG->FUNCERRADD;
        sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_B1_UNC_ERR;
        sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_ERR_ZERO_FLG;
#endif

        /* Setup the diag mode for 2 bit error */
        fdiagCtrl |= F021F_FDCTRL_DMODE_TEST_MODE;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        sl_flashWREG->FPAROVR = F021F_FPAROVR_PAR_OVR_ENABLE|F021F_FPAROVR_BUS_PAR_ENABLE;
        if((FLASH_ADDRESS_ECC_SELF_TEST == testType)||(FLASH_ADDRESS_ECC_FAULT_INJECT == testType ))
        {
            sl_flashWREG->FPAROVR |= F021F_FPAROVR_SYN_ADDRESS_ECC;
        }
        else /* FLASH_ECC_TEST_MODE_2BIT || FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT */
        {
            sl_flashWREG->FPAROVR |= F021F_FPAROVR_SYN_2BIT_DATA_ECC;
        }
#endif
        sl_flashWREG->FDIAGCTRL = fdiagCtrl;
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        sl_flashWREG->FEMU_DMSW = 0U;
        sl_flashWREG->FEMU_DLSW = 0U;
        sl_flashWREG->FEMU_ECC = F021F_FEMU_ECC_SYN_2BIT_DATA_ECC;
        BF_SET(sl_flashWREG->FDIAGCTRL, F021F_FDIAGCTRL_DIAG_BUF_SEL_A, F021F_FDIAGCTRL_DIAG_EN_START, F021F_FDIAGCTRL_DIAG_EN_LENGTH);
#endif


        /*flag is set to indicate the current test which is ongoing and
        These flags are used in the sl_esm.c so as to mask the esm callback*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        if ((FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT != testType)&&(FLASH_ADDRESS_ECC_FAULT_INJECT != testType)) 
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        if (FLASH_ECC_TEST_MODE_2BIT == testType) 
#endif
        {
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        	SL_FLAG_SET(FLASH_ECC_TEST_MODE_2BIT);
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
            /* Test must run from RAM - cannot execute the function SL_FLAG_SET
             * from Flash, so copying the functionality inline.  */
            extern boolean sl_priv_flag_set[];
            sl_priv_flag_set[FLASH_ECC_TEST_MODE_2BIT-TESTTYPE_MIN] = TRUE;
#endif

#if FUNCTION_PROFILING_ENABLED
		    SL_Record_Errorcreationtick(testType);
#endif

        	sl_flashWREG->FDIAGCTRL |= F021F_FDIAGCTRL_DIAG_TRIG;
		
            /*SAFETYMCUSW 58 S MR:14.3 <APPROVED> Comment_16*/
            flashread = *(volatile uint32 *)flashBadECC2;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
            /*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
            if ((F021F_FEDACSTATUS_B1_UNC_ERR==((uint32)sl_flashWREG->FEDACSTATUS & F021F_FEDACSTATUS_B1_UNC_ERR))
                    && (sl_flashWREG->FUNCERRADD == 0x8u)
                    && (BIT(ESM_G3ERR_FMC_UNCORR) == (sl_esmREG->SR1[2] & BIT(ESM_G3ERR_FMC_UNCORR)))) {
                _SL_HoldNClear_nError(); /* Clear nError */
                /* Clear flash & ESM status registers */
                sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_B1_UNC_ERR;
                sl_esmREG->SR1[2] = BIT(ESM_G3ERR_FMC_UNCORR);

                /* Repeat the test for a different address to ensure that the FCORERRADD register is updated correctly */
                /*SAFETYMCUSW 58 S MR:14.3 <APPROVED> Comment_16*/
                sl_flashWREG->FDIAGCTRL = fdiagCtrl;
                sl_flashWREG->FDIAGCTRL |= F021F_FDIAGCTRL_DIAG_TRIG;
                flashread = *(volatile uint32 *)flashBadECC1;
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                /*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                if ((F021F_FEDACSTATUS_B1_UNC_ERR == ((uint32)sl_flashWREG->FEDACSTATUS & F021F_FEDACSTATUS_B1_UNC_ERR))
                        && (sl_flashWREG->FUNCERRADD == 0x0u) && (BIT(ESM_G3ERR_FMC_UNCORR) == (sl_esmREG->SR1[2] & BIT(ESM_G3ERR_FMC_UNCORR)))) {
                    _SL_HoldNClear_nError();
                    *flash_stResult = ST_PASS;
                } else {
                    *flash_stResult = ST_FAIL;
                }
            } else {
                *flash_stResult = ST_FAIL;
            }
			/* Anyways clear flash & ESM status registers */
			#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
			sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_B1_UNC_ERR;
			flashread = sl_flashWREG->FUNCERRADD;
			#endif
            sl_esmREG->SR1[2] =  BIT(ESM_G3ERR_FMC_UNCORR);
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
            if (BIT(ESM_G2ERR_CR5_BUSERR) == (sl_esmREG->SSR2 & BIT(ESM_G2ERR_CR5_BUSERR))) {
                /* Clear nError */
                _SL_HoldNClear_nError();
                sl_esmREG->SSR2 = BIT(ESM_G2ERR_CR5_BUSERR);
                *flash_stResult = ST_PASS;
            } else {
                *flash_stResult = ST_FAIL;
            }
#endif
            /* Clear the diag mode settings */
			#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
			sl_flashWREG->FPAROVR = regBkupFparOvr;
			#endif
            sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;
            /* Clear the flags which indicate tests ongoing*/
            SL_FLAG_CLEAR(FLASH_ECC_TEST_MODE_2BIT);
        }
        /* Tests were run */
        retVal = TRUE;
        break;

    default:
        retVal = FALSE;
        break;
    }
    /* If the test was fault injection and not self test, then do not mask the call back & clear the *
     *  FAULT_INJECTION flag for subsequent runs*/
    if((TRUE) == _sl_fault_injection){
            _sl_fault_injection=FALSE;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_14*/
    SL_Log_Result(FUNC_ID_ST_FLASH, testType, *flash_stResult , 0U);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define feeBadECC1        (*(volatile uint32 *)0xF0200000u)
#define feeBadECC2        (*(volatile uint32 *)0xF0200008u)
#define ADDR_DATA_MSW                        (0xF0200004u)
#define ADDR_DATA_LSW                        (0xF0200000u)
#define FEE_TEST_DATA                        0u
#define FEE_ERROR_POS                        2u
#define FEE_SYNDROME_BIT2_SINGLE_BIT_ERROR    0xD3u
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_FEE (SL_SelfTestType testType, boolean bMode, SL_SelfTest_Result * fee_stResult)
{
    boolean retVal = FALSE;
    uint32 fdiagCtrl = F021F_FDIAGCTRL_DIAG_EN_KEY;
    uint32 regBckupFdiagctrl, regBkupIntEnaSet, regBckupErrInfulence;
    uint8 ecc;
    uint8 syndrome=0U;
    volatile boolean _sl_fault_injection = FALSE;
    volatile uint32 flashread = 0u;

#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif
    /* If fault inject set global variable to flag to the ESM handler that it is a fault injection */
    if((FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT == testType)||
        (FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT == testType)){
        _sl_fault_injection=TRUE;
    }
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in rage */
    /*LDRA_INSPECTWINDOW 50 */
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if ((_SELFTEST_FEEECC_MIN > testType) || (_SELFTEST_FEEECC_MAX < testType) || !CHECK_RANGE_RAM_PTR(fee_stResult)) {
        SL_Log_Error(FUNC_ID_ST_FLASH, ERR_TYPE_PARAM, 0U);
        return(FALSE);
    }
#endif
#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /* Error if not in privilege mode */
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_FLASH, ERR_TYPE_ENTRY_CON, 2U);
        return(FALSE);
    }

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_FLASH, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }
#endif
    switch(testType) {

    case FEE_ECC_DATA_CORR_MODE:
    case FEE_ECC_TEST_MODE_1BIT:
    case FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT:
    case FEE_ECC_TEST_MODE_2BIT:
    case FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT:
        regBckupFdiagctrl = sl_flashWREG->FDIAGCTRL;
        fdiagCtrl |= F021F_FDCTRL_DMODE_DATA_CORR;
        /*  Clear all the status errors*/
        sl_flashWREG->EESTATUS |= ~0;
        /* Clear this to unfree error reporting */
        flashread=sl_flashWREG->EEUNCERRADD;
        flashread=sl_flashWREG->EECORRERRADD;

        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
        status functions to get the pass/failure information*/        
        regBkupIntEnaSet = sl_esmREG->IESR4;
        regBckupErrInfulence = sl_esmREG->IEPSR4;
        if((FALSE) == _sl_fault_injection){
            sl_esmREG->IECR4 = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_CORRERR)|GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
            sl_esmREG->IEPCR4 = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_CORRERR)|GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
        }

        /* Load FEMU_XX regs in order to generate ecc */
        sl_flashWREG->FEMUADDR=ADDR_DATA_MSW;
        sl_flashWREG->FEMUDMSW = FEE_TEST_DATA;
        sl_flashWREG->FEMUDLSW = FEE_TEST_DATA;

        /*ecc for the correct data*/
        ecc = sl_flashWREG->FEMUECC;
        /* Load corrupt data in FEMU_XXX regs single bit error */
        sl_flashWREG->FEMUDMSW = FEE_TEST_DATA;
        /* Flip a bit and generate single bit error*/
        if((FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT == testType) || (FEE_ECC_TEST_MODE_2BIT == testType)){
            sl_flashWREG->FEMUDLSW = FEE_TEST_DATA ^ (0x3U<<(FEE_ERROR_POS));
        }else{
            sl_flashWREG->FEMUDLSW = FEE_TEST_DATA ^ BIT(FEE_ERROR_POS);
        }

        /* Set SECDED mode as fee as only fee is supported for this mode
         * and enable the Diagnosis*/
        sl_flashWREG->FDIAGCTRL = fdiagCtrl|F021F_FDIAGCTRL_DIAGECCSEL_FEE;
        sl_flashWREG->FEMUECC = ecc;
        /* Trigger the test */
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
        sl_flashWREG->FDIAGCTRL |=F021F_FDIAGCTRL_DIAG_TRIG;

        if((FEE_ECC_DATA_CORR_MODE == testType) || (FEE_ECC_TEST_MODE_1BIT == testType)){
            /* Check the error flags, if single bit error and only one of ERR_ONE_FLG or ERR_ZERO_FLG is set */
        	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        	/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
            if((F021F_EE_STATUS_D_CORR_ERR_FLG == ((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_D_CORR_ERR_FLG)) &&
                    (GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_CORRERR) == (sl_esmREG->SR4[0] & GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_CORRERR))) &&
                    (((F021F_EE_STATUS_ERR_ZERO_FLG ==
                    		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                    		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                    		/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                    		((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_ERR_ZERO_FLG))    &&
                        (!(F021F_EE_STATUS_ERR_ONE_FLG ==
                        		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                        		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                        		/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                        		((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_ERR_ONE_FLG))))
                        ||
                        ((!(F021F_EE_STATUS_ERR_ZERO_FLG ==
                        		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                        		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                        		/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                        		((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_ERR_ZERO_FLG)))    &&
                                (F021F_EE_STATUS_ERR_ONE_FLG ==
                                		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                                		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                                		/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                                		((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_ERR_ONE_FLG)))) &&
                                        (FEE_ERROR_POS ==
                                        		/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
                                        		/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                                        		/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                                        		((uint32)sl_flashWREG->EECORRERRPOS & F021F_EECORRERRPOS_MASK))){
                    *fee_stResult = ST_PASS;
                }
            else{
                *fee_stResult = ST_FAIL;
            }
            /*Clear the status set during the test*/
            sl_flashWREG->EESTATUS |= F021F_EE_STATUS_D_CORR_ERR_FLG|
                                    F021F_EE_STATUS_ERR_ZERO_FLG|
                                    F021F_EE_STATUS_ERR_ONE_FLG;
            sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_CORRERR);
        }
        else if(FEE_ECC_TEST_MODE_2BIT == testType){
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
        	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
        	/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
            if((F021F_EE_STATUS_D_UNC_ERR_FLG == ((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_D_UNC_ERR_FLG)) &&
                    ((F021F_EE_STATUS_UNC_ERR_FLG ==
                    		/*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
                    		((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_UNC_ERR_FLG)))
                    && (sl_flashWREG->EEUNCERRADD == (ADDR_DATA_MSW & F021F_EEUNCERRADD_MASK))
                    && (GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR) == (sl_esmREG->SR4[0] & GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR)))){
                *fee_stResult = ST_PASS;
                /*Clear the status set during the test*/
                sl_flashWREG->EESTATUS |= F021F_EE_STATUS_D_UNC_ERR_FLG|
                                        F021F_EE_STATUS_UNC_ERR_FLG;
                sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
            }
            else{
                *fee_stResult = ST_FAIL;
            }
        }
        else
        {
            *fee_stResult = ST_FAIL;
        }
        retVal = TRUE;
        /* Clear the diag mode settings */
        sl_flashWREG->FDIAGCTRL &=~(F021F_FDCTRL_DMODE_DATA_CORR|F021F_FDIAGCTRL_DIAG_EN_MASK); /* Clear Diagnostics enable key */
        sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;
        /* Read and unfreeze error reporting */
        flashread=sl_flashWREG->EEUNCERRADD;
        flashread=sl_flashWREG->EECORRERRADD;

        /*Restore grp1 esm interrupt enable*/
        sl_esmREG->IESR4 = regBkupIntEnaSet;
        sl_esmREG->IEPSR4 = regBckupErrInfulence;
        break;

    case FEE_ECC_SYN_REPORT_MODE:
        regBckupFdiagctrl = sl_flashWREG->FDIAGCTRL;
        /*  Clear all the status errors*/
        sl_flashWREG->EESTATUS |= ~0;
        /* Clear this to unfree error reporting */
        flashread=sl_flashWREG->EEUNCERRADD;
        /*load FEMU_XX regs in order to generate ecc and use it for next operations*/
        sl_flashWREG->FEMUADDR=ADDR_DATA_MSW;
        fdiagCtrl |= F021F_FDIAGCTRL_DMODE_SYN_RPT;
        sl_flashWREG->FEMUDMSW = FEE_TEST_DATA;
        sl_flashWREG->FEMUDLSW = FEE_TEST_DATA;
        /* Store ECC*/
        ecc = sl_flashWREG->FEMUECC;

        sl_flashWREG->FEMUDMSW = FEE_TEST_DATA;
        sl_flashWREG->FEMUDLSW = FEE_TEST_DATA ^ BIT(FEE_ERROR_POS);
        /* Calculate the Syndrome value */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        syndrome = ecc ^ sl_flashWREG->FEMUECC;

        /* Set the data back to the original data */
        sl_flashWREG->FEMUDMSW = FEE_TEST_DATA;
        sl_flashWREG->FEMUDLSW = FEE_TEST_DATA;
        /* Set the FDIAGCTRL register*/
        /* Set SECDED mode as fee as only fee is supported for this mode */
        sl_flashWREG->FDIAGCTRL = fdiagCtrl|F021F_FDIAGCTRL_DIAGECCSEL_FEE;
        /* Introduce the single bit error corresponding to the calculated syndrome */
        sl_flashWREG->FEMUDMSW = FEE_TEST_DATA;
        sl_flashWREG->FEMUDLSW = FEE_TEST_DATA ^ BIT(FEE_ERROR_POS);
        /* Trigger the diagnosis */
        sl_flashWREG->FDIAGCTRL |=(F021F_FDIAGCTRL_DIAG_TRIG);
        /* Now the FEMUECC would contain the actual syndrome, as the mode is FLASH_ECC_SYN_REPORT_MODE
         * Check this value to the calculated syndrome, if it is match the diagnosis passes */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if((syndrome) == sl_flashWREG->FEMUECC){
            *fee_stResult = ST_PASS;
        }
        else
        {
            *fee_stResult = ST_FAIL;
        }
        /* Clear the diag mode settings */
        sl_flashWREG->FDIAGCTRL &=~(F021F_FDCTRL_DMODE_DATA_CORR|F021F_FDIAGCTRL_DIAG_EN_MASK); /* Clear Diagnostics enable key */
        sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;
        /*Clear the status set during the test*/
        sl_flashWREG->EESTATUS |= ~0;
        retVal = TRUE;
        /* Read and unfreeze error reporting */
        flashread=sl_flashWREG->EEUNCERRADD;
        break;

    case FEE_ECC_MALFUNCTION_MODE1:
        regBckupFdiagctrl = sl_flashWREG->FDIAGCTRL;
        /* Clear this to unfree error reporting */
        flashread=sl_flashWREG->EEUNCERRADD;

        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
        status functions to get the pass/failure information*/        
        regBkupIntEnaSet = sl_esmREG->IESR4;
        regBckupErrInfulence = sl_esmREG->IEPSR4;
        if((FALSE) == _sl_fault_injection){
            sl_esmREG->IECR4 = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
            sl_esmREG->IEPCR4 = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
        }

        /* Set diagmode and SECDED mode as fee as only fee is supported for this mode */
        fdiagCtrl |= F021F_FDIAGCTRL_DMODE_MALFUNC1|F021F_FDIAGCTRL_DIAGECCSEL_FEE;
        /*  Clear all the status errors*/
        sl_flashWREG->EESTATUS |= (uint32)~0;

        /* Enable EDACMODE and put a non zero value is FRAWECC*/
        sl_flashWREG->FDIAGCTRL = fdiagCtrl;
        sl_flashWREG->FRAWECC = 1U;
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
        /* Trigger the test */
        sl_flashWREG->FDIAGCTRL |=F021F_FDIAGCTRL_DIAG_TRIG;
        /* Check error */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        /*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
        if((F021F_EE_STATUS_CME == ((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_CME))&&
            (GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR) == (sl_esmREG->SR4[0] & GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR)))){
            *fee_stResult = ST_PASS;
        }
        else
        {
            *fee_stResult = ST_FAIL;
        }
        /* Clear ESM bits */
        sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
        /* Clear the diag mode settings */
        sl_flashWREG->FDIAGCTRL &=~(F021F_FDCTRL_DMODE_DATA_CORR|F021F_FDIAGCTRL_DIAG_EN_MASK); /* Clear Diagnostics enable key */
        sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;
        /*Clear the status set during the test*/
        sl_flashWREG->EESTATUS |= ~0;
        retVal = TRUE;
        /* Read and unfreeze error reporting */
        flashread=sl_flashWREG->EEUNCERRADD;

        /*Restore grp1 esm interrupt enable*/
        sl_esmREG->IESR4 = regBkupIntEnaSet;
        sl_esmREG->IEPSR4 = regBckupErrInfulence;
        break;

    case FEE_ECC_MALFUNCTION_MODE2:
        regBckupFdiagctrl = sl_flashWREG->FDIAGCTRL;
        sl_flashWREG->FDIAGCTRL=0U;
        /* Read and unfreeze error reporting */
        flashread=sl_flashWREG->EEUNCERRADD;

        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
        status functions to get the pass/failure information*/        
        regBkupIntEnaSet = sl_esmREG->IESR4;
        regBckupErrInfulence = sl_esmREG->IEPSR4;
        if((FALSE) == _sl_fault_injection){
            sl_esmREG->IECR4= GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
            sl_esmREG->IEPCR4 = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
        }

        /* Set diagmode and SECDED mode as fee as only fee is supported for this mode */
        sl_flashWREG->FDIAGCTRL|= F021F_FDIAGCTRL_DIAGECCSEL_FEE;
        sl_flashWREG->FDIAGCTRL|= F021F_FDIAGCTRL_DMODE_MALFUNC2;

        /*  Clear all the status errors*/
        sl_flashWREG->EESTATUS |= ~0;

        /* Enable EDACMODE*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        BIT_SET(sl_flashWREG->FDIAGCTRL,F021F_FDIAGCTRL_DIAG_EN_KEY);
        /* Invert data for the ecc calculation and put zero in FRAWECC */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        sl_flashWREG->FRAWDATAH = ~sl_flashWREG->FRAWDATAH;
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        sl_flashWREG->FRAWDATAL = ~sl_flashWREG->FRAWDATAL;
        sl_flashWREG->FRAWECC = 0U;
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
        /* Trigger the test */
        sl_flashWREG->FDIAGCTRL |=F021F_FDIAGCTRL_DIAG_TRIG;

        /* Check Error */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        /*SAFETYMCUSW 96 S MR: 6.2,10.1,10.2,12.1,12.6 <APPROVED> Comment_25*/
        if((F021F_EE_STATUS_CME == ((uint32)sl_flashWREG->EESTATUS & F021F_EE_STATUS_CME))&&
                (GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR) == (sl_esmREG->SR4[0] & GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR)))){
            *fee_stResult = ST_PASS;
        }
        else
        {
            *fee_stResult = ST_FAIL;
        }
        /* Clear ESM bits */
        sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
        /* Clear the diag mode settings */
        sl_flashWREG->FDIAGCTRL &=~(F021F_FDCTRL_DMODE_DATA_CORR|F021F_FDIAGCTRL_DIAG_EN_MASK); /* Clear Diagnostics enable key */
        sl_flashWREG->FDIAGCTRL = regBckupFdiagctrl;
        /*Clear the status set during the test*/
        sl_flashWREG->EESTATUS |= ~0;
        retVal = TRUE;
        /* Read and unfreeze error reporting */
        flashread=sl_flashWREG->EEUNCERRADD;
        /*Restore grp1 esm interrupt enable*/
        sl_esmREG->IESR4 = regBkupIntEnaSet;
        sl_esmREG->IEPSR4 = regBckupErrInfulence;
        break;
    default:
        retVal = FALSE;
        break;
    }
    /* If the test was fault injection and not self test, then do not mask the call back & clear the *
     *  FAULT_INJECTION flag for subsequent runs*/
    if((TRUE) == _sl_fault_injection){
            _sl_fault_injection=FALSE;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_14*/
    SL_Log_Result(FUNC_ID_ST_FLASH, testType, *fee_stResult , 0U);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}
#endif

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
/*SAFETYMCUSW 62 D MR: 16.7 <APPROVED> Comment_17*/
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
boolean SL_SelfTest_PBIST (register SL_SelfTestType testType, register uint64 ramGroup, register uint32 algoInfo)
#else
boolean SL_SelfTest_PBIST (register SL_SelfTestType testType, register uint32 ramGroup, register uint32 algoInfo)
#endif
{
    register uint32 tempVal;
    register boolean retVal = FALSE;

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if (0 != sl_pbistREG->PACT)
    {
        SL_Log_Error(FUNC_ID_ST_PBIST, ERR_TYPE_ENTRY_CON, 1u);
        retVal = FALSE;
        return retVal;
    }

    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_PBIST, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return retVal;
    }

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_PBIST, ERR_TYPE_ENTRY_CON, 2u);
        return(FALSE);
    }
#endif

#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /*SAFETYMCUSW 437 S MR: 17.3 <APPROVED> Comment_19*/
    if ((_SELFTEST_PBIST_MIN > testType) || (_SELFTEST_PBIST_MAX < testType)) {
        SL_Log_Error(FUNC_ID_ST_PBIST, ERR_TYPE_PARAM, 0u);
        retVal = FALSE;
        return retVal;
    }
    if (ALL_PBIST_RAM_GROUPS != (ALL_PBIST_RAM_GROUPS | ramGroup)) {
        SL_Log_Error(FUNC_ID_ST_PBIST, ERR_TYPE_PARAM, 1u);
        retVal = FALSE;
        return retVal;
    }
    if (ALL_PBISALGO != (ALL_PBISALGO | algoInfo)) {
        SL_Log_Error(FUNC_ID_ST_PBIST, ERR_TYPE_PARAM, 2u);
        retVal = FALSE;
        return retVal;
    }
#endif
    switch(testType) {
        case PBIST_EXECUTE:
        case PBIST_EXECUTE_OVERRIDE:
            /* PBIST ROM clock frequency = HCLK frequency /2 */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            BF_SET(sl_systemREG1->MSTGCR,ROM_CLK_SRC_HCKL_DIV_2,BF_ROM_DIV_START,BF_ROM_DIV_LENGTH);
            /* Enable PBIST controller */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            sl_systemREG1->MSINENA = SYSREG1_MSINENA_MSIENA;
            /* Enable PBIST Contrlller */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            BF_SET(sl_systemREG1->MSTGCR,ENABLE_MEM_STEST_CTRL,BF_MSTGENA_START,BF_MSTGENA_LENGTH);
            /* Wait for 32 VBUS Clocks */
            for (tempVal=0u; tempVal<(VBUS_CLK_CYCLES + (VBUS_CLK_CYCLES * 1u)); tempVal++)
            {

            }
            /* Enable PBIST clocks and ROM clock */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            sl_pbistREG->PACT = (PBIST_PACT_PACT1 | PBIST_PACT_PACT0);
            /* Select the algorithms */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            sl_pbistREG->ALGO = algoInfo;
            if (PBIST_EXECUTE_OVERRIDE == testType) {
                /* Override RINFOx with ROM contents */
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                sl_pbistREG->OVER = ENABLE_OVERRIDE;
            }
            else {
                /* Select RAM groups */
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                sl_pbistREG->RINFOL = (uint32)(ramGroup&0xFFFFFFFFu);
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                sl_pbistREG->RINFOU = 0x00000000u; /* No RAM group between 33-64 */
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
                sl_pbistREG->RINFOU = (uint32)((ramGroup>>32)&0xFFFFFFFFu); /* No RAM group between 33-64 */
#endif
                /* Do not override RINFOx with ROM contents */
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                 sl_pbistREG->OVER = DISABLE_OVERRIDE;
            }
            /* Algorithm code is loaded from ROM */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            sl_pbistREG->ROM = ALG_RAM_FROM_ROM;
            /* Start PBIST */
            /* Note: If executing on TCM RAM, Stack Contents are corrupted, so be careful with return data */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            sl_pbistREG->DLR = (PBIST_DLR_DLR4 | PBIST_DLR_DLR2);
            retVal = TRUE;
         break;
        default:
            break;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_PBIST, testType, (SL_SelfTest_Result)retVal , 0u);
#endif
    return(retVal);
}


/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_Status_PBIST(SL_PBIST_FailInfo * param1)
{
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /*LDRA_INSPECTWINDOW 50 */
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if (FALSE == CHECK_RANGE_RAM_PTR(param1)) {
        SL_Log_Error(FUNC_ID_ST_PBIST_STATUS, ERR_TYPE_PARAM, 0u);
        return FALSE;
    }
#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_PBIST_STATUS, ERR_TYPE_ENTRY_CON, 0u);
        return FALSE;
    }
#endif
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if ((sl_systemREG1->MSTCGSTAT & 0x1u) == 0x0u)
        {
           return FALSE;
        }
        else
        {
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
        	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            if ((sl_pbistREG->FSRF0 == 0u) && (sl_pbistREG->FSRF1 == 0u)) {
                /* Return PASS*/
                param1->stResult             = ST_PASS;
                param1->port0FailCount         = 0u;
                param1->port1FailCount         = 0u;
                param1->port0FailAddress    = 0u;
                param1->port1FailAddress    = 0u;
                param1->port0FailData       = 0u;
                param1->port1FailData       = 0u;
            } else {
                /* Store the failure data in OUT params */
                param1->stResult            = ST_FAIL;
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                /*SAFETYMCUSW 439 S MR: 11.3 <APPROVED> "The base address of the peripheral registers are actually macros" */
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                param1->port0FailCount      = (uint8) (sl_pbistREG->FSRC0 & 0xFFu);
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                param1->port1FailCount      = (uint8) (sl_pbistREG->FSRC1 & 0xFFu);
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                param1->port0FailAddress    = (uint16) (sl_pbistREG->FSRA0 & 0xFFFFu);
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                param1->port1FailAddress    = (uint16) (sl_pbistREG->FSRA1 & 0xFFFFu);
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                param1->port0FailData       = sl_pbistREG->FSRDL0;
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                param1->port1FailData       = sl_pbistREG->FSRDL1;
            }
        /* Stop the PBIST */
        SL_SelfTest_PBIST_StopExec();
#if(FUNC_RESULT_LOG_ENABLED == 1)
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
        SL_Log_Result(FUNC_ID_ST_PBIST_STATUS, (SL_SelfTestType)0, param1->stResult, 0u);
#endif
        }
    return TRUE;
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_WaitCompletion_PBIST(void)
{
    register boolean retVal = FALSE;
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if ((sl_systemREG1->MSTCGSTAT & 0x1u) == 0x1u) {
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
        sl_systemREG1->MSTCGSTAT = 0x1u;
        retVal = TRUE;
    } else {
        retVal = FALSE;
    }
    return retVal;
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
boolean SL_SelfTest_PBIST_StopExec(void)
{
    /* Stop the PBIST */
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    sl_pbistREG->PACT = 0x0u;
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> Comment_10*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    BF_SET(sl_systemREG1->MSTGCR,DISABLE_MEM_STEST_CTRL,BF_MINITGENA_START,BF_MINITGENA_LENGTH);
    return(TRUE);
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_STC(register SL_SelfTestType testType, register boolean bMode, register SL_STC_Config * config)
{
    register uint32 tempVal = 0U;
    boolean retval = FALSE;

#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in rage */
    /*LDRA_INSPECTWINDOW 50 */
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 437 S MR: 17.3 <APPROVED> Comment_19*/
    if ((_SELFTEST_STC_MIN > testType) || (_SELFTEST_STC_MAX < testType)||
            !CHECK_RANGE_RAM_PTR(config)) {
        SL_Log_Error(FUNC_ID_ST_STC, ERR_TYPE_PARAM, 0U);
        retval = FALSE;
        return(retval);
    }
    /* Error if interval count is greater than supported */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    if(config->intervalCount>STC_MAX_INTERVALS)
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
    if(config->intervalCount>STC1_MAX_INTERVALS)
#endif
    {
        SL_Log_Error(FUNC_ID_ST_STC, ERR_TYPE_PARAM, 0U);
        retval = FALSE;
        return(retval);
    }
    if(config->stcClockDiv>STC_MAX_CLOCK_DIV){
        SL_Log_Error(FUNC_ID_ST_STC, ERR_TYPE_PARAM, 0U);
        retval = FALSE;
        return(retval);
    }
#endif
#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /* Error if not in privilege mode */
        if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
            SL_Log_Error(FUNC_ID_ST_STC, ERR_TYPE_ENTRY_CON, 2U);
            retval = FALSE;
            return(retval);
    }

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_STC, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }
#endif

    /* Configure the clock divider */
    sl_systemREG2->STCCLKDIV = ((config->stcClockDiv & 0x07u) << 24);

    /* Configure the interval count & restart/continue */
    tempVal = 0U;
    tempVal |= ((uint32)config->intervalCount << STC_STCGCR0_INTCOUNT_START);
    if (TRUE == config->restartInterval0) {
        tempVal |= STC_STCGCR0_RS_CNT;
    }
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    sl_stcREG->STCGCR0 = tempVal;

    /* If in compare selfcheck (fault injection) mode then setup STCSCSCR */
    if (STC_COMPARE_SELFCHECK == testType) {
        sl_stcREG->STCSCSCR = (uint32)(STC_STCSCSCR_FAULT_INS|STC_STCSCSCR_SELF_CHECK_KEY);
    }

    /* Setup the timeout value */
    sl_stcREG->STCTPR = config->timeoutCounter;

    /* Enable STC run */
    sl_stcREG->STCGCR1 = STC_STCGCR1_STC_ENA;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)

    sl_stcREG1->STCGCR0 = tempVal;

    /* If in compare selfcheck (fault injection) mode then setup STCSCSCR */
    if (STC1_COMPARE_SELFCHECK == testType) {
        sl_stcREG1->STCSCSCR = (uint32)(STC_STCSCSCR_FAULT_INS|STC_STCSCSCR_SELF_CHECK_KEY);
    }

    /* Setup the timeout value */
    sl_stcREG1->STCTPR = config->timeoutCounter;

    /* Segment 0 core select - to be run in parallel */
    BF_SET(sl_stcREG1->STCGCR1, STC_GCR1_SEG0_CORE_PARLL, STC_GCR1_SEG0_CORE_SEL_START, STC_GCR1_SEG0_CORE_SEL_LENGTH);

    /* Enable STC run */
    sl_stcREG1->STCGCR1 = STC_STCGCR1_STC_ENA;
#endif
    for (tempVal=0u; tempVal<32u; tempVal++) {
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    /* The system goes into reset after the test, and the result of the test has to be found out from there
     * Now logging the value of result as 0xEF which denotes 'do not know'state
     */
    SL_Log_Result(FUNC_ID_ST_STC,testType, (SL_SelfTest_Result)0xEFU , 0u);
#endif
    /* Kick off the STC execution */
    _SL_Kickoff_STC_execution();

    /* Execution never comes here !!! */
    retval = TRUE;
    return (retval);
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_Status_STC (SL_STC_FailInfo * failInfostc)
{
    boolean retVal = FALSE;
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if (STC_STCGSTAT_TEST_DONE == (STC_STCGSTAT_TEST_DONE & sl_stcREG->STCGSTAT)) 
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if (STC_STCGSTAT_TEST_DONE == (STC_STCGSTAT_TEST_DONE & sl_stcREG1->STCGSTAT)) 
#endif
    {
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        retVal = TRUE;/* Test was completed */

        /* Get test results */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (STC_STCGSTAT_TEST_FAIL == (STC_STCGSTAT_TEST_FAIL & sl_stcREG->STCGSTAT)) {
            /* Test completed & failed */
            failInfostc->stResult = ST_FAIL;
            /* Clear the error */
            sl_stcREG->STCGSTAT = STC_STCGSTAT_TEST_FAIL;
        }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        retVal = TRUE;/* Test was completed */
            /* Get test results */
        if (STC_STCGSTAT_TEST_FAIL == (STC_STCGSTAT_TEST_FAIL & sl_stcREG1->STCGSTAT)) {
            /* Test completed & failed */
            failInfostc->stResult = ST_FAIL;
            /* Clear the error */
            sl_stcREG1->STCGSTAT = STC_STCGSTAT_TEST_FAIL;
        }
#endif
        else {
            /* Test completed & passed */
            failInfostc->stResult = ST_PASS;
        }
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (STC_STCFSTAT_CPU1_FAIL == (STC_STCFSTAT_CPU1_FAIL & sl_stcREG->STCFSTAT)) {
            sl_stcREG->STCFSTAT = STC_STCFSTAT_CPU1_FAIL;
            failInfostc->CPU1Failure = ST_PASS;
        }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (STC_STCFSTAT_CPU1_FAIL == (STC_STCFSTAT_CPU1_FAIL & sl_stcREG1->STCFSTAT)) {
            sl_stcREG1->STCFSTAT = STC_STCFSTAT_CPU1_FAIL;
            failInfostc->CPU1Failure = ST_PASS;
        }
#endif
        else {
            failInfostc->CPU1Failure = ST_FAIL;
        }
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (STC_STCFSTAT_CPU2_FAIL == (STC_STCFSTAT_CPU2_FAIL & sl_stcREG->STCFSTAT)) {
            sl_stcREG->STCFSTAT = STC_STCFSTAT_CPU2_FAIL;
            failInfostc->CPU2Failure = ST_PASS;
        }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (STC_STCFSTAT_CPU2_FAIL == (STC_STCFSTAT_CPU2_FAIL & sl_stcREG1->STCFSTAT)) {
            sl_stcREG1->STCFSTAT = STC_STCFSTAT_CPU2_FAIL;
            failInfostc->CPU2Failure = ST_PASS;
        }
#endif
        else {
            failInfostc->CPU2Failure = ST_FAIL;
        }
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (STC_STCFSTAT_TO_ERR == (STC_STCFSTAT_TO_ERR & sl_stcREG->STCFSTAT)) {
            sl_stcREG->STCFSTAT = STC_STCFSTAT_TO_ERR;
            failInfostc->TimeOutFailure = ST_PASS;
        }
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (STC_STCFSTAT_TO_ERR == (STC_STCFSTAT_TO_ERR & sl_stcREG1->STCFSTAT)) {
            sl_stcREG1->STCFSTAT = STC_STCFSTAT_TO_ERR;
            failInfostc->TimeOutFailure = ST_PASS;
        }
#endif
        else {
            failInfostc->TimeOutFailure = ST_FAIL;
        }
    } else {
        /* Test did not complete */
        retVal = FALSE;
    }

    return(retVal);
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
boolean SL_SelfTest_PSCON(SL_SelfTestType testType, boolean mode, SL_PSCON_FailInfo * param1)
{
    register boolean retVal = TRUE;
    register uint32 regBkupIntEnaSet, regBckupErrInfulence;
    uint32 regbackup;

#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in rage */
    /*LDRA_INSPECTWINDOW 50 */
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if ((_SELFTEST_PSCON_MIN > testType) || (_SELFTEST_PSCON_MAX < testType) || !CHECK_RANGE_RAM_PTR(param1)) {
        SL_Log_Error(FUNC_ID_ST_PSCON, ERR_TYPE_PARAM, 0U);
            return(FALSE);
    }
#endif
#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /* Error if not in privilege mode */
    if ((PSCON_PMA_TEST != testType) && (ARM_MODE_USR == _SL_Get_ARM_Mode())) {
        SL_Log_Error(FUNC_ID_ST_PSCON, ERR_TYPE_ENTRY_CON, 2U);
        return(FALSE);
    }
    if ((PSCON_PMA_TEST == testType) && (ARM_MODE_USR != _SL_Get_ARM_Mode())) {
            SL_Log_Error(FUNC_ID_ST_PSCON, ERR_TYPE_ENTRY_CON, 2U);
            return(FALSE);
    }


    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_PSCON, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif
    retVal = TRUE;
    switch(testType) {
        case PSCON_SELF_TEST:
            /*flag is set to indicate the current test which is ongoing and
            These flags are used in the sl_esm.c so as to mask the esm callback*/  
        	SL_FLAG_SET(PSCON_SELF_TEST);
            sl_pmmREG->PRCKEYREG = PMM_PRCKEYREG_MKEY_ST_MODE;
            if ((TRUE) == mode) {
                /* Wait for the self-test to complete & get results*/
                /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
                while (TRUE != SL_SelfTest_Status_PSCON(param1)) {
                }
            }
            break;

        case PSCON_ERROR_FORCING:
        case PSCON_ERROR_FORCING_FAULT_INJECT:
            /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
        status functions to get the pass/failure information*/        
            regBkupIntEnaSet = sl_esmREG->IESR4;
            regBckupErrInfulence = sl_esmREG->IEPSR4;
            if(PSCON_ERROR_FORCING == testType){
                /*flag is set to indicate the current test which is ongoing and
               These flags are used in the sl_esm.c so as to mask the esm callback*/
            	SL_FLAG_SET(PSCON_ERROR_FORCING);
                sl_esmREG->IECR4 = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_COMPARE_ERR);
                sl_esmREG->IEPCR4 = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_COMPARE_ERR);
                sl_esmREG->IECR4 = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR);
                sl_esmREG->IEPCR4 = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR);
            }
            else
            {
                /*flag is set to indicate the current test which is ongoing and
                These flags are used in the sl_esm.c so as to mask the esm callback*/
            	SL_FLAG_SET(PSCON_ERROR_FORCING_FAULT_INJECT);
            }
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
            sl_pmmREG->PRCKEYREG = PMM_PRCKEYREG_MKEY_ERRFCG_MODE;
            if(PSCON_ERROR_FORCING == testType)
            {
                if (GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_COMPARE_ERR) == (sl_esmREG->SR4[0] & GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_COMPARE_ERR))) {
                    sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_COMPARE_ERR);
                    sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR);
                    param1->stResult = ST_PASS;
                } else {
                    param1->stResult = ST_FAIL;
                }
                /*clear the flags which indicate tests ongoing*/
                SL_FLAG_CLEAR(PSCON_ERROR_FORCING);
            }
            else
            {
            /*clear the flags which indicate tests ongoing*/
            	SL_FLAG_CLEAR(PSCON_ERROR_FORCING_FAULT_INJECT);
            }
            /*Restore grp1 esm interrupt enable*/
            sl_esmREG->IESR4 = regBkupIntEnaSet;
            sl_esmREG->IEPSR4 = regBckupErrInfulence;
            /* Revert to normal operation */
            sl_pmmREG->PRCKEYREG = PMM_PRCKEYREG_MKEY_LKSTEP_MODE;
            break;

        case PSCON_SELF_TEST_ERROR_FORCING:
        case PSCON_SELF_TEST_ERROR_FORCING_FAULT_INJECT:
            /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
             /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
            status functions to get the pass/failure information*/        
            regBkupIntEnaSet = sl_esmREG->IESR4;
            regBckupErrInfulence = sl_esmREG->IEPSR4;
            if(PSCON_SELF_TEST_ERROR_FORCING ==  testType){
                /*flag is set to indicate the current test which is ongoing and
                These flags are used in the sl_esm.c so as to mask the esm callback*/
            	SL_FLAG_SET(PSCON_SELF_TEST_ERROR_FORCING);
                sl_esmREG->IECR4 = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR);
                sl_esmREG->IEPCR4 = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR);
            }
            else
            {
                /*flag is set to indicate the current test which is ongoing and
                These flags are used in the sl_esm.c so as to mask the esm callback*/ 
            	SL_FLAG_SET(PSCON_SELF_TEST_ERROR_FORCING_FAULT_INJECT);
            }
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
            sl_pmmREG->PRCKEYREG = PMM_PRCKEYREG_MKEY_STERRF_MODE;
            if(PSCON_SELF_TEST_ERROR_FORCING == testType)
            {
                if (GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR) == (sl_esmREG->SR4[0] & GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR))) {
                    sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR);
                    param1->stResult = ST_PASS;
                } else {
                    param1->stResult = ST_FAIL;
                }
                /*clear the flags which indicate tests ongoing*/
                SL_FLAG_CLEAR(PSCON_SELF_TEST_ERROR_FORCING);
            }
            else
            {
            /*clear the flags which indicate tests ongoing*/
            	SL_FLAG_CLEAR(PSCON_SELF_TEST_ERROR_FORCING_FAULT_INJECT);
            }
            /*Restore grp1 esm interrupt enable*/
            sl_esmREG->IESR4 = regBkupIntEnaSet;
            sl_esmREG->IEPSR4 = regBckupErrInfulence;
            /* Revert to normal operation */
            sl_pmmREG->PRCKEYREG = PMM_PRCKEYREG_MKEY_LKSTEP_MODE;
            break;

        case PSCON_PMA_TEST:
            /* Make sure we are in non-privilege (user) mode */
            if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
                retVal = TRUE;
                regbackup = sl_pmmREG->PRCKEYREG;
                /*flag is set to indicate the current test which is ongoing and
                These flags are used in the sl_esm.c so as to mask the esm callback*/
                SL_FLAG_SET(PSCON_PMA_TEST);
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
                /* Try writing to GLOBALCTRL1.AUTO_CLK_WAKE_ENA, which should result in an abort*/
                sl_pmmREG->PRCKEYREG = 0x0000000FU;
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                if(regbackup == sl_pmmREG->PRCKEYREG)
                {
                    param1->stResult = ST_PASS;
                }
                else
                {
                    param1->stResult = ST_FAIL;
                }
            } else {
                /* Error, must be in user mode to run this test */
                retVal = FALSE;
            }
            /*clear the flags which indicate tests ongoing*/
            SL_FLAG_CLEAR(PSCON_PMA_TEST);
            /* Revert to normal operation */
            sl_pmmREG->PRCKEYREG = PMM_PRCKEYREG_MKEY_LKSTEP_MODE;
            break;

        default:
            /* Revert to normal operation */
            sl_pmmREG->PRCKEYREG = PMM_PRCKEYREG_MKEY_LKSTEP_MODE;
            break;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_PSCON, testType, param1->stResult , 0U);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}
#endif

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
boolean SL_SelfTest_Status_PSCON (SL_PSCON_FailInfo * param1)
{
    boolean retVal = FALSE;
    /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
    /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
    /*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if ((PMM_PSCON_LPDDCSTAT1_LSTC != ((uint32)(PMM_PSCON_LPDDCSTAT1_LSTC & sl_pmmREG->LPDDCSTAT1))) &&
            (PMM_PSCON_MPDDCSTAT1_MSTC != ((uint32)(PMM_PSCON_MPDDCSTAT1_MSTC & sl_pmmREG->MPDDCSTAT1)))){
        retVal = FALSE;
    } else {
        /* If one of the Power or Memory domains have failed, mark the ST as failure */
    	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
    	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
    	/*SAFETYMCUSW 408 S MR:12.4 <APPROVED> Comment_12*/
    	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if  ((0u == (PMM_PSCON_LPDDCSTAT2_LSTE & sl_pmmREG->LPDDCSTAT2)) &&
                (0u == (PMM_PSCON_MPDDCSTAT2_MSTE & sl_pmmREG->MPDDCSTAT2))) {
            param1->stResult = ST_PASS;
        } else {
            param1->stResult = ST_FAIL;
        }

        param1->pdFailure = sl_pmmREG->LPDDCSTAT2;
        param1->mdFailure = sl_pmmREG->MPDDCSTAT2;
        /*clear the flags which indicate tests ongoing*/
        SL_FLAG_CLEAR(PSCON_SELF_TEST);
        /* Revert to normal operation */
        sl_pmmREG->PRCKEYREG = PMM_PRCKEYREG_MKEY_LKSTEP_MODE;
        retVal = TRUE;
    }
    return(retVal);
}
#endif

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_EFuse (SL_SelfTestType testType, boolean bMode, SL_EFuse_Config * config)
{
    uint32 regBkup;
    volatile uint32 regBkupIntEnaSet, regBckupErrInfulence;
    volatile boolean _sl_fault_injection = FALSE;
    boolean retVal = FALSE;
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in range */
    /*LDRA_INSPECTWINDOW 50 */
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if ((_SELFTEST_EFUSE_MIN > testType) || (_SELFTEST_EFUSE_MAX < testType) || !CHECK_RANGE_RAM_PTR(config)){
        SL_Log_Error(FUNC_ID_ST_EFUSE, ERR_TYPE_PARAM, 0U);
        return(FALSE);
    }
#endif
#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_EFUSE, ERR_TYPE_ENTRY_CON, 2U);
        retVal =  FALSE;
        return(FALSE);
    }
    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_EFUSE, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif

    switch(testType) {
    case EFUSE_SELF_TEST_AUTOLOAD:
        /* \todo */
        retVal = FALSE;
        break;

    case EFUSE_SELF_TEST_ECC:
        retVal = TRUE; /* Inputs are fine. Tests are run :) */
        /*flag is set to indicate the current test which is ongoing and
        These flags are used in the sl_esm.c so as to mask the esm callback*/
        SL_FLAG_SET(EFUSE_SELF_TEST_ECC);
        /* Configure the self-test parameters */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        sl_efcREG->SELF_TEST_CYCLES = config->numPatterns;
        sl_efcREG->SELF_TEST_SIGN   = config->seedSignature;
        /* Kick off the self-test */
        sl_efcREG->BOUNDARY = EFC_BOUNDARY_INPUT_EN | EFC_BOUNDARY_SELF_TEST_EN;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        sl_efcREG->ST_CY = config->numPatterns;
        sl_efcREG->ST_SIG   = config->seedSignature;
        /* Kick off the self-test */
        sl_efcREG->BOUND = EFC_BOUNDARY_INPUT_EN | EFC_BOUNDARY_SELF_TEST_EN;
#endif
        /* If mode is synchronous, update the result now*/
        if((TRUE)==bMode){
            /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
            while((TRUE) != SL_SelfTest_Status_EFuse(&config->failInfo)){}
        }
        break;

    case EFUSE_SELF_TEST_STUCK_AT_ZERO:
        retVal = TRUE; /* Inputs are fine. Tests are run :) */
        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
        status functions to get the pass/failure information*/        
        regBkupIntEnaSet = sl_esmREG->IESR4;
        regBckupErrInfulence = sl_esmREG->IEPSR4;
        sl_esmREG->IECR4 = GET_ESM_BIT_NUM(ESM_G1ERR_EFUSE_SELTEST_ERR);
           sl_esmREG->IEPCR4 = GET_ESM_BIT_NUM(ESM_G1ERR_EFUSE_SELTEST_ERR);

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBkup = (sl_efcREG->BOUNDARY);
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBkup = (sl_efcREG->BOUND);
#endif
        /* Enable the output for errors */
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        sl_efcREG->BOUNDARY = (ST_BOUNDARY_VALUE | EFC_BOUNDARY_OUTPUT_EN);
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        sl_efcREG->BOUND = (ST_BOUNDARY_VALUE | EFC_BOUNDARY_OUTPUT_EN);
#endif
        /* Check the efuse controller output for the above error outputs */
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (ST_PIN_VALUE == (ST_PIN_VALUE & sl_efcREG->PINS)) {
            /* If all outputs are set, then test has passed */
            config->failInfo.stResult = ST_PASS;
        } else {
            /* One or more ouputs is/are stuck at zero !! return error */
            config->failInfo.stResult = ST_FAIL;
        }
        /* Clear interrupts & Error Pins only for this event but only if this is the only event */
        if ((uint32) (1u << ESM_G3ERR_EFUSE_AUTOLOAD_ERR) == (uint32) (sl_esmREG->SR1[2] & (1u << ESM_G3ERR_EFUSE_AUTOLOAD_ERR))) {
            _SL_HoldNClear_nError();
        }

        /* Clear any ESM interrupts */
        sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(ESM_G1ERR_EFUSE_ERR_OTHER)|GET_ESM_BIT_NUM(ESM_G1ERR_EFUSE_SELTEST_ERR);
        sl_esmREG->SR1[2] = GET_ESM_BIT_NUM(ESM_G3ERR_EFUSE_AUTOLOAD_ERR); /* \todo Should clear the ESMKEYR if this event is configured for nERROR */
        /*Restore grp1 esm interrupt enable*/
        sl_esmREG->IESR4 = regBkupIntEnaSet;
        sl_esmREG->IEPSR4 = regBckupErrInfulence;

        /* Restore boundary register */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
        sl_efcREG->BOUNDARY = regBkup;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
        sl_efcREG->BOUND = regBkup;
#endif
        break;
    default:
        break;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_EFUSE, testType, config->failInfo.stResult , 0U);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}


/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_Status_EFuse (SL_EFuse_FailInfo * failInfoefuse)
{
    boolean retVal = FALSE;

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if ((uint32) (EFC_BOUNDARY_INPUT_EN | EFC_BOUNDARY_SELF_TEST_EN)
            == (uint32) (sl_efcREG->BOUNDARY & (EFC_BOUNDARY_INPUT_EN | EFC_BOUNDARY_SELF_TEST_EN))) 
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if ((uint32) (EFC_BOUNDARY_INPUT_EN | EFC_BOUNDARY_SELF_TEST_EN)
            == (uint32) (sl_efcREG->BOUND & (EFC_BOUNDARY_INPUT_EN | EFC_BOUNDARY_SELF_TEST_EN))) 
#endif
    {
        /** ECC Self test has been enabled, check the results accordingly */
    	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (EFC_PINS_SELF_TEST_DONE == (uint32) (sl_efcREG->PINS & EFC_PINS_SELF_TEST_DONE)) {
            /* self test is complete */
            retVal = TRUE;
            /* Now check the status of the self test */
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            if ((EFC_PINS_SELF_TEST_ERROR != (uint32) (sl_efcREG->PINS & EFC_PINS_SELF_TEST_ERROR)))
            {
                /*EFUSE ECC*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
            	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                if(EFC_NO_ERROR == (sl_efcREG->ERROR & EFC_ERROR_CODE))
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                if(EFC_NO_ERROR == (sl_efcREG->ERR_STAT & EFC_ERROR_CODE))
#endif
                {
                    if(0==(sl_esmREG->SR4[0] &
                            (GET_ESM_BIT_NUM(ESM_G1ERR_EFUSE_ERR_OTHER)|GET_ESM_BIT_NUM(ESM_G1ERR_EFUSE_SELTEST_ERR)))){
                        failInfoefuse->stResult = ST_PASS;
                    }
                }
            } else {
                /* Self test failed */
                failInfoefuse->stResult = ST_FAIL;
            }
            /* Load the error info anyways */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            failInfoefuse->failInfo= (SL_EFuse_Error)(sl_efcREG->ERROR & EFC_ERROR_CODE);
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            failInfoefuse->failInfo= (SL_EFuse_Error)(sl_efcREG->ERR_STAT & EFC_ERROR_CODE);
#endif
            /*clear the flags which indicate tests ongoing*/
            SL_FLAG_CLEAR(EFUSE_SELF_TEST_ECC);
        } else {
            /* self test is not complete */
            retVal = FALSE;
        }
    } else {
        /* Since no test was triggered, return FALSE */
        retVal = FALSE;
    }

    return (retVal);
}

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_CCMR4F (SL_SelfTestType testType, boolean bMode, SL_CCMR4F_FailInfo * config)
{
    boolean retVal = FALSE;
    volatile boolean _sl_fault_injection = FALSE;
    volatile uint32 regBkupIntEnaSet, regBckupErrInfulence;
    if(CCMR4F_ERROR_FORCING_TEST_FAULT_INJECT == testType){
        _sl_fault_injection=TRUE;
    }
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_20*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_20*/
    if (FALSE == (boolean)CHECK_RANGE_RAM_PTR(config)) {
        SL_Log_Error(FUNC_ID_ST_CCMR4F, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return retVal;
    }
    if ((_SELFTEST_CCMR4F_MIN > testType) || (_SELFTEST_CCMR4F_MAX < testType)) {
        SL_Log_Error(FUNC_ID_ST_CCMR4F, ERR_TYPE_PARAM, 1u);
        retVal = FALSE;
        return retVal;
    }
#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_CCMR4F, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return(FALSE);
    }


    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_CCMR4F, ERR_TYPE_ENTRY_CON, 2u);
        return(FALSE);
    }
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if (CCMR4F_CCMKEYR_MODE_LOCKSTEP != (uint32) (ccmr4fREG1->_CCMKEYR & CCMR4F_CCMKEYR_MODE)) {
            retVal =  FALSE;
    }
    else
    {
        switch(testType) {
            case CCMR4F_SELF_TEST:
                /* Set mode to self_test but first ensure that a test is not in process*/
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
                  /*flag is set to indicate the current test which is ongoing and
                These flags are used in the sl_esm.c so as to mask the esm callback*/
            	SL_FLAG_SET(CCMR4F_SELF_TEST);
                ccmr4fREG1->_CCMKEYR = CCMR4F_CCMKEYR_MODE_SELFTEST;
                /* If sync mode loop for the test to complete */
                if ((TRUE) == bMode) {
                    /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
                    while (FALSE == SL_SelfTest_Status_CCMR4F(config))
                    {

                    }
                    retVal = TRUE;
                } else {
                /* Since the test is running return TRUE (though this func., call might have not started it) */
                retVal = TRUE;
            }
        break;
        case CCMR4F_ERROR_FORCING_TEST:
        case CCMR4F_ERROR_FORCING_TEST_FAULT_INJECT:
            retVal = TRUE;
            regBkupIntEnaSet = sl_esmREG->IESR1;
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            regBckupErrInfulence = sl_esmREG->EEPAPR1;
             if(CCMR4F_ERROR_FORCING_TEST ==testType) {
                 /*flag is set to indicate the current test which is ongoing and
                   These flags are used in the sl_esm.c so as to mask the esm callback*/
            	 SL_FLAG_SET(CCMR4F_ERROR_FORCING_TEST);
                 sl_esmREG->IECR1 = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST);
                    sl_esmREG->DEPAPR1 = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST);
             }
             /* Set the mode to error forcing mode */
             /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
             /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
             ccmr4fREG1->_CCMKEYR = CCMR4F_CCMKEYR_MODE_ERRFORCING;
             /* Error is injected & mode is switched back to lockstep (takes 1 cpu cycle).
             Wait for the mode to switch */
             /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
             /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
             /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
             while (CCMR4F_CCMKEYR_MODE_LOCKSTEP != (uint32) (ccmr4fREG1->_CCMKEYR & CCMR4F_CCMKEYR_MODE)) {
             }

             /* This section is applicable only for the CCMR4F_ERROR_FORCING_TEST case and not applicable for the
              * CCMR4F_ERROR_FORCING_TEST_FAULT_INJECT, where the application handler clears the error bits that
              * were set */
             if(CCMR4F_ERROR_FORCING_TEST ==testType) {
             /* Since this raises a G2 ESM Error, it will be cleared by the ESM handler. So check the shadow register */
             /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
              /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
              if(((uint32)1u == BF_GET(sl_esmREG->SSR2,BF_CCMR4_CMP_ERROR,BF_CCMR4_CMP_ERROR_LENGTH))) {
            	  config->stResult = ST_PASS;
              } else {
                  config->stResult = ST_FAIL;
              }

             /* Clear nERROR & Interrupt only if in normal self test mode */
                 /* Clear nERROR */
                 _SL_HoldNClear_nError();
                 /* Clear the interrupt */
                 /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
                 sl_esmREG->SR1[1] = GET_ESM_BIT_NUM(ESM_G2ERR_CCMR4_COMP);
                 sl_esmREG->SSR2 = GET_ESM_BIT_NUM(ESM_G2ERR_CCMR4_COMP);
                 sl_esmREG->SR1[0] = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST);
                 /*clear the flags which indicate tests ongoing*/
                 SL_FLAG_CLEAR(CCMR4F_ERROR_FORCING_TEST);
             }
             else
             {
                 /* Nothing to do incase of fault injection */
             }
             sl_esmREG->IESR1 = regBkupIntEnaSet;
             sl_esmREG->EEPAPR1 = regBckupErrInfulence;
             break;

        case CCMR4F_SELF_TEST_ERROR_FORCING:
             retVal = TRUE;
             /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
             /*the esm interrupts for selftests which generate group 1 interrupts is blocked.Users will have to rely on
             status functions to get the pass/failure information*/        
             regBkupIntEnaSet = sl_esmREG->IESR1;
             /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
             regBckupErrInfulence = sl_esmREG->EEPAPR1;
             sl_esmREG->IECR1 = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST);
                sl_esmREG->DEPAPR1 = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST);
             /*flag is set to indicate the current test which is ongoing and
             These flags are used in the sl_esm.c so as to mask the esm callback*/
                SL_FLAG_SET(CCMR4F_SELF_TEST_ERROR_FORCING);
             /* Set the mode to self test error forcing mode */
             /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
             ccmr4fREG1->_CCMKEYR = CCMR4F_CCMKEYR_MODE_STERRFORCG;
             /* Error is injected & mode is switched back to lockstep
             (takes 1 cpu cycle). Wait for the mode to switch */
             /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
             /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
             /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
             while (CCMR4F_CCMKEYR_MODE_LOCKSTEP != (uint32) (ccmr4fREG1->_CCMKEYR & CCMR4F_CCMKEYR_MODE)) {
             }

             /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
             if(GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST) == (sl_esmREG->SR1[0] & GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST))){
                 config->stResult = ST_PASS;
             }
             else
             {
                 config->stResult = ST_FAIL;
             }
             /* Clear nERROR */
             _SL_HoldNClear_nError();
             /* Clear the interrupt */
             /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
             sl_esmREG->SR1[0] = (1u << ESM_G1ERR_CCMR4_SELFTEST);
             /*clear the flags which indicate tests ongoing*/
             SL_FLAG_CLEAR(CCMR4F_ERROR_FORCING_TEST);
             /*Restore grp1 esm interrupt enable*/
             sl_esmREG->IESR1 = regBkupIntEnaSet;
             sl_esmREG->EEPAPR1 = regBckupErrInfulence;
             break;
        default:
             break;
        }
    }
    /* If the test was fault injection and not self test, then do not mask the call back & clear the *
     *  FAULT_INJECTION flag for subsequent runs*/
    if((TRUE) == _sl_fault_injection){
            _sl_fault_injection=FALSE;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_CCMR4F, testType, (SL_SelfTest_Result)retVal , 0u);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_CCMR5F (SL_SelfTestType testType, boolean bMode, SL_CCMR5F_FailInfo * config)
{
    boolean retVal = FALSE;
    volatile boolean _sl_fault_injection = FALSE;
    volatile uint32 regBkupIntEnaSet, regBckupErrInfulence;
    volatile uint32* ccmr5fKeyReg = (uint32*)0;

#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_20*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_20*/
    if (FALSE == (boolean)CHECK_RANGE_RAM_PTR(config)) {
        SL_Log_Error(FUNC_ID_ST_CCMR5F, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return retVal;
    }
    if ((_SELFTEST_CCMR5F_MIN > testType) || (_SELFTEST_CCMR5F_MAX < testType))
        {
        SL_Log_Error(FUNC_ID_ST_CCMR5F, ERR_TYPE_PARAM, 1u);
        retVal = FALSE;
        return retVal;
    }
#endif
    if ((testType >= CCMR5F_CPUCOMP_SELF_TEST) && 
            (testType <= CCMR5F_CPUCOMP_SELF_TEST_ERROR_FORCING)) 
        ccmr5fKeyReg = &(ccmr5fREG1->_CCMKEYR1);
    else if ((testType >= CCMR5F_VIMCOMP_SELF_TEST) && 
            (testType <= CCMR5F_VIMCOMP_SELF_TEST_ERROR_FORCING))
        ccmr5fKeyReg = &(ccmr5fREG1->_CCMKEYR2);
    else if ((testType >= CCMR5F_PDCOMP_SELF_TEST) && 
            (testType <= CCMR5F_PDCOMP_SELF_TEST_ERROR_FORCING)) 
        ccmr5fKeyReg = &(ccmr5fREG1->_CCMKEYR4);
    else if ((testType >= CCMR5F_INMCOMP_SELF_TEST) && 
            (testType <= CCMR5F_INMCOMP_SELF_TEST_ERROR_FORCING))
        ccmr5fKeyReg = &(ccmr5fREG1->_CCMKEYR3);

    if (ccmr5fKeyReg == (uint32*)0)
    {
        retVal = FALSE;
        return retVal;
    }

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_CCMR5F, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return(FALSE);
    }


    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active()){
        SL_Log_Error(FUNC_ID_ST_CCMR5F, ERR_TYPE_ENTRY_CON, 1u);
        return(FALSE);
    }

    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    if (CCMR5F_CCMKEYR_MODE_LOCKSTEP != (*ccmr5fKeyReg & CCMR5F_CCMKEYR_MODE)) {
        SL_Log_Error(FUNC_ID_ST_CCMR5F, ERR_TYPE_ENTRY_CON, 2u);
        return(FALSE);
    }
#endif
    if((CCMR5F_CPUCOMP_ERROR_FORCING_TEST_FAULT_INJECT == testType) ||
            (CCMR5F_VIMCOMP_ERROR_FORCING_TEST_FAULT_INJECT == testType) || 
            (CCMR5F_PDCOMP_ERROR_FORCING_TEST_FAULT_INJECT == testType)  || 
            (CCMR5F_INMCOMP_ERROR_FORCING_TEST_FAULT_INJECT == testType)) { 
        _sl_fault_injection=TRUE;
    }
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif
    switch(testType) {
        case CCMR5F_CPUCOMP_SELF_TEST:
        case CCMR5F_VIMCOMP_SELF_TEST:
        case CCMR5F_PDCOMP_SELF_TEST:
        case CCMR5F_INMCOMP_SELF_TEST:
            /* Set mode to self_test but first ensure that a test is not in process*/
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
           /* flag is set to indicate the current test which is ongoing and
            * These flags are used in the sl_esm.c so as to mask the esm
            * callback.
            * NOTE: even if the CCMR4F Selftest fails (selftest ESM occurs), the
            * application callback will not be made; application must use the
            * failinfo structure to get the status of selftest diagnostic */
            SL_FLAG_SET(testType);
            *ccmr5fKeyReg = CCMR5F_CCMKEYR_MODE_SELFTEST;
            /* If sync mode loop for the test to complete */
            if ((TRUE) == bMode) {
                /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
                while (FALSE == SL_SelfTest_Status_CCMR5F(testType, config)) {
                }
                retVal = TRUE;
            } else {
                /* Since the test is running return TRUE */
                retVal = TRUE;
            }
            break;

    case CCMR5F_CPUCOMP_ERROR_FORCING_TEST:
    case CCMR5F_CPUCOMP_ERROR_FORCING_TEST_FAULT_INJECT:
    case CCMR5F_VIMCOMP_ERROR_FORCING_TEST:
    case CCMR5F_VIMCOMP_ERROR_FORCING_TEST_FAULT_INJECT:
    case CCMR5F_PDCOMP_ERROR_FORCING_TEST:
    case CCMR5F_PDCOMP_ERROR_FORCING_TEST_FAULT_INJECT:
    case CCMR5F_INMCOMP_ERROR_FORCING_TEST:
    case CCMR5F_INMCOMP_ERROR_FORCING_TEST_FAULT_INJECT:
        retVal = TRUE;
        regBkupIntEnaSet = sl_esmREG->IESR1;
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBckupErrInfulence = sl_esmREG->EEPAPR1;
        SL_FLAG_SET(testType);
        if(_sl_fault_injection == FALSE) {
             /*flag is set to indicate the current test which is ongoing and
               These flags are used in the sl_esm.c so as to mask the esm callback*/
             sl_esmREG->IECR1 = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_SELFTEST);
             sl_esmREG->DEPAPR1 = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_SELFTEST);
         }
#if FUNCTION_PROFILING_ENABLED
    SL_Record_Errorcreationtick(testType);
#endif
         /* Set the mode to error forcing mode */
         /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
         /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
        *ccmr5fKeyReg = CCMR5F_CCMKEYR_MODE_ERRFORCING;
        /* Error is injected & mode is switched back to lockstep (takes 1 cpu
         * cycle).  
         * Wait for the mode to switch */

        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
        /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
        while (CCMR5F_CCMKEYR_MODE_LOCKSTEP != (*ccmr5fKeyReg & CCMR5F_CCMKEYR_MODE)) {
        }
        /* Since this raises a G2 ESM Error, it will be cleared by the ESM
         * handler. So check the shadow register */
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
        int g2interrupt;
        switch (testType) {
            case CCMR5F_CPUCOMP_ERROR_FORCING_TEST:
            case CCMR5F_CPUCOMP_ERROR_FORCING_TEST_FAULT_INJECT:
                g2interrupt = BF_CCMR5_CPUCOMP_ERROR;
                break;

            case CCMR5F_VIMCOMP_ERROR_FORCING_TEST:
            case CCMR5F_VIMCOMP_ERROR_FORCING_TEST_FAULT_INJECT:
                g2interrupt = BF_CCMR5_VIMCOMP_ERROR;
                break;

            case CCMR5F_PDCOMP_ERROR_FORCING_TEST:
            case CCMR5F_PDCOMP_ERROR_FORCING_TEST_FAULT_INJECT:
                g2interrupt = BF_CCMR5_PDCOMP_ERROR;
                break;

            case CCMR5F_INMCOMP_ERROR_FORCING_TEST:
            case CCMR5F_INMCOMP_ERROR_FORCING_TEST_FAULT_INJECT:
                g2interrupt = BF_CCMR5_INMCOMP_ERROR;
                break;
        }
        /* check if group 2 interrupt corresponding to error forced occured */
        if(((uint32)1u ==
                    BF_GET(sl_esmREG->SSR2,g2interrupt,BF_CCMR5_CMP_ERROR_LENGTH)))
        {
            config->stResult = ST_PASS;
        } else {
            config->stResult = ST_FAIL;
        }

        /* Clear nERROR & Interrupt only if in normal self test mode */
        if (_sl_fault_injection == FALSE) {
            /* Clear nERROR */
            _SL_HoldNClear_nError();
            /* Clear the interrupt */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
            /* ESM Status for Group2 is automatically cleared upon reading of
             * INTOFFH register on entering the ESM High interrupt handler. */
            /*sl_esmREG->SR1[1] = GET_ESM_BIT_NUM(ESM_G2ERR_CCMR5_COMP);*/
            sl_esmREG->SR1[0] = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_SELFTEST);
            sl_esmREG->SSR2 = GET_ESM_BIT_NUM(g2interrupt);
        } else {
             /* Nothing to do incase of fault injection */
        }
        /*clear the flags which indicate tests ongoing*/
        SL_FLAG_CLEAR(testType);
        sl_esmREG->IESR1 = regBkupIntEnaSet;
        sl_esmREG->EEPAPR1 = regBckupErrInfulence;
    break;

    case CCMR5F_CPUCOMP_SELF_TEST_ERROR_FORCING:
    case CCMR5F_VIMCOMP_SELF_TEST_ERROR_FORCING:
    case CCMR5F_PDCOMP_SELF_TEST_ERROR_FORCING:
    case CCMR5F_INMCOMP_SELF_TEST_ERROR_FORCING:
         retVal = TRUE;
         /*Backup grp1 esm interrupt enable register and clear the interrupt
          * enable */
         /*the esm interrupts for selftests which generate group 1 interrupts is
          * blocked. Users will have to rely on status values to get the
          * pass/failure information*/
         regBkupIntEnaSet = sl_esmREG->IESR1;
         /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
         regBckupErrInfulence = sl_esmREG->EEPAPR1;
         sl_esmREG->IECR1 = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_SELFTEST);
         sl_esmREG->DEPAPR1 = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_SELFTEST);
         /*flag is set to indicate the current test which is ongoing and
         These flags are used in the sl_esm.c so as to mask the esm callback*/
         SL_FLAG_SET(testType);
         /* Set the mode to self test error forcing mode */
         /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
#if FUNCTION_PROFILING_ENABLED
    SL_Record_Errorcreationtick(testType);
#endif
        *ccmr5fKeyReg = CCMR5F_CCMKEYR_MODE_STERRFORCING;
         /* Error is injected & mode is switched back to lockstep
         (takes 1 cpu cycle). Wait for the mode to switch */

         /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
        /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/

        while (CCMR5F_CCMKEYR_MODE_LOCKSTEP != (uint32) (*ccmr5fKeyReg & CCMR5F_CCMKEYR_MODE)) {
        }
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
        if(GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_SELFTEST) == (sl_esmREG->SR1[0] & GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_SELFTEST))){
            config->stResult = ST_PASS;
        }
        else
        {
            config->stResult = ST_FAIL;
        }
        /* Clear nERROR */
        _SL_HoldNClear_nError();
        /* Clear the interrupt */
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_21*/
        sl_esmREG->SR1[0] = (1u << ESM_G1ERR_CCMR5_SELFTEST);
        /*clear the flags which indicate tests ongoing*/
        SL_FLAG_CLEAR(testType);
        /*Restore grp1 esm interrupt enable*/
        sl_esmREG->IESR1 = regBkupIntEnaSet;
        sl_esmREG->EEPAPR1 = regBckupErrInfulence;
    break;

    default:
    break;
    }

    if (GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_OPERATING_STATUS) == (sl_esmREG->SR7[0] & GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_OPERATING_STATUS)))
    {
        sl_esmREG->SR7[0] = GET_ESM_BIT_NUM(ESM_G1ERR_CCMR5_OPERATING_STATUS);
    }
    /* If the test was fault injection and not self test, then do not mask the call back & clear the *
     *  FAULT_INJECTION flag for subsequent runs*/
    if((TRUE) == _sl_fault_injection){
            _sl_fault_injection=FALSE;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_CCMR5F, testType, (SL_SelfTest_Result)retVal , 0u);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 91 S MR: 5.2,5.6,5.7 <APPROVED> Comment_22*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_Status_CCMR4F (SL_CCMR4F_FailInfo * failInfoccmr4f)
{
    boolean retVal = FALSE;
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if (FALSE == (boolean)CHECK_RANGE_RAM_PTR(failInfoccmr4f)) {
        SL_Log_Error(FUNC_ID_ST_CCMR4F_STATUS, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return retVal;
    }
#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_CCMR4F_STATUS, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return(FALSE);
    }
#endif
    /* Proceed only if in self-test mode else entry condition is wrong */
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if (CCMR4F_CCMKEYR_MODE_SELFTEST != (uint32) (ccmr4fREG1->_CCMKEYR & CCMR4F_CCMKEYR_MODE)) {
            retVal =  FALSE;
    }
    else
    {
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if (CCMR4F_CCMSR_STC == (uint32) (ccmr4fREG1->_CCMSR & CCMR4F_CCMSR_STC)) {
        /* Restore the CPU mode */
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
        ccmr4fREG1->_CCMKEYR = CCMR4F_CCMKEYR_MODE_LOCKSTEP;
        /* \todo document that the Status API must be called after starting the
        self-test to restore the cpu mode */
        retVal = TRUE; /* Indicate that the self test was complete */
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (CCMR4F_CCMSR_STE == (uint32) (ccmr4fREG1->_CCMSR & CCMR4F_CCMSR_STE)) {
            /* self-test failed */
            failInfoccmr4f->stResult = ST_FAIL;
            /* retrieve the err info */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            if (CCMR4F_CCMSR_STET == (uint32) (ccmr4fREG1->_CCMSR & CCMR4F_CCMSR_STET)) {
                /* \todo Looks like there is a type in TRM w.r.t bit description. */
                failInfoccmr4f->failInfo = CCMR4F_ST_ERR_COMPARE_MATCH;
            } else {
                failInfoccmr4f->failInfo = CCMR4F_ST_ERR_COMPARE_MISMATCH;
            }
        } else {
            failInfoccmr4f->stResult = ST_PASS;
        }
        /*clear the flags which indicate tests ongoing*/
        SL_FLAG_CLEAR(CCMR4F_SELF_TEST);
    } else {
        retVal = FALSE; /* Indicate that the self test was not complete */
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_CCMR4F_STATUS, (SL_SelfTestType)0u, failInfoccmr4f->stResult, 0u);
#endif
    }
    return(retVal);
}
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 91 S MR: 5.2,5.6,5.7 <APPROVED> Comment_22*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_Status_CCMR5F (SL_SelfTestType testType, SL_CCMR5F_FailInfo * failInfoccmr5f)
{
    boolean retVal = FALSE;
    volatile uint32* ccmr5fKeyReg = (uint32*)0;
    volatile uint32* ccmr5fStReg = (uint32*)0;
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if (FALSE == (boolean)CHECK_RANGE_RAM_PTR(failInfoccmr5f)) {
        SL_Log_Error(FUNC_ID_ST_CCMR5F_STATUS, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return retVal;
    }

    if ((CCMR5F_CPUCOMP_SELF_TEST != testType) && 
            (CCMR5F_VIMCOMP_SELF_TEST != testType) &&
            (CCMR5F_PDCOMP_SELF_TEST != testType) &&
            (CCMR5F_INMCOMP_SELF_TEST != testType)) {
        SL_Log_Error(FUNC_ID_ST_CCMR5F, ERR_TYPE_PARAM, 1u);
        retVal = FALSE;
        return retVal;
    }
#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_CCMR5F_STATUS, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return(FALSE);
    }
#endif

    if (testType == CCMR5F_CPUCOMP_SELF_TEST) {
        ccmr5fKeyReg = &(ccmr5fREG1->_CCMKEYR1);
        ccmr5fStReg =  &(ccmr5fREG1->_CCMSR1);
    } else if (testType == CCMR5F_VIMCOMP_SELF_TEST) {
        ccmr5fKeyReg = &(ccmr5fREG1->_CCMKEYR2);
        ccmr5fStReg =  &(ccmr5fREG1->_CCMSR2);
    } else if (testType == CCMR5F_PDCOMP_SELF_TEST)  {
        ccmr5fKeyReg = &(ccmr5fREG1->_CCMKEYR4);
        ccmr5fStReg =  &(ccmr5fREG1->_CCMSR4);
    }
    else if (testType == CCMR5F_INMCOMP_SELF_TEST) {
        ccmr5fKeyReg = &(ccmr5fREG1->_CCMKEYR3);
        ccmr5fStReg =  &(ccmr5fREG1->_CCMSR3);
    }

    /* Proceed only if in self-test mode else entry condition is wrong */
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    if (CCMR5F_CCMKEYR_MODE_SELFTEST != (uint32) (*ccmr5fKeyReg & CCMR5F_CCMKEYR_MODE)) {
               retVal =  FALSE;
    } else {
        /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    	/*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        if (CCMR5F_CCMSR_STC == (uint32) (*ccmr5fStReg & CCMR5F_CCMSR_STC)) {
            /* Restore the CPU mode */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            *ccmr5fKeyReg = CCMR5F_CCMKEYR_MODE_LOCKSTEP;
            /* \todo document that the Status API must be called after starting the
            self-test to restore the cpu mode */
            retVal = TRUE; /* Indicate that the self test was complete */
            /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
            /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
            if (CCMR5F_CCMSR_STE == (uint32) (*ccmr5fStReg & CCMR5F_CCMSR_STE)) {
                /* self-test failed */
                failInfoccmr5f->stResult = ST_FAIL;
                /* retrieve the err info */
                /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
                /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
                if (CCMR5F_CCMSR_STET == (uint32) (*ccmr5fStReg & CCMR5F_CCMSR_STET)) {
                    /* \todo Looks like there is a type in TRM w.r.t bit description. */
                    failInfoccmr5f->failInfo = CCMR5F_ST_ERR_COMPARE_MATCH;
                } else {
                    failInfoccmr5f->failInfo = CCMR5F_ST_ERR_COMPARE_MISMATCH;
                }
            } else {
                failInfoccmr5f->stResult = ST_PASS;
            }
            /*clear the flags which indicate tests ongoing*/
            SL_FLAG_CLEAR(testType);
        } else {
            retVal = FALSE; /* Indicate that the self test was not complete */
        }
    }

#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_CCMR5F_STATUS, (SL_SelfTestType)0u, failInfoccmr5f->stResult, 0u);
#endif
    return(retVal);
}
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
/** @fn void adcCalibration(sl_adcBASE_t *adc)
*   @brief Computes offset error using Calibration mode
*   @param[in] adc Pointer to ADC module:
*              - sl_adcREG1: ADC1 module pointer
*              - sl_adcREG2: ADC2 module pointer
*   This function computes offset error using Calibration mode
*
*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_adcCalibration(sl_adcBASE_t * adc, uint32 * offset_error)
{
	uint32 calr_val[5]={0U,0U,0U,0U,0U};
    uint32 loop_index=0U;
    uint32 backup_mode;

    boolean retVal =  FALSE;

#ifdef FUNCTION_PARAM_CHECK_ENABLED

    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    if((adc != sl_adcREG1) && (adc != sl_adcREG2) && (adc != NULL)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_PARAM, 2u);
        retVal = FALSE;
        return retVal;
    }

#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_ENTRY_CON, 0u);
        retVal = FALSE;
        return retVal;
    }
    /*todo check if these checks are redundant*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if ((uint32)1u == adc->RSTCR) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_ENTRY_CON, 1u);
        retVal = FALSE;
        return retVal;
    }
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if(0u != (adc->OPMODECR & ADC_POWERDOWN_MODE)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_ENTRY_CON, 2u);
        retVal = FALSE;
        return retVal;
    }
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if(0u == (adc->OPMODECR & ADC_ENABLE)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_ENTRY_CON, 3u);
        retVal = FALSE;
        return retVal;
    }
#endif

    /** - Backup Mode before Calibration  */
    backup_mode = adc->OPMODECR;

    /** - Enable 12-BIT ADC  */
    sl_adcREG1->OPMODECR |= 0x80000000U;

    /* Disable all channels for conversion */
    adc->GxSEL[0]=0x00U;
    adc->GxSEL[1]=0x00U;
    adc->GxSEL[2]=0x00U;

    for(loop_index=0U;loop_index<4U;loop_index++)
    {
        /* Disable Self Test and Calibration mode */
        adc->CALCR=0x0U;

        switch(loop_index)
        {
            case 0U :     /* Test 1 : Bride En = 0 , HiLo =0 */
                        adc->CALCR=0x0U;
                        break;

            case 1U :    /* Test 1 : Bride En = 0 , HiLo =1 */
                        adc->CALCR=0x0100U;
                        break;

            case 2U :     /* Test 1 : Bride En = 1 , HiLo =0 */
                        adc->CALCR=0x0200U;
                        break;

            case 3U :     /* Test 1 : Bride En = 1 , HiLo =1 */
                        adc->CALCR=0x0300U;
                        break;
            default :
                        break;
        }

        /* Enable Calibration mode */
        adc->CALCR|=0x1U;

        /* Start calibration conversion */
        adc->CALCR|=0x00010000U;

        /* Wait for calibration conversion to complete */
        /*SAFETYMCUSW 28 D <APPROVED> Comment_13*/
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        while((adc->CALCR & 0x00010000U)==0x00010000U)
        {
        } /* Wait */

        /* Read converted value */
        calr_val[loop_index]= adc->CALR;
    }

    /* Disable Self Test and Calibration mode */
    adc->CALCR=0x0U;

    /* Compute the Offset error correction value */
    calr_val[4]=calr_val[0]+ calr_val[1] + calr_val[2] + calr_val[3];

    calr_val[4]=(calr_val[4]/4);

    *offset_error=calr_val[4]-0x7FFU;

    /*Write the offset error to the Calibration register */
    /* Load 2's complement of the computed value to ADCALR register */
    *offset_error=~(*offset_error);
    *offset_error=(*offset_error) & 0xFFFU;
    *offset_error=(*offset_error) + 1U;

    adc->CALR = *offset_error;

  /** - Restore Mode after Calibration  */
    adc->OPMODECR = backup_mode;

    retVal = TRUE;
    return retVal;

 /**   @note The function adcInit has to be called before using this function. */

}

/*SAFETYMCUSW 62 D MR: 16.7 <APPROVED> Comment_23*/
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTest_ADC(SL_SelfTestType testType, boolean mode, SL_ADC_Config * config, SL_ADC_Pinstatus * pinstatus)
{
    uint16 adRefHi = 0u;
    uint16 adRefLo = 0u;
    uint16 approximated_value;
    uint32 temp, offset;
    SL_ADC_Data Vd, Vu, Vn;

    boolean retVal =  FALSE;

    uint32 ModeCtrlRegWorkingCopy, IntEnaRegWorkingCopy, IntThrRegWorkingCopy;

#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if (FALSE == (boolean)CHECK_RANGE_RAM_PTR(pinstatus)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return retVal;
    }
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> Comment_4*/
    if (FALSE == (boolean)CHECK_RANGE_RAM_PTR(config)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_PARAM, 1u);
        retVal =  FALSE;
        return retVal;
    }
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    if((config->adcbase != sl_adcREG1) && (config->adcbase != sl_adcREG2) && (config->adcbase != NULL)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_PARAM, 2u);
        retVal =  FALSE;
        return retVal;
    }
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
/*ADC Channels 0 to 23 are available in TMS570LS31x, TMS570LS12x, RM48 and RM46x devices*/
    if((config->adc_channel >= 24u)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_PARAM, 3u);
        retVal =  FALSE;
        return retVal;
    }
#endif
#if defined(_RM42x_) || defined(_TMS570LS04x_)
/*These ADC channels are not available in TMS570LS04x and RM42x devices*/
    if(((config->adc_channel > 11u) && (config->adc_channel < 16u)) ||
           ((config->adc_channel > 17u) && (config->adc_channel < 20u)) ||
           (config->adc_channel > 21u)){
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_PARAM, 3u);
        retVal = FALSE;
        return retVal;
    }
#endif

#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return retVal;
    }
    /*todo check if these checks are redundant*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if ((uint32)1u == config->adcbase->RSTCR) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_ENTRY_CON, 1u);
        retVal =  FALSE;
        return retVal;
    }
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if(0u != (config->adcbase->OPMODECR & ADC_POWERDOWN_MODE)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_ENTRY_CON, 2u);
        retVal =  FALSE;
        return retVal;
    }
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    if(0u == (config->adcbase->OPMODECR & ADC_ENABLE)) {
        SL_Log_Error(FUNC_ID_ST_ADC, ERR_TYPE_ENTRY_CON, 3u);
        retVal =  FALSE;
        return retVal;
    }
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif
    sl_adcBASE_t * adc = config->adcbase;
    uint8 channel = config->adc_channel;
    /*initializing the Vd,Vu,Vn values to 0 on start*/
    Vd.value = 0u;
    Vu.value = 0u;
    Vn.value = 0u;

    /*setting pinstatus to default value*/
    *pinstatus = ADC_PIN_UNDETERMINED;
    /*backup copy of the interrupt and mode control registers of adc*/
    IntEnaRegWorkingCopy = adc->GxINTENA[sl_adcGROUP1];
    IntThrRegWorkingCopy = adc->GxINTCR[sl_adcGROUP1];
    ModeCtrlRegWorkingCopy = adc->OPMODECR;

    /*Disable Interrupts*/
    adc->GxINTENA[sl_adcGROUP1] = 0u;

    /*disable any self tst and Calib mode first */
    adc->CALCR = 0u;

    /*first find out AdRefHi and AdRefLo by using Calibration method*/
    /** - Backup Mode before Calibration  */

    /** - Enable 12-BIT ADC  */
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    (adc->OPMODECR) |= ADC_12BIT_RESOLUTION;
    /* Disable all channels for conversion */
    /*Event select*/
    adc->GxSEL[sl_adcGROUP1] = 0u;

    /* Test 1 : Bride En = 1 , HiLo =0 Calib Enabled, Start Calibration */
    /*SAFETYMCUSW 28 D MR: 16.7 <APPROVED> Comment_24*/
    adc->CALCR = (ADC_CALIB_BRIDGE_ENABLE | ADC_START_CALIBRATION | ADC_ENABLE_CALIBRATION);

    /* Wait for calibration conversion to complete */
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    while ((adc->CALCR & ADC_START_CALIBRATION) == ADC_START_CALIBRATION) {

    }
    /* Read converted value */
    /* Since adc->CALR is volatile, it is recommended not to cast directly to uint16 */
    temp = adc->CALR;
    adRefLo = (uint16)temp;

    /*  Bride En = 1 , HiLo =1 Calib Enabled, Start Calibration */
    adc->CALCR = (ADC_CALIB_BRIDGE_ENABLE | ADC_CALIB_HILO_ENABLE | ADC_START_CALIBRATION | ADC_ENABLE_CALIBRATION);
    /* Wait for calibration conversion to complete */
    /*SAFETYMCUSW 28 D MR: 16.7 <APPROVED> Comment_24*/
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    while ((adc->CALCR & ADC_START_CALIBRATION) == ADC_START_CALIBRATION) {
    }
    /* Read converted value */
    /* Since adc->CALR is volatile, it is recommended not to cast directly to uint16 */
    temp = adc->CALR;
    adRefHi = (uint16)temp;

    /** - Restore Mode after Calibration  */
    adc->OPMODECR = ModeCtrlRegWorkingCopy;
    /* disable any self tst and Calib mode first*/
    adc->CALCR = 0u;

    /*Calibrate the ADC so as to fill appropriate value in CALR register*/
    /*SAFETYMCUSW 440 S MR: 11.3 <APPROVED> Comment_18*/
    if(FALSE == SL_adcCalibration(adc, &offset))
    {
    	return retVal;
    }

    /*Calculate Vn*/
    /*enable ADC channel which needs to be tested */
    _SL_SelfTest_adcStartConversion_selChn(adc, channel, ADC_FIFO_SIZE, (uint32) sl_adcGROUP1);
    /* check for the status of the group to see if it is finished*/
    /*SAFETYMCUSW 28 D MR: 16.7 <APPROVED> Comment_24*/
    while (FALSE == _SL_SelfTest_adcIsConversionComplete(adc, (uint32) sl_adcGROUP1)) {

    }
    /* copy the results into the Vn*/
    _SL_SelfTest_adcGetSingleData(adc, (uint32) sl_adcGROUP1, &Vn);

    /* for this self test put the adc group in single shot non streaming mode & later restore the setting to original */
    /* set the ADC hw in the self test mode and start the test.*/
    /*Calculate Vd*/
    adc->CALCR |= ADC_ENABLE_SELFTEST;
    /* set HiLo to 0 to connect AdVref = low */
    /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
    adc->CALCR = (adc->CALCR)&(~ADC_CALIB_HILO_ENABLE);
    /*enable ADC channel which needs to be tested */
    _SL_SelfTest_adcStartConversion_selChn(adc, channel, ADC_FIFO_SIZE, (uint32) sl_adcGROUP1);
    /* check for the status of the group to see if it is finished*/
    /*SAFETYMCUSW 28 D MR: 16.7 <APPROVED> Comment_24*/
    while (FALSE == _SL_SelfTest_adcIsConversionComplete(adc, (uint32) sl_adcGROUP1)) {

    }
    /* copy the results into the Vd*/
    _SL_SelfTest_adcGetSingleData(adc, (uint32) sl_adcGROUP1, &Vd);

    /*Calculate Vu*/
    adc->CALCR |= ADC_CALIB_HILO_ENABLE;
    /*enable ADC channel which needs to be tested */
    _SL_SelfTest_adcStartConversion_selChn(adc, channel, ADC_FIFO_SIZE, (uint32) sl_adcGROUP1);
    /* check for the status of the group to see if it is finished*/
    /*SAFETYMCUSW 28 D MR: 16.7 <APPROVED> Comment_24*/
    while (FALSE == _SL_SelfTest_adcIsConversionComplete(adc, (uint32) sl_adcGROUP1)) {

    }
    /* copy the results into the Vn*/
    _SL_SelfTest_adcGetSingleData(adc, (uint32) sl_adcGROUP1, &Vu);

    /*Approximation of the values is done here*/
    /* find the differences and set the voltages levels to ref_low or rel_hig for proper comparision */
    approximated_value = _SL_Approximate_value(Vn.value,adRefHi,(uint8)ADC_APPROXIMATION_THRESHOLD);
    if(approximated_value == Vn.value)
    {
        Vn.value = _SL_Approximate_value(Vn.value,adRefLo,(uint8)ADC_APPROXIMATION_THRESHOLD);
    }
    else
    {
        Vn.value = approximated_value;
    }
    approximated_value = _SL_Approximate_value(Vu.value,adRefHi,(uint8)ADC_APPROXIMATION_THRESHOLD);
    if(approximated_value == Vu.value)
    {
        Vu.value = _SL_Approximate_value(Vu.value,adRefLo,(uint8)ADC_APPROXIMATION_THRESHOLD);
        Vu.value = Vu.value; /* Avoid warning that function return value not used */
    }
    else
    {
        Vu.value = approximated_value;
    }
    approximated_value = _SL_Approximate_value(Vd.value,adRefHi,(uint8)ADC_APPROXIMATION_THRESHOLD);
    if(approximated_value == Vd.value)
    {
        Vd.value = _SL_Approximate_value(Vd.value,adRefLo,(uint8)ADC_APPROXIMATION_THRESHOLD);
        Vd.value = Vd.value; /* Avoid warning that function return value not used */
    }
    else
    {
        Vd.value = approximated_value;
    }

    if ((Vn.value == adRefHi) && (Vu.value == adRefHi) && (Vd.value == adRefHi)) {
        *pinstatus = ADC_PIN_SHORTED_TO_ADVREF_HIGH;
    } else if ((Vn.value == adRefLo) && (Vu.value == adRefLo) && (Vd.value == adRefLo)) {
        *pinstatus = ADC_PIN_SHORTED_TO_ADVREF_LOW;
    } else if (((Vu.value < adRefHi) && (Vu.value > Vn.value)) && ((Vd.value > adRefLo) && (Vd.value < Vn.value))) {
        *pinstatus = ADC_PIN_GOOD;
    } else if ((Vu.value == adRefHi) && (Vd.value == adRefLo)) {
        *pinstatus = ADC_PIN_OPEN;
    } else {
        *pinstatus = ADC_PIN_UNDETERMINED;
    }

    /*disable any self tst and Calib mode first*/
    adc->CALCR = 0u;
    /* Disable all channels for conversion */
    /* Disable all channels for conversion */
    /*Event select*/
    adc->GxSEL[(uint32) sl_adcGROUP1] = 0u;
    /*Enable Interrupts if it was enabled before the selftest*/
    adc->GxINTENA[(uint32) sl_adcGROUP1] = IntEnaRegWorkingCopy;
    adc->GxINTCR[(uint32) sl_adcGROUP1] = IntThrRegWorkingCopy;

#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_ADC, testType, adc_stResult , 0u);
#endif

    retVal =  TRUE;
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return retVal;
}


/*reserved location in memory map*/
#define SCR_RESERVED_LOCATION 0x70000000U
/*reserved location in memory map*/
#define PCR_RESERVED_LOCATION 0xFD000000U
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_15*/
/*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
boolean SL_SelfTestL2L3Interconnect(SL_SelfTestType testType)
{
    boolean retVal = FALSE;
    uint32 read_reserved_word;
#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in rage */
    if ((_SELFTEST_L2L3INTERCONNECT_MIN > testType) || (_SELFTEST_L2L3INTERCONNECT_MAX < testType)) {
        SL_Log_Error(FUNC_ID_ST_L2L3INTERCONNECT, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return(retVal);
    }
#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_L2L3INTERCONNECT, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return(retVal);
    }
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif
    switch(testType)
    {
    /*Access a reserve location in the EMIF register space,as EMIF lies on SCR*/
        case L2INTERCONNECT_FAULT_INJECT:
            /*try to read reserved word in the emif register map*/
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
            _SL_Barrier_Data_Access();
            read_reserved_word = *((uint32*)SCR_RESERVED_LOCATION);
            read_reserved_word = read_reserved_word; /* Avoid compiler warning. */
            retVal = TRUE;
            break;
    /*Access a reserve location in the DCAN register space,as DCAN lies on PCR*/
        case L3INTERCONNECT_FAULT_INJECT:
            /*try to read reserved word in the pbist register map*/
#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif
            _SL_Barrier_Data_Access();
            read_reserved_word = *((uint32*)PCR_RESERVED_LOCATION);
            read_reserved_word = read_reserved_word; /* Avoid compiler warning. */
            retVal = TRUE;
            break;

        default:
            retVal = FALSE;
            break;
    }
#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_L2L3INTERCONNECT, testType, (SL_SelfTest_Result)retVal , 0u);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
    /*SAFETYMCUSW 7 C MR: 14.7 <APPROVED> Comment_3*/
}
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define dmaRAMBaseAddr    0xFFF80000u
#define dmaBadECC        0xFFF80010u
boolean SL_SelfTest_DMA(SL_SelfTestType testType)
{
    volatile boolean _sl_fault_injection = FALSE;
    volatile uint32 ramRead;
    register uint64 regBackupSECCTL, regBackupPCR, ramread64, regBckupErrInfulence, regBkupIntEnaSet;
    uint32 ram1erraddr, dataVal;
    uint32 *dmaRead = (uint32 *)dmaBadECC;

#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif

    boolean retVal = FALSE;

    regBackupPCR = sl_dmaREG->DMAPCR;
    regBackupSECCTL = sl_dmaREG->DMASECCCTRL;

#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in rage */
    if ((_SELFTEST_DMA_MIN > testType) || (_SELFTEST_DMA_MAX < testType)) {
        SL_Log_Error(FUNC_ID_ST_DMA, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return(retVal);
    }
#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for priveleged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_DMA, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return(retVal);
    }
#endif

    /* If DMA ECC is not enabled, return error. */
    if((DMA_SRAM_ECC_DISABLED) == BF_GET(sl_dmaREG->DMAPCR, DMA_PARCTRL_ECC_DETECT_EN_START, DMA_PARCTRL_ECC_DETECT_EN_LENGTH))
    {
        SL_Log_Error(FUNC_ID_ST_DMA, ERR_TYPE_ENTRY_CON, 1U);
        return(FALSE);
    }

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active())
    {
        SL_Log_Error(FUNC_ID_ST_DMA, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }

    /* If fault inject set global variable to flag to the ESM handler that it is a fault injection */
    if((DMA_ECC_TEST_MODE_1BIT_FAULT_INJECT == testType)||
        (DMA_ECC_TEST_MODE_2BIT_FAULT_INJECT == testType)){
        _sl_fault_injection=TRUE;
    }

    /* backup DATA stored at this location */
    dataVal = *dmaRead;

    /* Enable ECC */
    BF_SET(sl_dmaREG->DMAPCR, DMA_PARCTRL_ECC_EN, DMA_PARCTRL_ECC_EN_START, DMA_PARCTRL_ECC_EN_LENGTH);

    /* rewrite the data back to the DMA RAM, this will ensure that the ECC is
     * correctly computed for this location. */
    *dmaRead = dataVal;

    /* Enable ECC test mode */
    BIT_SET(sl_dmaREG->DMAPCR, DMA_ECC_TST_EN);

#if FUNCTION_PROFILING_ENABLED
    SL_Record_Errorcreationtick(testType);
#endif

#if FUNCTION_PROFILING_ENABLED
    SL_Record_Errorcreationtick(testType);
#endif

    switch(testType)
    {

    case DMA_ECC_TEST_MODE_1BIT:
    case DMA_ECC_TEST_MODE_1BIT_FAULT_INJECT:

        SL_FLAG_SET(testType);
        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        regBkupIntEnaSet = sl_esmREG->IESR7;
        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBckupErrInfulence = sl_esmREG->IEPSR7;

        if((FALSE) == _sl_fault_injection){
            /* Disable ESM notification */
            sl_esmREG->IECR7 = GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_SBERR);
            sl_esmREG->IEPCR7 = GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_SBERR);
        }

        /* Enable generation of single bit event to the ESM (will set the ESM
         * Status flag upon detection of single bit error.) If ESM is enabled
         * (fault injection), will cause ESM interrupt to occur.  */
        BF_SET(sl_dmaREG->DMASECCCTRL, DMA_SBE_EVT_EN, DMA_SBE_EVT_START, DMA_SBE_EVT_LENGTH);
        /* Write clear any pending single bit error flag */
        BIT_SET(sl_dmaREG->DMASECCCTRL, DMA_ECC_SBERR);
        /* Read clear the error address offset */
        ramRead = sl_dmaREG->DMAECCSBE;

        /* Check syndrome */
        /* Flip bit 0 to introduce a single bit error */
        *dmaRead ^= DMA_FLIP_DATA_1BIT;
        /*BIT_FLIP(*dmaRead, DMA_FLIP_DATA_1BIT);*/

        /* Cause 1 bit ECC error */
        ramRead = *dmaRead;
        ram1erraddr = (uint32)(sl_dmaREG->DMAECCSBE | dmaRAMBaseAddr);

        if(DMA_ECC_TEST_MODE_1BIT == testType) {
            if((BIT(DMA_ECC_SBERR) == (sl_dmaREG->DMASECCCTRL & BIT(DMA_ECC_SBERR))) &&
                    (dmaBadECC == ram1erraddr) &&
                    (((sl_esmREG->SR7[0]) & GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_SBERR)) 
                     == GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_SBERR))) {
                /* Clear ESM error status */
                sl_esmREG->SR7[0] =  GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_SBERR);
                retVal = TRUE;
            } else {
                retVal = FALSE;
            }
        }  else {
            /*nothing to be done for fault injection */
            retVal = TRUE;
        }

        /* Clear flags and read error address */
        BIT_SET(sl_dmaREG->DMASECCCTRL, BIT(DMA_ECC_SBERR));

        /* Disable ECC */
        BF_SET(sl_dmaREG->DMAPCR, DMA_PARCTRL_ECC_DIS, DMA_PARCTRL_ECC_EN_START, DMA_PARCTRL_ECC_EN_LENGTH);

        /* Restore data. ECC should remain the same as before. */
        *dmaRead = dataVal;

        SL_FLAG_CLEAR(testType);

        /* Clear the TEST bit of Parity control register */
        BIT_SET(sl_dmaREG->DMAPCR, DMA_ECC_TST_EN);

        /* Restore DMA register states */
        sl_dmaREG->DMAPCR = regBackupPCR;
        sl_dmaREG->DMASECCCTRL = regBackupSECCTL;

        /* Restore ESM registers states */
        sl_esmREG->IESR7 = regBkupIntEnaSet;
        sl_esmREG->IEPSR7 = regBckupErrInfulence;

        break;

    case DMA_ECC_TEST_MODE_2BIT:
    case DMA_ECC_TEST_MODE_2BIT_FAULT_INJECT:
        regBkupIntEnaSet = sl_esmREG->IESR1;
        regBckupErrInfulence = sl_esmREG->EEPAPR1;

        if((FALSE) == _sl_fault_injection){
            /* Disable ESM notification */
            sl_esmREG->IECR1 = GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_UNCORR);
            sl_esmREG->DEPAPR1 = GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_UNCORR);
        }

        /* Clear previous ECC error */
        BIT_SET(sl_dmaREG->DMAPAR, DMA_ECC_ERR);

        /* Read clear the error address */
         ramRead= sl_dmaREG->DMAPAR;
         SL_FLAG_SET(testType);

        /* Introduce a double bit error */
        *dmaRead ^= DMA_FLIP_DATA_2BIT;
        /*BIT_FLIP(*dmaRead, DMA_FLIP_DATA_2BIT);*/

        ramRead = *dmaRead;
        ram1erraddr = (BF_GET(sl_dmaREG->DMAPAR, DMA_ECC_UNCORERR_ADDR_START, DMA_ECC_UNCORERR_ADDR_LEN)| dmaRAMBaseAddr);

        if(DMA_ECC_TEST_MODE_2BIT == testType) {
            if(((sl_dmaREG->DMAPAR & BIT(DMA_ECC_ERR)) == BIT(DMA_ECC_ERR)) &&
                (dmaBadECC == ram1erraddr)    &&
                (GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_UNCORR) == 
                 (sl_esmREG->SR1[0] & GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_UNCORR)))) {
                retVal = TRUE;
                /* Clear ESM error status */
                sl_esmREG->SR1[0] =  GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_UNCORR);
            } else {
                retVal = FALSE;
            }
        } else {
            retVal = TRUE;
        }

        /* Clear flags and read error address */
        BIT_SET(sl_dmaREG->DMAPAR, BIT(DMA_ECC_ERR));

        /* Disable ECC */
        BF_SET(sl_dmaREG->DMAPCR, DMA_PARCTRL_ECC_DIS, DMA_PARCTRL_ECC_EN_START, DMA_PARCTRL_ECC_EN_LENGTH);

        /* Restore data */
        *dmaRead = dataVal;

        SL_FLAG_CLEAR(testType);

        /* Disable ECC test mode */
        BIT_SET(sl_dmaREG->DMAPCR, DMA_ECC_TST_EN);

        /* Restore DMA register states */
        sl_dmaREG->DMAPCR = regBackupPCR;
        sl_dmaREG->DMASECCCTRL = regBackupSECCTL;
        /* Restore ESM registers states */
        sl_esmREG->IESR1 = regBkupIntEnaSet;
        sl_esmREG->EEPAPR1 = regBckupErrInfulence;
        break;

    default:
        retVal = FALSE;
        break;
    }

    /* If the test was fault injection and not self test, then do not mask the call back & clear the *
    *  FAULT_INJECTION flag for subsequent runs*/
    if((TRUE) == _sl_fault_injection){
            _sl_fault_injection=FALSE;
    }

    ramread64 = ramread64; /* Avoid compiler warning. */

#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_DMA, testType, (SL_SelfTest_Result)retVal , 0u);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}

#define msgNo            2u
#define eccOffset        0x00001000u
#define dcan1RAMBase    (volatile uint32*)0xFF1E0000
#define dcan2RAMBase    (volatile uint32*)0xFF1C0000
#define dcan3RAMBase    (volatile uint32*)0xFF1A0000
#define dcan4RAMBase    (volatile uint32*)0xFF180000

/* ESM 2 bit registers  specific to each DCAN instance. */
#define dcan1uerrESMReg (volatile uint32*)&(sl_esmREG->SR1[0])
#define dcan1iesrESMReg (volatile uint32*)&(sl_esmREG->IESR1)
#define dcan1iecrESMReg (volatile uint32*)&(sl_esmREG->IECR1)
#define dcan1eepaprESMReg (volatile uint32*)&(sl_esmREG->EEPAPR1)
#define dcan1depaprESMReg (volatile uint32*)&(sl_esmREG->DEPAPR1)

#define dcan2uerrESMReg dcan1uerrESMReg
#define dcan2iesrESMReg dcan1iesrESMReg
#define dcan2iecrESMReg dcan1iecrESMReg
#define dcan2eepaprESMReg dcan1eepaprESMReg
#define dcan2depaprESMReg dcan1depaprESMReg

#define dcan3uerrESMReg dcan1uerrESMReg
#define dcan3iesrESMReg dcan1iesrESMReg
#define dcan3iecrESMReg dcan1iecrESMReg
#define dcan3eepaprESMReg dcan1eepaprESMReg
#define dcan3depaprESMReg dcan1depaprESMReg

#define dcan4uerrESMReg (volatile uint32*)&(sl_esmREG->SR4[0])
#define dcan4iesrESMReg (volatile uint32*)&(sl_esmREG->IESR4)
#define dcan4iecrESMReg (volatile uint32*)&(sl_esmREG->IECR4)
#define dcan4eepaprESMReg (volatile uint32*)&(sl_esmREG->IEPSR4)
#define dcan4depaprESMReg (volatile uint32*)&(sl_esmREG->IEPCR4)

boolean SL_SelfTest_CAN(SL_SelfTestType testType, SL_DCAN_Instance instance)
{
    volatile boolean _sl_fault_injection = FALSE;
    volatile uint32 ramRead;
    register uint32 ramread32, regBackupEccCS, regBackupCtl, regBckupErrInfulence, regBkupIntEnaSet;
    volatile uint32* data;
    boolean testPassed = FALSE;
    uint32           dataVal;

    sl_canBASE_t* sl_canREG;
    volatile uint32 *dcanRAMBase;
    uint32 esm1bitecc, esm2bitecc;
    volatile uint32* esm2bitStatReg;
    volatile uint32* esm2bitEEPAPR;
    volatile uint32* esm2bitDEPAPR; 
    volatile uint32* esm2bitIESR;   
    volatile uint32* esm2bitIECR;   

#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif

    boolean retVal = FALSE;

#ifdef FUNCTION_PARAM_CHECK_ENABLED
    /* Verify that the parameters are in rage */
    if ((_SELFTEST_CAN_MIN > testType) || (_SELFTEST_CAN_MAX < testType)) {
        SL_Log_Error(FUNC_ID_ST_CAN, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return(retVal);
    }

    if ((instance < DCAN_MIN) || (instance > DCAN_MAX)) {
        SL_Log_Error(FUNC_ID_ST_CAN, ERR_TYPE_PARAM, 0u);
        retVal =  FALSE;
        return(retVal);
    }

#endif

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for privileged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_CAN, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return(retVal);
    }
#endif
    switch(instance)
    {
        case SL_DCAN1:
            sl_canREG      = sl_canREG1;
            dcanRAMBase    = dcan1RAMBase;
            esm1bitecc     = ESM_G1ERR_CAN1_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_CAN1_ECC_UNCORR;
            esm2bitStatReg = dcan1uerrESMReg;
            esm2bitEEPAPR  = dcan1eepaprESMReg;
            esm2bitDEPAPR  = dcan1depaprESMReg;
            esm2bitIESR    = dcan1iesrESMReg;
            esm2bitIECR    = dcan1iecrESMReg;
            break;
        case SL_DCAN2:
            sl_canREG      = sl_canREG2;
            dcanRAMBase    = dcan2RAMBase;
            esm1bitecc     = ESM_G1ERR_CAN2_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_CAN2_ECC_UNCORR;
            esm2bitStatReg = dcan2uerrESMReg;
            esm2bitEEPAPR  = dcan2eepaprESMReg;
            esm2bitDEPAPR  = dcan2depaprESMReg;
            esm2bitIESR    = dcan2iesrESMReg;
            esm2bitIECR    = dcan2iecrESMReg;
            break;
        case SL_DCAN3:
            sl_canREG      = sl_canREG3;
            dcanRAMBase    = dcan3RAMBase;
            esm1bitecc     = ESM_G1ERR_CAN3_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_CAN3_ECC_UNCORR;
            esm2bitStatReg = dcan3uerrESMReg;
            esm2bitEEPAPR  = dcan3eepaprESMReg;
            esm2bitDEPAPR  = dcan3depaprESMReg;
            esm2bitIESR    = dcan3iesrESMReg;
            esm2bitIECR    = dcan3iecrESMReg;
            break;
        case SL_DCAN4:
            sl_canREG      = sl_canREG4;
            dcanRAMBase    = dcan4RAMBase;
            esm1bitecc     = ESM_G1ERR_CAN4_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_CAN4_ECC_UNCORR;
            esm2bitStatReg = dcan4uerrESMReg;
            esm2bitEEPAPR  = dcan4eepaprESMReg;
            esm2bitDEPAPR  = dcan4depaprESMReg;
            esm2bitIESR    = dcan4iesrESMReg;
            esm2bitIECR    = dcan4iecrESMReg;
            break;
    }

    if((CAN_SRAM_ECC_DISABLED) == 
            BF_GET(sl_canREG->CTL, CAN_CTRL_SECDED_START, CAN_CTRL_SECDED_LENGTH))
    {
        SL_Log_Error(FUNC_ID_ST_CAN, ERR_TYPE_ENTRY_CON, 1U);
        return(FALSE);
    }

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active())
    {
        SL_Log_Error(FUNC_ID_ST_CAN, ERR_TYPE_ENTRY_CON, 2U);
        return(FALSE);
    }

    /* If fault inject set global variable to flag to the ESM handler that it is a fault injection */
    if((CAN_ECC_TEST_MODE_1BIT_FAULT_INJECT == testType)||
        (CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT == testType)){
        _sl_fault_injection = TRUE;
    }

    /* DCAN1/2/3 uncorrectable ECC is in SR1  */


    regBackupEccCS = sl_canREG->ECC_CS;
    regBackupCtl = sl_canREG->CTL;

    /* initialize data */
    data = (volatile uint32 *)((((uint32)dcanRAMBase) + (msgNo*0x20u)));
    /* Offset the pointer by 4 bytes as the memory map for the MESSAGE RAM
     * is different based on RDA or debug, and offsetting this by 4 points
     * to an unreserved location regardless of RDA or debug mode access of
     * message RAM */
    data++;

    /* disable SECDED - write to PMD in CANCTL */
    BF_SET(sl_canREG->CTL, CAN_CTRL_SECDED_DIS, CAN_CTRL_SECDED_START, CAN_CTRL_SECDED_LENGTH);

    /* set Test bit to enable Test Mode (required for selecting test mode - RDA) */
    BIT_SET(sl_canREG->CTL, BIT(CAN_CTRL_TEST_EN));

    /* set Init bit (enter software initialization mode) and avoid conflicts with Message Handler
     * This step is required before entering RDA mode */
    BIT_SET(sl_canREG->CTL, BIT(CAN_CTRL_INIT));

    /* enable Ram Direct Access (RDA) */
    BIT_SET(sl_canREG->TEST, BIT(CAN_TEST_RDA_EN));

    /* backup DATA stored at this location */
    dataVal = *data;

    /* enable SECDED diagnostic mode */
    BF_SET(sl_canREG->ECCDIAG, CAN_ECCDIAG_SECDED_EN, 0, 4);

#if FUNCTION_PROFILING_ENABLED
    SL_Record_Errorcreationtick(testType);
#endif

    switch(testType)
    {
    case CAN_ECC_TEST_MODE_1BIT:
    case CAN_ECC_TEST_MODE_1BIT_FAULT_INJECT:

        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        regBkupIntEnaSet = sl_esmREG->IESR7;

        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBckupErrInfulence = sl_esmREG->IEPSR7;

        if((FALSE) == _sl_fault_injection) {
            sl_esmREG->IECR7 = GET_ESM_BIT_NUM(esm1bitecc);
            sl_esmREG->IEPCR7 = GET_ESM_BIT_NUM(esm1bitecc);
        } 

        /* enable ECC single bit error event */
        BF_SET(sl_canREG->ECC_CS , CAN_ECC_CS_SBE_EVT_EN, CAN_ECC_CS_SBE_EVT_START, CAN_ECC_CS_SBE_EVT_LENGTH);
        /* corrupt the data */
        *data ^= CAN_FLIP_DATA_1BIT;

        /* enable SECDED */
        BF_SET(sl_canREG->CTL, CAN_CTRL_SECDED_EN, CAN_CTRL_SECDED_START, CAN_CTRL_SECDED_LENGTH);

        /* set the SL Test flag as close as possible to the creation of error.
         * This minimizes the chance of a race condition in the reading of the
         * test flag incorrectly. */
        SL_FLAG_SET(testType);
        
        /* create fault */
        ramread32 = *data;

        if(CAN_ECC_TEST_MODE_1BIT == testType) {
            /* Correctable ESM event for all DCAN instances is registered in ESM
             * status SR7[0]  */
            if(GET_ESM_BIT_NUM(esm1bitecc) == 
                    (sl_esmREG->SR7[0] & GET_ESM_BIT_NUM(esm1bitecc))) {
                testPassed = TRUE;
                /* clear ESM error status */
                sl_esmREG->SR7[0] = GET_ESM_BIT_NUM(esm1bitecc);
            }

            if(testPassed && 
                ((sl_canREG->ECC_CS & BIT(CAN_ECC_SBERR)) == BIT(CAN_ECC_SBERR)) && 
                ((sl_canREG->ECCDIAG_STAT & BIT(CAN_ECC_SBERR)) == BIT(CAN_ECC_SBERR)) &&
                (msgNo == (sl_canREG->ECC_SBEC & CAN_ECC_SERR_MSG_NO))) {
                if(CAN_ECC_CS_ECC_MODE_DIS == 
                        BF_GET(sl_canREG->ECC_CS, CAN_ECC_CS_ECC_MODE_START, CAN_ECC_CS_ECC_MODE_LENGTH)) {
                    if((sl_canREG->ES & BIT(CAN_ECC_ES_PER)) == BIT(CAN_ECC_ES_PER)) {
                        ramRead = sl_canREG->ES;
                        retVal = TRUE;
                    } else {
                        retVal = FALSE;
                    }
                } else {
                    retVal = TRUE;
                }


            } else {
                retVal = FALSE;
            }
        }
        else {
            /* fault injected */
            retVal = TRUE;
        }


        /* clear the single bit error status */
        /*BIT_SET(sl_canREG->ECC_CS, BIT(CAN_ECC_SBERR));*/
        /*BIT_SET(sl_canREG->ECCDIAG_STAT, BIT(CAN_ECC_SBERR));*/
        sl_canREG->ECC_CS = BIT(CAN_ECC_SBERR);
        sl_canREG->ECCDIAG_STAT = BIT(CAN_ECC_SBERR);

        /* disable SECDED - write to PMD in CANCTL */
        BF_SET(sl_canREG->CTL, CAN_CTRL_SECDED_DIS, CAN_CTRL_SECDED_START, CAN_CTRL_SECDED_LENGTH);
        /* Restore data; in other cases data should be auto corrected. */
        *data = dataVal;
        /* enable SECDED */
        BF_SET(sl_canREG->CTL, CAN_CTRL_SECDED_EN, CAN_CTRL_SECDED_START, CAN_CTRL_SECDED_LENGTH);


        /* disable diagnostic mode */
        BF_SET(sl_canREG->ECCDIAG, CAN_ECCDIAG_SECDED_DIS, 0, 4);
        /* Disable Ram Direct Access (RDA) */
        BIT_CLEAR(sl_canREG->TEST, BIT(CAN_TEST_RDA_EN));

        /* Restore CAN registers states */
        sl_canREG->CTL = regBackupCtl;
        sl_canREG->ECC_CS = regBackupEccCS;

        /* Restore grp1 esm interrupt enable */
        sl_esmREG->IESR7 = regBkupIntEnaSet;
        sl_esmREG->IEPSR7 = regBckupErrInfulence;

        break;

    case CAN_ECC_TEST_MODE_2BIT:
    case CAN_ECC_TEST_MODE_2BIT_FAULT_INJECT:

        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        regBkupIntEnaSet = *esm2bitIESR;

        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBckupErrInfulence = *esm2bitEEPAPR;

        if((FALSE) == _sl_fault_injection) {
            *esm2bitIECR    = GET_ESM_BIT_NUM(esm2bitecc);
            *esm2bitDEPAPR = GET_ESM_BIT_NUM(esm2bitecc);
        }

        /* doubt - corrupt the data/ecc */
        *data ^= CAN_FLIP_DATA_2BIT;

        /* enable SECDED */
        BF_SET(sl_canREG->CTL, CAN_CTRL_SECDED_EN, CAN_CTRL_SECDED_START, CAN_CTRL_SECDED_LENGTH);

        /* set the SL Test flag as close as possible to the creation of error.
         * This minimizes the chance of a race condition in the reading of the
         * test flag incorrectly. */
        SL_FLAG_SET(testType);
        ramread32 = *data;

        if(CAN_ECC_TEST_MODE_2BIT == testType) {
            if(GET_ESM_BIT_NUM(esm2bitecc) == 
                    ((*esm2bitStatReg) & GET_ESM_BIT_NUM(esm2bitecc))) {
                testPassed = TRUE;
                /* clear ESM error status */
                *esm2bitStatReg = GET_ESM_BIT_NUM(esm2bitecc);
            }

            if(testPassed  && 
                    ((sl_canREG->ECC_CS & BIT(CAN_ECC_UNCORR_ERR)) == BIT(CAN_ECC_UNCORR_ERR)) &&
                    ((sl_canREG->ECCDIAG_STAT & BIT(CAN_ECC_UNCORR_ERR)) == BIT(CAN_ECC_UNCORR_ERR)) &&
                    ((sl_canREG->ES & BIT(CAN_ECC_UNCORR_ERR)) == BIT(CAN_ECC_UNCORR_ERR))) {
                retVal = TRUE;
            } else {
                retVal = FALSE;
            }
        }
        else {
            /* fault injected */
            retVal = TRUE;
        }
        /* disable SECDED - write to PMD in CANCTL */
        BF_SET(sl_canREG->CTL, CAN_CTRL_SECDED_DIS, CAN_CTRL_SECDED_START, CAN_CTRL_SECDED_LENGTH);
        /* Restore data */
        *data = dataVal;
        /* enable SECDED */
        BF_SET(sl_canREG->CTL, CAN_CTRL_SECDED_EN, CAN_CTRL_SECDED_START, CAN_CTRL_SECDED_LENGTH);

        /* clear the double bit error status */
        /*BIT_SET(sl_canREG1->ECC_CS, BIT(CAN_ECC_UNCORR_ERR));*/
        /*BIT_SET(sl_canREG1->ECCDIAG_STAT, BIT(CAN_ECC_UNCORR_ERR));*/
        sl_canREG->ECC_CS = BIT(CAN_ECC_UNCORR_ERR);
        sl_canREG->ECCDIAG_STAT = BIT(CAN_ECC_UNCORR_ERR);

        /* disable diagnostic mode */
        BF_SET(sl_canREG->ECCDIAG, CAN_ECCDIAG_SECDED_DIS, 0, 4);
        /* Disable Ram Direct Access (RDA) */
        BIT_CLEAR(sl_canREG->TEST, BIT(CAN_TEST_RDA_EN));

        /* Restore CAN registers states */
        sl_canREG->ECC_CS = regBackupEccCS;
        sl_canREG->CTL = regBackupCtl;

        /* Restore grp1 esm interrupt enable */
        *esm2bitIESR = regBkupIntEnaSet;
        *esm2bitEEPAPR = regBckupErrInfulence;
        break;

    default:
        retVal = FALSE;
        break;
    }
    SL_FLAG_CLEAR(testType);

    /* If the test was fault injection and not self test, then do not mask the call back & clear the *
    *  FAULT_INJECTION flag for subsequent runs*/
    if((TRUE) == _sl_fault_injection){
        _sl_fault_injection = FALSE;
    }
    ramread32 = ramread32; /* Avoid compiler warning. */

#if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_CAN, testType, (SL_SelfTest_Result)retVal , 0u);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}

#define mibspi1uerrESMReg (volatile uint32*)&(sl_esmREG->SR1[0])
#define mibspi1iesrESMReg (volatile uint32*)&(sl_esmREG->IESR1)
#define mibspi1iecrESMReg (volatile uint32*)&(sl_esmREG->IECR1)
#define mibspi1eepaprESMReg (volatile uint32*)&(sl_esmREG->EEPAPR1)
#define mibspi1depaprESMReg (volatile uint32*)&(sl_esmREG->DEPAPR1)

#define mibspi2uerrESMReg (volatile uint32*)&(sl_esmREG->SR4[0])
#define mibspi2iesrESMReg (volatile uint32*)&(sl_esmREG->IESR4)
#define mibspi2iecrESMReg (volatile uint32*)&(sl_esmREG->IECR4)
#define mibspi2eepaprESMReg (volatile uint32*)&(sl_esmREG->IEPSR4)
#define mibspi2depaprESMReg (volatile uint32*)&(sl_esmREG->IEPCR4)

#define mibspi3uerrESMReg (volatile uint32*)&(sl_esmREG->SR1[0])
#define mibspi3iesrESMReg (volatile uint32*)&(sl_esmREG->IESR1)
#define mibspi3iecrESMReg (volatile uint32*)&(sl_esmREG->IECR1)
#define mibspi3eepaprESMReg (volatile uint32*)&(sl_esmREG->EEPAPR1)
#define mibspi3depaprESMReg (volatile uint32*)&(sl_esmREG->DEPAPR1)

#define mibspi4uerrESMReg (volatile uint32*)&(sl_esmREG->SR4[0])
#define mibspi4iesrESMReg (volatile uint32*)&(sl_esmREG->IESR4)
#define mibspi4iecrESMReg (volatile uint32*)&(sl_esmREG->IECR4)
#define mibspi4eepaprESMReg (volatile uint32*)&(sl_esmREG->IEPSR4)
#define mibspi4depaprESMReg (volatile uint32*)&(sl_esmREG->IEPCR4)

#define mibspi5uerrESMReg (volatile uint32*)&(sl_esmREG->SR1[0])
#define mibspi5iesrESMReg (volatile uint32*)&(sl_esmREG->IESR1)
#define mibspi5iecrESMReg (volatile uint32*)&(sl_esmREG->IECR1)
#define mibspi5eepaprESMReg (volatile uint32*)&(sl_esmREG->EEPAPR1)
#define mibspi5depaprESMReg (volatile uint32*)&(sl_esmREG->DEPAPR1)
boolean SL_SelfTest_MibSPI(SL_SelfTestType testType, SL_MIBSPI_Instance instance)
{
    boolean testPassed = FALSE;
    volatile boolean _sl_fault_injection = FALSE;
    volatile uint32 ramRead;
    register uint32 ramread32, regBackupEccCtl, regBckupErrInfulence, regBkupIntEnaSet, regBackuMIBSPIE;
    volatile uint32* data;
    uint32           dataVal;
    uint32 offset;

    sl_mibspiBASE_t* sl_mibspiREG;
    volatile uint32 *sl_mibspiRAM;
    uint32 esm1bitecc, esm2bitecc;
    volatile uint32* esm2bitStatReg;
    volatile uint32* esm2bitEEPAPR;
    volatile uint32* esm2bitDEPAPR; 
    volatile uint32* esm2bitIESR;   
    volatile uint32* esm2bitIECR;   


#if FUNCTION_PROFILING_ENABLED
    SL_Start_Profiling(testType);
#endif

    boolean retVal = FALSE;

#ifdef FUNC_ENTRY_COND_CHECK_ENABLED
    /*check for privileged mode*/
    if (ARM_MODE_USR == _SL_Get_ARM_Mode()) {
        SL_Log_Error(FUNC_ID_ST_SPI, ERR_TYPE_ENTRY_CON, 0u);
        retVal =  FALSE;
        return(retVal);
    }
#endif

    /* If nERROR is active then do not proceed with tests that trigger nERROR */
    if((TRUE) == SL_ESM_nERROR_Active())
    {
        SL_Log_Error(FUNC_ID_ST_SPI, ERR_TYPE_ENTRY_CON, 3U);
        return(FALSE);
    }

    /* If fault inject set global variable to flag to the ESM handler that it is a fault injection */
    if((MIBSPI_ECC_TEST_MODE_1BIT_FAULT_INJECT == testType)||
        (MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT == testType)){
        _sl_fault_injection=TRUE;
    }

    switch(instance)
    {
        case SL_MIBSPI1:
            sl_mibspiREG = sl_mibspiREG1;
            sl_mibspiRAM = (volatile uint32*)sl_mibspiRAM1;
            esm1bitecc     = ESM_G1ERR_SPI1_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_SPI1_ECC_UNCORR;
            esm2bitStatReg = mibspi1uerrESMReg;
            esm2bitEEPAPR  = mibspi1eepaprESMReg;
            esm2bitDEPAPR  = mibspi1depaprESMReg;
            esm2bitIESR    = mibspi1iesrESMReg;
            esm2bitIECR    = mibspi1iecrESMReg;

            break;
        case SL_MIBSPI2:
            sl_mibspiREG = sl_mibspiREG2;
            sl_mibspiRAM = (volatile uint32*)sl_mibspiRAM2;
            esm1bitecc     = ESM_G1ERR_SPI2_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_SPI2_ECC_UNCORR;
            esm2bitStatReg = mibspi2uerrESMReg;
            esm2bitEEPAPR  = mibspi2eepaprESMReg;
            esm2bitDEPAPR  = mibspi2depaprESMReg;
            esm2bitIESR    = mibspi2iesrESMReg;
            esm2bitIECR    = mibspi2iecrESMReg;

            break;
        case SL_MIBSPI3:
            sl_mibspiREG = sl_mibspiREG3;
            sl_mibspiRAM = (volatile uint32*)sl_mibspiRAM3;
            esm1bitecc     = ESM_G1ERR_SPI3_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_SPI3_ECC_UNCORR;
            esm2bitStatReg = mibspi3uerrESMReg;
            esm2bitEEPAPR  = mibspi3eepaprESMReg;
            esm2bitDEPAPR  = mibspi3depaprESMReg;
            esm2bitIESR    = mibspi3iesrESMReg;
            esm2bitIECR    = mibspi3iecrESMReg;
            break;
        case SL_MIBSPI4:
            sl_mibspiREG = sl_mibspiREG4;
            sl_mibspiRAM = (volatile uint32*)sl_mibspiRAM4;
            esm1bitecc     = ESM_G1ERR_SPI4_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_SPI4_ECC_UNCORR;
            esm2bitStatReg = mibspi4uerrESMReg;
            esm2bitEEPAPR  = mibspi4eepaprESMReg;
            esm2bitDEPAPR  = mibspi4depaprESMReg;
            esm2bitIESR    = mibspi4iesrESMReg;
            esm2bitIECR    = mibspi4iecrESMReg;
            break;
        case SL_MIBSPI5:
            sl_mibspiREG = sl_mibspiREG5;
            sl_mibspiRAM = (volatile uint32*)sl_mibspiRAM5;
            esm1bitecc     = ESM_G1ERR_SPI5_ECC_SBERR;
            esm2bitecc     = ESM_G1ERR_SPI5_ECC_UNCORR;
            esm2bitStatReg = mibspi5uerrESMReg;
            esm2bitEEPAPR  = mibspi5eepaprESMReg;
            esm2bitDEPAPR  = mibspi5depaprESMReg;
            esm2bitIESR    = mibspi5iesrESMReg;
            esm2bitIECR    = mibspi5iecrESMReg;
            break;
    }

    if(BF_GET(sl_mibspiREG->MIBSPIE, SPI_MIBSPIE_EX_BUF_START, SPI_MIBSPIE_EX_BUF_LENGTH) == SPI_MIBSPIE_EX_BUF_DIS) {
        offset = DEFAULT;
    } else {
        offset = EXTENDED;
    }

    /* backUp registers */
    regBackupEccCtl = sl_mibspiREG->PAR_ECC_CTRL;
    regBackuMIBSPIE = sl_mibspiREG->MIBSPIE;

    /* enable ECC memory test */
    BIT_SET(sl_mibspiREG->PAR_ECC_CTRL, SPI_ECC_MEMTST_EN);

    /* enable RXRAM access */
    BIT_SET(sl_mibspiREG->MIBSPIE, SPI_MIBSPI_RXRAM_ACCESS);

    /* enable ECC diagnostic mode */
    BF_SET(sl_mibspiREG->ECC_DIAG_CTRL, SPI_DIAG_CTRL_ECC_EN, SPI_DIAG_CTRL_ECC_START, SPI_DIAG_CTRL_ECC_LENGTH);

    /* initialize data */
    data = (volatile uint32 *)((uint32)sl_mibspiRAM + offset + 4);

    /* backUp the data */
    dataVal = (uint32)(*data);

#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif

#if FUNCTION_PROFILING_ENABLED
        SL_Record_Errorcreationtick(testType);
#endif

    switch(testType)
    {
    case MIBSPI_ECC_TEST_MODE_1BIT:
    case MIBSPI_ECC_TEST_MODE_1BIT_FAULT_INJECT:

        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        regBkupIntEnaSet = sl_esmREG->IESR7;

        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBckupErrInfulence = sl_esmREG->IEPSR7;

        if((FALSE) == _sl_fault_injection) {
            sl_esmREG->IECR7 = GET_ESM_BIT_NUM(esm1bitecc);
            sl_esmREG->IEPCR7 = GET_ESM_BIT_NUM(esm1bitecc);
        } 

        SL_FLAG_SET(testType);
        /* enable single bit error event */
        BF_SET(sl_mibspiREG->PAR_ECC_CTRL, SPI_PAR_ECC_CTRL_SBE_EVT_EN, SPI_PAR_ECC_CTRL_SBE_EVT_START, SPI_PAR_ECC_CTRL_SBE_EVT_LENGTH);

        /* corrupt the data */
        BIT_FLIP(*data, SPI_FLIP_DATA_1BIT);

        /* enable SECDED */
        BF_SET(sl_mibspiREG->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_EN, SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);

        ramread32 = *data;

        if(MIBSPI_ECC_TEST_MODE_1BIT == testType) {
            if(GET_ESM_BIT_NUM(esm1bitecc) == 
                    (sl_esmREG->SR7[0] & GET_ESM_BIT_NUM(esm1bitecc))) {
                testPassed = TRUE;
                /* clear ESM error status */
                sl_esmREG->SR7[0] = GET_ESM_BIT_NUM(esm1bitecc);
            }

            
            if((testPassed == TRUE) && 
                    ((sl_mibspiREG->PAR_ECC_STAT & BIT(SPI_PAR_ECC_STAT_SBEFLGRX)) == BIT(SPI_PAR_ECC_STAT_SBEFLGRX)) &&
                    ((sl_mibspiREG->ECC_DIAG_STAT & BIT(SPI_ECCDIAG_STAT_SBEFLGRX)) == BIT(SPI_ECCDIAG_STAT_SBEFLGRX)) &&
                    ((offset + 4) == (uint32)BF_GET(sl_mibspiREG->SBERR_ADDRRX, SPI_SBERR_ADDRRX_START, SPI_SBERR_ADDRRX_LENGTH))){


                retVal = TRUE;

            } else {
                retVal = FALSE;
            }
        }
        else {
            /* Fault injected */
            retVal = TRUE;
        }

        /* clear the single bit error status */
        sl_mibspiREG->PAR_ECC_STAT = BIT(SPI_PAR_ECC_STAT_SBEFLGRX);
        sl_mibspiREG->ECC_DIAG_STAT = BIT(SPI_ECCDIAG_STAT_SBEFLGRX);
        /* Restore data */
        *data = dataVal;

        /* disable diagnostic mode */
        BF_SET(sl_mibspiREG->ECC_DIAG_CTRL, SPI_DIAG_CTRL_ECC_DIS, SPI_DIAG_CTRL_ECC_START, SPI_DIAG_CTRL_ECC_LENGTH);

        /* Restore SPI registers states */
        sl_mibspiREG->PAR_ECC_CTRL = regBackupEccCtl;
        sl_mibspiREG->MIBSPIE = regBackuMIBSPIE;

        /* Restore grp1 esm interrupt enable */
        sl_esmREG->IESR7 = regBkupIntEnaSet;
        sl_esmREG->IEPSR7 = regBckupErrInfulence;

        break;

    case MIBSPI_ECC_TEST_MODE_2BIT:
    case MIBSPI_ECC_TEST_MODE_2BIT_FAULT_INJECT:

        /*Backup grp1 esm interrupt enable register and clear the interrupt enable */
        regBkupIntEnaSet = *esm2bitIESR;

        /*SAFETYMCUSW 134 S MR: 12.2 <APPROVED> Comment_5*/
        regBckupErrInfulence = *esm2bitEEPAPR;

        if((FALSE) == _sl_fault_injection) {
            *esm2bitIECR = GET_ESM_BIT_NUM(esm2bitecc);
            *esm2bitDEPAPR = GET_ESM_BIT_NUM(esm2bitecc);
        } 

        SL_FLAG_SET(testType);

        /* corrupt the data */
        *data ^= SPI_FLIP_DATA_2BIT;

        /* enable SECDED */
        BF_SET(sl_mibspiREG->PAR_ECC_CTRL, SPI_PAR_CTRL_ECC_EN, SPI_PAR_CTRL_ECC_START, SPI_PAR_CTRL_ECC_LENGTH);

        ramread32 = *data;

        if(MIBSPI_ECC_TEST_MODE_2BIT == testType) {
            if(GET_ESM_BIT_NUM(esm2bitecc) == 
                    (*esm2bitStatReg & GET_ESM_BIT_NUM(esm2bitecc))) {
                        testPassed = TRUE;
                        /* clear ESM error status */
                        *esm2bitStatReg = GET_ESM_BIT_NUM(esm2bitecc);
            }

            
            if((testPassed == TRUE) && 
                    ((sl_mibspiREG->PAR_ECC_STAT & BIT(SPI_PAR_ECC_STAT_UERRFLGRX)) == BIT(SPI_PAR_ECC_STAT_UERRFLGRX)) &&
                    ((sl_mibspiREG->ECC_DIAG_STAT & BIT(SPI_ECCDIAG_STAT_DEFLGRX)) == BIT(SPI_ECCDIAG_STAT_DEFLGRX)) &&
                    ((offset + 4) == (uint32)BF_GET(sl_mibspiREG->UERRADDRRX, SPI_UERR_ADDRRX_START, SPI_UERR_ADDRRX_LENGTH))) {


                retVal = TRUE;
            } else {
                retVal = FALSE;
            }
        }
        else {
            /* Fault injected */
            retVal = TRUE;
        }

        /* clear the single bit error status */
        sl_mibspiREG->PAR_ECC_STAT = BIT(SPI_PAR_ECC_STAT_UERRFLGRX);
        sl_mibspiREG->ECC_DIAG_STAT = BIT(SPI_PAR_ECC_STAT_UERRFLGRX);

        /* Restore data */
        *data = dataVal;

        /* disable diagnostic mode */
        BF_SET(sl_mibspiREG->ECC_DIAG_CTRL, SPI_DIAG_CTRL_ECC_DIS, SPI_DIAG_CTRL_ECC_START, SPI_DIAG_CTRL_ECC_LENGTH);

        /* Restore SPI registers states */
        sl_mibspiREG->PAR_ECC_CTRL = regBackupEccCtl;
        sl_mibspiREG->MIBSPIE = regBackuMIBSPIE;

        /* restore grp1 ESM interrupt enable */
        *esm2bitIESR = regBkupIntEnaSet;
        *esm2bitEEPAPR = regBckupErrInfulence;

        break;

    default:
        retVal = FALSE;
        break;
    }

    /* If the test was fault injection and not self test, then do not mask the call back & clear the *
    *  FAULT_INJECTION flag for subsequent runs*/
    if((TRUE) == _sl_fault_injection){
            _sl_fault_injection=FALSE;
    }
    SL_FLAG_CLEAR(testType);

    ramread32 = ramread32; /* Avoid compiler warning. */

    #if(FUNC_RESULT_LOG_ENABLED == 1)
    SL_Log_Result(FUNC_ID_ST_SPI, testType, (SL_SelfTest_Result)retVal , 0u);
#endif
#if FUNCTION_PROFILING_ENABLED
    SL_Stop_Profiling(testType);
#endif
    return(retVal);
}

#endif

/*Comment_1:
 *  "Reason - used for creation of 16 byte aligned sram data array"*/

/*Comment_2:
 *  "Reason -  used for creation of aligned data section for sramEccTestBuff"*/

/*Comment_3:
 *  "Reviewed - The entry condition and parameter checks shall exit immediately"*/

/*Comment_4:
 *  "Reason -  This is an advisory by MISRA.Verified validity of operation by review"*/

/*Comment_5:
 * "Reason -  FALSE_POSITIVE The rule requires that the expression should be
 * guaranteed to have the same value regardless of the order in which the operands within the expression are evaluated.
 * This usually means no more than one volatile access occuring in the expression.The recommendation that simple
 * assignments should be used is intended to be restricted to simple assignments of the form x = v and not
 * "simple assignments" according to the ISO definition. However, the recommendation does not have to be followed
 * in order to be compliant with the rule. You should implement the rule (the value should be independent of the
 * evaluation order) for compliance purposes and you might choose to issue a warning for non-simple assignments
 * or you might choose to ignore them."*/

/*Comment_6:
 *  "Reason -  This can never be NULL as addresss points to an array"*/

/*Comment_7:
 *  "Reason -  This can never be NULL"*/

/*Comment_8:
 * "Address of volatile variable used, not the variable itself"*/

/*Comment_9:
 * "Address of volatile variable used, not the variable itself"*/

/*Comment_10:
 * "Assignment in an expression is valid here"*/

/*Comment_11:
 * "Needed to generate error"*/

/*Comment_12:
 * "Address of volatile variable accessed, not the variable itself"*/

/*Comment_13:
 * "Reason - Design change necessary to remove the infinite while loop"*/

/*Comment_14:
 * "Reason -  CHECK_RANGE_RAM_PTR is used to check the valid range"*/

/*Comment_15:
 * "This function will be called by application so not static" */

/*Comment_16:
 * "Reason -  Needed for the test"*/

/*Comment_17:
 * "changes may be made in the usage of the testType" */

/*Comment_18:
 * "The base address of the peripheral registers are actually macros" */

/*Comment_19:
 * "Reason - FALSE POSITIVE _SELFTEST_PBIST_MIN and testType are object pointers of same type*/

/*Comment_20:
 * "Reason -  This is an advisory by MISRA.Verified validity of operation by review"*/

/*Comment_21:
 * "The base address of the peripheral registers are actually macros"*/

/*Comment_22:
 * "Name declared in another C name space is deviated"*/

/*Comment_23:
 * "changes may be made in the usage of the config"*/

/*Comment_24:
 * "Overall design change needed" */

/*Comment_25:
 * "False positive" */
