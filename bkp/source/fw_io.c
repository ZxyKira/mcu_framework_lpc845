/*-----------------------------------------------------------------------------------------
 *    File Name   : fw_io.c
 *    Version     : V1.0.0
 *    Create Date : 2020-08-20
 *    Modufy Date : 2020-10-07
 *    Information :
 */
#include "fw_io.h"

#include "LPC845.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "ztool.h"

#include "fw_io_type.h"
#include "fw_define_type.h"

/*-----------------------------------------------------------------------------------------
 *    Parameter
 */
#define fw_io_maxOfPinNumb 32
#define fw_io_maxOfPortNumb 2
#define fw_io_pinNumbCheckEnable 0

/*-----------------------------------------------------------------------------------------
 *    Extern Function/Variable
 */
extern const fw_define_io_t fw_define_io[FW_DEFINE_IO_NUMB];

/*-----------------------------------------------------------------------------------------
 *    Local Type/Structure
 */
static inline const fw_define_io_t* const fw_io_getHwConf(uint8_t ch){
	assert(ch < FW_DEFINE_IO_NUMB);
	return &fw_define_io[ch];
}

static inline GPIO_Type * fw_io_getBase(uint8_t ch){
	assert(ch < FW_DEFINE_IO_NUMB);
	return ((GPIO_Type *)fw_define_io[ch].base);
}
 
/*-----------------------------------------------------------------------------------------
 *    Local Variable
 */
const fw_io_api_t fw_io_api;
const fw_io_entity_api_t fw_io_entity_api;

/*-----------------------------------------------------------------------------------------
 *    inline Function
 */

/*-----------------------------------------------------------------------------------------
 *    Public Function - API
 */
static void fw_io_init(const uint8_t ch){
	if(ch == 0){
		CLOCK_EnableClock(kCLOCK_Gpio0);
		CLOCK_EnableClock(kCLOCK_Gpio1);
		CLOCK_EnableClock(kCLOCK_GpioInt);
		CLOCK_EnableClock(kCLOCK_Iocon);
		RESET_PeripheralReset(kGPIO0_RST_N_SHIFT_RSTn);
		RESET_PeripheralReset(kGPIO1_RST_N_SHIFT_RSTn);
		RESET_PeripheralReset(kGPIOINT_RST_N_SHIFT_RSTn);
		RESET_PeripheralReset(kIOCON_RST_N_SHIFT_RSTn);
	}else{
		/* program cannot run in to here */
		while(1);
	}
}
  
static void fw_io_deinit(const uint8_t ch){
	if(ch == 0){
		CLOCK_DisableClock(kCLOCK_Gpio0);
		CLOCK_DisableClock(kCLOCK_Gpio1);
		CLOCK_DisableClock(kCLOCK_GpioInt);
		CLOCK_DisableClock(kCLOCK_Iocon);
	}else{
		/* program cannot run in to here */
		while(1);
	}
	
	return;
}
  
static fw_io_entity_t fw_io_getEntity(const uint8_t ch, const uint16_t port, const uint16_t pin){
	fw_io_entity_t result = {
		.pAPI = &fw_io_entity_api,
		.ch = ch,
		.port = port,
		.pin = pin, 
	};
	return result;
}
  
static inline void fw_io_setValue(const uint8_t ch, const uint16_t port, const uint16_t pin, const bool val){
	fw_io_getBase(ch)->B[port][pin] = val;
}
  
static inline void fw_io_setHigh(const uint8_t ch, const uint16_t port, const uint16_t pin){
	fw_io_getBase(ch)->B[port][pin] = 1;
}
  
static inline void fw_io_setLow(const uint8_t ch, const uint16_t port, const uint16_t pin){
	fw_io_getBase(ch)->B[port][pin] = 0;
}
  
static inline void fw_io_setToggle(const uint8_t ch, const uint16_t port, const uint16_t pin){
	fw_io_getBase(ch)->NOT[port] |= (1 << pin);
}
  
static inline void fw_io_setDir(const uint8_t ch, const uint16_t port, const uint16_t pin, bool dir){
	(dir) ? 
	/* set pin output. */
	(fw_io_getBase(ch)->DIR[port] |=  (1<<pin)): 
	/* set pin input. */
	(fw_io_getBase(ch)->DIR[port] &= ~(1<<pin));
}
  
static inline void fw_io_setInput(const uint8_t ch, const uint16_t port, const uint16_t pin){
	fw_io_getBase(ch)->DIR[port] &= ~(1<<pin);	
}
  
static inline void fw_io_setOutput(const uint8_t ch, const uint16_t port, const uint16_t pin){
	fw_io_getBase(ch)->DIR[port] |=  (1<<pin);	
}
  
static inline bool fw_io_getValue(const uint8_t ch, const uint16_t port, const uint16_t pin){
	return (bool)GPIO_PinRead(GPIO, port, pin);
}
  
static inline bool fw_io_getDir(const uint8_t ch, const uint16_t port, const uint16_t pin){
	return isBitHigh(((GPIO_Type *)GPIO)->DIR[port], pin);
}
		
static uint32_t fw_io_port_read(const uint8_t ch, const uint16_t port){
	return GPIO_PortRead(fw_io_getBase(ch), port);
}
		
static void fw_io_port_set(const uint8_t ch, const uint16_t port, uint32_t mask){
	GPIO_PortSet(fw_io_getBase(ch), port, mask);
	return;
}
		
static void fw_io_port_clear(const uint8_t ch, const uint16_t port, uint32_t mask){
	GPIO_PortClear(fw_io_getBase(ch), port, mask);
  return;
}
		
static void fw_io_port_toggle(const uint8_t ch, const uint16_t port, uint32_t mask){
	GPIO_PortToggle(fw_io_getBase(ch), port, mask);
	return;
}
		
static void fw_io_port_dir(const uint8_t ch, const uint16_t port, uint32_t val){
	fw_io_getBase(ch)->DIR[port] = val;
	return;
}
		
static void fw_io_port_dirClear(const uint8_t ch, const uint16_t port, uint32_t mask){
	fw_io_getBase(ch)->DIRCLR[port] = mask;
	return;
}
		
static void fw_io_port_dirSet(const uint8_t ch, const uint16_t port, uint32_t mask){
	fw_io_getBase(ch)->DIRSET[port] = mask;
	return;
}

/*-----------------------------------------------------------------------------------------
 *    API Link - API
 */
const fw_io_api_t fw_io_api = {
	.init      = fw_io_init,
	.deinit    = fw_io_deinit,
	.getEntity = fw_io_getEntity,
	.setValue  = fw_io_setValue,
	.setHigh   = fw_io_setHigh,
	.setLow    = fw_io_setLow,
	.setToggle = fw_io_setToggle,
	.setDir    = fw_io_setDir,
	.setInput  = fw_io_setInput,
	.setOutput = fw_io_setOutput,
	.getValue  = fw_io_getValue,
	.getDir    = fw_io_getValue,
	.Port = {
		.read     = fw_io_port_read,
		.set      = fw_io_port_set,
		.clear    = fw_io_port_clear,
		.toggle   = fw_io_port_toggle,
		.dir      = fw_io_port_dir,
		.dirClear = fw_io_port_dirClear,
		.dirSet   = fw_io_port_dirSet,
	},
};

/*-----------------------------------------------------------------------------------------
 *    Function - Entity API
 */
static void fw_io_entity_setValue(const fw_io_entity_t entity, bool val){
	fw_io_setValue(entity.ch, entity.port, entity.pin, val);
}
  
static void fw_io_entity_setHigh(const fw_io_entity_t entity){
	fw_io_setHigh(entity.ch, entity.port, entity.pin);
}
  
static void fw_io_entity_setLow(const fw_io_entity_t entity){
	fw_io_setLow(entity.ch, entity.port, entity.pin);
}
  
static void fw_io_entity_setToggle(const fw_io_entity_t entity){
	fw_io_setToggle(entity.ch, entity.port, entity.pin);
}  
  
static void fw_io_entity_setDir(const fw_io_entity_t entity, bool dir){
	fw_io_setDir(entity.ch, entity.port, entity.pin, dir);
}
  
static void fw_io_entity_setInput(const fw_io_entity_t entity){
	fw_io_setInput(entity.ch, entity.port, entity.pin);
}
  
static void fw_io_entity_setOutput(const fw_io_entity_t entity){
	fw_io_setOutput(entity.ch, entity.port, entity.pin);
}
  
static bool fw_io_entity_getValue(const fw_io_entity_t entity){
	return fw_io_getValue(entity.ch, entity.port, entity.pin);
}
  
static bool fw_io_entity_getDir(const fw_io_entity_t entity){
	return fw_io_getDir(entity.ch, entity.port, entity.pin);
}
/*-----------------------------------------------------------------------------------------
 *    API Link - Entity API
 */
const fw_io_entity_api_t fw_io_entity_api = {
	.setValue  = fw_io_entity_setValue,
	.setHigh   = fw_io_entity_setHigh,
	.setLow    = fw_io_entity_setLow,
	.setToggle = fw_io_entity_setToggle,
	.setDir    = fw_io_entity_setDir,
	.setInput  = fw_io_entity_setInput,
	.setOutput = fw_io_entity_setOutput,
	.getValue  = fw_io_entity_getValue,
	.getDir    = fw_io_entity_getDir,
};

/*-----------------------------------------------------------------------------------------
 *    End of file
 */
