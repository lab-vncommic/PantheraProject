/****************************************************************************
*
* Copyright © 2015-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/

/* Inclusion of the main header files of all the imported components in the
   order specified in the application wizard. The file is generated
   automatically.*/
#include "components.h"

// User's library
#include "Panthera.h"
#include "can_lld_cfg.h"


//can test
// CANTxFrame txf;
// CANRxFrame rxf;
//void mcanconf_panthera_rx_can(uint32_t msgbuf, CANRxFrame crfp){
//#ifdef USE_CANFD
//  if (crfp.OPERATION == CAN_OP_CANFD) {
//#endif
////	  rxf = crfp;
////    if (crfp.ID == 0x01) {
////      if(crfp.DLC == 2){//. check if DLC == 2
////    	  if (crfp.data8[1] == 0 ) // read data
////    	  {
////    		  txf.ID = crfp.ID;
////    		  txf.DLC = 2U;
////    		  txf.data8[0] = 0xAA;
////    		  txf.data8[1] = 0xFF;
////		  }
////    	  else if (crfp.data8[1] == 1){ //WRITE DATA
////    		  txf.ID = crfp.ID;
////    		  txf.DLC = 2U;
////    		  txf.data8[0] = 0xBB;
////    		  txf.data8[1] = 0xCC;
////    	  }
////    	  else {
////    		  txf.ID = crfp.ID;
////    		  txf.DLC = 2U;
////    		  txf.data8[0] = 0xDD;
////    		  txf.data8[1] = 0xEE;
////
////    	  }
////
////    	  can_lld_transmit(&CAND3, CAN_DEDICATED_TXBUFFER, &txf);
////      }
////    }
////	  txf.ID = crfp.ID;
////	  txf.data8[0] = crfp.data8[0];
////	  txf.data8[1] = crfp.data8[1];
////	  txf.DLC = crfp.DLC;
//	 // can_lld_transmit(&CAND3, CAN_DEDICATED_TXBUFFER, &txf);
//#ifdef USE_CANFD
//  }
//#endif
//  (void)msgbuf;
//
//}


uint16_t cnt;

void PIT0_ISR(void)
{
	PTR_main_interrupt_service();
}
uint32_t result_can;
/*
 * Application entry point.
 */
int main(void) {
	/* Initialization of all the imported components in the order specified in
	 the application wizard. The function is generated automatically.*/
	componentsInit();
	PTR_system_initialization();

	/* Interrupts are enabled and the system is ready.*/
	irqIsrEnable();



	/* prepare frame to be transmitted on MCAN SUB 0 CAN 1*/
//	  txf.TYPE = CAN_ID_STD;
//	  txf.ID = 0x00;
//	  txf.DLC = 2U;
//	  txf.data8[0] = 0x00;
//	  txf.data8[1] = 0x00;
//	#ifdef  USE_CANFD
//	  txf.OPERATION = CAN_OP_CANFD;
//	#endif




	/* Application main loop.*/
	for ( ; ; ) {
	  PTR_inf_loop_system();
		//result_can = can_lld_transmit(&CAND3, CAN_DEDICATED_TXBUFFER, &txf);
	}
}
