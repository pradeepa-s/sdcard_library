/* Comments regarding various justificatios for deviation from MISRA-C coding guidelines
 * are provided at the bottom of the file */

#include <sl_api.h>
#include <sl_priv.h>
#include <string.h>
#ifdef LOOP_BACK_ENABLE
#include <can.h>
#include <mibspi.h>
#endif
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_1*/
void SL_SW_Reset(void)
{
    sl_systemREG1->SYSECR = (0x2u << 14);
}


#define CRC_CTRL0 		*(volatile uint32 *)0xFE000000U
#define CRC_CTRL2 		*(volatile uint32 *)0xFE000010U
#define CRC_STATUS 		*(volatile uint32 *)0xFE000028U
#define CRC_SIGREGL1 	*(volatile uint32 *)0xFE000060U
#define CRC_SIGREGH1 	*(volatile uint32 *)0xFE000064U
#define CRC_SIGREG 		*(volatile uint64 *)0xFE000060U
#define CRC_REGL1 		*(volatile uint32 *)0xFE000068U
#define CRC_REGH1 		*(volatile uint32 *)0xFE00006CU
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_1*/
uint64 SL_CRC_Calculate (uint64 * startAddr, uint32 count64)
{
	uint32 count = 0u;

	CRC_CTRL0 |= 0x00000001U; /* Reset the CRC Module */
	CRC_CTRL0 &= 0xFFFFFFFEU;
	CRC_CTRL2 |= 0x00000003U; /* Configure to Full CPU Mode */

	for (count=0u; count < count64; count++) {
		/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_2*/
		CRC_SIGREG = *startAddr;
		/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> Comment_2*/
		startAddr++;
	}
    return(CRC_SIGREG);
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_1*/
void SL_Clear_nERROR(void)
{
    sl_esmREG->EKR = ESM_NERROR_CLEAR;
    return;
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> Comment_1*/
void SL_Set_nERROR(void)
{
    sl_esmREG->EKR = ESM_NERROR_SET;
    return;
}

/*Comment_1:
 * "This function will be called by application so not static"*/

/*Comment_2:
 * "Reason - value 0 for start address is valid as it is used for crc calculation"*/


