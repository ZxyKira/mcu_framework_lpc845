/*-----------------------------------------------------------------------------------------
 *    File Name   :fw_chip.h
 *    Version     :V1.0.0
 *    Create Date :2020-10-07
 *    Modufy Date :
 *    Information :
 */
#ifndef FW_CHIP_H_
#define FW_CHIP_H_
#include "fw_io.h"
#include "fw_usart.h"
#include "fw_spi.h"

#ifdef __cplusplus
extern "C"{
#endif 
/*-----------------------------------------------------------------------------------------
 *    Extern Function/Variable
 */
extern const fw_io_api_t fw_io_api;
extern const fw_io_entity_api_t fw_io_entity_api;

extern const fw_spim_api_t fw_spim_api;
extern const fw_spim_entity_api_t fw_spim_entity_api;

extern const fw_usart_api_t fw_usart_api;
extern const fw_usart_entity_t fw_usart_entity;

extern void fw_chip_init(void);

/*-----------------------------------------------------------------------------------------
 *    Function
 */

/*-----------------------------------------------------------------------------------------
 *    Variable
 */
 
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //FW_CHIP_H_
/*-----------------------------------------------------------------------------------------
 *    End of file
 */
 