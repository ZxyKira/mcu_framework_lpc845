/*****************************************************************************
 *			File Name		 : loop.c
 *			Create Date	 : 2020-11-26
 *      Modufy Date  : 
 *      Author       : Danny
 *			Information  :
 *****************************************************************************/
 
#include "fw_lpc845.h"

extern fw_io_handle_t io;
extern fw_io_pin_handle_t pin[8];


void delay(uint32_t us){
	uint32_t i;
	for(i=0; i<=us; i++){
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}
}
extern void fw_io_pin_setToggle(fw_io_pin_handle_t handle);


void loop(){
	int i = 0;
	while(1){
		pin[0].API->setToggle(pin[0]);
		pin[0].API->setToggle(pin[0]);
		pin[0].API->setToggle(pin[0]);
		pin[0].API->setToggle(pin[0]);
		pin[0].API->setToggle(pin[0]);
		pin[0].API->setToggle(pin[0]);
		pin[0].API->setToggle(pin[0]);
		pin[0].API->setToggle(pin[0]);
	}
}