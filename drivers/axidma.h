#ifndef _DMAC_H
#define _DMAC_H

#include "ck810.h"
#include "datatype.h"



//#define AHB_DMAC_INTR_ENABLE

#define AHB_DMAC_MAX_CHANNELS 5

//JJJ_DEBUG #define DMAC_BASE_ADDRESS     DMAC_BASE
#define DMAC_BASE_ADDRESS       PCK_AXI_DMA_Control

/*
 * whether to dump the register information on stdout
 */
#define DMA0_VERBOSE 0

#define DMAC_CHx_STAT_SRC     1	//1:set to TRUE   0:set to FALSE
#define DMAC_CHANEL(n) n
#define DMAC_MAX_BLOCK_SIZE         (4096)
#define DMAC_BLOCK_SIZE             (256)
	
//********************************************************************
//Core Configuration
//********************************************************************
#define CC_DMAC_VERSION_ID            0x3231302a
#define CC_DMAC_NUM_MASTER_INT        4
#define CC_DMAC_NUM_CHANNELS          4//
#define CC_DMAC_NUM_HS_INT            2
#define CC_DMAC_ID_NUM                0x0
#define CC_DMAC_INTR_POL              0
#define CC_DMAC_INTR_IO               2
#define CC_DMAC_BIG_ENDIAN            0
#define CC_DMAC_M1_AHB_LITE           0
#define CC_DMAC_M2_AHB_LITE           0
#define CC_DMAC_M3_AHB_LITE           0
#define CC_DMAC_M4_AHB_LITE           0
#define CC_DMAC_M1_HDATA_WIDTH        32
#define CC_DMAC_M2_HDATA_WIDTH        32
#define CC_DMAC_M3_HDATA_WIDTH        32
#define CC_DMAC_M4_HDATA_WIDTH        32
#define CC_DMAC_S_HDATA_WIDTH         32
#define CC_DMAC_MABRST                0
#define CC_DMAC_RETURN_ERR_RESP       1
#define CC_DMAC_ADD_ENCODED_PARAMS       0x1
#define CC_DMAC_CH0_FIFO_DEPTH        16
#define CC_DMAC_CH1_FIFO_DEPTH        16
#define CC_DMAC_CH2_FIFO_DEPTH        16
#define CC_DMAC_CH3_FIFO_DEPTH        16
#define CC_DMAC_CH4_FIFO_DEPTH        16
#define CC_DMAC_CH5_FIFO_DEPTH        16
#define CC_DMAC_CH6_FIFO_DEPTH        16
#define CC_DMAC_CH7_FIFO_DEPTH        16
#define CC_DMAC_CH0_STAT_SRC          0
#define CC_DMAC_CH1_STAT_SRC          0
#define CC_DMAC_CH2_STAT_SRC          0
#define CC_DMAC_CH3_STAT_SRC          0
#define CC_DMAC_CH4_STAT_SRC          0
#define CC_DMAC_CH5_STAT_SRC          0
#define CC_DMAC_CH6_STAT_SRC          0
#define CC_DMAC_CH7_STAT_SRC          0
#define CC_DMAC_CH0_STAT_DST          1
#define CC_DMAC_CH1_STAT_DST          0
#define CC_DMAC_CH2_STAT_DST          0
#define CC_DMAC_CH3_STAT_DST          0
#define CC_DMAC_CH4_STAT_DST          0
#define CC_DMAC_CH5_STAT_DST          0
#define CC_DMAC_CH6_STAT_DST          0
#define CC_DMAC_CH7_STAT_DST          0
#define CC_DMAC_CH0_MAX_MULT_SIZE     8
#define CC_DMAC_CH1_MAX_MULT_SIZE     8
#define CC_DMAC_CH2_MAX_MULT_SIZE     8
#define CC_DMAC_CH3_MAX_MULT_SIZE     8
#define CC_DMAC_CH4_MAX_MULT_SIZE     8
#define CC_DMAC_CH5_MAX_MULT_SIZE     8
#define CC_DMAC_CH6_MAX_MULT_SIZE     8
#define CC_DMAC_CH7_MAX_MULT_SIZE     8
#define CC_DMAC_CH0_MAX_BLK_SIZE 31
#define CC_DMAC_CH1_MAX_BLK_SIZE 31
#define CC_DMAC_CH2_MAX_BLK_SIZE 31
#define CC_DMAC_CH3_MAX_BLK_SIZE 31
#define CC_DMAC_CH4_MAX_BLK_SIZE 31
#define CC_DMAC_CH5_MAX_BLK_SIZE 31
#define CC_DMAC_CH6_MAX_BLK_SIZE 31
#define CC_DMAC_CH7_MAX_BLK_SIZE 31
#define CC_DMAC_CH0_FC                0
#define CC_DMAC_CH1_FC                0
#define CC_DMAC_CH2_FC                0
#define CC_DMAC_CH3_FC                0
#define CC_DMAC_CH4_FC                0
#define CC_DMAC_CH5_FC                0
#define CC_DMAC_CH6_FC                0
#define CC_DMAC_CH7_FC                0
#define CC_DMAC_CH0_LMS         4
#define CC_DMAC_CH1_LMS         4
#define CC_DMAC_CH2_LMS         4
#define CC_DMAC_CH3_LMS         4
#define CC_DMAC_CH4_LMS         4
#define CC_DMAC_CH5_LMS         4
#define CC_DMAC_CH6_LMS         4
#define CC_DMAC_CH7_LMS         4
#define CC_DMAC_CH0_SMS         4
#define CC_DMAC_CH1_SMS         4
#define CC_DMAC_CH2_SMS         4
#define CC_DMAC_CH3_SMS         4
#define CC_DMAC_CH4_SMS         4
#define CC_DMAC_CH5_SMS         4
#define CC_DMAC_CH6_SMS         4
#define CC_DMAC_CH7_SMS         4
#define CC_DMAC_CH0_DMS         4
#define CC_DMAC_CH1_DMS         4
#define CC_DMAC_CH2_DMS         4
#define CC_DMAC_CH3_DMS         4
#define CC_DMAC_CH4_DMS         4
#define CC_DMAC_CH5_DMS         4
#define CC_DMAC_CH6_DMS         4
#define CC_DMAC_CH7_DMS         4
#define CC_DMAC_CH0_LOCK_EN         0
#define CC_DMAC_CH1_LOCK_EN         0
#define CC_DMAC_CH2_LOCK_EN         0
#define CC_DMAC_CH3_LOCK_EN         0
#define CC_DMAC_CH4_LOCK_EN         0
#define CC_DMAC_CH5_LOCK_EN         0
#define CC_DMAC_CH6_LOCK_EN         0
#define CC_DMAC_CH7_LOCK_EN         0
#define CC_DMAC_CH0_STW         32
#define CC_DMAC_CH1_STW         32
#define CC_DMAC_CH2_STW         32
#define CC_DMAC_CH3_STW         32
#define CC_DMAC_CH4_STW         32
#define CC_DMAC_CH5_STW         32
#define CC_DMAC_CH6_STW         32
#define CC_DMAC_CH7_STW         32
#define CC_DMAC_CH0_DTW         32
#define CC_DMAC_CH1_DTW         32
#define CC_DMAC_CH2_DTW         32
#define CC_DMAC_CH3_DTW         32
#define CC_DMAC_CH4_DTW         32
#define CC_DMAC_CH5_DTW         32
#define CC_DMAC_CH6_DTW         32
#define CC_DMAC_CH7_DTW         32
#define CC_DMAC_CH0_SRC_NON_OK         1
#define CC_DMAC_CH1_SRC_NON_OK         1
#define CC_DMAC_CH2_SRC_NON_OK         1
#define CC_DMAC_CH3_SRC_NON_OK         1
#define CC_DMAC_CH4_SRC_NON_OK         1
#define CC_DMAC_CH5_SRC_NON_OK         1
#define CC_DMAC_CH6_SRC_NON_OK         1
#define CC_DMAC_CH7_SRC_NON_OK         1
#define CC_DMAC_CH0_DST_NON_OK         1
#define CC_DMAC_CH1_DST_NON_OK         1
#define CC_DMAC_CH2_DST_NON_OK         1
#define CC_DMAC_CH3_DST_NON_OK         1
#define CC_DMAC_CH4_DST_NON_OK         1
#define CC_DMAC_CH5_DST_NON_OK         1
#define CC_DMAC_CH6_DST_NON_OK         1
#define CC_DMAC_CH7_DST_NON_OK         1
#define CC_DMAC_CH0_LLP_NON_OK         0
#define CC_DMAC_CH1_LLP_NON_OK         0
#define CC_DMAC_CH2_LLP_NON_OK         0
#define CC_DMAC_CH3_LLP_NON_OK         0
#define CC_DMAC_CH4_LLP_NON_OK         0
#define CC_DMAC_CH5_LLP_NON_OK         0
#define CC_DMAC_CH6_LLP_NON_OK         0
#define CC_DMAC_CH7_LLP_NON_OK         0
#define CC_DMAC_CH0_SRC_GAT_EN         0
#define CC_DMAC_CH1_SRC_GAT_EN         0
#define CC_DMAC_CH2_SRC_GAT_EN         0
#define CC_DMAC_CH3_SRC_GAT_EN         0
#define CC_DMAC_CH4_SRC_GAT_EN         0
#define CC_DMAC_CH5_SRC_GAT_EN         0
#define CC_DMAC_CH6_SRC_GAT_EN         0
#define CC_DMAC_CH7_SRC_GAT_EN         0
#define CC_DMAC_CH0_DST_SCA_EN         0
#define CC_DMAC_CH1_DST_SCA_EN         0
#define CC_DMAC_CH2_DST_SCA_EN         0
#define CC_DMAC_CH3_DST_SCA_EN         0
#define CC_DMAC_CH4_DST_SCA_EN         0
#define CC_DMAC_CH5_DST_SCA_EN         0
#define CC_DMAC_CH6_DST_SCA_EN         0
#define CC_DMAC_CH7_DST_SCA_EN         0
#define CC_DMAC_CH0_HC_LLP         1
#define CC_DMAC_CH1_HC_LLP         1
#define CC_DMAC_CH2_HC_LLP         1
#define CC_DMAC_CH3_HC_LLP         1
#define CC_DMAC_CH4_HC_LLP         1
#define CC_DMAC_CH5_HC_LLP         1
#define CC_DMAC_CH6_HC_LLP         1
#define CC_DMAC_CH7_HC_LLP         1
#define CC_DMAC_CH0_MULTI_BLK_EN         1
#define CC_DMAC_CH1_MULTI_BLK_EN         0
#define CC_DMAC_CH2_MULTI_BLK_EN         0
#define CC_DMAC_CH3_MULTI_BLK_EN         0
#define CC_DMAC_CH4_MULTI_BLK_EN         0
#define CC_DMAC_CH5_MULTI_BLK_EN         0
#define CC_DMAC_CH6_MULTI_BLK_EN         0
#define CC_DMAC_CH7_MULTI_BLK_EN         0
#define CC_DMAC_CH0_MULTI_BLK_TYPE         0
#define CC_DMAC_CH1_MULTI_BLK_TYPE         0
#define CC_DMAC_CH2_MULTI_BLK_TYPE         0
#define CC_DMAC_CH3_MULTI_BLK_TYPE         0
#define CC_DMAC_CH4_MULTI_BLK_TYPE         0
#define CC_DMAC_CH5_MULTI_BLK_TYPE         0
#define CC_DMAC_CH6_MULTI_BLK_TYPE         0
#define CC_DMAC_CH7_MULTI_BLK_TYPE         0
#define CC_DMAC_CH0_CTL_WB_EN         0
#define CC_DMAC_CH1_CTL_WB_EN         0
#define CC_DMAC_CH2_CTL_WB_EN         0
#define CC_DMAC_CH3_CTL_WB_EN         0
#define CC_DMAC_CH4_CTL_WB_EN         0
#define CC_DMAC_CH5_CTL_WB_EN         0
#define CC_DMAC_CH6_CTL_WB_EN         0
#define CC_DMAC_CH7_CTL_WB_EN         0
	
//****************************************************************
//Register address
//****************************************************************
#define DMAC_CH_START_ADDR(n)              (DMAC_BASE_ADDRESS + n*0x58)
#define DMAC_COMMON_START_ADDR           (DMAC_BASE_ADDRESS + 0x2c0)
	
#define DMAC_SAR_ADDRESS_OFFSET          0x000
#define DMAC_DAR_ADDRESS_OFFSET          0x008
#define DMAC_LLP_ADDRESS_OFFSET          0x010
#define DMAC_CTL_ADDRESS_OFFSET          0x018
#define DMAC_SSTAT_ADDRESS_OFFSET        0x020
#define DMAC_DSTAT_ADDRESS_OFFSET        0x028
#define DMAC_SSTATAR_ADDRESS_OFFSET      0x030
#define DMAC_DSTATAR_ADDRESS_OFFSET      0x038
#define DMAC_CFG_ADDRESS_OFFSET          0x040
#define DMAC_SGR_ADDRESS_OFFSET          0x048
#define DMAC_DSR_ADDRESS_OFFSET          0x050
	
#define DMAC_SAR(n)      (DMAC_CH_START_ADDR(n) + DMAC_SAR_ADDRESS_OFFSET)
#define DMAC_DAR(n)      (DMAC_CH_START_ADDR(n) + DMAC_DAR_ADDRESS_OFFSET)
#define DMAC_LLP(n)      (DMAC_CH_START_ADDR(n) + DMAC_LLP_ADDRESS_OFFSET)
#define DMAC_CTL(n)      (DMAC_CH_START_ADDR(n) + DMAC_CTL_ADDRESS_OFFSET)
#define DMAC_CTLH(n)     (DMAC_CTL(n) + 4)
#define DMAC_SSTAT(n)    (DMAC_CH_START_ADDR(n) + DMAC_SSTAT_ADDRESS_OFFSET)
#define DMAC_DSTAT(n)    (DMAC_CH_START_ADDR(n) + DMAC_DSTAT_ADDRESS_OFFSET)
#define DMAC_SSTATAR(n)  (DMAC_CH_START_ADDR(n) + DMAC_SSTATAR_ADDRESS_OFFSET)
#define DMAC_DSTATAR(n)  (DMAC_CH_START_ADDR(n) + DMAC_DSTATAR_ADDRESS_OFFSET)
#define DMAC_CFG(n)      (DMAC_CH_START_ADDR(n) + DMAC_CFG_ADDRESS_OFFSET)
#define DMAC_CFGH(n)     (DMAC_CFG(n) + 4)
#define DMAC_SGR(n)      (DMAC_CH_START_ADDR(n) + DMAC_SGR_ADDRESS_OFFSET)
#define DMAC_DSR(n)      (DMAC_CH_START_ADDR(n) + DMAC_DSR_ADDRESS_OFFSET)
	
#define DMAC_RAWTFR            (DMAC_COMMON_START_ADDR + 0x000)
#define DMAC_RAWBLOCK          (DMAC_COMMON_START_ADDR + 0x008)
#define DMAC_RAWSRCTRAN        (DMAC_COMMON_START_ADDR + 0x010)
#define DMAC_RAWDSTTRAN        (DMAC_COMMON_START_ADDR + 0x018)
#define DMAC_RAWERR            (DMAC_COMMON_START_ADDR + 0x020)
#define DMAC_STATUSTFR         (DMAC_COMMON_START_ADDR + 0x028)
#define DMAC_STATUSBLOCK       (DMAC_COMMON_START_ADDR + 0x030)
#define DMAC_STATUSSRCTRAN     (DMAC_COMMON_START_ADDR + 0x038)
#define DMAC_STATUSDSTTRAN     (DMAC_COMMON_START_ADDR + 0x040)
#define DMAC_STATUSERR         (DMAC_COMMON_START_ADDR + 0x048)
#define DMAC_MASKTFR           (DMAC_COMMON_START_ADDR + 0x050)
#define DMAC_MASKBLOCK         (DMAC_COMMON_START_ADDR + 0x058)
#define DMAC_MASKSRCTRAN       (DMAC_COMMON_START_ADDR + 0x060)
#define DMAC_MASKDSTTRAN       (DMAC_COMMON_START_ADDR + 0x068)
#define DMAC_MASKERR           (DMAC_COMMON_START_ADDR + 0x070)
#define DMAC_CLEARTFR          (DMAC_COMMON_START_ADDR + 0x078)
#define DMAC_CLEARBLOCK        (DMAC_COMMON_START_ADDR + 0x080)
#define DMAC_CLEARSRCTRAN      (DMAC_COMMON_START_ADDR + 0x088)
#define DMAC_CLEARDSTTRAN      (DMAC_COMMON_START_ADDR + 0x090)
#define DMAC_CLEARERR          (DMAC_COMMON_START_ADDR + 0x098)
#define DMAC_STATUSINT         (DMAC_COMMON_START_ADDR + 0x0a0)
#define DMAC_REQSRCREG         (DMAC_COMMON_START_ADDR + 0x0a8)
#define DMAC_REQDSTREG         (DMAC_COMMON_START_ADDR + 0x0b0)
#define DMAC_SGLRQSRCREG       (DMAC_COMMON_START_ADDR + 0x0b8)
#define DMAC_SGLRQDSTREG       (DMAC_COMMON_START_ADDR + 0x0c0)
#define DMAC_LSTSRCREG         (DMAC_COMMON_START_ADDR + 0x0c8)
#define DMAC_LSTDSTREG         (DMAC_COMMON_START_ADDR + 0x0d0)
#define DMAC_DMACFGREG         (DMAC_COMMON_START_ADDR + 0x0d8)
#define DMAC_CHENREG           (DMAC_COMMON_START_ADDR + 0x0e0)
#define DMAC_DMAIDREG          (DMAC_COMMON_START_ADDR + 0x0e8)
#define DMAC_DMATESTREG        (DMAC_COMMON_START_ADDR + 0x0f0)
#define DMAC_DMA_COMP_PARAM_6  (DMAC_COMMON_START_ADDR + 0x108)
#define DMAC_DMA_COMP_PARAM_5  (DMAC_COMMON_START_ADDR + 0x110)
#define DMAC_DMA_COMP_PARAM_4  (DMAC_COMMON_START_ADDR + 0x118)
#define DMAC_DMA_COMP_PARAM_3  (DMAC_COMMON_START_ADDR + 0x120)
#define DMAC_DMA_COMP_PARAM_2  (DMAC_COMMON_START_ADDR + 0x128)
#define DMAC_DMA_COMP_PARAM_1  (DMAC_COMMON_START_ADDR + 0x130)
#define DMAC_DMA_COMP_ID       (DMAC_COMMON_START_ADDR + 0x138)
//#define DMAC_PING_1BIT_WR      (DMAC_SAR0)
	
#define DMAC_INTERRUPT_BLOCK 0x01
#define DMAC_INTERRUPT_TFR   0x02
#define DMAC_INTERRUPT_ERROR 0x04
#define DMAC_INTERRUPT_NO    0x00
	
//********************************************************************
//define struction
typedef struct dmac_ch_info 
{
	unsigned int sarx;					//Source address register
	unsigned int darx;					//Destination address register
	unsigned int ctlHx;					//Control register high 32-bit
	unsigned int ctlLx;					//Control register low 32-bit
	unsigned int cfgHx;					//Configuration register high 32-bit
	unsigned int cfgLx;					//Configuration register low 32-bit
	unsigned int sgrx;					//Source gather register
	unsigned int dsrx;					//Destination scatter register
	unsigned int llpx;
} DMAC_CH_INFO;


#if CC_DMAC_CH0_MULTI_BLK_EN
typedef struct dma_list_item 
{
	unsigned int sarx;					//Source address register
	unsigned int darx;					//Destination address register
	unsigned int llpx;
	unsigned int ctlLx;					//Control register
	unsigned int ctlHx;					//Configuration register high 32-bit
#if CC_DMAC_CH0_STAT_DST
	unsigned int sstatx;
	
#endif	/*  */
	 unsigned int dstatx;
} DMA_LIST_ITEM;


#endif	/*  */
//********************************************************************


//********************************************************************
//register bit function define
//********************************************************************
//LLP register.
//********************************************************************
#define DMAC_LLP_MASTER(n)      (n)
//********************************************************************
//SGR/DSR register.
//********************************************************************
#define DMAC_SG_SET(cnt,inc)   ((cnt << 20) | (inc & 0xFFFFF))
//********************************************************************
//Control register.
//********************************************************************
//CTLHx:
#define DMAC_CTL_DONE           (1<<12)
#define DMAC_CTL_BLOCK_TS(n)    (n<<0)
//CTLLx:
#define DMAC_CTL_LLP_SRC_EN     (1L<<28)
#define DMAC_CTL_LLP_DST_EN     (1L<<27)
#define DMAC_CTL_SMS_M1         (0L<<25)
#define DMAC_CTL_SMS_M2         (1L<<25)
#define DMAC_CTL_SMS_M3         (2L<<25)
#define DMAC_CTL_SMS_M4         (3L<<25)
#define DMAC_CTL_DMS_M1         (0L<<23)
#define DMAC_CTL_DMS_M2         (1L<<23)
#define DMAC_CTL_DMS_M3         (2L<<23)
#define DMAC_CTL_DMS_M4         (3L<<23)
#define DMAC_CTL_M2M_DW         (0L<<20)
#define DMAC_CTL_M2P_DW         (1L<<20)
#define DMAC_CTL_P2M_DW         (2L<<20)
#define DMAC_CTL_P2P_DW         (3L<<20)
#define DMAC_CTL_P2M_PER        (4L<<20)
#define DMAC_CTL_P2P_SrcP       (5L<<20)
#define DMAC_CTL_M2P_PER        (6L<<20)
#define DMAC_CTL_P2P_DesP       (7L<<20)
#define DMAC_CTL_DST_SCATTER_EN (1L<<18)
#define DMAC_CTL_SRC_GATHER_EN  (1L<<17)
#define DMAC_CTL_SRC_MSIZE1     (0L<<14)
#define DMAC_CTL_SRC_MSIZE4     (1L<<14)
#define DMAC_CTL_SRC_MSIZE8     (2L<<14)
#define DMAC_CTL_SRC_MSIZE16    (3L<<14)
#define DMAC_CTL_SRC_MSIZE32    (4L<<14)
#define DMAC_CTL_SRC_MSIZE64    (5L<<14)
#define DMAC_CTL_SRC_MSIZE128   (6L<<14)
#define DMAC_CTL_SRC_MSIZE256   (7L<<14)
#define DMAC_CTL_DEST_MSIZE1    (0L<<11)
#define DMAC_CTL_DEST_MSIZE4    (1L<<11)
#define DMAC_CTL_DEST_MSIZE8    (2L<<11)
#define DMAC_CTL_DEST_MSIZE16   (3L<<11)
#define DMAC_CTL_DEST_MSIZE32   (4L<<11)
#define DMAC_CTL_DEST_MSIZE64   (5L<<11)
#define DMAC_CTL_DEST_MSIZE128  (6L<<11)
#define DMAC_CTL_DEST_MSIZE256  (7L<<11)
#define DMAC_CTL_SINC_INC       (0L<<9)
#define DMAC_CTL_SINC_DEC       (1L<<9)
#define DMAC_CTL_SINC_NO        (2L<<9)
#define DMAC_CTL_DINC_INC       (0L<<7)
#define DMAC_CTL_DINC_DEC       (1L<<7)
#define DMAC_CTL_DINC_NO        (2L<<7)
#define DMAC_CTL_SRC_TR_WIDTH8  (0L<<4)
#define DMAC_CTL_SRC_TR_WIDTH16 (1L<<4)
#define DMAC_CTL_SRC_TR_WIDTH32 (2L<<4)
#define DMAC_CTL_DST_TR_WIDTH8  (0L<<1)
#define DMAC_CTL_DST_TR_WIDTH16 (1L<<1)
#define DMAC_CTL_DST_TR_WIDTH32 (2L<<1)
#define DMAC_CTL_INT_EN         (1<<0)
#define DMAC_CTL_INT_DIS        (0<<0)
	
//********************************************************************
//Configuration register
//********************************************************************
//CFGHx:
//Hardware handshake interface for each peripheral.
#define DMAC1_HSNUM_UART_TX(n) (2*n)
#define DMAC1_HSNUM_UART_RX(n) ((2*n)+1)
	
#define DMAC_CFG_DEST_PER(n)      (n<<11)	//Hardware handshake interface
#define DMAC_CFG_SRC_PER(n)       (n<<7)	//Hardware handshake interface
#define DMAC_CFG_SS_UPD_EN        (1<<6)	//Status update enable
#define DMAC_CFG_DS_UPD_EN        (1<<5)	//Status update enable
#define DMAC_CFG_PROTCTL(n)       (n<<2)
#define DMAC_CFG_FIFO_MODE_1      (1<<1)
#define DMAC_CFG_FIFO_MODE_0      (0<<1)
#define DMAC_CFG_FCMODE_1         (1<<0)	//Enable prefetch
#define DMAC_CFG_FCMODE_0         (0<<0)	//Enable prefetch
//CFGLx:
#define DMAC_CFG_RELOAD_DST       (1L<<31)
#define DMAC_CFG_RELOAD_SRC       (1L<<30)
#define DMAC_CFG_SRC_HS_POL_H     (0L<<19)	//Handshake polarity
#define DMAC_CFG_SRC_HS_POL_L     (1L<<19)	//Handshake polarity
#define DMAC_CFG_DST_HS_POL_H     (0L<<18)	//Handshake polarity
#define DMAC_CFG_DST_HS_POL_L     (1L<<18)	//Handshake polarity
#define DMAC_CFG_LOCK_B           (1L<<17)
#define DMAC_CFG_LOCK_CH          (1L<<16)
#define DMAC_CFG_LOCK_B_TSF       (0L<<14)	//Lock level
#define DMAC_CFG_LOCK_B_BLK       (1L<<14)	//Lock level
#define DMAC_CFG_LOCK_B_TSC       (2L<<14)	//Lock level
#define DMAC_CFG_LOCK_CH_TSF      (0L<<12)	//Lock level
#define DMAC_CFG_LOCK_CH_BLK      (1L<<12)	//Lock level
#define DMAC_CFG_LOCK_CH_TSC      (2L<<12)	//Lock level
#define DMAC_CFG_HS_SRC_HARDWARE  (0L<<11)	//Handshake mode
#define DMAC_CFG_HS_SRC_SOFTWARE  (1L<<11)	//Handshake mode
#define DMAC_CFG_HS_DST_HARDWARE  (0L<<10)	//Handshake mode
#define DMAC_CFG_HS_DST_SOFTWARE  (1L<<10)	//Handshake mode
#define DMAC_CFG_FIFO_EMPTY       (1<<9)
#define DMAC_CFG_CH_SUSP          (1<<8)
#define DMAC_CFG_CH_PRIOR(n)      (n<<5)	//n should be 0 to 7
	
//********************************************************************
//Interrupt register
//********************************************************************
//interrupt RAW status registers(0x2c0 to 0x2e0) and interrupt status registers(0x2e8 to 0x308)
#define DMAC_INT_STATUS(n)      (1L<<n)
//interrupt mask register                 //0x310 to 0x330
#define DMAC_INT_MASK(n)      (((0L<<n) | (1L<<(n+8))))
#define DMAC_INT_UNMASK(n)      (((1L<<n) | (1L<<(n+8))))
//interrupt clear register                  //0x338 to 0x358
#define DMAC_INT_CLR(n)       (1L<<n)
//combined interrupt status register            //0x360 
#define DMAC_INT_ERR        (1L<<4)
#define DMAC_INT_DSTT       (1L<<3)
#define DMAC_INT_SRCT       (1L<<2)
#define DMAC_INT_BLOCK      (1L<<1)
#define DMAC_INT_TFR        (1L<<0)
#define DMAC_ALL_MASK       ((1L<<AHB_DMAC_MAX_CHANNELS)-1)
	
//********************************************************************
//Software handshake register
//********************************************************************
#define DMAC_SW_REQ(n)        (((1<<n) | (1<<(n+8))))
#define DMAC_SW_REQ_DIS(n)      (1<<(n+8))
	
//********************************************************************
//DMA channel enable register
//********************************************************************
#define DMAC_CH0            (1L<<0)
#define DMAC_CH1            (1L<<1)
#define DMAC_CH2            (1L<<2)
#define DMAC_CH3            (1L<<3)
#define DMAC_CH4            (1L<<4)
#define DMAC_CH5            (1L<<5)
#define DMAC_CH6            (1L<<6)
#define DMAC_CH_EN(n)       ((1<<n) | (1<<(n+8)))
#define DMAC_CH_DIS(n)        (1<<(n+8))
	
//********************************************************************
//DMA configuration register
//********************************************************************
#define DMAC_EN           (1<<0)
//DMA peripheral_ID
#define peripheral_value            0x80
#define peripheral_uart0_rx         0
#define peripheral_uart0_tx         1
#define peripheral_uart1_rx         2
#define peripheral_uart1_tx         3
#define peripheral_uart2_rx         4
#define peripheral_uart2_tx         5
#define peripheral_spi_master_tx    6
#define peripheral_spi_master_rx	7
#define peripheral_spi_slave_rx		8
#define peripheral_spi_slave_tx		9
#define peripheral_i2c0_tx          10
//#define peripheral_i2c0_rx          10
#define peripheral_i2c1_tx          11
//#define peripheral_i2c1_rx          11
#define peripheral_i2c2_tx          12
//#define peripheral_i2c2_rx          12
#define peripheral_i2c3_tx          13
//#define peripheral_i2c3_rx          13
#define peripheral_pf_monitor       14


//JJJ_DEBUG #define peripheral_nfc			0 
//JJJ_DEBUG #define peripheral_spi_nfc_rx   (0 + peripheral_value )
//JJJ_DEBUG #define peripheral_spi_nfc_tx   (1 + peripheral_value )
//JJJ_DEBUG #define peripheral_spi0_tx		1
//JJJ_DEBUG #define peripheral_spi0_rx		2
//JJJ_DEBUG #define peripheral_spi1_rx		3
//JJJ_DEBUG #define peripheral_spi1_tx		4
//JJJ_DEBUG #define peripheral_spi_m2_rx	5
//JJJ_DEBUG #define peripheral_spi_m2_tx	6
//JJJ_DEBUG #define peripheral_uart1_rx     7
//JJJ_DEBUG #define peripheral_uart1_tx     8
//JJJ_DEBUG #define peripheral_uart2_rx     (7 + peripheral_value )
//JJJ_DEBUG #define peripheral_uart2_tx     (8 + peripheral_value )
//JJJ_DEBUG #define peripheral_uart3_rx     9
//JJJ_DEBUG #define peripheral_uart3_tx     10
//JJJ_DEBUG #define peripheral_uart4_rx     11
//JJJ_DEBUG #define peripheral_uart4_tx     12
//JJJ_DEBUG #define peripheral_uart0_rx     13
//JJJ_DEBUG #define peripheral_uart0_tx     14
//JJJ_DEBUG #define peripheral_i2s0_rx      (13 + peripheral_value )
//JJJ_DEBUG #define peripheral_i2s1         (14 + peripheral_value )
//JJJ_DEBUG #define peripheral_i2s2         15
//JJJ_DEBUG #define peripheral_i2s0_tx      (15 + peripheral_value )






#define   IIS0_TX_DMA_REQUEST              30
#define   UART0_TX_DMA_REQUEST             29
#define   UART0_RX_DMA_REQUEST             27
#define   NFC_DMA_REQUEST                  1







//*******************************************************************
//Miscellaneous DMA register
//********************************************************************
#define DMAC_NORMAL_MODE      (0<<0)

#define	DMAC_BLK_SIZE_256 256
//#define	DMAC_BLK_SIZE_256 48//256
#define	DMAC_BLK_SIZE_128 128
#define	DMAC_BLK_SIZE_64  64
#define	DMAC_BLK_SIZE_512 512
#define Align8 0x01
#define Align16 0x02
#define Align32 0x04



#define CHANNEL_WR   0
#define CHANNEL_RD   1




//function
extern void DMAC_Init (void);

extern void DMAC_Interrupt_en (unsigned int type);

extern void DMAC_Open (DMAC_CH_INFO * channel, unsigned int channel_number, unsigned short BlockSize);

extern void DMAC_Close (unsigned int channel_number);



extern void dmac_isr_handler (void);

extern unsigned int DMAC_CheckDone (unsigned int channel_number);


extern void DMAC_Cfg (DMAC_CH_INFO * channel, unsigned int channel_number, unsigned short BlockSize);

extern void DMAC_Start (unsigned int channel_flag);



void DMAMem2MemOpen(unsigned char channel,unsigned int src_addr,unsigned int des_addr,unsigned int count,unsigned char dma_intr,unsigned char UNIT);

extern void DMAMem2PeripheralOpen(unsigned char channel, unsigned int src_addr, unsigned int count,unsigned char peripheral_ID, unsigned char dma_intr,unsigned int PortNum);

extern void DMAPeripheral2MemOpen(unsigned char channel,unsigned int des_addr,unsigned int count,unsigned char peripheral_ID,unsigned char dma_intr,unsigned int PortNum);

extern volatile unsigned int DMAC_INT_Flag;

void DMAPeripheral2PeripheralOpen(unsigned char channel,unsigned int count,unsigned char PeripheraPassl_ID,unsigned char dma_intr);


void InitMem(unsigned int StartAddr,unsigned int Size);
void FillMemWithConstant(unsigned int StartAddr,unsigned int Size,unsigned int ConstVal);
void CheckMem(unsigned int SourAddr,unsigned int DestAddr,unsigned int Size);




#endif
