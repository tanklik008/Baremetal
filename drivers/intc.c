/*
  * Description: this file contains the functions support
  *              interrupts and exceptions
  * Copyright (C):  2008 C-SKY Microsystem  Ltd.
  * Author(s): Dongqi Hu  (dongqi_hu@c-sky.com)
  *            Jianyong jiang  (jianyong_jiang@c-sky.com)
  * Contributors: Chunqiang Li 
  * Date:         2008-9-26
  */

#include "ck810.h"

#include "datatype.h"
#include "intc.h"
#include "ckintc.h"

#include "misc.h"
#include "cache.h"


/*
 * NR_IRQS: The number of member in normal_irq_list and fast_irq_list 
 *          respective.It is equal to the number of priority levels.
 */
#define NR_IRQS  64

/*
 * normal_irq_list[NR_IRQS]: All the normal interrupt service routines should
 *                           been registered in this list.
 *
 * fast_irq_list[NR_IRQS]: All the fast interrupt service routines should been
 *                         registered in this list.
 */
static PCKStruct_IRQHandler  normal_irq_list[NR_IRQS] ALIGN_4;
static PCKStruct_IRQHandler  fast_irq_list[NR_IRQS] ALIGN_4;

/* PCK_INTC: The base address of interrupt controller registers */
volatile CKStruct_INTC *icrp = PCK_INTC;

static char *exception_names[] = 
{
    "RESET PC", "BUS ERROR", "ADDRESS ERROR", "ZERO DIVIDE",
    "ILLEGAL INSTRUCTION",  "PRIVILEGE VIOLATION",
    "TRACE", "BREAK POIOT ERROR", "FATAL ERROR" , "Idly4 ERROR",
    "" , "" ,  "UNASSIGNED RESERVED HAI", "UNASSIGNED RESERVED FP",  
    "" , "" , "TRAP #0" , "TRAP #1", "TRAP #2", "TRAP #3",  
    "UNASSIGNED RESERVED 20", "UNASSIGNED RESERVED 21", 
    "UNASSIGNED RESERVED 22", "UNASSIGNED RESERVED 23", 
    "UNASSIGNED RESERVED 24", "UNASSIGNED RESERVED 25", 
    "UNASSIGNED RESERVED 26", "UNASSIGNED RESERVED 27", 
    "UNASSIGNED RESERVED 28", "UNASSIGNED RESERVED 29", 
    "UNASSIGNED RESERVED 30",  "SYSTEM DESCRIPTION POINT", 
};


/*
 * CKCORE_SAVED_CONTEXT -- Saved by a normal interrupt or exception
 */
typedef struct
{       
    CK_UINT32 pc;
    CK_UINT32 psr;
    CK_UINT32 r1; 
    CK_UINT32 r2; 
    CK_UINT32 r3; 
    CK_UINT32 r4; 
    CK_UINT32 r5; 
    CK_UINT32 r6; 
    CK_UINT32 r7; 
    CK_UINT32 r8; 
    CK_UINT32 r9; 
    CK_UINT32 r10;
    CK_UINT32 r11;
    CK_UINT32 r12;
    CK_UINT32 r13;
    CK_UINT32 r14;
    CK_UINT32 r15;
} __attribute__ ((aligned, packed)) Ckcore_SavedRegisters;


extern void hw_vsr_default_exception();
extern void hw_vsr_autovec();
extern void hw_vsr_fastautovec();
extern void hw_vsr_tlbmiss();

/*************************************************************************** 
This function initializes normal_irq_list[NR_IRQS], fast_irq_list[NR_IRQS]
and PR0-PR63.

mode:   Interrupt controller working mode
        0   ->  Auto Vectored Mode  (Auto vectored interrupt requests)
        1   ->  Vectored Share Mode (Fast and Normal vectored interrupt requests share same vector number)
        2   ->  Vectored Unique Mode (Fast and Normal vectored interrupt requests have unique vector number)

***************************************************************************/ 
void CK_INTC_Init(IN CK_UINT32 mode)
{
    int i;

    icrp->ICR_ISR = 0;

    switch(mode) {
    case AUTO_MODE:
        for (i = 0; i < NR_IRQS; i++)
        {
            normal_irq_list[i] = NULL;
        }
            
        for (i = 0;i < NR_IRQS; i++)
        {
            fast_irq_list[i] = NULL;
        }
        icrp->ICR_ISR |= ICR_AVE;
        break;
    case VECTOR_SHARE_MODE:
        icrp->ICR_ISR &= ~(ICR_AVE | ICR_FVE);
        break;
    case VECTOR_UNIQUE_MODE:
        icrp->ICR_ISR &= ~ICR_AVE;
        icrp->ICR_ISR |= ICR_FVE;
        break;
    default:
        printf('error: unsupported mode 0x%x', mode);
    }
    
    /* initialize PR0-PR31, big endian */
    icrp->PR[0] = 0x00010203;
    icrp->PR[1] = 0x04050607;
    icrp->PR[2] = 0x08090a0b;
    icrp->PR[3] = 0x0c0d0e0f;
    icrp->PR[4] = 0x10111213;
    icrp->PR[5] = 0x14151617;
    icrp->PR[6] = 0x18191a1b;
    icrp->PR[7] = 0x1c1d1e1f;
    icrp->PR[8] = 0x20212223;
    icrp->PR[9] = 0x24252627;
    icrp->PR[10] = 0x28292a2b;
    icrp->PR[11] = 0x2c2d2e2f;
    icrp->PR[12] = 0x30313233;
    icrp->PR[13] = 0x34353637;
    icrp->PR[14] = 0x38393a3b;
    icrp->PR[15] = 0x3c3d3e3f;
    printf("JJJ_DEBUG CK_INTC_Init ICR_ISR=0x%x\n", icrp->ICR_ISR);
}

/******************************************************************
This function enables a priority level of normal interrupt.

priotity: A priority of normal interrupt which between 0 to 31.

*******************************************************************/
void CK_INTC_EnNormalIrq(IN CK_UINT32 irq_num) {
	CK_UINT32 psrbk;

	CK_CPU_EnterCritical(&psrbk);
	if (irq_num > 31) {
		icrp->NIERH |= (1 << (irq_num - 32));
        printf("JJJ_DEBUG CK_INTC_EnNormalIrq NIERH = 0x%x\n", icrp->NIERH);
	} else {
		icrp->NIERL |= (1 << irq_num);
        printf("JJJ_DEBUG CK_INTC_EnNormalIrq NIERL = 0x%x\n", icrp->NIERL);
	}
	CK_CPU_ExitCritical(psrbk);
}
 
/************************************************************************
This function disables a priority level of normal interrupt.

priotity: A priority of normal interrupt which between 0 to 31.

**********************************************************************/
void CK_INTC_DisNormalIrq(IN CK_UINT32 irq_num) {
	CK_UINT32 psrbk;

	CK_CPU_EnterCritical(&psrbk);
	if (irq_num > 31) {
		icrp->NIERH &= ~(1 << (irq_num - 32));
        printf("JJJ_DEBUG CK_INTC_DisNormalIrq NIERH = 0x%x\n", icrp->NIERH);
	} else {
		icrp->NIERL &= ~(1 << irq_num);
        printf("JJJ_DEBUG CK_INTC_DisNormalIrq NIERL = 0x%x\n", icrp->NIERL);
	}
	CK_CPU_ExitCritical(psrbk);
}

/***********************************************************************
This function enables a priority level of fast interrupt.

priotity: A priority of fast interrupt which between 0 to 31.

***********************************************************************/
void CK_INTC_EnFastIrq(IN CK_UINT32 irq_num) {
	CK_UINT32 psrbk;

	CK_CPU_EnterCritical(&psrbk);
	if (irq_num > 31) {
		icrp->FIERH |= (1 << (irq_num - 32));
        printf("JJJ_DEBUG CK_INTC_EnFastIrq FIERH = 0x%x\n", icrp->FIERH);
	} else {
		icrp->FIERL |= (1 << irq_num);
        printf("JJJ_DEBUG CK_INTC_EnFastIrq FIERL = 0x%x\n", icrp->FIERH);
	}
	CK_CPU_ExitCritical(psrbk);
}

/***************************************************************************
This function disables a priority level of fast interrupt.

priotity: A priority of fast interrupt which between 0 to 31.

**************************************************************************/
void CK_INTC_DisFastIrq(IN CK_UINT32 irq_num) {
	CK_UINT32 psrbk;

	CK_CPU_EnterCritical(&psrbk);
	if (irq_num > 31) {
		icrp->FIERH &= ~(1 << (irq_num - 32));
        printf("JJJ_DEBUG CK_INTC_DisFastIrq FIERH = 0x%x\n", icrp->FIERH);
	} else {
		icrp->FIERL &= ~(1 << irq_num);
        printf("JJJ_DEBUG CK_INTC_DisFastIrq FIERH = 0x%x\n", icrp->FIERH);
	}
	CK_CPU_ExitCritical(psrbk);
}
/*****************************************************************************
This function enables normal interrupt masking.

primask: The priority level that would be masked, leading to mask those 
         priority levels below primask.

****************************************************************************/
void CK_INTC_MaskNormalIrq(IN CK_UINT32 primask)
{
  IN CK_UINT32 temp_icr;
  CK_UINT32 psrbk;

  temp_icr = icrp->ICR_ISR;

  /*
   * This function will be implemented When fast interrupt masking is disabled,
   * or return at once.
   */
  if((temp_icr &= 0x10000000) == 0x10000000)
  {
      printf("JJJ_DEBUG CK_INTC_MaskNormalIrq 0x00\n");
    return;
  }

  else
  {
    CK_CPU_EnterCritical(&psrbk);
    icrp->ICR_ISR &= 0xffe0ffff;
    icrp->ICR_ISR |= ((primask & 0x0000001f) << 16);
    icrp->ICR_ISR |= ICR_ME;
    CK_CPU_ExitCritical(psrbk);
    printf("JJJ_DEBUG CK_INTC_MaskNormalIrq ICR_ISR = 0x%x\n", icrp->ICR_ISR);
  }
}

/******************************************************************
This function disables normal interrupt masking.

primask: The priority level that would be unmasked.

*********************************************************************/
void CK_INTC_UnMaskNormalIrq(IN CK_UINT32 primask)
{
    CK_UINT32 psrbk;
    
    CK_CPU_EnterCritical(&psrbk);
    icrp->ICR_ISR &= 0xffe0ffff;
    icrp->ICR_ISR |= ((primask & 0x0000001f) << 16);
    icrp->ICR_ISR &= ~ICR_ME;
    CK_CPU_ExitCritical(psrbk);
    printf("JJJ_DEBUG CK_INTC_UnMaskNormalIrq ICR_ISR = 0x%x\n", icrp->ICR_ISR);
}

/************************************************************************
This function enables fast interrupt masking.

primask: The priority level that would be masked, leading to mask those 
         priority levels below primask.

**********************************************************************/
void CK_INTC_MaskFastIrq(IN CK_UINT32 primask)
{
  CK_UINT32 psrbk;

  CK_CPU_EnterCritical(&psrbk);
  icrp->ICR_ISR &= 0xffe0ffff;
  icrp->ICR_ISR |= ((primask & 0x0000001f) << 16);
  icrp->ICR_ISR |= ICR_MFI;
  icrp->ICR_ISR |= ICR_ME;
  CK_CPU_ExitCritical(psrbk);
  printf("JJJ_DEBUG CK_INTC_MaskFastIrq ICR_ISR = 0x%x\n", icrp->ICR_ISR);
}

/**********************************************************************
This function disables fast interrupt masking.

primask: The priority level that would be unmasked.

***********************************************************************/
void CK_INTC_UnMaskFastIrq(IN CK_UINT32 primask)
{
    CK_UINT32 psrbk;
    
    CK_CPU_EnterCritical(&psrbk);
    icrp->ICR_ISR &= 0xffe0ffff;
    icrp->ICR_ISR |= ((primask & 0x0000001f) << 16);
    icrp->ICR_ISR &= ~ICR_ME;
    icrp->ICR_ISR &= ~ICR_MFI;
    CK_CPU_ExitCritical(psrbk);
    printf("JJJ_DEBUG CK_INTC_UnMaskFastIrq ICR_ISR = 0x%x\n", icrp->ICR_ISR);
}

/**********************************************************************
This function is used for registering interrupt functions

pirqhandler: The pointer pointing the interrupt data struct which would be 
             registerd.
             
mode:   Interrupt controller working mode
        0   ->  Auto Vectored Mode  (Auto vectored interrupt requests)
        1   ->  Vectored Share Mode (Fast and Normal vectored interrupt requests share same vector number)
        2   ->  Vectored Unique Mode (Fast and Normal vectored interrupt requests have unique vector number)
        
********************************************************************/
CK_INT32 CK_INTC_RequestIrq(PCKStruct_IRQHandler pirqhandler, IN CK_UINT32 mode)
{
  CK_UINT32 pr_index;
  CK_UINT32 shift;
  CK_UINT32 psrbk;

  /* Judge the validity of pirqhandler */
  if((NULL == pirqhandler) ||
     (pirqhandler->handler == NULL) ||
     (pirqhandler->priority < 0 || pirqhandler->priority > 63) ||
     ((pirqhandler->irqid < 0) || pirqhandler->irqid > 63))
  {
    return FAILURE;
  }
  
  printf("JJJ_DEBUG CK_INTC_RequestIrq irqid=0x%x\n", pirqhandler->irqid);
  /* Assigns pirqhandler->priority to corresponding interrupt source */
  pr_index = (pirqhandler->irqid) / 4;
  shift = (3-(pirqhandler->irqid) % 4) * 8;
  CK_CPU_EnterCritical(&psrbk);
  icrp->PR[pr_index] &= ~(0x000000ff << shift);
  icrp->PR[pr_index] |= ((pirqhandler->priority) << shift);
    printf("JJJ_DEBUG CK_INTC_RequestIrq pr_index=%d\n", pr_index);
    printf("JJJ_DEBUG CK_INTC_RequestIrq shift=0x%x\n", shift);
    printf("JJJ_DEBUG CK_INTC_RequestIrq icrp->PR[%d]=0x%x\n", pr_index, icrp->PR[pr_index]);
    
    switch(mode) {
    case AUTO_MODE:
        /* If is normal interrupt */
        if (!(pirqhandler->bfast))
        {
            /* If the list of this priority is empty */
            if(NULL == (normal_irq_list[pirqhandler->priority]))
            {
                //printf("JJJ_DEBUG CK_INTC_RequestIrq 0x00 priority=%d", pirqhandler->priority);
            normal_irq_list[pirqhandler->priority] = pirqhandler;
            normal_irq_list[pirqhandler->priority]->next = NULL;
            CK_INTC_EnNormalIrq( pirqhandler->priority );
            }
        
            /* If the list of this priority is not empty */
            else
            {
                //printf("JJJ_DEBUG CK_INTC_RequestIrq 0x01 priority=%d", pirqhandler->priority);
            pirqhandler->next = normal_irq_list[pirqhandler->priority];
            normal_irq_list[pirqhandler->priority] = pirqhandler;
            }
        }
        
        /* If is fast interrupt */
        else
        {
            /* If the list of this priority is empty */
            if (NULL == (fast_irq_list[pirqhandler->priority]))
            {
                //printf("JJJ_DEBUG CK_INTC_RequestIrq 0x02 priority=%d", pirqhandler->priority);
            fast_irq_list[pirqhandler->priority] = pirqhandler;
            fast_irq_list[pirqhandler->priority]->next = NULL;
            CK_INTC_EnFastIrq(pirqhandler->priority);
            }
            /* If the list of this priority is not empty */
            else
            {
                //printf("JJJ_DEBUG CK_INTC_RequestIrq 0x03 priority=%d", pirqhandler->priority);
            pirqhandler->next = fast_irq_list[pirqhandler->priority];
            fast_irq_list[pirqhandler->priority] = pirqhandler;
            }
        }
        break;
    case VECTOR_SHARE_MODE:
        ckcpu_vsr_table[CKCORE_VECTOR_SYS + pirqhandler->priority]
                    =(CK_UINT32) pirqhandler->handler;
        printf("JJJ_DEBUG CK_INTC_RequestIrq VECTOR_SHARE_MODE priority=%d\n", pirqhandler->priority);
        /* If is normal interrupt */
        if (!(pirqhandler->bfast)) {
            printf("JJJ_DEBUG CK_INTC_RequestIrq VECTOR_SHARE_MODE normal\n");
            CK_INTC_EnNormalIrq( pirqhandler->priority );
        }
        /* If is fast interrupt */
        else {
            printf("JJJ_DEBUG CK_INTC_RequestIrq VECTOR_SHARE_MODE fast\n");
            CK_INTC_EnFastIrq( pirqhandler->priority );
        }
        break;
    case VECTOR_UNIQUE_MODE:
        /* If is normal interrupt */
        if (!(pirqhandler->bfast)) 
        {
            printf("JJJ_DEBUG CK_INTC_RequestIrq VECTOR_UNIQUE_MODE normal\n");
            ckcpu_vsr_table[CKCORE_VECTOR_SYS + pirqhandler->priority]
                    =(CK_UINT32) pirqhandler->handler;
            CK_INTC_EnNormalIrq( pirqhandler->priority );
        } 
        /* If is fast interrupt */
        else 
        {
            printf("JJJ_DEBUG CK_INTC_RequestIrq VECTOR_UNIQUE_MODE fast\n");
            ckcpu_vsr_table[CKCORE_VECTOR_FASTVEC + pirqhandler->priority]
                    =(CK_UINT32) pirqhandler->handler;
            CK_INTC_EnFastIrq( pirqhandler->priority );
            
        }
        break;
    default:
        printf('error: unsupported mode 0x%x', mode);
    }
  CK_CPU_ExitCritical(psrbk);
  return SUCCESS;
}



/*************************************************************************** 
This function is used for freeing those functions which have been register

pirqhandler: The pointer pointing to the interrupt service function which
             has been register. 

mode:   Interrupt controller working mode
        0   ->  Auto Vectored Mode  (Auto vectored interrupt requests)
        1   ->  Vectored Share Mode (Fast and Normal vectored interrupt requests share same vector number)
        2   ->  Vectored Unique Mode (Fast and Normal vectored interrupt requests have unique vector number)
        
**************************************************************************/
CK_INT32 CK_INTC_FreeIrq(INOUT PCKStruct_IRQHandler pirqhandler, IN CK_UINT32 mode)
{
    PCKStruct_IRQHandler  ptemp;
    PCKStruct_IRQHandler  pre_node = NULL;
    
    printf("JJJ_DEBUG CK_INTC_FreeIrq irqid=0x%x, priority=0x%x\n", pirqhandler->irqid, pirqhandler->priority);

    if (pirqhandler == NULL)
        printf("JJJ_DEBUG pirqhandler == NULL\n");
    
    if (pirqhandler->handler == NULL)
        printf("JJJ_DEBUG pirqhandler->handler == NULL\n");
    
    /* Judge the validity of pirqhandler */
    if((pirqhandler == NULL) ||
        (pirqhandler->handler == NULL) ||
        ((pirqhandler->irqid < 0) || (pirqhandler->irqid > 63)))
    {
        printf("JJJ_DEBUG CK_INTC_FreeIrq FAILURE\n");
        return FAILURE;
    }
    
    switch (mode) {
    case AUTO_MODE:
        /* If is normal interrupt */
        if (!(pirqhandler->bfast))
        {
            if (NULL == normal_irq_list[pirqhandler->priority])
            {
            return FAILURE;
            }
            pre_node = ptemp = normal_irq_list[pirqhandler->priority];
            /* Find the location of pirqhandler in the list */
            while (ptemp != pirqhandler)
            {
            pre_node = ptemp;
            ptemp = ptemp->next;
            }
            /* Delete pirqhandler from the list */
            if (ptemp == normal_irq_list[pirqhandler->priority])
            {
            normal_irq_list[pirqhandler->priority] = ptemp->next;
            }
            else if (NULL != ptemp)
            {
            pre_node->next = ptemp->next;
            }
            else
            {
            return FAILURE;
            }
        
            /* If the list which has been deleted becomes empty */
            if (NULL == normal_irq_list[pirqhandler->priority])
            {
            CK_INTC_DisNormalIrq(pirqhandler->priority);
            }
        }
        
        /* If is fast interrupt */
        else
        {
            if (NULL == fast_irq_list[pirqhandler->priority])
            {
            return FAILURE;
            }
            pre_node = ptemp = fast_irq_list[pirqhandler->priority];
        
            /* Find the location of pirqhandler in the list */
            while (ptemp != pirqhandler)
            {
            pre_node = ptemp;
            ptemp = ptemp->next;
            }
        
            /* Delete pirqhandler from the list */
            if (ptemp == fast_irq_list[pirqhandler->priority])
            {
            fast_irq_list[pirqhandler->priority] = ptemp->next;
            }
            else if (NULL != ptemp)
            {
            pre_node->next = ptemp->next;
            }
            else
            {
            return FAILURE;
            }
        
            /* If the list which has been deleted becomes empty */
            if (NULL == fast_irq_list[pirqhandler->priority])
            {
            CK_INTC_DisFastIrq(pirqhandler->priority);
            }
        } /* if (!(pirqhandler->bfast)) */
        ptemp = NULL;
        break;
    case VECTOR_SHARE_MODE:
        printf("JJJ_DEBUG CK_INTC_FreeIrq VECTOR_SHARE_MODE 0x00\n");
        ckcpu_vsr_table[CKCORE_VECTOR_SYS + pirqhandler->priority]
                    =(CK_UINT32) hw_vsr_default_exception;
        printf("JJJ_DEBUG CK_INTC_FreeIrq VECTOR_SHARE_MODE 0x01\n");
        /* If is normal interrupt */
        if (!(pirqhandler->bfast))
            CK_INTC_DisNormalIrq( pirqhandler->priority );
        /* If is fast interrupt */
        else
            CK_INTC_DisFastIrq( pirqhandler->priority );
        printf("JJJ_DEBUG CK_INTC_FreeIrq VECTOR_SHARE_MODE 0x02\n");
        break;
    case VECTOR_UNIQUE_MODE:
        /* If is normal interrupt */
        if (!(pirqhandler->bfast)) 
        {
            ckcpu_vsr_table[CKCORE_VECTOR_SYS + pirqhandler->priority]
                    =(CK_UINT32) hw_vsr_default_exception;
            CK_INTC_DisNormalIrq( pirqhandler->priority );
        } 
        /* If is fast interrupt */
        else 
        {
            ckcpu_vsr_table[CKCORE_VECTOR_FASTVEC + pirqhandler->priority]
                    =(CK_UINT32) hw_vsr_default_exception;
            CK_INTC_DisFastIrq( pirqhandler->priority );
        }
        break;
    default:
        printf('error: unsupported mode 0x%x', mode);
    }

  return SUCCESS;
 }

/******************************************************************
CK_Default_Exception_Handler -- The default exception handler

INPUT: 
vector : the exception vector number

regs : the registers that have restored

RETURN VALUE: None

******************************************************************/

void CK_Default_Exception_Handler(int vector, Ckcore_SavedRegisters *regs)
{
  printf("Exception: %s\n", exception_names[vector]);
}

/*********************************************************
CK_Exception_Init -- Initialize the vector table

INPUT: None

RETURN VALUE: None

*********************************************************/

void CK_Exception_Init (void)
{
  int i;

  // set all exception vector table as hw_vsr_default_exception
  for (i = 0; i < CKCORE_VECTOR_SYS; i++)
  {
	  ckcpu_vsr_table[i] =(CK_UINT32) hw_vsr_default_exception;
  }

  // set the vector table for AUTOVEC as hw_vsr_autovec
  ckcpu_vsr_table[CKCORE_VECTOR_AUTOVEC] = (CK_UINT32) hw_vsr_autovec;
  // set the vector table for FASTAUTOVEC as hw_vsr_fastautovec
  //JJJ_DEBUG ckcpu_vsr_table[CKCORE_VECTOR_FASTAUTOVEC] =
                    (CK_UINT32) hw_vsr_fastautovec | 0x1;
    ckcpu_vsr_table[CKCORE_VECTOR_FASTAUTOVEC] = (CK_UINT32) hw_vsr_fastautovec;
#if CONFIG_CKCPU_MMU
//  // set the vector table for TLBMISS as hw_vsr_tlbmiss
  ckcpu_vsr_table[CKCORE_VECTOR_TLBMISS] = (CK_UINT32) hw_vsr_tlbmiss;
#endif

  CK_CPU_EnAllNormalIrq();
  CK_CPU_EnAllFastIrq();

#if CONFIG_CKCPU_DCACHE
  __clear_dcache();
#endif
#if CONFIG_CKCPU_ICACHE
  __flush_icache();
#endif  
}

/*********************************************************
CK_INTC_InterruptService -- Execute the interrupt service,
called by "Inthandler"

INPUT:
 
offset : the offset in  the array normal_irq_list

RETURN VALUE: None

*********************************************************/

void CK_INTC_InterruptService (int offset)
{
    PCKStruct_IRQHandler phandler;
    
    phandler = normal_irq_list[offset];
    //printf("JJJ_DEBUG CK_INTC_InterruptService offset = 0x%x", offset);
    
    while( phandler != NULL)
    {
        CK_INTC_MaskNormalIrq(offset); //JJJ_DEBUG
        printf("JJJ_DEBUG CK_INTC_InterruptService irqid = 0x%x", phandler->irqid);
        /* phandler -> handler must not be NULL */
        phandler -> handler(phandler->irqid);
        phandler = phandler->next;
        CK_INTC_UnMaskNormalIrq(offset); //JJJ_DEBUG
    }
}

/*********************************************************
CK_INTC_FastInterruptService -- Execute the fast interrupt service,
called by "Fasthandler"
INPUT:
 
offset : the offset in  the array fast_irq_list

RETURN VALUE: None

*********************************************************/

void CK_INTC_FastInterruptService (int offset)
{
    PCKStruct_IRQHandler phandler;
    
    phandler = fast_irq_list[offset];
    //printf("JJJ_DEBUG CK_INTC_FastInterruptService offset = 0x%x\n", offset);
    
    while( phandler != NULL)
    {
        CK_INTC_MaskFastIrq(offset); //JJJ_DEBUG
        printf("JJJ_DEBUG CK_INTC_FastInterruptService irqid = 0x%x\n", phandler->irqid);
        /* phandler -> handler must not be NULL */
        phandler -> handler(phandler->irqid);
        phandler = phandler->next;
        CK_INTC_UnMaskFastIrq(offset); //JJJ_DEBUG
    }
}

/*********************************************************
CK_CPU_EnAllNormalIrq -- Config cpu to enable all normal
interrupt

INPUT: None

RETURN VALUE: None

*********************************************************/

void CK_CPU_EnAllNormalIrq(void)
{
  asm  ("psrset ee,ie");
}

/*********************************************************
CK_CPU_DisAllNormalIrq -- Config cpu to disable all normal
interrupt

INPUT: None

RETURN VALUE: None

*********************************************************/

void CK_CPU_DisAllNormalIrq(void)
{
 asm  ("psrclr ie"); 
}

/*********************************************************
CK_CPU_EnAllFastIrq -- Config cpu to enable all fast
interrupt

INPUT: None

RETURN VALUE: None

*********************************************************/
void CK_CPU_EnAllFastIrq(void)
{
 asm  ("psrset fe");
}

/*********************************************************
CK_CPU_DisAllFastIrq -- Config cpu to disable all fast
interrupt

INPUT: None

RETURN VALUE: None

*********************************************************/
void CK_CPU_DisAllFastIrq(void)
{
  asm  ("psrclr fe"); 
}

/*********************************************************
CK_CPU_EnterCritical -- This function should be called 
before executing critical code which should not be 
interrupted by other interrupts  


INPUT: psr, to store the value of psr

RETURN VALUE: None

*********************************************************/

void CK_CPU_EnterCritical(CK_UINT32 *psr)
{
  asm volatile ("mfcr    %0, psr\n\r"
                "psrclr  ie, fe"
                 : "=r" (*psr) );
}

/*********************************************************
CK_CPU_ExitCritical -- This function should be called 
after exiting critical area.

INPUT: psr, contain the backup value of psr

RETURN VALUE: None

*********************************************************/

void CK_CPU_ExitCritical(CK_UINT32 psr)
{
  asm volatile ("mtcr   %0, psr"
                 : 
                 :"r"(psr));
}
