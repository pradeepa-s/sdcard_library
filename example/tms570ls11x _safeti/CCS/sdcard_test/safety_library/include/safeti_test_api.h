/*
 * safeti_test_api.h
 *
 *  Created on: Jun 22, 2016
 *      Author: PradeepaS
 */

#ifndef SAFETI_TEST_API_H_
#define SAFETI_TEST_API_H_

char safeti_SRAM_ECC_ERROR_FORCING_1BIT();
char safeti_SRAM_ECC_ERROR_FORCING_2BIT();
char safeti_FEE_ECC_DATA_CORR_MODE();
char safeti_FEE_ECC_TEST_MODE_1BIT();
char safeti_FEE_ECC_TEST_MODE_2BIT();
char safeti_FEE_ECC_SYN_REPORT_MODE();
char safeti_FEE_ECC_MALFUNCTION_MODE1();
char safeti_FEE_ECC_MALFUNCTION_MODE2();
char safeti_FLASH_ECC_ADDR_TAG_REG_MODE();
char safeti_FLASH_ECC_TEST_MODE_1BIT();
char safeti_FLASH_ECC_TEST_MODE_2BIT();
char safeti_PSCON_SELF_TEST();
char safeti_PSCON_ERROR_FORCING();
char safeti_PSCON_SELF_TEST_ERROR_FORCING();
char safeti_PSCON_PMA_TEST();
char safeti_EFUSE_SELF_TEST_STUCK_AT_ZERO();
char safeti_EFUSE_SELF_TEST_ECC();
char safeti_SRAM_RADECODE_DIAGNOSTICS();
char safeti_SRAM_PAR_ADDR_CTRL_SELF_TEST();
char safeti_SRAM_ECC_ERROR_PROFILING();
char safeti_FLASH_ADDRESS_ECC_FAULT_INJECT();
char safeti_FLASH_ADDRESS_PARITY_SELF_TEST();
char safeti_FLASH_ECC_TEST_MODE_2BIT_FAULT_INJECT();
char safeti_SRAM_ECC_1BIT_FAULT_INJECTION();
char safeti_SRAM_ECC_2BIT_FAULT_INJECT();
char safeti_SRAM_ECC_ERROR_PROFILING_FAULT_INJECT();
char safeti_CCMR4F_SELF_TEST();
char safeti_CCMR4F_ERROR_FORCING_TEST();
char safeti_CCMR4F_SELF_TEST_ERROR_FORCING();

#endif /* SAFETI_TEST_API_H_ */
