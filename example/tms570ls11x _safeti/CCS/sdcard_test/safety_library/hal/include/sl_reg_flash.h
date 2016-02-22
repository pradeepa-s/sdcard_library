/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                                                                          |**
**|         Copyright (c) 2012 Texas Instruments Incorporated                |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/
/**
 * @file        sl_reg_flash.h
 * @version     $$file_version$$
 * <\todo Brief description goes here>
 *
 * <\todo Detailed description goes here>
 */

#ifndef __SL_REG_FLASH_H__
#define __SL_REG_FLASH_H__

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
typedef volatile struct sl_flashWBase
{
    uint32 FRDCNTL;     /* 0x0000 */
    uint32   rsvd1;       /* 0x0004 */
    uint32 FEDACCTRL1;  /* 0x0008 */
    uint32 FEDACCTRL2;  /* 0x000C */
    uint32 FCORERRCNT;  /* 0x0010 */
    uint32 FCORERRADD;  /* 0x0014 */
    uint32 FCORERRPOS;  /* 0x0018 */
    uint32 FEDACSTATUS; /* 0x001C */
    uint32 FUNCERRADD;  /* 0x0020 */
    uint32 FEDACSDIS;   /* 0x0024 */
    uint32 FPRIMADDTAG; /* 0x0028 */
    uint32 FREDUADDTAG; /* 0x002C */
    uint32 FBPROT;      /* 0x0030 */
    uint32 FBSE;        /* 0x0034 */
    uint32 FBBUSY;      /* 0x0038 */
    uint32 FBAC;        /* 0x003C */
    uint32 FBFALLBACK;  /* 0x0040 */
    uint32 FBPRDY;      /* 0x0044 */
    uint32 FPAC1;       /* 0x0048 */
    uint32 FPAC2;       /* 0x004C */
    uint32 FMAC;        /* 0x0050 */
    uint32 FMSTAT;      /* 0x0054 */
    uint32 FEMUDMSW;    /* 0x0058 */
    uint32 FEMUDLSW;    /* 0x005C */
    uint32 FEMUECC;     /* 0x0060 */
    uint32 FLOCK;       /* 0x0064 */
    uint32 FEMUADDR;    /* 0x0068 */
    uint32 FDIAGCTRL;   /* 0x006C */
    uint32 FRAWDATAH;   /* 0x0070 */
    uint32 FRAWDATAL;   /* 0x0074 */
    uint32 FRAWECC;     /* 0x0078 */
    uint32 FPAROVR;     /* 0x007C */
    uint32   rsvd2[16U];  /* 0x009C */
    uint32 FEDACSDIS2;  /* 0x00C0 */
    uint32   rsvd3[15U]; /* 0x00C4 */
    uint32   rsvd4[13U];   /* 0x0100 */
    uint32   rsvd5[85U]; /* 0x0134 */
    uint32 FSMWRENA;    /* 0x0288 */
    uint32   rsvd6[6U];  /* 0x028C */
    uint32 FSMSECTOR;   /* 0x02A4 */
    uint32   rsvd7[4U];  /* 0x02A8 */
    uint32 EEPROMCONFIG;/* 0x02B8 */
    uint32   rsvd8[19U]; /* 0x02BC */
    uint32 EECTRL1;     /* 0x0308 */
    uint32 EECTRL2;     /* 0x030C */
    uint32 EECORRERRCNT;/* 0x0310 */
    uint32 EECORRERRADD;/* 0x0314 */
    uint32 EECORRERRPOS;/* 0x0318 */
    uint32 EESTATUS;    /* 0x031C */
    uint32 EEUNCERRADD; /* 0x0320 */
} sl_flashWBASE_t;
#endif

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
typedef volatile struct sl_flashWBase
{
    uint32 FRDCNTL;         /* 0x0000 */         
    uint32 rsvd1;           /* 0x0004 */
    uint32 EE_FEDACCTRL1;   /* 0x0008 */
    uint32 rsvd2;           /* 0x000C */
    uint32 rsvd3;           /* 0x0010 */
    uint32 FEDAC_PASTATUS;  /* 0x0014 */
    uint32 FEDAC_PBSTATUS;  /* 0x0018 */
    uint32 FEDAC_GBLSTATUS; /* 0x001C */
    uint32 FUNCERRADD;      /* 0x0020 */
    uint32 FEDACSDIS;       /* 0x0024 */
    uint32 FPRIM_ADD_TAG;   /* 0x0028 */
    uint32 FDUP_ADD_TAG;    /* 0x002C */
    uint32 FBPROT;          /* 0x0030 */
    uint32 FBSE;            /* 0x0034 */
    uint32 FBBUSY;          /* 0x0038 */
    uint32 FBAC;            /* 0x003C */
    uint32 FBPWRMODE;       /* 0x0040 */
    uint32 FBPRDY;          /* 0x0044 */
    uint32 FPAC1;           /* 0x0048 */
    uint32 rsvd5;           /* 0x004C */
    uint32 FMAC;            /* 0x0050 */
    uint32 FMSTAT;          /* 0x0054 */
    uint32 FEMU_DMSW;       /* 0x0058 */
    uint32 FEMU_DLSW;       /* 0x005C */
    uint32 FEMU_ECC;        /* 0x0060 */
    uint32 FLOCK;           /* 0x0064 */
    uint32 rsvd6;           /* 0x0068 */
    uint32 FDIAGCTRL;       /* 0x006C */
    uint32 rsvd7;           /* 0x0070 */
    uint32 FRAW_ADDR;       /* 0x0074 */
    uint32 rsvd8;           /* 0x0078 */
    uint32 FPAR_OVR;        /* 0x007C */
    uint32 rsvd9[13U];      /* 0x0080 - 0x00B0 */	
    uint32 RCR_VALID;       /* 0x00B4 */
    uint32 ACC_THRESHOLD;   /* 0x00B8 */	
    uint32 rsvd10;          /* 0x00BC */
    uint32 FEDACSDIS2;      /* 0x00C0 */
    uint32 rsvd11;          /* 0x00C4 */
    uint32 rsvd12;          /* 0x00C8 */
    uint32 rsvd13;          /* 0x00CC */	
    uint32 RCR_VALUE0;      /* 0x00D0 */
    uint32 RCR_VALUE1;      /* 0x00D4 */	
    uint32 rsvd14[109U];    /* 0x00D8 - 0x00284 */	
    uint32 FSM_WR_ENA;      /* 0x0288 */
    uint32 rsvd15[12U];     /* 0x028C - 0x002B4 */
    uint32 EEPROM_CONFIG;   /* 0x02B8 */
    uint32 rsvd16;          /* 0x02BC */
    /* added */
#if 0
    uint32 FSM_SECT1;     /* 0x02C0 */
    uint32 FSM_SECT2;     /* 0x02C4 */
    uint32 rsvd17[79U];     /* 0x02A8 */
    uint32 FCFG_BANK;       /* 0x02B8 */
#endif

} sl_flashWBASE_t;
#endif

/** @def sl_flashWREG
*   @brief Flash Wrapper Register Frame Pointer
*
*   This pointer is used by the system driver to access the flash wrapper registers.
*/
#define sl_flashWREG ((sl_flashWBASE_t *)(0xFFF87000U))

#endif /* __SL_REG_FLASH_H__ */
