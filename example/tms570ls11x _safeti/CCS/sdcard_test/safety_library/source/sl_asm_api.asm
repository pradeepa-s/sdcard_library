;/*******************************************************************************
;**+--------------------------------------------------------------------------+**
;**|                            ****                                          |**
;**|                            ****                                          |**
;**|                            ******o***                                    |**
;**|                      ********_///_****                                   |**
;**|                      ***** /_//_/ ****                                   |**
;**|                       ** ** (__/ ****                                    |**
;**|                           *********                                      |**
;**|                            ****                                          |**
;**|                            ***                                           |**
;**|                                                                          |**
;**|         Copyright (c) 2012 Texas Instruments Incorporated                |**
;**|                        ALL RIGHTS RESERVED                               |**
;**|                                                                          |**
;**| Permission is hereby granted to licensees of Texas Instruments           |**
;**| Incorporated (TI) products to use this computer program for the sole     |**
;**| purpose of implementing a licensee product based on TI products.         |**
;**| No other rights to reproduce, use, or disseminate this computer          |**
;**| program, whether in part or in whole, are granted.                       |**
;**|                                                                          |**
;**| TI makes no representation or warranties with respect to the             |**
;**| performance of this computer program, and specifically disclaims         |**
;**| any responsibility for any damages, special or consequential,            |**
;**| connected with the use of this program.                                  |**
;**|                                                                          |**
;**+--------------------------------------------------------------------------+**
;*******************************************************************************/
;/**
; * @file        sl_asm_api.inc
; * @version     2.2.0
; * @brief       Safety Library APIs in Assembly
; *
; * @notes		1. All functions are redirected to test_sl section
; *
; * <\todo Detailed description goes here>
; */
.text_sl
    .arm
    .include sl_asm.inc
    .cdecls C, LIST, "sl_config.h"





        .if $$isdefed("_TMS570LC43x_") = 0 & $$isdefed("_RM57Lx_") = 0
    .def     _SL_Init_ECCFlash
    .asmfunc
_SL_Init_ECCFlash

        STMFD   SP!, {R0}

        MRC     P15, #0, R0, C1, C0, #1
        ORR     R0, R0, #AUXCR_ATCMPCEN
        DMB
        MCR     P15, #0, R0, C1, C0, #1

        LDMFD   SP!, {R0}
        BX      LR

    .endasmfunc
        .endif




	.def _SL_Init_EnableEventExport
	.asmFunc
_SL_Init_EnableEventExport

		STMFD   SP!, {R0}

        MRC     P15, #0, R0, C9, C12, #0
        ORR     R0, R0, #PMCR_EVNTEXP
        MCR     P15, #0, R0, C9, C12, #0

        LDMFD   SP!, {R0}
        BX      LR

    .endasmfunc





        .if $$isdefed("_TMS570LC43x_") = 0 & $$isdefed("_RM57Lx_") = 0
    .def     _SL_Init_ECCTCMRAM
    .asmfunc

_SL_Init_ECCTCMRAM

        STMFD SP!, {R0}
        MRC     P15, #0, R0, C1, C0, #1
        ORR     R0, R0, #AUXCR_B0TCMPCEN
        ORR     R0, R0, #AUXCR_B1TCMPCEN
        MCR     P15, #0, R0, C1, C0, #1
        LDMFD   SP!, {R0}
        BX      LR

    .endasmfunc
        .endif



    .def _SL_Disable_IRQ
    .asmfunc

_SL_Disable_IRQ
		STMFD   SP!, {R1}		;Save R1 on stack

		MRS		R0, CPSR	;Get current CPSR Value
		MOV		R1, R0		;R1:For write back, R0: For return value
		ORR     	R1, R0, #0x80	;Disable IRQ & FIQ & write R1 to CPSR
		MSR     	CPSR_CSXF, R1   ;
		AND		R0, R0, #0x80	;Return just previous I & F bits

		LDMFD   SP!, {R1}		;Pop R1 from stack
		BX      LR

    .endasmfunc


    .def _SL_Restore_IRQ
    .asmfunc

_SL_Restore_IRQ
		STMFD   SP!, {R1,R2}	;Save R1 on stack

		MRS		R1, CPSR	;Retrieve current CPSR
		MOV		R2, #0x80	;Need to clear these two bits
		BIC     	R1, R1, R2      ;
		ORR		R1, R1, R0	;Set I & Q bits as per input
		MSR		CPSR_CSXF, R1	;Write to CPSR

    	LDMFD   SP!, {R1,R2}	;Pop R1 from stack
		BX      LR
    .endasmfunc



    .def _SL_Get_ARM_Mode
    .asmFunc
_SL_Get_ARM_Mode

        MRS     R0, CPSR                ;Get CPSR Value
        AND     R0, R0, #ARM_MODE_SYS   ;Clear rest of the information
		BX      LR

    .endasmfunc


    .def _SL_Get_DataFault_Status
    .asmFunc
_SL_Get_DataFault_Status

        MRC     p15, #0, r0, c5, c0, #0
        BX      LR

    .endasmfunc



    .def _SL_Get_DataFault_Address
    .asmFunc
_SL_Get_DataFault_Address

        MRC     p15, #0, r0, c6, c0, #0
        BX      LR

    .endasmfunc


        .if $$isdefed("_TMS570LC43x_") = 0 & $$isdefed("_RM57Lx_") = 0
;/**
; * @name    SL_Init_R4Registers
; * @brief   Initilizes Cortex-R4F CPU's internal registers.
; *
; * Hercules series of MCU include dual Cortex-R4F CPUs in lock-step. At power up registers are not
; * guaranteed to be in same state for both the CPUs. The CPU pushes the internal registers on to
; * the stack on a function call, which could lead to the detection of a core compare error.
; * Therefore, the CPU internal core registers need to be initialized to a predefined state before
; * any function call is made. This function is recommended to be executed right after a reset.
; *
; * @param [in]  None
; * @param [out] None
; *
; * @return      None
; *
; * Example Usage:
; * @code
; *      SL_Init_R4Registers();
; * @code
; *
; * @entrymode   Any Privilege Mode
; * @exitmode    SVC Mode
; *
; * @seealso     SL_Init_StackPointers()
; *
; * @note        This function does not initialize Stack Pointers. SL_Init_StackPointers() must be called
; *              to initialize the stack pointers.
; *
; * @reqtag      \todo Add requirement tag here
; */


    .def SL_Init_R4Registers
    .asmfunc
SL_Init_R4Registers
	    ; Start in Supervisor Mode
        ;CPS    ARM_MODE_SVC
        MOV     R0, #SL_REG_INIT_VAL
        MOV     R1, #SL_REG_INIT_VAL
        MOV     R2, #SL_REG_INIT_VAL
        MOV     R3, #SL_REG_INIT_VAL
        MOV     R4, #SL_REG_INIT_VAL
        MOV     R5, #SL_REG_INIT_VAL
        MOV     R6, #SL_REG_INIT_VAL
        MOV     R7, #SL_REG_INIT_VAL
        MOV     R8, #SL_REG_INIT_VAL
        MOV     R9, #SL_REG_INIT_VAL
        MOV     R10, #SL_REG_INIT_VAL
        MOV     R11, #SL_REG_INIT_VAL
        MOV     R12, #SL_REG_INIT_VAL
        MOV     R13, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_FIQ
        MOV     R8, #SL_REG_INIT_VAL
        MOV     R9, #SL_REG_INIT_VAL
        MOV     R10, #SL_REG_INIT_VAL
        MOV     R11, #SL_REG_INIT_VAL
        MOV     R12, #SL_REG_INIT_VAL
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_IRQ
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_ABT
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_UND
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_SYS
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_SVC

        .if	_VFP_SUPPORT_ = 1
        mrc   p15,     #0x00,      r2,       c1, c0, #0x02
        orr   r2,      r2,         #0xF00000
        mcr   p15,     #0x00,      r2,       c1, c0, #0x02
        mov   r2,      #0x40000000
        fmxr  fpexc,   r2

        fmdrr d0,         r1,     r1
        fmdrr d1,         r1,     r1
        fmdrr d2,         r1,     r1
        fmdrr d3,         r1,     r1
        fmdrr d4,         r1,     r1
        fmdrr d5,         r1,     r1
        fmdrr d6,         r1,     r1
        fmdrr d7,         r1,     r1
        fmdrr d8,         r1,     r1
        fmdrr d9,         r1,     r1
        fmdrr d10,        r1,     r1
        fmdrr d11,        r1,     r1
        fmdrr d12,        r1,     r1
        fmdrr d13,        r1,     r1
        fmdrr d14,        r1,     r1
        fmdrr d15,        r1,     r1
        .endif
        BX      LR
    .endasmfunc
        .endif

        .if $$isdefed("_TMS570LC43x_") | $$isdefed("_RM57Lx_")
;/**
; * @name    SL_Init_R5Registers
; * @brief   Initilizes Cortex-R5 CPU's internal registers.
; *
; * Hercules series of MCU include dual Cortex-R5F CPUs in lock-step. At power up registers are not
; * guaranteed to be in same state for both the CPUs. The CPU pushes the internal registers on to
; * the stack on a function call, which could lead to the detection of a core compare error.
; * Therefore, the CPU internal core registers need to be initialized to a predefined state before
; * any function call is made. This function is recommended to be executed right after a reset.
; *
; * @param [in]  None
; * @param [out] None
; *
; * @return      None
; *
; * Example Usage:
; * @code
; *      SL_Init_R5Registers();
; * @code
; *
; * @entrymode   Any Privilege Mode
; * @exitmode    SVC Mode
; *
; * @seealso     SL_Init_StackPointers()
; *
; * @note        This function does not initialize Stack Pointers. SL_Init_StackPointers() must be called
; *              to initialize the stack pointers.
; *
; * @reqtag      \todo Add requirement tag here
; */


    .def SL_Init_R5Registers
    .asmfunc
SL_Init_R5Registers

        ; Start in Supervisor Mode
        ;CPS    ARM_MODE_SVC                            
        MOV     R0, #SL_REG_INIT_VAL                
        MOV     R1, #SL_REG_INIT_VAL
        MOV     R2, #SL_REG_INIT_VAL
        MOV     R3, #SL_REG_INIT_VAL
        MOV     R4, #SL_REG_INIT_VAL
        MOV     R5, #SL_REG_INIT_VAL
        MOV     R6, #SL_REG_INIT_VAL
        MOV     R7, #SL_REG_INIT_VAL
        MOV     R8, #SL_REG_INIT_VAL
        MOV     R9, #SL_REG_INIT_VAL
        MOV     R10, #SL_REG_INIT_VAL
        MOV     R11, #SL_REG_INIT_VAL
        MOV     R12, #SL_REG_INIT_VAL
        MOV     R13, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_FIQ
        MOV     LR, R0
        MOV     R8, #SL_REG_INIT_VAL
        MOV     R9, #SL_REG_INIT_VAL
        MOV     R10, #SL_REG_INIT_VAL
        MOV     R11, #SL_REG_INIT_VAL
        MOV     R12, #SL_REG_INIT_VAL
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_IRQ
        MOV     LR, R0
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_ABT
        MOV     LR, R0
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_UND
        MOV     LR, R0
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_SYS
        MOV     LR, R0
        MOV     R13, #SL_REG_INIT_VAL
        MOV     R14, #SL_REG_INIT_VAL
        MRS     R1, CPSR
        MSR     SPSR_CXSF, R1

        CPS     #ARM_MODE_SVC

        .if	_VFP_SUPPORT_ = 1
        mrc   p15,     #0x00,      r2,       c1, c0, #0x02
        orr   r2,      r2,         #0xF00000
        mcr   p15,     #0x00,      r2,       c1, c0, #0x02
        mov   r2,      #0x40000000
        fmxr  fpexc,   r2

        fmdrr d0,         r1,     r1
        fmdrr d1,         r1,     r1
        fmdrr d2,         r1,     r1
        fmdrr d3,         r1,     r1
        fmdrr d4,         r1,     r1
        fmdrr d5,         r1,     r1
        fmdrr d6,         r1,     r1
        fmdrr d7,         r1,     r1
        fmdrr d8,         r1,     r1
        fmdrr d9,         r1,     r1
        fmdrr d10,        r1,     r1
        fmdrr d11,        r1,     r1
        fmdrr d12,        r1,     r1
        fmdrr d13,        r1,     r1
        fmdrr d14,        r1,     r1
        fmdrr d15,        r1,     r1
        .endif
;        bl    $+4
;        bl    $+4
;        bl    $+4
;        bl    $+4
        BX      LR
    .endasmfunc
        .endif


        .if $$isdefed("EXTERNAL_SP_INIT") = 0
;/**
; * @name    SL_Init_StackPointers
; * @brief   Initilizes CPU's stack pointer for SVC, FIQ, IRQ, ABT, UND & SYS modes.
; *
; * This function initializes the stack pointers for the privileged modes of the CPU.
; * Following variables are used as stack pointers and hence must be defined by the application.
; *                  StackModeSVC, StackModeFIQ, StackModeIRQ, StackModeABT, StackModeUND, StackModeSYS
; * Function uses following macros in bytes, from sl_config.h file, to calculate the stack start.
; *                  STACK_SIZE_MODE_SVC, STACK_SIZE_MODE_FIQ, STACK_SIZE_MODE_IRQ, STACK_SIZE_MODE_ABT,
; *                  STACK_SIZE_MODE_UND & STACK_SIZE_MODE_SYS
; *
; * @param [in]  None
; * @param [out] None
; *
; * @return      None
; *
; * Example Usage:
; * @code
; *      SL_Init_R4Registers();
; * @code
; *
; * @entrymode   Any Privilege Mode
; * @exitmode    SVC Mode
; *
; * @seealso     SL_Init_R4Registers()
; *
; * @note        Application can define the macro EXTERNAL_SP_INIT to disable
; * the use of this API and use its own implementation to initialize the stack
; * pointers. This provision is provided because the diagnostic library expects
; * the application to define global variables pointing to the Stack pointer
; * locations, and if not using this API, need to be removed.
; *
; * @reqtag      \todo Add requirement tag here
; */
    .global StackModeSVC
    .global StackModeFIQ
    .global StackModeIRQ
    .global StackModeABT
    .global StackModeUND
    .global StackModeSYS

    .def SL_Init_StackPointers
    .asmfunc
SL_Init_StackPointers

        CPS     #ARM_MODE_SVC
        LDR     SP, gStackModeSVC

        CPS     #ARM_MODE_FIQ
        LDR     SP, gStackModeFIQ

        CPS     #ARM_MODE_IRQ
        LDR     SP, gStackModeIRQ

        CPS     #ARM_MODE_ABT
        LDR     SP, gStackModeABT

        CPS     #ARM_MODE_UND
        LDR     SP, gStackModeUND

        CPS     #ARM_MODE_SYS
        LDR     SP, gStackModeSYS

        CPS     #ARM_MODE_SVC
        BX      LR

gStackModeSVC: .field (StackModeSVC + STACK_SIZE_MODE_SVC), 32
gStackModeFIQ: .field (StackModeFIQ + STACK_SIZE_MODE_FIQ), 32
gStackModeIRQ: .field (StackModeIRQ + STACK_SIZE_MODE_IRQ), 32
gStackModeABT: .field (StackModeABT + STACK_SIZE_MODE_ABT), 32
gStackModeUND: .field (StackModeUND + STACK_SIZE_MODE_UND), 32
gStackModeSYS: .field (StackModeSYS + STACK_SIZE_MODE_SYS), 32

    .endasmfunc
        .endif

;/* Kick off the STC execution */
	.def _SL_Kickoff_STC_execution
    .asmFunc
_SL_Kickoff_STC_execution

		 WFI
         BX     LR ;Though not required, to be safe adding branch here

	.endasmfunc

;/* Barriers to ensure the the data accesses are complete & instr., are executed */
	.def _SL_Barrier_Data_Access
    .asmFunc
_SL_Barrier_Data_Access

		DMB
		DSB
		BX		LR

	.endasmfunc
