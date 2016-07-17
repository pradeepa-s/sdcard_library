/*
 * esm_application_callback.c
 *
 *  Created on: Jun 21, 2013
 *      Author: a0131910
 */
#include <sl_types.h>
#include <sl_priv.h>

/* Needed for testing the call back parameters */
uint32 callbkParam1, callbkParam2, callbkParam3, callbkParam4, callbkParam5, callbkParam6;

/*Recieve the esm callbacks and make sure the callbacks are happening properly for the fault injections*/
#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
boolean pscon_errorforcing_app_callback = FALSE;
boolean cccmr4f_errorforcing_app_callback = FALSE;
boolean pscon_selftest_errorforcing_app_callback = FALSE;
boolean sram_radecode_diagnostics_app_callback = FALSE;
boolean fee_1bit_faultinject_app_callback = FALSE;
boolean fee_2bit_faultinject_app_callback = FALSE;
boolean sram_1bit_prof_faultinject_callback = FALSE;
boolean sram_2bit_faultinject_callback = FALSE;
boolean flash_2bit_faultinject_callback = FALSE;
boolean flash_addr_par_faultinject_callback = FALSE;
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
boolean dma_1bit_faultinject_callback = FALSE;
boolean dma_2bit_faultinject_callback = FALSE;
boolean can_1bit_faultinject_callback = FALSE;
boolean can_2bit_faultinject_callback = FALSE;
boolean mibspi_1bit_faultinject_callback = FALSE;
boolean mibspi_2bit_faultinject_callback = FALSE;
boolean cccmr5f_errorforcing_app_callback = FALSE;
#endif
boolean unknowncallback = FALSE;
void ESM_ApplicationCallback(uint32 grp_channel, uint32 param1, uint32 param2, uint32 param3)
{
volatile uint32 flashread = 0u;
/********************************************************************************************************/
	/*all GROUP 1 errors are captured here*/
	/********************************************************************************************************/
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
    uint32 channel;
    channel = grp_channel & 0xffffu;

    if ((grp_channel & ESM_GRP1_MASK) == ESM_GRP1_MASK)
    {
        /* process group 1 callbacks */
        if (ESM_G1ERR_DMA_ECC_SBERR == channel)
        {
            /* Clear flags and read error address */
            BIT_SET(sl_dmaREG->DMASECCCTRL, BIT(DMA_ECC_SBERR));

            /* Clear ESM error status */
            sl_esmREG->SR7[0] =  GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_SBERR);
            dma_1bit_faultinject_callback = TRUE;
        }
        else if (ESM_G1ERR_DMA_ECC_UNCORR == channel)
        {
            /* Clear flags and read error address */
            BIT_SET(sl_dmaREG->DMAPAR, BIT(DMA_ECC_ERR));

            /* Clear ESM error status */
            sl_esmREG->SR1[0] = GET_ESM_BIT_NUM(ESM_G1ERR_DMA_ECC_UNCORR);
            dma_2bit_faultinject_callback = TRUE;
        }
        else if ((ESM_G1ERR_CAN1_ECC_SBERR == channel) ||
                    (ESM_G1ERR_CAN2_ECC_SBERR == channel) ||
                    (ESM_G1ERR_CAN3_ECC_SBERR == channel) ||
                    (ESM_G1ERR_CAN4_ECC_SBERR == channel))
        {
            /* Clear ESM error status */
            sl_esmREG->SR7[0] = GET_ESM_BIT_NUM(channel);
            can_1bit_faultinject_callback = TRUE;
        }
        else if ((ESM_G1ERR_CAN1_ECC_UNCORR == channel) ||
                    (ESM_G1ERR_CAN2_ECC_UNCORR == channel) ||
                    (ESM_G1ERR_CAN3_ECC_UNCORR == channel)) 
        {
            /* Clear ESM error status */
            sl_esmREG->SR1[0] = GET_ESM_BIT_NUM(channel);
            can_2bit_faultinject_callback = TRUE;
        }
        else if ((ESM_G1ERR_CAN4_ECC_UNCORR == channel))
        {
            sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(channel);
            can_2bit_faultinject_callback = TRUE;
        }
        else if ((ESM_G1ERR_SPI1_ECC_SBERR == channel) ||
                    (ESM_G1ERR_SPI2_ECC_SBERR == channel) ||
                    (ESM_G1ERR_SPI3_ECC_SBERR == channel) ||
                    (ESM_G1ERR_SPI4_ECC_SBERR == channel) ||
                    (ESM_G1ERR_SPI5_ECC_SBERR == channel))
        {
            /* Clear ESM error status */
            sl_esmREG->SR7[0] = GET_ESM_BIT_NUM(channel);
            mibspi_1bit_faultinject_callback = TRUE;
        }
        else if ((ESM_G1ERR_SPI1_ECC_UNCORR == channel) ||
                    (ESM_G1ERR_SPI3_ECC_UNCORR == channel) ||
                    (ESM_G1ERR_SPI5_ECC_UNCORR == channel)) 
        {
            /* Clear ESM error status */
            sl_esmREG->SR1[0] = GET_ESM_BIT_NUM(channel);
            mibspi_2bit_faultinject_callback = TRUE;
        }
        else if ((ESM_G1ERR_SPI2_ECC_UNCORR == channel) ||
                (ESM_G1ERR_SPI4_ECC_UNCORR == channel))
        {
            sl_esmREG->SR4[0] = GET_ESM_BIT_NUM(channel);
            mibspi_2bit_faultinject_callback = TRUE;
        }
        else if (ESM_G1ERR_CCMR5_SELFTEST == channel)
        {
            sl_esmREG->SR1[0] = GET_ESM_BIT_NUM(channel);
            /* no need to set the flag cccmr5f_errorforcing_app_callback, as the
             * flag is already set by the G2 error */
        }

    }

    if ((grp_channel & ESM_GRP2_MASK) == ESM_GRP2_MASK)
    {
    	/* process group 2 call-backs */
    	if ((ESM_G2ERR_CCMR5_CPUCOMP_ERR == channel) ||
    		(ESM_G2ERR_CCMR5_VIMCOMP_ERR == channel) ||
    		(ESM_G2ERR_CCMR5_INMCOMP_ERR == channel) ||
    		(ESM_G2ERR_CCMR5_PDCOMP_ERR == channel))
    	{
    	   	sl_esmREG->SSR2 = GET_ESM_BIT_NUM(channel);
    	   	/* Clear nError */
    	   	_SL_HoldNClear_nError();
    	   	cccmr5f_errorforcing_app_callback = TRUE;
    	}
    }
#endif

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
	if((ESM_GRP1_MASK == ((grp_channel&ESM_GRP1_MASK)))&&(ESM_G1ERR_PSCON_COMPARE_ERR == (grp_channel & 0x0000ffff)))
	{
		/*clear nerror and esm flags*/
		sl_esmREG->SR4[0] |= GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_COMPARE_ERR);
		sl_esmREG->SR4[0] |= GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR);
		_SL_HoldNClear_nError();
		pscon_errorforcing_app_callback = TRUE;
	}
	else if((ESM_GRP1_MASK == ((grp_channel&ESM_GRP1_MASK)))&&(ESM_G1ERR_PSCON_SELTEST_ERR == (grp_channel & 0x0000ffff)))
	{
		/*clear nerror and esm flags*/
		sl_esmREG->SR4[0] |= GET_ESM_BIT_NUM(ESM_G1ERR_PSCON_SELTEST_ERR);
		_SL_HoldNClear_nError();
		pscon_selftest_errorforcing_app_callback = TRUE;
	}
	else if((ESM_GRP1_MASK == ((grp_channel&ESM_GRP1_MASK)))&&(ESM_G1ERR_CCMR4_SELFTEST == (grp_channel & 0x0000ffff)))
	{
		/* clear nError PIN and esm flags */
	    sl_esmREG->SR1[1] |= GET_ESM_BIT_NUM(ESM_G2ERR_CCMR4_COMP);
	    sl_esmREG->SSR2 |= GET_ESM_BIT_NUM(ESM_G2ERR_CCMR4_COMP);
	    sl_esmREG->SR1[0] |= GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST);
		_SL_HoldNClear_nError();
		cccmr4f_errorforcing_app_callback = TRUE;
	}
	else if((ESM_GRP1_MASK == ((grp_channel&ESM_GRP1_MASK)))&&(ESM_G1ERR_EEPROM_CORRERR == (grp_channel & 0x0000ffff)))
	{
		callbkParam1 = param1;
		callbkParam2 = param2;
		callbkParam3 = param3;
		/* clear nError PIN and esm flags */
	    sl_esmREG->SR4[0] |= GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_CORRERR);
		_SL_HoldNClear_nError();
		fee_1bit_faultinject_app_callback = TRUE;
	}
	else if((ESM_GRP1_MASK == ((grp_channel&ESM_GRP1_MASK)))&&(ESM_G1ERR_EEPROM_UNCORRERR == (grp_channel & 0x0000ffff)))
	{
		callbkParam1 = param1;
		callbkParam2 = param2;
		/* clear nError PIN and esm flags */
		sl_esmREG->SR4[0]  |= GET_ESM_BIT_NUM(ESM_G1ERR_EEPROM_UNCORRERR);
		_SL_HoldNClear_nError();
		fee_2bit_faultinject_app_callback = TRUE;
	}
	else if((ESM_GRP1_MASK == ((grp_channel&ESM_GRP1_MASK)))&&(ESM_G1ERR_B0TCM_CORRERR == (grp_channel & 0x0000ffff)))
	{
		callbkParam1 = param1;
		/* clear nError PIN and esm flags */
		sl_esmREG->SR1[0] |= GET_ESM_BIT_NUM(ESM_G1ERR_B0TCM_CORRERR);
		BIT_SET(sl_tcram1REG->RAMERRSTATUS,TCRAM_RAMERRSTATUS_ADDR_SERR);/* Clear for subsequent operation */
		BIT_SET(sl_tcram2REG->RAMERRSTATUS,TCRAM_RAMERRSTATUS_ADDR_SERR);
		_SL_HoldNClear_nError();
		sram_1bit_prof_faultinject_callback = TRUE;
	}
	else if((ESM_GRP1_MASK == ((grp_channel&ESM_GRP1_MASK)))&&(ESM_G1ERR_B1TCM_CORRERR == (grp_channel & 0x0000ffff)))
	{
		callbkParam4 = param1;
		sl_esmREG->SR1[0] |= GET_ESM_BIT_NUM(ESM_G1ERR_B1TCM_CORRERR);
		BIT_SET(sl_tcram1REG->RAMERRSTATUS,TCRAM_RAMERRSTATUS_ADDR_SERR);/* Clear for subsequent operation */
		BIT_SET(sl_tcram2REG->RAMERRSTATUS,TCRAM_RAMERRSTATUS_ADDR_SERR);
		_SL_HoldNClear_nError();
		sram_1bit_prof_faultinject_callback = TRUE;
	}
	/********************************************************************************************************/
	/*all GROUP 2 errors are captured here*/
	/********************************************************************************************************/
	else if((ESM_GRP2_MASK == ((grp_channel&ESM_GRP2_MASK)))&&(ESM_G2ERR_CCMR4_COMP == (grp_channel & 0x0000ffff)))
	{
		/* clear nError PIN and esm flags */
	    sl_esmREG->SR1[1] |= GET_ESM_BIT_NUM(ESM_G2ERR_CCMR4_COMP);
	    sl_esmREG->SSR2 |= GET_ESM_BIT_NUM(ESM_G2ERR_CCMR4_COMP);
	    sl_esmREG->SR1[0] |= GET_ESM_BIT_NUM(ESM_G1ERR_CCMR4_SELFTEST);
		_SL_HoldNClear_nError();
		cccmr4f_errorforcing_app_callback = TRUE;
	}
	else if((ESM_GRP2_MASK == ((grp_channel&ESM_GRP2_MASK)))&&(ESM_G2ERR_B0TCM_UNCORR == (grp_channel & 0x0000ffff)))
	{
		callbkParam1 = param1;
		/* clear nError PIN and esm flags */
		sl_esmREG->SR1[1] |= GET_ESM_BIT_NUM(ESM_G2ERR_B0TCM_UNCORR);
		sl_esmREG->SSR2 |= GET_ESM_BIT_NUM(ESM_G2ERR_B0TCM_UNCORR);
		_SL_HoldNClear_nError();
		sram_radecode_diagnostics_app_callback = TRUE;
	}
	else if((ESM_GRP2_MASK == ((grp_channel&ESM_GRP2_MASK)))&&(ESM_G2ERR_B1TCM_UNCORR == (grp_channel & 0x0000ffff)))
	{
		callbkParam1 = param1;
		sl_esmREG->SR1[1] |= GET_ESM_BIT_NUM(ESM_G2ERR_B1TCM_UNCORR);
		sl_esmREG->SSR2 |= GET_ESM_BIT_NUM(ESM_G2ERR_B1TCM_UNCORR);
		sram_radecode_diagnostics_app_callback = TRUE;
	}
	else if((ESM_GRP2_MASK == ((grp_channel&ESM_GRP2_MASK)))&&(ESM_G2ERR_FMC_UNCORR == (grp_channel & 0x0000ffff)))
	{
		callbkParam1 = param1;
		/* Anyways clear flash & ESM status registers */
		sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_ADD_PAR_ERR;
		sl_esmREG->SR1[1] =  GET_ESM_BIT_NUM(ESM_G2ERR_FMC_UNCORR);
	    sl_esmREG->SSR2 |= GET_ESM_BIT_NUM(ESM_G2ERR_FMC_UNCORR);
		flashread = sl_flashWREG->FUNCERRADD;
		flash_addr_par_faultinject_callback = TRUE;
	}
	/********************************************************************************************************/
	/*all GROUP 3 errors are captured here*/
	/********************************************************************************************************/
	else if((ESM_GRP3_MASK == ((grp_channel&ESM_GRP3_MASK)))&&(ESM_G3ERR_B0TCM_ECC_UNCORR == (grp_channel & 0x0000ffff)))
	{
		/* Clear nError */
		_SL_HoldNClear_nError();
		/* Clear the ESM Status */
		sl_esmREG->SR1[2] |= GET_ESM_BIT_NUM(ESM_G3ERR_B0TCM_ECC_UNCORR);
		/* Clear double bit error anyways */
		sl_tcram1REG->RAMERRSTATUS |= TCRAM_RAMERRSTATUS_DER;
		sram_2bit_faultinject_callback = TRUE;
	}
	else if((ESM_GRP3_MASK == ((grp_channel&ESM_GRP3_MASK)))&&(ESM_G3ERR_B1TCM_ECC_UNCORR == (grp_channel & 0x0000ffff)))
	{
		/* Clear nError */
		_SL_HoldNClear_nError();
		/* Clear the ESM Status */
		sl_esmREG->SR1[2] |= GET_ESM_BIT_NUM(ESM_G3ERR_B1TCM_ECC_UNCORR);
		/* Clear double bit error anyways */
		sl_tcram2REG->RAMERRSTATUS |= TCRAM_RAMERRSTATUS_DER;
		sram_2bit_faultinject_callback = TRUE;
	}
	else if((ESM_GRP3_MASK == ((grp_channel&ESM_GRP3_MASK)))&&(ESM_G3ERR_FMC_UNCORR == (grp_channel & 0x0000ffff)))
	{
		/* Clear nError */
		_SL_HoldNClear_nError(); /* Clear nError */
		/* Clear flash & ESM status registers */
		sl_flashWREG->FEDACSTATUS = F021F_FEDACSTATUS_B1_UNC_ERR;
		/*sl_flashWREG->FUNCERRADD = 0x100u;*/
		sl_esmREG->SR1[2] |= GET_ESM_BIT_NUM(ESM_G3ERR_FMC_UNCORR);
		flashread = sl_flashWREG->FUNCERRADD;
		flash_2bit_faultinject_callback = TRUE;
	}
	else
	{
		unknowncallback = TRUE;
	}
#endif
}
