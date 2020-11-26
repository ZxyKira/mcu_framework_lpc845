/* *****************************************************************************************
 *    File Name   :fw_chip.c
 *    Create Date :2020-11-26
 *    Modufy Date :2020-11-26
 *    Information :
 */

#include <stdint.h>
#include <stdbool.h>

#include "LPC845.h"
#include "fsl_power.h"
#include "fsl_clock.h"

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
extern const fw_define_system_t fw_define_system;
extern const fw_define_io_t fw_define_io[FW_DEFINE_IO_NUMB];

/* *****************************************************************************************
 *    Public Variable
 */

/* *****************************************************************************************
 *    Private Variable
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
 *    Inline Function
 */
static inline const fw_define_io_t* const fw_io_getHwConf(uint8_t ch){
	assert(ch < FW_DEFINE_IO_NUMB);
	return &fw_define_io[ch];
}

 /* *****************************************************************************************
 *    Private Function
 */
void fw_chip_initCoreClock(uint32_t clock){
	/*!< Ensure FRO is on  */
	POWER_DisablePD(kPDRUNCFG_PD_FRO_OUT);      
	
	/*!< Ensure FRO is on  */
	POWER_DisablePD(kPDRUNCFG_PD_FRO);         

	/*!< Set up FRO freq */	
	CLOCK_SetFroOscFreq((clock_fro_osc_freq_t)(clock/1000));   
	
	/*!< Set FRO clock source */
	CLOCK_SetFroOutClkSrc(kCLOCK_FroSrcFroOsc);        
	
	/*!< Ensure Main osc is on */
	POWER_DisablePD(kPDRUNCFG_PD_SYSOSC);       
	
	/*!< Set main osc freq */
	CLOCK_InitSysOsc(12000000U);                
	
	/*!< select fro for main clock */
	CLOCK_SetMainClkSrc(kCLOCK_MainClkSrcFro);  
	CLOCK_SetCoreSysClkDiv(1U);
	
	/*!< Set SystemCoreClock variable. */
	SystemCoreClock = clock;	
}
 
/* *****************************************************************************************
 *    Public Function
 */
void fw_chip_chipInit(void){
	fw_chip_initCoreClock(fw_define_system.coreClock);
}

bool fw_chip_setClock(uint32_t clock){
	fw_chip_initCoreClock(clock);
}

fw_io_handle_t fw_chip_construct_io(uint32_t ch, void* memory){
	fw_io_handle_t result;
	result.memory = memory;
	result.API = 0x0;
	
	if(ch > FW_DEFINE_IO_NUMB)
		return result;
	
	fw_chip_io_init();
	result.API = & fw_io_api;
	((fw_io_memory_t*)result.memory)->base = fw_io_getHwConf(ch)->base;
	
	return result;
}

fw_spim_handle_t fw_chip_construct_spim(uint32_t ch, void* memory){
	
}

fw_usart_handle_t fw_chip_construct_usart(uint32_t ch, void* memory){
	
}

/* *****************************************************************************************
 *    API Link
 */
const fw_chip_api_t fw_chip_api = {
	.chipInit = fw_chip_chipInit,
	.setClock = fw_chip_setClock,
	.Construct = {
		.io = fw_chip_construct_io,
		.spim = fw_chip_construct_spim,
		.usart = fw_chip_construct_usart
	}
};


/* *****************************************************************************************
 *    End of file
 */
 