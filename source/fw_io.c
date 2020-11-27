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
extern const fw_io_pin_api_t fw_io_pin_api;
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

fw_io_pin_handle_t fw_io_getIoPin(fw_io_handle_t handle, const uint16_t port, uint16_t pin){
	fw_io_pin_handle_t result;
	fw_io_pin_memory_t fw_io_pin_memory;
	result.base = fw_io_getBase(handle);
	result.API = &fw_io_pin_api;
	((fw_io_pin_memory_t*)&result.memory)->port = port;
	((fw_io_pin_memory_t*)&result.memory)->pin = pin;
	return result;
}

/* *****************************************************************************************
 *    API Link
 */
const fw_io_api_t fw_io_api = {
	.read = fw_io_read,
	.set = fw_io_set,
	.clear = fw_io_clear,
	.toggle = fw_io_toggle,
	.dir = fw_io_dir,
	.dirClear = fw_io_dirClear,
	.dirSet = fw_io_dirSet,
	.getIoPin = fw_io_getIoPin
};


/* *****************************************************************************************
 *    End of file
 */
 