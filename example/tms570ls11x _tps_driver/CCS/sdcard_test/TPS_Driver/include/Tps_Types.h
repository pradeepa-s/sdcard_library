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
 *         @file       Tps_Types.h
 *         @version    2.2.0
 *         @component  TPS
 *         @module     TPS Library
 *         @generator  No Configuration
 *
 *         @brief      This files contains the types used by the TPS Library API's
 *         @details    todo
 *                
 * 
 *---------------------------------------------------------------------------------------------------------------------
 * @author  Manoj
 *---------------------------------------------------------------------------------------------------------------------
 * Revision History
 *---------------------------------------------------------------------------------------------------------------------
   |Version         | Date        | Author           |    Change ID       | Description                         |
   |---------------:|:-----------:|:----------------:|:------------------:|:------------------------------------|
   |2.2.0|  20Dec2012  | Manoj R          |    00000000000     | Initial version created             |
*/

#ifndef TPS_TYPES_H_
#define TPS_TYPES_H_
/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "sl_hal_stdtypes.h"
/*********************************************************
Defines / data types / structs / unions /enums
**********************************************************/
/**
  * \enum     TPS_Registers
  *
  * \brief    Enumeration used for the TPS registers.This us used as cross reference for the commands used for the register write and read.
  */
typedef enum _TPS_Registers
{
    TPS_DEV_REV,                /**< 0x00: Alias for device revision register                    */
    TPS_DEV_ID,                 /**< 0x01: Alias for device identifier register                  */
    TPS_DEV_STAT,               /**< 0x02: Alias for device status register                      */
    TPS_DEV_CFG1,               /**< 0x03: Alias for config 1 register                           */
    TPS_DEV_CFG2,               /**< 0x04: Alias for config 2 register                           */
    TPS_VMON_STAT_1,            /**< 0x05: Alias for voltage monitor status 1 register           */
    TPS_VMON_STAT_2,            /**< 0x06: Alias for voltage monitor status 2 register           */
    TPS_SAFETY_STAT_1,          /**< 0x07: Alias for safety status 1 register                    */
    TPS_SAFETY_STAT_2,          /**< 0x08: Alias for safety status 2 register                    */
    TPS_SAFETY_STAT_3,          /**< 0x09: Alias for safety status 3 register                    */
    TPS_SAFETY_STAT_4,          /**< 0x0A: Alias for safety status 4 register                    */
    TPS_SAFETY_STAT_5,          /**< 0x0B: Alias for safety status 5 register                    */
    TPS_SAFETY_ERR_CFG,         /**< 0x0C: Alias for safety error config register                */
    TPS_SAFETY_BIST_CTRL,       /**< 0x0D: Alias for safety BIST control register                */
    TPS_SAFETY_CHECK_CTRL,      /**< 0x0E: Alias for safety check control register               */
    TPS_SAFETY_FUNC_CFG,        /**< 0x0F: Alias for safety function config register             */
    TPS_SAFETY_ERR_STAT,        /**< 0x10: Alias for safety error status register                */
    TPS_SAFETY_ERR_PWM_H,       /**< 0x11: Alias for safety error pwm high register              */
    TPS_SAFETY_ERR_PWM_L,       /**< 0x12: Alias for safety error pwm low register               */
    TPS_SAFETY_PWD_THR_CFG,     /**< 0x13: Alias for safety power down threshold config register */
    TPS_SAFETY_CFG_CRC,         /**< 0x14: Alias for safety config crc register                  */
    TPS_DIAG_CFG_CTRL,          /**< 0x15: Alias for diagnostic config control register          */
    TPS_DIAG_MUX_SEL,           /**< 0x16: Alias for diagnostic mux select register              */
    TPS_WDT_TOKEN_FDBCK,        /**< 0x17: Alias for watchdog token feedback register            */
    TPS_WDT_WIN1_CFG,           /**< 0x18: Alias for watchdog open window config register        */
    TPS_WDT_WIN2_CFG,           /**< 0x19: Alias for watchdog close window config register       */
    TPS_WDT_TOKEN_VALUE,        /**< 0x1A: Alias for watchdog token register                     */
    TPS_WDT_STATUS,             /**< 0x1B: Alias for watchdog status register                    */
    TPS_WDT_ANSWER,             /**< 0x1C: Alias for watchdog answer register                    */
    TPS_SENS_CTRL,              /**< 0x1D: Alias for sense control register                      */
    TPS_SW_LOCK,                /**< 0x1E: Alias for register lock register                      */
    TPS_SW_UNLOCK               /**< 0x1F: Alias for register unlock register                    */
}TPS_Registers;
/**
  * \enum     TPS_Test_Result
  *
  * \brief    Enumberation to indicate the status of a test
  */
typedef enum _TPS_Test_Result {
    TEST_FAIL   = 0xABU,     /**< Test failed */
    TEST_PASS   = 0xCDU      /**< Test Passed */
} TPS_Test_Result;


/**
  * \enum     TPS_SafetyCheckControl
  *
  * \brief    Enumeration for the different safety check controls in the TPS driver
  */
typedef enum _TPS_SafetyCheckControl {
    CFG_CRC   = 0U,          /**< crc enable disable.Once enabled it takes\n
                                  2.1us for completion of the crc check.*/
    DRV_OUTPUT,              /**< DRV output  of TPS device*/
    ERROR_PIN_MONITORING,    /**< MCU error pin monitoring */
    DIAG_EXIT_MASK,          /**< diag exit mask which disables the exit from\n
                                  the diagnostic mode of the TPS */
    DIAG_EXIT                /**< diag exit on enabling this the TPS exits\n
                                  diagnostic mode and enters active mode(when\n
                                  the diag exit mask is not set*/
} TPS_SafetyCheckControl;

/**
  * \enum     TPS_ErrorPinMonitoringMode
  *
  * \brief    Enumeration for error pin monitoring mode
  */
typedef enum _TPS_ErrorPinMonitoringMode {
    PWM_MONITORING_MODE   = 0U,     /**< PWM monitoring mode for the MCU error signal monitoring*/
    ERROR_PIN_MODE                 /**< MCU error signal low monitoring*/
} TPS_ErrorPinMonitoringMode;

/** \struct TPS_ERRORPinPWMWidthScaler
  *
  * \brief TPS_ERRORPinPWMWidthScaler  structure to be used with TPS_ConfigureErrorMonitoring API
  *
  */
typedef struct _TPS_ERRORPinPWMWidthScaler{
        uint8 u8PWM_HIGH;          /**< PWM high phase width scaler,Supported values 0-0xFF */
        uint8 u8PWM_LOW;           /**< PWM low phase width scalerSupported values 0-0xFF */
}TPS_ERRORPinPWMWidthScaler;
/**
  * \enum     TPS_WatchdogConfiguration
  *
  * \brief    Enumeration Watchdog mode configuration
  */
typedef enum _TPS_WatchdogMode {
    WDTI_MODE   = 0U,     /**< Watchdog trigger input configuration*/
    QANDA_MODE           /**< Watchdog question and answer mode*/
} TPS_WatchdogMode;

/**
  * \enum     TPS_VDD_3_5_SEL
  *
  * \brief    Enumeration for Voltage select
  */
typedef enum _TPS_VDD_3_5_SEL {
    SEL_5V   = 0U,     /**< select the 5v voltage source*/
    SEL_3_3V           /**< select the 3.3 volt voltage source*/
} TPS_VDD_3_5_SEL;

/**
  * \enum     TPS_HandleConfigurationRegisters
  *
  * \brief    Enumeration locking and unlocking of registers
  */
typedef enum _TPS_HandleConfigurationRegisters {
    LOCK   = 0U,     /**< select the 5v voltage source*/
    UNLOCK           /**< select the 3.3 volt voltage source*/
} TPS_HandleConfigurationRegisters;



/**
  * \enum     TPS_DeviceConfigRegisterSettings
  *
  * \brief    Enumerations indicating the different flags available in the TPS device configuration registers.\n
  *           This enumeration is used as parameter to the API TPS_SetTPSDeviceConfigurationRegisters.
  */
typedef enum _TPS_DeviceConfigRegisterSettings {
    MASK_VDD1_UV   = 0U,    /**< nMASK_VDD1_UV register field of DEV_CFG1 register*/
    VDD3_5_OT,              /**< EN_VDD3/5_OT register field of DEV_CFG2 register*/
    VDD5_OT,                /**< EN_VDD5_OT register field of DEV_CFG2 register */
    MASK_VBAT_OV,           /**< MASK_VBAT_OV register field of DEV_CFG2 register */
    POST_RUN_RST           /**< POST_RUN_RST register field of DEV_CFG2 register */
} TPS_DeviceConfigRegisterSettings;

/**
  * \enum     TPS_SafetyBISTControlRegisterSettings
  *
  * \brief    Enumerations indicating the different flags available in the TPS SAFETY_BIST_CTRL registers.\n
  *           This enumeration is used as parameter to the API TPS_SetTPSDeviceConfigurationRegisters.
  */
typedef enum _TPS_SafetyBISTControlRegisterSettings {
    EE_CRC_CHK = 0,      /**< EE_CRC_CHECK field of the SAFETY_BIST_CTRL register */
    AUTO_BIST       /**< AUTO_BIST field of the SAFETY_BIST_CTRL register */
} TPS_SafetyBISTControlRegisterSettings;

/**
  * \enum     TPS_BistTypes
  *
  * \brief    Enumerations indicating the different BISTS in the TPS Device.\n
  *           This enumeration is used as parameter to the API TPS_StartBIST.
  */
typedef enum _TPS_BistTypes {
    ABIST = 0,      /**< Analog built in selftest*/
    LBIST          /**< Logic built in selftest */
} TPS_BistTypes;

/**
  * \enum     TPS_WatchdogErrorStatus
  *
  * \brief    structure for collecting the watchdog error status.
  *
  */
typedef struct _TPS_WatchdogErrorStatus{
    boolean blTokenError;      /**< Token error*/
    boolean blWdWrongConfig;   /**< wrong open and closing window timings */
    boolean blSeqError;        /**< Token sequence error */
    boolean blTimeOutError;    /**< Timeout watchdog */
    boolean blTokenEarlyError; /**< Early token received */
} TPS_WatchdogErrorStatus;

/**
  * \enum     TPS_BistStatus
  *
  * \brief    Enumerations indicating the Running status of the BIST.
  */
typedef enum _TPS_TestRunningStatus {
    OVER = 0,      /**< Test is over*/
    RUNNING       /**< Test is running */
}TPS_TestRunningStatus;
/** \struct TPS_ABIST_Result
  *
  * \brief ABIST result  information to be used with TPS_GetABISTTestStatus API
  *
  */
typedef struct _TPS_ABIST_Result {
        TPS_Test_Result ABIST_Result;            /**< Overall ABIST Test result */
        TPS_Test_Result UVOV_Test;              /**< If Overall test result is TEST_FAIL then this field is valid and indicates Under\n
                                                      voltage and Over Voltage test result*/
        TPS_Test_Result LOCLK_Test;              /**< If Overall test result is TEST_FAIL then this field is valid and indicates Loss of \n
                                                      Oscillator clock test result*/
}TPS_ABIST_Result;

/**
  * \enum     TPS_AmuxSignals
  *
  * \brief    Enumerations indicating the AMUX diag out signal.
  */
typedef enum _TPS_AMUXSignals {
    VDD5 = 0,      /**< VDD5 AMUX signal*/
    VDD6,          /**< VDD6 AMUX signal*/
    VCP,           /**< external charge pump(VCP) AMUX signal*/
    VSFB1,         /**< Sensor supply feedback voltage AMUX signal*/
    VBAT_SAFING,   /**< safing battery supply AMUX signal*/
    VBAT,          /**< battery supply AMUX signal*/
    MAIN_BG,       /**< regulators band-gap reference AMUX signal*/
    VMON_BG        /**< Voltage-monitor band gap AMUX signal*/
} TPS_AMUXSignals;

/** \defgroup grp_amux_signals AMUX signal enumeration min and max values
 *  Following macros are used to define minimum and  maximum values of the AMUX signals
 *
 */
/**@{*/
/*SAFETYMCUSW 79 S MR:19.4 <APPROVED> "Reason -  Reviewed and the value assigned is an enumeration"*/
#define AMUX_MIN          (VDD5)     /**< starting AMUX signal */
/*SAFETYMCUSW 79 S MR:19.4 <APPROVED> "Reason -  Reviewed and the value assigned is an enumeration"*/
#define AMUX_MAX          (VMON_BG)     /**< ending AMUX signal*/
/**@}*/

/**
  * \enum     TPS_DmuxSignals
  *
  * \brief    Enumerations indicating the DMUX diag out signal.(the singals of \n
  *           group 5 are not supported.Please refer to table Table 4-4 to 4-7.
  * **Note: DMUX signals of GROUP 5 are not available for diagnostics**
  */
typedef enum _TPS_DMUXSignals {
    RSV1 = 0        ,     /**<Reserved, logic 0                              */
    AVDD_UVN        ,     /**<AVDD undervoltage comparator output            */
    BG_ERR1         ,     /**<VMON or main band gap is OFF                   */
    BG_ERR2         ,     /**<VMON or main band gap is OFF                   */
    VCP12_NUV       ,     /**<VCP12 charge-pump undervoltage comparator      */
    VCP12_OV        ,     /**<VCP12 charge-pump overvoltage comparator       */
    VCP17_OV        ,     /**<VCP17 charge-pump overvoltage comparator       */
    VDD6_NUV        ,     /**<VDD6 undervoltage comparator                   */
    VDD6_OV         ,     /**<VDD6 overvoltage comparator                    */
    VDD5_NUV        ,     /**<VDD5 undervoltage comparator                   */
    VDD5_OV         ,     /**<VDD5 overvoltage comparator                    */
    VDD3_5_NUV      ,     /**<VDD3/5 undervoltage comparator                 */
    VDD3_5_OV       ,     /**<VDD3/5 overvoltage comparator                  */
    VDD1_NUV        ,     /**<VDD1 undervoltage comparator                   */
    VDD1_OV         ,     /**<VDD1 overvoltage comparator                    */
    LOCLK           ,     /**<Loss-of-system-clock comparator                */
    RSV2            ,     /**<Reserved, logic 0                              */
    SYS_CLK         ,     /**<System clock source                            */
    DFT1            ,     /**<Signal reserved for production test            */
    WDT_CLK         ,     /**<Watchdog clock reference (0.55-ms period time) */
    RST_EXT_CLK     ,     /**<Reset extension oscillator output              */
    T_5US           ,     /**<5-µs time reference                            */
    T_15US          ,     /**<15-µs time reference                           */
    T_40US          ,     /**<40-µs time reference                           */
    T_2MS           ,     /**<2-ms time reference                            */
    UC_ERROR_WDI    ,     /**<External MCU ERROR/WDI input pin               */
    SPI_NCS         ,     /**<SPI chip-select input pin                      */
    SPI_SDI         ,     /**<SPI slave-data input pin                       */
    SPI_CLK         ,     /**<SPI clock input pin                            */
    SDO_RDBCK       ,     /**<SPI slave-data output-pin readback             */
    UC_ERROR_WDI_RED,     /**<Same signal as 2.10                            */
    NRST_EXT_IN     ,     /**<NRES pin readback (reset to external MCU)      */
    RSV3            ,     /**<Reserved, logic 0                              */
    DFT2            ,     /**<Signal reserved for production test            */
    DFT3            ,     /**<Signal reserved for production test            */
    CP_OV           ,     /**<Charge-pump overvoltage comparator             */
    CP_UVN          ,     /**<Charge-pump undervoltage comparator            */
    CP_PH1          ,     /**<Charge-pump switching phase 1                  */
    CP_PH2          ,     /**<Charge-pump switching phase 2                  */
    CP_DIFF3V       ,     /**<Indicates VCP-VBATP > 3 V                      */
    DFT4            ,     /**<Signal reserved for production test            */
    VBAT_UVN        ,     /**<VBAT undervoltage comparator                   */
    VBATP_OV        ,     /**<VBAT overvoltage comparator                    */
    DMUX_VDD5_OT   ,      /**<VDD5 overtemperature                           */
    DMUX_VDD3_5_OT  ,     /**<VDD3/5 overtemperature                         */
    VSOUT_OT        ,     /**<VSOUT overtemperature                          */
    VDD5_CL         ,     /**<VDD5 current-limit                             */
    VDD3_CL         ,     /**<VDD3 current-limit                             */
    RSV4            ,     /**<Reserved, logic 0                              */
    VSOUT_ILIM      ,     /**<VSOUT1 current-limit                           */
    VSOUT_UVN       ,     /**<VSOUT1 undervoltage comparator                 */
    VSOUT_OV        ,     /**<VSOUT1 overvoltage comparator                  */
    DVDD_UV         ,     /**<DVDD undervoltage comparator                   */
    DVDD_OV         ,     /**<DVDD overvoltage comparator                    */
    RSV5            ,     /**<Reserved                                       */
    VS_TRK_MODE     ,     /**<VSOUT1 in track-mode indication                */
    VMON_TRIM_ER          /**<VMON trim error                                */
} TPS_DMUXSignals;

/** \defgroup grp_dmux_signals DMUX signal enumeration min and max values
 *  Following macros are used to define minimum and  maximum values of the AMUX signals
 *
 */
/**@{*/
/*SAFETYMCUSW 79 S MR:19.4 <APPROVED> "Reason -  Reviewed and the value assigned is an enumeration"*/
#define DMUX_MIN          (RSV1)     /**< starting DMUX signal */
/*SAFETYMCUSW 79 S MR:19.4 <APPROVED> "Reason -  Reviewed and the value assigned is an enumeration"*/
#define DMUX_MAX          (VMON_TRIM_ER)     /**< ending DMUX signal*/
/**@}*/

/** \struct TPS_VoltageRailLimits
 *
 * \brief TPS_VoltageRailLimits structure is used to represent the maximum,minmum and typical\n
 *        voltage value for particular voltage rail.It used used by the structure TPS_AllVoltageRailLimits.
 *
 */
typedef struct _TPS_VoltageRailLimits
{
       uint16 u16voltage_min;           /**< minimum voltage(the adc sampled value) for the voltage rail*/
       uint16 u16voltage_max;           /**< maximum voltage(the adc sampled value) for the voltage rail*/
} TPS_VoltageRailLimits;

/** \struct TPS_AllVoltageRailLimits
  *
  * \brief TPS_AllVoltageRailLimits structure is updated on TPS_Init and the structure is used for\n
  *        storing the limits for  difference Voltage Rails which are selectable on the DIAG_OUT \n
  *        pin using AMUX diagnostics.refer to table 4-3  for more information.
  *
  */
typedef struct _TPS_AllVoltageRailLimits
{
        TPS_VoltageRailLimits VDD5_Limits;          /**< VDD5(Linear VDD5 regulator output) rail voltage limits*/
        TPS_VoltageRailLimits VDD6_Limits;          /**< VDD6(Switch-mode pre-regulator) rail voltage limits*/
        TPS_VoltageRailLimits VCP_Limits;           /**< VCP(external charge pump) rail voltage limits*/
        TPS_VoltageRailLimits VSFB1_Limits;         /**< VSFB1(Sensor supply feedback voltage  signal) rail voltage limits*/
        TPS_VoltageRailLimits VBAT_SAFING_Limits;   /**< VBAT_SAFING(safing battery supply  signal) rail voltage limits*/
        TPS_VoltageRailLimits VBAT_Limits;          /**< VBAT(battery supply signal) rail voltage limits*/
        TPS_VoltageRailLimits MAIN_BG_Limits;       /**< MAIN_BG(regulators band-gap reference signal) rail voltage limits*/
        TPS_VoltageRailLimits VMON_BG_Limits;       /**< VMON_BG(Voltage-monitor band gap signal) rail voltage limits*/
}TPS_AllVoltageRailLimits;

/** \struct TPS_VMONStatus
  *
  * \brief TPS_VMONStatus structure is used for recording the Over voltage\n
  *        and under voltage state of the of voltage sources.
  */
typedef struct _TPS_VMONStatus
{
        boolean blOverVoltage;   /**< Over voltage status of the source voltage considered*/
        boolean blUnderVoltage;  /**< Under voltage status of the source voltage considered*/
}TPS_VMONStatus;

/** \struct TPS_OverallVMONStatus
  *
  * \brief TPS_OverallVMONStatus structure is used api TPS_GetVMONStatus\n
  *        and it is used for recording the complete voltage monitoring status\n
  *        of the TPS device.
  */
typedef struct _TPS_OverallVMONStatus
{
        TPS_VMONStatus vbatp;      /**< voltage monitoring of VBATP*/
        TPS_VMONStatus vcp12;      /**< voltage monitoring of VCP12*/
        TPS_VMONStatus vdd6;       /**< voltage monitoring of VDD6*/
        TPS_VMONStatus vdd5;       /**< voltage monitoring of VDD5*/
        TPS_VMONStatus vdd3_5;     /**< voltage monitoring of VDD3/5*/
        TPS_VMONStatus vdd1;       /**< voltage monitoring of VDD1*/
        TPS_VMONStatus vsout1;     /**< voltage monitoring of VDD1*/
        boolean blbandgap2_err;      /**< Reference band-gap 2 error*/
        boolean blbandgap1_err;      /**< Reference band-gap 1 error*/
        boolean blvcp17_ov;          /**< VCP17 over voltage status bit */
        boolean blavdd_vmon_err;   /**< voltage-monitor power -supply power-good status*/
        boolean blVMON_TrimError;  /**< mismatch voltage-monitor trim error*/
}TPS_OverallVMONStatus;

/** \struct TPS_OverallVMONStatus
  *
  * \brief TPS_OverallVMONStatus structure is used api TPS_GetVMONStatus\n
  *        and it is used for recording the complete voltage monitoring status\n
  *        of the TPS device.
  */
typedef struct _TPS_JnTempCurrentLimitStatus
{
        boolean blvdd5_ilim;
        boolean blvdd3_5_ilim;
        boolean blvsout1_ilim;
        boolean blvsout1_ot;
        boolean blvdd5_ot;
        boolean blvdd3_5_ot;
} TPS_JnTempCurrentLimitStatus;


/** \struct TPS_DevCfg_reg_settings
  *
  * \brief structure which can store the TPS device configuration register settings this structure
  *        updated with the relavant parameters is to be passed to the TPS_DriverInit API.
  *
  */
typedef struct _TPS_DevCfg_reg_settings {
        boolean blMASK_VDD1_UV;     /**< nMASK_VDD1_UV register field of DEV_CFG1 register*/
        boolean blVDD3_5_OT;        /**< EN_VDD3/5_OT register field of DEV_CFG2 register*/
        boolean blVDD5_OT;          /**< EN_VDD5_OT register field of DEV_CFG2 register */
        boolean blMASK_VBAT_OV;     /**< MASK_VBAT_OV register field of DEV_CFG2 register */
        boolean blPOST_RUN_RST;     /**< POST_RUN_RST register field of DEV_CFG2 register */
}TPS_DevCfg_reg_settings;

/** \struct TPS_Readback_Config
  *
  * \brief TPS_Readback_Config structure is updated on TPS_Init and the structure is used for\n
  *        read back and compare of important registers of the TPS
  *
  */
typedef struct _TPS_Config
{
    uint8 u8safety_func_cfg;
    uint8 u8safety_err_cfg;
    uint8 u8safety_pwd_thr;
    uint8 u8dev_cfg1;
    uint8 u8dev_cfg2;
    uint8 u8wdt_token_fbck;
    uint8 u8wdt_win1_cfg;
    uint8 u8wdt_win2_cfg;
    uint8 u8safety_err_pwm_l;
    uint8 u8safety_err_pwm_h;
}TPS_Config;

/** \struct TPS_WatchdogWindows
 *
 * \brief TPS watchdog open and close window configuration to be used with TPS_ConfigureWatchdogWindows API
 *
 */
typedef enum _TPS_WatchdogWindows {
        OPEN_WINDOW, /**< open window of the watchdog*/
        CLOSE_WINDOW /**< close window of the watchdog*/
}TPS_WatchdogWindows;

/** \struct TPS_CRCErrorStatus
 *
 * \brief The structure will contain the data regarding the CRC error in config registers/EEPROM
 *
 */
typedef struct _TPS_CRCErrorStatus {
       boolean blCfgCRCError;            /**< If TRUE indicates that there was CFG CRC error */
       boolean blEECRCError;             /**< If TRUE indicates that there was EEPROM CRC error*/
}TPS_CRCErrorStatus;

/** \struct TPS_DeviceInformation
 *
 * \brief The structure will contain the device information such as device revision,device id.
 *
 */
typedef struct _TPS_DeviceInformation {
       uint8 u8dev_minor_version;       /**< device minor version*/
       uint8 u8dev_major_version;       /**< device major version*/
       uint8 u8dev_id;                  /**< device id*/
}TPS_DeviceInformation;

/** \struct TPS_DeviceState
 *
 * \brief The structure will contain the device state information such as latched CAN wake-up and deglitched IGN pin.
 *
 */
typedef struct _TPS_DeviceState {
       boolean blLatchedCanWakeUP;       /**< Latched CAN wake-up event*/
       boolean blDeglitchedIGNPin;       /**< Deglitched IGN pin*/
}TPS_DeviceState;

/**
 * \enum     TPS_WDResponse
 *
 * \brief    Enumerations answer type used by the API TPS_SendWdgAnswer
 */
typedef enum _TPS_WDResponse
{
    WD_TOKEN_RESP3 = 0,                /**< Watcdog RESP3 which should be sent during the close window*/
    WD_TOKEN_RESP2,                    /**< Watcdog RESP2 which should be sent during the open window*/
    WD_TOKEN_RESP1,                    /**< Watcdog RESP1 which should be sent during the open window*/
    WD_TOKEN_RESP0                     /**< Watcdog RESP0 which should be sent during the open window*/
} TPS_WDResponse;

/**
 * \enum     TPS_SPIErrorType
 *
 * \brief    Enumerations indicating the type of the SPI error which occurred\n
 *           in the previous SPI transmission.
 */
typedef enum _TPS_SPIErrorType
{
    NO_ERROR = 0,                      /**< SPI no error*/
    COMMAND_ERROR,                     /**< Wrong SPI command*/
    FORMAT_ERROR,                      /**< format error(received bit count is not equal to 16*/
    DATA_OP_MISMATCH                   /**< Data output mismatch*/
} TPS_SPIErrorType;

/**
 * \enum     TPS_WDFaultType
 *
 * \brief    Enumerations indicating the Watchdog fault type to be injected
 */
typedef enum
{
    BAD_ANSWER                       /**< Inject bad watchdog answer fault*/
} TPS_WDFaultType;

/**
 * \enum     TPS_DebugSeverity
 *
 * \brief    Enumerations debug message severity.
 */
typedef enum
{
    DEBUG = 0,                       /**< The message severity level is debug*/
    ERROR,                           /**< The message severity level is error*/
    INFO                             /**< The message is an information to the user*/
} TPS_DebugSeverity;

/** \typedef send_recieve_data
  *
  * \brief Definition for transmit_recieve_data data API, which is provided by the user application
  * \description The application should support such an API and should pass the function pointer to TPS initialization routine.
  * \param u16TransmitData - data to be sent on the Configured MibSPI channel.
  * \param u32TransmitSize   - size of the data to be sent
  * \param u16RecieveData - data received  on the Configured MibSPI channel.
  * \param u32RecieveSize   - size of the data received
  */
/*SAFETYMCUSW 298 S <APPROVED> "Not a MISRA-C:2004 violation"*/
typedef boolean (* send_recieve_data) (const uint16 * const u16TransmitData,const uint32 u32TransmitSize,volatile uint16* u16RecieveData,uint32 * u32RecieveSize);
/** \typedef GetAmuxDiagosticPinSampleddata
  *
  * \brief Definition for Diagnostic pin sampled data API.The API provides the ADC sampled value of the Diagnostic pin of the TPS
  * \description The API provides the ADC sampled value of the Diagnostic pin of the TPS.\n
  *              The application should support such an API and should pass the function \n
  *              pointer to TPS initialization routine.
  * \param u16Data - ADC sampled data of the Diagnostic pin
  */
/*SAFETYMCUSW 298 S <APPROVED> "Not a MISRA-C:2004 violation"*/
typedef boolean (*GetTPSDiagosticPinSampledData) (uint16* u16Data);
/** \typedef SendDebugText
  *
  * \brief Definition for send debug text API, which is provided by the user application.
  * \description The application should support such an API which can recieve the debug text\n
  *              and use the information appropriately
  * \param textbuf  - text buffer containing debug text.
  * \param length   - length of the text buffer
  */
/*SAFETYMCUSW 298 S <APPROVED> "Not a MISRA-C:2004 violation"*/
typedef boolean (* SendDebugText) (const uint8 * const u8textbuf,const uint32 u8textlength);

/** \defgroup TPS_DeviceState Macros for indicating the current TPS device state
*
*  macros indicating the current TPS device state
*
*/
/**@{*/

#define TPS_DEVICE_STANDBY     ((uint8)0x0U)     /**< the value of field FSM in register SAFETY_STAT_5 for STANDBY state*/
#define TPS_DEVICE_RESET       ((uint8)0x3U)     /**< the value of field FSM in register SAFETY_STAT_5 for RESET state*/
#define TPS_DEVICE_DIAGNOSTIC  ((uint8)0x7U)     /**< the value of field FSM in register SAFETY_STAT_5 for DIAGNOSTIC state*/
#define TPS_DEVICE_ACTIVE      ((uint8)0x5U)     /**< the value of field FSM in register SAFETY_STAT_5 for ACTIVE state*/
#define TPS_DEVICE_SAFE        ((uint8)0x4U)     /**< the value of field FSM in register SAFETY_STAT_5 for SAFE state*/
/**@}*/

/** \struct TPS_WatchdoConfiguration
  *
  * \brief TPS watchdog configuration  to be used with TPS_WatcdogInit API.
  *
  */
typedef struct _TPS_WatchdoConfiguration
{
    TPS_WatchdogMode              watchdog_mode;                      /**< watchdog mode selection Q&A or WDTI mode*/
    boolean                       blwatchdog_reset;                   /**< watchdog reset enable/disable*/
    uint8                         u8TokenSeed;                        /**< watchdog token seed value,default value is 0(range 0x0 to 0xF)*/
    uint8                         u8TokenFDBK;                        /**< watchdog token feedback value,default value is 0(range 0x0 to 0xF)*/
    uint8                         u8open_windowtime_scaler;           /**< watchdog open window time scaler(range 0 - 0x7F).minimum open window\n
                                                                           duration is .55ms maximum is 70.4ms*/
    uint8                         u8close_windowtime_scaler;           /**< watchdog close window time scaler(range 0 - 01F).minimum open window\n
                                                                           duration is .55ms maximum is 17.6ms*/
}TPS_WatchdoConfiguration;

/** \struct TPS_Initilization_struct
  *
  * \brief TPS Initialization struct  to be used with TPS_Init API
  *
  */
typedef struct _TPS_Initilization_struct {
        send_recieve_data             send_recieve_data_TPS;              /**< SPI interfacing API(function pointer) send and receive data to be provided by application*/
        GetTPSDiagosticPinSampledData GetTPSDiagPinSampledValue;          /**< The API is provided by Application and provides the ADC sampled value of the\n
                                                                            Diagnostic Pin.If this is Null the diagnostic data is not sampled.*/
        boolean                       blbist_at_startup;                  /**<TRUE bist at startup is enabled */
        boolean                       blenable_drv_op;                    /**< IF TRUE ENABLE_DRV output is enabled */
        boolean                       blerror_pin_monitoring;             /**<TRUE error pin monitoring is enabled */
        boolean                       blignition_power_latch;             /**<TRUE ignition power latch is enabled*/
        boolean                       blNRES_Monitoring;                  /**<TRUE NRES pin monitoring is enabled*/
        boolean                       blsafe_state_timeout;              /**<TRUE to clear NO_SAFE_TO flag */
        uint8                         u8safe_lock_threshhold;             /**< device ERROR COUNT threshold valuejhsjkdfhsdf value range = 0x0 to 0xF*/
        uint8                         u8pwd_threshhold_value;             /**< device error count threshold value to power down the device*/
        uint8                         u8safe_state_timeout_scaler;        /**< value decides the safe state time out duration value range = 0x0 to 0x7\n
                                                                               minimum duration fo timeout is 22ms and maximum is 330ms please check the\n
                                                                               data sheet section 4.16.8 for more information*/
        TPS_ErrorPinMonitoringMode    error_pin_monitoring_mode;          /**< mode of error pin monitoring pwm or normal mode*/
        TPS_ERRORPinPWMWidthScaler    error_pin_pwm_width;                /**< pwm width settings for error pin monitoring\n
                                                                               For error pin monitoring mode the low width\n
                                                                               specifies the width of the error pin low pulse,the PWM_HIGH\n
                                                                               width is ignored*/
        TPS_DevCfg_reg_settings       dev_configreg_settings;             /**< device configuration register settings to update the DEV_CF1 and DEV_CFG2 registers
                                                                               bitfields*/
        TPS_AllVoltageRailLimits      AMUXVoltageRailLimits;              /**< structure representing the expected voltage limits,typical voltages  different voltage rails\n
                                                                                which are available as Analog signals on the DIAG_OUT pin of the TPS*/
 }TPS_Initilization_struct;

#endif /* TPS_TYPES_H_ */
