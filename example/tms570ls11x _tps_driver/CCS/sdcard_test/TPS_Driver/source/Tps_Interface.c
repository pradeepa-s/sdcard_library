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
 *         @file       TPS_Driver.c
 *         @version    2.2.0
 *         @component  TPS
 *         @module     TPS Library
 *         @generator  No Configuration
 *
 *         @brief      Interfacing API to directly send commands to the TPS device.
 *         @details    The TPS_Interface.c file provides the interface api's to\n
 *                     the TPS.It provides API to set,clear and verify the\n
 *                     registers of the TPS device.
 *
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
#include "TPS_Interface.h"
#include "TPS_Priv.h"
/*********************************************************
 * Local Functions prototypes
 **********************************************************/
static boolean TpsIf_SendCommandOverSPI(uint16 u16Command, uint8* u8Data);

/*********************************************************
 Defines / data types / structs / unions /enums
 **********************************************************/
/** @var const TpsIf_COMMAND_t TpsIf_COMMAND[]
 *   @brief Commands
 *
 *   Tps command list.
 */

static const TpsIf_COMMAND_t TpsIf_COMMAND[32] =
{
    { RD_DEV_REV,               WR_COMMAND_NONE     },   /* 0x00: Tps_DEV_REV            */
    { RD_DEV_ID,                WR_COMMAND_NONE     },   /* 0x01: Tps_DEV_ID             */
    { RD_DEV_STAT,              WR_COMMAND_NONE     },   /* 0x02: Tps_DEV_STAT           */
    { RD_DEV_CFG1,              WR_DEV_CFG1         },   /* 0x03: DEV_CFG1               */
    { RD_DEV_CFG2,              WR_DEV_CFG2         },   /* 0x04: DEV_CFG2               */
    { RD_VMON_STAT_1,           WR_COMMAND_NONE     },   /* 0x05: Tps_VMON_STAT_1        */
    { RD_VMON_STAT_2,           WR_COMMAND_NONE     },   /* 0x06: Tps_VMON_STAT_2        */
    { RD_SAFETY_STAT_1,         WR_COMMAND_NONE     },   /* 0x07: Tps_SAFETY_STAT_1      */
    { RD_SAFETY_STAT_2,         WR_COMMAND_NONE     },   /* 0x08: Tps_SAFETY_STAT_2      */
    { RD_SAFETY_STAT_3,         WR_COMMAND_NONE     },   /* 0x09: Tps_SAFETY_STAT_3      */
    { RD_SAFETY_STAT_4,         WR_COMMAND_NONE     },   /* 0x0A: Tps_SAFETY_STAT_4      */
    { RD_SAFETY_STAT_5,         WR_COMMAND_NONE     },   /* 0x0B: Tps_SAFETY_STAT_5      */
    { RD_SAFETY_ERR_CFG,        WR_SAFETY_ERR_CFG   },   /* 0x0C: Tps_SAFETY_ERR_CFG     */
    { RD_SAFETY_BIST_CTRL,      WR_SAFETY_BIST_CTRL },   /* 0x0D: Tps_SAFETY_BIST_CTRL   */
    { RD_SAFETY_CHECK_CTRL,     WR_SAFETY_CHECK_CTRL},   /* 0x0E: Tps_SAFETY_CHECK_CTRL  */
    { RD_SAFETY_FUNC_CFG,       WR_SAFETY_FUNC_CFG  },   /* 0x0F: Tps_SAFETY_FUNC_CFG    */
    { RD_SAFETY_ERR_STAT,       WR_SAFETY_ERR_STAT  },   /* 0x10: Tps_SAFETY_ERR_STAT    */
    { RD_SAFETY_ERR_PWM_H,      WR_SAFETY_ERR_PWM_H },   /* 0x11: Tps_SAFETY_ERR_PWM_H   */
    { RD_SAFETY_ERR_PWM_L,      WR_SAFETY_ERR_PWM_L },   /* 0x12: Tps_SAFETY_ERR_PWM_L   */
    { RD_SAFETY_PWD_THR_CFG,    WR_SAFETY_PWD_THR_CFG},  /* 0x13: Tps_SAFETY_ERR_PWM_L   */
    { RD_SAFETY_CFG_CRC,        WR_SAFETY_CFG_CRC   },   /* 0x14: Tps_SAFETY_CFG_CRC     */
    { RD_DIAG_CFG_CTRL,         WR_DIAG_CFG_CTRL    },   /* 0x15: Tps_DIAG_CFG_CTRL      */
    { RD_DIAG_MUX_SEL,          WR_DIAG_MUX_SEL     },   /* 0x16: Tps_DIAG_MUX_SEL       */
    { RD_WDT_TOKEN_FDBCK,       WR_WDT_TOKEN_FDBCK  },   /* 0x17: Tps_WDT_TOKEN_FDBCK    */
    { RD_WD_WIN1_CFG,           WR_WD_WIN1_CFG      },   /* 0x18: Tps_WD_WIN1_CFG        */
    { RD_WD_WIN2_CFG,           WR_WD_WIN2_CFG      },   /* 0x19: Tps_WD_WIN2_CFG        */
    { RD_WDT_TOKEN_VALUE,       WR_COMMAND_NONE     },   /* 0x1A: Tps_WDT_TOKEN_VALUE    */
    { RD_WDT_STATUS,            WR_COMMAND_NONE     },   /* 0x1B: Tps_WDG_STATUS         */
    { RD_COMMAND_NONE,          WR_WDT_ANSWER       },   /* 0x1C: Tps_WDT_ANSWER         */
    { RD_SENS_CTRL,             WR_SENS_CTRL        },   /* 0x1D: Tps_SENS_CTRL          */
    { RD_COMMAND_NONE,          WR_SW_LOCK          },   /* 0x1E: Tps_SW_LOCK            */
    { RD_COMMAND_NONE,          WR_SW_UNLOCK        },   /* 0x1F: Tps_SW_UNLOCK          */
};

/*********************************************************
 Local  definitions
 **********************************************************/
/*SAFETYMCUSW 298 S <APPROVED> "Comment_1"*/
static send_recieve_data send_recieve_data_TPS;

/*********************************************************
 Functions
 **********************************************************/
/**
 * @brief   sends command over SPI.
 *
 * @description todo
 *
 * @image html  todo
 * @image rtf   todo
 * @image latex todo
 *
 * @param [in]  u16Command TPS command to be sent on SPI
 * @param [in] *u8Data data output for the SPI command
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
static boolean TpsIf_SendCommandOverSPI(uint16 u16Command, uint8* u8Data)
{
    volatile uint16 u16Content = 0u;
    uint8 u8SPIPreviousStatus = 0U;
    uint32 u32size = 1U;
    boolean blRetVal = FALSE;
    uint16 u16tempcontent = u16Content;
    uint8 u8SAFETY_STAT_4 = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    if (!CHECK_RANGE_RAM_PTR(u8Data))
    {
        blRetVal = FALSE;
        /*SAFETYMCUSW 95 S MR: 11.1,11.4 <APPROVED> Comment_5*/
        TPS_SendDebugText(ERROR,(uint8 *)"TpsIf_SendCommandOverSPI null pointer data",(uint32)blRetVal);

    }
    else
    {
        /*send  and recieve command*/
        if (TRUE
                == send_recieve_data_TPS(&u16Command, 1U, &u16Content,
                        &u32size))
        {
            u16tempcontent = u16Content;
            /*Extract the data*/
            *u8Data = (uint8) (u16tempcontent & TPSIF_DATA_MASK);
            u16Content = 0u;
            if (TRUE
                    == send_recieve_data_TPS(
                            &TpsIf_COMMAND[TPS_SAFETY_STAT_4].Read, 1U,
                            &u16Content, &u32size))
            {
                u16tempcontent = u16Content;
                /*Extract the data*/
                u8SAFETY_STAT_4 = (uint8) (u16tempcontent & TPSIF_DATA_MASK);
                /*Extract the spi transfer status flags byte*/
                u8SPIPreviousStatus = (uint8) ((u16tempcontent
                        & TPSIF_STATUS_FLAG_RESPOSE_MASK) >> 8U);
                if (NO_ERROR
                        == BFU8_GET(u8SAFETY_STAT_4, BF_SPI_ERR_START,
                                BF_SPI_ERR_LENGTH))
                {
                    /*checking whether there were no errors in the previous SPI transfer phase*/
                    if ((MULTIBITKEY_SET
                            == BFU8_GET(u8SPIPreviousStatus,
                                    BF_MULTIBITKEY_START, BF_MULTIBITKEY_LENGTH))
                            && (0u
                                    == BFU8_GET(u8SPIPreviousStatus,
                                            BF_SPI_ERRFLAG_START,
                                            BF_SPI_ERRFLAG_LENGTH)))
                    {
                        blRetVal = TRUE;/*Sth is wrong with the previous SPI transfer*/
                    }
                    else
                    {
                        blRetVal = FALSE;
                        /*SAFETYMCUSW 95 S MR: 11.1,11.4 <APPROVED> Comment_5*/
                        TPS_SendDebugText(ERROR,(uint8 *)"TpsIf_SendCommandOverSPI SPIPreviousStatus error",
                                (uint32) u8SPIPreviousStatus);
                    }
                }
                else
                {
                    blRetVal = FALSE;
                    /*SAFETYMCUSW 95 S MR: 11.1,11.4 <APPROVED> Comment_5*/
                    TPS_SendDebugText(ERROR,(uint8 *)"TpsIf_SendCommandOverSPI SPI ERR  ",
                            (uint32) u8SAFETY_STAT_4);
                    /*SAFETYMCUSW 95 S MR: 11.1,11.4 <APPROVED> Comment_5*/
                    TPS_SendDebugText(ERROR,(uint8 *)"TpsIf_SendCommandOverSPI  u8SPIPreviousStatus is",
                            (uint32) (uint32) u8SPIPreviousStatus);
                }
        }
        else
        {
           blRetVal = FALSE;
           /*SAFETYMCUSW 95 S MR: 11.1,11.4 <APPROVED> Comment_5*/
           TPS_SendDebugText(ERROR,(uint8 *)"TpsIf_SendCommandOverSPI send_recieve_data_TPSR",(uint32)blRetVal);
        }
    }
    else
    {
        blRetVal = FALSE;/*Send data command failed*/
        /*SAFETYMCUSW 95 S MR: 11.1,11.4 <APPROVED> Comment_5*/
        TPS_SendDebugText(ERROR,(uint8 *)"TpsIf_SendCommandOverSPI Send data command failed",
        		(uint32)blRetVal);

    }
}
return blRetVal;
}

void TpsIf_Init(send_recieve_data snd_rcv_data_api)
{
    send_recieve_data_TPS = snd_rcv_data_api;
}

boolean TpsIf_SetRegister(const TPS_Registers TPSReg, const uint8 u8value)
{
    uint8 u8Data = 0U;
    boolean blRetVal = FALSE;
    uint16 u16Command = TpsIf_COMMAND[TPSReg].Write | u8value;

    /*write data to register*/
    if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
    {
        blRetVal = TRUE;
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TpsIf_SetRegisterVerify(const TPS_Registers TPSReg, const uint8 u8value)
{
    uint8 u8Data = 0U;
    boolean blRetVal = FALSE;
    uint16 u16Command = TpsIf_COMMAND[TPSReg].Write | u8value;

    /*write data to register*/
    if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
    {
        u16Command = TpsIf_COMMAND[TPSReg].Read;
        /*read data from register*/
        if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
        {
            if (u8value == u8Data)
            {
                blRetVal = TRUE;
            }
            else
            {
                blRetVal = FALSE;
            }
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return (blRetVal);
}

boolean TpsIf_SetRegisterBitField(const TPS_Registers TPSReg, const uint8 u8bfstart,
        const uint8 u8bflength, const uint8 u8value)
{
    uint16 u16Command = TpsIf_COMMAND[TPSReg].Read;
    uint8 u8Data = 0U;
    boolean blRetVal = FALSE;

    /*get the data in the register*/
    if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
    {
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_3"*/
        BFU8_SET(u8Data, u8value, u8bfstart, u8bflength);
        u16Command = TpsIf_COMMAND[TPSReg].Write | (u8Data);
        /*write data back to the register*/
        if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
        {
            blRetVal = TRUE;
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return (blRetVal);
}

boolean TpsIf_SetRegisterBitFieldVerify(const TPS_Registers TPSReg,
        const uint8 u8bfstart, const uint8 u8bflength, const uint8 u8value)
{
    uint16 u16Command = TpsIf_COMMAND[TPSReg].Read;
    uint8 u8Data = 0U;
    boolean blRetVal = FALSE;
    /*get the data in the register*/
    if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
    {
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_3"*/
        BFU8_SET(u8Data, u8value, u8bfstart, u8bflength);
        u16Command = TpsIf_COMMAND[TPSReg].Write | (u8Data);
        /*write data back to the register*/
        if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
        {
            blRetVal = TRUE;
            u16Command = TpsIf_COMMAND[TPSReg].Read;
            /*read the data back and verify whether it has been correctly written*/
            if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
            {
                if (u8value == BFU8_GET(u8Data,u8bfstart,u8bflength))
                {
                    blRetVal = TRUE;
                }
                else
                {
                    blRetVal = FALSE;
                }
            }
            else
            {
                blRetVal = FALSE;
            }
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TpsIf_GetRegister(const TPS_Registers TPSReg, uint8* u8value)
{
    uint16 u16Command = TpsIf_COMMAND[TPSReg].Read;
    uint8 u8Data = 0U;
    boolean blRetVal = FALSE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    if (!CHECK_RANGE_RAM_PTR(u8value))
    {
        blRetVal = FALSE;
    }
    else
    {
        if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
        {
            blRetVal = TRUE;
            *u8value = u8Data;
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    return blRetVal;
}

boolean TpsIf_GetRegisterBitField(const TPS_Registers TPSReg, const uint8 u8bfstart,
        const uint8 u8bflength, uint8* u8value)
{
    uint16 u16Command = TpsIf_COMMAND[TPSReg].Read;
    uint8 u8Data = 0U;
    boolean blRetVal = FALSE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    if (!CHECK_RANGE_RAM_PTR(u8value))
    {
        blRetVal = FALSE;
    }
    else
    {
        if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
        {
            blRetVal = TRUE;
            *u8value = BFU8_GET(u8Data,u8bfstart,u8bflength);
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    return blRetVal;
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "Comment_4"*/
boolean TpsIf_TestCommandParityLogic(TPS_Test_Result * selftest_result)
{
    uint16 u16Command = RD_WRONG_PARITY;
    boolean blRetVal = FALSE;
    uint8 u8Data = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    if (!CHECK_RANGE_RAM_PTR(selftest_result))
    {
        blRetVal = FALSE;
    }
    else
    {
        /* the transfer should fail as the parity is wrong the command being sent to TPS*/
        if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
        {
            *selftest_result = TEST_FAIL;
        }
        else
        {
        	*selftest_result = TEST_PASS;
        }
        blRetVal = TRUE;
    }
    return blRetVal;
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "Comment_4"*/
boolean TpsIf_TestWrongCommandLogic(TPS_Test_Result * selftest_result)
{
    uint8 u8Data = 0U;
    uint16 u16Command = RD_WRONG_COMMAND;
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    if (!CHECK_RANGE_RAM_PTR(selftest_result))
    {
        blRetVal = FALSE;
    }
    else
    {
        /* the transfer should fail as the parity is wrong the command being sent to TPS*/
        if (TRUE == TpsIf_SendCommandOverSPI(u16Command, &u8Data))
        {
            *selftest_result = TEST_FAIL;
        }
        else
        {
            *selftest_result = TEST_PASS;
        }
        blRetVal = TRUE;
    }
    return blRetVal;
}

/*SAFETYMCUSW 61 D MR: 8.10,8.11 <APPROVED> "Comment_4"*/
boolean TpsIf_SpiIFTestPwmlow(TPS_Test_Result * selftest_result)
{
    uint8 u8Data =0U, pass = 1U;
    boolean blRetVal = TRUE;
    uint8 u8bckup_safety_err_pwm_l = 0U;
    uint8 u8pwmi = 0U; /* iterator for iterating through the pwm value array*/
    uint8 u8Apwmvalue[4] =
    { 0xFFu, 0xAAu, 0x00u, 0x55u }; /* various values for pwm being tested*/

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_2"*/
    if (!CHECK_RANGE_RAM_PTR(selftest_result))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_ERR_PWM_L,
                &u8bckup_safety_err_pwm_l)) && blRetVal);
        /*write to pwmlow register and read the value back to verify that the writes to pwmlow register are working correctly*/
        for (u8pwmi = 0U; ((u8pwmi < (sizeof(u8Apwmvalue) / sizeof(uint8)))&&((uint8)TRUE == pass)); u8pwmi++)
        {
            if (TRUE
                    == TpsIf_SendCommandOverSPI(
                            WR_SAFETY_ERR_PWM_L | u8Apwmvalue[u8pwmi], &u8Data))
            {
                if (TRUE
                        == TpsIf_SendCommandOverSPI(RD_SAFETY_ERR_PWM_L,
                                &u8Data))
                {
                    if (u8Apwmvalue[u8pwmi] == (u8Data))
                    {
                        *selftest_result = TEST_PASS;
                    }
                    else
                    {
                        *selftest_result = TEST_FAIL;
                        break;
                    }
                }
                else
                {
                    *selftest_result = TEST_FAIL;
                    pass = 0U;
                }
            }
            else
            {
                *selftest_result = TEST_FAIL;
                pass = 0U;
            }
        }
        /*restore pwm registers*/
        blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_ERR_PWM_L,
                u8bckup_safety_err_pwm_l)) && blRetVal);
    }
    return blRetVal;
}

/*Comment_1:
 * "Not a MISRA-C:2004 violation"*/

/*Comment_2:
 * "Reason -  This is an advisory by MISRA.Verified validity of operation by review"*/

/*Comment_3:
 * "Assignment in an expression is valid here"*/

/*Comment_4:
 * "This function will be called by application so not static"*/

/*Comment_5:
 * "Needed, to avoid implicit type-casting"*/
