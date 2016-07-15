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
 *         @brief     Driver Level API to configure TPS device
 *         @details   The TPS_Driver.c file provides the driver level api's to\n
 *                    the TPS.These API's should help the application in\n
 *                    setting up proper operation mode for the TPS device.
 *
 *
 *---------------------------------------------------------------------------------------------------------------------
 * @author  Manoj
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
 | Date        | Author           | Description                         											  |
 |:-----------:|:----------------:|:----------------------------------------------------------------------------------|
 |  20Dec2012  | Manoj R          | Initial version created             											  |
 |  Oct2014	   | Sharath		  | Fixed wrong data for commands SW_LOCK and SW_UNLOCK (SDOCM00113801, SDOCM00111757)|
 |  Oct2014	   | Sharath		  | Fixed TPS_ConfigureWatchdogWindows scalar_window param check <= instead of <	  |
 *---------------------------------------------------------------------------------------------------------------------
 */

/* Comments regarding various justificatios for deviation from MISRA-C code guidelines
 * are provided at the bottom of the file */

/***************************************************************
 * INCLUDE FILES
 ****************************************************************/
#include "TPS_Driver.h"
#include "TPS_DebugSupport.h"
#include "TPS_Priv.h"
/*********************************************************
 Local  definitions
 **********************************************************/
static boolean TPS_UpdateDatastringforRegister(TPS_Registers tps_register,
        uint8 u8bfdest_start, uint8 u8src_start, uint8 u8length, uint32* u32crc);
static TPS_Config init_configuration;
/*SAFETYMCUSW 298 S MR:- <APPROVED> "Comment_1"*/
static GetTPSDiagosticPinSampledData GetTPSDiagPinSampledValue;
static TPS_AllVoltageRailLimits TPSAMUXVoltageRailLimits;
/*default values of the EnabledAMUXSignal and EnabledDMUXSignal*/
static TPS_AMUXSignals EnabledAMUXSignal = VDD5;
static TPS_DMUXSignals EnabledDMUXSignal = AVDD_UVN;
static uint8 u8ActiveWDToken;
/*********************************************************
 Functions
 **********************************************************/
boolean TPS_DriverInit(const TPS_Initilization_struct* const init_struct)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U, u8dvcfg2 = 0U, u8safety_check_ctrl = 0U,
            u8safetyfunccfg = 0U, u8safetyerrcfg = 0U;

    /*Initialize the send and receive API used by the TPS interface to communicate to the TPS device*/
    TpsIf_Init(init_struct->send_recieve_data_TPS);
    /*wait for the device to be diagnostic state*/
    while (TPS_DEVICE_DIAGNOSTIC != u8tps_device_state)
    {
        blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    }

    if (TRUE == blRetVal)
    {
        /*Initialize the get api for getting the TPS diagnostic pin sampled value from the application*/
        GetTPSDiagPinSampledValue = init_struct->GetTPSDiagPinSampledValue;
        /*set safety check control register values cfg_crc,ENABLE_DRV,ERROR_PIN_MONITORING,DIAG_EXIT_MASK*/
        blRetVal = TpsIf_GetRegister(TPS_SAFETY_CHECK_CTRL,
                &u8safety_check_ctrl);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safety_check_ctrl, ((uint8)(init_struct->blenable_drv_op)),
                BF_ENABLE_DRV_START, BF_ENABLE_DRV_LENGTH);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safety_check_ctrl, ((uint8)(!init_struct->blerror_pin_monitoring)),
                BF_NO_ERROR_START, BF_NO_ERROR_LENGTH);
        blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_CHECK_CTRL,
                u8safety_check_ctrl)) && blRetVal);

        /*set device configuration registers*/
        /*set DEV_CFG1 register*/
        /*masked VDD1_OV does not impact ENDRV pin state*/
        blRetVal =
                ((TpsIf_SetRegisterBitFieldVerify(TPS_DEV_CFG1,
                        BF_nMASK_VDD1_UV_OV_START, BF_nMASK_VDD1_UV_OV_LENGTH,
                        ((uint8)(init_struct->dev_configreg_settings.blMASK_VDD1_UV))))
                        && blRetVal);
        /*update the init_configuration structure to support register readback*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(init_configuration.u8dev_cfg1,
                ((uint8)init_struct->dev_configreg_settings.blMASK_VDD1_UV),
                BF_nMASK_VDD1_UV_OV_START, BF_nMASK_VDD1_UV_OV_LENGTH);
        init_configuration.u8dev_cfg1 = init_configuration.u8dev_cfg1
                & DEV_CFG1_REG_MASK;

        /*set DEV_CFG2 register*/
        blRetVal = ((TpsIf_GetRegister(TPS_DEV_CFG2, &u8dvcfg2)) && blRetVal);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8dvcfg2,((uint8)init_struct->dev_configreg_settings.blVDD3_5_OT),
                BF_EN_VDD3_5_OT_START, BF_EN_VDD3_5_OT_LENGTH);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8dvcfg2, ((uint8)init_struct->dev_configreg_settings.blVDD5_OT),
                BF_EN_VDD5_OT_START, BF_EN_VDD5_OT_LENGTH);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8dvcfg2, ((uint8)init_struct->dev_configreg_settings.blMASK_VBAT_OV),
                BF_MASK_VBAT_OV_START, BF_MASK_VBAT_OV_LENGTH);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8dvcfg2, ((uint8)init_struct->dev_configreg_settings.blPOST_RUN_RST),
                BF_POST_RUN_RST_START, BF_POST_RUN_RST_LENGTH);
        blRetVal = ((TpsIf_SetRegisterVerify(TPS_DEV_CFG2, u8dvcfg2))
                && blRetVal);

        /*update the init_configuration structure to support register readback*/
        init_configuration.u8dev_cfg2 = u8dvcfg2 & DEV_CFG2_REG_MASK;

        /*Bist at startup*/
        blRetVal = ((TPS_ConfigureBISTatStartup(init_struct->blbist_at_startup))
                && blRetVal);

        /*Ignition power latch,wd reset,vdd3_5,wd_cfg,safe_state_timeout,error_config(PWM or error pin low monitoring),nres_monitoring*/
        /*sets the bit fields in the safety_func_cfg register*/
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_FUNC_CFG, &u8safetyfunccfg))
                && blRetVal);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safetyfunccfg, ((uint8)(!init_struct->blsafe_state_timeout)),
                BF_NO_SAFE_TO_START, BF_NO_SAFE_TO_LENGTH);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safetyfunccfg, ((uint8)init_struct->blignition_power_latch),
                BF_IGN_PWRL_START, BF_IGN_PWRL_LENGTH);
        /*disable watchdog reset temporarily enable it at the end of the API call*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safetyfunccfg, ((uint8)FALSE), BF_WD_RST_EN_START,
                BF_WD_RST_EN_LENGTH);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safetyfunccfg, ((uint8)(!(init_struct->blNRES_Monitoring))),
                BF_DIS_NRES_MON_START, BF_DIS_NRES_MON_LENGTH);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safetyfunccfg, ((uint8)(init_struct->error_pin_monitoring_mode)),
                BF_ERROR_CFG_START, BF_ERROR_CFG_LENGTH);
        blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_FUNC_CFG,
                u8safetyfunccfg)) && blRetVal);

        /*update the init_configuration structure to support register readback*/
        init_configuration.u8safety_func_cfg = u8safetyfunccfg
                & SAFETY_FUNC_CFG_MASK;

        /*set safe lock threshhold*/
        /*set safe state timeout duration*/
        /*sets the bit fields in the safety_err_cfg register*/
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_ERR_CFG, &u8safetyerrcfg))
                && blRetVal);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safetyerrcfg, init_struct->u8safe_lock_threshhold,
                BF_SAFE_LOCK_THR_START, BF_SAFE_LOCK_THR_LENGTH);
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safetyerrcfg, init_struct->u8safe_state_timeout_scaler,
                BF_SAFE_TO_START, BF_SAFE_TO_LENGTH);
        /*cfg lock for the register access is not set*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8safetyerrcfg, 0x0U, BF_CFG_LOCK_START, BF_CFG_LOCK_LENGTH);
        blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_ERR_CFG,
                u8safetyerrcfg)) && blRetVal);

        /*update the init_configuration structure to support register readback*/
        init_configuration.u8safety_err_cfg = u8safetyerrcfg;

        /*set power down threshold*/
        /*sets register safety_pwd_thr_cfg*/
        blRetVal = ((TPS_SetPowerDownThreshhold(
                init_struct->u8pwd_threshhold_value)) && blRetVal);

        /*update the init_configuration structure to support register readback*/
        init_configuration.u8safety_pwd_thr =
                init_struct->u8pwd_threshhold_value;
        blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_ERR_PWM_H,
                (init_struct->error_pin_pwm_width).u8PWM_HIGH)) && blRetVal);

        /*update the init_configuration structure to support register readback*/
        init_configuration.u8safety_err_pwm_h =
                (init_struct->error_pin_pwm_width).u8PWM_HIGH;
        blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_ERR_PWM_L,
                (init_struct->error_pin_pwm_width).u8PWM_LOW)) && blRetVal);

        /*update the init_configuration structure to support register readback*/
        init_configuration.u8safety_err_pwm_l =
                (init_struct->error_pin_pwm_width).u8PWM_LOW;
        memcpy(&TPSAMUXVoltageRailLimits, &init_struct->AMUXVoltageRailLimits,
                sizeof(init_struct->AMUXVoltageRailLimits));
    }
    else
    {
        blRetVal = FALSE;
        /*SAFETYMCUSW 95 S MR: 11.1,11.4 <APPROVED> Comment_4*/
        TPS_SendDebugText(DEBUG, (uint8 *)"TPSDriverInit TPS not in diagnostic state",
                (uint32)blRetVal);
    }

    return blRetVal;
}

boolean TPS_ConfigureSafetyCheckControl(TPS_SafetyCheckControl safety_check,
        boolean blstate)
{
    uint8 u8bfstart = 0U, u8bflength = 0U, u8bitvalue = 0U;
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        switch (safety_check)
        {
            case CFG_CRC:
                u8bfstart = BF_CFG_CRC_ENABLE_START;
                u8bflength = BF_CFG_CRC_ENABLE_LENGTH;
                if (TRUE == blstate)
                {
                    u8bitvalue = 1U;
                }
                else
                {
                    u8bitvalue = 0U;
                }
                break;

            case DRV_OUTPUT:
                u8bfstart = BF_ENABLE_DRV_START;
                u8bflength = BF_ENABLE_DRV_LENGTH;
                if (TRUE == blstate)
                {
                    u8bitvalue = 1U;
                }
                else
                {
                    u8bitvalue = 0U;
                }
                break;

            case ERROR_PIN_MONITORING:

                u8bfstart = BF_NO_ERROR_START;
                u8bflength = BF_NO_ERROR_LENGTH;
                if (TRUE == blstate)
                {
                    /*error pin monitoring is enabled and the ERROR_PIN_FAIL status bit is updated
                     * based on the mcu error pin status
                     */

                    u8bitvalue = 0U;
                }
                else
                {
                    /*error pin monitoring is disabled */
                    u8bitvalue = 1U;
                }
                break;

            case DIAG_EXIT_MASK:
                u8bfstart = BF_DIAG_EXIT_MASK_START;
                u8bflength = BF_DIAG_EXIT_MASK_LENGTH;
                if (TRUE == blstate)
                {
                    /*exit from the DIAGNOSTIC state is disabled */
                    u8bitvalue = 1U;
                }
                else
                {
                    /*exit from the DIAGNOSTIC state is enabled */
                    u8bitvalue = 0U;
                }
                break;

            case DIAG_EXIT:
                u8bfstart = BF_DIAG_EXIT_START;
                u8bflength = BF_DIAG_EXIT_LENGTH;
                if (TRUE == blstate)
                {
                    u8bitvalue = 1U;
                }
                else
                {
                    u8bitvalue = 0U;
                }
                break;

            default:
                blRetVal = FALSE;
                break;
        }
        if (TRUE == blRetVal)
        {
            blRetVal = ((TpsIf_SetRegisterBitField(TPS_SAFETY_CHECK_CTRL,
                    u8bfstart, u8bflength, u8bitvalue)) && blRetVal);
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
boolean TPS_ConfigureSafeStateTimeout(boolean blsafe_state_timeout)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        if (TRUE == blsafe_state_timeout)
        {
            /* Safe state timeout is enabled when bit is set to 0U*/
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                    BF_NO_SAFE_TO_START, BF_NO_SAFE_TO_LENGTH, 0U);
            /*update the init_configuration structure to support register readback*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8safety_func_cfg, 0U,
                    BF_NO_SAFE_TO_START, BF_NO_SAFE_TO_LENGTH);
        }
        else
        {
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                    BF_NO_SAFE_TO_START, BF_NO_SAFE_TO_LENGTH, (uint8)1U);
            /*update the init_configuration structure to support register readback*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8safety_func_cfg, 1U,
                    BF_NO_SAFE_TO_START, BF_NO_SAFE_TO_LENGTH);
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_SetSafeStateTimoutDuration(uint8 u8SafeStateTOtDuration_scaler)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)
            && (u8SafeStateTOtDuration_scaler <= SAFE_STATE_TIMEOUT_MAX))
    {
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_ERR_CFG,
                BF_SAFE_TO_START, BF_SAFE_TO_LENGTH,
                u8SafeStateTOtDuration_scaler);
        /*update the init_configuration structure to support register readback*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(init_configuration.u8safety_err_cfg,
                u8SafeStateTOtDuration_scaler, BF_SAFE_TO_START,
                BF_SAFE_TO_LENGTH);
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_ConfigureErrorMonitoring(TPS_ErrorPinMonitoringMode monitoring_mode,
        TPS_ERRORPinPWMWidthScaler pwm)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        switch (monitoring_mode)
        {
            case PWM_MONITORING_MODE:
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                        BF_ERROR_CFG_START, BF_ERROR_CFG_LENGTH, monitoring_mode);
                /*update the init_configuration structure to support register readback*/
                /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                BFU8_SET(init_configuration.u8safety_func_cfg, ((uint8)monitoring_mode),
                        BF_ERROR_CFG_START, BF_ERROR_CFG_LENGTH);
                blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_ERR_PWM_H,
                        pwm.u8PWM_HIGH)) && blRetVal);
                /*update the init_configuration structure to support register readback*/
                init_configuration.u8safety_err_pwm_h = pwm.u8PWM_HIGH;
                blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_ERR_PWM_L,
                        pwm.u8PWM_LOW)) && blRetVal);
                init_configuration.u8safety_err_pwm_l = pwm.u8PWM_LOW;
                break;

            case ERROR_PIN_MODE:
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                        BF_ERROR_CFG_START, BF_ERROR_CFG_LENGTH, monitoring_mode);
                /*update the init_configuration structure to support register readback*/
                /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                BFU8_SET(init_configuration.u8safety_func_cfg, ((uint8)monitoring_mode),
                        BF_ERROR_CFG_START, BF_ERROR_CFG_LENGTH);
                blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_ERR_PWM_L,
                        pwm.u8PWM_LOW)) && blRetVal);
                /*update the init_configuration structure to support register readback*/
                init_configuration.u8safety_err_pwm_l = pwm.u8PWM_LOW;
                break;
            default:
                blRetVal = FALSE;
                break;

        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}
boolean TPS_GetErrorPinFailureStatusFlag(boolean* blFailureStatus)
{
    boolean blRetVal = TRUE;
    uint8 u8bitavalue = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(blFailureStatus))
    {
        blRetVal = FALSE;
    }
    else
    {
		blRetVal = TpsIf_GetRegisterBitField(TPS_SAFETY_ERR_STAT,
		BF_ERROR_PIN_FAIL_START, BF_ERROR_PIN_FAIL_LENGTH, &u8bitavalue);
        if (1U == u8bitavalue)
        {
            *blFailureStatus = TRUE;
        }
        else
        {
            *blFailureStatus = FALSE;
        }
    }
    return blRetVal;
}

boolean TPS_ClearErrorPinFailureStatusFlag(void)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_ERR_STAT,
                BF_ERROR_PIN_FAIL_START, BF_ERROR_PIN_FAIL_LENGTH, 0U);
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_WatchdogInit(const TPS_WatchdoConfiguration *const watchdogconfig)
{
    uint8 u8wdt_token_fdbck = 0U;
    uint8 u8tps_device_state = 0U;
    boolean blRetVal = TRUE;

	/*Check that pointer values falls in range*/
	/*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
	/*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
	if (!CHECK_RANGE_RAM_PTR(watchdogconfig))
	{
		blRetVal = FALSE;
	}
	else
	{
		blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
		if ((TRUE == blRetVal)
				&& (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)) {
			blRetVal = TPS_SetWatchdogMode(watchdogconfig->watchdog_mode);
			/*setting pwm width for error monitoring*/
			if (QANDA_MODE == watchdogconfig->watchdog_mode) {

				/*sets register wdt_token_fdbck*/
				/*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
				BFU8_SET(u8wdt_token_fdbck, watchdogconfig->u8TokenSeed,
						BF_TOKEN_SEED_START, BF_TOKEN_SEED_LENGTH);
				/*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
				BFU8_SET(u8wdt_token_fdbck, watchdogconfig->u8TokenFDBK,
						BF_TOKEN_FDBK_START, BF_TOKEN_FDBK_LENGTH);
				blRetVal = ((TpsIf_SetRegisterVerify(TPS_WDT_TOKEN_FDBCK,
						u8wdt_token_fdbck)) && blRetVal);

				/*update the init_configuration structure to support register readback*/
				init_configuration.u8wdt_token_fbck = u8wdt_token_fdbck;
			} else {
				blRetVal = TpsIf_SetRegisterVerify(TPS_WDT_TOKEN_FDBCK, 0U);
				init_configuration.u8wdt_token_fbck = 0U;
			}

			blRetVal = ((TPS_ConfigureWatchdogReset(
					watchdogconfig->blwatchdog_reset)) && blRetVal);

			/*set watchdog window duration(wdt_win1_cfg,wdt_win2_cfg)*/
			blRetVal = ((TPS_ConfigureWatchdogWindows(OPEN_WINDOW,
					watchdogconfig->u8open_windowtime_scaler)) && blRetVal);
			/*update the init_configuration structure to support register readback*/
			init_configuration.u8wdt_win1_cfg =
					watchdogconfig->u8open_windowtime_scaler & WDT_WIN1_CFG_MASK;

			blRetVal = ((TPS_ConfigureWatchdogWindows(CLOSE_WINDOW,
					watchdogconfig->u8close_windowtime_scaler)) && blRetVal);
			/*update the init_configuration structure to support register readback*/
			init_configuration.u8wdt_win2_cfg =
					watchdogconfig->u8close_windowtime_scaler
							& WDT_WIN2_CFG_MASK;
		} else {
			blRetVal = FALSE;
		}
	}

    return blRetVal;

}

boolean TPS_SetWatchdogMode(TPS_WatchdogMode watchdogmode)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        switch (watchdogmode)
        {
            case WDTI_MODE:
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                        BF_WD_CFG_START, BF_WD_CFG_LENGTH, 0U);
                /*update the init_configuration structure to support register readback*/
                /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                BFU8_SET(init_configuration.u8safety_func_cfg, 0U,
                        BF_WD_CFG_START, BF_WD_CFG_LENGTH);
                break;
            case QANDA_MODE:
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                        BF_WD_CFG_START, BF_WD_CFG_LENGTH, (uint8)1U);
                /*update the init_configuration structure to support register readback*/
                /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                BFU8_SET(init_configuration.u8safety_func_cfg, 1U,
                        BF_WD_CFG_START, BF_WD_CFG_LENGTH);

                break;
            default:
                blRetVal = FALSE;
                break;
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_WDSetTokenSeedValue(uint8 u8TokenSeed)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && ((TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)))
    {
        if (u8TokenSeed > TOKEN_SEED_MAX)
        {
            blRetVal = FALSE;
        }
        else
        {
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_WDT_TOKEN_FDBCK,
                    BF_TOKEN_SEED_START, BF_TOKEN_SEED_LENGTH, u8TokenSeed);
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8wdt_token_fbck, u8TokenSeed,
                    BF_TOKEN_SEED_START, BF_TOKEN_SEED_LENGTH);
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_WDSetTokenFDBCKValue(uint8 u8TokenFDBCK)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && ((TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)))
    {
        if (u8TokenFDBCK > TOKEN_FDBCK_MAX)
        {
            blRetVal = FALSE;
        }
        else
        {
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_WDT_TOKEN_FDBCK,
                    BF_TOKEN_FDBK_START, BF_TOKEN_FDBK_LENGTH, u8TokenFDBCK);
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8wdt_token_fbck, u8TokenFDBCK,
                    BF_TOKEN_FDBK_START, BF_TOKEN_FDBK_LENGTH);
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}
boolean TPS_ConfigureWatchdogWindows(TPS_WatchdogWindows watchdog_windows,
        uint8 u8windowtime_scaler)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        switch (watchdog_windows)
        {
            case OPEN_WINDOW:
                if (u8windowtime_scaler <= OPEN_WINDOW_SCALER_MAX)
                {
                    blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_WDT_WIN1_CFG,
                    BF_WD_OPEN_WINDOW_SCALER_START,
                    BF_WD_OPEN_WINDOW_SCALER_LENGTH, u8windowtime_scaler);
                    /*update the init_configuration structure to support register readback*/
                    /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                    BFU8_SET(init_configuration.u8wdt_win1_cfg,
                            u8windowtime_scaler, BF_WD_OPEN_WINDOW_SCALER_START,
                            BF_WD_OPEN_WINDOW_SCALER_LENGTH);
                }
                else
                {
                    blRetVal = FALSE;
                }
                break;
            case CLOSE_WINDOW:
                if (u8windowtime_scaler <= CLOSE_WINDOW_SCALER_MAX)
                {
                    blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_WDT_WIN2_CFG,
                    BF_WD_CLOSE_WINDOW_SCALER_START,
                    BF_WD_CLOSE_WINDOW_SCALER_LEN, u8windowtime_scaler);
                    /*update the init_configuration structure to support register readback*/
                    /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                    BFU8_SET(init_configuration.u8wdt_win2_cfg,
                            u8windowtime_scaler,
                            BF_WD_CLOSE_WINDOW_SCALER_START,
                            BF_WD_CLOSE_WINDOW_SCALER_LEN);
                }
                else
                {
                    blRetVal = FALSE;
                }
                break;
            default:
                blRetVal = FALSE;
                break;
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_ConfigureWatchdogReset(boolean blreset_enable)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        if (TRUE == blreset_enable)
        {
            /*watchdog reset  is enabled when bit is set to 1U*/
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                    BF_WD_RST_EN_START, BF_WD_RST_EN_LENGTH, (uint8)1U);
            /*update the init_configuration structure to support register readback*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8safety_func_cfg, 1U, BF_WD_RST_EN_START,
                    BF_WD_RST_EN_LENGTH);
        }
        else
        {
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                    BF_WD_RST_EN_START, BF_WD_RST_EN_LENGTH, 0U);
            /*update the init_configuration structure to support register readback*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8safety_func_cfg, 0U, BF_WD_RST_EN_START,
                    BF_WD_RST_EN_LENGTH);
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_UpdateActiveWDToken(void)
{
    return TpsIf_GetRegisterBitField(TPS_WDT_TOKEN_VALUE,
            BF_WDT_TOKEN_VALUE_START, BF_WDT_TOKEN_VALUE_LENGTH,
            &u8ActiveWDToken);
}

boolean TPS_SendWdgResponse(TPS_WDResponse wd_response)
{
    /*todo how to verify that the table values are correct*/
    const uint8 TPS_WatchdogAnswTable[16][4] =
    {
    { 0xFFU, 0x0FU, 0xF0U, 0x00U },
    { 0xB0U, 0x40U, 0xBFU, 0x4FU },
    { 0xE9U, 0x19U, 0xE6U, 0x16U },
    { 0xA6U, 0x56U, 0xA9U, 0x59U },
    { 0x75U, 0x85U, 0x7AU, 0x8AU },
    { 0x3AU, 0xCAU, 0x35U, 0xC5U },
    { 0x63U, 0x93U, 0x6CU, 0x9CU },
    { 0x2CU, 0xDCU, 0x23U, 0xD3U },
    { 0xD2U, 0x22U, 0xDDU, 0x2DU },
    { 0x9DU, 0x6DU, 0x92U, 0x62U },
    { 0xC4U, 0x34U, 0xCBU, 0x3BU },
    { 0x8BU, 0x7BU, 0x84U, 0x74U },
    { 0x58U, 0xA8U, 0x57U, 0xA7U },
    { 0x17U, 0xE7U, 0x18U, 0xE8U },
    { 0x4EU, 0xBEU, 0x41U, 0xB1U },
    { 0x01U, 0xF1U, 0x0EU, 0xFEU } };

    return TpsIf_SetRegister(TPS_WDT_ANSWER,
            TPS_WatchdogAnswTable[u8ActiveWDToken][wd_response]);
}

boolean TPS_GetWatchdogFailureStatus(boolean* blFailureStatus)
{
    boolean blRetVal = TRUE;
    uint8 u8bitavalue = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(blFailureStatus))
    {
        blRetVal = FALSE;
    }
    else
    {
		blRetVal = TpsIf_GetRegisterBitField(TPS_SAFETY_ERR_STAT,
		BF_WD_FAIL_START, BF_WD_FAIL_LENGTH, &u8bitavalue);
        if (1U == u8bitavalue)
        {
            *blFailureStatus = TRUE;
        }
        else
        {
            *blFailureStatus = FALSE;
        }
    }
    return blRetVal;
}

boolean TPS_ClearWatchdogFailureStatusFlag(void)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_ERR_STAT,
                BF_WD_FAIL_START, BF_WD_FAIL_LENGTH, 0U);
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_GetWatchdogAnswerCount(uint8* u8AnswerCount)
{
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(u8AnswerCount))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegisterBitField(TPS_WDT_STATUS,
                BF_WDT_ANSW_CNT_START, BF_WDT_ANSW_CNT_LENGTH, u8AnswerCount);
    }
    return blRetVal;
}

boolean TPS_GetWatchdogErrorType(TPS_WatchdogErrorStatus * WDErrorStatus)
{
    boolean blRetVal = TRUE;
    uint8 u8wdtstatus = 0U;
    blRetVal = TpsIf_GetRegister(TPS_WDT_STATUS, &u8wdtstatus);
    if (1U
            == BFU8_GET(u8wdtstatus,BF_WDT_TOKEN_ERR_START,BF_WDT_TOKEN_ERR_LENGTH))
    {
        WDErrorStatus->blTokenError = TRUE;
    }
    else
    {
        WDErrorStatus->blTokenError = FALSE;
    }
    if (1U
            == BFU8_GET(u8wdtstatus,BF_WDT_WRONG_CFG_START,BF_WDT_WRONG_CFG_LENGTH))
    {
        WDErrorStatus->blWdWrongConfig = TRUE;
    }
    else
    {
        WDErrorStatus->blWdWrongConfig = FALSE;
    }
    if (1U == BFU8_GET(u8wdtstatus,BF_WDT_SEQ_ERR_START,BF_WDT_SEQ_ERR_LENGTH))
    {
        WDErrorStatus->blSeqError = TRUE;
    }
    else
    {
        WDErrorStatus->blSeqError = FALSE;
    }
    if (1U == BFU8_GET(u8wdtstatus,BF_WDT_TIME_OUT_START,BF_WDT_TIME_OUT_LENGTH))
    {
        WDErrorStatus->blTimeOutError = TRUE;
    }
    else
    {
        WDErrorStatus->blTimeOutError = FALSE;
    }
    if (1U
            == BFU8_GET(u8wdtstatus,BF_WDT_TOKEN_EARLY_START,BF_WDT_TOKEN_EARLY_LENGTH))
    {
        WDErrorStatus->blTokenEarlyError = TRUE;
    }
    else
    {
        WDErrorStatus->blTokenEarlyError = FALSE;
    }
    return blRetVal;
}

boolean TPS_GetWatchdogFailCount(uint8* u8watchdog_fail_count)
{
    return TpsIf_GetRegisterBitField(TPS_SAFETY_STAT_2, BF_WDT_FAIL_CNT_START,
            BF_WDT_FAIL_CNT_LENGTH, u8watchdog_fail_count);
}

boolean TPS_ConfigureIgnintionPowerLatchControl(
        boolean blignition_powerlatch_control)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        if (TRUE == blignition_powerlatch_control)
        {
            /*with ignition power down device will not power down when bit is set to 1U*/
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                    BF_IGN_PWRL_START, BF_IGN_PWRL_LENGTH, (uint8)1U);
            /*update the init_configuration structure to support register readback*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8safety_func_cfg, 1U, BF_IGN_PWRL_START,
                    BF_IGN_PWRL_LENGTH);
        }
        else
        {
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                    BF_IGN_PWRL_START, BF_IGN_PWRL_LENGTH, 0U);
            /*update the init_configuration structure to support register readback*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8safety_func_cfg, 0U, BF_IGN_PWRL_START,
                    BF_IGN_PWRL_LENGTH);
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_ConfigureNRESMonitoring(boolean blNRES_Monitoring)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        if (TRUE == blNRES_Monitoring)
        {
            /*with ignition power down device will not power down when bit is set to 1U*/
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                    BF_DIS_NRES_MON_START, BF_DIS_NRES_MON_LENGTH, 0U);
            /*update the init_configuration structure to support register readback*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8safety_func_cfg, 0U,
                    BF_DIS_NRES_MON_START, BF_DIS_NRES_MON_LENGTH);
        }
        else
        {
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_FUNC_CFG,
                    BF_DIS_NRES_MON_START, BF_DIS_NRES_MON_LENGTH, (uint8)1U);
            /*update the init_configuration structure to support register readback*/
            /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU8_SET(init_configuration.u8safety_func_cfg, 1U,
                    BF_DIS_NRES_MON_START, BF_DIS_NRES_MON_LENGTH);
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_Get3_3Vor5VSupply(TPS_VDD_3_5_SEL * voltage_selection)
{
    boolean blRetVal = TRUE;
    uint8 u8voltage_selection = 0U;

    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(voltage_selection))
	{
		blRetVal = FALSE;
	}
	else
	{
		blRetVal = TpsIf_GetRegisterBitField(TPS_SAFETY_FUNC_CFG,
		BF_VDD_3_5_SEL_START, BF_VDD_3_5_SEL_LENGTH, &u8voltage_selection);
		if (0U == u8voltage_selection) {
			*voltage_selection = SEL_5V;
		} else {
			*voltage_selection = SEL_3_3V;
		}
	}
    return blRetVal;
}

boolean TPS_SetSafeLockThreshhold(uint8 u8SafeLockThreshhold)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)
            && (u8SafeLockThreshhold <= SAFE_LOCK_THRESHHOLD_MAX))
    {
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_ERR_CFG,
                BF_SAFE_LOCK_THR_START, BF_SAFE_LOCK_THR_LENGTH,
                u8SafeLockThreshhold);
        /*update the init_configuration structure to support register readback*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(init_configuration.u8safety_err_cfg, u8SafeLockThreshhold,
                BF_SAFE_LOCK_THR_START, BF_SAFE_LOCK_THR_LENGTH);
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_GetCurrentTPSDeviceState(uint8* u8CurrentTPSDeviceState)
{
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(u8CurrentTPSDeviceState))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegister(TPS_SAFETY_STAT_5,
                u8CurrentTPSDeviceState);
    }
    return blRetVal;
}

boolean TPS_SetTPSDeviceConfigurationRegisters(
        TPS_DeviceConfigRegisterSettings field, boolean blvalue)
{
    uint8 u8bfstart = 0U, u8bflength = 0U, u8bitvalue = 0U;
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        if (MASK_VDD1_UV == field)
        {
            if (TRUE == blvalue)
            {
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_DEV_CFG1,
                        BF_nMASK_VDD1_UV_OV_START, BF_nMASK_VDD1_UV_OV_LENGTH, (uint8)1U);
                /*update the init_configuration structure to support register readback*/
                /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                BFU8_SET(init_configuration.u8dev_cfg1, 1U,
                        BF_nMASK_VDD1_UV_OV_START, BF_nMASK_VDD1_UV_OV_LENGTH);

            }
            else
            {
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_DEV_CFG1,
                        BF_nMASK_VDD1_UV_OV_START, BF_nMASK_VDD1_UV_OV_LENGTH, 0U);
                /*update the init_configuration structure to support register readback*/
                /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                BFU8_SET(init_configuration.u8dev_cfg1, 0U,
                        BF_nMASK_VDD1_UV_OV_START, BF_nMASK_VDD1_UV_OV_LENGTH);

            }
        }
        else
        {
            switch (field)
            {
                case VDD3_5_OT:
                    u8bfstart = BF_EN_VDD3_5_OT_START;
                    u8bflength = BF_EN_VDD3_5_OT_LENGTH;

                    if (TRUE == blvalue)
                    {
                        u8bitvalue = 1U;
                    }
                    else
                    {
                        u8bitvalue = 0U;
                    }
                    break;
                case VDD5_OT:
                    u8bfstart = BF_EN_VDD5_OT_START;
                    u8bflength = BF_EN_VDD5_OT_LENGTH;

                    if (TRUE == blvalue)
                    {
                        /*error pin monitoring is enabled and the ERROR_PIN_FAIL status bit is updated
                         * based on the mcu error pin status
                         */
                        u8bitvalue = 0U;
                    }
                    else
                    {
                        /*error pin monitoring is disabled */
                        u8bitvalue = 1U;
                    }
                    break;
                case MASK_VBAT_OV:
                    u8bfstart = BF_MASK_VBAT_OV_START;
                    u8bflength = BF_MASK_VBAT_OV_LENGTH;

                    if (TRUE == blvalue)
                    {
                        /*exit from the DIAGNOSTIC state is disabled */
                        u8bitvalue = 1U;
                    }
                    else
                    {
                        /*exit from the DIAGNOSTIC state is enabled */
                        u8bitvalue = 0U;
                    }
                    break;
                case POST_RUN_RST:
                    u8bfstart = BF_POST_RUN_RST_START;
                    u8bflength = BF_POST_RUN_RST_LENGTH;

                    if (TRUE == blvalue)
                    {
                        u8bitvalue = 1U;
                    }
                    else
                    {
                        u8bitvalue = 0U;
                    }
                    break;
                default:
                    blRetVal = FALSE;
                    break;
            }
            if (TRUE == blRetVal)
            {
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_DEV_CFG2,
                        u8bfstart, u8bflength, u8bitvalue);
                /*update the init_configuration structure to support register readback*/
                /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
                BFU8_SET(init_configuration.u8dev_cfg2, u8bitvalue, u8bfstart,
                        u8bflength);
            }
            else
            {
                blRetVal = FALSE;
            }
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_GetDeviceErrorCount(uint8* u8DeviceErrorCount)
{
    boolean blRetVal;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(u8DeviceErrorCount))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegisterBitField(TPS_SAFETY_ERR_STAT,
                BF_DEV_ERR_CNT_START, BF_DEV_ERR_CNT_LENGTH,
                u8DeviceErrorCount);
    }
    return blRetVal;
}

boolean TPS_ClearDeviceErrorCount(void)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_ERR_STAT,
                BF_DEV_ERR_CNT_START, BF_DEV_ERR_CNT_LENGTH, 0x0U);
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_SetMCUSoftwareDebugMode(void)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        /*enable and mask the exit from the diagnostic mode*/
        blRetVal = TPS_ConfigureSafetyCheckControl(DIAG_EXIT_MASK, TRUE);
        /*Disable the watchdog reset */
        blRetVal = ((TPS_ConfigureWatchdogReset(FALSE)) && blRetVal);
        /*disable the error pin monitoring*/
        /*disable the safe state timeout*/
        blRetVal = ((TPS_ConfigureSafeStateTimeout(FALSE)) && blRetVal);
        blRetVal = ((TPS_ConfigureSafetyCheckControl(ERROR_PIN_MONITORING,
                FALSE)) && blRetVal);

    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_ExitDiagnosticMode(void)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal =
            ((TPS_GetCurrentTPSDeviceState(&u8tps_device_state)) && blRetVal);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state))
    {
        TPS_ClearWatchdogFailureStatusFlag();
        TPS_ClearErrorPinFailureStatusFlag();
        blRetVal = TPS_ConfigureSafetyCheckControl(DIAG_EXIT_MASK, FALSE);
        blRetVal = ((TpsIf_SetRegisterBitField(TPS_SAFETY_CHECK_CTRL,
                BF_DIAG_EXIT_START, BF_DIAG_EXIT_LENGTH, (uint8)1U)) && blRetVal);
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_ProtectConfigurationRegisters(
        TPS_HandleConfigurationRegisters state)
{
    boolean blRetVal = TRUE;
    switch (state)
    {
    	/* Datasheet has wrong entry for data to lock/unlock registers (swapped each other) */
        case LOCK:
            blRetVal = TpsIf_SetRegister(TPS_SW_LOCK, SW_LOCK_DATA);
            break;
        case UNLOCK:
            blRetVal = TpsIf_SetRegister(TPS_SW_UNLOCK, SW_UNLOCK_DATA);
            break;
        default:
            blRetVal = FALSE;
            break;
    }
    return blRetVal;
}

boolean TPS_ConfigureBISTatStartup(boolean blbist_at_startup)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal)
            && ((TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)
                    || (TPS_DEVICE_ACTIVE == u8tps_device_state)))
    {
        if (TRUE == blbist_at_startup)
        {
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_BIST_CTRL,
                    BF_AUTO_BIST_DIS_START, BF_AUTO_BIST_DIS_LENGTH, 0U);
        }
        else
        {
            blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_BIST_CTRL,
                    BF_AUTO_BIST_DIS_START, BF_AUTO_BIST_DIS_LENGTH, (uint8)1U);
        }
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}
boolean TPS_StartEECRCCheck(void)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal)
            && ((TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)
                    || (TPS_DEVICE_ACTIVE == u8tps_device_state)))
    {

        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_BIST_CTRL,
                BF_EE_CRC_CHK_START, BF_EE_CRC_CHK_LENGTH, (uint8)1U);

    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_GetEECRCCheckRunningStatus(TPS_TestRunningStatus * runningstatus)
{
    boolean blRetVal = TRUE;
    uint8 u8value = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(runningstatus))
    {
        blRetVal = FALSE;
    }
    else
    {
        if (TRUE
                == TpsIf_GetRegisterBitField(TPS_SAFETY_BIST_CTRL,
                        BF_EE_CRC_CHK_START, BF_EE_CRC_CHK_LENGTH, &u8value))
        {

            if (1U == u8value)
            {
                *runningstatus = RUNNING;
            }
            else
            {
                *runningstatus = OVER;
            }
        }
        else
        {
            blRetVal = FALSE;
        }
    }

    return blRetVal;
}

/** @fn unsigned short TPS_UpdateDatastringforRegister(mibspiBASE_t *SPI_TPS, unsigned int *TpsDevConfig)
 *   @brief Read the Tps65381 configuration registers and save i n TpsDevConfig
 *   @param[in] SPI_TPS base address of Spi register
 *   @param[in] TpsDevConfig array to save the Tps65381 configuration values, length =2.
 *   @return 0U         : no errors
 *   @           others: error occur
 *
 *   Read the Tps65381 configuration registers and save in TpsDevConfig
 */
static boolean TPS_UpdateDatastringforRegister(TPS_Registers tps_register,
        uint8 u8bfdest_start, uint8 u8src_start, uint8 u8length, uint32* u32crc)
{
    uint8 u8value = 0U;
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(u32crc))
    {
        blRetVal = FALSE;
    }
    else
    {
        if (TRUE == TpsIf_GetRegister(tps_register, &u8value))
        {
        	/*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
            BFU32_SET(*u32crc, BFU8_GET(u8value,u8src_start,u8length),
                    u8bfdest_start, u8length);
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    return blRetVal;
}

boolean TPS_InitializeDatastringforCRCCaclcuation(uint32 u32DataforCRC[2])
{
    uint32 u32DataForCRCHi = 0U, u32DataForCRCLow = 0U;
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(u32DataforCRC))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TPS_UpdateDatastringforRegister(TPS_SAFETY_FUNC_CFG,
                BF_DST_SAFETY_FUNC_CFG_START, BF_SRC_SAFETY_FUNC_CFG_START,
                BF_SAFETY_FUNC_CFG_LENGTH, &u32DataForCRCHi);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_DEV_REV,
                BF_DST_DEV_REV_START, BF_SRC_DEV_REV_START, BF_DEV_REV_LENGTH,
                &u32DataForCRCHi)) && blRetVal);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_SAFETY_PWD_THR_CFG,
                BF_DST_SAFETY_PWD_THR_CFG_START,
                BF_SRC_SAFETY_PWD_THR_CFG_START, BF_SAFETY_PWD_THR_CFG_LENGTH,
                &u32DataForCRCHi)) && blRetVal);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_SAFETY_ERR_CFG,
                BF_DST_SAFETY_ERR_CFG_START, BF_SRC_SAFETY_ERR_CFG_START,
                BF_SAFETY_ERR_CFG_LENGTH, &u32DataForCRCHi)) && blRetVal);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_WDT_TOKEN_FDBCK,
                BF_DST_EDT_TOK_FBACK_7_3_START,
                BF_SRC_EDT_TOK_FBACK_7_3_START,
                BF_EDT_TOKEN_FDBACK_7_3_LENGTH, &u32DataForCRCHi)) && blRetVal);
        blRetVal =
                ((TPS_UpdateDatastringforRegister(TPS_WDT_TOKEN_FDBCK,
                        BF_DST_EDT_TOK_FBACK_2_0_START,
                        BF_SRC_EDT_TOK_FBACK_2_0_START,
                        BF_EDT_TOKEN_FDBACK_2_0_LENGTH, &u32DataForCRCLow))
                        && blRetVal);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_WDT_WIN2_CFG,
                BF_DST_WDT_WIN2_CFG_START, BF_SRC_WDT_WIN2_CFG_START,
                BF_WDT_WIN2_CFG_LENGTH, &u32DataForCRCLow)) && blRetVal);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_WDT_WIN1_CFG,
                BF_DST_WDT_WIN1_CFG_START, BF_SRC_WDT_WIN1_CFG_START,
                BF_WDT_WIN1_CFG_LENGTH, &u32DataForCRCLow)) && blRetVal);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_SAFETY_ERR_PWM_L,
                BF_DST_SAFETY_ERR_PWM_L_START, BF_SRC_SAFETY_ERR_PWM_L_START,
                BF_SAFETY_ERR_PWM_L_LENGTH, &u32DataForCRCLow)) && blRetVal);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_DEV_CFG2,
                BF_DST_DEV_CFG2_START, BF_SRC_DEV_CFG2_START,
                BF_DEV_CFG2_LENGTH, &u32DataForCRCLow)) && blRetVal);
        blRetVal = ((TPS_UpdateDatastringforRegister(TPS_DEV_CFG1,
                BF_DST_DEV_CFG1_START, BF_SRC_DEV_CFG1_START,
                BF_DEV_CFG1_LENGTH, &u32DataForCRCLow)) && blRetVal);

        u32DataforCRC[0U] = u32DataForCRCLow;
        u32DataforCRC[1U] = u32DataForCRCHi;
    }
    return blRetVal;
}

boolean TPS_UpdateDatastringforCRCCaclcuation(uint32 u32DataforCRC[2],
        uint16 u16reg)
{
    boolean blRetVal = TRUE;
    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(u32DataforCRC))
    {
        blRetVal = FALSE;
    }
    else
    {
        switch (u16reg)
        {
            case TPS_DEV_CFG1:
                blRetVal = TPS_UpdateDatastringforRegister(TPS_DEV_CFG1,
                        BF_DST_DEV_CFG1_START, BF_SRC_DEV_CFG1_START,
                        BF_DEV_CFG1_LENGTH, &u32DataforCRC[0U]);
                break;

            case TPS_DEV_CFG2:
                blRetVal = TPS_UpdateDatastringforRegister(TPS_DEV_CFG2,
                        BF_DST_DEV_CFG2_START, BF_SRC_DEV_CFG2_START,
                        BF_DEV_CFG2_LENGTH, &u32DataforCRC[0U]);
                break;

            case TPS_SAFETY_ERR_PWM_L:
                blRetVal = TPS_UpdateDatastringforRegister(
                        TPS_SAFETY_ERR_PWM_L, BF_DST_SAFETY_ERR_PWM_L_START,
                        BF_SRC_SAFETY_ERR_PWM_L_START,
                        BF_SAFETY_ERR_PWM_L_LENGTH, &u32DataforCRC[0U]);
                break;

            case TPS_WDT_WIN1_CFG:
                blRetVal = TPS_UpdateDatastringforRegister(TPS_WDT_WIN1_CFG,
                        BF_DST_WDT_WIN1_CFG_START,
                        BF_SRC_SAFETY_ERR_PWM_L_START, BF_WDT_WIN1_CFG_LENGTH,
                        &u32DataforCRC[0U]);
                break;

            case TPS_WDT_WIN2_CFG:
                blRetVal =
                        TPS_UpdateDatastringforRegister(TPS_WDT_WIN2_CFG,
                                BF_DST_WDT_WIN2_CFG_START,
                                BF_SRC_WDT_WIN2_CFG_START,
                                BF_WDT_WIN2_CFG_LENGTH, &u32DataforCRC[0U]);
                break;

            case TPS_SAFETY_ERR_CFG:
                blRetVal = TPS_UpdateDatastringforRegister(TPS_SAFETY_ERR_CFG,
                        BF_DST_SAFETY_ERR_CFG_START,
                        BF_SRC_SAFETY_ERR_CFG_START, BF_SAFETY_ERR_CFG_LENGTH,
                        &u32DataforCRC[1U]);
                break;

            case TPS_SAFETY_PWD_THR_CFG:
                blRetVal = TPS_UpdateDatastringforRegister(
                        TPS_SAFETY_PWD_THR_CFG, BF_DST_SAFETY_PWD_THR_CFG_START,
                        BF_SRC_SAFETY_PWD_THR_CFG_START,
                        BF_SAFETY_PWD_THR_CFG_LENGTH, &u32DataforCRC[1U]);
                break;

            case TPS_DEV_REV:
                blRetVal = TPS_UpdateDatastringforRegister(TPS_DEV_REV,
                        BF_DST_DEV_REV_START, BF_SRC_DEV_REV_START,
                        BF_DEV_REV_LENGTH, &u32DataforCRC[1U]);
                break;

            case TPS_SAFETY_FUNC_CFG:
                blRetVal = TPS_UpdateDatastringforRegister(
                        TPS_SAFETY_FUNC_CFG, BF_DST_SAFETY_FUNC_CFG_START,
                        BF_SRC_SAFETY_FUNC_CFG_START, BF_SAFETY_FUNC_CFG_LENGTH,
                        &u32DataforCRC[1U]);
                break;

            case TPS_WDT_TOKEN_FDBCK:
                blRetVal = TPS_UpdateDatastringforRegister(
                        TPS_WDT_TOKEN_FDBCK, BF_DST_EDT_TOK_FBACK_7_3_START,
                        BF_SRC_EDT_TOK_FBACK_7_3_START,
                        BF_EDT_TOKEN_FDBACK_7_3_LENGTH, &u32DataforCRC[1U]);
                blRetVal = ((TPS_UpdateDatastringforRegister(
                        TPS_WDT_TOKEN_FDBCK, BF_DST_EDT_TOK_FBACK_2_0_START,
                        BF_SRC_EDT_TOK_FBACK_2_0_START,
                        BF_EDT_TOKEN_FDBACK_2_0_LENGTH, &u32DataforCRC[0U]))
                        && blRetVal);
                break;

            default:
                blRetVal = FALSE;
                break;
        }
    }
    return blRetVal;
}

boolean TPS_CalculateCRC8(uint32 u32DataforCRC[2], uint8* u8crc_value)
{
    boolean blRetVal = TRUE;
    uint32 u32i;
    uint32 u32CrcSoure;
    uint8 u8xbit, u8Nextbit0;


    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!(CHECK_RANGE_RAM_PTR(u32DataforCRC)
            /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
            /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
            && CHECK_RANGE_RAM_PTR(u8crc_value)))
    {
        blRetVal = FALSE;
    }
    else
    {
        /*Initial seed value for the flip flops(The initial CRC value)\n
         *is 0xFF see figure 4-13 in data sheet for more information*/
        *u8crc_value = 0xFFU;
        /* change the order of the bytes so that it is easy to calculate the CRC, 0xABCDEFGH ->0xGHEFCDAB*/
        u32CrcSoure = CHANGE_BYTE_ORDER(u32DataforCRC[0U]);
        for (u32i = 32U; u32i > 0U; u32i--)
        {
            u8xbit = (uint8)BIT_GET(u32CrcSoure,(uint32)(u32i-(uint32)1U));
            u8Nextbit0 = u8xbit ^ (BIT_GET(*u8crc_value,7U));
            *u8crc_value ^= (u8Nextbit0 + (u8Nextbit0 << 1U));
            *u8crc_value = (*u8crc_value << 1U) + u8Nextbit0;
        }

        /* change the order of the bytes so that it is easy to calculate the CRC, 0xABCDEFGH ->0xGHEFCDAB*/
        u32CrcSoure = CHANGE_BYTE_ORDER(u32DataforCRC[1U]);
        for (u32i = 32U; u32i > 0U; u32i--)
        {
            u8xbit = (uint8)BIT_GET(u32CrcSoure,((uint32)(u32i-(uint32)1U)));
            u8Nextbit0 = u8xbit ^ (BIT_GET(*u8crc_value,7U));
            *u8crc_value ^= (u8Nextbit0 + (u8Nextbit0 << 1U));
            *u8crc_value = (*u8crc_value << 1U) + u8Nextbit0;
        }
        blRetVal = TRUE;
    }
    return blRetVal;
}

boolean TPS_GetCRCErrorStatus(TPS_CRCErrorStatus * crc_error_status)
{
    boolean blRetVal = TRUE;
    uint8 u8regavalue = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(crc_error_status))
    {
        blRetVal = FALSE;
    }
    else
    {
        if (TRUE == TpsIf_GetRegister(TPS_SAFETY_STAT_2, &u8regavalue))
        {
            if (1U
                    == BFU8_GET(u8regavalue,BF_CFG_CRC_ERR_START,BF_CFG_CRC_ERR_LENGTH))
            {
                crc_error_status->blCfgCRCError = TRUE;
            }
            else
            {
                crc_error_status->blCfgCRCError = FALSE;
            }
            if (1U
                    == BFU8_GET(u8regavalue,BF_EE_CRC_ERR_START,BF_EE_CRC_ERR_LENGTH))
            {
                crc_error_status->blEECRCError = TRUE;
            }
            else
            {
                crc_error_status->blEECRCError = FALSE;
            }
            blRetVal = TRUE;
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    return blRetVal;
}
boolean TPS_UpdateRegisterSafetyCfgCRC(uint8 u8safetycfgcrc)
{
    return TpsIf_SetRegisterVerify(TPS_SAFETY_CFG_CRC, u8safetycfgcrc);
}

boolean TPS_StartBIST(TPS_BistTypes bist)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal)
            && ((TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)
                    || (TPS_DEVICE_ACTIVE == u8tps_device_state)))
    {
        switch (bist)
        {
            case ABIST:
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_BIST_CTRL,
                        BF_ABIST_RUN_START, BF_ABIST_RUN_LENGTH, (uint8)1U);
                break;
            case LBIST:
                blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_BIST_CTRL,
                        BF_LBIST_EN_START, BF_LBIST_EN_LENGTH, (uint8)1U);
                break;
            default:
                blRetVal = FALSE;
                break;
        }

    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_GetBISTRunningStatus(TPS_BistTypes bist_type,
        TPS_TestRunningStatus* runningstatus)
{
    boolean blRetVal = TRUE;
    uint8 u8regavalue = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(runningstatus))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegister(TPS_SAFETY_STAT_3, &u8regavalue);
        switch (bist_type)
        {
            case ABIST:
                if (1U
                        == BFU8_GET(u8regavalue,BF_ABIST_RUN_START,BF_ABIST_RUN_LENGTH))
                {
                    *runningstatus = RUNNING;
                }
                else
                {
                    *runningstatus = OVER;
                }
                break;
            case LBIST:
                if (1U
                        == BFU8_GET(u8regavalue,BF_LBIST_RUN_START,BF_LBIST_RUN_LENGTH))
                {
                    *runningstatus = RUNNING;
                }
                else
                {
                    *runningstatus = OVER;
                }
                break;
            default:
                blRetVal = FALSE;
                break;
        }
    }
    return blRetVal;
}

boolean TPS_GetABISTTestStatus(TPS_ABIST_Result* test_status)
{
    boolean blRetVal = TRUE;
    TPS_TestRunningStatus abist_status = OVER;
    uint8 u8SafetyStat3 = 0U, u8SafetyStat4 = 0U, u8ABIST_UV_OV_ERROR = 0U, u8LOCLK_ERROR = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(test_status))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TPS_GetBISTRunningStatus(ABIST, &abist_status);
        if ((TRUE == blRetVal) && (OVER == abist_status))
        {
            blRetVal = TpsIf_GetRegister(TPS_SAFETY_STAT_3, &u8SafetyStat3);
            blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_STAT_4, &u8SafetyStat4))
                    && blRetVal);
            u8ABIST_UV_OV_ERROR =
                    BFU8_GET(u8SafetyStat3,BF_ABIST_UV_OV_ERR_START,BF_ABIST_UV_OV_ERR_LENGTH);
            u8LOCLK_ERROR =
                    BFU8_GET(u8SafetyStat4,BF_LOCLK_START,BF_LOCLK_LENGTH);
            if ((TRUE == blRetVal) && (0U == u8ABIST_UV_OV_ERROR)
                    && (0U == u8LOCLK_ERROR))
            {
                test_status->ABIST_Result = TEST_PASS;
                test_status->LOCLK_Test = TEST_PASS;
                test_status->UVOV_Test = TEST_PASS;
            }
            else
            {
                test_status->ABIST_Result = TEST_FAIL;
                if (0U == u8ABIST_UV_OV_ERROR)
                {
                    test_status->UVOV_Test = TEST_PASS;
                }
                else
                {
                    test_status->UVOV_Test = TEST_FAIL;
                }
                if (0U == u8LOCLK_ERROR)
                {
                    test_status->LOCLK_Test = TEST_PASS;
                }
                else
                {
                    test_status->LOCLK_Test = TEST_FAIL;
                }

            }

        }
        else
        {
            blRetVal = FALSE;
        }

    }
    return blRetVal;

}

boolean TPS_GetLBISTTestStatus(TPS_Test_Result* LBIST_Test_Result)
{
    boolean blRetVal = TRUE;
    TPS_TestRunningStatus lbist_status = OVER;
    uint8 u8SafetyStat3 = 0U;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(LBIST_Test_Result))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TPS_GetBISTRunningStatus(LBIST, &lbist_status);
        if ((TRUE == blRetVal) && (OVER == lbist_status))
        {
            blRetVal = TpsIf_GetRegister(TPS_SAFETY_STAT_3, &u8SafetyStat3);
            if ((TRUE == blRetVal)
                    && (0U
                            == BFU8_GET(u8SafetyStat3,BF_LBIST_ERR_START,BF_LBIST_ERR_LENGTH)))
            {
                *LBIST_Test_Result = TEST_PASS;
            }
            else
            {
                *LBIST_Test_Result = TEST_FAIL;
            }
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    return blRetVal;
}

boolean TPS_SetPowerDownThreshhold(uint8 u8threshhold_value)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

    blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state)
            && (u8threshhold_value <= PWD_THRESHHOLD_MAX))
    {
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_PWD_THR_CFG,
                BF_PWD_THR_START, BF_PWD_THR_LENGTH, u8threshhold_value);
        /*update the init_configuration structure to support register readback*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(init_configuration.u8safety_pwd_thr, u8threshhold_value,
                BF_PWD_THR_START, BF_PWD_THR_LENGTH);
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_RegReadBackandCompare(TPS_Test_Result * readback_compare)
{
    boolean blRetVal = TRUE;
    TPS_Config readback_config = {0U};

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(readback_compare))
    {
        blRetVal = FALSE;
    }
    else
    {
        /*read the registers for read back and compare aginst hte TPS readback config registers*/
        blRetVal = TpsIf_GetRegister(TPS_SAFETY_FUNC_CFG,
                &readback_config.u8safety_func_cfg);
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_ERR_CFG,
                &readback_config.u8safety_err_cfg)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_PWD_THR_CFG,
                &readback_config.u8safety_pwd_thr)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_DEV_CFG1,
                &readback_config.u8dev_cfg1)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_DEV_CFG2,
                &readback_config.u8dev_cfg2)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_WDT_TOKEN_FDBCK,
                &readback_config.u8wdt_token_fbck)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_WDT_WIN1_CFG,
                &readback_config.u8wdt_win1_cfg)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_WDT_WIN2_CFG,
                &readback_config.u8wdt_win2_cfg)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_ERR_PWM_L,
                &readback_config.u8safety_err_pwm_l)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_ERR_PWM_H,
                &readback_config.u8safety_err_pwm_h)) && blRetVal);

        if (blRetVal == TRUE)
        {
            /*update the readback_config using the masks(some bits are not used in the config registers\n
             * some are status bits which are not valid for the comparison.
             */
            readback_config.u8safety_func_cfg =
                    readback_config.u8safety_func_cfg & SAFETY_FUNC_CFG_MASK;
            readback_config.u8safety_pwd_thr = readback_config.u8safety_pwd_thr
                    & SAFETY_PWD_THR_CFG_MASK;
            readback_config.u8dev_cfg1 = readback_config.u8dev_cfg1
                    & DEV_CFG1_REG_MASK;
            readback_config.u8dev_cfg2 = readback_config.u8dev_cfg2
                    & DEV_CFG2_REG_MASK;
            readback_config.u8wdt_win1_cfg = readback_config.u8wdt_win1_cfg
                    & WDT_WIN1_CFG_MASK;
            readback_config.u8wdt_win2_cfg = readback_config.u8wdt_win2_cfg
                    & WDT_WIN2_CFG_MASK;
            if (0U
                    == memcmp(&readback_config, &init_configuration,
                            sizeof(readback_config)))
            {
                *readback_compare = TEST_PASS;
            }
            else
            {
                *readback_compare = TEST_FAIL;
            }
        }
        else
        {
            blRetVal = FALSE;
        }
    }
    return blRetVal;
}

boolean TPS_EnableAMUXSignal(TPS_AMUXSignals AMUXSignal)
{
    /* value of the diag_MUX_sel for specific AMUX Signal please refer to table Table 4-3. Analog MUX Selection Table*/
    uint8 const u8MUXsel[8] =
    { 0x01U, 0x02U, 0x04U, 0x08U, 0x10U, 0x20U, 0x40U, 0x80U };
    uint8 u8diag_cfg_ctrl = 0U;
    boolean blRetVal = TRUE;

    if (AMUXSignal > AMUX_MAX){
    blRetVal = FALSE;
}
else
{
    EnabledAMUXSignal = AMUXSignal;
    /*enable MUX*/
    /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
    BFU8_SET(u8diag_cfg_ctrl, 1U, BF_MUX_EN_START, BF_MUX_EN_LENGTH);
    /*No active interconnect test*/
    /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
    BFU8_SET(u8diag_cfg_ctrl, NO_INTERCONNECT_TEST, BF_INT_CON_START,
            BF_INT_CON_LENGTH);
    /*select Analog MUX*/
    /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
    BFU8_SET(u8diag_cfg_ctrl, ANALOG_MUX, BF_MUX_CFG_START,
            BF_MUX_CFG_LENGTH);
    /*write to DIAG_CFG_CTRL register*/
    blRetVal =((TpsIf_SetRegisterVerify(TPS_DIAG_CFG_CTRL, u8diag_cfg_ctrl)) && blRetVal);
    /*set the DIAG_MUX_SEL registers so as bring out the required Analog
     * signal on DIAG_OUT*/
    blRetVal = (( TpsIf_SetRegisterVerify(TPS_DIAG_MUX_SEL,
                            u8MUXsel[AMUXSignal])) && blRetVal);
}

    return blRetVal;
}

boolean TPS_DisableMUXDiagnostic(void)
{
    /*reset the global EnabledAMUXSignal and EnabledDMUXSignal*/
    /*set default values for the AMUX and DMUX globals*/
    EnabledAMUXSignal = VDD5;
    EnabledDMUXSignal = AVDD_UVN;
    /*clear the diag_cfg_ctrl register to disable the MUX*/
    return TpsIf_SetRegisterVerify(TPS_DIAG_CFG_CTRL, 0U);
}

boolean TPS_CheckEnabledAMUXSignalLimits(TPS_Test_Result * Voltage_limit_test)
{
    uint8 u8diag_cfg_ctrl = 0U, u8MUX_EN, u8MUX_CFG = 0U, u8AMUXSignal = 0U;
    uint32 u32temp;
    boolean blRetVal = TRUE;
    uint16 u16DiagnosticPinSampledData = 0U;
    const TPS_VoltageRailLimits * const VoltageLimits[8] =
    {
            &TPSAMUXVoltageRailLimits.VDD5_Limits,
            &TPSAMUXVoltageRailLimits.VDD6_Limits,
            &TPSAMUXVoltageRailLimits.VCP_Limits,
            &TPSAMUXVoltageRailLimits.VSFB1_Limits,
            &TPSAMUXVoltageRailLimits.VBAT_SAFING_Limits,
            &TPSAMUXVoltageRailLimits.VBAT_Limits,
            &TPSAMUXVoltageRailLimits.MAIN_BG_Limits,
            &TPSAMUXVoltageRailLimits.VMON_BG_Limits };

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(Voltage_limit_test))
    {
        blRetVal = FALSE;
    }
    else
    {
        for (u32temp = 0U; u32temp < 60000U; u32temp++)
        {

        }
        /*add some delay so that the analog output is stable*/
        blRetVal = TpsIf_GetRegister(TPS_DIAG_CFG_CTRL, &u8diag_cfg_ctrl);
        blRetVal = ((TpsIf_GetRegister(TPS_DIAG_MUX_SEL, &u8AMUXSignal))
                && blRetVal);
        u8MUX_EN = BFU8_GET(u8diag_cfg_ctrl,BF_MUX_EN_START,BF_MUX_EN_LENGTH);
        u8MUX_CFG = BFU8_GET(u8diag_cfg_ctrl,BF_MUX_CFG_START,BF_MUX_CFG_LENGTH);

        if ((1U == u8MUX_EN) && (ANALOG_MUX == u8MUX_CFG) && (TRUE == blRetVal))
        {
            blRetVal =
                    ((GetTPSDiagPinSampledValue(&u16DiagnosticPinSampledData))
                            && blRetVal);
            if (blRetVal == TRUE)
            {
                if ((u16DiagnosticPinSampledData
                        > VoltageLimits[EnabledAMUXSignal]->u16voltage_min)
                        && (u16DiagnosticPinSampledData
                                < VoltageLimits[EnabledAMUXSignal]->u16voltage_max))
                {
                    *Voltage_limit_test = TEST_PASS;
                }
                else
                {
                    *Voltage_limit_test = TEST_FAIL;
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
    return blRetVal;
}

boolean TPS_EnableDMUXSignal(TPS_DMUXSignals DMUXSignal)
{
    uint8 u8diag_cfg_ctrl = 0U;
    boolean blRetVal = TRUE;

    if (((DMUXSignal > DMUX_MAX))||
    (RSV1 == DMUXSignal)||
    (RSV2 == DMUXSignal)||
    (RSV3 == DMUXSignal)||
    (RSV4 == DMUXSignal)||
    (RSV5 == DMUXSignal)||
    (DFT1 == DMUXSignal)||
    (DFT2 == DMUXSignal)||
    (DFT3 == DMUXSignal)||
    (DFT4 == DMUXSignal))
    {
        blRetVal = FALSE;
    }
    else
    {
        EnabledDMUXSignal = DMUXSignal;
        /*enable MUX*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8diag_cfg_ctrl, 1U,
        BF_MUX_EN_START, BF_MUX_EN_LENGTH);
        /*No active interconnect test*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8diag_cfg_ctrl, NO_INTERCONNECT_TEST,
        BF_INT_CON_START, BF_INT_CON_LENGTH);
        /*select digital MUX*/
        /*SAFETYMCUSW 9 S MR: 12.2 <APPROVED> "Comment_2"*/
        BFU8_SET(u8diag_cfg_ctrl, DIGITAL_MUX,
        BF_MUX_CFG_START, BF_MUX_CFG_LENGTH);
        /*write to DIAG_CFG_CTRL register*/
        blRetVal = TpsIf_SetRegisterVerify(TPS_DIAG_CFG_CTRL, u8diag_cfg_ctrl);
        /*set the DIAG_MUX_SEL registers so as bring out the required Digital
         * signal on DIAG_OUT*/
        blRetVal = ((TpsIf_SetRegisterVerify(TPS_DIAG_MUX_SEL, DMUXSignal)) && blRetVal);
    }

    return blRetVal;
}

boolean TPS_FaultInjectCRC(void)
{
    uint32 u32dataforcrc[2] =
    { 0U, 0U };
    uint8 u8crcvalue = 0U;
    boolean blRetVal = TRUE;

    /*Initialize the 64 bit data string which used for the calculation of CRC-8*/
    if (TRUE == TPS_InitializeDatastringforCRCCaclcuation(u32dataforcrc))
    {
		blRetVal = TPS_CalculateCRC8(u32dataforcrc, &u8crcvalue);
        blRetVal = ((TPS_ConfigureSafetyCheckControl(CFG_CRC, FALSE))
               && blRetVal);
        /*writing a wrong value of CRC to SAFETY_CFG_CRC register(add 1U to actual value)*/
        blRetVal = ((TPS_UpdateRegisterSafetyCfgCRC(u8crcvalue + 1U))
                && blRetVal);
        blRetVal =
             ((TPS_ConfigureSafetyCheckControl(CFG_CRC, TRUE)) && blRetVal);

    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

boolean TPS_ConfigureVDD5(boolean blEnableVDD5)
{
    boolean blRetVal = TRUE;

    if (TRUE == blEnableVDD5)
    {
        /*with ignition power down device will not power down when bit is set to 1U*/
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SENS_CTRL,
                BF_VDD5_EN_START, BF_VDD5_EN_LENGTH, (uint8)1U);

    }
    else
    {
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SENS_CTRL,
                BF_VDD5_EN_START, BF_VDD5_EN_LENGTH, 0U);
    }

    return blRetVal;
}

boolean TPS_ConfigureVSOUT1(boolean blEnableVSOUT1)
{
    boolean blRetVal = TRUE;

    if (TRUE == blEnableVSOUT1)
    {
        /*with ignition power down device will not power down when bit is set to 1U*/
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SENS_CTRL,
                BF_VSOUT1_EN_START, BF_VSOUT1_EN_LENGTH, (uint8)1U);

    }
    else
    {
        blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SENS_CTRL,
                BF_VSOUT1_EN_START, BF_VSOUT1_EN_LENGTH, (uint8)0U);
    }

    return blRetVal;
}

boolean TPS_GetDeviceInformation(TPS_DeviceInformation * device_information)
{
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(device_information))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegisterBitField(TPS_DEV_REV,
                BF_MINOR_VERSION_START, BF_MINOR_VERSION_LENGTH,
                &device_information->u8dev_minor_version);
        blRetVal = ((TpsIf_GetRegisterBitField(TPS_DEV_REV,
                BF_MAJOR_VERSION_START, BF_MAJOR_VERSION_LENGTH,
                &device_information->u8dev_major_version)) && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_DEV_ID,
                &device_information->u8dev_id)) && blRetVal);
    }
    return blRetVal;
}

boolean TPS_GetVMONStatus(TPS_OverallVMONStatus * vmon_status)
{
    uint8 u8vmon_stat_1 = 0U, u8vmon_stat_2 = 0U, u8safety_stat1 = 0U, u8safety_stat_4 = 0U;
    boolean blRetVal = TRUE;
    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(vmon_status))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegister(TPS_VMON_STAT_1, &u8vmon_stat_1);
        blRetVal = ((TpsIf_GetRegister(TPS_VMON_STAT_2, &u8vmon_stat_2))
                && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_STAT_1, &u8safety_stat1))
                && blRetVal);
        blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_STAT_4, &u8safety_stat_4))
                && blRetVal);

        vmon_status->blbandgap1_err =
                (boolean)BFU8_GET(u8vmon_stat_1,BF_BG1_ERR_START,BF_BG1_ERR_LENGTH);
        vmon_status->blbandgap2_err =
                (boolean)BFU8_GET(u8vmon_stat_1,BF_BG2_ERR_START,BF_BG2_ERR_LENGTH);
        vmon_status->blvcp17_ov =
                (boolean)BFU8_GET(u8vmon_stat_1,BF_VCP17_OV_START,BF_VCP17_OV_LENGTH);
        vmon_status->vbatp.blOverVoltage =
                (boolean)BFU8_GET(u8vmon_stat_1,BF_VBATP_OV_START,BF_VBATP_OV_LENGTH);
        vmon_status->vbatp.blUnderVoltage =
                (boolean)BFU8_GET(u8vmon_stat_1,BF_VBATP_UV_START,BF_VBATP_UV_LENGTH);
        vmon_status->vcp12.blOverVoltage =
                (boolean)BFU8_GET(u8vmon_stat_1,BF_VCP12_OV_START,BF_VCP12_OV_LENGTH);
        vmon_status->vcp12.blUnderVoltage =
                (boolean)BFU8_GET(u8vmon_stat_1,BF_VCP12_UV_START,BF_VCP12_UV_LENGTH);
        vmon_status->blavdd_vmon_err =
                (boolean)BFU8_GET(u8vmon_stat_1,BF_AVDD_VMON_ERR_START,BF_AVDD_VMON_ERR_LENGTH);
        vmon_status->vdd6.blOverVoltage =
                (boolean)BFU8_GET(u8vmon_stat_2,BF_VDD6_OV_START,BF_VDD6_OV_LENGTH);
        vmon_status->vdd6.blUnderVoltage =
                (boolean)BFU8_GET(u8vmon_stat_2,BF_VDD6_UV_START,BF_VDD6_UV_LENGTH);
        vmon_status->vdd5.blOverVoltage =
                (boolean)BFU8_GET(u8vmon_stat_2,BF_VDD5_OV_START,BF_VDD5_OV_LENGTH);
        vmon_status->vdd5.blUnderVoltage =
                (boolean)BFU8_GET(u8vmon_stat_2,BF_VDD5_UV_START,BF_VDD5_UV_LENGTH);
        vmon_status->vdd3_5.blOverVoltage =
                (boolean)BFU8_GET(u8vmon_stat_2,BF_VDD3_5_OV_START,BF_VDD3_5_OV_LENGTH);
        vmon_status->vdd3_5.blUnderVoltage =
                (boolean)BFU8_GET(u8vmon_stat_2,BF_VDD3_5_UV_START,BF_VDD3_5_UV_LENGTH);
        vmon_status->vdd1.blOverVoltage =
                (boolean)BFU8_GET(u8vmon_stat_2,BF_VDD1_OV_START,BF_VDD1_OV_LENGTH);
        vmon_status->vdd1.blUnderVoltage =
                (boolean)BFU8_GET(u8vmon_stat_2,BF_VDD1_UV_START,BF_VDD1_UV_LENGTH);
        vmon_status->vsout1.blOverVoltage =
                (boolean)BFU8_GET(u8safety_stat1,BF_VSOUT1_OV_START,BF_VSOUT1_OV_LENGTH);
        vmon_status->vsout1.blUnderVoltage =
                (boolean)BFU8_GET(u8safety_stat1,BF_VSOUT1_UV_START,BF_VSOUT1_UV_LENGTH);
        vmon_status->blVMON_TrimError =
                (boolean)BFU8_GET(u8safety_stat_4,BF_VMON_TRIM_ERR_START,BF_VMON_TRIM_ERR_LENGTH);
    }

    return blRetVal;
}

boolean TPS_GetJnTempandCurrentLimitStatus(
        TPS_JnTempCurrentLimitStatus * JnTempCurrentLtMonitoring)
{
    uint8 u8safety_stat1 = 0U;
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(JnTempCurrentLtMonitoring))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegister(TPS_SAFETY_STAT_1, &u8safety_stat1);
        JnTempCurrentLtMonitoring->blvdd3_5_ot =
                (boolean)BFU8_GET(u8safety_stat1,BF_VDD3_5_OT_START,BF_VDD3_5_OT_LENGTH);
        JnTempCurrentLtMonitoring->blvdd5_ilim =
                (boolean)BFU8_GET(u8safety_stat1,BF_VDD5_ILIM_START,BF_VDD5_ILIM_START);
        JnTempCurrentLtMonitoring->blvdd5_ot =
                (boolean)BFU8_GET(u8safety_stat1,BF_VDD5_OT_START,BF_VDD5_OT_LENGTH);
        JnTempCurrentLtMonitoring->blvsout1_ilim =
                (boolean)BFU8_GET(u8safety_stat1,BF_VSOUT1_ILIM_START,BF_VSOUT1_ILIM_LENGTH);
        JnTempCurrentLtMonitoring->blvsout1_ot =
                (boolean)BFU8_GET(u8safety_stat1,BF_VSOUT1_OT_START,BF_VSOUT1_OT_LENGTH);
        JnTempCurrentLtMonitoring->blvdd3_5_ilim =
                (boolean)BFU8_GET(u8safety_stat1,BF_VDD3_5_ILIM_START,BF_VDD3_5_ILIM_LENGTH);
    }
    return blRetVal;
}
boolean TPS_GetTPSDeviceConfiguration(
        TPS_DevCfg_reg_settings * config_register_settings)
{
    uint8 u8devcfg1 = 0U, u8devcfg2 = 0U;
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(config_register_settings))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegister(TPS_DEV_CFG1, &u8devcfg1);
        blRetVal = ((TpsIf_GetRegister(TPS_DEV_CFG2, &u8devcfg2)) && blRetVal);

        config_register_settings->blMASK_VBAT_OV =
                (boolean)BFU8_GET(u8devcfg2,BF_MASK_VBAT_OV_START,BF_MASK_VBAT_OV_LENGTH);
        config_register_settings->blMASK_VDD1_UV =
                (boolean)BFU8_GET(u8devcfg1,BF_nMASK_VDD1_UV_OV_START,BF_nMASK_VDD1_UV_OV_LENGTH);
        config_register_settings->blPOST_RUN_RST =
                (boolean)BFU8_GET(u8devcfg2,BF_POST_RUN_RST_START,BF_POST_RUN_RST_LENGTH);
        config_register_settings->blVDD3_5_OT =
                (boolean)BFU8_GET(u8devcfg2,BF_EN_VDD3_5_OT_START,BF_EN_VDD3_5_OT_LENGTH);
        config_register_settings->blVDD5_OT =
                (boolean)BFU8_GET(u8devcfg2,BF_EN_VDD5_OT_START,BF_EN_VDD5_OT_LENGTH);
    }

    return blRetVal;
}

boolean TPS_GetSPIErrorStatus(TPS_SPIErrorType * spi_error_status)
{
    uint8 u8safety_stat4 = 0U;
    boolean blRetVal = TRUE;
    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(spi_error_status))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegister(TPS_SAFETY_STAT_4, &u8safety_stat4);
        *spi_error_status =
                (TPS_SPIErrorType) BFU8_GET(u8safety_stat4,BF_SPI_ERR_START,BF_SPI_ERR_LENGTH);
    }
    return blRetVal;
}

boolean TPS_GetDEV_STATE(TPS_DeviceState * dev_state)
{
    uint8 u8dev_stat = 0U;
    boolean blRetVal = TRUE;

    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(dev_state))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegister(TPS_DEV_STAT, &u8dev_stat);
        dev_state->blLatchedCanWakeUP =
                (boolean)BFU8_GET(u8dev_stat,BF_CANWU_L_START,BF_CANWU_L_LENGTH);
        dev_state->blDeglitchedIGNPin =
                (boolean)BFU8_GET(u8dev_stat,BF_IGN_START,BF_IGN_LENGTH);
    }
    return blRetVal;
}

boolean TPS_GetNRESErrorStatus(boolean* blNRESError)
{
    boolean blRetVal = TRUE;
    uint8 u8bitavalue = 0U;
    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(blNRESError))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegisterBitField(TPS_SAFETY_ERR_STAT,
                BF_NRES_ERR_START, BF_NRES_ERR_LENGTH, &u8bitavalue);
        if (1U == u8bitavalue)
        {
            *blNRESError = TRUE;
        }
        else
        {
            *blNRESError = FALSE;
        }
    }
    return blRetVal;
}

boolean TPS_GetENDRVErrorStatus(boolean* blENDRVError)
{
    boolean blRetVal = TRUE;
    uint8 u8bitavalue = 0U;
    /*Check that pointer values falls in range*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(blENDRVError))
    {
        blRetVal = FALSE;
    }
    else
    {
        blRetVal = TpsIf_GetRegisterBitField(TPS_SAFETY_STAT_4,
                BF_ENDRV_ERR_START, BF_ENDRV_ERR_LENGTH, &u8bitavalue);
        if (1U == u8bitavalue)
        {
            *blENDRVError = TRUE;
        }
        else
        {
            *blENDRVError = FALSE;
        }
    }
    return blRetVal;
}

boolean TPS_FaultInjectWD(TPS_WDFaultType WDFault)
{
    boolean blRetVal = TRUE;
    switch (WDFault)
    {
        case BAD_ANSWER:
            /*0xEEU is a bad answer is never a appropriate watchdog response*/
            blRetVal = TpsIf_SetRegister(TPS_WDT_ANSWER, (uint8)0xEEU);
            break;
        default:
            blRetVal = FALSE;
            break;
    }
    return blRetVal;
}

boolean TPS_TestErrorPinMonitoring(TPS_Test_Result * ErrorPinMonitoring_Test)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;
    boolean blErrorPinFailFlag = FALSE;
    uint8 u8bckup_safety_check_control = 0U, u8bckup_safety_err_pwm_l = 0U,
            u8bckup_safety_func_cfg = 0U;
    uint32 u32maxwait = 0xffffU;
    TPS_ERRORPinPWMWidthScaler pwm;
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    /*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Comment_3"*/
    if (!CHECK_RANGE_RAM_PTR(ErrorPinMonitoring_Test))
    {
        blRetVal = FALSE;
    }
    else
    {
		blRetVal = ((TPS_GetCurrentTPSDeviceState(&u8tps_device_state))
				&& blRetVal);
		if (TPS_DEVICE_DIAGNOSTIC == u8tps_device_state) {
			/*set error pin low monitoring duration to 15us*/
			pwm.u8PWM_LOW = 0U;

			/*backup registers*/
			blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_CHECK_CTRL,
					&u8bckup_safety_check_control)) && blRetVal);
			blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_ERR_PWM_L,
					&u8bckup_safety_err_pwm_l)) && blRetVal);
			blRetVal = ((TpsIf_GetRegister(TPS_SAFETY_FUNC_CFG,
					&u8bckup_safety_func_cfg)) && blRetVal);

			/*configure error monitoring to monitor low pulse of error pin*/
			blRetVal = ((TPS_ConfigureErrorMonitoring(ERROR_PIN_MODE, pwm))
					&& blRetVal);
			blRetVal = ((TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_CHECK_CTRL,
			BF_NO_ERROR_START, BF_NO_ERROR_LENGTH, 0U)) && blRetVal);
			sl_esmREG->EKR = ESM_NERROR_SET; /*start error force*/
			do {
				blRetVal = ((TPS_GetErrorPinFailureStatusFlag(
						&blErrorPinFailFlag)) && blRetVal);
				u32maxwait--;
			} while ((FALSE == blErrorPinFailFlag) && (TRUE == blRetVal)
					&& (0U != u32maxwait));
			if (0U == u32maxwait) {
				*ErrorPinMonitoring_Test = TEST_FAIL;
			} else {
				*ErrorPinMonitoring_Test = TEST_PASS;
			}

			sl_esmREG->EKR = ESM_NERROR_CLEAR; /*start error force*/
			TPS_ClearErrorPinFailureStatusFlag();
			/*restore safety function cfg register*/
			blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_FUNC_CFG,
					u8bckup_safety_func_cfg)) && blRetVal);
			/*update the init_configuration structure to support register readback*/
			init_configuration.u8safety_func_cfg = u8bckup_safety_func_cfg
					& SAFETY_FUNC_CFG_MASK;

			/*restore pwm registers*/
			blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_ERR_PWM_L,
					u8bckup_safety_err_pwm_l)) && blRetVal);
			/*update the init_configuration structure to support register readback*/
			init_configuration.u8safety_err_pwm_l = u8bckup_safety_err_pwm_l;

			/*restore safety check control*/
			blRetVal = ((TpsIf_SetRegisterVerify(TPS_SAFETY_CHECK_CTRL,
					u8bckup_safety_check_control)) && blRetVal);

		} else {
			blRetVal = FALSE;
		}
	}
    return blRetVal;
}

boolean TPS_GoToSafeState(void)
{
    boolean blRetVal = TRUE;
    uint8 u8tps_device_state = 0U;

	blRetVal = TPS_GetCurrentTPSDeviceState(&u8tps_device_state);
    if ((TRUE == blRetVal) && (TPS_DEVICE_ACTIVE == u8tps_device_state))
    {
		blRetVal = TpsIf_SetRegisterBitFieldVerify(TPS_SAFETY_CHECK_CTRL,
		BF_NO_ERROR_START, BF_NO_ERROR_LENGTH, 0U);
        sl_esmREG ->EKR = ESM_NERROR_SET; /*start error force*/
    }
    else
    {
        blRetVal = FALSE;
    }
    return blRetVal;
}

/*Comment_1:
 * "Reason -  The function pointer is gets updated in the PAI call TPS_DriverInit"*/

/*Comment_2:
 * "Assignment in an expression is valid here"*/

/*Comment_3:
 * "Reason -  This is an advisory by MISRA.Verified validity of operation by review"*/

/*Comment_4:
 * "Needed, to avoid implicit type-casting"*/
