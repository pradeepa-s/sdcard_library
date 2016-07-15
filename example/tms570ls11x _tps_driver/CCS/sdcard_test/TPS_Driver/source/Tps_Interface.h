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
 *         @file       Tps_Interface.h
 *         @version    2.2.0
 *         @component  TPS
 *         @module     TPS Library
 *         @generator  No Configuration
 *
 *         @brief     File contains the TPS interfacing api's prototypes for\n
 *                    read,write and verifying SPI mapped registers.
 *         @details    todo
 *
 *
 *---------------------------------------------------------------------------------------------------------------------
 * @author  Manoj
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
   |Version       | Date        | Author           |    Change ID       | Description                         |
   |------------: |:-----------:|:----------------:|:------------------:|:------------------------------------|
   |2.2.0         |  20Dec2012  | Manoj R          |    00000000000     | Initial version created             |
*/
#ifndef __TPS_IF_H__
#define __TPS_IF_H__
/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "TPS_Types.h"
#include "TPS_DebugSupport.h"
/*********************************************************
Defines / data types / structs / unions /enums
**********************************************************/
/*********************************************************
Function Prototypes
**********************************************************/
/**
 * @brief   Initialize TPS interface.
 *
 * @description Initialize TPS interface.Updates the global inteface function pointer which can be used to communicate to TPS device.
 *
 * \if imageenabled
 * @image html  TpsIf_Init.jpg
 * @image rtf   TpsIf_Init.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  snd_rcv_data_api function pointer for send and receive data api(application supported)
 *
 * @return      None
 *
 * Example Usage:
 * @code
 *      //SendandRecieveDataTPS is an API of type boolean SendandRecieveDataTPS(const uint16* const u16TransmitData,const uint32 u32TransmitSize,volatile uint16* u16RecieveData,uint32 *u32RecieveSize)
 *       TpsIf_Init(send_recieve_data_TPS);
 * @endcode
 *
 * @entrymode   Diagnostic state of the TPS device.
 * @exitmode    Diagnostic state of the TPS device.
 *
 * @seealso     @ref TPS_DriverInit
 *
 * \if destagenabled
 * @destag      DES_01
 * @reqtag      TPS_SR21,TPS_SR29
 * \endif
 */
void TpsIf_Init(send_recieve_data snd_rcv_data_api);
/**
 * @brief   Write Register Value.
 *
 * @description Seg value given for a TPS Register specified.
 *
 * \if imageenabled
 * @image html  TpsIf_SetRegister.jpg
 * @image rtf   TpsIf_SetRegister.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  TPSReg    offest of Tps register
 * @param [in]  u8value  binary value to write to bit.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //TPS_SW_LOCK is the TPS register being set
 *      blRetVal = TpsIf_SetRegister(TPS_SW_LOCK, (uint8)0x55U);
 * @endcode
 *
 * @entrymode   The TPS state can be anything depends on whether the register being set is accesible in the particular TPS mode
 * @exitmode    same as entry
 *
 * \if destagenabled
 * @destag      DES_02
 * @reqtag      TPS_SR27,TPS_SR28,
 * \endif
 */
boolean TpsIf_SetRegister(const TPS_Registers TPSReg, const uint8 u8value);
/**
 * @brief   Write Register Value and Verify.
 *
 * @description Set given value in the specified register and vrify by reading it back.
 *
 * \if imageenabled
 * @image html  TpsIf_SetRegVerify.jpg
 * @image rtf   TpsIf_SetRegVerify.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  TPSReg    offest of Tps register
 * @param [in]  u8value  binary value to write to bit.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //TPS_SAFETY_CFG_CRC is the TPS register being set
 *      TpsIf_SetRegisterVerify(TPS_SAFETY_CFG_CRC, u8safetycfgcrc);
 * @endcode
 *
 * @entrymode   The TPS state can be anything depends on whether the register being set is accesible in the particular TPS mode
 * @exitmode    same as entry
 *
 *\if destagenabled
 * @destag      DES_03
 * @reqtag      TPS_SR27,TPS_SR28
 * \endif
 */
boolean TpsIf_SetRegisterVerify(const TPS_Registers TPSReg, const uint8 u8value);
/**
 * @brief   Set Register bitfield Value.
 *
 * @description Set Register bitfield vaule in the specified register.
 *
 * \if imageenabled
 * @image html  TpsIf_SetRegisterBitField.jpg
 * @image rtf   TpsIf_SetRegisterBitField.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  TPSReg      offest of Tps register
 * @param [in]  u8bfstart   start of the bitfied(value ranges from 0 to 7).
 * @param [in]  u8length    length of the bitfied to be read.
 * @param [out]  *u8value binary value of the bitfield.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //TPS_SAFETY_CHECK_CTRL is the TPS register
 *       blRetVal = ((TpsIf_SetRegisterBitField(TPS_SAFETY_CHECK_CTRL,
 *               BF_DIAG_EXIT_START, BF_DIAG_EXIT_LENGTH, (uint8)1U)) && blRetVal);
 * @endcode
 *
 * @entrymode   The TPS state can be anything depends on whether the register being set is accesible in the particular TPS mode
 * @exitmode    same as entry
 *
 *
 *\if destagenabled
 * @destag      DES_04
 * @reqtag      TPS_SR27,TPS_SR28
 * \endif
 */
boolean TpsIf_SetRegisterBitField(const TPS_Registers TPSReg, const uint8 u8bfstart,const uint8 u8bflength,const uint8 u8value);
/**
 * @brief   Set Register bitfield Value and verify.
 *
 * @description Set value given in the secifed bitfield of the specified register.
 *
 * \if imageenabled
 * @image html  TpsIf_SetRegisterBitFieldVerify.jpg
 * @image rtf   TpsIf_SetRegisterBitFieldVerify.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  TPSReg      offest of Tps register
 * @param [in]  u8bfstart   start of the bitfied(value ranges from 0 to 7).
 * @param [in]  u8length    length of the bitfied to be read.
 * @param [out]  *u8value binary value of the bitfield.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *           //TPS_SAFETY_FUNC_CFG is the TPS register
 *           blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
 *                  BF_NO_SAFE_TO_START, BF_NO_SAFE_TO_LENGTH, 0U);
 * @endcode
 *
 * @entrymode   The TPS state can be anything depends on whether the register being set is accesible in the particular TPS mode
 * @exitmode    same as entry
 *
 *
 *\if destagenabled
 * @destag      DES_05
 * @reqtag      TPS_SR27,TPS_SR28
 * \endif
 */
boolean TpsIf_SetRegisterBitFieldVerify(const TPS_Registers TPSReg,
        const uint8 u8bfstart, const uint8 u8bflength, const uint8 u8value);
/**
 * @brief   Read Register Value.
 *
 * @description Read the value in the specified register of the TPS device.
 *
 * \if imageenabled
 * @image html  TpsIf_GetRegister.jpg
 * @image rtf   TpsIf_GetRegister.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  TPSReg    offest of Tps register
 * @param [out]  *u8value binary value read from the register.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //TPS_SAFETY_FUNC_CFG is the TPS register
 *       blRetVal = TpsIf_GetRegister(TPS_SAFETY_CHECK_CTRL,
 *               &u8safety_check_ctrl);
 * @endcode
 *
 * @entrymode   anymode of TPS
 * @exitmode    anymode of TPS
 *
 *
 *\if destagenabled
 * @destag      DES_06
 * @reqtag      TPS_SR27,TPS_SR28
 * \endif
 */
boolean TpsIf_GetRegister(const TPS_Registers TPSReg, uint8* u8value);
/**
 * @brief   Read Register bitfield Value.
 *
 * @description Read the value in the appropriate bitfield of specified register.
 *
 * \if imageenabled
 * @image html  TpsIf_GetRegisterBitField.jpg
 * @image rtf   TpsIf_GetRegisterBitField.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  TPSReg      offest of Tps register
 * @param [in]  u8bfstart   start of the bitfied(value ranges from 0 to 7).
 * @param [in]  u8length    length of the bitfied to be read.
 * @param [out]  *u8value binary value of the bitfield.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //TPS_SAFETY_ERR_STAT is the TPS register
 *       blRetVal = TpsIf_GetRegisterBitField(TPS_SAFETY_ERR_STAT,
 *       BF_ERROR_PIN_FAIL_START, BF_ERROR_PIN_FAIL_LENGTH, &u8bitavalue);
 * @endcode
 *
 * @entrymode   anymode of TPS
 * @exitmode    anymode of TPS
 *
 *
 *\if destagenabled
 * @destag      DES_07
 * @reqtag      TPS_SR27,TPS_SR28
 * \endif
 */
boolean TpsIf_GetRegisterBitField(const TPS_Registers TPSReg, const uint8 u8bfstart,
        const uint8 u8bflength, uint8* u8value);
/**
 * @brief   test command parity logic.
 *
 * @description Send a u16Command with wrong parity.
 *
 * \if imageenabled
 * @image html  TpsIf_TestCommandParityLogic.jpg
 * @image rtf   TpsIf_TestCommandParityLogic.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]  *selftest_result   the parameter stores the pass fail status of the test performed.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *     TPS_Test_Result CommandParityTest;
 *     TpsIf_TestCommandParityLogic(&CommandParityTest)
 * @endcode
 *
 * @entrymode   anymode of TPS
 * @exitmode    anymode of TPS
 *
 *
 *\if destag enabled
 * @destag      DES_08
 * @reqtag      TPS_SR319
 * \endif
 */
boolean TpsIf_TestCommandParityLogic(TPS_Test_Result * selftest_result);
/**
 * @brief   test wrong command logic.
 *
 * @description Send a wrong u16Command with correct parity
 *
 * \if imageenabled
 * @image html  TpsIf_TestWrongCommandLogic.jpg
 * @image rtf   TpsIf_TestWrongCommandLogic.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]  *selftest_result   the parameter stores the pass fail status of the test performed.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_Test_Result testwrongcommand;
 *      TpsIf_TestWrongCommandLogic(&testwrongcommand)
 * @endcode
 *
 * @entrymode   anymode of TPS
 * @exitmode    anymode of TPS
 *
 *
 *\if destagenabled
 * @destag      DES_09
 * @reqtag      TPS_SR320
 * \endif
 */
boolean TpsIf_TestWrongCommandLogic(TPS_Test_Result * selftest_result);
/**
 * @brief  test PwmLow Register writes.
 *
 * @description Write a arbitrary value to the PwmLow register, reads back to verify.\n
 *              It effectively tests that the SPI interface is working fine.
 *
 * \if imageenabled
 * @image html  TpsIf_SpiIFTestPwmlow.jpg
 * @image rtf   TpsIf_SpiIFTestPwmlow.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]  *selftest_result   the parameter stores the pass fail status of the test performed.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_Test_Result testSPIIFTest;
 *      TpsIf_SpiIFTestPwmlow(&testSPIIFTest)
 * @endcode
 *
 * @entrymode   anymode of TPS
 * @exitmode    anymode of TPS
 *
 *
 *\if destagenabled
 * @destag      DES_10
 * @reqtag      TPS_SR322
 * \endif
 */
boolean TpsIf_SpiIFTestPwmlow(TPS_Test_Result * selftest_result);
#endif
