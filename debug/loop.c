/*****************************************************************************
 *			File Name		 : loop.c
 *			Create Date	 : 2020-11-26
 *      Modufy Date  : 
 *      Author       : Danny
 *			Information  :
 *****************************************************************************/
 
#include "fw_lpc845.h"

extern fw_io_handle_t io;

void delay(uint32_t us){
	uint32_t i;
	for(i=0; i<=us; i++){
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}
}


void loop(){
	io.API->toggle(&io, 1, 0xFFFFFFFF);
	delay(1000000);
}