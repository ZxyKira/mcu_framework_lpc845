/* *****************************************************************************************
 *    File Name   :fw_chip_construct_spim.c
 *    Create Date :2020-11-27
 *    Modufy Date :
 *    Information :
 */

#include <stdint.h>
#include <stdbool.h>

#include "LPC845.h"
#include "fsl_power.h"
#include "fsl_clock.h"
#include "fsl_swm.h"
#include "fsl_swm_connections.h"
#include "fsl_iocon.h"

#include "fw_chip.h"
#include "fw_chip_type.h"
#include "fw_define_type.h"
#include "fw_lpc845.h"

/* *****************************************************************************************
 *    Parameter
 */
 
/* *****************************************************************************************
 *    Type/Structure
 */ 

/* *****************************************************************************************
 *    Extern Function/Variable
 */
extern const fw_define_spi_t fw_define_spi[FW_DEFINE_SPI_NUMB];

extern void fw_chip_iocon_pinMuxSet(uint8_t pin, uint32_t modefunc);
/* *****************************************************************************************
 *    Public Variable
 */

/* *****************************************************************************************
 *    Private Variable
 */
static const fw_spim_hwInfo_t fw_spi_hwInfo[2] = {
	{	
		.maxOfHwSselNumb = 4,
		.clockSel = kSPI0_Clk_From_MainClk,
		.pinMoveable = {
			.mosi = kSWM_SPI0_MOSI, 
			.miso = kSWM_SPI0_MISO, 
			.sck = kSWM_SPI0_SCK, 
			.ssel = {kSWM_SPI0_SSEL0, kSWM_SPI0_SSEL1, kSWM_SPI0_SSEL2, kSWM_SPI0_SSEL3}
		},
	},
	{
		.maxOfHwSselNumb = 2,
		.clockSel = kSPI1_Clk_From_MainClk,
		.pinMoveable = {
			.mosi = kSWM_SPI1_MOSI, 
			.miso = kSWM_SPI1_MISO, 
			.sck = kSWM_SPI1_SCK, 
			.ssel = {kSWM_SPI1_SSEL0, kSWM_SPI1_SSEL1, kSWM_MOVABLE_NUM_FUNCS, kSWM_MOVABLE_NUM_FUNCS}
		},
	}
};

/* *****************************************************************************************
 *    Inline Function
 */
static inline const fw_define_spi_t* const fw_spi_getHwConf(uint8_t ch){
	assert(ch < FW_DEFINE_SPI_NUMB);
	return &fw_define_spi[ch];
}

static inline const fw_spim_hwInfo_t* const fw_spi_getHwInfo(uint8_t ch){
	assert(ch < FW_DEFINE_SPI_NUMB);
	return (fw_spim_hwInfo_t*)&fw_spi_hwInfo[ch];
}
/* *****************************************************************************************
 *    Private Function
 */ 
static void fw_spim_setSSEL(uint8_t ch, uint8_t sselNumb){
	const fw_define_spi_t* const pHwConf = fw_spi_getHwConf(ch);
	const fw_spim_hwInfo_t* const pHwInfo = fw_spi_getHwInfo(ch);
	
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

static void fw_spim_setPin(uint8_t ch){
	const fw_define_spi_t* const pHwConf = fw_spi_getHwConf(ch);
	const fw_spim_hwInfo_t* const pHwInfo = fw_spi_getHwInfo(ch);
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
	
	CLOCK_DisableClock(kCLOCK_Swm);
}
 
/* *****************************************************************************************
 *    Public Function
 */

/* *****************************************************************************************
 *    API Function
 */
fw_spim_handle_t fw_chip_construct_spim(uint32_t ch, void* memory){
	fw_spim_handle_t result = {0};
	spi_master_config_t config = {0};
	
}

/* *****************************************************************************************
 *    End of file
 */
 