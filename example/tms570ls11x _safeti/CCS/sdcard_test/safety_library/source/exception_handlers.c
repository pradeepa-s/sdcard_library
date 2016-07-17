#include <sl_api.h>
#include "register_readback.h"
#include "esm_application_callback.h"
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#if FUNCTION_PROFILING_ENABLED
#include "sys_pmu.h"
#endif
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#if FUNCTION_PROFILING_ENABLED
#include "HL_sys_pmu.h"
#endif
#endif

/*macros*/
#define ESM_GRP3_MASK (3U<<16U)
/*macros for bit operations*/
#define BIT(n)                  ((uint32)((uint32)1u <<(n)))

#define BIT_SET(y, mask)        ((y) |=  (mask))

#define BIT_CLEAR(y, mask)      ((y) &= ~(mask))

#define BIT_FLIP(y, mask)       ((y) ^=  (mask))

/* Create a bitmask of length \a len.*/
#define BIT_MASK(len)           ((uint32)(BIT(len)-((uint32)1u)))

/* Create a bitfield mask of length \a starting at bit \a start.*/
#define BF_MASK(start, len)     (BIT_MASK(len)<<(start))

/* Prepare a bitmask for insertion or combining.*/
#define BF_PREP(x, start, len)  (((x)&BIT_MASK(len)) << (start))
/* SAFETYMCUSW 79 S MR:19.4 <INSPECTED> "Use of ternary operator is allowed" */
#define GET_ESM_BIT_NUM(x)		((x)<(uint32)32U? (BIT((x))): (BIT(((x)-(uint32)32U))))
/* Extract a bitfield of length \a len starting at bit \a start from \a y.
 * y is the register
 * start is the starting bit of the field
 * len is the length of the field*/
#define BF_GET(y, start, len)   (((y)>>(start)) & BIT_MASK((len)))

boolean SL_FLAG_GET(sint32 flag_id); /* avoid compiler warning */

/*Insert a new bitfield value \a x into \a y.
 * y is the register
 * x is the value of the field
 * start is the starting bit of the field
 * len is the length of the field*/
#define BF_SET(y, x, start, len)    \
    ( (y) = ((y) & (~ (BF_MASK((start), (len))))) | BF_PREP((x), (start), (len)) )

#ifdef __TI_COMPILER_VERSION__
#pragma INTERRUPT ( _excpt_vec_udef_instr, UDEF)
void _excpt_vec_udef_instr()
#endif
#ifdef __IAR_SYSTEMS_ICC__
__irq __arm void _excpt_vec_udef_instr()
#endif
{
	while(1);

}

#ifdef __TI_COMPILER_VERSION__
#pragma INTERRUPT ( _excpt_vec_svc, SWI)
void _excpt_vec_svc()
#endif
#ifdef __IAR_SYSTEMS_ICC__
__irq __arm void _excpt_vec_svc()
#endif
{
	while(1);
}

#ifdef __TI_COMPILER_VERSION__
#pragma INTERRUPT ( _excpt_vec_abort_pref, PABT)
void _excpt_vec_abort_pref()
#endif
#ifdef __IAR_SYSTEMS_ICC__
__irq __arm void _excpt_vec_abort_pref()
#endif
{
	while(1);
}

extern volatile uint64 sramEccTestBuff[];

#ifdef __TI_COMPILER_VERSION__
#pragma INTERRUPT ( _excpt_vec_abort_data, DABT)
void _excpt_vec_abort_data()
#endif
#ifdef __IAR_SYSTEMS_ICC__
__irq __arm void _excpt_vec_abort_data()
#endif
{
	register uint32 callbkParam1 = 0u, callbkParam2 = 0u, callbkParam3 = 0u;
	boolean maskDAbort = FALSE;
#if FUNCTION_PROFILING_ENABLED
    register uint32 entrytick=0u;
    entrytick = _pmuGetCycleCount_();
#endif

   /*
    * Check if this abort was intentional, via a self-test. If intentional, then
    * mask this abort.
    */

	/* DAbort due to access to ECC Memory? */
#if 0
	if ((0x00000409u == (0x00000409u & _SL_Get_DataFault_Status())) &&
		(0x00400000u == (0x00400000u & _SL_Get_DataFault_Address())) &&
		(TRUE == sl_isSelfTest)) {
		maskDAbort = TRUE;
	}
#endif

	/*
	 * DAbort due to access to illegal transcation to L2  Memory?
     * 0x00000009 indicates that it is an external abort caused by read and is AXI decode error
     * 0x70000000 is the reserved location accessed to create the L2 interconnect error trap AXI decode error
     */
	if ((0x00000008u == (0x00000008u & _SL_Get_DataFault_Status())) &&
		(0x70000000 == _SL_Get_DataFault_Address())) {
		maskDAbort = TRUE;
	}

	/*
	 * DAbort due to access to illegal transcation to L2  Memory?
     * 0x00000009 indicates that it is an external abort caused by read and is AXI decode error
     * 0xFD000000 is the reserved location accessed to create the L2 interconnect error trap AXI decode error
     */
	if ((0x00000008u == (0x0000008u & _SL_Get_DataFault_Status())) &&
		(0xFD000000 == _SL_Get_DataFault_Address())){
		maskDAbort = TRUE;
	}
	/*
	 * DAbort due to access to  PMM global control register (sl_pmmREG->GLOBALCTRL1) access in user mode?
     * 0x00000009 indicates that it is an external abort caused by read and is AXI decode error
     * 0xFD000000 is the reserved location accessed to create the L2 interconnect error trap AXI decode error
     */
	if ((0x00000800u == (0x0000800u & _SL_Get_DataFault_Status())) &&
		(0xFFFF00A0 == _SL_Get_DataFault_Address())){
		maskDAbort = TRUE;
	}

#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)

	/* DAbort due to an SRAM ECC 2Bit self test? */
    if (((TCRAM_RAMCTRL_ECCWREN == (sl_tcram1REG->RAMCTRL & TCRAM_RAMCTRL_ECCWREN))||(TCRAM_RAMCTRL_ECCWREN == (sl_tcram2REG->RAMCTRL & TCRAM_RAMCTRL_ECCWREN)))&&(TRUE == SL_FLAG_GET(SRAM_ECC_ERROR_FORCING_2BIT))) {
    	/* So looks like writes to ECC region is enabled, check if the error address is in the test buffer range */
    		maskDAbort = TRUE;
    };

	/* DAbort due to an SRAM ECC 2Bit self test? */
    if (((TCRAM_RAMCTRL_ECCWREN == (sl_tcram1REG->RAMCTRL & TCRAM_RAMCTRL_ECCWREN))||(TCRAM_RAMCTRL_ECCWREN == (sl_tcram2REG->RAMCTRL & TCRAM_RAMCTRL_ECCWREN)))&&(TRUE == SL_FLAG_GET(SRAM_ECC_2BIT_FAULT_INJECT))) {
    	/* So looks like writes to ECC region is enabled, check if the error address is in the test buffer range */
    		maskDAbort =FALSE;
    		if(GET_ESM_BIT_NUM(ESM_G3ERR_B0TCM_ECC_UNCORR) == (sl_esmREG->SR1[2] & GET_ESM_BIT_NUM(ESM_G3ERR_B0TCM_ECC_UNCORR)))
    		{
    			callbkParam1 = sl_tcram1REG->RAMUERRADDR;
    			callbkParam2 = sl_tcram1REG->RAMERRSTATUS;
    			ESM_ApplicationCallback((uint32)(ESM_GRP3_MASK | ESM_G3ERR_B0TCM_ECC_UNCORR), callbkParam1, callbkParam2, callbkParam3);
    		}
    		if(GET_ESM_BIT_NUM(ESM_G3ERR_B1TCM_ECC_UNCORR) == (sl_esmREG->SR1[2] & GET_ESM_BIT_NUM(ESM_G3ERR_B1TCM_ECC_UNCORR)))
    		{
    			callbkParam1 = sl_tcram2REG->RAMUERRADDR;
    			callbkParam2 = sl_tcram2REG->RAMERRSTATUS;
    			ESM_ApplicationCallback((uint32)(ESM_GRP3_MASK | ESM_G3ERR_B1TCM_ECC_UNCORR), callbkParam1, callbkParam2, callbkParam3);
    		}

    }
#endif
#if 0
    /* DAbort due to an Flash Wrapper test ? */
    if (0 != (sl_flashWREG->FDIAGCTRL & 0x7) &&
			(TRUE == sl_isSelfTest)) {
    	//TODO: Need to compare the memory addresses also
    	/* Though it's not necessary, turn-off the diag mode */
    	sl_flashWREG->FDIAGCTRL &= 0xFFF0FFF8; /* Clear Diagnostics enable key */
    	maskDAbort = TRUE;
    }
#endif
    /* DAbort due to an Flash Wrapper test ? */
    if (((TRUE == SL_FLAG_GET(FLASH_ECC_TEST_MODE_2BIT)))||(TRUE == SL_FLAG_GET(FLASH_ECC_ADDR_TAG_REG_MODE))) {
    	maskDAbort = TRUE;
    	//TODO: Need to compare the memory addresses also
    	/* Though it's not necessary, turn-off the diag mode */
    	sl_flashWREG->FDIAGCTRL &= 0xFFF0FFF8; /* Clear Diagnostics enable key */
    }
	
#if !defined(_TMS570LC43x_) && !defined(_RM57Lx_)

    /* DAbort due to an Flash Wrapper test 2 bit ecc fault inject? */
    else if (0 != (sl_flashWREG->FDIAGCTRL & 0x7)) {
    	maskDAbort = FALSE;
    	//TODO: Need to compare the memory addresses also
    	/* Though it's not necessary, turn-off the diag mode */
    	sl_flashWREG->FDIAGCTRL &= 0xFFF0FFF8; /* Clear Diagnostics enable key */
		callbkParam1 = sl_flashWREG->FUNCERRADD;
		callbkParam2 = sl_flashWREG->FEDACSTATUS;
		ESM_ApplicationCallback((uint32)(ESM_GRP3_MASK | ESM_G3ERR_FMC_UNCORR), callbkParam1, callbkParam2, callbkParam3);
    }
#endif

    if (FALSE == maskDAbort) {
    	//TODO: Extract err address & report to application
    	//while(1);
    }
#if FUNCTION_PROFILING_ENABLED
{
if(SL_Profile_Struct[SL_Active_Profile_Testtype-TESTTYPE_MIN].aborthandler_entrytick == 0 )
{
    SL_Profile_Struct[SL_Active_Profile_Testtype - TESTTYPE_MIN].aborthandler_entrytick = entrytick;
    SL_Profile_Struct[SL_Active_Profile_Testtype-TESTTYPE_MIN].aborthandler_exittick = _pmuGetCycleCount_();
}
/* Update the return address, on stack, so that we return to the next instruction */
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
#ifdef __TI_COMPILER_VERSION__
	__asm(" LDR R0, [SP, #108]");
	__asm(" ADD R0, R0, #8");
	__asm(" STR R0, [SP, #108]");
#endif
#endif
#if defined(_RM42x_) || defined(_TMS570LS04x_)
#ifdef __TI_COMPILER_VERSION__
    	__asm(" add SP, SP, #12 ");
	__asm(" ldmfd	r13!, {r0 - r4, r12, lr} ");
	__asm(" subs	pc, lr, #4 " );
#endif
#endif
}
#else
{
/* Update the return address, on stack, so that we return to the next instruction */

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_)
#ifdef __TI_COMPILER_VERSION__
    	__asm(" LDR R0, [SP, #100]");
    	__asm(" ADD R0, R0, #8");
    	__asm(" STR R0, [SP, #100]");
#endif
#endif
#if defined(_RM42x_) || defined(_TMS570LS04x_)
#ifdef __TI_COMPILER_VERSION__
   	__asm(" add SP, SP, #4 ");
	__asm(" ldmfd	r13!, {r0 - r4, r12, lr} ");
	__asm(" subs	pc, lr, #4 " );
#endif

#endif
}
#endif
}
