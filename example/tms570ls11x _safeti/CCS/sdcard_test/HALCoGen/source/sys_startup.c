/** @example example_SafetyLib.c
*   This is an example which describes the steps to create an sample startup which
*   configures runs safety diagnostic tests using SafeTI Diagnostic Library(2.0.0 or higher).
*
*   @b Step @b 1:
*
*   Create a HALCoGen -CCS project with required configuration.
*
*   @b Step @b 2:
*
*   Add all the files in the folder "<SL installation path>\safety_library\source" in the project
*   Include the following directories to #include search path
*      - "${workspace_loc:/${ProjName}/include}"
*      - "${workspace_loc:/${ProjName}/<folder where SL files are added>}"
*      - "<SL installation path>\hal\include"
*      - "<SL installation path>\safety_library\include"
*
*   NOTE: Edit the sl_config.h file in the folder safety_library/include as follows:
*
*   @image html SafetyLib_config_h_file.JPG "Figure: sl_config.h file"
*
*   @b Step @b 3:
*
*   Add the following macros (without quotes) as predefined symbols to the project (Project Properties->CCS Build->ARM Compiler->Advanced Options->Predefined Symbols):
*      - "_TMS570LS12x_"
*      - "_VFP_SUPPORT_=1"
*      - "EXTERNAL_SP_INIT"
*
*   @b Step @b 4:
*
*   Add the following in the SECTIONS block in the linker command file
*
*      LOG_DATA          : START( ulLOGStartAddr ), END( ulLOGEndAddr ) > RAM
*
*
*   @b Step @b 6:
*
*   Copy the source code below into your sys_startup.c or replace sys_startup.c with this file.
*
*   The example file can also be found in the examples folder: ../HALCoGen/examples
*
*
*/

/* 
* Copyright (C) 2009-2015 Texas Instruments Incorporated - www.ti.com
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/* Include Files */

#include "sys_common.h"
#include "system.h"
#include "sys_vim.h"
#include "sys_core.h"
#include "sys_selftest.h"
#include "esm.h"
#include "esm_application_callback.h"
#include "sl_api.h"
#include "sl_priv.h"

/* External Functions */
/*SAFETYMCUSW 218 S MR:20.2 <APPROVED> "Functions from library" */
extern void __TI_auto_init(void);
/*SAFETYMCUSW 354 S MR:NA <APPROVED> " Startup code(main should be declared by the user)" */
extern void main(void);
/*SAFETYMCUSW 122 S MR:20.11 <APPROVED> "Startup code(exit and abort need to be present)" */
/*SAFETYMCUSW 354 S MR:NA <APPROVED> " Startup code(Extern declaration present in the library)" */
extern void exit(void);

void afterSTC(void);

//void esmCallBackFunction (uint32 grp_channel, uint32 param1, uint32 param2, uint32 param3);
#define SL_REG_INIT_VAL         0u

/* Startup Routine */
void _c_int00(void);

SL_ResetReason 	resetReason;		/* Reset reason */

#pragma CODE_STATE(_c_int00, 32)
#pragma INTERRUPT(_c_int00, RESET)
void _c_int00(void)
{
	/* NOTE: DO NOT INITIALIZE ANY VARIABLES !! (will cause DABORT because of un-initialized SP) */
	volatile boolean 			retVal;             /* For function return values */
    SL_STC_Config 				stcSelfTestConfig;  /* STC Configuration */

    _coreInitRegisters_();

    _coreInitStackPointer_();

    resetReason = SL_Init_ResetReason();
    if (RESET_TYPE_POWERON == resetReason)
    {
        /* clear all reset status flags */
        SYS_EXCEPTION = 0xFFFFU;

        /* Workaround for Errata CORTEXR4 66 */
        _errata_CORTEXR4_66_();


        /* Workaround for Errata CORTEXR4 57 */
        _errata_CORTEXR4_57_();
	}
	else if (RESET_TYPE_OSC_FAILURE == resetReason)
	{
		/* Empty */
	}
	else if (RESET_TYPE_WATCHDOG == resetReason)
	{
		/* Reset caused due
		 *  1) windowed watchdog violation - Add user code here to handle watchdog violation.
		 *  2) ICEPICK Reset - After loading code via CCS / System Reset through CCS
		 */
		/* Check the WatchDog Status register */
	    if(WATCHDOG_STATUS != 0U)
		{
            /* Clear the Watchdog reset flag in Exception Status register */
            SYS_EXCEPTION = WATCHDOG_RESET;
        }
        else
        {
            /* Clear the ICEPICK reset flag in Exception Status register */
            SYS_EXCEPTION = ICEPICK_RESET;
		}
	}
	else if ((RESET_TYPE_CPU == resetReason)&&((RESET_TYPE_DEBUG != resetReason)))
	{
		/* Reset caused due to CPU reset.
		CPU reset can be caused by CPU self-test completion, or
		by toggling the "CPU RESET" bit of the CPU Reset Control Register. */

        /* clear all reset status flags */
        SYS_EXCEPTION = CPU_RESET;

        /* reset could be caused by stcSelfCheck run or by an actual CPU self-test run */

        /* check if this was an stcSelfCheck run */
        if ((stcREG->STCSCSCR & 0xFU) == 0xAU)
        {
            /* check if the self-test fail bit is set */
            if ((stcREG->STCGSTAT & 0x3U) != 0x3U)
            {
                /* STC self-check has failed */
                while(1);
            }
            /* STC self-check has passed */
            else
            {
                /* clear self-check mode */
                stcREG->STCSCSCR = 0x05U;

                /* clear STC global status flags */
                stcREG->STCGSTAT = 0x3U;

                /* clear ESM group1 channel 27 status flag */
                esmREG->SR1[0U] = 0x08000000U;

                /* Start CPU Self-Test */
				/*********************************************************************************************************/
				/*cpuSelfTest(STC_INTERVAL, STC_MAX_TIMEOUT, TRUE);*/
		    	stcSelfTestConfig.stcClockDiv		= 0; 			/* STC Clock divider = 1 */
		    	stcSelfTestConfig.intervalCount 	= 1; 			/* One interval only */
		    	stcSelfTestConfig.restartInterval0 	= TRUE; 		/* Start from interval 0 */
		    	stcSelfTestConfig.timeoutCounter 	= 0xFFFFFFFF; 	/* Timeout counter*/
		    	_SL_HoldNClear_nError();
				SL_SelfTest_STC(STC_RUN, TRUE, &stcSelfTestConfig);
				/*********************************************************************************************************/
            }
        }
        /* CPU reset caused by CPU self-test completion */
        else if ((stcREG->STCGSTAT & 0x1U) == 0x1U)
        {
            /* Self-Test Fail flag is set */
            if ((stcREG->STCGSTAT & 0x2U) == 0x2U)
            {
                /* Call CPU self-test failure handler */
                while(1);
            }
            /* CPU self-test completed successfully */
            else
            {
                /* clear STC global status flag */
                stcREG->STCGSTAT = 0x1U;

                /* Continue start-up sequence after CPU STC completed */
                afterSTC();
            }
        }
        /* CPU reset caused by software writing to CPU RESET bit */
        else
        {
            /* Add custom routine here to handle the case where software causes CPU reset */
        }

	}
	else if (RESET_TYPE_SWRST == resetReason)
	{
		/* Empty */
    }
    else
    {
        /* Reset caused by nRST being driven low externally.
        Add user code to handle external reset. */
    }

    /* Check if there were ESM group3 errors during power-up.
     * These could occur during eFuse auto-load or during reads from flash OTP
     * during power-up. Device operation is not reliable and not recommended
     * in this case.
     * An ESM group3 error only drives the nERROR pin low. An external circuit
     * that monitors the nERROR pin must take the appropriate action to ensure that
     * the system is placed in a safe state, as determined by the application.
     */
    if ((esmREG->SR1[2]) != 0U)
    {
    	//esmREG->SR1[2] = 0xFFFFFFFF;
        for(;;)
        {
        }/* Wait */
    }

    /* Initialize System - Clock, Flash settings with Efuse self check */
    systemInit();


	if(RESET_TYPE_DEBUG != resetReason)
	{
		/* Make sure that the CPU self-test controller can actually detect a fault inside CPU */
		stcSelfTestConfig.stcClockDiv		= 0; 			/* STC Clock divider = 1 */
		stcSelfTestConfig.intervalCount 	= 1; 			/* One interval only */
		stcSelfTestConfig.restartInterval0 	= TRUE; 		/* Start from interval 0 */
		stcSelfTestConfig.timeoutCounter 	= 0xFFFFFFFF; 	/* Timeout counter*/
		_SL_HoldNClear_nError();
		SL_SelfTest_STC(STC_COMPARE_SELFCHECK, TRUE, &stcSelfTestConfig);
	}else
	{
		afterSTC();
	}

}

void afterSTC(void)
{

    SL_CCMR4F_FailInfo			failInfoCCMR4F;		/* CCMR4 Self Test fail info */
	volatile boolean 			retVal;             /* For function return values */
    SL_PBIST_FailInfo           failInfoPBISTSRAM;  /* PBIST Failure information for TCM RAM */
    SL_SelfTest_Result          failInfoFlash;      /* Flash Self test failure information */
    SL_SelfTest_Result          failInfoTCMRAM;     /* TCM RAM Failure  information */
    SL_PBIST_FailInfo           failInfoPBISTOthers;/* PBIST Failure information for non-TCM memories */
    SL_PSCON_FailInfo           failInfoPSCON;      /* PSCON failure information */
    SL_EFuse_Config             stConfigEFuse;      /* EFuse self test configuration */


	/* Make sure that CCM-R4F is working as expected.
	 * This function puts the CCM-R4F module through its self-test modes.
	 * It ensures that the CCM-R4F is indeed capable of detecting a CPU mismatch,
	 * and is also capable of indicating a mismatch error to the ESM.
	 */
	/* Can be enabled when in release*/

    /* Execute PBIST tests on required peripheral SRAMs */
	_SL_HoldNClear_nError();
    retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
                                PBIST_RAMGROUP_01_PBIST_ROM,
                                PBISTALGO_TRIPLE_READ_FAST_READ);
    while (TRUE != SL_SelfTest_WaitCompletion_PBIST());
    SL_SelfTest_Status_PBIST(&failInfoPBISTSRAM);
    if (failInfoPBISTSRAM.stResult != ST_PASS)
	{
    	while(1);
    }

	/* Run a diagnostic check on the memory self-test controller.
	 * This function chooses a RAM test algorithm and runs it on an on-chip ROM.
	 * The memory self-test is expected to fail. The function ensures that the PBIST controller
	 * is capable of detecting and indicating a memory self-test failure.
	 */


    /* PBIST test on STC ROM */
	retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
									(PBIST_RAMGROUP_02_STC_ROM),
									PBISTALGO_TRIPLE_READ_FAST_READ);

	while (TRUE != SL_SelfTest_WaitCompletion_PBIST());
	SL_SelfTest_Status_PBIST(&failInfoPBISTOthers);

	if (failInfoPBISTSRAM.stResult != ST_PASS)
	{
		while(1);
	}

	/* Run PBIST on CPU RAM.
	 * The PBIST controller needs to be configured separately for single-port and dual-port SRAMs.
	 * The CPU RAM is a single-port memory. The actual "RAM Group" for all on-chip SRAMs is defined in the
	 * device datasheet.
	 */
	retVal = SL_SelfTest_PBIST(	PBIST_EXECUTE,                                          /* Execute PBIST test */
								(PBIST_RAMGROUP_06_ESRAM1 | PBIST_RAMGROUP_21_ESRAM5  /* On all TCM RAMs */
								),
								PBISTALGO_MARCH13N_RED_1PORT);
    while (TRUE != SL_SelfTest_WaitCompletion_PBIST());

	/* Get SRAM PBIST Status */
	SL_SelfTest_Status_PBIST(&failInfoPBISTSRAM);
	if (failInfoPBISTSRAM.stResult != ST_PASS)
	{
		while(1);
	}

	/* Disable PBIST clocks and disable memory self-test mode */
	SL_SelfTest_PBIST_StopExec();

	/* Initialize CPU RAM.
	 * This function uses the system module's hardware for auto-initialization of memories and their
	 * associated protection schemes. The CPU RAM is initialized by setting bit 0 of the MSIENA register.
	 * Hence the value 0x1 passed to the function.
	 * This function will initialize the entire CPU RAM and the corresponding ECC locations.
	 */
	SL_Init_Memory(RAMTYPE_RAM);
	//SL_Init_ECCFlash(10, FLASHECC_DEFAULT); /* Enable Flash ECC */

	SL_Init_ECCTCMRAM(10, TRUE);            /* Enable TCM RAM ECC */

    /* Execute PBIST tests on required peripheral SRAMs */
    retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
                                (PBIST_RAMGROUP_03_DCAN1	|
        						PBIST_RAMGROUP_04_DCAN2		|
        						PBIST_RAMGROUP_05_DCAN3		|
								PBIST_RAMGROUP_07_MIBSPI1  	|
                                PBIST_RAMGROUP_08_MIBSPI3   |
                                PBIST_RAMGROUP_09_MIBSPI5   |
                                PBIST_RAMGROUP_10_VIM       |
                                PBIST_RAMGROUP_11_MIBADC1   |
								PBIST_RAMGROUP_12_DMA		|
								PBIST_RAMGROUP_13_N2HET1	|
								PBIST_RAMGROUP_14_HETTU1	|
								PBIST_RAMGROUP_16_FRAY		|
                                PBIST_RAMGROUP_18_MIBADC2	|
								PBIST_RAMGROUP_19_N2HET2	|
								PBIST_RAMGROUP_20_HETTU2
								),
                                PBISTALGO_MARCH13N_RED_2PORT);

    while (TRUE != SL_SelfTest_WaitCompletion_PBIST());
	SL_SelfTest_Status_PBIST(&failInfoPBISTOthers);

	/* Check the PBIST status and if there is a failure then wait*/
	if (failInfoPBISTOthers.stResult != ST_PASS)
	{
		while(1);
	}

	/* PBIST test on STC ROM */
	retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
									(PBIST_RAMGROUP_17_FRAY),
									PBISTALGO_MARCH13N_RED_1PORT);

	while (TRUE != SL_SelfTest_WaitCompletion_PBIST());
	SL_SelfTest_Status_PBIST(&failInfoPBISTOthers);

	/* Check the PBIST status and if there is a failure then wait*/
	if (failInfoPBISTOthers.stResult != ST_PASS)
	{
		while(1);
	}

    /* Run 1Bit ECC test on TCM RAM */
    retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_1BIT, TRUE, &failInfoTCMRAM);
    if(retVal == FALSE) while(1);

    /* Run 1Bit ECC error profiling test on TCM RAM */
   retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_PROFILING, TRUE, &failInfoTCMRAM);
   if(retVal == FALSE) while(1);

   retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_2BIT, TRUE, &failInfoTCMRAM);
	if(retVal == FALSE) while(1);


	//SL_Init_ECCFlash(10, FLASHECC_DEFAULT); /* Enable Flash ECC */
	/* Run Diagmode 1 */
	retVal = SL_SelfTest_FEE(FEE_ECC_DATA_CORR_MODE, TRUE, &failInfoFlash);
	if(retVal == FALSE) while(1);

	/* Run 1 bit selftest */
	retVal = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_1BIT, TRUE, &failInfoFlash);
	if(retVal == FALSE) while(1);

	/* Run 2 bit selftest */
	retVal = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_2BIT, TRUE, &failInfoFlash);
	if(retVal == FALSE) while(1);

	/* Run Diagmode 2 */
	retVal = SL_SelfTest_FEE(FEE_ECC_SYN_REPORT_MODE, TRUE, &failInfoFlash);
	if(retVal == FALSE) while(1);

	/* Run Diagmode 3 */
	retVal = SL_SelfTest_FEE(FEE_ECC_MALFUNCTION_MODE1, TRUE, &failInfoFlash);
	if(retVal == FALSE) while(1);

	/* Run Diagmode 4 */
	retVal = SL_SelfTest_FEE(FEE_ECC_MALFUNCTION_MODE2, TRUE, &failInfoFlash);
	if(retVal == FALSE) while(1);

    /* Run 1Bit ECC test on Flash */
    retVal = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_1BIT, TRUE, &failInfoFlash);
    if(retVal == FALSE) while(1);

    /* Run 2Bit ECC test on Flash */
   retVal = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_2BIT, TRUE, &failInfoFlash);
   if(retVal == FALSE) while(1);

	SL_SelfTest_Status_PBIST(&failInfoPBISTOthers);

	/* Check the PBIST status and if there is a failure then wait*/
	if (failInfoPBISTOthers.stResult != ST_PASS)
	{
		while(1);
	}

	/* Disable PBIST clocks and disable memory self-test mode */
	SL_SelfTest_PBIST_StopExec();

	/* Run PSCON self tests in sync mode */
	retVal = SL_SelfTest_PSCON(PSCON_SELF_TEST, TRUE, &failInfoPSCON);
	if(retVal == FALSE) while(1);

	retVal = SL_SelfTest_PSCON(PSCON_ERROR_FORCING, TRUE, &failInfoPSCON);
	if(retVal == FALSE) while(1);

	retVal = SL_SelfTest_PSCON(PSCON_SELF_TEST_ERROR_FORCING, TRUE, &failInfoPSCON);
	if(retVal == FALSE) while(1);

	retVal = SL_SelfTest_PSCON(PSCON_PMA_TEST, TRUE, &failInfoPSCON);
	if (FALSE != retVal)
	{ /* Must fail, since PMA tests cannot be run in privilege modes */
		while(1);
	}

	/* Run EFuse self tests */
	stConfigEFuse.numPatterns      = 600u;
	stConfigEFuse.seedSignature    = 0x5362F97Fu;
	stConfigEFuse.failInfo.stResult= ST_FAIL;
	stConfigEFuse.failInfo.failInfo= EFUSE_ERROR_NONE;
	retVal = SL_SelfTest_EFuse(EFUSE_SELF_TEST_STUCK_AT_ZERO, TRUE, &stConfigEFuse);
	if(retVal == FALSE) while(1);

	retVal = SL_SelfTest_EFuse(EFUSE_SELF_TEST_ECC, TRUE, &stConfigEFuse);
	while (TRUE != SL_SelfTest_Status_EFuse(&stConfigEFuse.failInfo));
	if(retVal == FALSE) while(1);

#if 0
	/*This block can be enabled when not running the code with debugger.The ccmr4f tests do not run with debugger connected*/
	/* With debugger connected, CCM is disabled so do not run when debugger is connected */
	if (RESET_TYPE_DEBUG != resetReason) {
		/* Try CCMR4F Fault Injection */
		retVal = SL_SelfTest_CCMR4F(CCMR4F_SELF_TEST, TRUE, &failInfoCCMR4F);
		if(retVal == FALSE) while(1);

		retVal = SL_SelfTest_CCMR4F(CCMR4F_ERROR_FORCING_TEST, TRUE, &failInfoCCMR4F);
		if(retVal == FALSE) while(1);

		retVal = SL_SelfTest_CCMR4F(CCMR4F_SELF_TEST_ERROR_FORCING, TRUE, &failInfoCCMR4F);
		if(retVal == FALSE) while(1);
	}
#endif

	/* Run RAD Self tests on TCMRAM */
	retVal = SL_SelfTest_SRAM(SRAM_RADECODE_DIAGNOSTICS, TRUE, &failInfoTCMRAM);
	if(retVal == FALSE) while(1);

	retVal = SL_SelfTest_SRAM(SRAM_PAR_ADDR_CTRL_SELF_TEST, TRUE, &failInfoTCMRAM);
	if(retVal == FALSE) while(1);
    /* Enable IRQ offset via Vic controller */
    _coreEnableIrqVicOffset_();

    /* Initialize VIM table */
    vimInit();

    SL_ESM_Init(ESM_ApplicationCallback);

    /* Configure system response to error conditions signaled to the ESM group1 */
	/* This function can be configured from the ESM tab of HALCoGen */
	esmInit();

	/* initialize copy table */
	__TI_auto_init();

    /* call the application */
/*SAFETYMCUSW 296 S MR:8.6 <APPROVED> "Startup code(library functions at block scope)" */
/*SAFETYMCUSW 326 S MR:8.2 <APPROVED> "Startup code(Declaration for main in library)" */
/*SAFETYMCUSW 60 D MR:8.8 <APPROVED> "Startup code(Declaration for main in library;Only doing an extern for the same)" */
    main();

/*SAFETYMCUSW 122 S MR:20.11 <APPROVED> "Startup code(exit and abort need to be present)" */
    exit();

}

/*
void esmCallBackFunction (uint32 grp_channel, uint32 param1, uint32 param2, uint32 param3)
{

}

*/
