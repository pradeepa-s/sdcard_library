/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 TEXAS INSTRUMENTS INCORPORATED PROPRIETARY INFORMATION
 *
 *                 Property of Texas Instruments, Unauthorized reproduction and/or distribution
 *                 is strictly prohibited.  This product  is  protected  under  copyright  law
 *                 and  trade  secret law as an  unpublished work.
 *                 (C) Copyright Texas Instruments.  All rights reserved.
 *
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *  ------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         @File       :  Interface_Support.c
 *         @version    :  $file_version$
 *         Component   :  application
 *         Module      :  application
 *         Generator   :  -
 *
 *         @brief      : Provides Interfacing API for TPS driver to communicate to TPS device.These API's are used by
 *                       the Tps_Interface.c
 *         @detail     :  todo
 *
 *---------------------------------------------------------------------------------------------------------------------
 * Author:  Manoj R
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
 * Version        Date         Author               Change ID        Description
 *---------------------------------------------------------------------------------------------------------------------
 * 0.0.1          20Dec2012    Manoj R              00000000000      Initial version created
 ********************************************************************************************************************/

/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "Interface_Support.h"
#include "Application_Types.h"
#include "Board_Config.h"
#include "sci.h"


/*********************************************************
* External global variables
**********************************************************/
SL_Spi_Param_t gSpi_Parameter = SPI_TPS_DEFAULTS;
SL_Adc_Param_t gAdc_Parameter = ADC_TPS_DEFAULTS;
SL_Spi_Param_t *Spi_Param_Handle = &gSpi_Parameter;
SL_Adc_Param_t *Adc_Param_Handle = &gAdc_Parameter;
/*used for testing purposes*/
uint16 u16PrevCommand;

/*********************************************************
* Local Functions prototypes
**********************************************************/
/*********************************************************
Defines / data types / structs / unions /enums
**********************************************************/
/**
 * @brief   get the ADC sampled data of the Diagnostic Pin.
 *
 * @description todo
 *
 * @image html  todo
 * @image rtf   todo
 * @image latex todo
 *
 * @param [in]  *u16Data received data pointe.After the execution of the API this will hold the sampled data.

 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      todo
 * @endcode
 *
 * @entrymode   todo
 * @exitmode    todo
 *
 * @seealso     todo
 *
 * **Note: todo**
 *
 * @destag      todo
 * @reqtag      todo
 */
boolean TPSDiagosticPinSampledData(uint16* u16Data)
{
    adcData_t adcdata;
    uint32 count;
    boolean blRetVal = TRUE;

    adcStartConversion(Adc_Param_Handle->ADC_TPS, Adc_Param_Handle->group);
    while(!adcIsConversionComplete(Adc_Param_Handle->ADC_TPS, Adc_Param_Handle->group));
    count = adcGetData(Adc_Param_Handle->ADC_TPS, Adc_Param_Handle->group, &adcdata);
    if((1U == count)&&(0U == adcdata.id))
    {
        *u16Data = adcdata.value;
        blRetVal = TRUE;
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;;
}
/**
 * @brief   get the ADC sampled data of the Diagnostic Pin(Error case which returns FALSE).
 *
 * @description todo
 *
 * @image html  todo
 * @image rtf   todo
 * @image latex todo
 *
 * @param [in]  *u16Data received data pointe.After the execution of the API this will hold the sampled data.

 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      todo
 * @endcode
 *
 * @entrymode   todo
 * @exitmode    todo
 *
 * @seealso     todo
 *
 * **Note: todo**
 *
 * @destag      todo
 * @reqtag      todo
 */
boolean TPSDiagosticPinSampledData_ERR(uint16* u16Data)
{
    return FALSE;
}

/**
 * @brief   send command  and recieve response from TPS device.
 *
 * @description todo
 *
 * @image html  todo
 * @image rtf   todo
 * @image latex todo
 *
 * @param [in]  *u16RecieveData received data pointer
 * @param [in]  u32RecieveSize data received
 * @param [in]  *u16TransmitData Data to be sent on SPI
 * @param [in]  u32TransmitSize data to be sent
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      todo
 * @endcode
 *
 * @entrymode   todo
 * @exitmode    todo
 *
 * @seealso     todo
 *
 * **Note: todo**
 *
 * @destag      todo
 * @reqtag      todo
 */
boolean SendandRecieveDataTPS(const uint16* const u16TransmitData,const uint32 u32TransmitSize,volatile uint16* u16RecieveData,uint32 *u32RecieveSize)
{
    boolean blRetVal = TRUE;
    volatile uint32 temp_read;

    if(1U != u32TransmitSize)
    {
        blRetVal = FALSE;
    }
    else
    {
#if (SPI_COMPATIBILITY_MODE == 0)
        mibspiSetData(Spi_Param_Handle->SPI_TPS,0,(uint16*)u16TransmitData);
        mibspiTransfer(Spi_Param_Handle->SPI_TPS, 0);
        while(mibspiIsTransferComplete(Spi_Param_Handle->SPI_TPS, 0) != 1);
        *u32RecieveSize = 1;
        if (0U
                != mibspiGetData(Spi_Param_Handle->SPI_TPS, 0,
                        (uint16*) u16RecieveData))
        {
            blRetVal = FALSE;
        }
        else
        {
            blRetVal = TRUE;
        }
#else
        (Spi_Param_Handle->SPI_TPS)->DAT1 = 0x04FE0000U | *u16TransmitData;
        while(((Spi_Param_Handle->SPI_TPS)->FLG & 0x100)!=0x100);
        *u16RecieveData = ((Spi_Param_Handle->SPI_TPS)->BUF);
        (Spi_Param_Handle->SPI_TPS)->FLG = 0x300;
        temp_read=(Spi_Param_Handle->SPI_TPS)->FLG;
        blRetVal = TRUE;
#endif
    }

    return blRetVal;
}


/**
 * @brief   send command  and recieve response from TPS device(A FAILURE CASE which ).API is used for testing purposes in the test suite.
 *
 * @description todo
 *
 * @image html  todo
 * @image rtf   todo
 * @image latex todo
 *
 * @param [in]  *u16RecieveData received data pointer
 * @param [in]  u32RecieveSize data received
 * @param [in]  *u16TransmitData Data to be sent on SPI
 * @param [in]  u32TransmitSize data to be sent
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      todo
 * @endcode
 *
 * @entrymode   todo
 * @exitmode    todo
 *
 * @seealso     todo
 *
 * **Note: todo**
 *
 * @destag      todo
 * @reqtag      todo
 */
boolean SendandRecieveDataTPS_ERR(const uint16* const u16TransmitData,const uint32 u32TransmitSize,volatile uint16* u16RecieveData,uint32 *u32RecieveSize)
{
    boolean blRetVal = FALSE;

    /* if the command is RD_WRONG_PARITY return TRUE*/
    if(*u16TransmitData == (uint16)0x0D00U)
    {
        blRetVal = TRUE;
        u16PrevCommand = *u16TransmitData;
    }
    /* if the command is RD_WRONG_COMMAND return TRUE*/
    else if(*u16TransmitData == (uint16)0x2C00U)
    {
        blRetVal = TRUE;
        u16PrevCommand = *u16TransmitData;
    }
    /* if the command is RD_SAFETY_STAT_4 return TRUE*/
    else if((*u16TransmitData == 0xA500U)&&((u16PrevCommand == 0x2C00U)||(u16PrevCommand == 0x0D00U)))
    {
        blRetVal = TRUE;
        *u16RecieveData = 0xA000;
        *u32RecieveSize = 1;
        u16PrevCommand = 0x0000;
    }
    return blRetVal;
}


/**
 * @brief   Send debug text API.
 *
 * @description The API can be used by TPS driver to send the debug text buffer.The Application\n
 *              should appropriately make use of the debug information.
 *
 * @image html  todo
 * @image rtf   todo
 * @image latex todo
 *
 * @param [in]  *u8textbuf pointer to text buffer
 * @param [in]  u32textlength length of the text buffer
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      todo
 * @endcode
 *
 * @entrymode   todo
 * @exitmode    todo
 *
 * @seealso     todo
 *
 * **Note: todo**
 *
 * @destag      todo
 * @reqtag      todo
 */
boolean SendDebugTextTPS(const uint8 * const u8textbuf,const uint32 u32textlength)
{
    boolean blRetVal = TRUE;
    sciSend(scilinREG, u32textlength, (uint8*)u8textbuf);
    return blRetVal;
}

/**
 * @brief   Send debug text API.
 *
 * @description The API can be used by TPS driver to send the debug text buffer.The Application\n
 *              should appropriately make use of the debug information.
 *
 * @image html  todo
 * @image rtf   todo
 * @image latex todo
 *
 * @param [in]  *u8textbuf pointer to text buffer
 * @param [in]  u32textlength length of the text buffer
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      todo
 * @endcode
 *
 * @entrymode   todo
 * @exitmode    todo
 *
 * @seealso     todo
 *
 * **Note: todo**
 *
 * @destag      todo
 * @reqtag      todo
 */
boolean SendDebugTextTPS_ERR(const uint8 * const u8textbuf,const uint32 u32textlength)
{
    return FALSE;
}
