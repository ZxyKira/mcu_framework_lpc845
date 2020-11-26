/*-----------------------------------------------------------------------------------------
 *    File Name   :fw_spi_type.c
 *    Version     :V1.0.0
 *    Create Date :2020-10-07
 *    Modufy Date :
 *    Information :
 */
#ifndef FW_SPI_TYPE_H_
#define FW_SPI_TYPE_H_

/* Chip */
#include "fsl_spi.h"

/* Framework */
#include "fw_spi.h"

/*-----------------------------------------------------------------------------------------
 *    Parameter
 */
#ifndef FW_DEFINE_SPI_SSEL_MAX_NUMB
#define FW_DEFINE_SPI_SSEL_MAX_NUMB 8
#endif

/*-----------------------------------------------------------------------------------------
 *    Extern
 */

/*-----------------------------------------------------------------------------------------
 *    Type/Structure
 */ 
typedef struct _fw_spi_local_handleMem_t{ 
	spi_master_handle_t handle;
	fw_spim_callback_t callback;
	fw_spim_entity_t entity;		
}fw_spi_local_handleMem_t;

/*-----------------------------------------------------------------------------------------
 *    Variable
 */

/*-----------------------------------------------------------------------------------------
 *    Function
 */


#endif //FW_SPI_TYPE_H_
/*-----------------------------------------------------------------------------------------
 *    End of file
 */