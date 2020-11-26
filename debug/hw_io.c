/*****************************************************************************
 *			File Name		 : hw_io.c
 *			Version			 : V1.0
 *			Create Date	 : 2020-10-06
 *      Modufy Date  : 2020-10-06
 *      Author       : Danny
 *			Information  :
 */
 
#include "fw_chip.h"
#include "LPC845.h"

fw_io_entity_t LED[8];

void hw_io_init(void){
	/* init gpio hardware */
	fw_io_api.init(0);
	
	int i;
	
	for(i=0; i<8; i++){
		
		/* Get io PIO1_i profile to LED[i] */
		LED[i] = fw_io_api.getEntity(0, 1, i);
		fw_io_entity_api.setOutput(LED[i]);

		fw_io_entity_api.setLow(LED[i]);
		/* Set io to low */
	}
}