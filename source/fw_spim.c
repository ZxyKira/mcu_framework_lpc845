/* *****************************************************************************************
 *    File Name   :fw_spim.c
 *    Create Date :2020-11-27
 *    Modufy Date :
 *    Information :
 */

#include <stdint.h>
#include <stdbool.h>
#include "LPC845.h"

#include "fsl_swm.h"
#include "fsl_swm_connections.h"
#include "fsl_spi.h"
#include "fsl_iocon.h"

#include "fw_lpc845.h"
#include "fw_define_type.h"
#include "fw_chip_type.h"

/* *****************************************************************************************
 *    Parameter
 */
 
/* *****************************************************************************************
 *    Macro
 */
#define fw_spim_getMemory(handle) ((fw_spim_memory_t*)handle.memory)
#define fw_spim_getFwInfo(handle) ((fw_spi_hwInfo_t*)fw_spim_getMemory(handle)->hwInfo)
#define fw_spim_getFwConfig(handle) ((fw_define_spi_t*)fw_spim_getMemory(handle)->hwConfig)
#define fw_spim_getBase(handle) fw_spim_getMemory(handle)->base
 
/* *****************************************************************************************
 *    Type/Structure
 */ 
 
/* *****************************************************************************************
 *    Extern Function/Variable
 */
extern void fw_chip_iocon_pinMuxSet(uint8_t pin, uint32_t modefunc);
/* *****************************************************************************************
 *    Public Variable
 */

/* *****************************************************************************************
 *    Private Variable
 */
 
static void fw_spim_setSselIo(uint8_t pin){
	if(!(pin < FW_DEFINE_IO_MAX_PIN_NUMB))
		return;
	
	uint8_t port = pin>>5;
	pin &= 0x1F;
	
	
	GPIO->DIR[port] |=  (1 << pin);
	GPIO->B[port][pin] = 1;
}
 
static void fw_spim_setPin(const fw_define_spi_t* const pHwConf, const fw_spi_hwInfo_t* const pHwInfo){
	int i;
	
	uint32_t pinMod =  IOCON_MODE_PULLUP | IOCON_PIO_HYS_MASK;
	
	CLOCK_EnableClock(kCLOCK_Swm);
	
	if(pHwConf->Pin.sck < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.sck,  (swm_port_pin_type_t)(pHwConf->Pin.sck));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.sck, pinMod);
	}
	
	if(pHwConf->Pin.miso < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.miso, (swm_port_pin_type_t)(pHwConf->Pin.miso));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.miso, pinMod);
	}
	
	if(pHwConf->Pin.miso < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.mosi, (swm_port_pin_type_t)(pHwConf->Pin.mosi));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.mosi, pinMod);
	}
	
	for(i=0; i<pHwInfo->maxOfHwSselNumb; i++){		
		if(pHwConf->Pin.ssel[i] < FW_DEFINE_IO_MAX_PIN_NUMB){
			SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.ssel[i], (swm_port_pin_type_t)(pHwConf->Pin.ssel[i]));
			fw_chip_iocon_pinMuxSet(pHwConf->Pin.ssel[i], pinMod);
		}
	}
	
	for(i=0; i<FW_DEFINE_SPI_SSEL_NUMB; i++){
		fw_spim_setSselIo(pHwConf->Pin.ssel[i]);
	}
	
	CLOCK_DisableClock(kCLOCK_Swm);
}

/* *****************************************************************************************
 *    Inline Function
 */
 
/* *****************************************************************************************
 *    Private Function
 */ 
static void fw_spim_callback_handle(SPI_Type *base, spi_master_handle_t *handle, status_t status, void *userData){
	if(userData == 0)
		return;
	
	fw_spim_entity_handle_t* entityHandle = userData;
	fw_spim_entity_memory_t* entityMemory = entityHandle->memory;

	
	if(entityMemory->Event.onXferFinish != NULL){
		entityMemory->Event.onXferFinish(*entityHandle);
	}

	return;
}
/* *****************************************************************************************
 *    Public Function
 */


/* *****************************************************************************************
 *    API Function
 */
bool fw_spim_init(fw_spim_handle_t handle){
	spi_master_config_t config = {0};
	CLOCK_Select(fw_spim_getFwInfo(handle)->clockSel);
	
	/* Get hardware  */
	SPI_MasterGetDefaultConfig(&config);
	
	config.baudRate_Bps = fw_spim_getFwConfig(handle)->clock;
	config.sselNumber   = kSPI_Ssel0Assert;
	
	SPI_MasterInit(fw_spim_getFwConfig(handle)->base, &config, CLOCK_GetFreq(kCLOCK_MainClk));
	
	SPI_MasterTransferCreateHandle(fw_spim_getBase(handle), 
	                               &fw_spim_getMemory(handle)->handle, 
	                               fw_spim_callback_handle, 
	                               &fw_spim_getMemory(handle)->entityHandle);
	
	fw_spim_setPin(fw_spim_getFwConfig(handle), fw_spim_getFwInfo(handle));

	return true;
}

bool fw_spim_deinit(fw_spim_handle_t handle){
	/* Hardware deinit */
	SPI_Deinit(fw_spim_getBase(handle));	
	return true;
} 

bool fw_spim_isBusy(fw_spim_handle_t handle){
	if(fw_spim_getMemory(handle)->handle.state == kStatus_SPI_Busy)
		return true;
	else
		return false;
}

fw_spim_entity_handle_t fw_spim_getEntity(fw_spim_handle_t handle, void* memory, uint32_t ssel){
	fw_spim_entity_handle_t result = {0};
	memset(memory, 0x00, sizeof(fw_spim_entity_memory_t));
	
	//write ssel to spimEntity memory
	((fw_spim_entity_memory_t*)memory)->ssel = ssel;
	((fw_spim_entity_memory_t*)memory)->hwSetting = 0x00000005;
	((fw_spim_entity_memory_t*)memory)->baudrate = fw_spim_getFwConfig(handle)->clock;
	
	result.base = handle.memory;
	result.memory = memory;
	result.API = &fw_spim_entity_api;
	return result;
}

uint32_t getMaxSSEL(fw_spim_handle_t handle){
	return 8;
}


/* *****************************************************************************************
 *    API Lisk
 */
const fw_spim_api_t fw_spim_api = {
	.init = fw_spim_init,
	.deinit = fw_spim_deinit,
	.isBusy = fw_spim_isBusy,
	.getEntity = fw_spim_getEntity,
};

/* *****************************************************************************************
 *    End of file
 */
 