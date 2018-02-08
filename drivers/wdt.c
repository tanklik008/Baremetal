/*****************************************************************************
 *  File: wdt.c
 *  
 *  Descirption: contains the functions support Synopsys Watchdog Timer.
 *
 *  Copyright (C) : 2018 VeriSilicon.
 * 
 *  Author: Chunming Li
 *  Mail:   shuli_wu@c-sky.com
 *  Date:   Feb 9 2018
 *
 *****************************************************************************/

#include "ck810.h"
#include "datatype.h"
#include "ckwdt.h"
#include "wdt.h"
#include "intc.h"

static CKStruct_WDTInfo CK_WDT_Table[] =
{
    {0, (PCKPStruct_WDT)CK_WDT_ADDRBASE0, CK_INTC_WDT0, FALSE, 0, NULL},
    //{1, (PCKPStruct_WDT)CK_WDT_ADDRBASE1, CK_INTC_WDT0, FALSE, 0, NULL},
    //{2, (PCKPStruct_WDT)CK_WDT_ADDRBASE2, CK_INTC_WDT0, FALSE, 0, NULL},
};


#define WDTRID_MAX  (sizeof(CK_WDT_Table) / sizeof(CKStruct_WDTInfo))


/*
 * Make all the watchdog timers in the idle state;
 * this function should be called before
 * INTC module working;
 */

void CK_Deactive_WDTModule()
{
    int i;
    PCKPStruct_WDT pwdt;
    for( i = 0; i < WDTRID_MAX; i++ )
    {
        pwdt = CK_WDT_Table[i].addr;
        /* Set Response mode to system reset*/
		/* stop the corresponding watchdog timer */
		pwdt->WDT_CR &= ~(CK_WDT_CR_ENABLE | CK_WDT_CR_RMOD);
        
        #if CK_WDT_DEBUG
            printf("JJJ_DEBUG  WDT%d WDT_COMP_PARAMS_5=0x%x\n", i, pwdt->WDT_COMP_PARAMS_5);
            printf("JJJ_DEBUG  WDT%d WDT_COMP_PARAMS_4=0x%x\n", i, pwdt->WDT_COMP_PARAMS_4);
            printf("JJJ_DEBUG  WDT%d WDT_COMP_PARAMS_3=0x%x\n", i, pwdt->WDT_COMP_PARAMS_3);
            printf("JJJ_DEBUG  WDT%d WDT_COMP_PARAMS_2=0x%x\n", i, pwdt->WDT_COMP_PARAMS_2);
            printf("JJJ_DEBUG  WDT%d WDT_COMP_PARAMS_1=0x%x\n", i, pwdt->WDT_COMP_PARAMS_1);
            printf("JJJ_DEBUG  WDT%d WDT_COMP_VERSION=0x%x\n", i, pwdt->WDT_COMP_VERSION);
            printf("JJJ_DEBUG  WDT%d WDT_COMP_TYPE=0x%x\n", i, pwdt->WDT_COMP_VERSION);
        #endif
    }
}

/*
 * initialize the watchdog timer driver.
 */
void CK_WDT_Init(void)
{
   CK_Deactive_WDTModule();
}

/*
 * open the timer, register the interrupt.
 *
 * wdtid: the number of the corresponding timer port;
 * handler: the interrupt service function of the corresponding timer;
 * bfast: indicate whether the fast interrupt ornot;
 * bopened: indicate the state whether be opened ornot
 */
CK_INT32 CK_WDT_Open ( CKEnum_WDT_Device wdtid,
		         IN void(*handler)(void),
		         IN CK_UINT16 priority,
		         IN BOOL bfast
                       )
{
    PCKStruct_WDTInfo info;
    PCKPStruct_WDT pwdt;
        
    if ((wdtid < 0) || (wdtid > WDTRID_MAX))
    {
        #if CK_WDT_DEBUG
            printf("JJJ_DEBUG  WDT%d Open failure 1\n", wdtid);
        #endif
        return FAILURE;
    }
    
    #if CK_WDT_DEBUG
        printf("JJJ_DEBUG  WDT%d Open\n", wdtid);
    #endif

    info = &(CK_WDT_Table[wdtid]);
    pwdt = info->addr ;
    if(info->bopened)
    { 
        #if CK_WDT_DEBUG
            printf("JJJ_DEBUG  WDT%d Open failure 2\n", wdtid);
        #endif
        return FAILURE;
    }
    /* initialize irqhandler */
    if (NULL != handler)
    {
        info->irqhandler.devname = "WDT";
        info->irqhandler.irqid = info->irq;
        info->irqhandler.priority = priority;
        info->irqhandler.handler = handler;
        info->irqhandler.bfast = bfast;
        info->irqhandler.next = NULL;
        /* register timer isr */
        CK_INTC_RequestIrq(&(info->irqhandler), AUTO_MODE);
    }
    info->bopened = TRUE;
  return SUCCESS;
}

/*
 * close the timer, free interrupt.
 *
 * wdtid: the mumber of the corresponding timer port;
 * bopened: indicate the state whether be opened ornot
 */
CK_INT32 CK_WDT_Close(CKEnum_WDT_Device wdtid)
{
    PCKStruct_WDTInfo info;
	PCKPStruct_WDT pwdt;

    if ((wdtid < 0) || (wdtid > WDTRID_MAX))
        return FAILURE;

    #if CK_WDT_DEBUG
        printf("JJJ_DEBUG  WDT%d Close\n", wdtid);
    #endif
    info = &(CK_WDT_Table[wdtid]);
    pwdt = info->addr;
    if(!(info->bopened))
    {
        return FAILURE;
    }

    /*clear the backcall function*/
    CK_INTC_FreeIrq(&(info->irqhandler), AUTO_MODE);
    info->bopened = FALSE;
    return SUCCESS;
}

/*
 * start the corresponding watchdog timer
 *
 * wdtid: the number of the corresponding timer;
 * mode:    the watchdog timer mode: 
 *              0 - System reset
 *              1 - Interrupt and System reset
 *
 * timeout: the set time (uS)
 */
CK_INT32 CK_WDT_Start(CKEnum_WDT_Device wdtid, IN CK_UINT32 mode, IN CK_UINT32 timeout)
{   
    CK_UINT32 load;
    PCKStruct_WDTInfo info;
    PCKPStruct_WDT pwdt;
    
    if ((wdtid < 0) || (wdtid > WDTRID_MAX))
        return FAILURE;
    
    #if CK_WDT_DEBUG
        printf("JJJ_DEBUG  WDT%d Start model=0x%x, timeout=0x%x\n", wdtid, mode, timeout);
    #endif

    info = &(CK_WDT_Table[wdtid]);
    pwdt = info->addr;
    if(!(info->bopened))
        return FAILURE;
    
    #if CK_WDT_DEBUG
        printf("\nJJJ_DEBUG  CK_WDT_Start 0x00 WDT_CR=0x%x\n", pwdt->WDT_CR);
        printf("\nJJJ_DEBUG  CK_WDT_Start 0x00 WDT_CCVR=0x%x\n", pwdt->WDT_CCVR);
    #endif

    /*  Set Time out range  */
    pwdt->WDT_TORR = timeout;
    /* enable the corresponding timer */
    pwdt->WDT_CR &= ~(CK_WDT_CR_ENABLE | CK_WDT_CR_RMOD);
    pwdt->WDT_CR |= CK_WDT_CR_ENABLE | (mode < 1);

    #if CK_WDT_DEBUG
        printf("\nJJJ_DEBUG  CK_WDT_Start 0x01 WDT_CR=0x%x\n", pwdt->WDT_CR);
        printf("\nJJJ_DEBUG  CK_WDT_Start 0x01 WDT_CCVR=0x%x\n", pwdt->WDT_CCVR);
    #endif

    return SUCCESS;
}

/*
 * stop a designated timer
 *
 * wdtid: the number of the corresponding timer;
 * stop_val: the count value when the timer stops
 */
CK_UINT32 CK_WDT_Stop(CKEnum_WDT_Device wdtid)
{   
    CK_UINT32 stop_val;
    PCKStruct_WDTInfo info;
    PCKPStruct_WDT pwdt;
    
    #if CK_WDT_DEBUG
        printf("JJJ_DEBUG  WDT%d Stop\n", wdtid);
    #endif
    
    /* if the timer does not open,return failure */
    info = &(CK_WDT_Table[wdtid]);
    if(!(info->bopened))
    {
        return FAILURE;
    }
    
    pwdt = info->addr;
    /* disable the WDT*/
    pwdt->WDT_CR &= ~CK_WDT_CR_ENABLE;
    #if CK_WDT_DEBUG
        printf("\nJJJ_DEBUG CK_WDT_Stop 0x01 WDT_CR=0x%x\n", pwdt->WDT_CR);
    #endif
    stop_val = pwdt->WDT_CCVR;
    return stop_val;
}

/* 
 * clear a timer interrupt 
 * by reading its Interrupt Clear Register(EOI)
 */
void  CK_WDT_ClearIrqFlag(CKEnum_WDT_Device wdtid)
{   
    PCKStruct_WDTInfo info;
    PCKPStruct_WDT pwdt;
    
    #if CK_WDT_DEBUG
        printf("JJJ_DEBUG  WDT%d ClearIrqFlag\n", wdtid);
    #endif

    info = &(CK_WDT_Table[wdtid]);
    pwdt = info->addr ;
    *((volatile CK_UINT32 *)(&(pwdt->WDT_EOI)));
}

/*
 * read the current value of the timer
 *
 * wdtid: the mumber of the corresponding timer;
 * current_val: the current count-value 
 */
CK_UINT32 CK_WDT_CurrentValue(CKEnum_WDT_Device wdtid)
{   
    CK_UINT32 current_val;
    PCKStruct_WDTInfo info;
    PCKPStruct_WDT pwdt;
    
    info = &(CK_WDT_Table[wdtid]);
    pwdt = info->addr;
    current_val = pwdt->WDT_CCVR;
    #if CK_WDT_DEBUG
        printf("JJJ_DEBUG  WDT%d CurrentValue = 0x%x\n", wdtid, current_val);
    #endif
    return current_val ;
}

/*
 * restart the counter of the timer
 *
 * wdtid: the mumber of the corresponding timer;
 * current_val: the current count-value 
 */
void CK_WDT_Counter_RST(CKEnum_WDT_Device wdtid) {
    PCKStruct_WDTInfo info;
    PCKPStruct_WDT pwdt;
    
    #if CK_WDT_DEBUG
        printf("JJJ_DEBUG  CK_WDT_Counter_RST\n");
    #endif

    info = &(CK_WDT_Table[wdtid]);
    pwdt = info->addr;
    pwdt->WDT_CRR = COUNTER_RST_CMD; 
}
