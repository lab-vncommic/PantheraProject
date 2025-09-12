/*
 * can.c
 *
 *  Created on: Sep 9, 2025
 *      Author: ADMIN
 */
#include "can.h"

// Global variable to store received CAN data
CAN_RxData_t can_rx_data;
CANTxFrame can_tx_data;
extern CANConfig can_config_mcanconf;

void CAN_Init(void)
{
	// Initialize CAN receive data structure
	can_rx_data.data_available = false;
	memset(&can_rx_data.frame, 0, sizeof(CANRxFrame));
	
	// init TX frame
	can_tx_data.ID = 0x00;
	can_tx_data.DLC =1;
	can_tx_data.data32[0]=0;
	can_tx_data.data32[1]=0;

	// TODO: Implement CAN initialization
	can_lld_start(CAN_DRIVER, &can_config_mcanconf);
}
void CAN_TX_Set_ID(uint32_t ID){can_tx_data.ID = ID;}
//void CAN_TX_Set_byte_RW(uint8_t data){can_tx_data.data8[0] = data;}
void CAN_TX_Set_byte_DATA(uint8_t data){can_tx_data.data8[0] = data;}

void CAN_HandleRequest(void)
{
	// TODO: Implement CAN request handling
}

void CAN_Transmit_Data()
{
    can_lld_transmit(CAN_DRIVER, CAN_DEDICATED_TXBUFFER, &can_tx_data);
}

_Bool CAN_Receive_Data(CANRxFrame *rxframe)
{
    if (can_rx_data.data_available && rxframe != NULL) {
        // Copy the received frame to the output parameter
        memcpy(rxframe, &can_rx_data.frame, sizeof(CANRxFrame));
        
        // Clear the data available flag
        can_rx_data.data_available = false;
        
        return true; // Data was available and copied
    }
    
    return false; // No data available
}

void mcanconf_panthera_rx_can(uint32_t msgbuf, CANRxFrame crfp){
#ifdef USE_CANFD
  if (crfp.OPERATION == CAN_OP_CANFD) {
#endif
    // Store the received frame in global variable
    memcpy(&can_rx_data.frame, &crfp, sizeof(CANRxFrame));
    can_rx_data.data_available = true;
    
    // Previous commented code for reference:
    //	  txf.ID = crfp.ID;
    //	  txf.data8[0] = crfp.data8[0];
    //	  txf.data8[1] = crfp.data8[1];
    //	  txf.DLC = crfp.DLC;
    // can_lld_transmit(&CAND3, CAN_DEDICATED_TXBUFFER, &txf);
#ifdef USE_CANFD
  }
#endif
  (void)msgbuf;
}
