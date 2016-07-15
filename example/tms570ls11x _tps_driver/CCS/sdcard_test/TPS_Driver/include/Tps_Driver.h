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
 *         @file       Tps_Driver.h
 *         @version    2.2.0
 *         @component  TPS
 *         @module     TPS Driver
 *         @generator  No Configuration
 *
 *         @brief      This files contains the configuration items for the the TPS Driver.
 *         @details    The TPS_Driver.c file provides the driver level api
 *                     prototypes.These API's should help the application\n
 *                     in setting up proper operation mode for the TPS device.\n
 *                     This single file needs to be included by the application\n
 *                     in order to use the TPS Driver.
 *
 *
 *---------------------------------------------------------------------------------------------------------------------
 * @author  Manoj
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
 |Version       | Date         | Author           |    Change ID       | Description                 |
 |------------: |:------------:|:----------------:|:------------------:|:----------------------------|
 |2.2.0         | 20Dec2012    | Manoj R          |    00000000000     | Initial version             |
 */
#ifndef __TPS_DRV_H__
#define __TPS_DRV_H__
/***************************************************************
 * INCLUDE FILES
 ****************************************************************/
#include "TPS_Interface.h"
#include "TPS_Types.h"
#include "TPS_DebugSupport.h"
#include "reg_bitdefn_esm.h"
#include <string.h>
/*********************************************************
 Defines / data types / structs / unions /enums
 **********************************************************/

/*********************************************************
 Function Prototypes
 **********************************************************/
/**
 * @brief  API to set up  and initialize TPS driver.
 *
 *
 * @description The single API can be used to set up all the required parameters of the TPS statemachine\n
 *              and configure all settings.
 *
 * \if imageenabled
 * @image html  TPS_DriverInit.jpg
 * @image rtf   TPS_DriverInit.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]   init_struct  pointer to the initialization structure which which is passed by the application.

 * @return      TRUE : no errors.
 *              FALSE: Errors in execution.
 *
 * Example Usage:
 * @code
 * TPS_Initilization_struct initstruct;
 * memcpy(&initstruct.AMUXVoltageRailLimits,
 *        &TPS65381Q_VoltageLimits,
 *        sizeof(TPS65381Q_VoltageLimits));
 * initstruct.GetTPSDiagPinSampledValue =
 *        TPSDiagosticPinSampledData;
 * initstruct.blNRES_Monitoring = TRUE;
 * initstruct.blbist_at_startup = TRUE;
 * initstruct.blenable_drv_op = TRUE;
 * \\Error pin monitoring is disabled here,The Halcogen startup toggles the error pin,If error monitoring is
 * \\enabled then appropriate settings has to be made to error monitoring(PWM low width) width
 * \\so that the error monitoring does not reset the TPS device.
 * initstruct.blerror_pin_monitoring = FALSE;
 * initstruct.blignition_power_latch = FALSE;
 * initstruct.blsafe_state_timeout = TRUE;
 * initstruct.dev_configreg_settings.blMASK_VBAT_OV = TRUE;
 * initstruct.dev_configreg_settings.blMASK_VDD1_UV = TRUE;
 * initstruct.dev_configreg_settings.blPOST_RUN_RST = TRUE;
 * initstruct.dev_configreg_settings.blVDD3_5_OT = TRUE;
 * initstruct.dev_configreg_settings.blVDD5_OT = TRUE;
 * initstruct.error_pin_monitoring_mode = ERROR_PIN_MODE;
 * initstruct.error_pin_pwm_width.u8PWM_HIGH = 8;
 * \\the min error pin low dyuration in 204.8 us
 * initstruct.error_pin_pwm_width.u8PWM_LOW = 40;
 * initstruct.send_recieve_data_TPS = SendandRecieveDataTPS;
 * initstruct.u8pwd_threshhold_value = 5;
 * initstruct.u8safe_lock_threshhold = 5;
 * \\timeout duration in 198 ns
 * initstruct.u8safe_state_timeout_scaler = 4;
 *      TPS_DriverInit(&initstruct);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @reg TpsIf_Init
 *
 *\if destagenabled
 * @destag      DES_11
 * @reqtag      TPS_SR21
 * \endif
 */
boolean TPS_DriverInit(const TPS_Initilization_struct* const init_struct);
/**
 * @brief  API to set up  and initialize TPS watchdog.
 *
 *
 * @description The  API can be used to set up all the required parameters for the configuring the TPS watchdog.
 *
 * **Note: calling of this API immediately starts new watchdog sequence starting  with close window and applications should start serving the watcdog appropriately**
 * **Note: if watcdog reset is enabled then on reaching watcdog fail count of 7 the TPS device will move to reset state reset resetting the MCU**
 *
 * \if imageenabled
 * @image html  TPS_DriverInit.jpg
 * @image rtf   TPS_DriverInit.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param [out]   watchdogconfig  pointer to the watchdog configuration structure which which is passed by the application.

 * @return      TRUE : no errors.
 *              FALSE: Errors in execution.
 *
 * Example Usage:
 * @code
 * TPS_WatchdoConfiguration watchdogconfig;
 * //Initialize watchdog and open a new sequence run
 * watchdogconfig.u8TokenFDBK = 0;
 * watchdogconfig.u8TokenSeed = 0;
 * watchdogconfig.blwatchdog_reset = FALSE;
 * // approx 40ms
 * watchdogconfig.u8open_windowtime_scaler =
 *         (uint8) (OPEN_WINDOW_CONFIG / 0.55 - 1);
 * //approx 15 ms
 * watchdogconfig.u8close_windowtime_scaler =
 *         (uint8) (CLOSE_WINDOW_CONFIG / 0.55 - 1);
 * watchdogconfig.watchdog_mode = QANDA_MODE;
 * TPS_WatchdogInit(&watchdogconfig);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 *\if destagenabled
 * @destag      DES_12
 * @reqtag      TPS_SR45,TPS_SR44,TPS_SR54
 * \endif
 */
boolean TPS_WatchdogInit(const TPS_WatchdoConfiguration *const watchdogconfig);
/**
 * @brief  sets the safety check control register flags.
 *
 * @description Write to the various flags of the safety check control register to enable or disable a particular safety mechanism.
 *
 *
 * \if imageenabled
 * @image html  TPS_ConfigureSafetyCheckControl.jpg
 * @image rtf   TPS_ConfigureSafetyCheckControl.jpg
 * @image latex todo
 * \endif
 * @ingroup CRC_API
 *
 * @param [in]  safety_check The Safety Check parameter(a flag in  safety check control register) to be enabled of disabled.
 * @param [in]  blstate Enable or disable the safety check control flag.

 *
 * @return      TRUE : no errors.
 *              FALSE: Errors in execution.
 *
 * Example Usage:
 * @code
 *      TPS_ConfigureSafetyCheckControl(CFG_CRC, FALSE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 *
 * \if destagenabled
 * @destag      DES_13
 * @reqtag      TPS_SR65,TPS_SR66,TPS_SR67,TPS_SR68,TPS_SR69,TPS_SR70,TPS_SR71
 * \endif
*/
boolean TPS_ConfigureSafetyCheckControl(TPS_SafetyCheckControl safety_check,
        boolean blstate);
/**
 * @brief  Enable or disable the safe state timeout.
 *
 * @description Writes to the safety function control register so as safe state time out.
 *
 * \if imageenabled
 * @image html  TPS_ConfigureSafeStateTimeout.jpg
 * @image rtf   TPS_ConfigureSafeStateTimeout.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  blsafe_state_timeout Enabled: After device error count limit has reached the TPS transitions to the Reset State\n
 *                    Disabled: The TPS remains locked to the safe state even after the device error count has been reached.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //disable safe state timeout
 *      TPS_ConfigureSafeStateTimeout(FALSE)
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso @ref TPS_SetSafeStateTimoutDuration,@ref TPS_SetSafeStateTimoutDuration
 *
 *
 * \if destagenabled
 * @destag      DES_14
 * @reqtag      TPS_SR80
 * \endif
*/
boolean TPS_ConfigureSafeStateTimeout(boolean blsafe_state_timeout);
/**
 * @brief  Set Safe State time out duration.
 *
 * @description Writes to the safety error configuration register so as to set safe state time out duration.Afer each error the device enters safe state from the active state\n
 *              It remains in safe state until this time out duration before trasiting to the reset state.(when the device error count is lesser than the
 *              configured threshold value.
 *
 * \if imageenabled
 * @image html  TPS_SetSafeStateTimoutDuration.jpg
 * @image rtf   TPS_SetSafeStateTimoutDuration.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  u8SafeStateTimoutDuration_scaler  scaler value which decides the safe state timeout duration.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_SetSafeStateTimoutDuration(3);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso @ref TPS_ConfigureSafeStateTimeout
 *
 *
 * \if destagenabled
 * @destag      DES_15
 * @reqtag      TPS_SR18,TPS_SR19
 * \endif
*/
boolean TPS_SetSafeStateTimoutDuration(uint8 u8SafeStateTOtDuration_scaler);
/**
 * @brief  Set Error signal  monitoring mode.
 *
 * @description Writes to the safety function control register so as to set MCU error signal monitoring mode.
 *
 * \if imageenabled
 * @image html  TPS_ConfigureErrorMonitoring.jpg
 * @image rtf   TPS_ConfigureErrorMonitoring.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  monitoring_mode parameter indicating the PWM mode or error pin monitoring mode.
 * @param [in]  pwm the scaler values for the register PWM_LOW and PWM_HIGH registers\n
 *              which decide the pwm widths for the error monitoring.
 ***Note:when the mode is ERROR_PIN_MODE then the pwm_high width is not considered only the pwm_low width is used to set the pwm low duration.*
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ERRORPinPWMWidthScaler pwm;
 *      pwm.u8PWM_LOW = 0U;
 *      //PWM high is irrelavant since ERROR_PIN_MODE is being used.
 *      TPS_ConfigureErrorMonitoring(ERROR_PIN_MODE, pwm)
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 *
 *
 * \if destagenabled
 * @destag      DES_16
 * @reqtag      TPS_SR55,TPS_SR56,TPS_SR81
 * \endif
*/
boolean TPS_ConfigureErrorMonitoring(TPS_ErrorPinMonitoringMode monitoring_mode,
        TPS_ERRORPinPWMWidthScaler pwm);
/**
 * @brief  Set Watchdog configuration.
 *
 * @description Sets the watchdog configuration mode to either Q&A mode or the WDTI mode.
 *
 * **Note: Configuring watchdog in the WDTI mode will disable the error pin monitoring**
 *
 * \if imageenabled
 * @image html  TPS_SetWatchdogMode.jpg
 * @image rtf   TPS_SetWatchdogMode.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param [in]  watchdog_configuration parameter type of watchdog to be configured(Q&A or WDTI).
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //watchdog mode can be either QANDA_MODE or it can be WDTI mode
 *      TPS_SetWatchdogMode(QANDA_MODE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_17
 * @reqtag      TPS_SR45,TPS_SR56
 * \endif
*/
boolean TPS_SetWatchdogMode(TPS_WatchdogMode watchdogmode);
/**
 * @brief  configure watchdog windows.
 *
 * @description sets the scaling values which decide the watchdog CLOSE and OPEN window duration.
 * **Note: calling of this API immediately starts new watchdog sequence starting  with close window and applications should start serving the watcdog appropriately**
 *
 *
 * \if imageenabled
 * @image html  TPS_ConfigureWatchdogWindows.jpg
 * @image rtf   TPS_ConfigureWatchdogWindows.jpg
 * @image latex todo
 * \endif
* @ingroup WD_SUPPORT
 *
 * @param [in]  watchdog_windows OPEN or CLOSE window to be configured.
 * @param [in]  u8windowtime_scaler scaler value which has to be configured\n
 *              to WIN1/WIN2 registers which decides the OPEN/CLOSE window.
 * **Note: Watchdog open window time scaler(range 0 - 0x7F).minimum open window   duration is .55ms maximum is 70.4ms**
 * **Note: Watchdog close window time scaler(range 0 - 01F).minimum open window duration is .55ms maximum is 17.6ms**
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ConfigureWatchdogWindows(OPEN_WINDOW,
 *                               (uint8) (40 / 0.55 - 1))
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso @ref WD_SUPPORT
 *
 *
 * \if destagenabled
 * @destag      DES_18
 * @reqtag      TPS_SR58,TPS_SR59,TPS_SR56,TPS_SR52
 * \endif
*/
boolean TPS_ConfigureWatchdogWindows(TPS_WatchdogWindows watchdog_windows,
        uint8 u8windowtime_scaler);
/**
 * @brief  enable/disable watchdog reset
 *
 * @description enables or disable the watchdog reset.Should be used during the configuration of the Watchdhog.
 * **Note: if watcdog reset is enabled then on reaching watcdog fail count of 7 the TPS device will move to reset state reset resetting the MCU**
 *
 * \if imageenabled
 * @image html  TPS_ConfigureWatchdogReset.jpg
 * @image rtf   TPS_ConfigureWatchdogReset.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param [in]  blreset_enable parameter to enable/disable the watchdog reset.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ConfigureWatchdogReset(TRUE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_19
 * @reqtag      TPS_SR46,TPS_SR56
 * \endif
*/
boolean TPS_ConfigureWatchdogReset(boolean blreset_enable);
/**
 * @brief Update active Watchdog Token.
 *
 * @description Updates the global variable which holds the current token\n
 *              value with currently active token in the TPS device.
 *
 * \if imageenabled
 * @image html  TPS_UpdateActiveWDToken.jpg
 * @image rtf   TPS_UpdateActiveWDToken.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_UpdateActiveWDToken();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active state
 * @exitmode    TPS in Diagnostic/Active state
 *
 * @seealso     @ref WD_SUPPORT
 *
 *
 * \if destagenabled
 * @destag      DES_20
 * @reqtag      TPS_SR56
 * \endif
*/
boolean TPS_UpdateActiveWDToken(void);
/**
 * @brief  API to send watchdog response to TPS.
 *
 * @description The API sends the requested watchdog response to the TPS device\n
 *              The response sent is based on the currently active token.At the\n
 *              start of every watchdog window API TPS_UpdateActiveWDToken has\n
 *              to be called by the application so as update the global \n
 *              variable(active token) with the currently active token in\n
 *              the TPS Device.
 *
 * \if imageenabled
 * @image html  TPS_SendWdgResponse.jpg
 * @image rtf   TPS_SendWdgResponse.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param[in] wd_response    WD_TOKEN_RESP0 close window response\n
 *                           WD_TOKEN_RESP1 open window response\n
 *                           WD_TOKEN_RESP2 open window response\n
 *                           WD_TOKEN_RESP3 open window response\n
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_SendWdgResponse(WD_TOKEN_RESP0);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active state
 * @exitmode    TPS in Diagnostic/Active state
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_21
 * @reqtag      TPS_SR60,TPS_SR56
 * \endif
*/
boolean TPS_SendWdgResponse(TPS_WDResponse wd_response);
/**
 * @brief  The API provides watchdog failure count information.
 *
 * @description The API provides failure status information of the watchdog.
 *
 * \if imageenabled
 * @image html  TPS_GetWatchdogFailureStatus.jpg
 * @image rtf   TPS_GetWatchdogFailureStatus.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param[in] *blFailureStatus if TRUE the watchdog failure has ocurred
 *                             if False no watchdog failure

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      boolean blWDFailureStatus;
 *      TPS_GetWatchdogFailureStatus(&blWDFailureStatus)
 * @endcode
 *
 * @seealso     @ref WD_SUPPORT
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_22
 * @reqtag      TPS_SR50,TPS_SR56,TPS_SR44,TPS_SR48
 * \endif
*/
boolean TPS_GetWatchdogFailureStatus(boolean* blFailureStatus);
/**
 * @brief  The API clears the watchdog fail status bit in the register SAFETY_ERR_STAT TPS register.
 *
 * @description The API clears the watchdog fail status bit in the register SAFETY_ERR_STAT TPS register.
 *
 * \if imageenabled
 * @image html  TPS_ClearWatchdogFailureStatusFlag.jpg
 * @image rtf   TPS_ClearWatchdogFailureStatusFlag.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ClearWatchdogFailureStatusFlag();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_23
 * @reqtag      TPS_SR49,TPS_SR56,TPS_SR44
 * \endif
*/
boolean TPS_ClearWatchdogFailureStatusFlag(void);
/**
 * @brief  The API provides watchdog answer count.
 *
 * @description The API provides the number of answers received\n
 *              for the current active watchdog window.
 *
 * \if imageenabled
 * @image html  TPS_GetWatchdogAnswerCount.jpg
 * @image rtf   TPS_GetWatchdogAnswerCount.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param[in] *u8AnswerCount answer count indicating the no of answers\n
 *            Received by the TPS device till now for the current\n
 *            window.

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      uint8 u8wdasnwercount;
 *      TPS_GetWatchdogAnswerCount(&u8wdasnwercount);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active state
 * @exitmode    TPS in Diagnostic/Active state
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_24
 * @reqtag      TPS_SR324
 * \endif
*/
boolean TPS_GetWatchdogAnswerCount(uint8* u8AnswerCount);
/**
 * @brief  The API provides failure status information of the watchdog.
 *
 * @description The API provides failure status information of the watchdog.
 *
 * \if imageenabled
 * @image html  TPS_GetWatchdogErrorType.jpg
 * @image rtf   TPS_GetWatchdogErrorType.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param[in] *WDErrorStatus structure which stores the information regarding the\n
 *                           various watchdog failures.
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_WatchdogErrorStatus WDErrorStatus;
 *      TPS_GetWatchdogErrorType(&WDErrorStatus);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_25
 * @reqtag      TPS_SR51
 * \endif
*/
boolean TPS_GetWatchdogErrorType(TPS_WatchdogErrorStatus *WDErrorStatus);
/**
 * @brief  The API provides watchdog failure count information.
 *
 * @description The API provides watchdog failure count information.
 *
 * \if imageenabled
 * @image html  TPS_GetWatchdogFailCount.jpg
 * @image rtf   TPS_GetWatchdogFailCount.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param[in] *u8watchdog_fail_count  watchdog fail count

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      uint8 u8wdfailcnt;
 *      TPS_GetWatchdogFailCount(u8wdfailcnt);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_26
 * @reqtag      TPS_SR47
 * \endif
*/
boolean TPS_GetWatchdogFailCount(uint8* u8watchdog_fail_count);
/**
 * @brief  Set ignition power latch behavior.
 *
 * @description Enables ignition power latch in TPS device\n
 *              The TPS powers the MCu even if the ignition goes low.
 *
 * \if imageenabled
 * @image html  TPS_ConfigureIgnintionPowerLatchControl.jpg
 * @image rtf   TPS_ConfigureIgnintionPowerLatchControl.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  blignition_powerlatch_control parameter which enables or\n
 *              disables ignition power latch.When enabled the device remains\n
 *              powered on even if ignition goes low.
 *
 * @return       TRUE: no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ConfigureIgnintionPowerLatchControl(TRUE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 *
 * \if destagenabled
 * @destag      DES_27
 * @reqtag      TPS_SR82
 * \endif
*/
boolean TPS_ConfigureIgnintionPowerLatchControl(
        boolean blignition_powerlatch_control);
/**
 * @brief  Configure NRES pin  Monitoring.
 *
 * @description Disables/Enables the NRES ping(EXTERNAL MCCU RESET Pin) monitoring.
 *
 * \if imageenabled
 * @image html  TPS_ConfigureNRESMonitoring.jpg
 * @image rtf   TPS_ConfigureNRESMonitoring.jpg
 * @image latex todo
 * \endif
 * @ingroup NRES_API
 *
 * @param [in]  blNRES_Monitoring TRUE: NRES monitoring is enabled and \n
 *              NRES ERROR will cause the TPS transition to the safe state.\n
 *              FALSE: NRES monitoring is disabled.
 *
 * @return       TRUE: no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ConfigureNRESMonitoring(TRUE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref NRES_API
 *
 * \if destagenabled
 * @destag      DES_28
 * @reqtag      TPS_SR325
 * \endif
*/
boolean TPS_ConfigureNRESMonitoring(boolean blNRES_Monitoring);
/**
 * @brief  gets  VDD3/VDD5 selection.
 *
 * @description The API gets the VDD3/VDD5 selection at power up.The VDD3/5 volt selection can be made using the pin SEL_VDD3/5.
 *
 * \if imageenabled
 * @image html  TPS_Get3_3Vor5VSupply.jpg
 * @image rtf   TPS_Get3_3Vor5VSupply.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]  voltage_selection  SEL_5V if 5V is selected by using the SEL_VDD3/5 pin.\n
 *                                  SEL_3_3V if 3.3V is selected by using the SEL_VDD3/5 pin.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_VDD_3_5_SEL voltage_selection;
 *      TPS_Get3_3Vor5VSupply(&voltage_selection);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_29
 * @reqtag      TPS_SR326
 * \endif
*/
boolean TPS_Get3_3Vor5VSupply(TPS_VDD_3_5_SEL *voltage_selection);
/**
 * @brief  Set device ERROR COUNT threshold value.
 *
 * @description The API sets the device error count threshold value.Be Safe state timeout is disabled the device remains locked in the Safe State\n
 *              even after the device error count even beyond this safe lock threshold value.
 *
 * \if imageenabled
 * @image html  TPS_SetSafeLockThreshhold.jpg
 * @image rtf   TPS_SetSafeLockThreshhold.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  u8SafeLockThreshhold Device ERROR COUNT threshold value.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_SetSafeLockThreshhold(0x5);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 *
 * \if destagenabled
 * @destag      DES_30
 * @reqtag      TPS_SR20
 * \endif
*/
boolean TPS_SetSafeLockThreshhold(uint8 u8SafeLockThreshhold);
/**
 * @brief  Get Current TPS device State.
 *
 * @description The API provides the value indicating the current TPS device state.
 *
 * \if imageenabled
 * @image html  TPS_GetCurrentTPSDeviceState.jpg
 * @image rtf   TPS_GetCurrentTPSDeviceState.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  u8CurrentTPSDeviceState value indicating the current TPS\n
 *              state.Please refer to section 4.16.7 to get more infomration\n
 *              on the values representing the different states of the TPS.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      uint8 u8TPSDevState;
 *      TPS_GetCurrentTPSDeviceState(u8TPSDevState);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_31
 * @reqtag      TPS_SR9
 * \endif
*/
boolean TPS_GetCurrentTPSDeviceState(uint8* u8CurrentTPSDeviceState);
/**
 * @brief  The API helps in setting the device configuration register.
 *
 * @description The APi when called with specific field value and proper setting of the enable_disable configures the features of TPS
 *              such as MASK_VVD3_OV,POST_RUN_RST etc.Please refer to section 4.15.3 in the TPS65381 detached.
 *
 * \if imageenabled
 * @image html  TPS_SetTPSDeviceConfigurationRegisters.jpg
 * @image rtf   TPS_SetTPSDeviceConfigurationRegisters.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  field  Feature(in the device configuration or the flag) which\n
 *                     has to enabled or disabled.It also indicates the field\n
 *                     in the configuration which has to be modified.
 * @param [in]  blvalue parameter indicating whether the feature has to be \n
 *                      enabled or disabled
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_SetTPSDeviceConfigurationRegisters(MASK_VDD1_UV,TRUE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref TPS_GetTPSDeviceConfiguration
 *
 * \if destagenabled
 * @destag      DES_32
 * @reqtag      TPS_SR10,TPS_SR11,TPS_SR12,TPS_SR13,TPS_SR14,TPS_SR15,TPS_SR16
 * \endif
*/
boolean TPS_SetTPSDeviceConfigurationRegisters(
        TPS_DeviceConfigRegisterSettings field,
        boolean blvalue);
/**
 * @brief  API provides the device error count value in TPS.
 *
 * @description API provides the device error count value in TPS.The device\n
 *             error count increases every time when the device transits to\n
 *             Safe state.
 *
 * \if imageenabled
 * @image html  TPS_GetDeviceErrorCount.jpg
 * @image rtf   TPS_GetDeviceErrorCount.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]  u8DeviceErrorCount: Device error count
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      uint8 u8DevErrCount;
 *      TPS_GetDeviceErrorCount(&u8DevErrCount);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref TPS_ClearDeviceErrorCount
 *
 * \if destagenabled
 * @destag      DES_33
 * @reqtag      TPS_SR87
 * \endif
*/
boolean TPS_GetDeviceErrorCount(uint8* u8DeviceErrorCount);
/**
 * @brief  API clears the devices error count values in the TPS register SAFETY_ERR_STAT.
 *
 * @description API clears the devices error count values in the TPS register SAFETY_ERR_STAT.
 *
 * \if imageenabled
 * @image html  TPS_ClearDeviceErrorCount.jpg
 * @image rtf   TPS_ClearDeviceErrorCount.jpg
 * @image latex todo
 * \endif
 *
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ClearDeviceErrorCount();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref TPS_GetDeviceErrorCount
 *
 * \if destagenabled
 * @destag      DES_34
 * @reqtag      TPS_SR83
 * \endif
*/
boolean TPS_ClearDeviceErrorCount(void);

/**
 * @brief  Set TPS device configuration so to support MCU software debug
 *
 * @description Set TPS device configuration so to support MCU software debug.\n
 *              The API sets specific flags such as DIAG EXIT MASK,NO_SAFE _TO\n
 *              etc. bits so as to support MCU debug.Calling this APi corrupts\n
 *              the initial settings made by the TPS_DriverInit API. So the\n
 *              TPS_DriverInit has to be called so as to come back to the\n
 *              init state.
 *
 * \if imageenabled
 * @image html  TPS_SetMCUSoftwareDebugMode.jpg
 * @image rtf   TPS_SetMCUSoftwareDebugMode.jpg
 * @image latex todo
 * \endif
 *
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_SetMCUSoftwareDebugMode();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 *
 * \if destagenabled
 * @destag      DES_35
 * @reqtag      TPS_SR94
 * \endif
*/
boolean TPS_SetMCUSoftwareDebugMode(void);
/**
 * @brief  Takes the necessary actions so as to exit DIAGNOSTIC mode and enter the active mode of TPS device operation.
 *
 * @description Clears the flags related to WD_FAIL,ERROR_PIN_FAIL etc. and sets appropriate flags so as to exit\n
 *              the DIAGNSOTIC state safely and enter the active state operating mode of the TPS device.
 *
 * \if imageenabled
 * @image html  TPS_ExitDiagnosticMode.jpg
 * @image rtf   TPS_ExitDiagnosticMode.jpg
 * @image latex todo
 * \endif
 *
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ExitDiagnosticMode();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Active state
 *
 * @seealso     @ref TPS_ConfigureSafetyCheckControl
 *
 * \if destagenabled
 * @destag      DES_36
 * @reqtag      TPS_SR67
 * \endif
*/
boolean TPS_ExitDiagnosticMode(void);
/**
 * @brief  LOCK/UNLOCK (protect) the configuration registers for writing.
 *
 * @description The Device configuration registers can be protected/released (for writing) by using this API.
 *
 * \if imageenabled
 * @image html  TPS_ProtectConfigurationRegisters.jpg
 * @image rtf   TPS_ProtectConfigurationRegisters.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  state    LOCK - Lock the device configuration registers for writing.
 *                     UNLOCK - unlock device configuration registers for writing
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //LOCK the writing to TPS protected registers
 *      TPS_ProtectConfigurationRegisters(LOCK);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_37
 * @reqtag      TPS_SR62,TPS_SR63,TPS_SR64
 * \endif
*/
boolean TPS_ProtectConfigurationRegisters(
        TPS_HandleConfigurationRegisters state);
/**
 * @brief  The API enables and disables BIST at startup.
 *
 * @description The API sets the flags of the registers SAFETY_BIST_CTRL so as to\n
 *              enable or disable the BIST at startup.
 *
 * \if imageenabled
 * @image html  TPS_ConfigureBISTatStartup.jpg
 * @image rtf   TPS_ConfigureBISTatStartup.jpg
 * @image latex todo
 * \endif
 * @ingroup BIST_API
 *
 * @param [in]  blbist_at_startup  TRUE:BIST at startup is enabled.\n
 *                                FALSE:BIST at startup is disabled.\n
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ConfigureBISTatStartup(TRUE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref BIST_API
 *
 * \if destagenabled
 * @destag      DES_38
 * @reqtag      TPS_SR39
 * \endif
*/
boolean TPS_ConfigureBISTatStartup(boolean blbist_at_startup);
/**
 * @brief  The API starts EEPROM CRC check.
 *
 * @description The API sets the EE_CRC_CHK in the SAFETY_BIST_CTRL register so as to trigger the built in self test.
 *
 * \if imageenabled
 * @image html  TPS_StartEECRCCheck.jpg
 * @image rtf   TPS_StartEECRCCheck.jpg
 * @image latex todo
 * \endif
 * @ingroup CRC_API
 *
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_StartEECRCCheck();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active state
 * @exitmode    TPS in Diagnostic/Active state
 *
 * @seealso     @ref CRC_API
 *
 * \if destagenabled
 * @destag      DES_39
 * @reqtag      TPS_SR74
 * \endif
*/
boolean TPS_StartEECRCCheck(void);
/**
 * @brief  The API initializes the 64 bit data string for 8 bit crc calculation.
 *
 * @description The API initializes the 64 bit data for crc.It updates the necessary fields in the 64 bit data with the\n
 *              contents of the specific fields of the TPS configuration registers as specified in the section\n
 *              Device Configuration Register Protection of the TPS65381 data sheet.
 *
 * \if imageenabled
 * @image html  TPS_InitializeDatastringforCRCCaclcuation.jpg
 * @image rtf   TPS_InitializeDatastringforCRCCaclcuation.jpg
 * @image latex todo
 * \endif
 * @ingroup CRC_API
 *
 * @param [in]  *u32DataforCRC  pointer to the 64 bit data which has to be\n
 *               initialized for the calculation of the 8 bit crc.The 64 bit\n
 *               data is stored in uint32 array of 2 elements.Application\n
 *               should make sure that the array passed is of correct size.
 *
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      uint32 u32dataforcrc[2] = {0U,0U};
 *      TPS_InitializeDatastringforCRCCaclcuation(u32dataforcrc);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state(called after setting up all the\n
 *              necessary parameters for TPS)
 * @exitmode    TPS in Diagnostic/Active state.
 *
 * @seealso     @ref CRC_API
 *
 * \if destagenabled
 * @destag      DES_40
 * @reqtag      TPS_SR72
 * \endif
*/
boolean TPS_InitializeDatastringforCRCCaclcuation(uint32 u32DataforCRC[2]);
/**
 * @brief  The API updates the 64 bit data string for 8 bit crc calculation for specified register.
 *
 * @description The API updates the 64 bit data for crc with updated contents for the specified register.
 *              It updates the necessary fields for the specified register in the 64 bit data.
 *
 * \if imageenabled
 * @image html  TPS_UpdateDatastringforCRCCaclcuation.jpg
 * @image rtf   TPS_UpdateDatastringforCRCCaclcuation.jpg
 * @image latex todo
 * \endif
 *
 * @param [in]  *u32DataforCRC  pointer to the 64 bit data which has to be initialized for the calculation of the 8 bit crc.
 * @param [in]          u16reg  the register for which the contents in the data string (for crc calculation) needs to be updated.
 *
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      uint32 u32dataforcrc[2] = {0U,0U};
 *      TPS_UpdateDatastringforCRCCaclcuation(u32dataforcrc,TPS_DEV_CFG2);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/active state(This is used normally after\n
 *              TPS_InitializeDatastringforCRCCaclcuation called once) to\n
 *              a individual field of the CRC string.
 * @exitmode   TPS in Diagnostic/Active state.
 *
 * @seealso      @ref CRC_API
 *
 * \if destagenabled
 * @destag      DES_41
 * @reqtag      TPS_SR72
 * \endif
*/
boolean TPS_UpdateDatastringforCRCCaclcuation(uint32 u32DataforCRC[2],
        uint16 u16reg);
/** \defgroup CRC_API API used for handling CRC protection of configuration registers and EEPROM.
*
*  CRC support API
*
*/
/**
 * @brief  Calculate the CRC 8 value of the Tps65381 configuration registers(stored as 64 bit data)
 *
 * @description The API Calculates the CRC 8 value for the 64 bit data.This can be used for
 *              calculation of the CRC for the device configuration registers of the TPS device
 *
 * \if imageenabled
 * @image html  TPS_CalculateCRC8.jpg
 * @image rtf   TPS_CalculateCRC8.jpg
 * @image latex todo
 * \endif
 * @ingroup CRC_API
 *
 * @param [in]  *u32DataforCRC  pointer to the 64 bit data for which the CRC 8 has to be calculated.
 * @param [out]  *u8crc_value   pointer to the calculated crc8 value.
 *
 *
 * @return      TRUE : if crc value is calculated.
 *              FALSE: if the execution of the API failed(entry condition and parameter check failed)
 *
 * Example Usage:
 * @code
 *      uint32 u32dataforcrc[2] = {0U,0U};
 *      uint8 u8CRC8;
 *      TPS_CalculateCRC8(u32dataforcrc,&u8CRC8);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state(This is used normally called after\n
 *              data string for the crc calcuation has been initialized\n
 *              by calling TPS_InitializeDatastringforCRCCaclcuation\n
 *              or TPS_UpdateDatastringforCRCCaclcuation.
 * @exitmode   TPS in Diagnostic/Active state.
 *
 * @seealso     @ref CRC_API
 *
 * \if destagenabled
 * @destag      DES_42
 * @reqtag      TPS_SR75,TPS_SR72
 * \endif
*/
boolean TPS_CalculateCRC8(uint32 u32DataforCRC[2], uint8* u8crc_value);
/**
 * @brief  The API is used to get the error status of Configuration register CRC calculation and EEPROM CRC calculation.
 *
 * @description The API updates the passed structure with CRC error status for the EEPROM and Config register CRC calculation
 *
 * \if imageenabled
 * @image html  TPS_GetCRCErrorStatus.jpg
 * @image rtf   TPS_GetCRCErrorStatus.jpg
 * @image latex todo
 * \endif
 * @ingroup CRC_API
 *
 * @param [out]  *crc_error_status The structure will be updated with the CRC error status for the EEPROM and Configuration registers\n
 *                    blCfgCRCError if TRUE there was an error in cofig register CRC calculation.\n
 *                    blEECRCError  if TRUE there was an error in the EE CRC calculation
 *
 *
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_CRCErrorStatus crc_error_status;
 *      TPS_GetCRCErrorStatus(&crc_error_status);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref CRC_API
 *

 * \if destagenabled
 * @destag      DES_46
 * @reqtag      TPS_SR76
 * \endif
*/
boolean TPS_GetCRCErrorStatus(TPS_CRCErrorStatus *crc_error_status);

/** \defgroup BIST_API API used for handling BIST Runs in TPS.
*
*  BIST support API
*
*/
/**
 * @brief  The API starts the built in selftest ABIST/LBIST.
 *
 * @description The API sets the flags of the registers SAFETY_BIST_CTRL so as to start the\n
 *              LBIST or ABIST.
 *
 * \if imageenabled
 * @image html  TPS_StartBIST.jpg
 * @image rtf   TPS_StartBIST.jpg
 * @image latex todo
 * \endif
 * @ingroup BIST_API
 *
 * @param [in]  bist  Built in selftest to be started.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      //Start ABIST
 *      TPS_StartBIST(ABIST)
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active state
 * @exitmode    TPS in Diagnostic/Active state
 *
 * @seealso     @ref BIST_API
 *
 * \if destagenabled
 * @destag      DES_44
 * @reqtag      TPS_SR32,TPS_SR43,TPS_SR31,TPS_SR40
 * \endif
*/
boolean TPS_StartBIST(TPS_BistTypes bist);
/**
 * @brief  The API gets the built in selftest running status.
 *
 * @description The API gets the status of the built in selftest which is running\n
 *
 * \if imageenabled
 * @image html  TPS_GetBISTRunningStatus.jpg
 * @image rtf   TPS_GetBISTRunningStatus.jpg
 * @image latex todo
 * \endif
 * @ingroup BIST_API
 *
 * @param [in]   bist_type     Built in selftest to be started.
 * @param [out]  runningstatus the api updates the BIST running status in  this variable.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_TestRunningStatus runninstatus;
 *      TPS_GetBISTRunningStatus(ABIST,&runninstatus);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref BIST_API
 *
 * \if destagenabled
 * @destag      DES_45
 * @reqtag      TPS_SR42,TPS_SR34,TPS_SR33
 * \endif
*/
boolean TPS_GetBISTRunningStatus(TPS_BistTypes bist_type,
        TPS_TestRunningStatus* runningstatus);
/**
 * @brief  The API gets the EE CRC Check running status.
 *
 * @description The API gets the whether the EEPROM CRC check on the TPS device is running or over.\n
 *
 * \if imageenabled
 * @image html  TPS_GetEECRCCheckRunningStatus.jpg
 * @image rtf   TPS_GetEECRCCheckRunningStatus.jpg
 * @image latex todo
 * \endif
 * @ingroup CRC_API
 *
 * @param [out]  runningstatus the api updates the EE CRC Check running running status in  this variable.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_TestRunningStatus runningstatus;
 *      TPS_GetEECRCCheckRunningStatus(&runningstatus);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref CRC_API
 *
 * \if destagenabled
 * @destag      DES_46
 * @reqtag      TPS_SR42,TPS_SR34
 * \endif
*/
boolean TPS_GetEECRCCheckRunningStatus(TPS_TestRunningStatus * runningstatus);
/**
 * @brief  The provides the ABIST test status.
 *
 * @description The API check if the ABIST is over and returns the ABIST Test status.It returns overall\n
 *              ABIST test status.It also provides the status of LOCLK and Under and Over voltage test status.
 *
 * \if imageenabled
 * @image html  TPS_GetABISTTestStatus.jpg
 * @image rtf   TPS_GetABISTTestStatus.jpg
 * @image latex todo
 * \endif
 * @ingroup BIST_API
 *
 * @param [out]   test_status Structure which holds the ABIST test status

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ABIST_Result teststatus;
 *      TPS_GetABISTTestStatus(&teststatus);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref BIST_API
 *
 * \if destagenabled
 * @destag      DES_47
 * @reqtag      TPS_SR35
 * \endif
*/
boolean TPS_GetABISTTestStatus(TPS_ABIST_Result* test_status);
/**
 * @brief  The API provides the LBIST test status.
 *
 * @description The API check if the LBIST is over and returns the LBIST Test status.
 *
 * \if imageenabled
 * @image html  TPS_GetLBISTTestStatus.jpg
 * @image rtf   TPS_GetLBISTTestStatus.jpg
 * @image latex todo
 * \endif
 * @ingroup BIST_API
 *
 * @param [out]   LBIST_Test_Result TEST_PASS if the LBIST has passed.\n
 *                                  TEST_FAIL if LBIST has failed.

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_Test_Result testresult;
 *      TPS_GetLBISTTestStatus(&testresult);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref BIST_API
 *
 * \if destagenabled
 * @destag      DES_48
 * @reqtag      TPS_SR41,TPS_SR35
 * \endif
*/
boolean TPS_GetLBISTTestStatus(TPS_Test_Result* LBIST_Test_Result);
/**
 * @brief  Set power down threshold value.
 *
 * @description The API is used to set the power down threshold value in the\n
 *              register PWD_THR_CFG register.
 *
 * \if imageenabled
 * @image html  TPS_SetPowerDownThreshhold.jpg
 * @image rtf   TPS_SetPowerDownThreshhold.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]   u8threshhold_value Device error-count threshold to power down the device.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_SetPowerDownThreshhold(0x3);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 *
 * \if destagenabled
 * @destag      DES_49
 * @reqtag      TPS_SR88
 * \endif
*/
boolean TPS_SetPowerDownThreshhold(uint8 u8threshhold_value);
/**
 * @brief  Register read back and compare
 *
 *
 * @description The API compares the init_configuration(the user configured configuration)\n
 *         against the read back configuration(reads the configuration from the TPS device).
 *
 * \if imageenabled
 * @image html  TPS_RegReadBackandCompare.jpg
 * @image rtf   TPS_RegReadBackandCompare.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]   readback_compare TEST_PASS if the read back and comparison has passed.\n
 *                                 TEST_FAIL if read back and comparison has failed
 *
 * @return      TRUE : no errors execution has succeeded.
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_Test_Result readback_compare_test;
 *      TPS_RegReadBackandCompare(&readback_compare_test);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_50
 * @reqtag      TPS_SR328
 * \endif
 */
boolean TPS_RegReadBackandCompare(TPS_Test_Result * readback_compare);
/**
 * @brief  Enable particular AMUX signal
 *
 *
 * @description The user selected AMUX signal is enabled and will be available on the DIAG_OUT pin.
 *
 * \if imageenabled
 * @image html  TPS_EnableAMUXSignal.jpg
 * @image rtf   TPS_EnableAMUXSignal.jpg
 * @image latex todo
 * \endif
 * @ingroup MUX_DIAG
 *
 * @param [in]   AMUXSignal AMUX signal to be enabled.
 *
 * @return      TRUE : no errors execution has succeeded.
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_EnableAMUXSignal(VDD5);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref MUX_DIAG
 *
 * \if destagenabled
 * @destag      DES_51
 * @reqtag      TPS_SR37,TPS_SR36
 * \endif
*/
boolean TPS_EnableAMUXSignal(TPS_AMUXSignals AMUXSignal);
/**
 * @brief  Disable MUX diagnostics.
 *
 *
 * @description Any active MUX diagnostics is disabled and the output on the DIAG_OUT pin will be invalid.
 *
 * \if imageenabled
 * @image html  TPS_DisableMUXDiagnostic.jpg
 * @image rtf   TPS_DisableMUXDiagnostic.jpg
 * @image latex todo
 * \endif
 * @ingroup MUX_DIAG
 *
 *
 * @return      TRUE : no errors execution has succeeded.
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_DisableMUXDiagnostic();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref MUX_DIAG
 *
 * \if destagenabled
 * @destag      DES_52
 * @reqtag      TPS_SR37,TPS_SR36
 * \endif
*/
boolean TPS_DisableMUXDiagnostic(void);
/** \defgroup MUX_DIAG API used for MUX Diagnostics.
*
*  MUX Diagnostics support API
*
*/
/**
 * @brief  Check Enabled AMUX signal limits
 *
 *
 * @description The user selected AMUX signal limits are compared against the initialized limits(application).
 *
 * \if imageenabled
 * @image html  TPS_CheckEnabledAMUXSignalLimits.jpg
 * @image rtf   TPS_CheckEnabledAMUXSignalLimits.jpg
 * @image latex todo
 * \endif
 * @ingroup MUX_DIAG
 *
 * @param [out]   Voltage_limit_test TEST_PASS if voltage limit test has passed.\n
 *                                 TEST_FAIL if voltage limit test has failed.
 *
 * @return      TRUE : no errors execution has succeeded.
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_Test_Result voltage_limit_test;
 *      TPS_CheckEnabledAMUXSignalLimits(&voltage_limit_test);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref MUX_DIAG
 *

 * \if destagenabled
 * @destag      DES_56
 * @reqtag      TPS_SR37
 * \endif
*/
boolean TPS_CheckEnabledAMUXSignalLimits(TPS_Test_Result * Voltage_limit_test);
/**
 * @brief  Enable particular DMUX signal
 *
 *
 * @description The user selected DMUX signal is enabled and will be available on the DIAG_OUT pin.\n
 *              Please refer to table Table 4-4 to 4-7 of the data sheet for more information.
 *
 * **Note: DMUX signals of GROUP 5 are not available for diagnostics**
 *
 * \if imageenabled
 * @image html  TPS_EnableDMUXSignal.jpg
 * @image rtf   TPS_EnableDMUXSignal.jpg
 * @image latex todo
 * \endif
 * @ingroup MUX_DIAG
 *
 * @param [in]   DMUXSignal : DMUX signal to be enabled.\n
 * **Note: The signals reserved for production test and reserved logic cannot be used(as parameter for DMUXSignal)**
 *
 * @return      TRUE : no errors execution has succeeded.
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_EnableDMUXSignal(SPI_NCS);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref MUX_DIAG
 *
 * \if destagenabled
 * @destag      DES_54
 * @reqtag      TPS_SR38,TPS_SR36
 * \endif
*/
boolean TPS_EnableDMUXSignal(TPS_DMUXSignals DMUXSignal);
/**
 * @brief  Inject CRC fault.
 *
 *
 * @description The API initializes the SAFETY_CFG_CRC with incorrect CRC and.\n
 *              enables CRC check so as to generate a CRC error.Application\n
 *              should make sure that it is reading the error status after\n
 *              injecting the fault(2.1 us is needed for completion of the crc\n
 *              check.If the status of the crc error is not read the behavior\n
 *              of the following EEPROM CRC check will be erraneous please\n
 *              check section 4.9.15(page no 48) of the data sheet for more\n
 *              information.
 *
 *
 * \if imageenabled
 * @image html  TPS_FaultInjectCRC.jpg
 * @image rtf   TPS_FaultInjectCRC.jpg
 * @image latex todo
 * \endif
 * @ingroup CRC_API
 *
 *
 * @return      TRUE : no errors execution has succeeded.
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_FaultInjectCRC();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref CRC_API
 *
 * \if destagenabled
 * @destag      DES_55
 * @reqtag      TPS_SR93
 * \endif
*/
boolean TPS_FaultInjectCRC(void);
/**
 * @brief  Set Token Seed value for the watchdog.
 *
 * @description Sets the token seed value for the watchdog.This token seed\n
 *              value is used for the generation of the new token by\n
 *              watchodog.The token seed value is taken by the Watchdog\n
 *              function after the device transitions through the RESET State.
 *
 * \if imageenabled
 * @image html  TPS_WDSetTokenSeedValue.jpg
 * @image rtf   TPS_WDSetTokenSeedValue.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param [in]  u8TokenSeed: parameter new token seed value(range: 0x0U to 0xFU).
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_WDSetTokenSeedValue(0x4);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_56
 * @reqtag      TPS_SR330
 * \endif
*/
boolean TPS_WDSetTokenSeedValue(uint8 u8TokenSeed);
/**
 * @brief  Set Token FeedBack value for the watchdog.
 *
 * @description Sets the token feedback value for the watchdog.This token\n
 *              feedback value is used for controlling the sequence of the\n
 *              tokens generated by the watchdog function.
 *
 * \if imageenabled
 * @image html  TPS_WDSetTokenFDBCKValue.jpg
 * @image rtf   TPS_WDSetTokenFDBCKValue.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param [in]  u8TokenFDBCK: parameter new token feedback value(range: 0x0U to 0xFU).
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_WDSetTokenFDBCKValue(0x4);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_57
 * @reqtag      TPS_SR330
 * \endif
*/
boolean TPS_WDSetTokenFDBCKValue(uint8 u8TokenFDBCK);
/**
 * @brief  The API provides failure status information of the error pin of MCU.
 *
 * @description The API provides failure status information of the error pin.
 *
 * \if imageenabled
 * @image html  TPS_GetErrorPinFailureStatusFlag.jpg
 * @image rtf   TPS_GetErrorPinFailureStatusFlag.jpg
 * @image latex todo
 * \endif
 *
 * @param[in] *blFailureStatus if TRUE the error pin  failure has occurred\n
 *                             if False no error pin failure

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      boolean blfailstatus;
 *      TPS_GetErrorPinFailureStatusFlag(&blfailstatus);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref TPS_ClearErrorPinFailureStatusFlag
 *
 * \if destagenabled
 * @destag      DES_58
 * @reqtag      TPS_SR84
 * \endif
*/
boolean TPS_GetErrorPinFailureStatusFlag(boolean* blFailureStatus);
/**
 * @brief  The API clears the error pin fail status bit in the register SAFETY_ERR_STAT TPS register.
 *
 * @description The API clears the error pin  fail status bit in the register SAFETY_ERR_STAT TPS register.
 *
 * \if imageenabled
 * @image html  TPS_ClearErrorPinFailureStatusFlag.jpg
 * @image rtf   TPS_ClearErrorPinFailureStatusFlag.jpg
 * @image latex todo
 * \endif
 *

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ClearErrorPinFailureStatusFlag();
 * @endcode
 *
 * @entrymode   TPS in Diagnostic state.
 * @exitmode    TPS in Diagnostic state.
 *
 * @seealso     @ref TPS_GetErrorPinFailureStatusFlag
 *
 * \if destagenabled
 * @destag      DES_59
 * @reqtag      TPS_SR85
 * \endif
*/
boolean TPS_ClearErrorPinFailureStatusFlag(void);
/**
 * @brief  The API enables voltage source VDD5.
 *
 * @description The API sets the flag VDD5_EN in the SENS_CTRL register so as to enable the VDD5 voltage source.
 *
 * \if imageenabled
 * @image html  TPS_ConfigureVDD5.jpg
 * @image rtf   TPS_ConfigureVDD5.jpg
 * @image latex todo
 * \endif
 *

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ConfigureVDD5(TRUE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_60
 * @reqtag      TPS_SR90
 * \endif
*/
boolean TPS_ConfigureVDD5(boolean blEnableVDD5);
/**
 * @brief  The API enables voltage source VSOUT1.
 *
 * @description The API sets the flag VSOUT1_ in the SENS_CTRL register so as to enable the VSOUT1 voltage source.
 *
 * \if imageenabled
 * @image html  TPS_ConfigureVSOUT1.jpg
 * @image rtf   TPS_ConfigureVSOUT1.jpg
 * @image latex todo
 * \endif
 *

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_ConfigureVSOUT1(TRUE);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_61
 * @reqtag      TPS_SR89
 * \endif
*/
boolean TPS_ConfigureVSOUT1(boolean blEnableVSOUT1);
/**
 * @brief  Provide status of the Voltage monitoring by the TPS
 *
 * @description The API gets the registers VMON_STAT_1 and VMON_STAT_2 and\n
 *              updates the vmon status structure with the relevant information\n
 *              The API can be used to know the voltage monitoring status\n
 *              of the TPS device.
 *
 * \if imageenabled
 * @image html  TPS_GetVMONStatus.jpg
 * @image rtf   TPS_GetVMONStatus.jpg
 * @image latex todo
 * \endif
 *
 * @param[in] *vmon_status  Structure which stores the information regarding the\n
 *                          voltage monitoring status(VMON_STAT_1,VMON_STAT_2)
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_OverallVMONStatus overall_vmon_status;
 *      TPS_GetVMONStatus(&overall_vmon_status);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_65
 * @reqtag      TPS_SR24,TPS_SR23
 * \endif
*/
boolean TPS_GetVMONStatus(TPS_OverallVMONStatus * vmon_status);
/**
 * @brief  Provide status of junction temperature monitoring and current limit status.
 *
 * @description The API gets the registers SAFETY_STAT_1 and k\n
 *              updates the vmon status structure with the relevant information\n
 *              The API can be used to know the voltage monitoring status\n
 *              of the TPS device.
 *
 * \if imageenabled
 * @image html  TPS_GetJnTempandCurrentLimitStatus.jpg
 * @image rtf   TPS_GetJnTempandCurrentLimitStatus.jpg
 * @image latex todo
 * \endif
 *
 * @param[in] *JnTempCurrentLtMonitoring  Structure which stores the\n
 *                                       information regarding the\ junction\n
 *                                       Temperature monitoring and current\n
 *                                       limit status.
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_JnTempCurrentLimitStatus jntempcurrentmonitoringstatus;
 *      TPS_GetJnTempandCurrentLimitStatus(&jntempcurrentmonitoringstatus);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_63
 * @reqtag      TPS_SR25
 * \endif
 */
boolean TPS_GetJnTempandCurrentLimitStatus(TPS_JnTempCurrentLimitStatus * JnTempCurrentLtMonitoring);
/**
 * @brief  Provide device confiuration(setting made in the DEV_CFG1 and DEV_CFG2 register)
 *
 * @description The API gets the registers DEV_CFG1 and DEV_CFG2 and updates\n
 *              device_configuation structure with the relevant information\n
 *              The API can be used to know the current TPS device configuration.
 *
 * \if imageenabled
 * @image html  TPS_GetTPSDeviceConfiguration.jpg
 * @image rtf   TPS_GetTPSDeviceConfiguration.jpg
 * @image latex todo
 * \endif
 *
 * @param[in] *device_configuation  Structure which stores the\n
 *                                  information regarding the dev config\n
                                    register settings of the TPS device
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_DevCfg_reg_settings tps_dev_cfg_regs;
 *      TPS_GetTPSDeviceConfiguration(&tps_dev_cfg_regs);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref TPS_SetTPSDeviceConfigurationRegisters
 *
 * \if destagenabled
 * @destag      DES_64
 * @reqtag      TPS_SR17
 * \endif
 */
boolean TPS_GetTPSDeviceConfiguration(TPS_DevCfg_reg_settings * config_register_settings);
/**
 * @brief  Provide device the error status of the previous SPI transmission.
 *
 * @description The API reads the register SAFETY_STAT_4 and provides the SPI\n
 *              error status information.
 *
 * \if imageenabled
 * @image html  TPS_GetSPIErrorStatus.jpg
 * @image rtf   TPS_GetSPIErrorStatus.jpg
 * @image latex todo
 * \endif
 *
 * @param[in] *spi_error_status     Structure which stores the\n
 *                                  information regarding the spi error status.
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_SPIErrorType spierr;
 *      TPS_GetSPIErrorStatus(&spierr);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_65
 * @reqtag      TPS_SR78
 * \endif
 */
boolean TPS_GetSPIErrorStatus(TPS_SPIErrorType * spi_error_status);
/**
 * @brief  Provide information regarding TPS
 *
 * @description The API reads the register DEV_STATE and provides the canwu_l\n
 *              and IGN deglitched pin status.
 *
 * \if imageenabled
 * @image html  TPS_GetDEV_STATE.jpg
 * @image rtf   TPS_GetDEV_STATE.jpg
 * @image latex todo
 * \endif
 *
 * @param[in] *dev_state            Structure which stores the\n
 *                                  information regarding latched can wakeup\n
 *                                  and deglitched ignition pin.
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_DeviceState devstate;
 *      TPS_GetDEV_STATE(&devstate);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.c
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_69
 * @reqtag      TPS_SR7,TPS_SR8
 * \endif
*/
boolean TPS_GetDEV_STATE(TPS_DeviceState * dev_state);
/** \defgroup NRES_API API used for handling NRES monitoring and test.
*
*  NRES Monitoring support API
*
*/
/**
 * @brief  The API provides failure information of NRES pin(readback state of NRES PIN).
 *
 * @description The API provides status or Reset Input status (readback state of NRES PIN).
 *
 * \if imageenabled
 * @image html  TPS_GetNRESErrorStatus.jpg
 * @image rtf   TPS_GetNRESErrorStatus.jpg
 * @image latex todo
 * \endif
 * @ingroup NRES_API
 *
 * @param[in] *blNRESError if TRUE the NRES_ERR has occured the readback status and current status of the NRES pin are not correct\n
 *                             if False NRES error readback status and current status of the NRES pin are same.

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      boolean blNRESErrstatus;
 *      TPS_GetNRESErrorStatus(&blNRESErrstatus);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 * @seealso     @ref NRES_API
 *
 * \if destagenabled
 * @destag      DES_67
 * @reqtag      TPS_SR77
 * \endif
*/
boolean TPS_GetNRESErrorStatus(boolean* blNRESError);
/** \defgroup WD_SUPPORT API used for handling the TPS watchdog(external watchdog)
*
*  Watchdog support API
*
*/
/**
 * @brief  Inject fault in watchdog
 *
 *
 * @description The API injects fault in the watchdog.The fault inject mode\n
 *              such as BAD Answer are supported by the API.When called by\n
 *              application API sends a wrong answer to the QANDA watchdog in\n
 *              in the TPS device.The API can be used by applications only when\n
 *              watchdog is configured in QANDA mode.
 *
 *
 * \if imageenabled
 * @image html  TPS_FaultInjectWD.jpg
 * @image rtf   TPS_FaultInjectWD.jpg
 * @image latex todo
 * \endif
 * @ingroup WD_SUPPORT
 *
 * @param[in] WDFault watchdog fault to be injected.
 *
 * @return      TRUE : no errors execution has succeeded.
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_FaultInjectWD(BAD_ANSWER);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active state.
 * @exitmode    TPS in Diagnostic/Active state.
 *
 * @seealso     @ref WD_SUPPORT
 *
 * \if destagenabled
 * @destag      DES_68
 * @reqtag      TPS_SR92
 * \endif
*/
boolean TPS_FaultInjectWD(TPS_WDFaultType WDFault);
/**
 * @brief  The API provides Enable drive error status.
 *
 * @description The API provide enable drive error status.The enable drive\n
 * error gets set when there is mismatch between EN_DRV output and EN_DRV\n
 * feedback.
 *
 * \if imageenabled
 * @image html  TPS_GetENDRVErrorStatus.jpg
 * @image rtf   TPS_GetENDRVErrorStatus.jpg
 * @image latex todo
 * \endif
 *
 * @param[in] *blENDRVError if TRUE the ENDRV_ERR is set.ENDRV_ERR get set\n
 *            is mismatch between the EN_DRV output and EN_DRV feedback is\n
 *            detected.

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      boolean blEndrvErrstatus;
 *      TPS_GetENDRVErrorStatus(&blEndrvErrstatus);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe state.
 * @exitmode    TPS in Diagnostic/Active/Safe state.
 *
 *
 * \if destagenabled
 * @destag      DES_69
 * @reqtag      TPS_SR79
 * \endif
*/
boolean TPS_GetENDRVErrorStatus(boolean* blENDRVError);
/**
 * @brief Update SAFETY_CFG_CRC register
 *
 * @description The API writes the user provide value to the register\n
 *              SAFETY_CFG_CRC
 *
 * \if imageenabled
 * @image html  TPS_UpdateRegisterSafetyCfgCRC.jpg
 * @image rtf   TPS_UpdateRegisterSafetyCfgCRC.jpg
 * @image latex todo
 * \endif
 * @ingroup CRC_API
 *
 * @param [in]  u8safetycfgcrc  value to be written to the SAFETY_CFG_CRC register
 *
 *
 * @return      TRUE : no errors.
 *              FALSE: errors in execution
 *
 * Example Usage:
 * @code
 *      //u8safetycfgcrc contains the calculated CRC8 using the crc calculation routines.
 *      TPS_UpdateRegisterSafetyCfgCRC(u8safetycfgcrc);
 * @endcode
 *
 * @entrymode  TPS in Diagnostic state.
 * @exitmode   TPS in Diagnostic state.
 *
 * @seealso     @ref CRC_API
 *
 * \if destagenabled
 * @destag      DES_70
 * @reqtag      TPS_SR75
 * \endif
*/
boolean TPS_UpdateRegisterSafetyCfgCRC(uint8 u8safetycfgcrc);
/**
 * @brief  The API does the selftest of the error pin monitoring.
 *
 * @description The API does of the selftest of the error pin monitoring.Can be called only in diagnostic mode\n.
 *              The API makes sure that the ERROR_PIN_FAIL flag is appropriately set when the mcu error pin is\n
 *              low for more than 15us.
 *
 *
 * \if imageenabled
 * @image html  TPS_TestErrorPinMonitoring.jpg
 * @image rtf   TPS_TestErrorPinMonitoring.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]   ErrorPinMonitoring_Test TEST_PASS if the error monitoring is successful has passed.\n
 *                                        TEST_FAIL if error monitoring has failed.

 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_Test_Result errorpinmonitoringtest;
 *      TPS_TestErrorPinMonitoring(&errorpinmonitoringtest);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic
 * @exitmode    TPS in Diagnostic
 *
 * @seealso     @ref TPS_ConfigureErrorMonitoring
 *
 * \if destagenabled
 * @destag      DES_71
 * @reqtag      TPS_SR41,TPS_SR35
 * \endif
 */
boolean TPS_TestErrorPinMonitoring(TPS_Test_Result * ErrorPinMonitoring_Test);
/**
 * @brief  The API moves the TPS device safe state(from Active state).
 *
 * @description The API moves the TPS device to safe state.by creating an error in the MCU.
 * **Note: The error monitoring is enabled by setting the bit NO_ERROR in safety check control to 0**
 *
 *
 * \if imageenabled
 * @image html  TPS_GoToSafeState.jpg
 * @image rtf   TPS_GoToSafeState.jpg
 * @image latex todo
 * \endif
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_GoToSafeState();
 * @endcode
 *
 * @entrymode   TPS in Active
 * @exitmode    TPS in Safe
 *
 *
 * \if destagenabled
 * @destag      DES_72
 * @reqtag      TPS_SR41,TPS_SR35,TPS_SR315
 * \endif
*/
boolean TPS_GoToSafeState(void);
/**
 * @brief  The API provides TPS device information.
 *
 * @description The API provides TPS device information such as major version,minor version and device ID.
 *
 *
 * \if imageenabled
 * @image html  TPS_GetDeviceInformation.jpg
 * @image rtf   TPS_GetDeviceInformation.jpg
 * @image latex todo
 * \endif
 *
 * @param [out]   device_information Structure containing the device information such as device ID and major,minor version.
 *
 * @return      TRUE : no errors
 *              FALSE: Errors in execution
 *
 * Example Usage:
 * @code
 *      TPS_DeviceInformation devinfo;
 *      TPS_GetDeviceInformation(&devinfo);
 * @endcode
 *
 * @entrymode   TPS in Diagnostic/Active/Safe
 * @exitmode    TPS in Diagnostic/Active/Safe
 *
 *
 * \if destagenabled
 * @destag      DES_76
 * @reqtag      TPS_SR4,TPS_SR5
 * \endif
 */
boolean TPS_GetDeviceInformation(TPS_DeviceInformation * device_information);
#endif
