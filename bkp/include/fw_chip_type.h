/*-----------------------------------------------------------------------------------------
 *    File Name   :fw_chip_type.c
 *    Version     :V1.0.0
 *    Create Date :2020-10-07
 *    Modufy Date :
 *    Information :
 */
#ifndef FW_CHIP_TYPE_H_
#define FW_CHIP_TYPE_H_

/* Standrad */
#include <stdint.h>

/* Framework */
#include "fw_io_type.h"

/*-----------------------------------------------------------------------------------------
 *    Paraneter
 */ 

/*-----------------------------------------------------------------------------------------
 *    Extern Variable
 */
extern const uint8_t fw_io_ioconMap[FW_DEFINE_IO_MAX_PIN_NUMB];

/*-----------------------------------------------------------------------------------------
 *    Extern Function
 */
void fw_chip_init(void);



/*-----------------------------------------------------------------------------------------
 *    Type/Structure
 */ 

/*-----------------------------------------------------------------------------------------
 *    Variable
 */

/*-----------------------------------------------------------------------------------------
 *    Function
 */
static inline void fw_chip_iocon_pinMuxSet(uint8_t pin, uint32_t modefunc){
	assert(pin < FW_DEFINE_IO_MAX_PIN_NUMB);
	IOCON_PinMuxSet(IOCON, fw_io_ioconMap[pin], modefunc);
}

#endif //FW_CHIP_TYPE_H_
/*-----------------------------------------------------------------------------------------
 *    End of file
 */