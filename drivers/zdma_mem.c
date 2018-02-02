// Assume device version is ZSP_ZDMA2, so only copy ZSP_ZDMA2 code
#include "zdma.h"
#include "datatype.h"

ZDMA_Chan_Obj_Type zdma_memcpy_descr   ALIGN_4;

/***********************************************************************
 *
 *
 * Function Description: Setup a DMA descriptor for ZDMA version of memcpy().
 *   It needs the DMA control and channel parameters passed.
 *   It uses channel 0 for this descriptor.
 *   It assumes that you have a 64bit AXI bus connected to ZDMA.
 *   It configures the ZDMA for the maximum AXI burst of 16 beats.
 *
 * Input Parameters: Dma channel object.
 *
 * Return:
 *    DMA_STAT_OK           Succesful
 *    Any other value       An error occured
 *
 **********************************************************************/
// Memcpy descriptor for AXI64 only.
void setup_zdma_memcpy(ZDMA_Controller_Cfg_Type *DmaCtrlCfg, ZDMA_Channel_Cfg_Type *DmaChanCfg ) {
      ZDMA_Controller_Init(DmaCtrlCfg);  // with the ZDMA base address and number of channels.
      ZDMA_Get_Controller_Addr(&(DmaChanCfg->pDmaControllerObj));

      // Initialize a descriptor memcpy_zdma_descr to use channel 0.
      DmaChanCfg->Channel = 0;
      ZDMA_ChInit(&zdma_memcpy_descr, DmaChanCfg);

      // 880M has zdma2. 880 has zdma1
//JJJ_DEBUG #ifdef ZSP_ZDMA2
      // Configure the descriptor for memcopy (simple format 1).
      zdma_memcpy_descr.DefaultDescriptor.cfg1.lVal   = ZDMA_CFG1_FMT_1 
                                                      + ZDMA_CFG1_DIM_1 + ZDMA_CFG1_IGEN_DESCRIPTOR;
      zdma_memcpy_descr.DefaultDescriptor.cfg2.lVal   = ZDMA_CFG2_BSTS + ZDMA_CFG2_BSTD 
                                                      + ZDMA_CFG2_SSIZ_64  // AXI64 only
                                                      + ZDMA_CFG2_DSIZ_64  // AXI64 only
                                                      + ZDMA_CFG2_BLIM_16; // largest available
      zdma_memcpy_descr.DefaultDescriptor.ahbLink     = 0L;
//JJJ_DEBUG #endif

}

/***********************************************************************
 *
 *
 * Function Description: Copy memory from src to dst of n words.
 *   Wait(poll) for the channel to be done.
 *   It uses the presetup memcpy descriptor (zdma_memcpy_descr).
 *   It assumes that the descriptor and DMA channel are not in use.
 *
 * Input Parameters: Dma channel object.
 *
 * Return:
 *    DMA_STAT_OK           Succesful
 *    Any other value       An error occured
 *
 **********************************************************************/
int ZDMA_memcpy_poll(void *destn, const void *src, CK_UINT32 n)
  { 
  //JJJ_DEBUG ZDMA_memcpy_nowait(&zdma_memcpy_descr, destn, src, n);
  //JJJ_DEBUG return ZDMA_WaitChDone(&zdma_memcpy_descr);
  return 1;
  }

/***********************************************************************
 *
 *
 * Function Description: Wait(poll) for a channel to be done.
 *   If this is called when a channel was not started, or if a channel
 *   is paused, it will hang.
 *
 * Input Parameters: Dma channel object.
 *
 * Return:
 *    DMA_STAT_OK           Succesful
 *    Any other value       An error occured
 *
 *
 **********************************************************************/
/*JJJ_DEBUG>>
int ZDMA_WaitChDone(ZDMA_Chan_Obj_Type *pDmaChanObj)
  {
  ZDMA_ChannelStatus_Type done;

  // Poll for completion.
  done.lVal = 0L;
  while (!(done.bits.done)) 
    {
    done.lVal = iZDMA_ChGetStatus(pDmaChanObj);
    // Don't need to make a tight loop.
    asm("stopgrp 7; stopgrp 3");
    };

  if (done.bits.ill)
    { 
    return (done.bits.err);
    }
  else
    return (ZDMA_STAT_OK);
  }

int ZDMA_memcpy_nowait(ZDMA_Chan_Obj_Type *pDmaChanObj, void *destn, const void *src, CK_UINT32 n)
  { 
  // This routine is assumes the descriptor is setup for the format, size, etc.
  // Set the source, destination and amount.
  pDmaChanObj->DefaultDescriptor.ahbSrc      = (long int) src;
  pDmaChanObj->DefaultDescriptor.ahbDst      = (long int) destn;
  pDmaChanObj->DefaultDescriptor.Loop0SrcCnt = (unsigned int ) n;

  // Instead of calling this just do the write since we only have the 1 descriptor.
  //status = ZDMA_ChWriteDescriptor(pDmaChanObj, &pDmaChanObj->DefaultDescriptor, 1);
  pDmaChanObj->pChanReg->ndl = (unsigned long) &pDmaChanObj->DefaultDescriptor ;

  // Write the start bit.
  ZDMA_ChWriteControl(pDmaChanObj, ZDMA_CCR_CMD_START);
  return 0;
  }
<<JJJ_DEBUG*/