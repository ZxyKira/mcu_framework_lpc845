/* *****************************************************************************************
 *    File Name   :fw_chip_construct_io.c
 *    Create Date :2020-11-27
 *    Modufy Date :
 *    Information :
 */

#include <stdint.h>
#include <stdbool.h>

#include "LPC845.h"

#include "fw_chip.h"
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
extern const fw_define_io_t fw_define_io[FW_DEFINE_IO_NUMB];

/* *****************************************************************************************
 *    Public Variable
 */

/* *****************************************************************************************
 *    Private Variable
 */

/* *****************************************************************************************
 *    Inline Function
 */
static inline const fw_define_io_t* const fw_chip_io_getHwConf(uint8_t ch){
	assert(ch < FW_DEFINE_IO_NUMB);
	return &fw_define_io[ch];
}
/* *****************************************************************************************
 *    Private Function
 */ 
static void fw_chip_io_init(void){
	CLOCK_EnableClock(kCLOCK_Gpio0);
	CLOCK_EnableClock(kCLOCK_Gpio1);
	CLOCK_EnableClock(kCLOCK_GpioInt);
	CLOCK_EnableClock(kCLOCK_Iocon);
	RESET_PeripheralReset(kGPIO0_RST_N_SHIFT_RSTn);
	RESET_PeripheralReset(kGPIO1_RST_N_SHIFT_RSTn);
	RESET_PeripheralReset(kGPIOINT_RST_N_SHIFT_RSTn);
	RESET_PeripheralReset(kIOCON_RST_N_SHIFT_RSTn);
}

/* *****************************************************************************************
 *    Public Function
 */
fw_io_handle_t fw_chip_construct_io(uint32_t ch, void* memory){
	fw_io_handle_t result;
	result.memory = memory;
	result.API = 0x0;
	
	if(ch > FW_DEFINE_IO_NUMB)
		return result;
	
	fw_chip_io_init();
	result.API = & fw_io_api;
	result.memory = fw_chip_io_getHwConf(ch)->base;
	
	return result;
}
/* *****************************************************************************************
 *    End of file
 */
 