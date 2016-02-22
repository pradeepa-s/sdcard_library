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
    
    section .text_sl:CODE
    arm
    
    INCLUDE sl_asm_IAR.inc
   ; #include "sl_config.h"
    
    ;/**< Stack size for SVC mode */
    #define STACK_SIZE_MODE_SVC 1024u   

    ;/**< Stack size for FIQ mode */
    #define STACK_SIZE_MODE_FIQ 1024u   

    ;/**< Stack size for IRQ mode */
    #define STACK_SIZE_MODE_IRQ 1024u   

    ;/**< Stack size for ABT mode */
    #define STACK_SIZE_MODE_ABT 1024u   

    ;/**< Stack size for UND mode */
    #define STACK_SIZE_MODE_UND 1024u 
    
    ;/**< Stack size for SYS mode */
    #define STACK_SIZE_MODE_SYS 1024u 
    
    #define SL_REG_INIT_VAL         0u


;        #if _TMS570LC43x_ = 1 | _RM57Lx_ = 1

    public     _SL_Init_ECCFlash

_SL_Init_ECCFlash

        STMFD   SP!, {R0}

        MRC     P15, #0, R0, C1, C0, #1
        ORR     R0, R0, #AUXCR_ATCMPCEN
        DMB
        MCR     P15, #0, R0, C1, C0, #1

        LDMFD   SP!, {R0}
        BX      LR


;        #endif




	public _SL_Init_EnableEventExport

_SL_Init_EnableEventExport

		STMFD   SP!, {R0}

        MRC     P15, #0, R0, C9, C12, #0
        ORR     R0, R0, #PMCR_EVNTEXP
        MCR     P15, #0, R0, C9, C12, #0

        LDMFD   SP!, {R0}
        BX      LR


;        #if _TMS570LC43x_ = 1 | _RM57Lx_ = 1
    public     _SL_Init_ECCTCMRAM


_SL_Init_ECCTCMRAM

        STMFD SP!, {R0}
        MRC     P15, #0, R0, C1, C0, #1
        ORR     R0, R0, #AUXCR_B0TCMPCEN
        ORR     R0, R0, #AUXCR_B1TCMPCEN
        MCR     P15, #0, R0, C1, C0, #1
        LDMFD   SP!, {R0}
        BX      LR


;        #endif



    public _SL_Disable_IRQ


_SL_Disable_IRQ
		STMFD   SP!, {R1}		;Save R1 on stack

		MRS		R0, CPSR	;Get current CPSR Value
		MOV		R1, R0		;R1:For write back, R0: For return value
		ORR     	R1, R0, #0x80	;Disable IRQ & FIQ & write R1 to CPSR
		MSR     	CPSR_CSXF, R1   ;
		AND		R0, R0, #0x80	;Return just previous I & F bits

		LDMFD   SP!, {R1}		;Pop R1 from stack
		BX      LR




    public _SL_Restore_IRQ


_SL_Restore_IRQ
		STMFD   SP!, {R1,R2}	;Save R1 on stack

		MRS		R1, CPSR	;Retrieve current CPSR
		MOV		R2, #0x80	;Need to clear these two bits
		BIC     	R1, R1, R2      ;
		ORR		R1, R1, R0	;Set I & Q bits as per input
		MSR		CPSR_CSXF, R1	;Write to CPSR

    	LDMFD   SP!, {R1,R2}	;Pop R1 from stack
		BX      LR




    public _SL_Get_ARM_Mode

_SL_Get_ARM_Mode

        MRS     R0, CPSR                ;Get CPSR Value
        AND     R0, R0, #ARM_MODE_SYS   ;Clear rest of the information
		BX      LR

    public _SL_Get_DataFault_Status

_SL_Get_DataFault_Status

        MRC     p15, #0, r0, c5, c0, #0
        BX      LR

    public _SL_Get_DataFault_Address

_SL_Get_DataFault_Address

        MRC     p15, #0, r0, c6, c0, #0
        BX      LR

;        #if _TMS570LC43x_ = 0 && _RM57Lx_ = 0
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
; * @seealso     SL_Init_StackPointers(), SL_Init_VFPRegisters()
; *
; * @note        This function does not initialize Stack Pointers. SL_Init_StackPointers() must be called
; *              to initialize the stack pointers.
; *
; * @reqtag      \todo Add requirement tag here
; */


    public SL_Init_R4Registers

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

        #ifdef	_VFP_SUPPORT_
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
        #endif
        BX      LR

;        #endif

        #if _TMS570LC43x_ = 1 | _RM57Lx_ = 1
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
; * @seealso     SL_Init_StackPointers(), SL_Init_VFPRegisters()
; *
; * @note        This function does not initialize Stack Pointers. SL_Init_StackPointers() must be called
; *              to initialize the stack pointers.
; *
; * @reqtag      \todo Add requirement tag here
; */


    public SL_Init_R5Registers

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

        #ifdef	_VFP_SUPPORT_
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
        #endif
;        bl    $+4
;        bl    $+4
;        bl    $+4
;        bl    $+4
        BX      LR

        #endif

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
; * @seealso     SL_Init_R4Registers(), SL_Init_VFPRegisters()
; *
; * @note        This function does not initialize the SP for User Mode.
; *
; * @reqtag      \todo Add requirement tag here
; */
    EXTERN StackModeSVC
    EXTERN StackModeFIQ
    EXTERN StackModeIRQ
    EXTERN StackModeABT
    EXTERN StackModeUND
    EXTERN StackModeSYS

    public SL_Init_StackPointers

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

;need to extract bit field of 32
gStackModeSVC dcd (StackModeSVC + STACK_SIZE_MODE_SVC)
gStackModeFIQ dcd (StackModeFIQ + STACK_SIZE_MODE_FIQ)
gStackModeIRQ dcd (StackModeIRQ + STACK_SIZE_MODE_IRQ)
gStackModeABT dcd (StackModeABT + STACK_SIZE_MODE_ABT)
gStackModeUND dcd (StackModeUND + STACK_SIZE_MODE_UND)
gStackModeSYS dcd (StackModeSYS + STACK_SIZE_MODE_SYS)



;/* Kick off the STC execution */
	public _SL_Kickoff_STC_execution

_SL_Kickoff_STC_execution

		 WFI
         BX     LR ;Though not required, to be safe adding branch here



;/* Barriers to ensure the the data accesses are complete & instr., are executed */
	public _SL_Barrier_Data_Access

_SL_Barrier_Data_Access

		DMB
		DSB
		BX		LR


  #undef __ASSM__

  end