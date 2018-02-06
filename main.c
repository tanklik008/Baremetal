/*
 * main.c - main function Modulation.
 *
 * Copyright (C): 2008~2010 Hangzhou C-SKY Microsystem Co.,LTD.
 * Author: Liu Bing  (bing_liu@c-sky.com)
 * Contributor: Liu Bing
 * Date: 2010-6-28
 *
 */


#include "datatype.h"
#include "ck810.h"
#include "timer.h"
#include "uart.h"
#include "misc.h"
#include "intc.h";

CK_Uart_Device consoleuart = CONFIG_TERMINAL_UART;
extern void CK_Exception_Init (void);
extern void CK_INTC_Init(IN CK_UINT32 mode);
extern void CK_UART_Test();
extern void CK_Timer_Test();

static void CK_Console_CallBack(CK_INT8 error)
{   
  if(error==CK_Uart_CTRL_C)
  {
    CK_UART_ClearRxBuffer(consoleuart);
  }
} 

static void CK_Console_Init()
{
  CK_Uart_Open(consoleuart,CK_Console_CallBack);
}

/* 
 * initialize the device registered 
 */
static void CK_Drivers_Init(void)
{
	CK_Uart_DriverInit();
#ifndef CONFIG_INTC_DIS  
    CK_INTC_Init(AUTO_MODE);
#endif
}

/* 
 * the main function of Uart demo project
 */
int main ( void )
{
    CK_Drivers_Init();
#ifndef CONFIG_INTC_DIS      
    CK_Exception_Init();
#endif    
    CK_Console_Init();	

    printf ("\n");
    {
	    /*
	     * Call the uart test case.
         */
    	//CK_UART_Test();
		/*
	     * Call the interrupt controller test case.
         */
        #if CK_TIMER_TEST
            CK_Timer_Init();
            CK_Timer_Test();
        #endif
        
        #if CK_AHB_DMA_TEST
            CK_AHBDMA_Test();
        #endif
        
        #if CK_INTC_TEST
            // Test will reconfigure the INTC, so it should be the last test
            CK_INTC_Test();
        #endif
    } 

    return 0x00;
}


