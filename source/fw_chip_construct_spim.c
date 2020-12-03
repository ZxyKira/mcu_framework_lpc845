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

/* *****************************************************************************************
 *    Public Variable
 */

/* *****************************************************************************************
 *    Private Variable
 */
static const fw_spi_hwInfo_t fw_spi_hwInfo[2] = {
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

static inline const fw_spi_hwInfo_t* const fw_spi_getHwInfo(uint8_t ch){
	assert(ch < FW_DEFINE_SPI_NUMB);
	return (fw_spi_hwInfo_t*)&fw_spi_hwInfo[ch];
}
/* *****************************************************************************************
 *    Private Function
 */ 
 
/* *****************************************************************************************
 *    Public Function
 */

/* *****************************************************************************************
 *    API Function
 */
fw_spim_handle_t fw_chip_construct_spim(uint32_t ch, void* memory){
	fw_spim_handle_t result = {0};
	if(ch >= FW_DEFINE_SPI_NUMB)
		return result;
	
	//write mamory point to handle
	result.memory = memory;
	memset(memory, 0x00 ,sizeof(fw_spim_memory_t));
	
	//get hardware config
	((fw_spim_memory_t*)result.memory)->hwInfo = fw_spi_getHwInfo(ch);
	((fw_spim_memory_t*)result.memory)->hwConfig = fw_spi_getHwConf(ch);
	((fw_spim_memory_t*)result.memory)->base = fw_spi_getHwConf(ch)->base;

	return result;
}

/* *****************************************************************************************
 *    End of file
 */
 