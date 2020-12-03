/* *****************************************************************************************
 *    File Name   :fw_spim_entity.c
 *    Create Date :2020-12-03
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
#include "fsl_spi.h"

#include "fw_lpc845.h"
#include "fw_define_type.h"
#include "fw_chip_type.h"

/* *****************************************************************************************
 *    Parameter
 */
 
/* *****************************************************************************************
 *    Macro
 */
#define fw_sipm_entity_getBaseMemory(handle)   ((fw_spim_memory_t*)handle.base)
#define fw_sipm_entity_getMemory(handle)       ((fw_spim_entity_memory_t*)handle.memory)
#define fw_spim_entity_getBase(handle)         ((SPI_Type*)fw_sipm_entity_getBaseMemory(handle)->base)
#define fw_spim_entity_getDelayConfig(handle)  ((spi_delay_config_t*)&fw_sipm_entity_getMemory(handle)->Delay)
#define fw_spim_entity_getFwInfo(handle)       ((fw_spi_hwInfo_t*)fw_sipm_entity_getBaseMemory(handle)->hwInfo)
#define fw_spim_entity_getFwConfig(handle)     ((fw_define_spi_t*)fw_sipm_entity_getBaseMemory(handle)->hwConfig)
 
/* *****************************************************************************************
 *    Type/Structure
 */ 
 
/* *****************************************************************************************
 *    Extern Function/Variable
 */

/* *****************************************************************************************
 *    Public Variable
 */

/* *****************************************************************************************
 *    Private Variable
 */

/* *****************************************************************************************
 *    Inline Function
 */
 
/* *****************************************************************************************
 *    Private Function
 */ 
static void fw_spim_entity_setSSEL(const fw_define_spi_t* pHwConf, const fw_spi_hwInfo_t* pHwInfo, uint8_t sselNumb){
	SPI_Type* hwBase = pHwConf->base;
	
	/* Unset all hardware ssel */
	hwBase->TXCTL &= 0xFFF0FFFF;

	
	if(sselNumb >= (pHwInfo->maxOfHwSselNumb-1)){
		uint32_t sselMask = ~(1 << (16 + (pHwInfo->maxOfHwSselNumb-1)));
		sselMask &= 0x000F0000;
		
		/* Set hardware ssel */
		hwBase->TXCTL |= sselMask;
		
		CLOCK_EnableClock(kCLOCK_Swm);
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.ssel[3], (swm_port_pin_type_t)(pHwConf->Pin.ssel[sselNumb]));
		CLOCK_DisableClock(kCLOCK_Swm);
		
		
	}else{
		/* Set hardware ssel */
		uint32_t sselMask = ~(1 << (16 + (sselNumb)));
		sselMask &= 0x000F0000;
		
		hwBase->TXCTL |= sselMask;
	}
}
/* *****************************************************************************************
 *    Public Function
 */ 
 
/* *****************************************************************************************
 *    API Function
 */
bool fw_spim_entity_isBusy(fw_spim_entity_handle_t handle){
	if(fw_sipm_entity_getBaseMemory(handle)->handle.state != kStatus_SPI_Busy)
		return true;
	else
		return false;
	
	
	
}

bool fw_spim_entity_xfer(fw_spim_entity_handle_t handle, fw_spim_entity_xfer_t *xfer){
	if(fw_spim_entity_isBusy(handle))
		return false;
	
	SPI_Type* hwBase = fw_spim_entity_getBase(handle);
	uint32_t localResult = 0;
	
	
	
	SPI_SetTransferDelay(hwBase, fw_spim_entity_getDelayConfig(handle));
	
	localResult = SPI_MasterSetBaudRate(hwBase, 
	                                    fw_sipm_entity_getMemory(handle)->baudrate, 
	                                    CLOCK_GetFreq(kCLOCK_MainClk));
	
		/* Write hardware config to registor */
	hwBase->CFG = (fw_sipm_entity_getMemory(handle)->hwSetting | 0x00000005);
	
	/* Clear Transfer finish flag */
	hwBase->TXCTL &= ~(1<<SPI_TXCTL_EOT_SHIFT);
	
	fw_spim_entity_setSSEL(fw_spim_entity_getFwConfig(handle), 
	                       fw_spim_entity_getFwInfo(handle),
	                       fw_sipm_entity_getMemory(handle)->ssel);
												 
  spi_transfer_t hwXfer = {
		.txData = (uint8_t*)xfer->tx,
		.rxData = (uint8_t*)xfer->rx,
		.dataSize = xfer->len,
		.configFlags = 	kSPI_EndOfTransfer
	};
	
	fw_sipm_entity_getBaseMemory(handle)->onHandle = &handle;
	
		/* Create spi master transfer handle  */
	status_t handleResult = SPI_MasterTransferNonBlocking(hwBase, 
	                                                      &fw_sipm_entity_getBaseMemory(handle)->handle, 
	                                                      &hwXfer);
	
	if(handleResult == kStatus_Success)
		return true;
	
	else
		return false;
}

void fw_spim_entity_setCpha(fw_spim_entity_handle_t handle, bool enable){
	(enable)?
	(fw_sipm_entity_getMemory(handle)->hwSetting |=  SPI_CFG_CPHA_MASK):
	(fw_sipm_entity_getMemory(handle)->hwSetting &= ~SPI_CFG_CPHA_MASK);
}

void fw_spim_entity_setCpol(fw_spim_entity_handle_t handle, bool enable){
	(enable)?
	(fw_sipm_entity_getMemory(handle)->hwSetting |=  SPI_CFG_CPOL_MASK):
	(fw_sipm_entity_getMemory(handle)->hwSetting &= ~SPI_CFG_CPOL_MASK);
}

void fw_spim_entity_setLsb(fw_spim_entity_handle_t handle, bool enable){
	(enable)?
	(fw_sipm_entity_getMemory(handle)->hwSetting |=  SPI_CFG_LSBF_MASK):
	(fw_sipm_entity_getMemory(handle)->hwSetting &= ~SPI_CFG_LSBF_MASK);
}

void fw_spim_entity_setLoop(fw_spim_entity_handle_t handle, bool enable){
	(enable)?
	(fw_sipm_entity_getMemory(handle)->hwSetting |=  SPI_CFG_LOOP_MASK):
	(fw_sipm_entity_getMemory(handle)->hwSetting &= ~SPI_CFG_LOOP_MASK);
}

void fw_spim_entity_setPreDelay(fw_spim_entity_handle_t handle, uint8_t val){
	if(val>=8)
		val = 7;
	
	fw_sipm_entity_getMemory(handle)->Delay.preDelay = val;
}

void fw_spim_entity_setPostDelay(fw_spim_entity_handle_t handle, uint8_t val){
	if(val>=8)
		val = 7;
	
	fw_sipm_entity_getMemory(handle)->Delay.postDelay = val;
}

void fw_spim_entity_setFrameDelay(fw_spim_entity_handle_t handle, uint8_t val){
	if(val>=8)
		val = 7;
	
	fw_sipm_entity_getMemory(handle)->Delay.frameDelay = val;
}

void fw_spim_entity_setTransferDelay(fw_spim_entity_handle_t handle, uint8_t val){
	if(val>=8)
		val = 7;
	
	fw_sipm_entity_getMemory(handle)->Delay.transferDelay = val;
}

bool fw_spim_entity_setBaudrate(fw_spim_entity_handle_t handle, uint32_t baudrate){
	fw_sipm_entity_getMemory(handle)->baudrate = baudrate;
}

/* *****************************************************************************************
 *    API Link
 */
const fw_spim_entity_api_t fw_spim_entity_api = {
  .xfer             = fw_spim_entity_xfer,
  .isBusy           = fw_spim_entity_isBusy,
  .setCpha          = fw_spim_entity_setCpha,
  .setCpol	        = fw_spim_entity_setCpol,
  .setLsb           = fw_spim_entity_setLsb,
  .setLoop          = fw_spim_entity_setLoop,
  .setPreDelay      = fw_spim_entity_setPreDelay,
  .setPostDelay     = fw_spim_entity_setPostDelay,
  .setFrameDelay    = fw_spim_entity_setFrameDelay,
  .setTransferDelay = fw_spim_entity_setTransferDelay,
  .setBaudrate      = fw_spim_entity_setBaudrate
};

/* *****************************************************************************************
 *    End of file
 */
 