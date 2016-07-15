/*******************************************************************************
 **+--------------------------------------------------------------------------+**
 **|                            ****                                          |**
 **|                            ****                                          |**
 **|                            ******o***                                    |**
 **|                      ********_///_****                                   |**
 **|                      ***** /_//_/ ****                                   |**
 **|                       ** ** (__/ ****                                    |**
 **|                           *********                                      |**
 **|                            ****                                          |**
 **|                            ***                                           |**
 **|                                                                          |**
 **|         Copyright (c) 2012 Texas Instruments Incorporated                |**
 **|                        ALL RIGHTS RESERVED                               |**
 **|                                                                          |**
 **| Permission is hereby granted to licensees of Texas Instruments           |**
 **| Incorporated (TI) products to use this computer program for the sole     |**
 **| purpose of implementing a licensee product based on TI products.         |**
 **| No other rights to reproduce, use, or disseminate this computer          |**
 **| program, whether in part or in whole, are granted.                       |**
 **|                                                                          |**
 **| TI makes no representation or warranties with respect to the             |**
 **| performance of this computer program, and specifically disclaims         |**
 **| any responsibility for any damages, special or consequential,            |**
 **| connected with the use of this program.                                  |**
 **|                                                                          |**
 **+--------------------------------------------------------------------------+**
 *******************************************************************************/
/**
 * @file        Board_Config.h
 * @version     0.1.0
 * @brief       File contains the board configure definitions
 *
 * <\todo Detailed description goes here>
 */


#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "mibspi.h"
#include "Application_Configuration.h"
#include "gio.h"
#include "rti.h"
#include "adc.h"
#include "TPS_Types.h"


#define SPI_COMPATIBILITY_MODE 1U

#define RTI_TPS_DEFAULTS { 0, \
                           0, \
						 }
#define SPI_TPS_DEFAULTS { (mibspiBASE_t *)spiREG2, /*SPI2*/\
                           0, \
						 }
#define ADC_TPS_DEFAULTS { (adcBASE_t *)adcREG1, /*ADC1*/\
						   1, \
                           0, \
						 }

/**<voltage limits 2.45-2.55*/
/**<voltage limits 1.9-2.1*/
/**<voltage limits 0.8-5.5*/
/**<voltage limits 1.20148-5*/
/**<voltage limits 0.4-4*/
/**<voltage limits 0.4-4*/
/**<voltage limits 1.20148-1.25052*/
/**<voltage limits 1.20148-1.25052*/
#define VOLTAGE_RAIL_LIMITS                                {{3041,3165},\
                                                           {2358,2607},\
                                                           {993,4096},\
                                                           {1491,4096},\
                                                           {496,4096},\
                                                           {496,4096},\
                                                           {3041,3165},\
                                                           {3041,3165}}
#define DMUX_MIN_ADC 410
#define DMUX_MAX_ADC 3700

#endif
