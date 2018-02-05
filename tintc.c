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

CKStruct_INTCTestInfo INTC_AUTO_MODE_Table[] ALIGN_4 =
{
    {0, 0,  FALSE},
    {1, 63, FALSE},
    {2, 0,  TRUE},
    {3, 63, TRUE}
};

// PCK_INTC: The base address of interrupt controller registers
volatile CKStruct_INTC *icrp_intc = PCK_INTC;
CK_UINT8  intc_test = 1;

/*
 * Callback function for interrupt
 */
void CK_Normal0_Handler()
{
    // Force interrupt clear
    icrp_intc->IFRL &= ~1;
    intc_test = 0;
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Normal0_Handler intc_test=0x%x\n", intc_test);
    #endif    
}

void CK_Normal63_Handler()
{
    // Force interrupt clear
    icrp_intc->IFRH &= ~(1 << 31);
    intc_test = 0;
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Normal63_Handler intc_test=0x%x\n", intc_test);
    #endif  
}

void CK_Fast0_Handler()
{
    // Force interrupt clear
    icrp_intc->IFRL &= ~1;
    intc_test = 0;
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Fast0_Handler intc_test=0x%x\n", intc_test);
    #endif
}

void CK_Fast63_Handler()
{
    // Force interrupt clear
    icrp_intc->IFRH &= ~(1 << 31);
    intc_test = 0;
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Fast63_Handler intc_test=0x%x\n", intc_test);
    #endif
}

void CK_Normal0_Vector_Handler()
{
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Normal0_Vector_Handler 0x00 intc_test=0x%x\n", intc_test);
    #endif
    // Force interrupt clear
    icrp_intc->IFRL &= ~1;
    intc_test = 0;
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Normal0_Vector_Handler 0x01 intc_test=0x%x\n", intc_test);
    #endif
}

void CK_Normal63_Vector_Handler()
{
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Normal63_Vector_Handler 0x00 intc_test=0x%x\n", intc_test);
    #endif
    // Force interrupt clear
    icrp_intc->IFRH &= ~(1 << 31);
    intc_test = 0;
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Normal63_Vector_Handler 0x01 intc_test=0x%x\n", intc_test);
    #endif
}

void CK_Fast0_Vector_Handler()
{
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Fast0_Vector_Handler 0x00 intc_test=0x%x\n", intc_test);
    #endif
    icrp_intc->IFRL &= ~1;
    intc_test = 0;
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Fast0_Vector_Handler 0x01 intc_test=0x%x\n", intc_test);
    #endif
}

void CK_Fast63_Vector_Handler()
{
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Fast63_Vector_Handler 0x00 intc_test=0x%x\n", intc_test);
    #endif
    icrp_intc->IFRH &= ~(1 << 31);
    intc_test = 0;
    #if CK_INTC_DEBUG
        printf("JJJ_DEBUG CK_Fast63_Vector_Handler 0x01 intc_test=0x%x\n", intc_test);
    #endif
}

void CK_INTC_AV_Mode_Test()
{
    PCKStruct_INTCTestInfo info;
    PCKStruct_INTCTestInfo info1;
    PCKStruct_INTCTestInfo info2;
    PCKStruct_INTCTestInfo info3;

    printf("  1.	Auto Vectored Interrupt Mode. . . \n");
    CK_INTC_Init(AUTO_MODE);
    CK_Exception_Init();

    // initialize irqhandler for normal interrupt 0
    printf("            Trigger Normal interrupt 0\n");
    info = &(INTC_AUTO_MODE_Table[0]);
    info->irqhandler.devname = "Normal0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal0_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), AUTO_MODE);
    
    intc_test = 1;
    icrp_intc->IFRL |= 1;
    
    //delay(1);

    if(intc_test == 0)
        printf("                - - - PASS.\n");
    else
        printf("                - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), AUTO_MODE);

    // initialize irqhandler for normal interrupt 63
    printf("            Trigger Normal interrupt 63\n");
    info1 = &(INTC_AUTO_MODE_Table[1]);
    info1->irqhandler.devname = "Normal63";
    info1->irqhandler.irqid = info1->irq;
    info1->irqhandler.priority = info1->irq;
    info1->irqhandler.handler = CK_Normal63_Handler;
    info1->irqhandler.bfast = info1->bfast;
    info1->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info1->irqhandler), AUTO_MODE);
    
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;

    //delay(1);
    
    if(intc_test == 0)
        printf("                - - - PASS.\n");
    else
        printf("                - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info1->irqhandler), AUTO_MODE);

    // initialize irqhandler for fast interrupt 0
    printf("            Trigger Fast interrupt 0\n");
    info2 = &(INTC_AUTO_MODE_Table[2]);
    info2->irqhandler.devname = "Fast0";
    info2->irqhandler.irqid = info2->irq;
    info2->irqhandler.priority = info2->irq;
    info2->irqhandler.handler = CK_Fast0_Handler;
    info2->irqhandler.bfast = info2->bfast;
    info2->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info2->irqhandler), AUTO_MODE);
    
    intc_test = 1;
    icrp_intc->IFRL |= 1;
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info2->irqhandler), AUTO_MODE);

    // initialize irqhandler for fast interrupt 63
    printf("            Trigger Fast interrupt 63\n");
    info3 = &(INTC_AUTO_MODE_Table[3]);
    info3->irqhandler.devname = "Fast63";
    info3->irqhandler.irqid = info3->irq;
    info3->irqhandler.priority = info3->irq;
    info3->irqhandler.handler = CK_Fast63_Handler;
    info3->irqhandler.bfast = info3->bfast;
    info3->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info3->irqhandler), AUTO_MODE);
    
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;
    
    if(intc_test == 0)
        printf("                - - - PASS.\n");
    else
        printf("                - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info3->irqhandler), AUTO_MODE);
    
    printf("    	Auto Vectored Interrupt Mode Done \n");
}

void CK_INTC_SHARE_Mode_Test()
{
    PCKStruct_INTCTestInfo info;
    PCKStruct_INTCTestInfo info1;

    CK_CPU_DisAllNormalIrq();
    CK_CPU_DisAllFastIrq();
    printf("  2.	Share Vectored Interrupt Mode. . . \n");
    CK_INTC_Init(VECTOR_SHARE_MODE);
    CK_Exception_Init();
    
    // initialize irqhandler for normal interrupt 0
    printf("            Trigger Normal interrupt 0\n");
    info = &(INTC_AUTO_MODE_Table[0]);
    info->irqhandler.devname = "Normal0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal0_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_SHARE_MODE);
    
    intc_test = 1;
    icrp_intc->IFRL |= 1;
    
    if(intc_test == 0)
        printf("                - - - PASS.\n");
    else
        printf("                - - - FAILURE.\n");

    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_SHARE_MODE);
    
    // initialize irqhandler for fast interrupt 63
    printf("            Trigger Fast interrupt 63\n");
    info1 = &(INTC_AUTO_MODE_Table[3]);
    info1->irqhandler.devname = "Fast63";
    info1->irqhandler.irqid = info1->irq;
    info1->irqhandler.priority = info1->irq;
    info1->irqhandler.handler = CK_Fast63_Vector_Handler;
    info1->irqhandler.bfast = info1->bfast;
    info1->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info1->irqhandler), VECTOR_SHARE_MODE);
    
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;
    
    if(intc_test == 0)
        printf("                - - - PASS.\n");
    else
        printf("                - - - FAILURE.\n");

    CK_INTC_FreeIrq(&(info1->irqhandler), VECTOR_SHARE_MODE);
    
    printf("    	Share Vectored Interrupt Mode Done \n");
}

void CK_INTC_UNIQUE_Mode_Test()
{
    PCKStruct_INTCTestInfo info;

    CK_CPU_DisAllNormalIrq();
    CK_CPU_DisAllFastIrq();
    printf("  3.	Unique Vectored Interrupt Mode. . . \n");
    CK_INTC_Init(VECTOR_UNIQUE_MODE);
    CK_Exception_Init();
    
    // initialize irqhandler for normal interrupt 0
    printf("        Trigger Normal interrupt 0\n");
    info = &(INTC_AUTO_MODE_Table[0]);
    info->irqhandler.devname = "Normal0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal0_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    
    intc_test = 1;
    icrp_intc->IFRL |= 1;
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");

    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);

    // initialize irqhandler for normal interrupt 63
    printf("        Trigger Normal interrupt 63\n");
    info = &(INTC_AUTO_MODE_Table[1]);
    info->irqhandler.devname = "Normal63";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Normal63_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;

    //delay(1);
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);

    // initialize irqhandler for fast interrupt 0
    printf("        Trigger Fast interrupt 0\n");
    info = &(INTC_AUTO_MODE_Table[2]);
    info->irqhandler.devname = "Fast0";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Fast0_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    
    intc_test = 1;
    icrp_intc->IFRL |= 1;
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);

    // initialize irqhandler for fast interrupt 63
    printf("        Trigger Fast interrupt 63\n");
    info = &(INTC_AUTO_MODE_Table[3]);
    info->irqhandler.devname = "Fast63";
    info->irqhandler.irqid = info->irq;
    info->irqhandler.priority = info->irq;
    info->irqhandler.handler = CK_Fast63_Vector_Handler;
    info->irqhandler.bfast = info->bfast;
    info->irqhandler.next = NULL;
    /* register isr */
    CK_INTC_RequestIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    
    intc_test = 1;
    icrp_intc->IFRH |= 1 << 31;
    
    if(intc_test == 0)
        printf("            - - - PASS.\n");
    else
        printf("            - - - FAILURE.\n");
    CK_INTC_FreeIrq(&(info->irqhandler), VECTOR_UNIQUE_MODE);
    
    printf("    	Unique Vectored Interrupt Mode Done\n");
}
/*
 * main function of timer test program.
 */
void CK_INTC_Test()
{
  printf("\nInterrupt Controller Test. . . \n");
  
  CK_INTC_AV_Mode_Test();
  CK_INTC_SHARE_Mode_Test();
  CK_INTC_UNIQUE_Mode_Test();
  
  printf("\nEnd Interrupt Controller Test. . . \n");
}




 
