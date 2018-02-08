/*****************************************************************************
 *  File: twdt.c
 *  
 *  Descirption: this file contains the WDT test cases.
 *
 *  Copyright (C) : 2018 VeriSilicon.
 * 
 *  Author: Chunming Li
 *  Mail:   shuli_wu@c-sky.com
 *  Date:   Feb 9 2018
 *
 *****************************************************************************/

#include "ck810.h"
#include "intc.h"
#include "wdt.h"
#include "ckwdt.h"
#include "misc.h"

#define CK_WDT_TEST_TIME        100

static CK_UINT8  wdt_test;

static CKStruct_WDTInfo CK_WDT_Table[] =
{
    {0, (PCKPStruct_WDT)CK_WDT_ADDRBASE0, CK_INTC_WDT0, FALSE, 0, NULL},
    //{1, (PCKPStruct_WDT)CK_WDT_ADDRBASE1, CK_INTC_WDT0, FALSE, 0, NULL},
    //{2, (PCKPStruct_WDT)CK_WDT_ADDRBASE2, CK_INTC_WDT0, FALSE, 0, NULL},
};

#define WDTRID_MAX  (sizeof(CK_WDT_Table) / sizeof(CKStruct_WDTInfo))

/*
 * Callback function for timer interrupt, set timer_flag.
 */
void CK_WDT_Handler()
{
    #if CK_WDT_DEBUG
        printf("JJJ_DEBUG  CK_WDT_Handler 0x00\n");
    #endif

    CK_WDT_ClearIrqFlag(0);
    wdt_test = 0;
}

void CK_WDT_Counter_RST_Test() {
    int i;
    CK_UINT32 lastvalue;
    PCKPStruct_WDT pwdt;
    
    printf("  1.	Watchdog Timer Counter Restart Test. . . \n");
    
    for(i = 0; i < WDTRID_MAX; i++) {
        #if CK_WDT_DEBUG
            printf("JJJ_DEBUG  CK_WDT_Counter_RST_Test WDT%d\n", i);
        #endif
        CK_WDT_Open(CK_WDT_Table[i].id,
                    CK_WDT_Handler,
                    CK_WDT_Table[i].irq,
                    FALSE);
        
        CK_WDT_Start(i, CK_WDT_SYSTEM_RST, CK_WDT_TEST_TIME);
        
        delay(1);
        
        lastvalue = CK_WDT_CurrentValue(i);
        CK_WDT_Counter_RST();
        
        if (CK_WDT_CurrentValue(i) > lastvalue)
            printf("                - - - Watchdog Timer %d PASS.\n", i);
        else
            printf("                - - - Watchdog Timer %d FAILURE.\n", i);
        
        CK_WDT_Stop(i);
        CK_WDT_Close(i);
    }
}

void CK_WDT_INTC_RST_Test(CKEnum_WDT_Device wdtid) {
    int reset = 0;
    CK_UINT32 get;
    CK_UINT32 lastvalue;
    PCKPStruct_WDT pwdt;

    printf("  2.	Watchdog Timer Interrupt Mode Test. . . \n");
    
    printf("\n\n\t- - - Reset System?...\n");
    printf("\r\tY: Trigger interrupt and reset system later.\n\t\t");
    printf("\r\tN: Trigger interrupt and restart counter\n\t\t");
    printf("- - - [y/n] ");

    while(1){
        get = CK_WaitForReply();
        if((get == 1) ||(get == 0))
        break;
        else
        printf("\n\tPlease enter 'y' or 'n'   ");
    }
    
    if(get == 1)
        reset == 1;
    else
        reset == 0;;

    wdt_test = 1;

    printf("\n");
    CK_WDT_Open(CK_WDT_Table[wdtid].id,
                CK_WDT_Handler,
                CK_WDT_Table[wdtid].irq,
                FALSE);
    
    CK_WDT_Start(wdtid, CK_WDT_INTC_RST, CK_WDT_TEST_TIME);
    
    printf("                - - - Wait Interrupt Trigger.\n");
    
    while(wdt_test) {
        
    }
    
    if (reset) {
        printf("                - - - Wait System Reset.\n");
        while(1) {
        }    
    } else {
        CK_WDT_Counter_RST();
        printf("                - - - Watchdog Timer %d Interrupt Mode PASS.\n", wdtid);
        CK_WDT_Stop(wdtid);
        CK_WDT_Close(wdtid);
    }
}

void CK_WDT_System_RST_Test(CKEnum_WDT_Device wdtid) {
    CK_UINT32 lastvalue;
    PCKPStruct_WDT pwdt;

    printf("  2.	Watchdog Timer System Reset Mode Test. . . \n");

    CK_WDT_Open(CK_WDT_Table[wdtid].id,
                CK_WDT_Handler,
                CK_WDT_Table[wdtid].irq,
                FALSE);

    CK_WDT_Start(wdtid, CK_WDT_SYSTEM_RST, CK_WDT_TEST_TIME);
    
    printf("                - - - Wait System Reset.\n");
    while(1) {
    }
}

/*
 * main function of watchdog timer test program.
 */
void CK_Watchdog_Test()
{
    CK_UINT32 get;
    printf("\nWatchdog Timer Test. . . \n");
    
    CK_WDT_Counter_RST_Test();
    
    printf("\n\n\t- - - Testing mode...\n");
    printf("\r\tY: Interrupt and Reset Mode; N: System Reset mode\n\t\t");
    
    printf("- - - [y/n] ");
    while(1){
        get = CK_WaitForReply();
        if((get == 1) ||(get == 0))
        break;
        else
        printf("\n\tPlease enter 'y' or 'n'   ");
    }
    
    printf("\n");
    
    if(get == 1)
        CK_WDT_INTC_RST_Test(0);
    else 
        CK_WDT_System_RST_Test(0);
    printf("\n");
}




 
