/* *****************************************************************************************
 *    File Name   :fw_pin_io.c
 *    Create Date :2020-11-27
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

/* *****************************************************************************************
 *    Macro
 */
#define fw_io_pin_getBase(handle) ((GPIO_Type*)handle.base)
#define fw_io_pin_getPort(handle) ((uint16_t)(((uint32_t)handle.memory) & 0x0000FFFF))
#define fw_io_pin_getPin(handle)  ((uint16_t)((((uint32_t)handle.memory) >> 16) & 0x0000FFFF))

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
 
/* *****************************************************************************************
 *    Public Function
 */
void fw_io_pin_setValue(fw_io_entity_handle_t handle, bool val){
	fw_io_pin_getBase(handle)->B[fw_io_pin_getPort(handle)][fw_io_pin_getPin(handle)] = val;
}

void fw_io_pin_setHigh(fw_io_entity_handle_t handle){
	fw_io_pin_getBase(handle)->B[fw_io_pin_getPort(handle)][fw_io_pin_getPin(handle)] = 1;
}
	
void fw_io_pin_setLow(fw_io_entity_handle_t handle){
	fw_io_pin_getBase(handle)->B[fw_io_pin_getPort(handle)][fw_io_pin_getPin(handle)] = 0;
}
		
void fw_io_pin_setToggle(fw_io_entity_handle_t handle){
	fw_io_pin_getBase(handle)->NOT[fw_io_pin_getPort(handle)] |= (1 << fw_io_pin_getPin(handle));
}
			
void fw_io_pin_setDir(fw_io_entity_handle_t handle, bool dir){
	(dir) ? 
	/* set pin output. */
	(fw_io_pin_getBase(handle)->DIR[fw_io_pin_getPort(handle)] |=  (1 << fw_io_pin_getPin(handle))): 
	/* set pin input. */
	(fw_io_pin_getBase(handle)->DIR[fw_io_pin_getPort(handle)] &= ~(1 << fw_io_pin_getPin(handle)));
}
				
void fw_io_pin_setInput(fw_io_entity_handle_t handle){
	fw_io_pin_getBase(handle)->DIR[fw_io_pin_getPort(handle)] &= ~(1 << fw_io_pin_getPin(handle));	
}

void fw_io_pin_setOutput(fw_io_entity_handle_t handle){
	fw_io_pin_getBase(handle)->DIR[fw_io_pin_getPort(handle)] |=  (1 << fw_io_pin_getPin(handle));
}

bool fw_io_pin_getValue(fw_io_entity_handle_t handle){
	return (bool)GPIO_PinRead(GPIO, fw_io_pin_getPort(handle), fw_io_pin_getPin(handle));
}

bool fw_io_pin_getDir(fw_io_entity_handle_t handle){
	return (fw_io_pin_getBase(handle)->DIR[fw_io_pin_getPort(handle)] & (1 << fw_io_pin_getPin(handle)));
}

/* *****************************************************************************************
 *    API Link
 */
const fw_io_entity_api_t fw_io_entity_api = {
	.setValue 	= fw_io_pin_setValue,
	.setHigh 		= fw_io_pin_setHigh,
	.setLow 		= fw_io_pin_setLow,
	.setToggle 	= fw_io_pin_setToggle,
	.setDir 		= fw_io_pin_setDir,
	.setInput 	= fw_io_pin_setInput,
	.setOutput 	= fw_io_pin_setOutput,
	.getValue 	= fw_io_pin_getValue,
	.getDir 		= fw_io_pin_getDir
};

/* *****************************************************************************************
 *    End of file
 */
