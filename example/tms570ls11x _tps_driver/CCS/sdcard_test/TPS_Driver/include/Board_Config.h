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

#if defined(_TMS570LS12x_) || defined(_TMS570LS1227CNCD_) || defined(_RM46CNCD_)

#define SPI_COMPATIBILITY_MODE 1U

#define RTI_TPS_DEFAULTS { 0, \
                           0, \
						 }
#define SPI_TPS_DEFAULTS { (mibspiBASE_t *)0xFFF7F600U, /*SPI2*/\
                           0, \
						 }
#define ADC_TPS_DEFAULTS { (adcBASE_t *)0xFFF7C000U, /*ADC1*/\
						   1, \
                           0, \
						 }
#define GIO_TPS_DEFAULTS { gioPORTA, /*gioPORTA*/\
						   4, \
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
#elif defined(_TMS570LS3137HITEXKIT_)|| defined(_RM48HITEXKIT_)
/*Hitex board, 5v adc*/
#define SPI_COMPATIBILITY_MODE 0U


#define RTI_TPS_DEFAULTS { 0, \
                           0, \
						 }
#define SPI_TPS_DEFAULTS { (mibspiBASE_t *)0xFFF7F800U, /*SPI2*/\
                           0, \
						 }
#define ADC_TPS_DEFAULTS { (adcBASE_t *)0xFFF7C000U, /*ADC1*/\
						   1, \
                           0, \
						 }
/*D-DiagOut*/
#define GIO_TPS_DEFAULTS { gioPORTA, /*gioPORTA*/\
						   3, \
						 }
#define  VOLTAGE_RAIL_LIMITS                              {{2007,2088},\
                                                           {1556,1720},\
                                                           {655,4096},\
                                                           {984,4096},\
                                                           {327,3276},\
                                                           {327,3276},\
                                                           {2007,2089},\
                                                           {2007,2089}};
#define DMUX_MIN_ADC 271
#define DMUX_MAX_ADC 2442
#else
/*Hitex board, 5v adc*/

#define SPI_COMPATIBILITY_MODE 0U

#define RTI_TPS_DEFAULTS { 0, \
                           0, \
                         }
#define SPI_TPS_DEFAULTS { (mibspiBASE_t *)0xFFF7F800U, /*SPI2*/\
                           0, \
                         }
#define ADC_TPS_DEFAULTS { (adcBASE_t *)0xFFF7C000U, /*ADC1*/\
                           1, \
                           0, \
                         }
/*D-DiagOut*/
#define GIO_TPS_DEFAULTS { gioPORTA, /*gioPORTA*/\
                           3, \
                         }
#define  VOLTAGE_RAIL_LIMITS                              {{2007,2088},\
                                                           {1556,1720},\
                                                           {655,4096},\
                                                           {984,4096},\
                                                           {327,3276},\
                                                           {327,3276},\
                                                           {2007,2089},\
                                                           {2007,2089}};
#define DMUX_MIN_ADC 271
#define DMUX_MAX_ADC 2442
#endif
#endif
