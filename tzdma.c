// $COPYRIGHT
/*
*  This test shows how to use the ZDMA routines to copy data from external memory
*  to internal memory and from internal memory to external memory.
*  The program does the following steps:
*  1. Declare the 3 structures used by the ZDMA support routines
*     o DmaCtrlCfg to provide the Hardware configuration information.
*     o DmaChanObj to contain the descriptor and channel information for doing DMAs.
*     o DmaChanCfg to configure the channel object.
*
*  2. Initialize the 3 structures and the ZDMA hardware.
*     o Initialize the ZDMA hardware with the DmaCtrlCfg structure.
*     o Initialize the channel configuration structure
*     o Initialize the ZDMA channel for doing dma transfers.
*
*  3. Use ZDMA_CopyIn or ZDMA_CopyOut to transfer data.
*     Both routines require the following arguments:
*     o source address
*     o destination address
*       [ Internal addresses are ZSP based. External addresses are XXX based.]
*     o number of words to transfer
*     o the desired AHB burst size, or nonburst, or ZDMA_CFG2_BLIM_MAX to choose
*       the largest burst size based on the transfer size.
*
*     [The following restrictions are expected to be followed:
*      o The internal address is aligned to a (.walign 4) boundary.
*      o The external address is aligned to the burst boundary (.walign 8, 16, 32, 64)
*      o The number of words to transfer is a multiple of the burst or any number
*        for a nonburst transfer.
*      o The ZDMA_Chan_Obj_Type  descriptor is aligned to a (.walign 4 boundary)
*     ]
*
*  4. Poll for completion of the DMA using the ZDMA_WaitChDone() routine.
*     If there were any errors (illegal descriptor) an error status is returned.
*
*  5. Verifies the transferred data.
*
*  6. Performs the copyOut as an example of that routine working.
*/


#include "ck810.h"
#include "intc.h"
#include "datatype.h"
#include "misc.h"
#include <zdma.h>         // ZDMA include file

//#include "bsp.h"
//#include "printf.h"        // prototypes for the printf functions
//#include "exsys_ahbmap.h"     // address map defines
//#include <stdio.h>
//
//#include <stdlib.h>
//#include <creg.h>
//#include <string.h>
//#ifdef ZSP_CACHE
//  #include "zspcache.h"         // cache functions & defines
//  #include "cache_sw.h"         // prototypes for halt functions
//#endif

/*********************************************************************



**********************************************************************/
int dataset[] = {
#include "rand_data.h"
};
int dataset1[257];

ZDMA_Controller_Cfg_Type DmaCtrlCfg ALIGN_4 = { (PZDMA_Reg_Type)CK_AXI_DMA_Control, ZDMA_NUM_CHANNELS};
ZDMA_Channel_Cfg_Type    DmaChanCfg ALIGN_4; // Configures the channel object.
ZDMA_Chan_Obj_Type       DmaChanObj ALIGN_4; // This holds the DMA descriptor.

static int dmem_dest1[257] ALIGN_256; // Internal memory destination buffer.
static int *dmem_dest;

int CK_ZDMA_Test (void)
{
    int status;
    int failed;
    unsigned long data_length = sizeof(dataset);
    int i;
    char msg[32];

    printf("Copying data from external memory to internal.\n");
    // Initialize the DMA structures.
    ZDMA_Controller_Init(&DmaCtrlCfg);  // with the ZDMA base address and number of channels.
    ZDMA_Get_Controller_Addr(&DmaChanCfg.pDmaControllerObj);

    // Initialize a descriptor DmaChanObj to use channel 0.
    DmaChanCfg.Channel = 0;
    ZDMA_ChInit(&DmaChanObj, &DmaChanCfg);
    printf("Finished setup of ZDMA. Starting copy.\n");

    int *start_addr = dataset;
    dmem_dest = dmem_dest1;

    // test parameters
    //dmem_dest = dmem_dest1 + 1;
    //dmem_dest = dmem_dest1 + 2;

    //start_addr += 1; dmem_dest += 0;
    //start_addr += 1; dmem_dest += 1;
    //start_addr += 1; dmem_dest += 2;

    //start_addr += 2; dmem_dest += 0;
    //start_addr += 2; dmem_dest += 1;
    //start_addr += 2; dmem_dest += 2;

    //start_addr += 1; dmem_dest += 1; data_length = 1 + 16 + 1;
    //start_addr += 1; dmem_dest += 1; data_length = 1 + 32 + 1;

    status = 0;

    // Copy internal data memory out to AHB.
    //printf("Copying from %lx to %lx, %d words.\n", start_addr, dmem_dest, data_length);
    //printf("Copying %d words.\n", data_length);
    //printf("%d\n", data_length);
    printf(msg, "%d\n", data_length);
    printf(msg);
    setup_zdma_memcpy(&DmaCtrlCfg, &DmaChanCfg );
    ZDMA_memcpy_poll(dmem_dest, start_addr, data_length);

    printf("DMA completed with status %d. Checking %d data words.\n",
	       status, data_length);

    failed = status;
    if (!status)
      {
       printf ("Comparing data.\n");
       for(i=0;i<data_length;i++) // Comparing words instead of words.
        {
        if (start_addr[i] != dmem_dest[i])
	  {
          printf("Error: address %lx( offset %d) returned %x instead of data %x@%lx.\n",
                     &dmem_dest[i], i, dmem_dest[i], start_addr[i], &start_addr[i]);
          failed = 1;
	  }
        }
      }

    //return failed;

    if (!status)
      {
      printf("Now copy data back to sram.\n");

      // Copy internal data memory out to AHB into the next buffer location.
      printf("Copying from %lx to %lx, %d words.\n",
	         dmem_dest, start_addr+data_length, data_length);
      setup_zdma_memcpy(&DmaCtrlCfg, &DmaChanCfg );
      ZDMA_memcpy_poll( &start_addr[data_length], dmem_dest, data_length);

      printf("DMA completed with status %d. Checking %d data words.\n",
                 status, data_length);

      failed = status;
      if (!status)
        {
        printf ("Comparing data.\n");
        for(i=0;i<data_length;i++) // Comparing words
          {
          if (start_addr[i+data_length] != dmem_dest[i])
            {
            printf("Error: address %lx( offset %d) returned %x instead of internal data %x@%lx .\n",
                       &dmem_dest[i], i, dmem_dest[i], start_addr[i+data_length],&start_addr[i+data_length]);
            failed = 1;
            }
          }
        }
      }


    return failed; // 0= test passed; 1 = test failed
}