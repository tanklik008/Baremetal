/*
 * Filename: tintc.c
 * Description: To test the C-SKY interrupt controller
 * Copyright: VeriSilicon
 * Author: Chunming Li
 * date: Jan 31 2018
 */

#include "ck810.h"
#include "intc.h"
#include "timer.h"
#include "misc.h"
#include "ckintc.h"

static CKStruct_INTCTestInfo INTC_AUTO_MODE_Table[] =
{
    {0, 0,  FALSE},
    {1, 63, FALSE},
    {2, 0,  TRUE},
    {3, 63, TRUE}
};

// PCK_INTC: The base address of interrupt controller registers
volatile CKStruct_INTC *icrp_intc = PCK_INTC;
static CK_UINT8  intc_test = 1;

/*
 * Callback function for interrupt
 */
void CK_Normal0_Handler()
{
  // Force interrupt clear
  icrp_intc->IFRL &= ~1;
  intc_test = 0;
  printf("JJJ_DEBUG CK_Normal0_Handler intc_test=0x%x\n", intc_test);
}

void CK_Normal63_Handler()
{
  // Force interrupt clear
  icrp_intc->IFRH &= ~(1 << 31);
  intc_test = 0;
  printf("JJJ_DEBUG CK_Normal63_Handler intc_test=0x%x\n", intc_test);
}

void CK_Fast0_Handler()
{
  // Force interrupt clear
  icrp_intc->IFRL &= ~1;
  intc_test = 0;
  printf("JJJ_DEBUG CK_Fast0_Handler intc_test=0x%x\n", intc_test);
}

void CK_Fast63_Handler()
{
  // Force interrupt clear
  icrp_intc->IFRH &= ~(1 << 31);
  intc_test = 0;
  printf("JJJ_DEBUG CK_Fast63_Handler intc_test=0x%x\n", intc_test);
}

void CK_Normal0_Vector_Handler()
{
    printf("JJJ_DEBUG CK_Normal0_Vector_Handler 0x00 intc_test=0x%x\n", intc_test);
    IRQ_HANDLER_START();
    // Force interrupt clear
    icrp_intc->IFRL &= ~1;
    intc_test = 0;
    IRQ_HANDLER_END();
    printf("JJJ_DEBUG CK_Normal0_Vector_Handler 0x01 intc_test=0x%x\n", intc_test);
}

void CK_Normal63_Vector_Handler()
{
    printf("JJJ_DEBUG CK_Normal63_Vector_Handler 0x00 intc_test=0x%x\n", intc_test);
    IRQ_HANDLER_START();
    // Force interrupt clear
    icrp_intc->IFRH &= ~(1 << 31);
    intc_test = 0;
    IRQ_HANDLER_END();
    printf("JJJ_DEBUG CK_Normal63_Vector_Handler 0x01 intc_test=0x%x\n", intc_test);
}

void CK_Fast0_Vector_Handler()
{
    printf("JJJ_DEBUG CK_Fast0_Vector_Handler 0x00 intc_test=0x%x\n", intc_test);
    FIQ_HANDLER_START();
    CK_Fast0_Handler();
    FIQ_HANDLER_END();
    printf("JJJ_DEBUG CK_Fast0_Vector_Handler 0x01 intc_test=0x%x\n", intc_test);
}

void CK_Fast63_Vector_Handler()
{
    printf("JJJ_DEBUG CK_Fast63_Vector_Handler 0x00 intc_test=0x%x\n", intc_test);
    FIQ_HANDLER_START();
    CK_Fast63_Handler();
    FIQ_HANDLER_END();
    printf("JJJ_DEBUG CK_Fast63_Vector_Handler 0x01 intc_test=0x%x\n", intc_test);
}

void CK_INTC_AV_Mode_Test()
{
    PCKStruct_INTCTestInfo info;
    volatile int i;
    int time_out;

    printf("  1.	Auto Vectored Interrupt Mode. . . \n");
    CK_INTC_Init(AUTO_MODE);
    CK_Exception_Init();

    //printf("JJJ_DEBUG CK_INTC_AV_Mode_Test ICSR=0x%x\n", icrp_intc->ICR_ISR);

    // initialize irqhandler for normal interrupt 0
    info = &(INTC_AUTO_MODE_Table[0]);
    info->irqhandler.devname = "Normal0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal0_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), AUTO_MODE);
    printf("        Trigger Normal interrupt 0\n");
    intc_test = 1;
    icrp_intc->IFRL |= 1;
    
    //delay(1);

    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), AUTO_MODE);

    // initialize irqhandler for normal interrupt 63
    info = &(INTC_AUTO_MODE_Table[1]);
    info->irqhandler.devname = "Normal63";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal63_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), AUTO_MODE);
    printf("        Trigger Normal interrupt 63\n");
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;

    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), AUTO_MODE);

    // initialize irqhandler for fast interrupt 0
    info = &(INTC_AUTO_MODE_Table[2]);
    info->irqhandler.devname = "Fast0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Fast0_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), AUTO_MODE);
    printf("        Trigger Fast interrupt 0\n");
    intc_test = 1;
    icrp_intc->IFRL |= 1;

    //printf("JJJ_DEBUG fast interrupt 0 intc_test=0x%x\n", intc_test);
    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), AUTO_MODE);

    // initialize irqhandler for fast interrupt 63
    info = &(INTC_AUTO_MODE_Table[3]);
    info->irqhandler.devname = "Fast63";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Fast63_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), AUTO_MODE);
    printf("        Trigger Fast interrupt 63\n");
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;

    //printf("JJJ_DEBUG fast interrupt 63 intc_test=0x%x\n", intc_test);
    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), AUTO_MODE);
}

void CK_INTC_SHARE_Mode_Test()
{
    PCKStruct_INTCTestInfo info;
    unsigned int i;
    unsigned int time_out;

    printf("  2.	Share Vectored Interrupt Mode. . . \n");
    CK_INTC_Init(VECTOR_SHARE_MODE);
    CK_Exception_Init();
    
    // initialize irqhandler for normal interrupt 0
    info = &(INTC_AUTO_MODE_Table[0]);
    info->irqhandler.devname = "Normal0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal0_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    printf("JJJ_DEBUG 0xff RequestIrq\n");
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_SHARE_MODE);
    printf("        Trigger Normal interrupt 0\n");
    intc_test = 1;
    icrp_intc->IFRL |= 1;
    
    //delay(1);
    
    //i = 0;
    //while(icrp_intc->IFRL != 0)
    //{
    //    i += 1;
    //    if (i > 1000)
    //        break;
    //}
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");

    //printf("JJJ_DEBUG 0xff intc_test = 0x%x\n", intc_test);

    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_SHARE_MODE);
    
    // initialize irqhandler for fast interrupt 63
    info = &(INTC_AUTO_MODE_Table[3]);
    info->irqhandler.devname = "Fast63";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Fast63_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_SHARE_MODE);
    printf("        Trigger Fast interrupt 63\n");
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;
    
    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_SHARE_MODE);
}

void CK_INTC_UNIQUE_Mode_Test()
{
    PCKStruct_INTCTestInfo info;
    int i;
    int time_out;

    printf("  3.	Unique Vectored Interrupt Mode. . . \n");
    CK_INTC_Init(VECTOR_UNIQUE_MODE);
    CK_Exception_Init();
    
    // initialize irqhandler for normal interrupt 0
    info = &(INTC_AUTO_MODE_Table[0]);
    info->irqhandler.devname = "Normal0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal0_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    printf("        Trigger Normal interrupt 0\n");
    intc_test = 1;
    icrp_intc->IFRL |= 1;
    
    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    printf("JJJ_DEBUG CK_INTC_UNIQUE_Mode_Test IFRL=0x%x", icrp_intc->IFRL);
    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);

    // initialize irqhandler for normal interrupt 63
    info = &(INTC_AUTO_MODE_Table[1]);
    info->irqhandler.devname = "Normal63";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal63_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    printf("        Trigger Normal interrupt 63\n");
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;

    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);

    // initialize irqhandler for fast interrupt 0
    info = &(INTC_AUTO_MODE_Table[2]);
    info->irqhandler.devname = "Fast0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Fast0_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    printf("        Trigger Fast interrupt 0\n");
    intc_test = 1;
    icrp_intc->IFRL |= 1;

    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), AUTO_MODE);

    // initialize irqhandler for fast interrupt 63
    info = &(INTC_AUTO_MODE_Table[3]);
    info->irqhandler.devname = "Fast63";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Fast63_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    printf("        Trigger Fast interrupt 63\n");
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;

    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
}
/*
 * main function of timer test program.
 */
void CK_INTC_Test()
{
  printf("\nInterrupt Controller Test. . . \n");
  
  CK_INTC_AV_Mode_Test();
  //CK_INTC_SHARE_Mode_Test();
  //CK_INTC_UNIQUE_Mode_Test();
  
  printf("\nEnd Interrupt Controller Test. . . \n");
}




 
