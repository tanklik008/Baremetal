/* ===========================================================================
 *
 * Module     libbspzsp.a
 * File       psp_mcdma.h
 * Company    VeriSilicon, Inc.
 *
 * Abstract   Peripheral Support Package (PSP) API for the ZDMA peripheral.
 *
 * ===========================================================================
 *
 * Dependencies:
 *
 *      All addresses provided to the DMA controller must be AHB addresses,
 *      not ZSP core addresses.
 *
 * ===========================================================================
 * Use of this software is covered by the terms and conditions
 * of the ZSP License Agreement under which it is provided.
 *
 * Copyright (C) 2009 VeriSilicon Inc.  All rights reserved.
 */

/*** NEEDED CHANGES ***
indexes need to be signed long, not unsigned

**********************/

#ifndef PSP_ZDMA_H
#define PSP_ZDMA_H

#include "datatype.h"

//#include "csp_zsp500.h"

// Define the largest BLIM value based on the buffer size and the AHB bus width.
#define ZDMA_CFG2_BLIM_HWMAX ((unsigned long)6<<ZDMA_CFG2_BLIM_BIT)
#define ZDMA_CFG2_AHB_SIZE ZDMA_CFG2_SIZE_64
#define ZDMA_NUM_CHANNELS 2


//-------------------
// MACRO CONSTANTS
//-------------------

#define iZDMA_ChIsIdle(pDmaChanObj)	((pDmaChanObj)->pChanReg->cst.bits.done)
#define iZDMA_ChGetState(pDmaChanObj)	((pDmaChanObj)->pChanReg->cst.bits.state)
#define iZDMA_ChGetError(pDmaChanObj)	((pDmaChanObj)->pChanReg->cst.bits.err)
#define iZDMA_ChGetStatus(pDmaChanObj)	((pDmaChanObj)->pChanReg->cst.lVal)
#define ZDMA_ChWriteCmd(pDmaChanObj, command)  pDmaChanObj->pChanReg->ccr.bits.cmd = command

#define SIZE_OF_AHB_HALFWORD    2
#define SIZE_OF_AHB_WORD        4
#define SIZE_OF_AHB_DWORD       8

// API function Return values
#define ZDMA_STAT_OK				 0	// Function successful
#define ZDMA_STAT_LIST_NOT_ALIGNED		3	// Descriptor list address not
                                                        // aligned to 32-bit word
#define	ZDMA_STAT_SRC_NOT_ALIGNED		2	// A source address in descriptor list
                                                        // is not aligned to transfer size in config
#define	ZDMA_STAT_DST_NOT_ALIGNED		2	// A destination address in descriptor list
                                                        // is not aligned to transfer size in config
#define ZDMA_STAT_INVALID_SIZE			1	// Transfer size is invalid
#define ZDMA_STAT_ENDOFLIST_NOT_FOUND	        5	// Could not find end of descriptor list

/******************************************/
/***** DMA Descriptor Bit Definitions *****/
/******************************************/

//*** Configuration Register 1 (CFG1) ***

#define ZDMA_CFG1_FMT_BIT      0   // Descriptor Format
#define ZDMA_CFG1_SFA_BIT      2   // Source Flag Assignment
#define ZDMA_CFG1_DFA_BIT      8   // Destination Flag Assignment
#define ZDMA_CFG1_DIM_BIT      14  // Number of Dimensions
#define ZDMA_CFG1_BST_BIT      18  // Burst mode enable
#define ZDMA_CFG1_IGEN_BIT     21  // Interrupt Generation
#define ZDMA_CFG1_END_BIT      25  // Endianess

#define ZDMA_CFG1_FMT_MASK     ((unsigned long)0x00000003)
#define ZDMA_CFG1_SFA_MASK     ((unsigned long)0x0000003c)
#define ZDMA_CFG1_DFA_MASK     ((unsigned long)0x00000c00)
#define ZDMA_CFG1_DIM_MASK     ((unsigned long)0x0000c000)
#define ZDMA_CFG1_BST_MASK     ((unsigned long)0x00040000)
#define ZDMA_CFG1_IGEN_MASK    ((unsigned long)0x00e00000)
#define ZDMA_CFG1_END_MASK     ((unsigned long)0x06000000)

#define ZDMA_CFG1_FMT_1        1 // << ZDMA_CFG1_FMT_BIT;

/*
 * ZSP-RS specific peripheral flag connections to the ZDMA
 */
#if ZSP800FPGA
# define ZDMA_MAX_CHANNELS			2	// Nnumber of channels on ZSP_REF_BOARD
#elif ZSP_REF_BOARD
# define ZDMA_MAX_CHANNELS			8	// Nnumber of channels on ZSP_REF_BOARD
#else
# define ZDMA_MAX_CHANNELS		(64)	// Maximum number of channels that can be supported
#endif //ZSP_REF_BOARD

# define ZDMA_FLAG_HPI_HINE		15
# define ZDMA_FLAG_HPI_HONF		14
# define ZDMA_FLAG_TDM1_RXNE		13
# define ZDMA_FLAG_TDM1_TXNF		12
# define ZDMA_FLAG_TDM0_RXNE		11
# define ZDMA_FLAG_TDM0_TXNF		10
# define ZDMA_FLAG_MBX				9
# define ZDMA_FLAG_DFLAG1			2
# define ZDMA_FLAG_DFLAG0			1

# define ZDMA_FLAG_HPI_HINE_BITS	(ZDMA_FLAG_HPI_HINE << ZDMA_CFG1_SFA_BIT)
# define ZDMA_FLAG_HPI_HONF_BITS	(ZDMA_FLAG_HPI_HONF << ZDMA_CFG1_DFA_BIT)
# define ZDMA_FLAG_TDM1_RXNE_BITS	(ZDMA_FLAG_TDM1_RXNE << ZDMA_CFG1_SFA_BIT)
# define ZDMA_FLAG_TDM1_TXNF_BITS	(ZDMA_FLAG_TDM1_TXNF << ZDMA_CFG1_DFA_BIT)
# define ZDMA_FLAG_TDM0_RXNE_BITS	(ZDMA_FLAG_TDM0_RXNE << ZDMA_CFG1_SFA_BIT)
# define ZDMA_FLAG_TDM0_TXNF_BITS	(ZDMA_FLAG_TDM0_TXNF << ZDMA_CFG1_DFA_BIT)
# define ZDMA_FLAG_DFLAG1_SRC_BITS	(ZDMA_FLAG_DFLAG1 << ZDMA_CFG1_SFA_BIT)
# define ZDMA_FLAG_DFLAG1_DST_BITS	(ZDMA_FLAG_DFLAG1 << ZDMA_CFG1_DFA_BIT)
# define ZDMA_FLAG_DFLAG0_SRC_BITS	(ZDMA_FLAG_DFLAG0 << ZDMA_CFG1_SFA_BIT)
# define ZDMA_FLAG_DFLAG0_DST_BITS	(ZDMA_FLAG_DFLAG0 << ZDMA_CFG1_DFA_BIT)
# define ZDMA_FLAG_MBX_SRC_BITS	(ZDMA_FLAG_MBX << ZDMA_CFG1_SFA_BIT)
# define ZDMA_FLAG_MBX_DST_BITS	(ZDMA_FLAG_MBX << ZDMA_CFG1_DFA_BIT)


// DIM field
#define ZDMA_CFG1_DIM_1        ((unsigned long)0<<ZDMA_CFG1_DIM_BIT)     // 1-dimensional
#define ZDMA_CFG1_DIM_2        ((unsigned long)1<<ZDMA_CFG1_DIM_BIT)     // 2-dimensional
#define ZDMA_CFG1_DIM_3        ((unsigned long)2<<ZDMA_CFG1_DIM_BIT)     // 3-dimensional
#define ZDMA_CFG1_DIM_4        ((unsigned long)3<<ZDMA_CFG1_DIM_BIT)     // 4-dimensional

// IGEN field
#define ZDMA_CFG1_IGEN_NONE        ((unsigned long)0<<ZDMA_CFG1_IGEN_BIT)   // Don't generate interrupt
#define ZDMA_CFG1_IGEN_DESCRIPTOR  ((unsigned long)1<<ZDMA_CFG1_IGEN_BIT)   // At end of descriptor
#define ZDMA_CFG1_IGEN_LIST        ((unsigned long)2<<ZDMA_CFG1_IGEN_BIT)   // At end of descriptor if also end of list
#define ZDMA_CFG1_IGEN_D0          ((unsigned long)4<<ZDMA_CFG1_IGEN_BIT)   // End of dimension 0 (inner loop)
#define ZDMA_CFG1_IGEN_D1          ((unsigned long)5<<ZDMA_CFG1_IGEN_BIT)   // End of dimension 1
#define ZDMA_CFG1_IGEN_D2          ((unsigned long)6<<ZDMA_CFG1_IGEN_BIT)   // End of dimension 2
#define ZDMA_CFG1_IGEN_D3          ((unsigned long)7<<ZDMA_CFG1_IGEN_BIT)   // End of dimension 3

//*** Configuration Register 2 (CFG2) ***

#define ZDMA_CFG2_EMOD_BIT         0   // Event synchronization mode
#define ZDMA_CFG2_EGEN_BIT         4   // Event select
#define ZDMA_CFG2_SSIZ_BIT         11  // Source transfer size
#define ZDMA_CFG2_DSIZ_BIT         14  // Destination transfer size
#define ZDMA_CFG2_PCK_BIT          19  // Packing and unpacking modes
#define ZDMA_CFG2_BLIM_BIT         24  // Buffer limit
#define ZDMA_CFG2_BSTS_BIT         28  // Burst mode source enable
#define ZDMA_CFG2_BSTD_BIT         29  // Burst mode destination enable

#define ZDMA_CFG2_EMOD_MASK        ((unsigned long)0x00000007)
#define ZDMA_CFG2_EGEN_MASK        ((unsigned long)0x000007f0)
#define ZDMA_CFG2_SSIZ_MASK        ((unsigned long)0x00003800)
#define ZDMA_CFG2_DSIZ_MASK        ((unsigned long)0x0001c000)
#define ZDMA_CFG2_PCK_MASK         ((unsigned long)0x00780000)
#define ZDMA_CFG2_BLIM_MASK        ((unsigned long)0x0F000000)
#define ZDMA_CFG2_BSTS_MASK        ((unsigned long)0x10000000)
#define ZDMA_CFG2_BSTD_MASK        ((unsigned long)0x20000000)

// EMOD field
#define ZDMA_CFG2_EMOD_NONE        ((unsigned long)0<<ZDMA_CFG2_EMOD_BIT)   // No event synchronization
#define ZDMA_CFG2_EMOD_DESCRIPTOR  ((unsigned long)1<<ZDMA_CFG2_EMOD_BIT)   // Descriptor level sync
#define ZDMA_CFG2_EMOD_D0          ((unsigned long)4<<ZDMA_CFG2_EMOD_BIT)   // Dimension 0 sync (inner loop)
#define ZDMA_CFG2_EMOD_D1          ((unsigned long)5<<ZDMA_CFG2_EMOD_BIT)   // Dimension 1 sync
#define ZDMA_CFG2_EMOD_D2          ((unsigned long)6<<ZDMA_CFG2_EMOD_BIT)   // Dimension 2 sync
#define ZDMA_CFG2_EMOD_D3          ((unsigned long)7<<ZDMA_CFG2_EMOD_BIT)   // Dimension 3 sync

// EGEN field
#define ZDMA_CFG2_EGEN_DMA(channel)    (((channel)&ZDMA_CFG2_EGEN_MASK)<<ZDMA_CFG2_EGEN_BIT)
#define ZDMA_CFG2_EGEN_XINT(int_id)    (((int_id+0x64)&ZDMA_CFG2_EGEN_MASK)<<ZDMA_CFG2_EGEN_BIT)

#define ZDMA_CFG2_SIZE_8           0
#define ZDMA_CFG2_SIZE_16          1
#define ZDMA_CFG2_SIZE_32          2
#define ZDMA_CFG2_SIZE_64          3
#define ZDMA_CFG2_SIZE_128         4
#define ZDMA_CFG2_SIZE_256         5
#define ZDMA_CFG2_SIZE_512         6

// SSIZ field
#define ZDMA_CFG2_SSIZ_8           ((unsigned long)ZDMA_CFG2_SIZE_8   << ZDMA_CFG2_SSIZ_BIT)
#define ZDMA_CFG2_SSIZ_16          ((unsigned long)ZDMA_CFG2_SIZE_16  << ZDMA_CFG2_SSIZ_BIT)
#define ZDMA_CFG2_SSIZ_32          ((unsigned long)ZDMA_CFG2_SIZE_32  << ZDMA_CFG2_SSIZ_BIT)
#define ZDMA_CFG2_SSIZ_64          ((unsigned long)ZDMA_CFG2_SIZE_64  << ZDMA_CFG2_SSIZ_BIT)
#define ZDMA_CFG2_SSIZ_128         ((unsigned long)ZDMA_CFG2_SIZE_128 << ZDMA_CFG2_SSIZ_BIT)
#define ZDMA_CFG2_SSIZ_256         ((unsigned long)ZDMA_CFG2_SIZE_256 << ZDMA_CFG2_SSIZ_BIT)

// DSIZ field
#define ZDMA_CFG2_DSIZ_8           ((unsigned long)ZDMA_CFG2_SIZE_8   << ZDMA_CFG2_DSIZ_BIT)
#define ZDMA_CFG2_DSIZ_16          ((unsigned long)ZDMA_CFG2_SIZE_16  << ZDMA_CFG2_DSIZ_BIT)
#define ZDMA_CFG2_DSIZ_32          ((unsigned long)ZDMA_CFG2_SIZE_32  << ZDMA_CFG2_DSIZ_BIT)
#define ZDMA_CFG2_DSIZ_64          ((unsigned long)ZDMA_CFG2_SIZE_64  << ZDMA_CFG2_DSIZ_BIT)
#define ZDMA_CFG2_DSIZ_128         ((unsigned long)ZDMA_CFG2_SIZE_128 << ZDMA_CFG2_DSIZ_BIT)
#define ZDMA_CFG2_DSIZ_256         ((unsigned long)ZDMA_CFG2_SIZE_256 << ZDMA_CFG2_DSIZ_BIT)

// PCK field
#define ZDMA_CFG2_PCK_NONE         ((unsigned long)0<<ZDMA_CFG2_PCK_BIT)   // No packing or unpacking
#define ZDMA_CFG2_PCK_8PU16        ((unsigned long)1<<ZDMA_CFG2_PCK_BIT)   // 8pu-to-16
#define ZDMA_CFG2_PCK_8PS16        ((unsigned long)2<<ZDMA_CFG2_PCK_BIT)   // 8ps-to-16
#define ZDMA_CFG2_PCK_16TO8        ((unsigned long)3<<ZDMA_CFG2_PCK_BIT)   // 16-to-8
#define ZDMA_CFG2_PCK_DUPUPBYTE    ((unsigned long)4<<ZDMA_CFG2_PCK_BIT)   // Dup-upper-byte
#define ZDMA_CFG2_PCK_DUPLOBYTE    ((unsigned long)5<<ZDMA_CFG2_PCK_BIT)   // Dup-lower-byte
#define ZDMA_CFG2_PCK_SGLLO8PU16   ((unsigned long)6<<ZDMA_CFG2_PCK_BIT)   // single-lower-8pu-to-16
#define ZDMA_CFG2_PCK_SGLUP8PU16   ((unsigned long)7<<ZDMA_CFG2_PCK_BIT)   // single-upper-8pu-to-16
#define ZDMA_CFG2_PCK_SGLLO8PS16   ((unsigned long)8<<ZDMA_CFG2_PCK_BIT)   // single-lower-8ps-to-16
#define ZDMA_CFG2_PCK_SGLUP8PS16   ((unsigned long)9<<ZDMA_CFG2_PCK_BIT)   // single-upper-8ps-to-16

// BLIM field
#define ZDMA_CFG2_BLIM_00	   ((unsigned long)0L)                       // Pick best burst size.
#define ZDMA_CFG2_BLIM_ONE	   ((unsigned long)1L<<ZDMA_CFG2_BLIM_BIT)   // to divide BLIM by 2 subtract this
#define ZDMA_CFG2_BLIM_16	   ((unsigned long)4L<<ZDMA_CFG2_BLIM_BIT)   // Burst transfer 16 bytes
#define ZDMA_CFG2_BLIM_32	   ((unsigned long)5L<<ZDMA_CFG2_BLIM_BIT)   // Burst transfer 32 bytes
#define ZDMA_CFG2_BLIM_64	   ((unsigned long)6L<<ZDMA_CFG2_BLIM_BIT)   // Burst transfer 64 bytes
#define ZDMA_CFG2_BLIM_128	   ((unsigned long)7L<<ZDMA_CFG2_BLIM_BIT)   // Burst transfer 128 bytes
#define ZDMA_CFG2_BLIM_MAX         ((unsigned long)15L<<ZDMA_CFG2_BLIM_BIT)  // Max value - computed based on the burst size.
//#define ZDMA_CFG2_BLIM_HWMAX     // Value determined by bus width and buffer size.

// Burst enable - necessary when using blim values.
#define ZDMA_CFG2_BSTS		   ((unsigned long)1<<ZDMA_CFG2_BSTS_BIT)   // Burst mode source enable
#define ZDMA_CFG2_BSTD		   ((unsigned long)1<<ZDMA_CFG2_BSTD_BIT)   // Burst mode destination enable

/******************************************/
/*****  DMA Register Bit Definitions  *****/
/******************************************/

//*** Channel Control Register ***

#define ZDMA_CCR_SR_BIT        0   // Soft Reset
#define ZDMA_CCR_CMD_BIT       1   // Command
#define ZDMA_CCR_BPCT          4   // Breakpoint control
#define ZDMA_CCR_PRI           8   // Priority

#define ZDMA_CCR_SR_MASK       ((unsigned long)0x00000001)
#define ZDMA_CCR_CMD_MASK      ((unsigned long)0x0000000e)
#define ZDMA_CCR_BPCT_MASK     ((unsigned long)0x00000030)
#define ZDMA_CCR_PRI_MASK      ((unsigned long)0x00000700)

// CMD field - adjust to enable direct use.
# define ZDMA_CCR_CMD_NONE      0 << 1
# define ZDMA_CCR_CMD_START     1 << 1
# define ZDMA_CCR_CMD_PAUSE     2 << 1
# define ZDMA_CCR_CMD_CONTINUE  3 << 1
# define ZDMA_CCR_CMD_STOP_IMM  4 << 1
# define ZDMA_CCR_CMD_STOP_DSCR 5 << 1

//*** Channel Status Register ***
#define ZDMA_CST_STAT_BIT      0
#define ZDMA_CST_DONE_BIT      4
#define ZDMA_CST_FULL_BIT      5
#define ZDMA_CST_ERROR_BIT     6
#define ZDMA_CST_INT_BIT       10
#define ZDMA_CST_ILLEGAL_BIT   11

#define ZDMA_CST_STAT_MASK     ((unsigned long)0x00000003)
#define ZDMA_CST_DONE_MASK     ((unsigned long)0x00000010)
#define ZDMA_CST_FULL_MASK     ((unsigned long)0x00000020)
#define ZDMA_CST_ERROR_MASK    ((unsigned long)0x000001c0)
#define ZDMA_CST_INT_MASK      ((unsigned long)0x00000400)
#define ZDMA_CST_ILLEGAL_MASK  ((unsigned long)0x00000800)

// STAT field
#define ZDMA_CST_STAT_IDLE     0
#define ZDMA_CST_STAT_BUSY     1
#define ZDMA_CST_STAT_WAITEVT  2
#define ZDMA_CST_STAT_TRFR     3
#define ZDMA_CST_STAT_REQDSCR  5

//*** Next Descriptor List Address ***
#define ZDMA_NDL_AHB_ALIGNMENT_MASK	((CK_UINT32)0x00000003)
#define ZDMA_NDL_ALIGNMENT_MASK		((CK_UINT32)0x00000001)


#ifndef ASM_CODE

//-------------------
// MACROS
//-------------------
// Invalidate Data cache
#define ZDMA_FLUSH_DCACHE()      bitinvert_creg(%smode, DCT_BIT)
#define ZDMA_FLUSH_ICACHE()      bitinvert_creg(%smode, ICT_BIT)

//-------------------
// TYPE DEFINITIONS
//-------------------

typedef struct ZDMA_Descriptor_tag {
  union {
    unsigned long   lVal;
    struct {
      unsigned long fmt  :2;	// 1:0 descriptor format
      unsigned long sfa  :4;	// 5:2 source flag assignment
      unsigned long res1 :2;	// 6:7 reserved
      unsigned long dfa  :4;	// 11:8 destination flag assignment
      unsigned long res2 :2;	// 13:12 reserved
      unsigned long dim  :2;	// 15:14 number of dimensions
      unsigned long res3 :5;	// 20:16 reserved
      unsigned long igen :3;	// 23:21 Interrupt generation
      unsigned long erm  :4; // 27:24 Error Mode
      unsigned long eri  :4;	// 31:28 Error Input			
    };
    } cfg1;

  union {
    unsigned long   lVal;
    struct {
      unsigned long emod :3;	// 2:0 Event synchronization mode
      unsigned long res1 :1;	// 3:3 reserved
      unsigned long egen :7;	// 10:4 Event to generate
      unsigned long ssiz :3;	// 13:11 source transfer size
      unsigned long dsiz :3;	// 16:14 destination transfer size
      unsigned long res2 :2;	// 18:17 reserved
      unsigned long pck  :4;	// 22:19 Packing and Unpacking
      unsigned long res3 :1;	// 23:23 reserved
      unsigned long blim :4;	// 27:24 Buffer limit
      unsigned long bsts :1;	// 28 burst mode source enable
      unsigned long bstd :1;	// 29 burst mode destination enable
      unsigned long res4 :2;    // 31:30 reserved
    };
    } cfg2;

  unsigned long   ahbSrc;		// AHB address of source data
  unsigned long   ahbDst;		// AHB address of destination data
  unsigned long   ahbLink;	// AHB address of next descriptor
  unsigned long   Loop0SrcCnt;
           long   Loop0SrcIdx;
           long   Loop0DstIdx;
  unsigned long   Loop1SrcCnt;
           long   Loop1SrcIdx;
           long   Loop1DstIdx;
  unsigned long   Loop2SrcCnt;
           long   Loop2SrcIdx;
           long   Loop2DstIdx;
  unsigned long   Loop3SrcCnt;
           long   Loop3SrcIdx;
           long   Loop3DstIdx;
  unsigned long   filler;		//this structure needs to be quad word aligned
} ZDMA_Descriptor_Type;

typedef struct {
  union { // Channel Status register
    volatile unsigned long	lVal;
    struct {
      volatile unsigned long state:3;	// 2:0 State
      volatile unsigned long res1:1;	// 3 reserved
      volatile unsigned long done:1;	// 4 Done
      volatile unsigned long res2:1;	// 5 reserved
      volatile unsigned long err:3;	// 8:6 Error Indication
      volatile unsigned long res3:2;	// 10:9 reserved
      volatile unsigned long ill:1;	// 11 Illegal
      volatile unsigned long res4:20;   // 31:12 reserved
    }bits;
  };
} ZDMA_ChannelStatus_Type;

typedef struct  {
  union { // Channel Control register
    volatile unsigned long	lVal;
    struct {
      volatile unsigned long sr:1;	// 0 soft reset
      volatile unsigned long cmd:3;	// 3:1 command
      volatile unsigned long bpct:2;	// 5:4 Breakpoint control
      volatile unsigned long res1:2;	// 6:7 reserved
      volatile unsigned long pri:3;	// 10:8 Priority
      volatile unsigned long bpri:2;	// 12:11 Bus priority
      volatile unsigned long res2:3;	// 15:13 reserved
      volatile unsigned long esel:7;	// 22:16 Event select
      volatile unsigned long res3:9;    // 31:23 reserved
    }bits;
  }ccr;								// Channel Control Register
  ZDMA_ChannelStatus_Type cst;
  //union { // Channel Status register
  //  volatile unsigned long	lVal;
  //  struct {
  //    volatile unsigned state:3;	// 2:0 State
  //    volatile unsigned res1:1;	// 3 reserved
  //    volatile unsigned done:1;	// 4 Done
  //    volatile unsigned res2:1;	// 5 reserved
  //    volatile unsigned err:3;	// 8:6 Error Indication
  //    volatile unsigned res3:2;	// 10:9 reserved
  //    volatile unsigned ill:1;	// 11 Illegal
  //    // 31:12 reserved
  //  }bits;
  //}cst;								// Channel Status Register
  volatile unsigned long      ndl;	// AHB address of Next Descriptor List
  volatile unsigned long      src;	// AHB address of Source data
  volatile unsigned long      dst;	// AHB address of Destination data
  volatile unsigned long      cdb;	// AHB address of Current Descriptor Base
  unsigned long      			dummy[10];//channel structure is 0x20 word long
} ZDMA_RegPerChannel_Type;

// All address registers refer to AHB address, not core address,
typedef struct {
    volatile unsigned long      intrl;      // Interrupt Request Low
    volatile unsigned long      intrh;      // Interrupt Request High
    volatile unsigned long      intsl;      // Interrupt Set Low
    volatile unsigned long      intsh;      // Interrupt Set High
    volatile unsigned long      intcl;      // Interrupt Clear Low
    volatile unsigned long      intch;      // Interrupt Clear High
    volatile unsigned long      intml;      // Interrupt Mask Low
    volatile unsigned long      intmh;      // Interrupt Mask High
    volatile unsigned long      evtrl;      // Event Request Low
    volatile unsigned long      evtrh;      // Event Request High
    volatile unsigned long      evtsl;      // Event Set Low
    volatile unsigned long      evtsh;      // Event Set High
    volatile unsigned long      evtcl;      // Event Clear Low
    volatile unsigned long      evtch;      // Event Clear High
    volatile unsigned long      acsl;		// Active Channel Status Low
    volatile unsigned long      acsh;		// Active Channel Status high
    volatile unsigned long      errrl;      // Error Request Low
    volatile unsigned long      errrh;      // Error Request High
    volatile unsigned long      errsl;      // Error Set Low
    volatile unsigned long      errsh;      // Error Set High
    volatile unsigned long      errcl;      // Error Clear Low
    volatile unsigned long      errch;      // Error Clear High
    volatile unsigned long      errml;      // Error Mask Low
    volatile unsigned long      errmh;      // Error Mask High
    volatile unsigned long      dummy[8];	// reserved space
    ZDMA_RegPerChannel_Type  channel[ZDMA_MAX_CHANNELS];	//channel descriptors start at 0x40
} ZDMA_Reg_Type, *PZDMA_Reg_Type;

typedef struct {
    struct mcdma_cntr_object_struct       *pDmaControllerObj; // Address of controller data object
    unsigned short              Channel;            // Channel number
} ZDMA_Channel_Cfg_Type;

typedef struct mcdma_chan_obj_tag{
    ZDMA_Descriptor_Type       DefaultDescriptor;  // Single descriptor list to provide easy API for simple
                                                    // memory copy.  Must be 32-bit aligned.
    ZDMA_Reg_Type              *pSharedReg;        // Base address of the DMA registers in ZSP500 space
    ZDMA_RegPerChannel_Type    *pChanReg;          // Base address of the channel specific DMA registers
    unsigned short              Channel;            // Channel number.  Needed to access shared register bits
    void                   (*CallbackFunction)(struct mcdma_chan_obj_tag *a, void *b);   // User function to call when DMA completes (NULL if not used)
    void                        *pCallbackData;     // User supplied pointer to data that will be passed to callback routine
    short                       alignment_filler;
    long                        alignment_filler2;
} ZDMA_Chan_Obj_Type;       // Must be 32-bit aligned

/*JJJ_DEBUG
typedef struct {
    PZDMA_Reg_Type  pDmaBaseAddr;  // Base address of the DMA registers in ZSP500 internal memory space.
                                    // Code does not switch to external memory space, but takes advantage
                                    // that non-populated internal memory access goes to external space.
    unsigned short  NumberChannels;
} ZDMA_Controller_Cfg_Type;
<<JJJ_DEBUG*/

typedef struct {
    ZDMA_Reg_Type  *pDmaBaseAddr;  // Base address of the DMA registers in ZSP500 internal memory space.
                                    // Code does not switch to external memory space, but takes advantage
                                    // that non-populated internal memory access goes to external space.
    unsigned short  NumberChannels;
} ZDMA_Controller_Cfg_Type;

typedef struct mcdma_cntr_object_struct {
    ZDMA_Reg_Type          *pDmaBaseAddr;      // Address of the DMA registers
    unsigned short          NumberChannels;     // Number of DMA channels configured for controller
    ZDMA_Chan_Obj_Type     *pDmaChanObj[ZDMA_MAX_CHANNELS];   // Pointers to DMA objects for maximum possible channels
} ZDMA_Controller_Obj_Type;

 // Callback functions that will be called by the DMA channel
typedef  void (*ZDMA_Isr_Callback_Type)(ZDMA_Chan_Obj_Type *pDmaChanObj, void *pData);

extern ZDMA_Controller_Obj_Type ZDMA_Obj;

#endif	// ASM_CODE

void ZDMA_ChReset(ZDMA_Chan_Obj_Type *pDmaChanObj);
void ZDMA_Controller_Init(ZDMA_Controller_Cfg_Type *pDmaCtrlCfg);
void ZDMA_Get_Controller_Addr(ZDMA_Controller_Obj_Type **pDmaCtrlObj);
void ZDMA_ChInit(ZDMA_Chan_Obj_Type *pDmaChanObj, ZDMA_Channel_Cfg_Type *pDmaChanCfg);
int ZDMA_ChWriteDescriptor(ZDMA_Chan_Obj_Type *pDmaChanObj, ZDMA_Descriptor_Type *pDescriptor, 
                           int maxCnt);
int ZDMA_ChNeedsService(ZDMA_Chan_Obj_Type *pDmaChanObj);
void ZDMA_ChClearService(ZDMA_Chan_Obj_Type *pDmaChanObj);
void ZDMA_ChWriteControl(ZDMA_Chan_Obj_Type *pDmaChanObj, unsigned int command);
int ZDMA_CopyOut(ZDMA_Chan_Obj_Type *pDmaChanObj, void *pIntSrc, void *pAHBDst, 
                 unsigned long length, unsigned long blim);
int ZDMA_2D_CopyOut(
                 ZDMA_Chan_Obj_Type *pDmaChanObj,
                 void *pIntSrc,
                 void *pAHBDst,
                 unsigned long length,
                 unsigned long blim,

                 unsigned long loop1_count,
                 unsigned long loop1_src_stride,
                 unsigned long loop1_dst_stride);
int ZDMA_CopyIn(ZDMA_Chan_Obj_Type *pDmaChanObj, void *pAHBSrc, void *pIntDst, 
                unsigned long length, unsigned long blim);
int ZDMA_2D_CopyIn(ZDMA_Chan_Obj_Type *pDmaChanObj, void *pAHBSrc, void *pIntDst, 
                unsigned long length, unsigned long blim,

                 unsigned long loop1_count,
                 unsigned long loop1_src_stride,
                 unsigned long loop1_dst_stride);
int ZDMA_WaitChDone(ZDMA_Chan_Obj_Type *pDmaChanObj);

int ZDMA_memcpy_nowait(ZDMA_Chan_Obj_Type *pDmaChanObj, void *destn, const void *src, CK_UINT32 n);
int ZDMA_memcpy_intr(void *destn, const void *src, CK_UINT32 n);
int ZDMA_memcpy_poll(void *destn, const void *src, CK_UINT32 n);
void setup_zdma_memcpy(ZDMA_Controller_Cfg_Type *DmaCtrlCfg, ZDMA_Channel_Cfg_Type *DmaChanCfg );




#endif	//PSP_ZDMA_H
