/*****************************************************************************
 *			File Name		 : hw_pin.c
 *			Version			 : V1.0
 *			Create Date	 : 2020-10-06
 *      Modufy Date  : 2020-10-06
 *      Author       : Danny
 *			Information  :
 */

#include <stdint.h>

#include "fsl_swm.h"
typedef enum _hw_pin_func_t{
	HW_PIN_FUNC_PULL_UP,
	HW_PIN_FUNC_PULL_UP_,
	HW_PIN_FUNC_PULL_DOWN,
	HW_PIN_FUNC_PULL_OPEN,
}hw_pin_func_t;


typedef struct _hw_pin_t{
	swm_select_movable_t moveable;
	uint8_t iocon;
}hw_pin_t;