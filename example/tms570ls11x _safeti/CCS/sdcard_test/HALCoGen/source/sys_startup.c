/** @file sys_startup.c 
*   @brief Startup Source File
*   @date 28.Aug.2015
*   @version 04.05.01
*
*   This file contains:
*   - Include Files
*   - Type Definitions
*   - External Functions
*   - VIM RAM Setup
*   - Startup Routine
*   .
*   which are relevant for the Startup.
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
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */

/* USER CODE END */


/* Include Files */

#include "sys_common.h"
#include "system.h"
#include "sys_vim.h"
#include "sys_core.h"
#include "sys_selftest.h"
#include "esm.h"
#include "mibspi.h"

/* USER CODE BEGIN (1) */
#include "sl_api.h"
#include "sl_priv.h"
/* USER CODE END */


/* USER CODE BEGIN (2) */
#define SAFETI_INTEGRATE
/* USER CODE END */


/* External Functions */
/*SAFETYMCUSW 218 S MR:20.2 <APPROVED> "Functions from library" */
extern void __TI_auto_init(void);
/*SAFETYMCUSW 354 S MR:NA <APPROVED> " Startup code(main should be declared by the user)" */
extern void main(void);
/*SAFETYMCUSW 122 S MR:20.11 <APPROVED> "Startup code(exit and abort need to be present)" */
/*SAFETYMCUSW 354 S MR:NA <APPROVED> " Startup code(Extern declaration present in the library)" */
extern void exit(int _status);

void afterSTC(void);

/* USER CODE BEGIN (3) */
void esmCallBackFunction (uint32 grp_channel, uint32 param1, uint32 param2, uint32 param3);
/* USER CODE END */

/* Startup Routine */
void _c_int00(void);
/* USER CODE BEGIN (4) */
SL_ResetReason 	resetReason;		/* Reset reason */
/* USER CODE END */

#pragma CODE_STATE(_c_int00, 32)
#pragma INTERRUPT(_c_int00, RESET)
#pragma WEAK(_c_int00)

/* SourceId : STARTUP_SourceId_001 */
/* DesignId : STARTUP_DesignId_001 */
/* Requirements : HL_SR508 */
void _c_int00(void)
{
    
/* USER CODE BEGIN (5) */
	volatile boolean 			retVal;             /* For function return values */
	SL_STC_Config 				stcSelfTestConfig;  /* STC Configuration */

	SL_Init_R4Registers();
/* USER CODE END */

    /* Initialize Core Registers to avoid CCM Error */
    _coreInitRegisters_();

/* USER CODE BEGIN (6) */
/* USER CODE END */

    /* Initialize Stack Pointers */
    _coreInitStackPointer_();

/* USER CODE BEGIN (7) */
    resetReason = SL_Init_ResetReason();

#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Enable CPU Event Export */
    /* This allows the CPU to signal any single-bit or double-bit errors detected
     * by its ECC logic for accesses to program flash or data RAM.
     */
    _coreEnableEventBusExport_();
/* USER CODE BEGIN (9) */
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Enable response to ECC errors indicated by CPU for accesses to flash */
    flashWREG->FEDACCTRL1 = 0x000A060AU;

/* USER CODE BEGIN (10) */
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Enable CPU ECC checking for ATCM (flash accesses) */
    _coreEnableFlashEcc_();
	

/* USER CODE BEGIN (11) */
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */

        /* Workaround for Errata CORTEXR4 66 */
        _errata_CORTEXR4_66_();
    
        /* Workaround for Errata CORTEXR4 57 */ 
        _errata_CORTEXR4_57_();

    /* Reset handler: the following instructions read from the system exception status register
     * to identify the cause of the CPU reset.
     */

    /* check for power-on reset condition */
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    if ((SYS_EXCEPTION & POWERON_RESET) != 0U)
    {
/* USER CODE BEGIN (12) */
#else

    if(RESET_TYPE_POWERON == resetReason){
/* USER CODE END */
        
    	/* clear all reset status flags */
    	SYS_EXCEPTION = 0xFFFFU;



/* USER CODE BEGIN (13) */
        /* Workaround for Errata CORTEXR4 66 */
		_errata_CORTEXR4_66_();

		/* Workaround for Errata CORTEXR4 57 */
		_errata_CORTEXR4_57_();
/* USER CODE END */

/* USER CODE BEGIN (14) */
#endif
/* USER CODE END */
/* USER CODE BEGIN (15) */
#ifndef SAFETI_INTEGRATE
/* USER CODE END */
      /* continue with normal start-up sequence */
    }
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    else if ((SYS_EXCEPTION & OSC_FAILURE_RESET) != 0U)
    {
        /* Reset caused due to oscillator failure.
        Add user code here to handle oscillator failure */

/* USER CODE BEGIN (16) */
#else
    }
    else if(RESET_TYPE_OSC_FAILURE == resetReason){

    }
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */
    }
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    else if ((SYS_EXCEPTION & WATCHDOG_RESET) !=0U)
    {
        /* Reset caused due 
         *  1) windowed watchdog violation - Add user code here to handle watchdog violation.
         *  2) ICEPICK Reset - After loading code via CCS / System Reset through CCS
         */
        /* Check the WatchDog Status register */
        if(WATCHDOG_STATUS != 0U)
        {
            /* Add user code here to handle watchdog violation. */ 
/* USER CODE BEGIN (17) */
#else
    else if(RESET_TYPE_WATCHDOG == resetReason){

    	/* Reset caused due
		 *  1) windowed watchdog violation - Add user code here to handle watchdog violation.
		 *  2) ICEPICK Reset - After loading code via CCS / System Reset through CCS
		 */
		/* Check the WatchDog Status register */
		if(WATCHDOG_STATUS != 0U)
		{
			/* Add user code here to handle watchdog violation. */

/* USER CODE END */

            /* Clear the Watchdog reset flag in Exception Status register */ 
            SYS_EXCEPTION = WATCHDOG_RESET;
        
/* USER CODE BEGIN (18) */
#endif
/* USER CODE END */
        }
        else
        {
            /* Clear the ICEPICK reset flag in Exception Status register */ 
            SYS_EXCEPTION = ICEPICK_RESET;
/* USER CODE BEGIN (19) */
#ifndef SAFETI_INTEGRATE
/* USER CODE END */
        }
    }
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    else if ((SYS_EXCEPTION & CPU_RESET) !=0U)
    {
        /* Reset caused due to CPU reset.
        CPU reset can be caused by CPU self-test completion, or
        by toggling the "CPU RESET" bit of the CPU Reset Control Register. */

/* USER CODE BEGIN (20) */
#endif
    	}
    }
    else if((RESET_TYPE_CPU == resetReason) && (RESET_TYPE_DEBUG != resetReason)){

/* USER CODE END */

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
                stcSelfCheckFail();                        
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
                //cpuSelfTest(STC_INTERVAL, STC_MAX_TIMEOUT, TRUE);

                /* THIS WILL BE REPLACED BY HALCOGEN */
                stcSelfTestConfig.stcClockDiv = 0;
                stcSelfTestConfig.intervalCount = 1;
                stcSelfTestConfig.restartInterval0 = TRUE;
                stcSelfTestConfig.timeoutCounter = 0xFFFFFFFF;
                _SL_HoldNClear_nError();
                SL_SelfTest_STC(STC_RUN, TRUE, &stcSelfTestConfig);
            }
        }
        /* CPU reset caused by CPU self-test completion */
        else if ((stcREG->STCGSTAT & 0x1U) == 0x1U)        
        {
            /* Self-Test Fail flag is set */
            if ((stcREG->STCGSTAT & 0x2U) == 0x2U)        
            {
                /* Call CPU self-test failure handler */
                cpuSelfTestFail();                    
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
/* USER CODE BEGIN (21) */
#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    }
    /*SAFETYMCUSW 139 S MR:13.7 <APPROVED> "Hardware status bit read check" */
    else if ((SYS_EXCEPTION & SW_RESET) != 0U)
    {
        /* Reset caused due to software reset.
        Add user code to handle software reset. */

/* USER CODE BEGIN (22) */
#else
    }
    else if(RESET_TYPE_SWRST == resetReason){

/* USER CODE END */
    }
    else
    {
        /* Reset caused by nRST being driven low externally.
        Add user code to handle external reset. */

/* USER CODE BEGIN (23) */
/* USER CODE END */
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
/* USER CODE BEGIN (24) */
/* USER CODE END */
    /*SAFETYMCUSW 5 C MR:NA <APPROVED> "for(;;) can be removed by adding "# if 0" and "# endif" in the user codes above and below" */
    /*SAFETYMCUSW 26 S MR:NA <APPROVED> "for(;;) can be removed by adding "# if 0" and "# endif" in the user codes above and below" */
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "for(;;) can be removed by adding "# if 0" and "# endif" in the user codes above and below" */
        for(;;)
        { 
        }/* Wait */                 
/* USER CODE BEGIN (25) */
/* USER CODE END */
    }

/* USER CODE BEGIN (26) */
#endif
#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Initialize System - Clock, Flash settings with Efuse self check */
    systemInit();
    
    /* Workaround for Errata PBIST#4 */
    errata_PBIST_4();
	
    /* Run a diagnostic check on the memory self-test controller.
     * This function chooses a RAM test algorithm and runs it on an on-chip ROM.
     * The memory self-test is expected to fail. The function ensures that the PBIST controller
     * is capable of detecting and indicating a memory self-test failure.
     */
    pbistSelfCheck();	
	
	/* Run PBIST on STC ROM */
    pbistRun((uint32)STC_ROM_PBIST_RAM_GROUP,
             ((uint32)PBIST_TripleReadSlow | (uint32)PBIST_TripleReadFast));
    
    /* Wait for PBIST for STC ROM to be completed */
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while(pbistIsTestCompleted() != TRUE)
    { 
    }/* Wait */ 
    
    /* Check if PBIST on STC ROM passed the self-test */
    if( pbistIsTestPassed() != TRUE)
    {
        /* PBIST and STC ROM failed the self-test.
         * Need custom handler to check the memory failure
         * and to take the appropriate next step.
         */
         
        pbistFail();

    }   
	
    /* Disable PBIST clocks and disable memory self-test mode */
    pbistStop();

	/* Run PBIST on PBIST ROM */
    pbistRun((uint32)PBIST_ROM_PBIST_RAM_GROUP,
             ((uint32)PBIST_TripleReadSlow | (uint32)PBIST_TripleReadFast));
    
    /* Wait for PBIST for PBIST ROM to be completed */
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while(pbistIsTestCompleted() != TRUE)
    { 
    }/* Wait */ 
    
    /* Check if PBIST ROM passed the self-test */
    if( pbistIsTestPassed() != TRUE)
    {
        /* PBIST and STC ROM failed the self-test.
         * Need custom handler to check the memory failure
         * and to take the appropriate next step.
         */
         
        pbistFail();

    } 
	
    /* Disable PBIST clocks and disable memory self-test mode */
    pbistStop();	
/* USER CODE BEGIN (27) */
/* USER CODE END */

    /* Make sure that the CPU self-test controller can actually detect a fault inside CPU */
    stcSelfCheck();

/* USER CODE BEGIN (28) */
#else
    /* Initialize System - Clock, Flash settings with Efuse self check */
    systemInit();

#ifndef DEBUG_MODE
    if(RESET_TYPE_DEBUG != resetReason){
    	/* THIS WILL BE REPLACED BY HALCOGEN */
		stcSelfTestConfig.stcClockDiv = 0;
		stcSelfTestConfig.intervalCount = 1;
		stcSelfTestConfig.restartInterval0 = TRUE;
		stcSelfTestConfig.timeoutCounter = 0xFFFFFFFF;
		_SL_HoldNClear_nError();
		SL_SelfTest_STC(STC_COMPARE_SELFCHECK, TRUE, &stcSelfTestConfig);

    }
    else{
    	afterSTC();
    }
#endif
    afterSTC();
#endif
/* USER CODE END */
}

void afterSTC(void)
{
/* USER CODE BEGIN (29) */
#ifdef SAFETI_INTEGRATE
#ifndef DEBUG_MODE
	SL_CCMR4F_FailInfo			failInfoCCMR4F;		/* CCMR4 Self Test fail info */
#endif
	volatile boolean 			retVal;             /* For function return values */
	SL_PBIST_FailInfo           failInfoPBISTSRAM;  /* PBIST Failure information for TCM RAM */
	SL_SelfTest_Result          failInfoFlash;      /* Flash Self test failure information */
	SL_SelfTest_Result          failInfoTCMRAM;     /* TCM RAM Failure  information */
	SL_PBIST_FailInfo           failInfoPBISTOthers;/* PBIST Failure information for non-TCM memories */
	SL_PSCON_FailInfo           failInfoPSCON;      /* PSCON failure information */
	SL_EFuse_Config             stConfigEFuse;      /* EFuse self test configuration */
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Make sure that CCM-R4F is working as expected.
     * This function puts the CCM-R4F module through its self-test modes.
     * It ensures that the CCM-R4F is indeed capable of detecting a CPU mismatch,
     * and is also capable of indicating a mismatch error to the ESM.
     */
    ccmSelfCheck();

/* USER CODE BEGIN (30) */

/* USER CODE END */

/* USER CODE BEGIN (31) */
#endif
#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Disable RAM ECC before doing PBIST for Main RAM */
    _coreDisableRamEcc_();

    /* Run PBIST on CPU RAM.
     * The PBIST controller needs to be configured separately for single-port and dual-port SRAMs.
     * The CPU RAM is a single-port memory. The actual "RAM Group" for all on-chip SRAMs is defined in the
     * device datasheet.
     */
    pbistRun(0x00300020U, /* ESRAM Single Port PBIST */
             (uint32)PBIST_March13N_SP);

/* USER CODE BEGIN (32) */
/* USER CODE END */

    /* Wait for PBIST for CPU RAM to be completed */
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while(pbistIsTestCompleted() != TRUE)
    {
    }/* Wait */


/* USER CODE BEGIN (33) */
/* USER CODE END */

    /* Check if CPU RAM passed the self-test */
    if( pbistIsTestPassed() != TRUE)
    {
        /* CPU RAM failed the self-test.
         * Need custom handler to check the memory failure
         * and to take the appropriate next step.
         */
/* USER CODE BEGIN (34) */
/* USER CODE END */

        pbistFail();

/* USER CODE BEGIN (35) */
/* USER CODE END */
    }

/* USER CODE BEGIN (36) */
/* USER CODE END */

    /* Disable PBIST clocks and disable memory self-test mode */
    pbistStop();


/* USER CODE BEGIN (37) */
#else
    /* Execute PBIST tests on required peripheral SRAMs */
    _SL_HoldNClear_nError();
    retVal = SL_SelfTest_PBIST(PBIST_EXECUTE, PBIST_RAMGROUP_01_PBIST_ROM, PBISTALGO_TRIPLE_READ_FAST_READ);

    while(TRUE != SL_SelfTest_WaitCompletion_PBIST());

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

	/* Run PBIST on CPU RAM.
	 * The PBIST controller needs to be configured separately for single-port and dual-port SRAMs.
	 * The CPU RAM is a single-port memory. The actual "RAM Group" for all on-chip SRAMs is defined in the
	 * device datasheet.
	 */
	retVal = SL_SelfTest_PBIST(	PBIST_EXECUTE,                                          /* Execute PBIST test */
								(PBIST_RAMGROUP_06_ESRAM1 | PBIST_RAMGROUP_21_ESRAM5),   /* On all TCM RAMs */
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
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */


    /* Initialize CPU RAM.
     * This function uses the system module's hardware for auto-initialization of memories and their
     * associated protection schemes. The CPU RAM is initialized by setting bit 0 of the MSIENA register.
     * Hence the value 0x1 passed to the function.
     * This function will initialize the entire CPU RAM and the corresponding ECC locations.
     */
    memoryInit(0x1U);

/* USER CODE BEGIN (38) */
/* USER CODE END */

    /* Enable ECC checking for TCRAM accesses.
     * This function enables the CPU's ECC logic for accesses to B0TCM and B1TCM.
     */
    _coreEnableRamEcc_();

/* USER CODE BEGIN (39) */
/* USER CODE END */

    /* Start PBIST on all dual-port memories */
    /* NOTE : Please Refer DEVICE DATASHEET for the list of Supported Dual port Memories.
       PBIST test performed only on the user selected memories in HALCoGen's GUI SAFETY INIT tab.
     */
    pbistRun(  (uint32)0x00000000U    /* EMAC RAM */
             | (uint32)0x00000000U    /* USB RAM */
             | (uint32)0x00000800U    /* DMA RAM */
             | (uint32)0x00000200U    /* VIM RAM */
             | (uint32)0x00000040U    /* MIBSPI1 RAM */
             | (uint32)0x00000080U    /* MIBSPI3 RAM */
             | (uint32)0x00000100U    /* MIBSPI5 RAM */
             | (uint32)0x00000004U    /* CAN1 RAM */
             | (uint32)0x00000008U    /* CAN2 RAM */
             | (uint32)0x00000010U    /* CAN3 RAM */
             | (uint32)0x00000400U    /* ADC1 RAM */
             | (uint32)0x00020000U    /* ADC2 RAM */
             | (uint32)0x00001000U    /* HET1 RAM */
             | (uint32)0x00040000U    /* HET2 RAM */
             | (uint32)0x00002000U    /* HTU1 RAM */
             | (uint32)0x00080000U    /* HTU2 RAM */
             | (uint32)0x00000000U    /* RTP RAM */
             | (uint32)0x00000000U    /* FRAY RAM */
             ,(uint32) PBIST_March13N_DP);

/* USER CODE BEGIN (40) */
#else
    /* Initialize CPU RAM.
	 * This function uses the system module's hardware for auto-initialization of memories and their
	 * associated protection schemes. The CPU RAM is initialized by setting bit 0 of the MSIENA register.
	 * Hence the value 0x1 passed to the function.
	 * This function will initialize the entire CPU RAM and the corresponding ECC locations.
	 */
    SL_Init_Memory(RAMTYPE_RAM);
	SL_Init_ECCFlash(10, FLASHECC_DEFAULT); /* Enable Flash ECC */

	SL_Init_ECCTCMRAM(10, TRUE);            /* Enable TCM RAM ECC */

	/* Execute PBIST tests on required peripheral SRAMs */
	retVal = SL_SelfTest_PBIST( PBIST_EXECUTE,
								(PBIST_RAMGROUP_07_MIBSPI1  |
								PBIST_RAMGROUP_08_MIBSPI3   |
								PBIST_RAMGROUP_09_MIBSPI5   |
								PBIST_RAMGROUP_10_VIM       |
								PBIST_RAMGROUP_11_MIBADC1   |
								PBIST_RAMGROUP_18_MIBADC2),
								PBISTALGO_MARCH13N_RED_2PORT);
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Test the CPU ECC mechanism for RAM accesses.
     * The checkBxRAMECC functions cause deliberate single-bit and double-bit errors in TCRAM accesses
     * by corrupting 1 or 2 bits in the ECC. Reading from the TCRAM location with a 2-bit error
     * in the ECC causes a data abort exception. The data abort handler is written to look for
     * deliberately caused exception and to return the code execution to the instruction
     * following the one that caused the abort.
     */
    checkRAMECC();

/* USER CODE BEGIN (41) */
#else
    /* Run 1Bit ECC test on TCM RAM */
	retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_1BIT, TRUE, &failInfoTCMRAM);
	if(retVal == FALSE) while(1);

	/* Run 1Bit ECC error profiling test on TCM RAM */
   retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_PROFILING, TRUE, &failInfoTCMRAM);
   if(retVal == FALSE) while(1);

   retVal = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_2BIT, TRUE, &failInfoTCMRAM);
	if(retVal == FALSE) while(1);
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Test the CPU ECC mechanism for Flash accesses.
     * The checkFlashECC function uses the flash interface module's diagnostic mode 7
     * to create single-bit and double-bit errors in CPU accesses to the flash. A double-bit
     * error on reading from flash causes a data abort exception.
     * The data abort handler is written to look for deliberately caused exception and
     * to return the code execution to the instruction following the one that was aborted.
     *
     */
    checkFlashECC();
    flashWREG->FDIAGCTRL = 0x000A0007U;                    /* disable flash diagnostic mode */

/* USER CODE BEGIN (42) */
/* USER CODE END */

/* USER CODE BEGIN (43) */
/* USER CODE END */

    /* Wait for PBIST for CPU RAM to be completed */
    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while(pbistIsTestCompleted() != TRUE)
    {
    }/* Wait */


/* USER CODE BEGIN (44) */
/* USER CODE END */

    /* Check if CPU RAM passed the self-test */
    if( pbistIsTestPassed() != TRUE)
    {

/* USER CODE BEGIN (45) */
/* USER CODE END */

        /* CPU RAM failed the self-test.
         * Need custom handler to check the memory failure
         * and to take the appropriate next step.
         */
/* USER CODE BEGIN (46) */
/* USER CODE END */

        pbistFail();

/* USER CODE BEGIN (47) */
/* USER CODE END */
    }

/* USER CODE BEGIN (48) */
/* USER CODE END */

    /* Disable PBIST clocks and disable memory self-test mode */
    pbistStop();

/* USER CODE BEGIN (55) */
#else
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

	while (TRUE != SL_SelfTest_WaitCompletion_PBIST());
	SL_SelfTest_Status_PBIST(&failInfoPBISTOthers);

	/* Check the PBIST status and if there is a failure then wait*/
	if (failInfoPBISTOthers.stResult != ST_PASS)
	{
		while(1);
	}

	/* Disable PBIST clocks and disable memory self-test mode */
	SL_SelfTest_PBIST_StopExec();
#endif

#ifndef SAFETI_INTEGRATE
/* USER CODE END */

    /* Release the MibSPI1 modules from local reset.
     * This will cause the MibSPI1 RAMs to get initialized along with the parity memory.
     */
     mibspiREG1->GCR0 = 0x1U;

    /* Release the MibSPI3 modules from local reset.
     * This will cause the MibSPI3 RAMs to get initialized along with the parity memory.
     */
    mibspiREG3->GCR0 = 0x1U;

    /* Release the MibSPI5 modules from local reset.
     * This will cause the MibSPI5 RAMs to get initialized along with the parity memory.
     */
    mibspiREG5->GCR0 = 0x1U;

/* USER CODE BEGIN (56) */
/* USER CODE END */

    /* Enable parity on selected RAMs */
    enableParity();

    /* Initialize all on-chip SRAMs except for MibSPIx RAMs
     * The MibSPIx modules have their own auto-initialization mechanism which is triggered
     * as soon as the modules are brought out of local reset.
     */
    /* The system module auto-init will hang on the MibSPI RAM if the module is still in local reset.
     */
    /* NOTE : Please Refer DEVICE DATASHEET for the list of Supported Memories and their channel numbers.
              Memory Initialization is perfomed only on the user selected memories in HALCoGen's GUI SAFETY INIT tab.
     */
    memoryInit( (uint32)((uint32)1U << 1U)    /* DMA RAM */
              | (uint32)((uint32)1U << 2U)    /* VIM RAM */
              | (uint32)((uint32)1U << 5U)    /* CAN1 RAM */
              | (uint32)((uint32)1U << 6U)    /* CAN2 RAM */
              | (uint32)((uint32)1U << 10U)   /* CAN3 RAM */
              | (uint32)((uint32)1U << 8U)    /* ADC1 RAM */
              | (uint32)((uint32)1U << 14U)   /* ADC2 RAM */
              | (uint32)((uint32)1U << 3U)    /* HET1 RAM */
              | (uint32)((uint32)1U << 4U)    /* HTU1 RAM */
              | (uint32)((uint32)1U << 15U)   /* HET2 RAM */
              | (uint32)((uint32)1U << 16U)   /* HTU2 RAM */
              );

    /* Disable parity */
    disableParity();

    /* Test the parity protection mechanism for peripheral RAMs
       NOTE : Please Refer DEVICE DATASHEET for the list of Supported Memories with parity.
                 Parity Self check is perfomed only on the user selected memories in HALCoGen's GUI SAFETY INIT tab.
    */

/* USER CODE BEGIN (57) */
/* USER CODE END */

    het1ParityCheck();

/* USER CODE BEGIN (58) */
/* USER CODE END */

    htu1ParityCheck();

/* USER CODE BEGIN (59) */
/* USER CODE END */

    het2ParityCheck();

/* USER CODE BEGIN (60) */
/* USER CODE END */

    htu2ParityCheck();

/* USER CODE BEGIN (61) */
/* USER CODE END */

    adc1ParityCheck();

/* USER CODE BEGIN (62) */
/* USER CODE END */

    adc2ParityCheck();

/* USER CODE BEGIN (63) */
/* USER CODE END */

    can1ParityCheck();

/* USER CODE BEGIN (64) */
/* USER CODE END */

    can2ParityCheck();

/* USER CODE BEGIN (65) */
/* USER CODE END */

    can3ParityCheck();

/* USER CODE BEGIN (66) */
/* USER CODE END */

    vimParityCheck();

/* USER CODE BEGIN (67) */
/* USER CODE END */

    dmaParityCheck();


/* USER CODE BEGIN (68) */
/* USER CODE END */

/*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while ((mibspiREG1->FLG & 0x01000000U) == 0x01000000U)
    {
    }/* Wait */
    /* wait for MibSPI1 RAM to complete initialization */
/*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while ((mibspiREG3->FLG & 0x01000000U) == 0x01000000U)
    {
    }/* Wait */
    /* wait for MibSPI3 RAM to complete initialization */
/*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
    while ((mibspiREG5->FLG & 0x01000000U) == 0x01000000U)
    {
    }/* Wait */
    /* wait for MibSPI5 RAM to complete initialization */

/* USER CODE BEGIN (69) */
/* USER CODE END */

    mibspi1ParityCheck();

/* USER CODE BEGIN (70) */
/* USER CODE END */

    mibspi3ParityCheck();

/* USER CODE BEGIN (71) */
/* USER CODE END */

    mibspi5ParityCheck();


/* USER CODE BEGIN (72) */
#endif
/* USER CODE END */

    /* Enable IRQ offset via Vic controller */
    _coreEnableIrqVicOffset_();


/* USER CODE BEGIN (73) */
#ifdef SAFETI_INTEGRATE
    SL_ESM_Init(esmCallBackFunction);

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

#ifndef DEBUG_MODE

	// Reset variable gets replaced when RAM self tests are in progress
	resetReason = SL_Init_ResetReason();

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
	retVal = SL_SelfTest_SRAM(SRAM_RADECODE_DIAGNOSTICS, TRUE, &failInfoTCMRAM);;
	if(retVal == FALSE) while(1);

	retVal = SL_SelfTest_SRAM(SRAM_PAR_ADDR_CTRL_SELF_TEST, TRUE, &failInfoTCMRAM);;
	if(retVal == FALSE) while(1);
#endif
/* USER CODE END */

    /* Initialize VIM table */
    vimInit();

/* USER CODE BEGIN (74) */
/* USER CODE END */

    /* Configure system response to error conditions signaled to the ESM group1 */
    /* This function can be configured from the ESM tab of HALCoGen */
    esmInit();
    /* initialize copy table */
    __TI_auto_init();
/* USER CODE BEGIN (75) */
/* USER CODE END */

    /* call the application */
/*SAFETYMCUSW 296 S MR:8.6 <APPROVED> "Startup code(library functions at block scope)" */
/*SAFETYMCUSW 326 S MR:8.2 <APPROVED> "Startup code(Declaration for main in library)" */
/*SAFETYMCUSW 60 D MR:8.8 <APPROVED> "Startup code(Declaration for main in library;Only doing an extern for the same)" */
    main();

/* USER CODE BEGIN (76) */
/* USER CODE END */
/*SAFETYMCUSW 122 S MR:20.11 <APPROVED> "Startup code(exit and abort need to be present)" */
    exit(0);

/* USER CODE BEGIN (77) */
/* USER CODE END */
}

/* USER CODE BEGIN (78) */
void esmCallBackFunction (uint32 grp_channel, uint32 param1, uint32 param2, uint32 param3)
{

}
/* USER CODE END */


