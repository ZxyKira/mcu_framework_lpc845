/*-----------------------------------------------------------------------------------------
 *    File Name   :fw_usart_type.c
 *    Version     :V1.0.0
 *    Create Date :2020-10-07
 *    Modufy Date :
 *    Information :
 */
#ifndef fw_usart_TYPE_H_
#define fw_usart_TYPE_H_

/* Chip */
#include "fsl_usart.h"

/* Framework */
#include "fw_usart.h"

/*-----------------------------------------------------------------------------------------
 *    Extern
 */

/*-----------------------------------------------------------------------------------------
 *    Type/Structure
 */ 
typedef struct _fw_usart_local_handleMem_t{
	usart_transfer_t 		xferSend;
	usart_transfer_t 		xferReciver;
	fw_usart_callback_t 	txCallback;
	fw_usart_callback_t 	rxBallback;
	fw_usart_entity_t 	entity;
	usart_handle_t handle;
	struct{
		bool txBusy : 1;
		bool rxBusy : 1;
	}Flag;
}fw_usart_local_handleMem_t;

/*-----------------------------------------------------------------------------------------
 *    Variable
 */

/*-----------------------------------------------------------------------------------------
 *    Function
 */


#endif //fw_usart_TYPE_H_
/*-----------------------------------------------------------------------------------------
 *    End of file
 */