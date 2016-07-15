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
; * @file        sl_asm.inc
; * @version     2.2.0
; * @brief       Macro definitions used in the assembly source files
; *
; * <\todo Detailed description goes here>
; */

ARM_MODE_USR    .equ (0x10)
ARM_MODE_FIQ    .equ (0x11)
ARM_MODE_IRQ    .equ (0x12)
ARM_MODE_SVC    .equ (0x13)
ARM_MODE_ABT    .equ (0x17)
ARM_MODE_UND    .equ (0x1B)
ARM_MODE_SYS    .equ (0x1F)


;CP15 Auxillary Control Register bit Definitions
AUXCR_B1TCMPCEN .equ (0x08000000)
AUXCR_B0TCMPCEN .equ (0x04000000)
AUXCR_ATCMPCEN	.equ (0x02000000)

PMCR_EVNTEXP	.equ (0x00000010)
