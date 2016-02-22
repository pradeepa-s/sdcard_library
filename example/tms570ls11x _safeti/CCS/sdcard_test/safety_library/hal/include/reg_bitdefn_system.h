
/* System Register Frame 1 Definition */
/** @struct sl_systemBase1
*   @brief System Register Frame 1 Definition
*
*   This type is used to access the System 1 Registers.
*/
#ifndef __HAL_SYSTEM_H__
#define __HAL_SYSTEM_H__

#define SYSESR_PORRST   (uint32)0x00008000u
#define SYSESR_OSCRST   (uint32)0x00004000u
#define SYSESR_WDRST    (uint32)0x00002000u

#if defined(_TMS570LS31x_) || defined(_TMS570LS12x_) || defined(_RM48x_) || defined(_RM46x_) || defined(_RM42x_) || defined(_TMS570LS04x_)
#define SYSESR_CPURST   (uint32)0x00000020u
#endif
#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define SYSESR_CPURST0   (uint32)0x00000020u
#define SYSESR_CPURST1   (uint32)0x00000040u
#endif

#define SYSESR_SWRST    (uint32)0x00000010u
#define SYSESR_EXTRST   (uint32)0x00000008u

#define SYS1_SYSECR_RST1 (uint32)(0x1u << 15u)
#define SYS1_SYSECR_RST0 (uint32)(0x1u << 14u)


/** @struct pcrBase
*   @brief Pcr Register Frame Definition
*
*   This type is used to access the Pcr Registers.
*/
/** @typedef pcrBASE_t
*   @brief PCR Register Frame Type Definition
*
*   This type is used to access the PCR Registers.
*/
typedef volatile struct pcrBase
{
    uint32 PMPROTSET0;    /* 0x0000 */
    uint32 PMPROTSET1;    /* 0x0004 */
    uint32 : 32U;         /* 0x0008 */
    uint32 : 32U;         /* 0x000C */
    uint32 PMPROTCLR0;    /* 0x0010 */
    uint32 PMPROTCLR1;    /* 0x0014 */
    uint32 : 32U;         /* 0x0018 */
    uint32 : 32U;         /* 0x001C */
    uint32 PPROTSET0;     /* 0x0020 */
    uint32 PPROTSET1;     /* 0x0024 */
    uint32 PPROTSET2;     /* 0x0028 */
    uint32 PPROTSET3;     /* 0x002C */
    uint32 : 32U;         /* 0x0030 */
    uint32 : 32U;         /* 0x0034 */
    uint32 : 32U;         /* 0x0038 */
    uint32 : 32U;         /* 0x003C */
    uint32 PPROTCLR0;     /* 0x0040 */
    uint32 PPROTCLR1;     /* 0x0044 */
    uint32 PPROTCLR2;     /* 0x0048 */
    uint32 PPROTCLR3;     /* 0x004C */
    uint32 : 32U;         /* 0x0050 */
    uint32 : 32U;         /* 0x0054 */
    uint32 : 32U;         /* 0x0058 */
    uint32 : 32U;         /* 0x005C */
    uint32 PCSPWRDWNSET0; /* 0x0060 */
    uint32 PCSPWRDWNSET1; /* 0x0064 */
    uint32 : 32U;         /* 0x0068 */
    uint32 : 32U;         /* 0x006C */
    uint32 PCSPWRDWNCLR0; /* 0x0070 */
    uint32 PCSPWRDWNCLR1; /* 0x0074 */
    uint32 : 32U;         /* 0x0078 */
    uint32 : 32U;         /* 0x007C */
    uint32 PSPWRDWNSET0;  /* 0x0080 */
    uint32 PSPWRDWNSET1;  /* 0x0084 */
    uint32 PSPWRDWNSET2;  /* 0x0088 */
    uint32 PSPWRDWNSET3;  /* 0x008C */
    uint32 : 32U;         /* 0x0090 */
    uint32 : 32U;         /* 0x0094 */
    uint32 : 32U;         /* 0x0098 */
    uint32 : 32U;         /* 0x009C */
    uint32 PSPWRDWNCLR0;  /* 0x00A0 */
    uint32 PSPWRDWNCLR1;  /* 0x00A4 */
    uint32 PSPWRDWNCLR2;  /* 0x00A8 */
    uint32 PSPWRDWNCLR3;  /* 0x00AC */
} pcrBASE_t;

/** @def pcrREG
*   @brief Pcr Register Frame Pointer
*
*   This pointer is used by the system driver to access the Pcr registers.
*/
#define pcrREG ((pcrBASE_t *)0xFFFFE000U)

#define SYSREG1_MSINENA_MSIENA (uint32)0x00000001u


#endif /* __HAL_SYSTEM_H__ */
