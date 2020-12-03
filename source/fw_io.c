/* *****************************************************************************************
 *    File Name   :fw_io.c
 *    Create Date :2020-11-26
 *    Modufy Date :
 *    Information :
 */

#include "LPC845.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"

#include "fw_lpc845.h"
#include "fw_define_type.h"

/* *****************************************************************************************
 *    Parameter
 */
#define fw_io_maxOfPortNumb 2

/* *****************************************************************************************
 *    Macro
 */
#define fw_io_getBase(handle) ((GPIO_Type*)handle.memory)

/* *****************************************************************************************
 *    Type/Structure
 */ 

/* *****************************************************************************************
 *    Extern Function/Variable
 */
extern const fw_io_entity_api_t fw_io_entity_api;
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

/* *****************************************************************************************
 *    Public Function
 */
bool fw_io_init(fw_io_handle_t handle){
	CLOCK_EnableClock(kCLOCK_Gpio0);
	CLOCK_EnableClock(kCLOCK_Gpio1);
	CLOCK_EnableClock(kCLOCK_GpioInt);
	CLOCK_EnableClock(kCLOCK_Iocon);
	RESET_PeripheralReset(kGPIO0_RST_N_SHIFT_RSTn);
	RESET_PeripheralReset(kGPIO1_RST_N_SHIFT_RSTn);
	RESET_PeripheralReset(kGPIOINT_RST_N_SHIFT_RSTn);
	RESET_PeripheralReset(kIOCON_RST_N_SHIFT_RSTn);
	return true;
}
 
bool fw_io_deinit(fw_io_handle_t handle){
	CLOCK_DisableClock(kCLOCK_Gpio0);
	CLOCK_DisableClock(kCLOCK_Gpio1);
	CLOCK_DisableClock(kCLOCK_GpioInt);
	CLOCK_DisableClock(kCLOCK_Iocon);
	return true;
}

uint32_t fw_io_read(fw_io_handle_t handle, const uint16_t port){
	return GPIO_PortRead(fw_io_getBase(handle), port);
}
		
void fw_io_set(fw_io_handle_t handle, const uint16_t port, uint32_t mask){
	GPIO_PortSet(fw_io_getBase(handle), port, mask);
	return;
}
		
void fw_io_clear(fw_io_handle_t handle, const uint16_t port, uint32_t mask){
	GPIO_PortClear(fw_io_getBase(handle), port, mask);
  return;
}
		
void fw_io_toggle(fw_io_handle_t handle, const uint16_t port, uint32_t mask){
	GPIO_PortToggle(fw_io_getBase(handle), port, mask);
	return;
}
		
void fw_io_dir(fw_io_handle_t handle, const uint16_t port, uint32_t val){
	fw_io_getBase(handle)->DIR[port] = val;
	return;
}
		
void fw_io_dirClear(fw_io_handle_t handle, const uint16_t port, uint32_t mask){
	fw_io_getBase(handle)->DIRCLR[port] = mask;
	return;
}
		
void fw_io_dirSet(fw_io_handle_t handle, const uint16_t port, uint32_t mask){
	fw_io_getBase(handle)->DIRSET[port] = mask;
	return;
}

fw_io_entity_handle_t fw_io_getEntity(fw_io_handle_t handle, void* memory, const uint16_t port, uint16_t pin){
	fw_io_entity_handle_t result;
	fw_io_entity_memory_t fw_io_pin_memory;
	result.base = fw_io_getBase(handle);
	result.API = &fw_io_entity_api;
	((fw_io_entity_memory_t*)&result.memory)->port = port;
	((fw_io_entity_memory_t*)&result.memory)->pin = pin;
	return result;
}

/* *****************************************************************************************
 *    API Link
 */
const fw_io_api_t fw_io_api = {
	.init  =fw_io_init,
	.deinit = fw_io_deinit,
	.read = fw_io_read,
	.set = fw_io_set,
	.clear = fw_io_clear,
	.toggle = fw_io_toggle,
	.dir = fw_io_dir,
	.dirClear = fw_io_dirClear,
	.dirSet = fw_io_dirSet,
	.getEntity = fw_io_getEntity
};


/* *****************************************************************************************
 *    End of file
 */
 