/* ===========================================================================
 *
 * Module     libbspzsp.a
 * File       psp_mcdma.c
 * Company    VeriSilicon, Inc.
 * Abstract   Peripheral Support Package (PSP) API for the ZDMA peripheral.
 *
 * ===========================================================================
 *
 * Dependencies:
 *
 *      All addresses provided to the DMA controller must be AHB addresses,
 *      not ZSP core addresses.  Those are entries in the descriptor.
 * 		ZDMA functions are called with the ZSP address pointers
 * 		ZDMA functions will do conversion of internal memory ranges to
 * 		aliases accessible from the AHB
 *
 * ===========================================================================
 * Use of this software is covered by the terms and conditions
 * of the ZSP License Agreement under which it is provided.
 *
 * Copyright (C) 2009 VeriSilicon Inc.  All rights reserved.
 *
 */

#if BSP_USE_SIM == 0
//-------------------
// INCLUDE FILES
//-------------------
#include <stdio.h>
//JJJ_DEBUG #include <creg.h>
#include <string.h>
//JJJ_DEBUG #include "bsp.h"
#include "datatype.h"
#include "misc.h"
#include "zdma.h"

ZDMA_Controller_Obj_Type ZDMA_Obj; // ALIGN_2;
//-------------------------------
// LOCAL FUNCTION PROTOTYPES
//-------------------------------

//-------------------------------
// FUNCTION DEFINITIONS
//-------------------------------

/***********************************************************************
 *
 * Function Description:
 * 	Initialize global ZDMA object
 * 	If *cfg is null, return pointer to the object
 *
 * Input Parameters:
 *
 * Return:
 *
 ***********************************************************************
 *
 * Dependencies:
 *
 **********************************************************************/
void ZDMA_Controller_Init(ZDMA_Controller_Cfg_Type *pDmaCtrlCfg)
{
    unsigned short  i;
    ZDMA_Reg_Type  *pDmaReg;

    memset (&ZDMA_Obj, 0, sizeof (ZDMA_Controller_Obj_Type));
    ZDMA_Obj.pDmaBaseAddr = pDmaCtrlCfg->pDmaBaseAddr;
    ZDMA_Obj.NumberChannels = pDmaCtrlCfg->NumberChannels;
    //cleared by memset above
    //    for (i=0;i<pDmaCtrlObj->NumberChannels;i++) {
    //    for (i=0;i<ZDMA_MAX_CHANNELS;i++) {
    //        pDmaCtrlObj->pDmaChanObj[i]=NULL;
    //    }
    pDmaReg = ZDMA_Obj.pDmaBaseAddr;
   	//cleared by memset above
    //pDmaReg->iml=0;                             // Mask off all DMA interrupts
    //pDmaReg->imh=0;
    pDmaReg->intcl=0xffffffff;                    // Clear all DMA channel interrupts
    pDmaReg->evtcl=0xffffffff;                    // Clear all events
    pDmaReg->errcl=0xffffffff;                    // Clear all errors
#if ZDMA_MAX_CHANNELS > 32
    pDmaReg->intch=0xffffffff;
    pDmaReg->evtch=0xffffffff;
    pDmaReg->errch=0xffffffff;
#endif
    for (i = 0; i < ZDMA_Obj.NumberChannels; i++) {
	   	//cleared by memset above
        //pDmaReg->channel[i].pNdl = NULL;
        //pDmaReg->channel[i].ccr.lVal = ZDMA_CCR_SR_MASK;
        pDmaReg->channel[i].ccr.bits.sr = 1;  // Reset each channel
        pDmaReg->channel[i].ccr.bits.sr = 0;
    }
}

/***********************************************************************
 *
 * Function Description:
 *
 * This function retrieves an address to the ZDMA object for further references
 *
 * Input Parameters:
 *
 * Return: void
 *
 ***********************************************************************
 *
 * Dependencies:
 *
 **********************************************************************/
void ZDMA_Get_Controller_Addr(ZDMA_Controller_Obj_Type **pDmaCtrlObj)
{
	*pDmaCtrlObj = &ZDMA_Obj;
}

/***********************************************************************
 *
 * Function Description: Initialize the channel DMA descriptor.
 *
 * Input Parameters: The channel object and the channel configuration object.
 *
 * Return: The channel object.
 *
 ***********************************************************************
 *
 * Dependencies:
 *
 **********************************************************************/
void ZDMA_ChInit(ZDMA_Chan_Obj_Type *pDmaChanObj, ZDMA_Channel_Cfg_Type *pDmaChanCfg)
{
    pDmaChanObj->pSharedReg = pDmaChanCfg->pDmaControllerObj->pDmaBaseAddr;
    pDmaChanObj->pChanReg = &(pDmaChanCfg->pDmaControllerObj->pDmaBaseAddr->channel[pDmaChanCfg->Channel]);
    pDmaChanObj->Channel = pDmaChanCfg->Channel;
    pDmaChanObj->CallbackFunction = NULL;
    pDmaChanObj->pCallbackData = NULL;
    pDmaChanCfg->pDmaControllerObj->pDmaChanObj[pDmaChanCfg->Channel] = pDmaChanObj;
    ZDMA_ChReset(pDmaChanObj);
}

/***********************************************************************
 *
 * Function Description: Reset the hardware of the channel.
 *
 * Input Parameters: The channel object.
 *
 * Return:
 *
 ***********************************************************************
 *
 * Dependencies:
 *
 **********************************************************************/
void ZDMA_ChReset(ZDMA_Chan_Obj_Type *pDmaChanObj)
{
    CK_UINT32 bit;
    pDmaChanObj->pChanReg->ndl = 0L;
    pDmaChanObj->pChanReg->ccr.lVal = 1;
    pDmaChanObj->pChanReg->ccr.lVal = 0;
    //pDmaChanObj->pChanReg->ccr.bits.sr = 1;
    //pDmaChanObj->pChanReg->ccr.bits.sr = 0;
#if ZDMA_MAX_CHANNELS > 32
    if (pDmaChanObj->Channel >= 32) {
	bit = 1 << (pDmaChanObj->Channel-32);
        pDmaChanObj->pSharedReg->intch = bit;
	    pDmaChanObj->pSharedReg->evtch = bit;
	pDmaChanObj->pSharedReg->errch = bit;
    } else
#endif //ZDMA_MAX_CHANNELS > 32
	{
	bit = 1 << (pDmaChanObj->Channel);
        pDmaChanObj->pSharedReg->intcl = bit;
	    pDmaChanObj->pSharedReg->evtcl = bit;
	pDmaChanObj->pSharedReg->errcl = bit;
	}
}

/***********************************************************************
 *
 * Function Description:
 *
 * Input Parameters:
 * pDmaObj - pointer to structure describing ZDMA object
 * pDescriptor - pointer to the transfer descriptor in ZSP int/ext memory
 *
 * Return:
 *
 ***********************************************************************
 *
 * Dependencies:
 *
 **********************************************************************/

/*
static void xlateAhbInt2AhbExt (pUINT32 pVal)
{
	if (*pVal < (ZSP800_IMEM_BASE_AHB + ZSP800_IMEM_SIZE_AHB))
		*pVal += (ZSP800_IMEM_ALIAS_AHB - ZSP800_IMEM_BASE_AHB);
	else if (*pVal < (ZSP800_DMEM_BASE_AHB + ZSP800_DMEM_SIZE_AHB))
		*pVal += (ZSP800_DMEM_ALIAS_AHB - ZSP800_DMEM_BASE_AHB);
}
*/

/***********************************************************************
 *
 * Function Description: Write a descriptor (list) into memory
 *
 * Input Parameters: The Dma channel descriptor, the descriptor chain and a maximum count.
 *
 * Return: status of the operation.
 *
 ***********************************************************************
 *
 * Dependencies:
 *
 **********************************************************************/
int ZDMA_ChWriteDescriptor(ZDMA_Chan_Obj_Type *pDmaChanObj, ZDMA_Descriptor_Type *pDescriptor, 
                           int maxCnt)
{
	ZDMA_Descriptor_Type *pThis, *pNext = pDescriptor;
	while (pNext != 0 && maxCnt--) {
		pThis = pNext;
		// Descriptor list must be aligned to 32 bit address
		if ( ((CK_UINT32)pThis & ZDMA_NDL_ALIGNMENT_MASK) != 0)
			return(ZDMA_STAT_LIST_NOT_ALIGNED);
		if (pThis->ahbLink == 0)
			break;
		pNext = (ZDMA_Descriptor_Type *)(pThis->ahbLink >> 1);
	}
	pDmaChanObj->pChanReg->ndl = (CK_UINT32)pDescriptor;
	return (ZDMA_STAT_OK);
}

/***********************************************************************
 *
 * ZDMA_ChNeedsService
 *
 * Function Description:
 *
 *      This function checks to see if the DMA has completed a transfer.
 *      It's different than checking for DMA to not be busy, if there is
 *      a possibility that the DMA never really got started.
 * 		It requires configuring transfer with interrupt enabled, and
 * 		clearing interrupt bit before starting it
 *
 *  Input Parameters:
 *
 *  Return:
 *
 *      0 = DMA doesn't require service
 *      1 = DMA channel requires service
 *
 ***********************************************************************
 *
 * Dependencies:
 *
 **********************************************************************/
int ZDMA_ChNeedsService(ZDMA_Chan_Obj_Type *pDmaChanObj)
{
    #if ZDMA_MAX_CHANNELS > 32
	if (pDmaChanObj->Channel >= 32)
		return ( ((pDmaChanObj->pSharedReg->intrh) >> (pDmaChanObj->Channel-32)) & 0x1 );
    #endif //ZDMA_MAX_CHANNELS > 32
	return ( ((pDmaChanObj->pSharedReg->intrl) >> pDmaChanObj->Channel) & 0x1 );
}

/***********************************************************************
 *
 * Function Description: Clear out interrupt register for the specified DMA.
 *
 * Input Parameters: DMA channel object.
 *
 * Return: void
 *
 ***********************************************************************
 *
 * Dependencies:
 *
 **********************************************************************/
void ZDMA_ChClearService(ZDMA_Chan_Obj_Type *pDmaChanObj)
{
    #if ZDMA_MAX_CHANNELS > 32
    if (pDmaChanObj->Channel >= 32)
        pDmaChanObj->pSharedReg->intch = 1 << (pDmaChanObj->Channel-32);
    else
    #endif //ZDMA_MAX_CHANNELS > 32
        pDmaChanObj->pSharedReg->intcl = 1 << pDmaChanObj->Channel;
}

#endif //BSP_USE_SIM == 0
