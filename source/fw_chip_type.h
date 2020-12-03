/* *****************************************************************************************
 *    File Name   :fw_chip_type.h
 *    Create Date :2020-10-13
 *    Modufy Date :
 *    Information :
 */

#ifndef fw_chip_type_H_
#define fw_chip_type_H_

#include "fsl_swm.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

/* *****************************************************************************************
 *    Parameter
 */ 

/* *****************************************************************************************
 *    Type/Structure
 */ 
typedef struct _fw_spi_hwInfo_t{
	uint32_t maxOfHwSselNumb;
	clock_select_t clockSel;
	struct{
		swm_select_movable_t mosi;
		swm_select_movable_t miso;
		swm_select_movable_t sck;
		swm_select_movable_t ssel[4];
	}pinMoveable;
}fw_spi_hwInfo_t;


/* *****************************************************************************************
 *    Extern Function/Variable
 */


#ifdef __cplusplus
}
#endif //__cplusplus
#endif //fw_chip_type_H_
/* *****************************************************************************************
 *    End of file
 */
 