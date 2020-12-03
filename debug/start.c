/*****************************************************************************
 *			File Name		 : start.c
 *			Version			 : V1.0
 *			Create Date	 : 2020-10-06
 *      Modufy Date  : 2020-10-06
 *      Author       : Danny
 *			Information  :
 *****************************************************************************/
#include <stdint.h>
#include "fw_lpc845.h"


/*****************************************************************************
 *	Extern Function/Variable
 */

/*****************************************************************************
 *	Local Type/Structure
 */ 

/*****************************************************************************
 *	Local Function/Variable
 */

/*****************************************************************************
 *	Local Function
 */
fw_io_handle_t io;
fw_io_memory_t ioMemory;

fw_io_entity_handle_t pin[8];

/*****************************************************************************
 *	Public Function
 */

void start(){
	fw_chip_api.chipInit();
	io = fw_chip_api.Construct.io(0, &ioMemory);
	
	io.API->init(io);
	
	io.API->dir(io, 1, 0xFFFFFFFF);
	int i;
	for(i=0; i<8; i++){
		pin[i] = io.API->getEntity(io, NULL, 1, i);
	}
}

/*****************************************************************************
 *	End of file
 */
