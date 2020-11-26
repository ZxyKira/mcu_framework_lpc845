/*-----------------------------------------------------------------------------------------
 *			File Name		:fw_define.c
 *			Create Date	:2020-11-26
 *			Modufy Date	:
 *			Information :
 */
 
#include <stdint.h>

#include "fw_define.h"
#include "fw_define_type.h"

/*-----------------------------------------------------------------------------------------
 *    System
 */ 
const fw_define_system_t fw_define_system = FW_DEFINE_SYSTEM_WRITE;

/*-----------------------------------------------------------------------------------------
 *    IO
 */ 
const fw_define_io_t fw_define_io[FW_DEFINE_IO_NUMB] = {
	FW_DEFINE_IO_WRITE(0)
};

/*-----------------------------------------------------------------------------------------
 *    USART
 */ 
 
const fw_define_usart_t fw_define_usart[FW_DEFINE_USART_NUMB] = {
	FW_DEFINE_USART_WRITE(0),
	FW_DEFINE_USART_WRITE(1),
	FW_DEFINE_USART_WRITE(2),
	FW_DEFINE_USART_WRITE(3),
	FW_DEFINE_USART_WRITE(4),
};

/*-----------------------------------------------------------------------------------------
 *    SPI
 */

const fw_define_spi_t fw_define_spi[FW_DEFINE_SPI_NUMB] = {
	FW_DEFINE_SPI_WRITE(0),
	FW_DEFINE_SPI_WRITE(1),
};

/*-----------------------------------------------------------------------------------------
 *    End of file
 */
