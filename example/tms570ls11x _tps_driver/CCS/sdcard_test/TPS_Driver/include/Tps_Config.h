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
 *         @file       Tps_Config.h
 *         @version    2.2.0
 *         @component  TPS
 *         @module     TPS Library
 *         @generator  No Configuration
 *
 *         @brief      This files contains the configuration items for the the TPS Driver.
 *         @details    The file contains configuration items such as SRAM RANGE degines etc.
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


#ifndef __TPS_CONFIG_H__
#define __TPS_CONFIG_H__
/**
 *  @defgroup config_group configuration items for TPS Driver library
 *  configuration items for TPS Driver library
 */
/** \defgroup grp_sram_range SRAM Range defines
 *  @ingroup config_group
 *  Following macros are used to define the range of TCM RAM
 *
 */
/**@{*/
#define SRAM_START          ((uint32)0x08000000U)     /**< Start address of TCM SRAM */
#define SRAM_END            ((uint32)0x0803FFFFU)     /**< End address of TCM SRAM */
/**@}*/

/** \defgroup tps_debug macros for enabling the debug messages
 *  @ingroup config_group
 *  Following macros are used to enable/disable debug information from TPS
 *
 */
/**@{*/
#define DEBUG_ENABLE        0U     /**< enable/disable the debug messages from TPS */
#define ERROR_ENABLE        0U     /**< enable/disable the error messages from TPS*/
/**@}*/
#endif /* __TPS_CONFIG_H__ */
