/*
 *  intr.h - Define the struct and interface for interrupt controller
 *
 *  Copyright (C):  2008 Hangzhou C-SKY Microsystems Co.,LTD.
 *  Author: Dongqi Hu  (dongqi_hu@c-sky.com)
 *          Jianyong Jiang  (jianyong_jiang@c-sky.com)
 *  Contributiors: Chunqiang Li  
 *  Date: 2008-09-26
 *  Modify by liu jirang  on 2012-09-11
 */           

#ifndef _INTR_H_
#define _INTR_H_

#include "datatype.h"

//Interrupt mode
#define AUTO_MODE           0
#define VECTOR_SHARE_MODE   1
#define VECTOR_UNIQUE_MODE  2
#define CKCORE_VECTOR_SYS  32
#define CKCORE_VECTOR_AUTOVEC 10
#define CKCORE_VECTOR_FASTAUTOVEC  11
#define CKCORE_VECTOR_TLBMISS 14
#define CKCORE_VECTOR_FASTVEC 96

/* define the data structure of interrupt description */
typedef struct CKS_IRQ_Handler{
      char        *devname;
      CK_UINT32   irqid;
      CK_UINT32    priority;
      void        (*handler)(CK_UINT32 irqid);
      BOOL        bfast;  
      struct CKS_IRQ_Handler  *next;  
}CKStruct_IRQHandler, *PCKStruct_IRQHandler;

typedef struct {
        CK_UINT32 id;               /* the number of INTC test */
        CK_UINT32 irq;              /* the interrupt number of timer */
        BOOL bfast;                 /* fast interrupt or not */
        CKStruct_IRQHandler irqhandler;
} CKStruct_INTCTestInfo, * PCKStruct_INTCTestInfo;

// VSR table
extern  volatile unsigned int ckcpu_vsr_table[128];
/* Statement of those functions which are used in intc.c*/
void CK_CPU_EnAllNormalIrq(void);
void CK_CPU_DisAllNormalIrq(void);
void CK_CPU_EnAllFastIrq(void);
void CK_CPU_DisAllFastIrq(void);
void CK_CPU_EnterCritical(CK_UINT32 *psr);
void CK_CPU_ExitCritical(CK_UINT32 psr);
void CK_INTC_EnNormalIrq(IN CK_UINT32 priority);
void CK_INTC_DisNormalIrq(IN CK_UINT32 priority);
void CK_INTC_EnFastIrq(IN CK_UINT32 priority); 
void CK_INTC_DisFastIrq(IN CK_UINT32 priority);
void CK_INTC_MaskNormalIrq(IN CK_UINT32 primask);
void CK_INTC_UnMaskNormalIrq(IN CK_UINT32 primask);
void CK_INTC_MaskFastIrq(IN CK_UINT32 primask);
void CK_INTC_UnMaskFastIrq(IN CK_UINT32 primask);
CK_INT32 CK_INTC_RequestIrq(PCKStruct_IRQHandler priqhandler, IN CK_UINT32 mode);
CK_INT32 CK_INTC_FreeIrq(INOUT PCKStruct_IRQHandler priqhandler, IN CK_UINT32 mode);


/* save context */
#define IRQ_HANDLER_START()  \
asm (  \
	"subi    sp, 28\n\t"   \
	"stw     a0, (sp, 0)\n\t"   \
	"stw     a1, (sp, 4)\n\t"   \
	"stw     a2, (sp, 8)\n\t"   \
	"stw     a3, (sp, 12)\n\t"   \
	"stw     t0, (sp, 16)\n\t"   \
	"stw     t1, (sp, 20)\n\t"   \
	"stw     lr, (sp, 24)\n\t"   \
\
	"subi    sp, 8\n\t"   \
	"mfcr    a3, epsr\n\t"   \
	"stw     a3, (sp, 4)\n\t"   \
	"mfcr    a2, epc\n\t"   \
	"stw     a2, (sp, 0)\n\t"   \
)

/*   Restore the psr and pc     */
#define IRQ_HANDLER_END()   \
asm (   \
	"ldw     a3, (sp, 0)\n\t"  \
	"mtcr    a3, epc\n\t"  \
	"ldw     a2, (sp, 4)\n\t"   \
	"mtcr    a2, epsr\n\t"   \
	"addi    sp, 8\n\t"   \
\
	"ldw     a0, (sp, 0)\n\t"   \
	"ldw     a1, (sp, 4)\n\t"   \
	"ldw     a2, (sp, 8)\n\t"   \
	"ldw     a3, (sp, 12)\n\t"   \
	"ldw     t0, (sp, 16)\n\t"   \
	"ldw     t1, (sp, 20)\n\t"   \
	"ldw     lr, (sp, 24)\n\t"   \
	"addi    sp, 28\n\t"   \
\
	"rte"   \
)

/*
 * Fast interrupt vector handler.
 */

/* do nothing */
#define FIQ_HANDLER_START()\
asm (  \
	"subi    sp, 28\n\t"   \
	"stw     a0, (sp, 0)\n\t"   \
	"stw     a1, (sp, 4)\n\t"   \
	"stw     a2, (sp, 8)\n\t"   \
	"stw     a3, (sp, 12)\n\t"   \
	"stw     t0, (sp, 16)\n\t"   \
	"stw     t1, (sp, 20)\n\t"   \
	"stw     lr, (sp, 24)\n\t"   \
)

/*   Restore the psr and pc     */
#define FIQ_HANDLER_END()  \
asm (   \
    "ldw     a0, (sp, 0)\n\t"   \
	"ldw     a1, (sp, 4)\n\t"   \
	"ldw     a2, (sp, 8)\n\t"   \
	"ldw     a3, (sp, 12)\n\t"   \
	"ldw     t0, (sp, 16)\n\t"   \
	"ldw     t1, (sp, 20)\n\t"   \
	"ldw     lr, (sp, 24)\n\t"   \
	"addi    sp, 28\n\t"   \
\
	"rfi"   \
)

/*
 * return the irq ID
 */
#define IRQ_ID()  \
({  \
	int  irqid;   \
	asm ( "mfcr		%0, psr\n\t"   \
		  "lsri		%0, 16\n\t"   \
		  "sextb	%0\n\t"   \
		  : "=&r" (irqid)  \
		  :   \
	); \
	irqid; \
})

#endif
