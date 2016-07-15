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
 *         @file       Tps_Priv.h
 *         @version    2.2.0
 *         @component  TPS
 *         @module     TPS Library
 *         @generator  No Configuration
 *
 *         @brief      private macros used by the TPS driver.
 *         @details    The file defines private macros used within the TPS\n
 *                     Driver.These macros are not available to application.
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
#ifndef TPS_PRIV_H_
#define TPS_PRIV_H_

/***************************************************************
* INCLUDE FILES
****************************************************************/
#include "Tps_Config.h"
/*********************************************************
Defines / data types / structs / unions /enums
**********************************************************/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT(n)                   ((1u <<(n)))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BITU8(n)                 ((uint8)(((uint8)1u) <<(n)))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BITU32(n)                ((uint32)((((uint32)1u) <<(n))))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_GET(y,n)             (((y)>>(n)) & 0x1u)

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_SET(y, mask)         ((y) |=  (mask))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_CLEAR(y, mask)       ((y) &= ~(mask))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_FLIP(y, mask)        ((y) ^=  (mask))


/* Create a bitmask of length \a len.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BIT_MASK(len)            (BIT(len)-1u)

/* Create a bitmask of length \a len.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BITU8_MASK(len)          ((uint8)((BITU8(len))-((uint8)1u)))


/* Create a bitmask of length \a len.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BITU32_MASK(len)         ((uint32)(BITU32(len)-((uint32)1u)))

/* Create a bitfield mask of length \a starting at bit \a start.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_MASK(start, len)      (BIT_MASK(len)<<(start))

/* Create a bitfield mask of length \a starting at bit \a start.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BFU8_MASK(start, len)    ((uint8)(BITU8_MASK(len)<<(start)))

/* Create a bitfield mask of length \a starting at bit \a start.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BFU32_MASK(start, len)   ((uint32)(BITU32_MASK(len)<<(start)))

/* Prepare a bitmask for insertion or combining.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_PREP(x, start, len)   (((x)&BIT_MASK(len)) << (start))

/* Prepare a bitmask for insertion or combining.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BFU8_PREP(x, start, len) ((uint8)((uint8)((x)&BITU8_MASK(len)) << (start)))


/* Prepare a bitmask for insertion or combining.*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BFU32_PREP(x, start, len)  ((uint32)((uint32)((x)&BITU32_MASK(len)) << (start)))

/* Extract a bitfield of length \a len starting at bit \a start from \a y.
 * y is the register
 * start is the starting bit of the field
 * len is the length of the field*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_GET(y, start, len)   (((((y)>>(start))) & BIT_MASK(len)))

/* Extract a bitfield of length \a len starting at bit \a start from \a y.
 * y is the register
 * start is the starting bit of the field
 * len is the length of the field*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BFU8_GET(y, start, len)   ((uint8)((((uint8)((y)>>(start))) & BITU8_MASK(len))))

/* Extract a bitfield of length \a len starting at bit \a start from \a y.
 * y is the register
 * start is the starting bit of the field
 * len is the length of the field*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BFU32_GET(y, start, len)   ((uint32)((((uint32)((y)>>(start))) & BIT_MASK(len))))

/*Insert a new bitfield value \a x into \a y.
 * y is the register
 * x is the value of the field
 * start is the starting bit of the field
 * len is the length of the field*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BF_SET(y, x, start, len)    \
    ( (y) = (uint8)(((y) & (uint8)(~(BF_MASK((start),(len))))) | (uint8)BF_PREP((x),(start),(len))))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BFU8_SET(y, x, start, len)    \
    ( (y) = (uint8)(((y) & ((uint8)(((uint8)~(BFU8_MASK((start),(len))))))) | ((uint8)BFU8_PREP((x),(start),(len)))))

/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define BFU32_SET(y, x, start, len)    \
    ( (y) = (uint32)(((y) & ((uint32)(((uint32)~(BFU32_MASK((start),(len))))))) | ((uint32)BFU32_PREP((x),(start),(len)))))

/* change the order of the bytes so that it is easy to calculate the CRC, 0xABCDEFGH ->0xGHEFDCBA*/
/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define CHANGE_BYTE_ORDER(x)    ((((x) & 0xFFU) << 24U) + (((x) & 0xFF00U) << 8U)\
                                + (((x) & 0xFF0000U) >> 8U)+ (((x) >> 24U) & 0xFFU))


/*Bit fields CRC High byte*/
#define BF_DST_SAFETY_FUNC_CFG_START       ((uint8)25U)
#define BF_SRC_SAFETY_FUNC_CFG_START       ((uint8)0U)
#define BF_SAFETY_FUNC_CFG_LENGTH          ((uint8)7u)

#define BF_DST_DEV_REV_START               ((uint8)17U)
#define BF_SRC_DEV_REV_START               ((uint8)0U)
#define BF_DEV_REV_LENGTH                  ((uint8)8u)

#define BF_DST_SAFETY_PWD_THR_CFG_START    ((uint8)13U)
#define BF_SRC_SAFETY_PWD_THR_CFG_START    ((uint8)0U)
#define BF_SAFETY_PWD_THR_CFG_LENGTH       ((uint8)4u)

#define BF_DST_SAFETY_ERR_CFG_START        ((uint8)5U)
#define BF_SRC_SAFETY_ERR_CFG_START        ((uint8)0U)
#define BF_SAFETY_ERR_CFG_LENGTH           ((uint8)8u)

#define BF_DST_EDT_TOK_FBACK_7_3_START     ((uint8)0U)
#define BF_SRC_EDT_TOK_FBACK_7_3_START     ((uint8)3U)
#define BF_EDT_TOKEN_FDBACK_7_3_LENGTH     ((uint8)5u)
/*Bit fields CRC low byte*/
#define BF_DST_EDT_TOK_FBACK_2_0_START     ((uint8)29U)
#define BF_SRC_EDT_TOK_FBACK_2_0_START     ((uint8)0U)
#define BF_EDT_TOKEN_FDBACK_2_0_LENGTH     ((uint8)3u)

#define BF_DST_WDT_WIN2_CFG_START          ((uint8)24U)
#define BF_SRC_WDT_WIN2_CFG_START          ((uint8)0U)
#define BF_WDT_WIN2_CFG_LENGTH             ((uint8)5u)

#define BF_DST_WDT_WIN1_CFG_START          ((uint8)17U)
#define BF_SRC_WDT_WIN1_CFG_START          ((uint8)0U)
#define BF_WDT_WIN1_CFG_LENGTH             ((uint8)7u)

#define BF_DST_SAFETY_ERR_PWM_L_START      ((uint8)9U)
#define BF_SRC_SAFETY_ERR_PWM_L_START      ((uint8)0U)
#define BF_SAFETY_ERR_PWM_L_LENGTH         ((uint8)8u)

#define BF_DST_DEV_CFG2_START              ((uint8)1U)
#define BF_SRC_DEV_CFG2_START              ((uint8)0U)
#define BF_DEV_CFG2_LENGTH                 ((uint8)8U)

#define BF_DST_DEV_CFG1_START              ((uint8)0u)
#define BF_SRC_DEV_CFG1_START              ((uint8)6u)
#define BF_DEV_CFG1_LENGTH                 ((uint8)1u)

/*Bit field safety check control register*/
#define BF_CFG_CRC_ENABLE_START            ((uint8)7U)
#define BF_CFG_CRC_ENABLE_LENGTH           ((uint8)1U)

#define BF_ENABLE_DRV_START                ((uint8)5U)
#define BF_ENABLE_DRV_LENGTH               ((uint8)1U)

#define BF_NO_ERROR_START                  ((uint8)2U)
#define BF_NO_ERROR_LENGTH                 ((uint8)1U)

#define BF_DIAG_EXIT_MASK_START            ((uint8)1U)
#define BF_DIAG_EXIT_MASK_LENGTH           ((uint8)1U)

#define BF_DIAG_EXIT_START                 ((uint8)0U)
#define BF_DIAG_EXIT_LENGTH                ((uint8)1U)


/*Bit field safety function config register*/
#define BF_NO_SAFE_TO_START                ((uint8)7U)
#define BF_NO_SAFE_TO_LENGTH               ((uint8)1U)

#define BF_ERROR_CFG_START                 ((uint8)6U)
#define BF_ERROR_CFG_LENGTH                ((uint8)1U)

#define BF_WD_CFG_START                    ((uint8)5U)
#define BF_WD_CFG_LENGTH                   ((uint8)1U)

#define BF_WD_RST_EN_START                 ((uint8)3U)
#define BF_WD_RST_EN_LENGTH                ((uint8)1U)

#define BF_IGN_PWRL_START                  ((uint8)4U)
#define BF_IGN_PWRL_LENGTH                 ((uint8)1U)

#define BF_DIS_NRES_MON_START              ((uint8)2U)
#define BF_DIS_NRES_MON_LENGTH             ((uint8)1U)

#define BF_VDD_3_5_SEL_START               ((uint8)0U)
#define BF_VDD_3_5_SEL_LENGTH              ((uint8)1U)


/*Bit field SAFETY_ERR_CFG register*/
#define BF_SAFE_TO_START                   ((uint8)5U)
#define BF_SAFE_TO_LENGTH                  ((uint8)3U)

#define BF_SAFE_LOCK_THR_START             ((uint8)1U)
#define BF_SAFE_LOCK_THR_LENGTH            ((uint8)4U)

/*Bit field DEV_CFG1 register*/
#define BF_nMASK_VDD1_UV_OV_START             ((uint8)6U)
#define BF_nMASK_VDD1_UV_OV_LENGTH            ((uint8)1U)


/*Bit field DEV_CFG2 register*/
#define BF_EN_VDD3_5_OT_START              ((uint8)7U)
#define BF_EN_VDD3_5_OT_LENGTH             ((uint8)1U)

#define BF_EN_VDD5_OT_START                ((uint8)6U)
#define BF_EN_VDD5_OT_LENGTH               ((uint8)1U)

#define BF_MASK_VBAT_OV_START              ((uint8)5U)
#define BF_MASK_VBAT_OV_LENGTH             ((uint8)1U)

#define BF_POST_RUN_RST_START              ((uint8)4U)
#define BF_POST_RUN_RST_LENGTH             ((uint8)1U)


/*Bit field SAFETY_ERR_CFG register*/
#define BF_CFG_LOCK_START                  ((uint8)0U)
#define BF_CFG_LOCK_LENGTH                 ((uint8)1U)


/*Bit field SAFETY_ERR_STAT register*/
#define BF_DEV_ERR_CNT_START               ((uint8)0U)
#define BF_DEV_ERR_CNT_LENGTH              ((uint8)4U)

#define BF_WD_FAIL_START                   ((uint8)4U)
#define BF_WD_FAIL_LENGTH                  ((uint8)1U)

#define BF_ERROR_PIN_FAIL_START            ((uint8)5U)
#define BF_ERROR_PIN_FAIL_LENGTH           ((uint8)1U)

/*Bit field WDT_TOKEN_VALUE register*/
#define BF_WDT_TOKEN_VALUE_START           ((uint8)0U)
#define BF_WDT_TOKEN_VALUE_LENGTH          ((uint8)4U)


/*Bit field WDT_STATUS register*/
#define BF_WDT_ANSW_CNT_START              ((uint8)6U)
#define BF_WDT_ANSW_CNT_LENGTH             ((uint8)2U)

#define BF_WDT_TOKEN_ERR_START             ((uint8)5U)
#define BF_WDT_TOKEN_ERR_LENGTH            ((uint8)1U)

#define BF_WDT_WRONG_CFG_START             ((uint8)4U)
#define BF_WDT_WRONG_CFG_LENGTH            ((uint8)1U)

#define BF_WDT_SEQ_ERR_START               ((uint8)2U)
#define BF_WDT_SEQ_ERR_LENGTH              ((uint8)1U)

#define BF_WDT_TIME_OUT_START              ((uint8)1U)
#define BF_WDT_TIME_OUT_LENGTH             ((uint8)1U)

#define BF_WDT_TOKEN_EARLY_START           ((uint8)0U)
#define BF_WDT_TOKEN_EARLY_LENGTH          ((uint8)1U)


/*Bit field SAFETY_STAT_2 register*/
#define BF_WDT_FAIL_CNT_START              ((uint8)0U)
#define BF_WDT_FAIL_CNT_LENGTH             ((uint8)3U)

#define BF_CFG_CRC_ERR_START               ((uint8)5U)
#define BF_CFG_CRC_ERR_LENGTH              ((uint8)1U)

#define BF_EE_CRC_ERR_START                ((uint8)4U)
#define BF_EE_CRC_ERR_LENGTH               ((uint8)1U)


/*Bit field SAFETY_STAT_3 register*/
#define BF_ABIST_RUN_START                 ((uint8)0U)
#define BF_ABIST_RUN_LENGTH                ((uint8)1U)

#define BF_LBIST_RUN_START                 ((uint8)1U)
#define BF_LBIST_RUN_LENGTH                ((uint8)1U)

#define BF_ABIST_UV_OV_ERR_START           ((uint8)2U)
#define BF_ABIST_UV_OV_ERR_LENGTH          ((uint8)1U)

#define BF_LBIST_ERR_START                 ((uint8)4U)
#define BF_LBIST_ERR_LENGTH                ((uint8)1U)

#define BF_NRES_ERR_START                  ((uint8)5U)
#define BF_NRES_ERR_LENGTH                 ((uint8)1U)


/*Bit field SAFETY_STAT_4 register*/
#define BF_LOCLK_START                     ((uint8)5U)
#define BF_LOCLK_LENGTH                    ((uint8)1U)

#define BF_SPI_ERR_START                   ((uint8)6U)
#define BF_SPI_ERR_LENGTH                  ((uint8)2U)

#define BF_VMON_TRIM_ERR_START             ((uint8)0U)
#define BF_VMON_TRIM_ERR_LENGTH            ((uint8)1U)

#define BF_ENDRV_ERR_START                 ((uint8)1U)
#define BF_ENDRV_ERR_LENGTH                ((uint8)1U)

/*values of the bit field SPI_ERR*/
#define NO_ERROR                           0x0u
#define COMMAND_ERROR                      0x1u
#define FORMAT_ERROR                       0x2u
#define DATA_OUTPUT_MISMATCH               0x3u


/*Bit field SAFETY_BIST_CTRL register*/
#define BF_AUTO_BIST_DIS_START             ((uint8)5U)
#define BF_AUTO_BIST_DIS_LENGTH            ((uint8)1U)

#define BF_EE_CRC_CHK_START                ((uint8)4U)
#define BF_EE_CRC_CHK_LENGTH               ((uint8)1U)

#define BF_LBIST_EN_START                  ((uint8)2U)
#define BF_LBIST_EN_LENGTH                 ((uint8)1U)

#define BF_ABIST_EN_START                  ((uint8)1U)
#define BF_ABIST_EN_LENGTH                 ((uint8)1U)


/*Bit field SAFETY_PWD_THR_CFG register*/
#define BF_PWD_THR_START                   ((uint8)0U)
#define BF_PWD_THR_LENGTH                  ((uint8)4U)

/*Bit field WDT_TOKEN_FDBCK register*/
#define BF_TOKEN_SEED_START                ((uint8)0U)
#define BF_TOKEN_SEED_LENGTH               ((uint8)4U)

#define BF_TOKEN_FDBK_START                ((uint8)4U)
#define BF_TOKEN_FDBK_LENGTH               ((uint8)4U)

/*Bit field WDT_WIN1_CFG register*/
#define BF_WD_OPEN_WINDOW_SCALER_START     ((uint8)0U)
#define BF_WD_OPEN_WINDOW_SCALER_LENGTH    ((uint8)7U)
#define BF_WD_CLOSE_WINDOW_SCALER_START    ((uint8)0U)
#define BF_WD_CLOSE_WINDOW_SCALER_LEN      ((uint8)5U)

/*Bit field DIAG_CFG_CTRL register*/
#define BF_MUX_EN_START                    ((uint8)7U)
#define BF_MUX_EN_LENGTH                   ((uint8)1U)

#define BF_SPI_SDO_START                   ((uint8)6U)
#define BF_SPI_SDO_LENGTH                  ((uint8)1U)

#define BF_MUX_OUT_START                   ((uint8)5U)
#define BF_MUX_OUT_LENGTH                  ((uint8)1U)

#define BF_INT_CON_START                   ((uint8)2U)
#define BF_INT_CON_LENGTH                  ((uint8)3U)

#define BF_MUX_CFG_START                   ((uint8)0U)
#define BF_MUX_CFG_LENGTH                  ((uint8)2U)

/*Bit field SENS_CTRL register*/
#define BF_VDD5_EN_START                   ((uint8)4U)
#define BF_VDD5_EN_LENGTH                  ((uint8)1U)

#define BF_VSOUT1_EN_START                 ((uint8)0U)
#define BF_VSOUT1_EN_LENGTH                ((uint8)1U)


/*Bit field DEV_REV register*/
#define BF_MINOR_VERSION_START             ((uint8)0U)
#define BF_MINOR_VERSION_LENGTH            ((uint8)4U)

#define BF_MAJOR_VERSION_START             ((uint8)4U)
#define BF_MAJOR_VERSION_LENGTH            ((uint8)4U)

/*Bit field VMON_STAT_2 Register*/
#define BF_VDD6_OV_START                   ((uint8)7U)
#define BF_VDD6_OV_LENGTH                  ((uint8)1U)

#define BF_VDD6_UV_START                   ((uint8)6U)
#define BF_VDD6_UV_LENGTH                  ((uint8)1U)

#define BF_VDD5_OV_START                   ((uint8)5U)
#define BF_VDD5_OV_LENGTH                  ((uint8)1U)

#define BF_VDD5_UV_START                   ((uint8)4U)
#define BF_VDD5_UV_LENGTH                  ((uint8)1U)

#define BF_VDD3_5_OV_START                 ((uint8)3U)
#define BF_VDD3_5_OV_LENGTH                ((uint8)1U)

#define BF_VDD3_5_UV_START                 ((uint8)2U)
#define BF_VDD3_5_UV_LENGTH                ((uint8)1U)

#define BF_VDD1_OV_START                   ((uint8)1U)
#define BF_VDD1_OV_LENGTH                  ((uint8)1U)

#define BF_VDD1_UV_START                   ((uint8)0U)
#define BF_VDD1_UV_LENGTH                  ((uint8)1U)

/*Bit field VMON_STAT_1 Register*/
#define BF_VBATP_OV_START                  ((uint8)7U)
#define BF_VBATP_OV_LENGTH                 ((uint8)1U)

#define BF_VBATP_UV_START                  ((uint8)6U)
#define BF_VBATP_UV_LENGTH                 ((uint8)1U)

#define BF_VCP17_OV_START                  ((uint8)5U)
#define BF_VCP17_OV_LENGTH                 ((uint8)1U)

#define BF_VCP12_OV_START                  ((uint8)4U)
#define BF_VCP12_OV_LENGTH                 ((uint8)1U)

#define BF_VCP12_UV_START                  ((uint8)3U)
#define BF_VCP12_UV_LENGTH                 ((uint8)1U)

#define BF_AVDD_VMON_ERR_START             ((uint8)2U)
#define BF_AVDD_VMON_ERR_LENGTH            ((uint8)1U)

#define BF_BG2_ERR_START                   ((uint8)1U)
#define BF_BG2_ERR_LENGTH                  ((uint8)1U)

#define BF_BG1_ERR_START                   ((uint8)0U)
#define BF_BG1_ERR_LENGTH                  ((uint8)1U)


/*Bit field SAFETY_STAT_1 Register*/
#define BF_VDD5_ILIM_START                 ((uint8)7U)
#define BF_VDD5_ILIM_LENGTH                ((uint8)1U)

#define BF_VDD3_5_ILIM_START               ((uint8)6U)
#define BF_VDD3_5_ILIM_LENGTH              ((uint8)1U)

#define BF_VSOUT1_UV_START                 ((uint8)5U)
#define BF_VSOUT1_UV_LENGTH                ((uint8)1U)

#define BF_VSOUT1_OV_START                 ((uint8)4U)
#define BF_VSOUT1_OV_LENGTH                ((uint8)1U)

#define BF_VSOUT1_ILIM_START               ((uint8)3U)
#define BF_VSOUT1_ILIM_LENGTH              ((uint8)1U)

#define BF_VSOUT1_OT_START                 ((uint8)2U)
#define BF_VSOUT1_OT_LENGTH                ((uint8)1U)

#define BF_VDD5_OT_START                   ((uint8)1U)
#define BF_VDD5_OT_LENGTH                  ((uint8)1U)

#define BF_VDD3_5_OT_START                 ((uint8)0U)
#define BF_VDD3_5_OT_LENGTH                ((uint8)1U)


/*Bit field DEV_STAT Register*/
#define BF_CANWU_L_START                   ((uint8)1U)
#define BF_CANWU_L_LENGTH                  ((uint8)1U)

#define BF_IGN_START                       ((uint8)0U)
#define BF_IGN_LENGTH                      ((uint8)1U)




/*values for the bit field MUX_CFG*/
#define MUX_OUT_CONTROLLED                 0x0u
#define DIGITAL_MUX                        0x1u
#define ANALOG_MUX                         0x2u
#define DEVICE_INTERCONNECT_MODE           0x3u

/*values for the bit field INT_CON*/
#define NO_INTERCONNECT_TEST               0x0U

/*limits for the maximum value of some register fields of TPS*/
#define OPEN_WINDOW_SCALER_MAX             0x7FU
#define CLOSE_WINDOW_SCALER_MAX            0x1FU
#define PWD_THRESHHOLD_MAX                 0x0FU
#define SAFE_STATE_TIMEOUT_MAX             0x07U
#define SAFE_LOCK_THRESHHOLD_MAX           0x0FU
#define TOKEN_SEED_MAX                     0xFU
#define TOKEN_SEED_MIN                     0x0U
#define TOKEN_FDBCK_MAX                    0xFU
#define TOKEN_FDBCK_MIN                    0x0U

/*Include and typedefs and enums used by  TPS_Interface.c*/
/** \defgroup grp_read_commands Read commands for the various registers of the TPS
 *  Following macros are the Read commands for the various registers of the TPS
 *
 */
/**@{*/
#define RD_COMMAND_NONE                    ((uint16)0x0000U)  /**< Alias for no read command                          */
#define RD_WD_WIN2_CFG                     ((uint16)0x0500U)  /**< Alias for read watchdog close window config        */
#define RD_DEV_ID                          ((uint16)0x0600U)  /**< Alias for read device identifier                   */
#define RD_DEV_REV                         ((uint16)0x0C00U)  /**< Alias for read device revision                     */
#define RD_DEV_STAT                        ((uint16)0x1100U)  /**< Alias for read device status                       */
#define RD_VMON_STAT_1                     ((uint16)0x1200U)  /**< Alias for read voltage monitor status 1            */
#define RD_SAFETY_STAT_1                   ((uint16)0x2400U)  /**< Alias for read safety status 1                     */
#define RD_WD_WIN1_CFG                     ((uint16)0x2E00U)  /**< Alias for read watchdog open window config         */
#define RD_SAFETY_ERR_CFG                  ((uint16)0x3000U)  /**< Alias for read safety error config                 */
#define RD_WDT_TOKEN_VALUE                 ((uint16)0x3600U)  /**< Alias for read watchdog token                      */
#define RD_SAFETY_PWD_THR_CFG              ((uint16)0x3900U)  /**< Alias for read safety power down threshold config  */
#define RD_SAFETY_FUNC_CFG                 ((uint16)0x3A00U)  /**< Alias for read safety function config              */
#define RD_SAFETY_BIST_CTRL                ((uint16)0x3C00U)  /**< Alias for read safety BIST control                 */
#define RD_SAFETY_CHECK_CTRL               ((uint16)0x4400U)  /**< Alias for read safety check control                */
#define RD_DEV_CFG2                        ((uint16)0x4800U)  /**< Alias for read config 2                            */
#define RD_WDT_STATUS                      ((uint16)0x4E00U)  /**< Alias for read watchdog status                     */
#define RD_SENS_CTRL                       ((uint16)0x5600U)  /**< Alias for read sense control                       */
#define RD_SAFETY_ERR_PWM_L                ((uint16)0x5900U)  /**< Alias for read safety error pwm low                */
#define RD_SAFETY_CFG_CRC                  ((uint16)0x5A00U)  /**< Alias for read safety config crc                   */
#define RD_WDT_TOKEN_FDBCK                 ((uint16)0x7800U)  /**< Alias for read watchdog token feedback             */
#define RD_SAFETY_STAT_3                   ((uint16)0xA300U)  /**< Alias for read safety status 3                     */
#define RD_SAFETY_STAT_4                   ((uint16)0xA500U)  /**< Alias for read safety status 4                     */
#define RD_VMON_STAT_2                     ((uint16)0xA600U)  /**< Alias for read voltage monitor status 2            */
#define RD_SAFETY_ERR_STAT                 ((uint16)0xAA00U)  /**< Alias for read safety error status                 */
#define RD_DIAG_MUX_SEL                    ((uint16)0xAC00U)  /**< Alias for read diagnostic mux select               */
#define RD_DEV_CFG1                        ((uint16)0xAF00U)  /**< Alias for read config 1                            */
#define RD_SAFETY_STAT_5                   ((uint16)0xC000U)  /**< Alias for read safety status 5                     */
#define RD_SAFETY_STAT_2                   ((uint16)0xC500U)  /**< Alias for read safety status 2                     */
#define RD_SAFETY_ERR_PWM_H                ((uint16)0xD700U)  /**< Alias for read safety error pwm high               */
#define RD_DIAG_CFG_CTRL                   ((uint16)0xDD00U)   /**< Alias for read diagnostic config control          */
#define RD_WRONG_PARITY                    ((uint16)0x0D00U)  /**< Valid Command with incorrect parity bit            */
#define RD_WRONG_COMMAND                   ((uint16)0x2C00U)  /**< InValid Command with correct parity bit            */

/** \defgroup grp_write_commands Write commands for the various registers of the TPS
 *  Following macros are the Write commands for the various registers of the TPS
 *
 */
/**@{*/
#define WR_COMMAND_NONE                    ((uint16)0x0000U)  /**< Alias for no write command                          */
#define WR_WD_WIN2_CFG                     ((uint16)0x0900U)  /**< Alias for write close window config                 */
#define WR_SAFETY_FUNC_CFG                 ((uint16)0x3500U)  /**< Alias for write safety function config              */
#define WR_SAFETY_CFG_CRC                  ((uint16)0x6300U)  /**< Alias for write safe config crc                     */
#define WR_WDT_TOKEN_FDBCK                 ((uint16)0x7700U)  /**< Alias for write wdg token feedback                  */
#define WR_SENS_CTRL                       ((uint16)0x7B00U)  /**< Alias for write sense control                       */
#define WR_CAN_STBY                        ((uint16)0x7D00U)  /**< Alias for write CAN standby                         */
#define WR_SAFETY_ERR_PWM_L                ((uint16)0x7E00U)  /**< Alias for write safety error pwm low                */
#define WR_SAFETY_CHECK_CTRL               ((uint16)0x9300U)  /**< Alias for write safety check control                */
#define WR_DEV_CFG2                        ((uint16)0x9500U)  /**< Alias for write device config 2                     */
#define WR_SAFETY_PWD_THR_CFG              ((uint16)0x9900U)  /**< Alias for write safety power down threshold config  */
#define WR_DIAG_MUX_SEL                    ((uint16)0xC900U)  /**< Alias for write diagnostic mux select               */
#define WR_SAFETY_BIST_CTRL                ((uint16)0x9F00U)  /**< Alias for write safety BIST control                 */
#define WR_SAFETY_ERR_STAT                 ((uint16)0xA900U)  /**< Alias for write safety error status                 */
#define WR_DEV_CFG1                        ((uint16)0xB700U)  /**< Alias for write device config 1                     */
#define WR_SW_UNLOCK                       ((uint16)0xBB00U)  /**< Alias for write SW unlock                           */
#define WR_SW_LOCK                         ((uint16)0xBD00U)  /**< Alias for write SW lock                             */
#define WR_DIAG_CFG_CTRL                   ((uint16)0xCC00U)  /**< Alias for write diagnostic config control           */
#define WR_SAFETY_ERR_PWM_H                ((uint16)0xD800U)  /**< Alias for write safety error pwm high               */
#define WR_SAFETY_ERR_CFG                  ((uint16)0xDB00U)  /**< Alias for write safety error config                 */
#define WR_WDT_ANSWER                      ((uint16)0xE100U)  /**< Alias for write wdg answer                          */
#define WR_WD_WIN1_CFG                     ((uint16)0xED00U)  /**< Alias for write open window config                  */

#define TPSIF_STATUS_FLAG_RESPOSE_MASK     0xFF00U
#define TPSIF_DATA_MASK                    0x00FFU
#define TPSIF_SPI_TRANSFER_ERRFLG_MASK     0xF7U
#define TPSIF_MULTIBITKEY_MASK             0xF7U

/*Bit Fields*/
#define BF_MULTIBITKEY_START               ((uint8)0x4U)
#define BF_MULTIBITKEY_LENGTH              ((uint8)0x4U)
#define MULTIBITKEY_SET                    ((uint8)0xaU)

/*Bit Fields*/

/*Device Status Flag Byte Response*/
#define BF_SPI_ERRFLAG_START               ((uint8)0x0U)
#define BF_SPI_ERRFLAG_LENGTH              ((uint8)0x3U)

/*Masks for various configuration registers the
 * unused bits of registers have to not considered for readback etc.*/
#define SAFETY_FUNC_CFG_MASK               0xFCU
#define SAFETY_PWD_THR_CFG_MASK            0x0FU
#define DEV_CFG1_REG_MASK                  0x40U
#define DEV_CFG2_REG_MASK                  0xF0U
#define WDT_WIN1_CFG_MASK                  0x7FU
#define WDT_WIN2_CFG_MASK                  0x1FU

/*Data for SPI access TPS_SW_LOCK/TPS_SW_UNLOCK registers */
#define SW_LOCK_DATA                       ((uint8)0xAAU)
#define SW_UNLOCK_DATA                     ((uint8)0x55U)

/** @typedef TpsIf_COMMAND_t
*   @brief Tps Interface Instruction Type Definition
*
*   This type is used to access a Tps Instructions.
*/
typedef struct TpsIf_Command
{
    uint16 Read;      /**< Read Command for the register   */
    uint16 Write;     /**< Write command for the register  */
} TpsIf_COMMAND_t;






/*SAFETYMCUSW 340 S MR:19.7 <APPROVED> "Reason -  This is an advisory by MISRA.We accept this as a coding convention*/
#define CHECK_RANGE_RAM_PTR(x)      ((((uint32)(x)) >= SRAM_START) && ((((uint32)(x)) <= ((SRAM_END-(uint32)sizeof((x)))))))

#endif /* TPS_PRIV_H_ */
