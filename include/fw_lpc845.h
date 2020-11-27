/* *****************************************************************************************
 *    File Name   :fw_lpc845.h
 *    Create Date :2020-10-13
 *    Modufy Date :
 *    Information :
 */

#ifndef fw_lpc845_H_
#define fw_lpc845_H_

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include "fw_chip.h"
#include "fw_spim.h"
#include "fw_spim_entity.h"
#include "fsl_spi.h"

/* *****************************************************************************************
 *    Parameter
 */ 
#define FW_CHIP_CLOCK_18M 18000000U
#define FW_CHIP_CLOCK_24M 24000000U
#define FW_CHIP_CLOCK_30M 30000000U

#define FW_DEFINE_IO_MAX_PIN_NUMB 54
/* *****************************************************************************************
 *    Type/Structure
 */ 

typedef struct _fw_io_memory_t{ 
	void* base;
}fw_io_memory_t;

typedef struct _fw_io_pin_memory_t{ 
	uint16_t port;
	uint16_t pin;
}fw_io_pin_memory_t;

typedef struct fw_io_pin_memory_t{ 
	uint8_t port;
	uint8_t pin;
}_fw_io_pin_memory_t;

typedef struct _fw_spim_memory_t{ 
	void* base;
	const void* hwConfig;
	spi_master_handle_t handle;
}fw_spim_memory_t;

typedef struct _fw_spim_entity_memory_t{
	uint32_t ssel;
	fw_spim_entity_xfer_t *xfer;
	struct{
		fw_spim_entity_event_onXferFinish onXferFinish;
		fw_spim_entity_event_onXferByte onXferByte;
	}Event;
}fw_spim_entity_memory_t;

/* *****************************************************************************************
 *    Extern Function/Variable
 */

extern const fw_io_api_t fw_io_api;
extern const fw_chip_api_t fw_chip_api;
extern const fw_io_pin_api_t fw_io_pin_api;

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //fw_lpc845_H_
/* *****************************************************************************************
 *    End of file
 */
 