/*****************************************************************************
 *			File Name		 : hw_spi.c
 *			Version			 : V1.0
 *			Create Date	 : 2020-10-06
 *      Modufy Date  : 2020-10-06
 *      Author       : Danny
 *			Information  :
 */
 
#include "fw_chip.h"
#include "LPC845.h"
 
uint8_t SPIM_TXD[32] = "AAAA0000";
uint8_t SPIM_RXD[32];
fw_spim_xfer_t FW_SPI0_XFER;

static void fw_spi_0_cb(const fw_spim_entity_t* const pAPI, void* userData){
	GPIO->NOT[1] = 8;
}
 
void hw_spi_init_0(){
	
	
	fw_spim_api.init(0);
	FW_SPI0_XFER.config = fw_spim_api.getDefaultConfig(0);
	FW_SPI0_XFER.Data.pTx = & SPIM_TXD;
	FW_SPI0_XFER.Data.pRx = & SPIM_RXD;
	FW_SPI0_XFER.Data.len = 2;
	FW_SPI0_XFER.callback.foo = fw_spi_0_cb;
}