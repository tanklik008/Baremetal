/************************************************************************
 *  File: timer.c
 *  
 *  Descirption: This file contains the functions support timer operations.
 *
 *  Copyright (C) : 2008 Hangzhou C-Sky Microsystems Co.,Ltd.
 *  Author: ShuLi Wu
 *  Mail:   shuli_wu@c-sky.com
 *  Date:   Oct 9 2008
 *
 ************************************************************************/

#include "ck810.h"

#include "datatype.h"

#include "cktimer.h"
#include "timer.h"
#include "intc.h"


static CKStruct_TimerInfo CK_Timer_Table[] =
{
    {0, (PCKPStruct_TIMER)CK_TIMER0_BASSADDR, CK_TIMER_IRQ0, FALSE, 0},
    {1, (PCKPStruct_TIMER)CK_TIMER1_BASSADDR, CK_TIMER_IRQ1, FALSE, 0},
    {2, (PCKPStruct_TIMER)CK_TIMER2_BASSADDR, CK_TIMER_IRQ2, FALSE, 0},
    {3, (PCKPStruct_TIMER)CK_TIMER3_BASSADDR, CK_TIMER_IRQ3, FALSE, 0}
};

#define TIMERID_MAX  (sizeof(CK_Timer_Table) / sizeof(CKStruct_TimerInfo) - 1)


/*
 * Make all the timers in the idle state;
 * this function should be called before
 * INTC module working;
 */

void CK_Deactive_TimerModule()
{
    int i;
    PCKPStruct_TIMER ptimer;
    for( i = 0; i < TIMERID_MAX; i++ )
    {
        ptimer = CK_Timer_Table[i].addr;
		/* stop the corresponding timer */
		ptimer->TxControl &= ~CK_TIMER_TXCONTROL_ENABLE;
		/* Disable interrupt. */
		ptimer->TxControl |= CK_TIMER_TXCONTROL_INTMASK;
			  
    }
}



/*
 * intilize the timer driver.
 */
void CK_Timer_Init(void)
{
   CK_Deactive_TimerModule();
}


/*
 * open the timer, register the interrupt.
 *
 * timerid: the number of the corresponding timer port;
 * handler: the interrupt service function of the corresponding timer;
 * bfast: indicate whether the fast interrupt ornot;
 * bopened: indicate the state whether be opened ornot
 */
CK_INT32 CK_Timer_Open ( CKEnum_Timer_Device timerid,
		         IN void(*handler)(void),
		         IN CK_UINT16 priority,
		         IN BOOL bfast
                       )
{
   PCKStruct_TimerInfo info;
   PCKPStruct_TIMER ptimer;
	
   if ((timerid < 0) || (timerid > TIMERID_MAX))
   {
	return FAILURE;
   }
   info = &(CK_Timer_Table[timerid]);
   ptimer = info->addr ;
   if(info->bopened)
   { 
	return FAILURE;
   }
   /* intilize irqhandler */
   if (NULL != handler)
   {
     info->irqhandler.devname = "TIMER";
     info->irqhandler.irqid = info->irq;
     info->irqhandler.priority = priority;
     info->irqhandler.handler = handler;
     info->irqhandler.bfast = bfast;
     info->irqhandler.next = NULL;
     /* register timer isr */
     CK_INTC_RequestIrq(&(info->irqhandler), AUTO_MODE);
   }
   info->bopened = TRUE;
   /* Enable Timer interrupt. */
   ptimer->TxControl &= ~(CK_TIMER_TXCONTROL_INTMASK);
  return SUCCESS;
}

/*
 * close the timer, free interrupt.
 *
 * timerid: the mumber of the corresponding timer port;
 * bopened: indicate the state whether be opened ornot
 */
CK_INT32 CK_Timer_Close(CKEnum_Timer_Device timerid)
{
    PCKStruct_TimerInfo info;
	PCKPStruct_TIMER ptimer;

    if ((timerid < 0) || (timerid > TIMERID_MAX))
    {
	return FAILURE;
    }
    info = &(CK_Timer_Table[timerid]);
    ptimer = info->addr;
    if(!(info->bopened))
    { 
	return FAILURE;
    }

   /* stop the corresponding timer */
   ptimer->TxControl &= ~CK_TIMER_TXCONTROL_ENABLE;
   /* Disable interrupt. */
   ptimer->TxControl |= CK_TIMER_TXCONTROL_INTMASK;
   /*clear the backcall function*/
   CK_INTC_FreeIrq(&(info->irqhandler), AUTO_MODE);
   info->bopened = FALSE;
   return SUCCESS;
}



/*
 * start the corresponding timer
 *
 * timerid: the mumber of the corresponding timer port;
 * timeout: the set time (uS)
 */
CK_INT32 CK_Timer_Start(CKEnum_Timer_Device timerid,IN CK_UINT32 timeout)
{   
   CK_UINT32 load;
   PCKStruct_TimerInfo info;
   PCKPStruct_TIMER ptimer;

   if ((timerid < 0) || (timerid > TIMERID_MAX))
   {
	return FAILURE;
   }
 
   info = &(CK_Timer_Table[timerid]);
   ptimer = info->addr;
   if(!(info->bopened))
   {
	return FAILURE;
   }
    printf("\nJJJ_DEBUG  CK_Timer_Start 0x00 TxControl=0x%x\n", ptimer->TxControl);
    printf("\nJJJ_DEBUG  CK_Timer_Start 0x01 TxCurrentValue=0x%x\n", ptimer->TxCurrentValue);
   load = (CK_UINT32)((APB_DEFAULT_FREQ / 1000000) * timeout);
   /*  load time(us)  */
   ptimer->TxLoadCount = load;
   info->timeout = timeout;
   /* enable the corresponding timer */
   //JJJ_DEBUGptimer->TxControl |= CK_TIMER_TXCONTROL_ENABLE;
  /*in user-defined running mode*/
  //JJJ_DEBUG ptimer->TxControl |= CK_TIMER_TXCONTROL_MODE;
  ptimer->TxControl = CK_TIMER_TXCONTROL_ENABLE | CK_TIMER_TXCONTROL_MODE;

    printf("\nJJJ_DEBUG  CK_Timer_Start 0x02 TxLoadCount=0x%x\n", ptimer->TxLoadCount);
    printf("\nJJJ_DEBUG  CK_Timer_Start 0x03 TxControl=0x%x\n", ptimer->TxControl);
    printf("\nJJJ_DEBUG  CK_Timer_Start 0x04 TxCurrentValue=0x%x\n", ptimer->TxCurrentValue);
    printf("\nJJJ_DEBUG  CK_Timer_Start 0x05 TxIntStatus=0x%x\n", ptimer->TxIntStatus);
   return SUCCESS;
}

/*
 * stop a designated timer
 *
 * timerid: the mumber of the corresponding timer port;
 * stop_val: the count value when the timer stops
 */
CK_UINT32 CK_Timer_Stop(CKEnum_Timer_Device timerid)
{   
   CK_UINT32 stop_val;
   PCKStruct_TimerInfo info;
   PCKPStruct_TIMER ptimer;

   /* if the timer does not open,return failure */
   info = &(CK_Timer_Table[timerid]);
   if(!(info->bopened))
   {
	return FAILURE;
   }

   ptimer = info->addr;
   /* disable the timer*/
   ptimer->TxControl &= ~CK_TIMER_TXCONTROL_ENABLE;
   stop_val = ptimer->TxCurrentValue;
   return stop_val;
}

/* 
 * clear a timer interrupt 
 * by reading its End of Interrupt register(EOI)
 */
void  CK_Timer_ClearIrqFlag(CKEnum_Timer_Device timerid)
{   
   PCKStruct_TimerInfo info;
   PCKPStruct_TIMER ptimer;

   info = &(CK_Timer_Table[timerid]);
   ptimer = info->addr ;
   *((volatile CK_UINT32 *)(&(ptimer->TxEOI)));
}

/*
 * read the current value of the timer
 *
 * timerid: the mumber of the corresponding timer;
 * current_val: the current count-value 
 */
CK_UINT32 CK_Timer_CurrentValue(CKEnum_Timer_Device timerid)
{   
   CK_UINT32 current_val;
   PCKStruct_TimerInfo info;
   PCKPStruct_TIMER ptimer;

   info = &(CK_Timer_Table[timerid]);
   ptimer = info->addr;
   current_val = ptimer->TxCurrentValue;
   return current_val ;
}

