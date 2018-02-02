/*
 * Description: ckintc.h - Define the structure and state the interface for 
 * interrupt controller.
 * 
 * Copyright (C) : 2008 Hangzhou C-SKY Microsystems Co.,LTD.
 * Author(s): Jianyong Jiang (jianyong_jiang@c-sky.com)
              Dongqi Hu  (dongqi_hu@c-sky.com)
 * Contributors: Chunqiang Li
 * Date:  2008-09-26
 */

#ifndef _CKINTC_H_
#define _CKINTC_H_

#include "ck810.h"

/* define the registers structure of the interrupt controller */
typedef struct CKS_INTC
{
  CK_REG    ICR_ISR;//0x00
  CK_REG    Rev0;	//0x04
  CK_REG    IFRL;	//0x08
  CK_REG    IPRL;	//0x0c
  CK_REG    NIERL;	//0x10
  CK_REG    NIPRL;	//0x14
  CK_REG    FIERL;	//0x18
  CK_REG    FIPRL;	//0x1c
  CK_REG    IFRH;	//0x20
  CK_REG    IPRH;	//0x24
  CK_REG    NIERH;	//0x28
  CK_REG    NIPRH;	//0x2c
  CK_REG    FIERH;	//0x30
  CK_REG    FIPRH;	//0x34
  CK_REG    Rev[2];	//0x38 - 0x3c
  CK_REG    PR[16];	//0x40 - 0x7f
}CKStruct_INTC, *PCKStruct_INTC;

 
#define PCK_INTC    ((PCKStruct_INTC)CK_INTC_BASEADDRESS)


/*
 *  Bit Definition for the PIC Interrupt control register
 */
#define ICR_AVE   0x80000000  /* Select vectored interrupt */
#define ICR_FVE   0x40000000  /* Unique vector number for fast vectored*/
#define ICR_ME    0x20000000  /* Interrupt masking enabled */
#define	ICR_MFI	  0x10000000  /* Fast interrupt requests masked by MASK value */


#endif























