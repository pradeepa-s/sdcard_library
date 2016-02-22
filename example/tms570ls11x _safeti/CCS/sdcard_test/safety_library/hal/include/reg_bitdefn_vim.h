
/* Vim Register Frame Definition */
/** @struct sl_vimBase
*   @brief Vim Register Frame Definition
*
*   This type is used to access the Vim Registers.
*/

#ifndef __HAL_VIM_H__
#define __HAL_VIM_H__

#include <sl_sys_vim.h>

/* VIM General Configuration */

typedef volatile struct sl_vimRam
{
    sl_t_isrFuncPTR ISR[SL_VIM_CHANNELS + 1];
} sl_vimRAM_t;

#define sl_vimRAM ((sl_vimRAM_t *)0xFFF82000U)

#if defined(_TMS570LC43x_) || defined(_RM57Lx_)
#define VIM_CHANNEL_EPC_FIFO_CAM_FULL		(1u << 28u)
#endif

#endif /* __HAL_VIM_H__ */
