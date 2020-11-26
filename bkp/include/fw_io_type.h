/*-----------------------------------------------------------------------------------------
 *    File Name   :fw_io_type.c
 *    Version     :V1.0.0
 *    Create Date :2020-10-07
 *    Modufy Date :
 *    Information :
 */
#ifndef FW_IO_TYPE_H_
#define FW_IO_TYPE_H_

/* Standrad */
#include <stdint.h>
/* Chip */
#include "fsl_gpio.h"
#include "fsl_iocon.h"

/* Framework */
#include "fw_io.h"

/*-----------------------------------------------------------------------------------------
 *    Paraneter
 */ 
#define FW_DEFINE_IO_MAX_PIN_NUMB 54

/*-----------------------------------------------------------------------------------------
 *    Extern
 */
extern const uint8_t fw_io_ioconMap[FW_DEFINE_IO_MAX_PIN_NUMB];

/*-----------------------------------------------------------------------------------------
 *    Type/Structure
 */ 

/*-----------------------------------------------------------------------------------------
 *    Variable
 */

/*-----------------------------------------------------------------------------------------
 *    Function
 */

#endif //FW_IO_TYPE_H_
/*-----------------------------------------------------------------------------------------
 *    End of file
 */