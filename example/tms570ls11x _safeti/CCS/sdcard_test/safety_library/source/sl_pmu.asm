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
; * @file        sl_pmu.asm
; * @version     2.2.0
; * @brief       PMU APIs in Assembly
; *
; *
; */

; Initialize Pmu
; Note: It will reset all counters

    .def     _sl_pmuInit_
    .asmfunc

_sl_pmuInit_

        stmfd sp!, {r0}
        ; set control register
        mrc   p15, #0, r0, c9, c12, #0 
        orr   r0,  r0, #(1 << 4) + 6 + 1
        mcr   p15, #0, r0, c9, c12, #0
        ; clear flags
		mov   r0,  #0
		sub   r1,  r1,  #1
		mcr   p15, #0, r0, c9, c12, #3 
        ; select counter 0 event
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mov   r0,  #0x11
        mcr   p15, #0, r0, c9, c13, #1 ; select event
        ; select counter 1 event
		mov   r0,  #1
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mov   r0,  #0x11
        mcr   p15, #0, r0, c9, c13, #1 ; select event
        ; select counter 2 event
		mov   r0,  #2
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mov   r0,  #0x11
        mcr   p15, #0, r0, c9, c13, #1 ; select event
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Enable Counters Global [Cycle, Event [0..2]]
; Note: It will reset all counters

    .def     _sl_pmuEnableCountersGlobal_
    .asmfunc

_sl_pmuEnableCountersGlobal_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0 
        orr   r0,  r0, #7
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}		
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Disable Counters Global [Cycle, Event [0..2]]

    .def     _sl_pmuDisableCountersGlobal_
    .asmfunc

_sl_pmuDisableCountersGlobal_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0 
        bic   r0,  r0, #1
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}		
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Reset Cycle Counter

    .def     _sl_pmuResetCycleCounter_
    .asmfunc

_sl_pmuResetCycleCounter_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0 
        orr   r0,  r0, #4
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}		
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Reset Event Counters [0..2]

    .def     _sl_pmuResetEventCounters_
    .asmfunc

_sl_pmuResetEventCounters_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0 
        orr   r0,  r0, #2
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}		
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Reset Cycle Counter abd Event Counters [0..2]

    .def     _sl_pmuResetCounters_
    .asmfunc

_sl_pmuResetCounters_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0 
        orr   r0,  r0, #6
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}		
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Start Counters [Cycle, 0..2]

    .def     _sl_pmuStartCounters_
    .asmfunc

_sl_pmuStartCounters_

        mcr   p15, #0, r0, c9, c12, #1
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Stop Counters [Cycle, 0..2]

    .def     _sl_pmuStopCounters_
    .asmfunc

_sl_pmuStopCounters_

        mcr   p15, #0, r0, c9, c12, #2
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Set Count event

    .def     _sl_pmuSetCountEvent_
    .asmfunc

_sl_pmuSetCountEvent_
 
        lsr   r0,  r0, #1
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mcr   p15, #0, r1, c9, c13, #1 ; select event
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Get Cycle Count

    .def     _sl_pmuGetCycleCount_
    .asmfunc

_sl_pmuGetCycleCount_

        mrc   p15, #0, r0, c9, c13, #0
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Get Event Counter Count Value

    .def     _sl_pmuGetEventCount_
    .asmfunc

_sl_pmuGetEventCount_

        lsr   r0,  r0, #1
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mrc   p15, #0, r0, c9, c13, #2 ; read event counter
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Get Overflow Flags

    .def     _sl_pmuGetOverflow_
    .asmfunc

_sl_pmuGetOverflow_

        mrc   p15, #0, r0, c9, c12, #3 ; read overflow
		mov   r1,  #0
		sub   r1,  r1,  #1
		mcr   p15, #0, r1, c9, c12, #3 ; clear flags
        bx    lr

    .endasmfunc

    

;-------------------------------------------------------------------------------

