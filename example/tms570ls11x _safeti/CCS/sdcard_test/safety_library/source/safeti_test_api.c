/*
 * safeti_test_api.c
 *
 *  Created on: Jun 22, 2016
 *      Author: PradeepaS
 */

#include "sl_api.h"
#include "safeti_test_api.h"
#include "uartstdio.h"

enum
{
	SAFETI_FAIL = 0,
	SAFETI_PASS = ~SAFETI_FAIL
};


char safeti_SRAM_ECC_ERROR_FORCING_1BIT()
{
	boolean self_test_status = false;
	SL_SelfTest_Result sram_stResult;
	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_1BIT, TRUE, &sram_stResult);

	if(self_test_status & (ST_PASS == sram_stResult)){
		UARTprintf("[PASS] SRAM_ECC_ERROR_FORCING_1BIT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_ERROR_FORCING_1BIT\r\n");
		sram_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}


char safeti_SRAM_ECC_ERROR_FORCING_2BIT()
{
	boolean self_test_status = false;
	SL_SelfTest_Result sram_stResult;

	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_ERROR_FORCING_2BIT, TRUE, &sram_stResult);

	if(self_test_status & (ST_PASS == sram_stResult)){
		UARTprintf("[PASS] SRAM_ECC_ERROR_FORCING_2BIT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_ERROR_FORCING_2BIT\r\n");
		sram_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}


char safeti_FEE_ECC_DATA_CORR_MODE()
{
	boolean self_test_status = false;
	SL_SelfTest_Result fee_stResult;
	self_test_status = SL_SelfTest_FEE(FEE_ECC_DATA_CORR_MODE, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_DATA_CORR_MODE\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_DATA_CORR_MODE\r\n");
		fee_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}


char safeti_FEE_ECC_TEST_MODE_1BIT()
{
	boolean self_test_status = false;
	SL_SelfTest_Result fee_stResult;
	self_test_status = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_1BIT, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_TEST_MODE_1BIT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_TEST_MODE_1BIT\r\n");
		fee_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}


char safeti_FEE_ECC_TEST_MODE_2BIT()
{
	boolean self_test_status = false;
	SL_SelfTest_Result fee_stResult;
	self_test_status = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_2BIT, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_TEST_MODE_2BIT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_TEST_MODE_2BIT\r\n");
		fee_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}

char safeti_FEE_ECC_SYN_REPORT_MODE()
{
	boolean self_test_status = false;
	SL_SelfTest_Result fee_stResult;
	self_test_status = SL_SelfTest_FEE(FEE_ECC_SYN_REPORT_MODE, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_SYN_REPORT_MODE\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_SYN_REPORT_MODE\r\n");
		fee_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}

char safeti_FEE_ECC_MALFUNCTION_MODE1()
{
	boolean self_test_status = false;
	SL_SelfTest_Result fee_stResult;
	self_test_status = SL_SelfTest_FEE(FEE_ECC_MALFUNCTION_MODE1, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_MALFUNCTION_MODE1\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_MALFUNCTION_MODE1\r\n");
		fee_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}

char safeti_FEE_ECC_MALFUNCTION_MODE2()
{
	boolean self_test_status = false;
	SL_SelfTest_Result fee_stResult;
	self_test_status = SL_SelfTest_FEE(FEE_ECC_MALFUNCTION_MODE2, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_MALFUNCTION_MODE2\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_MALFUNCTION_MODE2\r\n");
		fee_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}

char safeti_FLASH_ECC_ADDR_TAG_REG_MODE()
{
	boolean self_test_status = false;
	SL_SelfTest_Result flash_stResult;
	self_test_status = SL_SelfTest_Flash(FLASH_ECC_ADDR_TAG_REG_MODE, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ECC_ADDR_TAG_REG_MODE\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FLASH_ECC_ADDR_TAG_REG_MODE\r\n");
		flash_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}


char safeti_FLASH_ECC_TEST_MODE_1BIT()
{
	boolean self_test_status = false;
	SL_SelfTest_Result flash_stResult;
	self_test_status = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_1BIT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ECC_TEST_MODE_1BIT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FLASH_ECC_TEST_MODE_1BIT\r\n");
		flash_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}


char safeti_FLASH_ECC_TEST_MODE_2BIT()
{
	boolean self_test_status = false;
	SL_SelfTest_Result flash_stResult;
	self_test_status = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_2BIT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ECC_TEST_MODE_2BIT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FLASH_ECC_TEST_MODE_2BIT\r\n");
		flash_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}


char safeti_PSCON_SELF_TEST()
{
	boolean self_test_status = false;
	SL_PSCON_FailInfo psconFailInfo;

	self_test_status = SL_SelfTest_PSCON(PSCON_SELF_TEST, TRUE, &psconFailInfo);

	if(self_test_status & (ST_PASS == psconFailInfo.stResult)){
		UARTprintf("[PASS] PSCON_SELF_TEST\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] PSCON_SELF_TEST\r\n");
		return SAFETI_FAIL;
	}

}


char safeti_PSCON_ERROR_FORCING()
{
	boolean self_test_status = false;
	SL_PSCON_FailInfo psconFailInfo;
    self_test_status = SL_SelfTest_PSCON(PSCON_ERROR_FORCING, TRUE, &psconFailInfo);

	if(self_test_status & (ST_PASS == psconFailInfo.stResult)){
		UARTprintf("[PASS] PSCON_ERROR_FORCING\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] PSCON_ERROR_FORCING\r\n");
		return SAFETI_FAIL;
	}
}


char safeti_PSCON_SELF_TEST_ERROR_FORCING()
{
	boolean self_test_status = false;
	SL_PSCON_FailInfo psconFailInfo;
	self_test_status = SL_SelfTest_PSCON(PSCON_SELF_TEST_ERROR_FORCING, TRUE, &psconFailInfo);

	if(self_test_status & (ST_PASS == psconFailInfo.stResult)){
		UARTprintf("[PASS] PSCON_SELF_TEST_ERROR_FORCING\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] PSCON_SELF_TEST_ERROR_FORCING\r\n");
		return SAFETI_FAIL;
	}
}

char safeti_PSCON_PMA_TEST()
{
	/* Must fail, since PMA tests cannot be run in privilege modes */
	boolean self_test_status = false;
	SL_PSCON_FailInfo psconFailInfo;
	self_test_status = SL_SelfTest_PSCON(PSCON_PMA_TEST, TRUE, &psconFailInfo);

	if(!self_test_status & (ST_PASS == psconFailInfo.stResult)){
		UARTprintf("[PASS] PSCON_PMA_TEST\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] PSCON_PMA_TEST\r\n");
		return SAFETI_FAIL;
	}
}

char safeti_EFUSE_SELF_TEST_STUCK_AT_ZERO()
{
	SL_EFuse_Config stConfigEFuse;
	boolean self_test_status = false;

	stConfigEFuse.numPatterns      = 600u;
	stConfigEFuse.seedSignature    = 0x5362F97Fu;
	stConfigEFuse.failInfo.stResult= ST_FAIL;
	stConfigEFuse.failInfo.failInfo= EFUSE_ERROR_NONE;

	self_test_status = SL_SelfTest_EFuse(EFUSE_SELF_TEST_STUCK_AT_ZERO, TRUE, &stConfigEFuse);

	if(!self_test_status & (ST_PASS == stConfigEFuse.failInfo.stResult)){
		UARTprintf("[PASS] EFUSE_SELF_TEST_STUCK_AT_ZERO\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] EFUSE_SELF_TEST_STUCK_AT_ZERO\r\n");
		return SAFETI_FAIL;
	}
}

char safeti_EFUSE_SELF_TEST_ECC()
{
	SL_EFuse_Config stConfigEFuse;
	boolean self_test_status = false;

	stConfigEFuse.numPatterns      = 600u;
	stConfigEFuse.seedSignature    = 0x5362F97Fu;
	stConfigEFuse.failInfo.stResult= ST_FAIL;
	stConfigEFuse.failInfo.failInfo= EFUSE_ERROR_NONE;

	self_test_status = SL_SelfTest_EFuse(EFUSE_SELF_TEST_ECC, TRUE, &stConfigEFuse);

	while (TRUE != SL_SelfTest_Status_EFuse(&stConfigEFuse.failInfo));

	if(!self_test_status & (ST_PASS == stConfigEFuse.failInfo.stResult)){
		UARTprintf("[PASS] EFUSE_SELF_TEST_ECC\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] EFUSE_SELF_TEST_ECC\r\n");
		return SAFETI_FAIL;
	}
}

char safeti_SRAM_RADECODE_DIAGNOSTICS()
{
	boolean self_test_status = false;
	SL_SelfTest_Result sram_stResult;
	self_test_status = SL_SelfTest_SRAM(SRAM_RADECODE_DIAGNOSTICS, TRUE, &sram_stResult);

	if(self_test_status & (ST_PASS == sram_stResult)){
		UARTprintf("[PASS] SRAM_RADECODE_DIAGNOSTICS\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] SRAM_RADECODE_DIAGNOSTICS\r\n");
		sram_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}

char safeti_SRAM_PAR_ADDR_CTRL_SELF_TEST()
{
	boolean self_test_status = false;
	SL_SelfTest_Result sram_stResult;
	self_test_status = SL_SelfTest_SRAM(SRAM_PAR_ADDR_CTRL_SELF_TEST, TRUE, &sram_stResult);

	if(self_test_status & (ST_PASS == sram_stResult)){
		UARTprintf("[PASS] SRAM_PAR_ADDR_CTRL_SELF_TEST\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] SRAM_PAR_ADDR_CTRL_SELF_TEST\r\n");
		sram_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}



/* --------------------------------------------- */
char safeti_SRAM_ECC_ERROR_PROFILING()
{
	boolean self_test_status = false;
	SL_SelfTest_Result sram_stResult;
	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_ERROR_PROFILING, TRUE, &sram_stResult);

	if(self_test_status & (ST_PASS == sram_stResult)){
		UARTprintf("[PASS] SRAM_ECC_ERROR_PROFILING\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_ERROR_PROFILING\r\n");
		sram_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
}

char safeti_FLASH_ADDRESS_ECC_FAULT_INJECT()
{
	/* Only enable to inject errors to the respective module. Not a test. */

#if 0
	boolean self_test_status = false;
	SL_SelfTest_Result flash_stResult;
	self_test_status = SL_SelfTest_Flash(FLASH_ADDRESS_ECC_FAULT_INJECT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ADDRESS_ECC_FAULT_INJECT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FLASH_ADDRESS_ECC_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
#else
	return SAFETI_PASS;
#endif
}


char safeti_FLASH_ADDRESS_PARITY_SELF_TEST()
{
	boolean self_test_status = false;
	SL_SelfTest_Result flash_stResult;
	self_test_status = SL_SelfTest_Flash(FLASH_ADDRESS_PARITY_SELF_TEST, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ADDRESS_PARITY_SELF_TEST\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FLASH_ADDRESS_PARITY_SELF_TEST\r\n");
		flash_stResult = ST_PASS;
		return SAFETI_FAIL;

	}
}


char safeti_FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT()
{
	/* Only enable to inject errors to the respective module. Not a test. */
#if 0
	boolean self_test_status = false;
	SL_SelfTest_Result flash_stResult;
	self_test_status = SL_SelfTest_Flash(FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT\r\n");
		flash_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
#else
	return SAFETI_PASS;
#endif
}


char safeti_SRAM_ECC_1BIT_FAULT_INJECTION()
{
	/* Only enable to inject errors to the respective module. Not a test. */

#if 0
	boolean self_test_status = false;
	SL_SelfTest_Result sram_stResult;
	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_1BIT_FAULT_INJECTION, TRUE, &sram_stResult);

	if(self_test_status & (ST_PASS == sram_stResult)){
		UARTprintf("[PASS] SRAM_ECC_1BIT_FAULT_INJECTION\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_1BIT_FAULT_INJECTION\r\n");
		sram_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
#else
	return SAFETI_PASS;
#endif
}


char safeti_SRAM_ECC_2BIT_FAULT_INJECT()
{
	/* Only enable to inject errors to the respective module. Not a test. */

#if 0
	boolean self_test_status = false;
	SL_SelfTest_Result sram_stResult;
	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_2BIT_FAULT_INJECT, TRUE, &sram_stResult);

	if(self_test_status & (ST_PASS == sram_stResult)){
		UARTprintf("[PASS] SRAM_ECC_2BIT_FAULT_INJECT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_2BIT_FAULT_INJECT\r\n");
		sram_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
#else
	return SAFETI_PASS;
#endif
}

char safeti_SRAM_ECC_ERROR_PROFILING_FAULT_INJECT()
{
	/* Only enable to inject errors to the respective module. Not a test. */

#if 0
	boolean self_test_status = false;
	SL_SelfTest_Result sram_stResult;
	self_test_status = SL_SelfTest_SRAM(SRAM_ECC_ERROR_PROFILING_FAULT_INJECT, TRUE, &sram_stResult);

	if(self_test_status & (ST_PASS == sram_stResult)){
		UARTprintf("[PASS] SRAM_ECC_ERROR_PROFILING_FAULT_INJECT\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] SRAM_ECC_ERROR_PROFILING_FAULT_INJECT\r\n");
		sram_stResult = ST_PASS;
		return SAFETI_FAIL;
	}
#else
	return SAFETI_PASS;
#endif
}

char safeti_CCMR4F_SELF_TEST()
{
	boolean self_test_status = false;
	SL_CCMR4F_FailInfo failInfoCCMR4F;

	self_test_status = SL_SelfTest_CCMR4F(CCMR4F_SELF_TEST, TRUE, &failInfoCCMR4F);

	if(self_test_status & (ST_PASS == failInfoCCMR4F.stResult)){
		UARTprintf("[PASS] CCMR4F_SELF_TEST\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] CCMR4F_SELF_TEST\r\n");
		return SAFETI_FAIL;
	}
}

char safeti_CCMR4F_ERROR_FORCING_TEST()
{
	boolean self_test_status = false;
	SL_CCMR4F_FailInfo failInfoCCMR4F;

	self_test_status = SL_SelfTest_CCMR4F(CCMR4F_ERROR_FORCING_TEST, TRUE, &failInfoCCMR4F);

	if(self_test_status & (ST_PASS == failInfoCCMR4F.stResult)){
		UARTprintf("[PASS] CCMR4F_ERROR_FORCING_TEST\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] CCMR4F_ERROR_FORCING_TEST\r\n");
		return SAFETI_FAIL;
	}
}

char safeti_CCMR4F_SELF_TEST_ERROR_FORCING()
{
	boolean self_test_status = false;
	SL_CCMR4F_FailInfo failInfoCCMR4F;
	self_test_status = SL_SelfTest_CCMR4F(CCMR4F_SELF_TEST_ERROR_FORCING, TRUE, &failInfoCCMR4F);

	if(self_test_status & (ST_PASS == failInfoCCMR4F.stResult)){
		UARTprintf("[PASS] CCMR4F_SELF_TEST_ERROR_FORCING\r\n");
		return SAFETI_PASS;
	}
	else{
		UARTprintf("[FAIL] CCMR4F_SELF_TEST_ERROR_FORCING\r\n");
		return SAFETI_FAIL;
	}
}


#if 0
	self_test_status = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_TEST_MODE_2BIT_FAULT_INJECT\r\n");
		fee_stResult = ST_PASS;
	}
#endif

#if 0
	self_test_status = SL_SelfTest_FEE(FEE_ECC_ADDR_TAG_REG_MODE, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_ADDR_TAG_REG_MODE\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_ADDR_TAG_REG_MODE\r\n");
		fee_stResult = ST_PASS;
	}


	self_test_status = SL_SelfTest_FEE(FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_TEST_MODE_1BIT_FAULT_INJECT\r\n");
		fee_stResult = ST_PASS;
	}
#endif

#if 0
	self_test_status = SL_SelfTest_FEE(FEE_ECC_DATA_CORR_MODE, TRUE, &fee_stResult);

	if(self_test_status & (ST_PASS == fee_stResult)){
		UARTprintf("[PASS] FEE_ECC_DATA_CORR_MODE\r\n");
	}
	else{
		UARTprintf("[FAIL] FEE_ECC_DATA_CORR_MODE\r\n");
		fee_stResult = ST_PASS;
	}
#endif

#if 0
	self_test_status = SL_SelfTest_Flash(FLASH_ADDRESS_ECC_SELF_TEST, TRUE, &flash_stResult);

	if(self_test_status & (ST_PASS == flash_stResult)){
		UARTprintf("[PASS] FLASH_ADDRESS_ECC_SELF_TEST\r\n");
	}
	else{
		UARTprintf("[FAIL] FLASH_ADDRESS_ECC_SELF_TEST\r\n");
		flash_stResult = ST_PASS;
	}
#endif
