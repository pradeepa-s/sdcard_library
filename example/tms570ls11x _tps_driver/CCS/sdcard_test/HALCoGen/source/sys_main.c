/** @file sys_main.c 
*   @brief Application main file
*   @date 02-Mar-2016
*   @version 04.05.02
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com 
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

/* USER CODE BEGIN (1) */

#include "sci.h"
#include "mibspi.h"
#include "rti.h"
#include "uartstdio.h"
#include "mmc-test.h"
#include "sl_api.h"
#include "spi.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

extern int SD_Test(void);

/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */

	SL_PSCON_FailInfo psconFailInfo;
	SL_CCMR4F_FailInfo failInfoCCMR4F;
	SL_SelfTest_Result flash_stResult;
	boolean self_test_status = false;

#if 1

	spiInit();



    /** - Initialize LIN/SCI2 Routines to receive Command and transmit data */
    sciInit();

    UARTprintf("--> ITS SDCARD H/W Test V3.0<--\r\n ");
    UARTprintf("Type \'help\' for help.\r\n");
    UARTprintf("TI project modified for ITS. (Pradeepa) \r\n");

    /* Initialize RTI driver */
    rtiInit();
    /* Enable RTI Compare 3 interrupt notification */
    rtiEnableNotification(rtiNOTIFICATION_COMPARE3);
    /* Enable IRQ - Clear I flag in CPS register */
    _enable_IRQ();
    /* Start RTI Counter Block 1 */
    rtiStartCounter(rtiCOUNTER_BLOCK1);

    mmcSelectSpi(mibspiPORT5, mibspiREG5);  // SD card is on the SPI5

    self_test_status = SL_SelfTest_PSCON(PSCON_SELF_TEST, TRUE, &psconFailInfo);
   // self_test_status = SL_SelfTest_PBIST_StopExec();

    if(self_test_status & (ST_PASS == psconFailInfo.stResult)){
    	UARTprintf("[PASS] PSCON_SELF_TEST\r\n");
    }
    else{
    	UARTprintf("[FAIL] PSCON_SELF_TEST\r\n");
    }

    self_test_status = SL_SelfTest_PSCON(PSCON_ERROR_FORCING, TRUE, &psconFailInfo);
	//self_test_status = SL_SelfTest_PBIST_StopExec();

	if(self_test_status & (ST_PASS == psconFailInfo.stResult)){
		UARTprintf("[PASS] PSCON_ERROR_FORCING\r\n");
	}
	else{
		UARTprintf("[FAIL] PSCON_ERROR_FORCING\r\n");
	}

	self_test_status = SL_SelfTest_PSCON(PSCON_SELF_TEST_ERROR_FORCING, TRUE, &psconFailInfo);
	//self_test_status = SL_SelfTest_PBIST_StopExec();

	if(self_test_status & (ST_PASS == psconFailInfo.stResult)){
		UARTprintf("[PASS] PSCON_SELF_TEST_ERROR_FORCING\r\n");
	}
	else{
		UARTprintf("[FAIL] PSCON_SELF_TEST_ERROR_FORCING\r\n");
	}

	self_test_status = SL_SelfTest_PSCON(PSCON_PMA_TEST, TRUE, &psconFailInfo);
	//self_test_status = SL_SelfTest_PBIST_StopExec();

	if(!self_test_status & (ST_PASS == psconFailInfo.stResult)){
		UARTprintf("[PASS] PSCON_PMA_TEST\r\n");
	}
	else{
		UARTprintf("[FAIL] PSCON_PMA_TEST\r\n");
	}

#ifndef DEBUG_MODE
	self_test_status = SL_SelfTest_CCMR4F(CCMR4F_SELF_TEST, TRUE, &failInfoCCMR4F);

	if(self_test_status & (ST_PASS == failInfoCCMR4F.stResult)){
		UARTprintf("[PASS] CCMR4F_SELF_TEST\r\n");
	}
	else{
		UARTprintf("[FAIL] CCMR4F_SELF_TEST\r\n");
	}

	self_test_status = SL_SelfTest_CCMR4F(CCMR4F_ERROR_FORCING_TEST, TRUE, &failInfoCCMR4F);

	if(self_test_status & (ST_PASS == failInfoCCMR4F.stResult)){
		UARTprintf("[PASS] CCMR4F_ERROR_FORCING_TEST\r\n");
	}
	else{
		UARTprintf("[FAIL] CCMR4F_ERROR_FORCING_TEST\r\n");
	}

	self_test_status = SL_SelfTest_CCMR4F(CCMR4F_SELF_TEST_ERROR_FORCING, TRUE, &failInfoCCMR4F);

	if(self_test_status & (ST_PASS == failInfoCCMR4F.stResult)){
		UARTprintf("[PASS] CCMR4F_SELF_TEST_ERROR_FORCING\r\n");
	}
	else{
		UARTprintf("[FAIL] CCMR4F_SELF_TEST_ERROR_FORCING\r\n");
	}
#endif


	self_test_status = SL_SelfTest_Flash(FLASH_ECC_ADDR_TAG_REG_MODE, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ECC_ADDR_TAG_REG_MODE\r\n");
	}
	else{
		UARTprintf("[FAIL] FLASH_ECC_ADDR_TAG_REG_MODE\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_Flash(FLASH_ADDRESS_ECC_SELF_TEST, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ADDRESS_ECC_SELF_TEST\r\n");
	}
	else{
		UARTprintf("[FAIL] FLASH_ADDRESS_ECC_SELF_TEST\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_Flash(FLASH_ADDRESS_ECC_FAULT_INJECT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ADDRESS_ECC_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] FLASH_ADDRESS_ECC_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_Flash(FLASH_ADDRESS_PARITY_SELF_TEST, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ADDRESS_PARITY_SELF_TEST\r\n");
	}
	else{
		UARTprintf("[FAIL] FLASH_ADDRESS_PARITY_SELF_TEST\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_1BIT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ECC_TEST_MODE_1BIT\r\n");
	}
	else{
		UARTprintf("[FAIL] FLASH_ECC_TEST_MODE_1BIT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_2BIT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ECC_TEST_MODE_2BIT\r\n");
	}
	else{
		UARTprintf("[FAIL] FLASH_ECC_TEST_MODE_2BIT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_FEE(FEE_ECC_DATA_CORR_MODE, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FEE_ECC_DATA_CORR_MODE\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_DATA_CORR_MODE\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_FEE(FEE_ECC_MALFUNCTION_MODE1, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FEE_ECC_MALFUNCTION_MODE1\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_MALFUNCTION_MODE1\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_FEE(FEE_ECC_MALFUNCTION_MODE2, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FEE_ECC_MALFUNCTION_MODE2\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_MALFUNCTION_MODE2\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_FEE(FEE_ECC_ADDR_TAG_REG_MODE, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FEE_ECC_ADDR_TAG_REG_MODE\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_ADDR_TAG_REG_MODE\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_1BIT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FEE_ECC_TEST_MODE_1BIT\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_TEST_MODE_1BIT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_2BIT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FEE_ECC_TEST_MODE_2BIT\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_TEST_MODE_2BIT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_1BIT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_ECC_ERROR_FORCING_1BIT\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_ERROR_FORCING_1BIT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_1BIT_FAULT_INJECTION, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_ECC_1BIT_FAULT_INJECTION\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_1BIT_FAULT_INJECTION\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_2BIT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_ECC_ERROR_FORCING_2BIT\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_ERROR_FORCING_2BIT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_2BIT_FAULT_INJECT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_ECC_2BIT_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_2BIT_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_SRAM(SRAM_PAR_ADDR_CTRL_SELF_TEST, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_PAR_ADDR_CTRL_SELF_TEST\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_PAR_ADDR_CTRL_SELF_TEST\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_ERROR_PROFILING, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_ECC_ERROR_PROFILING\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_ERROR_PROFILING\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_ERROR_PROFILING_FAULT_INJECT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_ECC_ERROR_PROFILING_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_ERROR_PROFILING_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
	}

	self_test_status = SL_SelfTest_SRAM(SRAM_RADECODE_DIAGNOSTICS, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_RADECODE_DIAGNOSTICS\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_RADECODE_DIAGNOSTICS\r\n");
		flash_stResult = ST_PASS;
	}

/*	self_test_status = SL_SelfTestL2L3Interconnect(L2INTERCONNECT_FAULT_INJECT);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] L2INTERCONNECT_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] L2INTERCONNECT_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
	}*/

/*	self_test_status = SL_SelfTestL2L3Interconnect(L3INTERCONNECT_FAULT_INJECT);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] L3INTERCONNECT_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] L3INTERCONNECT_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
	}*/

	/*self_test_status = SL_SelfTest_ADC(SRAM_RADECODE_DIAGNOSTICS, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] SRAM_RADECODE_DIAGNOSTICS\r\n");
	}
	else{
		UARTprintf("[FAIL] SRAM_RADECODE_DIAGNOSTICS\r\n");
		flash_stResult = ST_PASS;
	}
*/

    SD_Test();

#endif

#if 0
uint16_t tx_buf = 5;
uint16_t rx_buf;



mibspiInit();
//while(!(mibspiIsTransferComplete(mibspiREG5, 0)));

mibspiPORT5-> DCLR = 0x00;
//mibspiEnableLoopback(mibspiREG5, Digital_Lbk);
mibspiSetData(mibspiREG5, 0, &tx_buf);

mibspiTransfer(mibspiREG5, 0);

while(!(mibspiIsTransferComplete(mibspiREG5, 0)));

mibspiGetData(mibspiREG5,0,&rx_buf);

mibspiPORT5-> DCLR = 0x01;
//mibspiEnableLoopback(mibspiREG5, Digital_Lbk);
mibspiSetData(mibspiREG5, 0, &tx_buf);

mibspiTransfer(mibspiREG5, 0);

while(!(mibspiIsTransferComplete(mibspiREG5, 0)));

mibspiGetData(mibspiREG5,0,&rx_buf);
#endif
    while(1);

/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
