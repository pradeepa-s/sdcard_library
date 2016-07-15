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

    section .text:CODE
    arm

; Initialize Pmu
; Note: It will reset all counters

    public     _sl_pmuInit_


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




;-------------------------------------------------------------------------------
; Enable Counters Global [Cycle, Event [0..2]]
; Note: It will reset all counters

    public     _sl_pmuEnableCountersGlobal_


_sl_pmuEnableCountersGlobal_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #7
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}
        bx    lr



;-------------------------------------------------------------------------------
; Disable Counters Global [Cycle, Event [0..2]]

    public     _sl_pmuDisableCountersGlobal_


_sl_pmuDisableCountersGlobal_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0
        bic   r0,  r0, #1
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}
        bx    lr



;-------------------------------------------------------------------------------
; Reset Cycle Counter

    public     _sl_pmuResetCycleCounter_


_sl_pmuResetCycleCounter_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #4
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}
        bx    lr



;-------------------------------------------------------------------------------
; Reset Event Counters [0..2]

    public     _sl_pmuResetEventCounters_


_sl_pmuResetEventCounters_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #2
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}
        bx    lr



;-------------------------------------------------------------------------------
; Reset Cycle Counter abd Event Counters [0..2]

    public     _sl_pmuResetCounters_


_sl_pmuResetCounters_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #6
        mcr   p15, #0, r0, c9, c12, #0
        ldmfd sp!, {r0}
        bx    lr



;-------------------------------------------------------------------------------
; Start Counters [Cycle, 0..2]

    public     _sl_pmuStartCounters_


_sl_pmuStartCounters_

        mcr   p15, #0, r0, c9, c12, #1
        bx    lr



;-------------------------------------------------------------------------------
; Stop Counters [Cycle, 0..2]

    public     _sl_pmuStopCounters_


_sl_pmuStopCounters_

        mcr   p15, #0, r0, c9, c12, #2
        bx    lr



;-------------------------------------------------------------------------------
; Set Count event

    public     _sl_pmuSetCountEvent_


_sl_pmuSetCountEvent_

        lsr   r0,  r0, #1
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mcr   p15, #0, r1, c9, c13, #1 ; select event
        bx    lr



;-------------------------------------------------------------------------------
; Get Cycle Count

    public     _sl_pmuGetCycleCount_


_sl_pmuGetCycleCount_

        mrc   p15, #0, r0, c9, c13, #0
        bx    lr



;-------------------------------------------------------------------------------
; Get Event Counter Count Value

    public     _sl_pmuGetEventCount_


_sl_pmuGetEventCount_

        lsr   r0,  r0, #1
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mrc   p15, #0, r0, c9, c13, #2 ; read event counter
        bx    lr



;-------------------------------------------------------------------------------
; Get Overflow Flags

    public     _sl_pmuGetOverflow_


_sl_pmuGetOverflow_

        mrc   p15, #0, r0, c9, c12, #3 ; read overflow
		mov   r1,  #0
		sub   r1,  r1,  #1
		mcr   p15, #0, r1, c9, c12, #3 ; clear flags
        bx    lr





;-------------------------------------------------------------------------------

  end

;-------------------------------------------------------------------------------