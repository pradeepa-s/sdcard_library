/*
 * Application_Types.h
 *
 *  Created on: Dec 26, 2013
 *      Author: a0131910
 */

#ifndef APPLICATION_TYPES_H_
#define APPLICATION_TYPES_H_

/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "adc.h"
#include "mibspi.h"
#include "rti.h"


/*board type macros*/
typedef volatile struct SL_Rti_Para
{
	unsigned int counter;/*the counter used to measure time to test the ESM response*/
	unsigned int compare;/*the compare used to measure time to test the ESM response*/
} SL_Rti_Param_t;

typedef volatile struct SL_Spi_Para
{
	mibspiBASE_t *SPI_TPS;/*SPI base address*/
	unsigned int nCS_TPS;/*CS number used to talk to the TPS65381*/
} SL_Spi_Param_t;

typedef volatile struct SL_Adc_Para
{
	adcBASE_t *ADC_TPS;/*ADC base address*/
	unsigned int group;/*ADC group number, must be 1 or 2*/
	unsigned int channel;/*ADC channel number, must be the one connected to the DiagOUT pin of TPS65381*/
} SL_Adc_Param_t;


typedef volatile struct SL_Gio_Para
{
	gioPORT_t *gioPort;/*gioPort number, must be gioPORTA or gioPORTB*/
	unsigned int channel;/*GIO channel number, must be the one connected to the EnDrv pin of TPS65381*/
} SL_Gio_Param_t;

typedef enum _WDWindow
{
    CLOSEWDW = 0,
    OPENWDW
} WDWindow;

typedef enum _TestState
{
    TEST_BASIC_SETTINGS = 0,
    TEST_WD_DIAGMODE,
    TEST_WDDIAG_ERRORS,
    TEST_WD_ACTMODE,
    TEST_WDACT_ERRORS,
    IDLE
 } TestState;

 typedef enum _WDErrorState
 {
     TIMEOUT,
     TOKENEARLY,
     SEQERR,
     TOKENERR,
     NOERROR
 }WDErrorState;

 typedef struct _TestStateMachine
 {
         TestState current_state;
         TestState next_state;
         TestState prev_state;
 }TestStateMachine;


#endif /* APPLICATION_TYPES_H_ */
