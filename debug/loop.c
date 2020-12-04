/*****************************************************************************
 *			File Name		 : loop.c
 *			Create Date	 : 2020-11-26
 *      Modufy Date  : 
 *      Author       : Danny
 *			Information  :
 *****************************************************************************/
 
#include "fw_lpc845.h"

extern fw_io_handle_t io;
extern fw_io_entity_handle_t pin[8];
extern fw_spim_entity_handle_t spiEntity[8];

uint8_t txData[8] = "12345678";
uint8_t rxData[8];

fw_spim_entity_xfer_t xfer = {
	.tx = &txData,
	.rx = &rxData,
	.len = 8
};

void delay(uint32_t us){
	uint32_t i;
	for(i=0; i<=us; i++){
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}
}
extern void fw_io_pin_setToggle(fw_io_entity_handle_t handle);


void loop(){
	int i = 0;
	for(i=0; i<8; i++){
		pin[i].API->setToggle(pin[i]);
		while(spiEntity[i].API->isBusy(spiEntity[i])){
		}
		spiEntity[i].API->xfer(spiEntity[i], &xfer);
	}

}
