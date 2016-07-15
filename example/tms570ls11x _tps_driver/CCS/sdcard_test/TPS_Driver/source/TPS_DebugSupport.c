/**
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
 *         @File       :  TPS_DebugSupport.c
 *         @version    :  2.2.0
 *         Component   :  TPS
 *         Module      :  TPS Driver
 *         Generator   :  -
 *
 *         @brief      : Provides debug support API used by TPS driver can also be used by application.
 *         @detail     :  todo
 *
 *---------------------------------------------------------------------------------------------------------------------
 * @author  Manoj
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
   |Version         | Date        | Author           |    Change ID       | Description                 |
   |---------------:|:-----------:|:----------------:|:------------------:|:----------------------------|
   |2.2.0|  20Dec2012  | Manoj R          |    00000000000     | Initial version             |
*/

/* Comments regarding various justificatios for deviation from MISRA-C code guidelines
 * are provided at the bottom of the file */

/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "TPS_DebugSupport.h"
/*********************************************************
 Local  definitions
 **********************************************************/
  /*SAFETYMCUSW 298 S MR:- <APPROVED> "Comment_1"*/
static SendDebugText SendDebugText_TPS;
/*********************************************************
* External global variables
**********************************************************/

/*********************************************************
* Local Functions prototypes
**********************************************************/
/*********************************************************
Defines / data types / structs / unions /enums
**********************************************************/
/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "Comment_2"*/
void TPS_DebugInit(SendDebugText SendTPSDebugText)
{
    SendDebugText_TPS = SendTPSDebugText;
}

boolean TPS_SendDebugText(TPS_DebugSeverity severity, const uint8 * const u8buftext,
        uint32 u32value)
{
    //const uint8 txtCRLF2[3] = { '\r', '\n', '\0' };
    //uint8 u8valbuf[20] = {0U};
    boolean blRetVal = TRUE;

    if (NULL == SendDebugText_TPS)
    {
        blRetVal = FALSE;
    }
    else
    {
        switch(severity)
        {

            case DEBUG:
#if (DEBUG_ENABLE == 1)
                SendDebugText_TPS(u8buftext,
                        (uint32) strlen(u8buftext));
                sprintf(u8valbuf,"  %x",u32value);
                strcat(u8valbuf,txtCRLF2);
                SendDebugText_TPS(u8valbuf,
                        (uint32) strlen(u8valbuf));

                blRetVal = TRUE;
#else
                blRetVal = FALSE;
#endif
                break;
            case ERROR:
#if (ERROR_ENABLE == 1)
                SendDebugText_TPS(u8buftext,
                        (uint32) strlen(u8buftext));
                sprintf(u8valbuf,"  %x",u32value);
                strcat(u8valbuf,txtCRLF2);
                SendDebugText_TPS(u8valbuf,
                        (uint32) strlen(u8valbuf));
                blRetVal = TRUE;
#else
                blRetVal = FALSE;
#endif
                break;
            case INFO:
#if (DEBUG_ENABLE == 1)
                SendDebugText_TPS(u8buftext,
                        (uint32) strlen(u8buftext));
                SendDebugText_TPS(txtCRLF2,
                        (uint32) strlen(txtCRLF2));
                blRetVal = TRUE;
#else
                blRetVal = FALSE;
#endif
                break;
            default:
                blRetVal = FALSE;
                break;
        }

}
    return blRetVal;
}

/*Comment_1:
 * "Reason - The pointer is updated by application by doing an TPS_DebugInit"*/

/*Comment_2:
 * "This function will be called by application so not static"*/
