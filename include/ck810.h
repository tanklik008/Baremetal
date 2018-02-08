/*
 * Description: ck5a6.h - Define the system configuration, memory & IO base
 * address, flash size & address, interrupt resource for ck5a6 soc.
 *
 * Copyright (C) : 2008 Hangzhou C-SKY Microsystems Co.,LTD.
 * Author(s): Liu Bing (bing_liu@c-sky.com)
 * Contributors: Liu Bing
 * Date:  2010-06-26
 * Modify by liu jirang  on 2012-09-11
 */

#ifndef __INCLUDE_CK5A6_H
#define __INCLUDE_CK5A6_H

#if CONFIG_CKCPU_MMU
//#define PERI_BASE 0xa0000000
#define PERI_BASE 0x00000000
#else
#define PERI_BASE 0x0
#endif

/**************************************
 * MCU & Borads.
 *************************************/

/* PLL input ckock(crystal frequency) */
#define CONFIG_PLL_INPUT_CLK   12000000   /* HZ */
/* CPU frequence definition */
#define CPU_DEFAULT_FREQ       80000000  /* Hz */
/* AHB frequence definition */
#define AHB_DEFAULT_FREQ       20000000   /* Hz */
/* APB frequence definition */
#define APB_DEFAULT_FREQ       16000000 //40000000   /* Hz */

/**********************************************
 * Config CPU cache
 *********************************************/
/* 0 - rw; 1 - rwc; 2 - rwc; 3 - rw */
#define CONFIG_CKCPU_MGU_BLOCKS         0xff06

/* 0 - baseaddr: 0x0; size: 4G */
#define CONFIG_CKCPU_MGU_REGION1        0x3f
/* 1- baseaddr: 0x8000000; size: 8M */
#define CONFIG_CKCPU_MGU_REGION2        0x800002f
/* 2- baseaddr: 0x8600000; size: 256K for MAC */
#define CONFIG_CKCPU_MGU_REGION3        0x2f
/* 3- Disable */
#undef CONFIG_CKCPU_MGU_REGION4

/*******************************
 * Config CPU cache
 ******************************/
//#define CONFIG_CKCPU_ICACHE             1
//#define CONFIG_CKCPU_DCACHE             1

/************************************************
 * perpheral module baseaddress and irq number
 ***********************************************/
/**** off-chip Nor FLASH ****/
#define FLASH_START					(volatile CK_UINT32 *)(0x00000000+PERI_BASE)
#define FLASH_END					(volatile CK_UINT32 *)(0x00800000+PERI_BASE)

/** off-chip SDRAM **/
#define MMC_SCONR					(volatile CK_UINT32 *)(0x08000000+PERI_BASE)

/**** AHB ****/
/**** AHB arbiter ****/
#define CK_AHBBUS_BASE				(volatile CK_UINT32 *)(0x10000000+PERI_BASE)

/** MMC Register **/
#define MMC_Register				(volatile CK_UINT32 *)(0x10001000+PERI_BASE)

/****** POWM  *************/
#define CK_POWM_ADDRBASE			(volatile CK_UINT32 *)(0x10002000+PERI_BASE)

/** Define DMA control base address **/
#define CK_AHB_DMA_TEST             0
#define CK_AHBDMA_DEBUG             0
//JJJ_DEBUG#define CK_AHB_DMA_CONTROL      	(volatile CK_UINT32 *)(0xFC100000+PERI_BASE)
#define CK_AHB_DMA_CONTROL      	0xFC100000 + PERI_BASE

/****** USBD  *************/
#define CK_USBD_ADDRBASE			(volatile CK_UINT32 *)(0x10005000+PERI_BASE)

/**** MAC *******/
#define CKMAC_BASEADDR				(volatile CK_UINT32 *)(0x10006000+PERI_BASE) /* mac base address */

/****** NFC  *************/
#define CK_NFC_ADDRBASE				(volatile CK_UINT32 *)(0x10008000+PERI_BASE)

/****** USBH  *************/
#define CK_USBH_ADDRBASE			(volatile CK_UINT32 *)(0x1000B000+PERI_BASE)

/****** SDHC  *************/
#define CK_SDHC_ADDRBASE			(volatile CK_UINT32 *)(0x1000C000+PERI_BASE)


/**** APB ****/
/***** Intc ******/
#define CK_INTC_TEST                0
#define CK_INTC_DEBUG               1
#if CONFIG_QEMU
#define CK_INTC_BASEADDRESS			(0x10010000+PERI_BASE)
#else
// For deepeye1000_DDR3_rdy_bp2_0118.bit
#define CK_INTC_BASEADDRESS			(0xFC20D000+PERI_BASE)
// For fpga_top_deepeye1000_0201.bit
//#define CK_INTC_BASEADDRESS			(0xFC600000+PERI_BASE)
#endif

//to enable INTC!
//JJJ_DEBUG #define CONFIG_INTC_DIS     1

/*
 * define irq number of perpheral modules
 */
#define  CK_INTC_MIPI	        1
#define  CK_INTC_BT1120_N       2
#define  CK_INTC_BT1120_ABN     3
#define  CK_INTC_H1	            4
#define  CK_INTC_G1	            5
#define  CK_INTC_G2             6
#define  CK_INTC_USB            7
#define  CK_INTC_BUSMON         8
#define  CK_INTC_AXIDMA0        9
#define  CK_INTC_AXIDMA1        10
#define  CK_INTC_AHBDMA         11 
#define  CK_INTC_MAILBOX0       12
#define  CK_INTC_MAILBOX1       13
#define  CK_INTC_MAILBOX2       14
#define  CK_INTC_MAILBOX3       15
#define  CK_INTC_SDIO0          16
#define  CK_INTC_SDIO1          17
#define  CK_INTC_SDIO2          18
#define  CK_INTC_SPI_MASTER     19
#define  CK_INTC_SPI_SLAVE      20
#if CONFIG_QEMU     
#define  CK_INTC_UART0          16
#else
// For deepeye1000_DDR3_rdy_bp2_0118.bit
//#define  CK_INTC_UART0          20
// fpga_top_deepeye1000_bp2_0207.bit
#define  CK_INTC_UART0          21
#endif      
#define  CK_INTC_UART1          22
#define  CK_INTC_UART2          23
#define  CK_INTC_I2C0           24
#define  CK_INTC_I2C1           25
#define  CK_INTC_I2C2           26
#define  CK_INTC_I2C3           27
#define  CK_INTC_TIM0           28
#define  CK_INTC_TIM1           29
#define  CK_INTC_TIM2           30
#define  CK_INTC_TIM3           31 
#define  CK_INTC_WDT0           32
#define  CK_INTC_WDT1           33
#define  CK_INTC_WDT2           34
#define  CK_INTC_GPIO           35
#define  CK_INTC_RTC            37



/**** Timer ****/
#define  CK_TIMER_TEST          0
#define  CK_TIMER0_BASSADDR			(volatile CK_UINT32 *)(0xFC200000+PERI_BASE)
#define  CK_TIMER1_BASSADDR			(volatile CK_UINT32 *)(0xFC201000+PERI_BASE)
#define  CK_TIMER2_BASSADDR			(volatile CK_UINT32 *)(0xFC202000+PERI_BASE)
#define  CK_TIMER3_BASSADDR			(volatile CK_UINT32 *)(0xFC203000+PERI_BASE)

/****** WDT *************/
#define CK_WDT_TEST                 1
#define CK_WDT_DEBUG                1
#define CK_WDT_ADDRBASE0			(volatile CK_UINT32 *)(0xFC204000+PERI_BASE)
#define CK_WDT_ADDRBASE1			(volatile CK_UINT32 *)(0xFC205000+PERI_BASE)
#define CK_WDT_ADDRBASE2			(volatile CK_UINT32 *)(0xFC206000+PERI_BASE)

/****** PWM  *************/
#define CK_PWM_ADDRBASE0			(volatile CK_UINT32 *)(0x10014000+PERI_BASE)

/***** Uart *******/
#if CONFIG_QEMU
#define CK_UART_ADDRBASE0			(volatile CK_UINT32 *)(0x10015000+PERI_BASE)
#define CK_UART_ADDRBASE1			(volatile CK_UINT32 *)(0x10016000+PERI_BASE)
#define CK_UART_ADDRBASE2			(volatile CK_UINT32 *)(0xFC402000+PERI_BASE)
#else
#define CK_UART_ADDRBASE0			(volatile CK_UINT32 *)(0xFC400000+PERI_BASE)
#define CK_UART_ADDRBASE1			(volatile CK_UINT32 *)(0xFC401000+PERI_BASE)
#define CK_UART_ADDRBASE2			(volatile CK_UINT32 *)(0xFC402000+PERI_BASE)
#define CK_UART_ADDR0			    (0xFC400000+PERI_BASE)
#endif

/***** PINMUX *****/
#define PCK_PINMUX_Control			(volatile CK_UINT32 *)(0xFC209000+PERI_BASE)

/***** GPIO *****/
#define CK_GPIO_Control			    (volatile CK_UINT32 *)(0xFC300000+PERI_BASE)

/****** SPI *************/
#define CK_SPI_ADDRBASE0			(volatile CK_UINT32 *)(0x1001A000+PERI_BASE)

/****** AXI DMA *************/
#define CK_AXI_DMA_Control			(volatile CK_UINT32 *)(0xFC500000+PERI_BASE)

#endif /* __INCLUDE_CK5A6_H */
