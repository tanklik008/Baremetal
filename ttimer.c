/*
 * Filename: ttimer.c
 * Description: To test the timer driver program
 * Copyright: Hangzhou C-Sky Microsystem Co., Ltd.
 * Author: Yun Ye (yun_ye@c-sky.com)
 * date: Dec 27 2008
 */

#include "ck810.h"
#include "intc.h"
#include "timer.h"
#include "misc.h"

//JJJ_DEBUG>>
#include "ckintc.h" //JJJ_DEBUG
// PCK_INTC: The base address of interrupt controller registers
volatile CKStruct_INTC *icrp_timer = PCK_INTC;
//<<JJJ_DEBUG

//JJJ_DEBUG #define CK_TIMER_TEST_TIME       1000000  /* 2s */
#define CK_TIMER_TEST_TIME       100  /* 2s */
#define TIMERID_MAX  1

CK_UINT8  timer_test;
CK_UINT32 t_timerid;


/*
 * Callback function for timer interrupt, set timer_flag.
 */
void CK_TIMER_Handler(CK_UINT32 irqid);

// static void CK_TIMER_Handler()
// {
  // printf("JJJ_DEBUG  CK_TIMER_Handler 0x00\n");
  // IRQ_HANDLER_START();
  // printf("JJJ_DEBUG  CK_TIMER_Handler 0x01\n");
  // CK_TIMER_Handler_Real(IRQ_ID());
  // printf("JJJ_DEBUG  CK_TIMER_Handler 0x02\n");
  // IRQ_HANDLER_END();
  // printf("JJJ_DEBUG  CK_TIMER_Handler 0x03\n");
// }

void CK_TIMER_Handler(CK_UINT32 irqid)
{
  printf("JJJ_DEBUG  CK_TIMER_Handler 0x00\n");
  CK_Timer_ClearIrqFlag(t_timerid);
  //JJJ_DEBUG>>
  // Force Time0 interrupt clear
  //icrp_timer->IFRL = 0;
  printf("\nJJJ_DEBUG  CK_Timer_Test IFRL0x%x\n", icrp_timer->IFRL);
  //<<JJJ_DEBUG
  timer_test = 0;
}

/*
 * main function of timer test program.
 */
void CK_Timer_Test()
{
  CK_UINT32 currentvalue;
  CK_UINT32 lastvalue;
  CK_UINT32 irq;
  CK_UINT32 i;

  printf("\nDemo: Timer Interrupt. . . \n");
  for(i = 0; i < TIMERID_MAX; i++)
  {
    t_timerid = i;
    printf("\tTesting Timer%d ", t_timerid);
    timer_test = 1;
    if(i <= 1)
    {
      irq = i + 8;
    }
    else
    { 
      irq = i + 21;
    }
    if (i < 2)
    {
      CK_Timer_Open(t_timerid,  /* open timer0 & timer1 as normal interrupt */
                    CK_TIMER_Handler,
                    irq,
                    FALSE);
    }
    else
    {
      CK_Timer_Open(t_timerid,  /* open timer0 & timer1 as fast interrupt */
                    CK_TIMER_Handler,
                    i,
                    TRUE);
    }
    CK_Timer_Start(t_timerid, CK_TIMER_TEST_TIME);
    currentvalue = CK_Timer_CurrentValue(t_timerid);
    /* if currentvalue > lastvalue, it means that it's time out
        but there is no interrupt generate. */
    
    //JJJ_DEBUG>>
    // Force Time0 interrupt trigger
    //icrp_timer->IFRL |= 1 << 27;
    //printf("\nJJJ_DEBUG  CK_Timer_Test IFRL=0x%x\n", icrp_timer->IFRL);
    //<<JJJ_DEBUG
    
    while(timer_test)
    {
      printf("\nJJJ_DEBUG  CK_Timer_Test timer_test=0x%x\n", timer_test);
      lastvalue = currentvalue;
      currentvalue = CK_Timer_CurrentValue(t_timerid);
      printf("\nJJJ_DEBUG  CK_Timer_Test lastvalue=0x%x, currentvalue=0x%x\n", lastvalue, currentvalue);
      if(currentvalue > lastvalue)
      {
        break;
      }
    }
    CK_Timer_Stop(t_timerid);
    CK_Timer_Close(t_timerid); 
    if(timer_test == 0)
        printf("- - - PASS.\n");
    else
        printf("- - - FAILURE.\n");
  }
}




 
