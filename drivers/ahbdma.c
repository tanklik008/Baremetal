
#include "ck810.h"
#include "intc.h"
#include "ahbdma.h"
#include "datatype.h"

#include "uart.h"
#include "ckuart.h"
//#include "spi.h"
//#include "config.h"
//#include "nand.h"
//#include "gd25q128.h"
//#include "iis.h"
//#include "w25n01.h"

#define UART0_THR (CK_UART_ADDR0 + 0x00)
#define MEM2MEM_TEST_L	        0x1000
#define MEM2UART0_TEST_L        0x24
#define MEM_TEST_SRC            0x20000000
#define MEM_TEST_DEST           0x20004000
// Channle 0 and 1
#define DMA_CH01_MAX_BLK_SIZE   127
// Channel 2 - 7
#define DMA_CH27_MAX_BLK_SIZE   63
#define DMA_CHANNEL_0           0
#define DMA_CHANNEL_1           1
#define DMA_CHANNEL_2           2

//********************************************************************
//  Punsigned charlic data
//********************************************************************
volatile unsigned int DMAC_INT_Flag;

static CK_UINT32 test_mem_data[] = {
    0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff
};

static CK_UINT8 test_char_data[] = {
	'V','e','r','i','S','i','l','i','c','o','n', '\n'
};

//****************************************************************
//DMAC initial
//****************************************************************
void DMAC_Init (void)
{
	
	//enable DMAC    
    write_mreg32 (DMAC_DMACFGREG, DMAC_EN);//D8+2C0
	//enable DMAC to normal mode
	write_mreg32 (DMAC_DMATESTREG, DMAC_NORMAL_MODE);//F0
	
	//clear software request
	write_mreg32 (DMAC_REQSRCREG, (DMAC_ALL_MASK << 8));//A8-368
	write_mreg32 (DMAC_REQDSTREG, (DMAC_ALL_MASK << 8));//B0--370
	write_mreg32 (DMAC_SGLRQSRCREG, (DMAC_ALL_MASK << 8));//B8--378
	write_mreg32 (DMAC_SGLRQDSTREG, (DMAC_ALL_MASK << 8));//C0--380
	write_mreg32 (DMAC_LSTSRCREG, (DMAC_ALL_MASK << 8));//C8--388
	write_mreg32 (DMAC_LSTDSTREG, (DMAC_ALL_MASK << 8));//D8--398
	
	//disable DMAC channel   
	write_mreg32 (DMAC_CHENREG, (DMAC_ALL_MASK << 8));//E0--3A0



#ifdef	AHB_DMAC_INTR_ENABLE
    Intc_EnableIntSr(DMA_INTR_NUM);
	DMAC_INT_Flag=0;
	DMAC_Interrupt_en (DMAC_INTERRUPT_BLOCK);
#endif
	
} 

//****************************************************************
// interrupt enable
//****************************************************************
void DMAC_Interrupt_en (unsigned int type)
{
	unsigned int tmp32;
	
	//clear DMAC interrupt
	write_mreg32 (DMAC_CLEARTFR, DMAC_ALL_MASK);
	write_mreg32 (DMAC_CLEARBLOCK, DMAC_ALL_MASK);
	write_mreg32 (DMAC_CLEARSRCTRAN, DMAC_ALL_MASK);
	write_mreg32 (DMAC_CLEARDSTTRAN, DMAC_ALL_MASK);
	write_mreg32 (DMAC_CLEARERR, DMAC_ALL_MASK);
	
	//check DMAC interrupt status
	tmp32 = (read_mreg32 (DMAC_STATUSINT));
	if (tmp32 != 0)

	{
		printf ("DMAC initial fail!\r\n");
	}
	write_mreg32 (DMAC_MASKTFR, (DMAC_ALL_MASK << 8));	//disable interrupt
	write_mreg32 (DMAC_MASKBLOCK, (DMAC_ALL_MASK << 8));	//disable block interrupt
	write_mreg32 (DMAC_MASKSRCTRAN, (DMAC_ALL_MASK << 8));	//disable interrupt
	write_mreg32 (DMAC_MASKDSTTRAN, (DMAC_ALL_MASK << 8));	//disable interrupt
	write_mreg32 (DMAC_MASKERR, (DMAC_ALL_MASK << 8));	//disable error interrupt
	if (type & DMAC_INTERRUPT_BLOCK)

	{
		write_mreg32 (DMAC_MASKBLOCK, (DMAC_ALL_MASK | DMAC_ALL_MASK << 8));
	}
	if (type & DMAC_INTERRUPT_TFR)

	{
		write_mreg32 (DMAC_MASKTFR, (DMAC_ALL_MASK | DMAC_ALL_MASK << 8));
	}
	if (type & DMAC_INTERRUPT_ERROR)

	{
		write_mreg32 (DMAC_MASKERR, (DMAC_ALL_MASK | DMAC_ALL_MASK << 8));
	}
}


//****************************************************************
//DMAC open
//channel number should be from 0 to 7
//BlockSize should be from 1 to 1023
//****************************************************************
void DMAC_RUN()
{
	write_mreg32 (DMAC_CHENREG, 0xf0f);
}
void DMAC_Open (DMAC_CH_INFO * channel, unsigned int channel_number, unsigned short BlockSize)
{


    #ifdef	AHB_DMAC_INTR_ENABLE
	DMAC_INT_Flag &= (~(1<<channel_number));
	DMAC_Interrupt_en (DMAC_INTERRUPT_BLOCK);
    #endif
	DMAC_Cfg (channel, channel_number, BlockSize);
} 


void DMAC_Cfg (DMAC_CH_INFO * channel, unsigned int channel_number, unsigned short BlockSize)
{
	unsigned int tmp32;
	//check if the channel exist
 	if (channel_number > CC_DMAC_NUM_CHANNELS)
 	{
 	   printf ("DMAC channel don't exist!\r\n");
 	}
 
 	if (BlockSize > DMAC_MAX_BLOCK_SIZE)
	 {
   		 printf ("DMA Block Size too large!\r\n");
 	}

 	while (1)
 	{
    	tmp32 = (read_mreg32 (DMAC_CHENREG));
    	if (tmp32 & (1 << channel_number))
    	{
 	      printf ("DMAC is used!\r\n");
    	}
    	else
    	{
       	break;
    	}
 	}
   	write_mreg32 (DMAC_SAR(channel_number), channel->sarx);   
   	write_mreg32 (DMAC_DAR(channel_number), channel->darx);   
   	write_mreg32 (DMAC_CTLH(channel_number), (unsigned int )(BlockSize));   
   	write_mreg32 (DMAC_CTL(channel_number), channel->ctlLx);   
   	write_mreg32 (DMAC_CFGH(channel_number), channel->cfgHx);   
   	write_mreg32 (DMAC_CFG(channel_number), channel->cfgLx);   
   	//
   	if (channel->ctlLx & DMAC_CTL_SRC_GATHER_EN)
      write_mreg32 (DMAC_SGR(channel_number), channel->sgrx);
   	if (channel->ctlLx & DMAC_CTL_DST_SCATTER_EN)
      write_mreg32 (DMAC_DSR(channel_number), channel->dsrx);
   	write_mreg32 (DMAC_LLP(channel_number), channel->llpx);   

	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_SAR(channel_number),read_mreg32 (DMAC_SAR(channel_number)));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_DAR(channel_number),read_mreg32 (DMAC_DAR(channel_number)));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_CTLH(channel_number),read_mreg32 (DMAC_CTLH(channel_number)));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_CTL(channel_number),read_mreg32 (DMAC_CTL(channel_number)));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_CFGH(channel_number),read_mreg32 (DMAC_CFGH(channel_number)));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_CFG(channel_number),read_mreg32 (DMAC_CFG(channel_number)));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_SGR(channel_number),read_mreg32 (DMAC_SGR(channel_number)));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_DSR(channel_number),read_mreg32 (DMAC_DSR(channel_number)));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_LLP(channel_number),read_mreg32 (DMAC_LLP(channel_number)));
}

void DMAC_Start (unsigned int channel_number)
{
	write_mreg32 (DMAC_CHENREG, DMAC_CH_EN(channel_number));
	printf(" read_mreg32 (0x%x)  = 0x%x\r\n ",DMAC_CHENREG,read_mreg32 (DMAC_CHENREG));
}

//****************************************************************
//Read DMA done information
//****************************************************************
unsigned int DMAC_CheckDone (unsigned int channel_number)
{
	unsigned int temp; 
#ifdef	AHB_DMAC_INTR_ENABLE

	temp=(DMAC_INT_Flag & (1<<channel_number));
	if(temp)
	{		
		DMAC_INT_Flag &= ~(1<<channel_number);
		return	1;
	}
	else
	{
		return	0;
	}
#else
	temp = (read_mreg32 (DMAC_RAWTFR));
	temp &= (1 << channel_number);
	if (temp){
		write_mreg32 (DMAC_CLEARTFR, temp);
		return 1;
	}
	else
		return 0;
#endif

	
}

//****************************************************************
//DMAC close
//****************************************************************
void DMAC_Close (unsigned int channel_number)
{

	//clear software request
	write_mreg32 (DMAC_REQSRCREG, DMAC_SW_REQ_DIS (channel_number));
	write_mreg32 (DMAC_REQDSTREG, DMAC_SW_REQ_DIS (channel_number));
	write_mreg32 (DMAC_SGLRQSRCREG, DMAC_SW_REQ_DIS (channel_number));
	write_mreg32 (DMAC_SGLRQDSTREG, DMAC_SW_REQ_DIS (channel_number));
	write_mreg32 (DMAC_LSTSRCREG, DMAC_SW_REQ_DIS (channel_number));
	write_mreg32 (DMAC_LSTDSTREG, DMAC_SW_REQ_DIS (channel_number));	
		//enable DMAC    
		//write_mreg32 (DMAC_DMACFGREG, DMAC_EN);		
	write_mreg32 (DMAC_DMACFGREG, 0);	
		//disable DMAC channel
	write_mreg32 (DMAC_CHENREG, DMAC_CH_DIS (channel_number));
		//clear DMAC interrupt
	write_mreg32 (DMAC_CLEARTFR, DMAC_INT_CLR (channel_number));
	write_mreg32 (DMAC_CLEARBLOCK, DMAC_INT_CLR (channel_number));
	write_mreg32 (DMAC_CLEARSRCTRAN, DMAC_INT_CLR (channel_number));
	write_mreg32 (DMAC_CLEARDSTTRAN, DMAC_INT_CLR (channel_number));
	write_mreg32 (DMAC_CLEARERR, DMAC_INT_CLR (channel_number));	
		//disable DMAC interrupt
	write_mreg32 (DMAC_MASKTFR, DMAC_INT_MASK (channel_number));
	write_mreg32 (DMAC_MASKBLOCK, DMAC_INT_MASK (channel_number));
	write_mreg32 (DMAC_MASKSRCTRAN, DMAC_INT_MASK (channel_number));
	write_mreg32 (DMAC_MASKDSTTRAN, DMAC_INT_MASK (channel_number));
	write_mreg32 (DMAC_MASKERR, DMAC_INT_MASK (channel_number));
} 

 

//****************************************************************
//DMAC interrupt handler
//****************************************************************
void dmac_isr_handler (void)
{
	unsigned int tmp32;
	//read block interrupt status,then clear interrupt
	tmp32 = (read_mreg32 (DMAC_STATUSTFR));	
	write_mreg32 (DMAC_CLEARTFR, tmp32);
	tmp32 = (read_mreg32 (DMAC_STATUSBLOCK));
	DMAC_INT_Flag |= (tmp32 & DMAC_ALL_MASK);
	printf("  <<<<<<<<<<<< DMA_IRQHandler  >>>>>>>>>>>>>>>>  DMAC_INT_Flag  = 0x%x  \r\n",DMAC_INT_Flag);
	write_mreg32 (DMAC_CLEARBLOCK, tmp32);
	
		//read error interrupt status,then clear interrupt
	tmp32 = (read_mreg32 (DMAC_STATUSERR));

	//clrar error interrupt
	write_mreg32 (DMAC_CLEARERR, tmp32);
	if (tmp32 != 0)
	{
		printf ("DMAC error occur\r\n");
	}
	printf("DMA interrupt!\r\n");

}

void DMAMem2MemOpen(unsigned char channel,unsigned int src_addr,unsigned int des_addr,unsigned int count,unsigned char dma_intr,unsigned char UNIT)
{

	DMAC_CH_INFO  channel_info;
	channel_info.sarx = src_addr;   			
	channel_info.darx = des_addr;    		
	channel_info.ctlHx = count;
	#ifdef	AHB_DMAC_INTR_ENABLE
		dma_intr=1;
	#else
		dma_intr=0;
	#endif
	switch (UNIT)
	{
			case Align8:
				channel_info.ctlLx = (DMAC_CTL_M2M_DW|DMAC_CTL_SRC_MSIZE4 |DMAC_CTL_DEST_MSIZE4 |DMAC_CTL_SINC_INC |DMAC_CTL_DINC_INC |DMAC_CTL_SRC_TR_WIDTH8 |DMAC_CTL_DST_TR_WIDTH8|dma_intr);//|			
				break;
			case Align16:
				channel_info.ctlLx = (DMAC_CTL_M2M_DW|DMAC_CTL_SRC_MSIZE4 |DMAC_CTL_DEST_MSIZE4 |DMAC_CTL_SINC_INC |DMAC_CTL_DINC_INC |DMAC_CTL_SRC_TR_WIDTH16 |DMAC_CTL_DST_TR_WIDTH16|dma_intr);//|			
				break;
			case Align32:
				channel_info.ctlLx = (DMAC_CTL_M2M_DW|DMAC_CTL_SRC_MSIZE4 |DMAC_CTL_DEST_MSIZE4 |DMAC_CTL_SINC_INC |DMAC_CTL_DINC_INC |DMAC_CTL_SRC_TR_WIDTH32 |DMAC_CTL_DST_TR_WIDTH32|dma_intr);//|Msize = 4 is for aes			
				break;
	}
	channel_info.cfgLx =  DMAC_CFG_HS_SRC_SOFTWARE|DMAC_CFG_HS_DST_SOFTWARE|DMAC_CFG_SRC_HS_POL_H|DMAC_CFG_DST_HS_POL_H;
	//Source handshake mode
	//Destination handshake mode 
	//Source handshake polarity
	//Destination handshake polarity
	DMAC_Open(&channel_info,channel,count);
}
/*
	channel: DMA channel number
	src_addr: source address
	count:   32bit word length for transfering, the max count is 4095
	dma_intr: 0 disable dma interrupt, 1 enable dma interrupt	
*/
void DMAMem2PeripheralOpen(CK_UINT8 channel, CK_UINT32 src_addr, CK_UINT32 count,CK_UINT8 peripheral_ID, CK_UINT8 dma_intr,CK_UINT32 PortNum)
{
	DMAC_CH_INFO  channel_info;
	#ifdef	AHB_DMAC_INTR_ENABLE
		dma_intr=1;
	#else
		dma_intr=0;
	#endif
    
    switch(peripheral_ID)
	{		
		case peripheral_uart0_tx:
			channel_info.sarx =  (CK_UINT32)src_addr;		
			channel_info.darx =  (CK_UINT32)UART0_THR;
			channel_info.ctlHx = count;			
			channel_info.ctlLx = DMAC_CTL_M2P_DW | DMAC_CTL_SRC_MSIZE1 | 
								 DMAC_CTL_DEST_MSIZE1 | DMAC_CTL_SINC_INC | 
								 DMAC_CTL_DINC_NO | DMAC_CTL_SRC_TR_WIDTH8 | 
								 DMAC_CTL_DST_TR_WIDTH8 | dma_intr;		
			channel_info.cfgLx = DMAC_CFG_CH_PRIOR(0)|DMAC_CFG_HS_SRC_SOFTWARE|DMAC_CFG_HS_DST_HARDWARE;		
			channel_info.cfgHx = DMAC_CFG_DEST_PER(peripheral_uart0_tx);
			break;
			
		case peripheral_spi_master_tx:
			channel_info.sarx =  (CK_UINT32)src_addr;		
			//JJJ_DEBUGchannel_info.darx =  (CK_UINT32)SPI_TXREG;	//JJJ_DEBUG
			channel_info.ctlHx = count;			
			channel_info.ctlLx = DMAC_CTL_M2P_DW | DMAC_CTL_SRC_MSIZE4 | 
								 DMAC_CTL_DEST_MSIZE4 | DMAC_CTL_SINC_INC | 
								 DMAC_CTL_DINC_NO | DMAC_CTL_SRC_TR_WIDTH8 | 
								 DMAC_CTL_DST_TR_WIDTH8 | dma_intr;		
			channel_info.cfgLx = DMAC_CFG_CH_PRIOR(0)|DMAC_CFG_HS_SRC_SOFTWARE|DMAC_CFG_HS_DST_HARDWARE;		
			channel_info.cfgHx = DMAC_CFG_DEST_PER(peripheral_spi_master_tx);
			break;
	}
	channel_info.sgrx = 0x0;
	channel_info.dsrx = 0x0;
	channel_info.llpx = 0x0;
	DMAC_Open(&channel_info,channel,count);
} 

/*
	channel: DMA channel number
	des_addr: destination address
	count:   32bit word length for transfering, the max count is 4095
	dma_intr: 0 disable dma interrupt, 1 enable dma interrupt	
*/
void DMAPeripheral2MemOpen(CK_UINT8 channel,CK_UINT32 des_addr,CK_UINT32 count,CK_UINT8 peripheral_ID,CK_UINT8 dma_intr,CK_UINT32 PortNum)
{
	DMAC_CH_INFO  channel_info;
	#ifdef	AHB_DMAC_INTR_ENABLE
		dma_intr=1;
	#else
		dma_intr=0;
	#endif

	switch(peripheral_ID)
	{	
		 case peripheral_uart0_rx:
			channel_info.sarx =  (CK_UINT32)UART0_THR;
			channel_info.darx =  (CK_UINT32)des_addr;		
			channel_info.ctlHx = count;			
			channel_info.ctlLx = DMAC_CTL_P2M_DW | DMAC_CTL_SRC_MSIZE4 | 
								 DMAC_CTL_DEST_MSIZE4 | DMAC_CTL_SINC_NO | 
								 DMAC_CTL_DINC_INC | DMAC_CTL_SRC_TR_WIDTH8 | 
								 DMAC_CTL_DST_TR_WIDTH8 | dma_intr;		
			channel_info.cfgLx = DMAC_CFG_CH_PRIOR(0)|DMAC_CFG_HS_SRC_HARDWARE|DMAC_CFG_HS_DST_SOFTWARE;		
			channel_info.cfgHx = DMAC_CFG_SRC_PER(peripheral_uart0_rx);
	
		    break;
	
		case peripheral_spi_master_rx :
			//JJJ_DEBUGchannel_info.sarx =  (CK_UINT32)SPI_RXREG;		
			channel_info.darx =  (CK_UINT32)des_addr;		
			channel_info.ctlHx = count;			
			channel_info.ctlLx = DMAC_CTL_P2M_DW | DMAC_CTL_SRC_MSIZE4 | 
								 DMAC_CTL_DEST_MSIZE4 | DMAC_CTL_SINC_NO | 
								 DMAC_CTL_DINC_INC | DMAC_CTL_SRC_TR_WIDTH8 | 
								 DMAC_CTL_DST_TR_WIDTH8 | dma_intr;		
			channel_info.cfgLx = DMAC_CFG_CH_PRIOR(0)|DMAC_CFG_HS_SRC_HARDWARE|DMAC_CFG_HS_DST_SOFTWARE;		
			channel_info.cfgHx = DMAC_CFG_SRC_PER(peripheral_spi_master_rx);
			break;
	}
    
	channel_info.sgrx = 0x0;
	channel_info.dsrx = 0x0;
	channel_info.llpx = 0x0;
	DMAC_Open(&channel_info,channel,count);	
} 






void CK_AHBDMA_MEM2UART0_Test(void)
{
    int loop, i;

    printf("  2.	Memory to UART0 Transfer. . . \n");

    // Init test memory area
    for(loop = 0;loop < MEM2UART0_TEST_L; loop += 12) {
        for (i = 0; i < 12; i++)
            write_mreg8((MEM_TEST_SRC + loop + i), test_char_data[i]);
    }

    // Set UART0 to DMA mode 0
    CK_Uart_Set_DMA_Mode(0, CK_UART_DMA_MODE0);
    // Configure channel 0 to transfer data
    DMAC_Init();
    DMAMem2PeripheralOpen(DMA_CHANNEL_0, MEM_TEST_SRC, MEM2UART0_TEST_L, peripheral_uart1_tx,  1, 0);
    DMAC_Start(DMA_CHANNEL_0); 
    while(!(DMAC_CheckDone(DMA_CHANNEL_0)));
    DMAC_Close(DMA_CHANNEL_0); 
}

void CK_AHBDMA_UART02MEM_Test(void)
{
    // Set UART0 to DMA mode 1
    CK_Uart_Set_DMA_Mode(0, CK_UART_DMA_MODE1);

    printf("  3.	UART0 to Memory Transfer. . . \n");
	DMAC_Init();
    DMAPeripheral2MemOpen(CHANNEL_RD, 0xF0006000, 0x30*6, peripheral_uart1_rx,  1, 0);
    DMAC_Start(CHANNEL_RD); 
    while(!(DMAC_CheckDone(CHANNEL_RD)));
    DMAC_Close(CHANNEL_RD); 
}

void CK_AHBDMA_UART_Test(void)
{
	CK_AHBDMA_MEM2UART0_Test();
	CK_AHBDMA_UART02MEM_Test();
}


void CK_AHBDMA_MEM2MEM_Test() {
    unsigned int data_flag,rest_val,loop,SRC,DEST;
    int i;
    
	printf("  1.	Memory to Memory Transfer. . . \n");
    
    // Init test memory area
    for(loop = 0;loop < MEM2MEM_TEST_L; loop += 16) {
        for (i = 0; i < 4; i++)
            write_mreg32((MEM_TEST_SRC + loop + i * 4), test_mem_data[i]);
    }
    
    // Configure channel 0 to transfer data
    rest_val = MEM2MEM_TEST_L;
    SRC = MEM_TEST_SRC;
    DEST = MEM_TEST_DEST;
    DMAC_Init();
    
    printf("        	Transfer %d bytes from address 0x%x to 0x%x\n", MEM2MEM_TEST_L, MEM_TEST_SRC, MEM_TEST_DEST);
    
    while(1) {
        if(rest_val > (DMA_CH01_MAX_BLK_SIZE * 4)) {
            #if CK_AHBDMA_DEBUG
                printf("JJJ_DEBUG Rest 0x%x bytes > (DMA_CH01_MAX_BLK_SIZE * 4) 0x%x bytes\n", rest_val, DMA_CH01_MAX_BLK_SIZE * 4);
            #endif
            DMAMem2MemOpen(DMA_CHANNEL_0, SRC, DEST, DMA_CH01_MAX_BLK_SIZE, 0, 4);
            DMAC_Start(DMA_CHANNEL_0);
    
            printf ("read_mreg32 (DMAC_DMA_COMP_PARAM_1) =0x%x  \r\n  ",read_mreg32 (DMAC_DMA_COMP_PARAM_1));
            while(!(DMAC_CheckDone(DMA_CHANNEL_0)));
            rest_val -= (DMA_CH01_MAX_BLK_SIZE * 4);
            SRC += (DMA_CH01_MAX_BLK_SIZE * 4);
            DEST += (DMA_CH01_MAX_BLK_SIZE * 4);
        } else {
            #if CK_AHBDMA_DEBUG
                printf("JJJ_DEBUG Rest 0x%x bytes < (DMA_CH01_MAX_BLK_SIZE * 4) 0x%x bytes\n", rest_val, DMA_CH01_MAX_BLK_SIZE * 4);
            #endif
            DMAMem2MemOpen(DMA_CHANNEL_0, SRC, DEST, (rest_val / 4), 0, 4);
            DMAC_Start(DMA_CHANNEL_0);
            rest_val = 0;
            break;
        }
    }
    
    printf("        	Transfer Done\n");
    while(!(DMAC_CheckDone(DMA_CHANNEL_0)));
    DMAC_Close(DMA_CHANNEL_0);

    printf("        	Compare transfer data value\n");
    
    data_flag = 0;
    for(loop = 0;loop < MEM2MEM_TEST_L; loop += 16) {
        for (i = 0; i < 4; i++) {
            if (read_mreg32(MEM_TEST_SRC + loop + i * 4) != test_mem_data[i]) {
                #if CK_AHBDMA_DEBUG
                    printf("        	    Address 0x%x value 0x%x != 0x%x\n", MEM_TEST_SRC + loop + i * 4, read_mreg32(MEM_TEST_SRC + loop + i * 4), test_mem_data[i]);
                #endif
                data_flag = 1;
            }
        }
    }
    
    if (data_flag == 0)
        printf("        	    - - - PASS.\n");
    else
        printf("        	    - - - FAILURE.\n");
    
    printf("    	Memory to Memory Transfer Done\n");
}

void CK_AHBDMA_Test()
{
    printf("\nSynopsys AHB DMA Controller Test. . . \n");
    CK_AHBDMA_MEM2MEM_Test();
    CK_AHBDMA_UART_Test();
    printf("\nEnd Synopsys AHB DMA Controller Test. . . \n");
}



