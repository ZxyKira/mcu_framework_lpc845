/*-----------------------------------------------------------------------------------------
 *    File Name   : fw_usart.c
 *    Version     : V1.0.0
 *    Create Date : 2020-09-02
 *    Modufy Date : 2020-10-07
 *    Information :
 */
#include "fsl_iocon.h"
#include "fsl_usart.h"
#include "fsl_swm.h"
#include "fsl_swm_connections.h"

#include "fw_io.h"
#include "fw_usart.h"

#include "fw_usart_type.h"
#include "fw_define_type.h"
#include "fw_chip_type.h"

/*-----------------------------------------------------------------------------------------
 *    Extern Function/Variable
 */
extern const fw_define_usart_t fw_define_usart[FW_DEFINE_USART_NUMB];

/*-----------------------------------------------------------------------------------------
 *    Local Type/Structure
 */
typedef struct _fw_usart_hwInfo_t{
	clock_select_t clockSel;
	struct{
		swm_select_movable_t txd;
		swm_select_movable_t rxd;
		swm_select_movable_t rts;
		swm_select_movable_t cts;
		swm_select_movable_t sclk;
	}pinMoveable;
}fw_usart_hwInfo_t;

/*-----------------------------------------------------------------------------------------
 *    Local Variable
 */
static const fw_usart_hwInfo_t fw_usart_hwInfo[FW_DEFINE_USART_NUMB] = {
	{
		.clockSel = kUART0_Clk_From_MainClk,
		.pinMoveable = {
			.txd  = kSWM_USART0_TXD,
			.rxd  = kSWM_USART0_RXD,
			.rts  = kSWM_USART0_RTS,
			.cts  = kSWM_USART0_CTS,
			.sclk = kSWM_USART0_SCLK
		}
	},
	{
		.clockSel = kUART1_Clk_From_MainClk,
		.pinMoveable = {
			.txd  = kSWM_USART1_TXD,
			.rxd  = kSWM_USART1_RXD,
			.rts  = kSWM_USART1_RTS,
			.cts  = kSWM_USART1_CTS,
			.sclk = kSWM_USART1_SCLK
		}
	},
	{
		.clockSel = kUART2_Clk_From_MainClk,
		.pinMoveable = {
			.txd  = kSWM_USART2_TXD,
			.rxd  = kSWM_USART2_RXD,
			.rts  = kSWM_USART2_RTS,
			.cts  = kSWM_USART2_CTS,
			.sclk = kSWM_USART2_SCLK
		}
	},
	{
		.clockSel = kUART3_Clk_From_MainClk,
		.pinMoveable = {
			.txd  = kSWM_USART3_TXD,
			.rxd  = kSWM_USART3_RXD,
			.rts  = kSWM_MOVABLE_NUM_FUNCS,
			.cts  = kSWM_MOVABLE_NUM_FUNCS,
			.sclk = kSWM_USART3_SCLK
		}
	},
	{
		.clockSel = kUART4_Clk_From_MainClk,
		.pinMoveable = {
			.txd  = kSWM_USART4_TXD,
			.rxd  = kSWM_USART4_RXD,
			.rts  = kSWM_MOVABLE_NUM_FUNCS,
			.cts  = kSWM_MOVABLE_NUM_FUNCS,
			.sclk = kSWM_USART4_SCLK
		}
	},
};

/*-----------------------------------------------------------------------------------------
 *    Local Inline Function
 */ 
static inline const fw_define_usart_t* const fw_usart_getHwConf(uint8_t ch){
	assert(ch < FW_DEFINE_USART_NUMB);
	return &fw_define_usart[ch];
}

static inline fw_usart_local_handleMem_t* fw_usart_getHandleMem(uint8_t ch){
	assert(ch <	FW_DEFINE_USART_NUMB);
	return (fw_usart_local_handleMem_t*)fw_define_usart[ch].handleMem;
}

static inline const fw_usart_hwInfo_t* const fw_usart_getHwInfo(uint8_t ch){
	assert(ch < FW_DEFINE_USART_NUMB);
	return (fw_usart_hwInfo_t*)&fw_usart_hwInfo[ch];
}

/*-----------------------------------------------------------------------------------------
 *    Local Function
 */ 
static void fw_usart_setPin(uint8_t ch){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	const fw_usart_hwInfo_t* const pHwInfo = fw_usart_getHwInfo(ch);
	int i;
	
	uint32_t pinMod =  IOCON_MODE_PULLUP | IOCON_PIO_HYS_MASK;
	
	CLOCK_EnableClock(kCLOCK_Swm);
	
	
	if(pHwConf->Pin.txd < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.txd,  (swm_port_pin_type_t)(pHwConf->Pin.txd));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.txd, pinMod);
	}
	
	if(pHwConf->Pin.rxd < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.rxd, (swm_port_pin_type_t)(pHwConf->Pin.rxd));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.rxd, pinMod);
	}
	
	if(pHwConf->Pin.rts < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.rts, (swm_port_pin_type_t)(pHwConf->Pin.rts));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.rts, pinMod);
	}
		
	if(pHwConf->Pin.cts < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.cts, (swm_port_pin_type_t)(pHwConf->Pin.cts));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.cts, pinMod);
	}
		
	if(pHwConf->Pin.sclk < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.sclk, (swm_port_pin_type_t)(pHwConf->Pin.sclk));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.sclk, pinMod);
	}
	
	CLOCK_DisableClock(kCLOCK_Swm);
}

/*-----------------------------------------------------------------------------------------
 *    Global Variable
 */
const fw_usart_api_t fw_usart_api;
const fw_usart_entity_api_t fw_usart_entity_api;

/*-----------------------------------------------------------------------------------------
 *    Local Function
 */
static void fw_usart_callback_handle(USART_Type *base, usart_handle_t *handle, status_t status, void *userData){
	fw_usart_local_handleMem_t *pHandle = (fw_usart_local_handleMem_t*)userData;
	
  if (kStatus_USART_TxIdle == status){
		pHandle->Flag.txBusy = false;
		if(pHandle->txCallback.foo!=NULL)
			pHandle->txCallback.foo(pHandle->entity, pHandle->txCallback.userData);
  }

  if (kStatus_USART_RxIdle == status){
		pHandle->Flag.rxBusy = false;
		if(pHandle->txCallback.foo!=NULL)
			pHandle->rxBallback.foo(pHandle->entity, pHandle->rxBallback.userData);
  }
}

/*-----------------------------------------------------------------------------------------
 *    Function - API
 */
static bool fw_usart_init(uint8_t ch){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	fw_usart_local_handleMem_t *pHandle = fw_usart_getHandleMem(ch);
	const fw_usart_hwInfo_t* const pHwInfo = fw_usart_getHwInfo(ch);
	
	usart_config_t config = {0};
	
	/* SPIM not enable */
	assert(pHwConf->enable != 0);
	
	/* Select SPI source clock. */
	CLOCK_Select(pHwInfo->clockSel);

	/* Clear hardware memory */
	memset(pHwConf->handleMem, 0x00, sizeof(fw_usart_local_handleMem_t));
	
	USART_GetDefaultConfig(&config);
  config.baudRate_Bps = pHwConf->baudrate;
  config.enableRx     = true;
  config.enableTx     = true;
  USART_Init(pHwConf->base, &config, CLOCK_GetFreq(kCLOCK_MainClk));
	fw_usart_setPin(ch);
	
	USART_TransferCreateHandle(pHwConf->base, &pHandle->handle, fw_usart_callback_handle, pHandle);
	
	return true;
}

static void fw_usart_deinit(uint8_t ch){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	/* Hardware deinit */
	USART_Deinit(pHwConf->base);	
	
	/* Clear Handle memory to 0x00 */
	memset(pHwConf->handleMem, 0x00, sizeof(fw_usart_local_handleMem_t));
	
	return;
}

static fw_usart_entity_t fw_usart_getEntity(uint8_t ch){
	fw_usart_entity_t result = {
		.pAPI = &fw_usart_entity_api,
		.ch = ch
	};
	return result;
}

static bool fw_usart_reciver(uint8_t ch, fw_usart_xfer_t *xfer){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	fw_usart_local_handleMem_t *pHandle = fw_usart_getHandleMem(ch);
	
	if(pHandle->Flag.rxBusy)
		return false;
	
	pHandle->Flag.rxBusy = true;
	pHandle->rxBallback = xfer->callback;
	pHandle->xferReciver.data = xfer->data;
	pHandle->xferReciver.dataSize = xfer->len;
	USART_TransferReceiveNonBlocking(pHwConf->base, &pHandle->handle, &pHandle->xferReciver, NULL);
	return true;
}

static bool fw_usart_send(uint8_t ch, fw_usart_xfer_t *xfer){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	fw_usart_local_handleMem_t *pHandle = fw_usart_getHandleMem(ch);
	
	if(pHandle->Flag.txBusy)
		return false;	
	
	pHandle->Flag.txBusy = true;
	pHandle->txCallback = xfer->callback;
	pHandle->xferSend.data = (uint8_t*)xfer->data;
	pHandle->xferSend.dataSize = xfer->len;
	USART_TransferSendNonBlocking(pHwConf->base, &pHandle->handle, &pHandle->xferSend);
	return true;
}

static void fw_usart_abortReceive(uint8_t ch){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	fw_usart_local_handleMem_t *pHandle = fw_usart_getHandleMem(ch);
	
	if(pHandle->Flag.rxBusy)
		USART_TransferAbortReceive(pHwConf->base, &pHandle->handle);
}

static void fw_usart_abortSend(uint8_t ch){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	fw_usart_local_handleMem_t *pHandle = fw_usart_getHandleMem(ch);
	
	if(pHandle->Flag.rxBusy)
		USART_TransferAbortSend(pHwConf->base, &pHandle->handle);
}

//--------Setting--------
static bool fw_usart_setting_setBaudrate(uint8_t ch, uint32_t baudrate){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	//fw_usart_local_handleMem_t *pHandle = getHandleMem(ch);
	
	if(USART_SetBaudRate(pHwConf->base, baudrate, CLOCK_GetFreq(kCLOCK_MainClk)) == kStatus_Success)
		return true;
	else
		return false;
}

static void fw_usart_setting_ringBufferEnable(uint8_t ch, void* buffer, uint32_t size){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	fw_usart_local_handleMem_t *pHandle = fw_usart_getHandleMem(ch);
	
	USART_TransferStartRingBuffer(pHwConf->base, &pHandle->handle, buffer, size);
}

static void fw_usart_setting_ringBufferDisable(uint8_t ch){
	const fw_define_usart_t* const pHwConf = fw_usart_getHwConf(ch);
	fw_usart_local_handleMem_t *pHandle = fw_usart_getHandleMem(ch);
	
	USART_TransferStopRingBuffer(pHwConf->base, &pHandle->handle);
}
///--------Setting--------END

/*-----------------------------------------------------------------------------------------
 *    API Link - API
 */
const fw_usart_api_t fw_usart_api = {
	.init 				= fw_usart_init,
	.deinit 			= fw_usart_deinit,
	.getEntity 		= fw_usart_getEntity,
	.reciver 			= fw_usart_reciver,
	.send 				= fw_usart_send,
	.abortReceive = fw_usart_abortReceive,
	.abortSend		= fw_usart_abortSend,
	.Setting = {
		.setBaudrate       = fw_usart_setting_setBaudrate,
		.ringBufferEnable  = fw_usart_setting_ringBufferEnable,
		.ringBufferDisable = fw_usart_setting_ringBufferDisable,
	},
};

/*-----------------------------------------------------------------------------------------
 *    Function - Entity API
 */
static inline bool fw_usart_entity_reciver(fw_usart_entity_t entity, fw_usart_xfer_t *xfer){
	return fw_usart_reciver(entity.ch, xfer);
}

static inline bool fw_usart_entity_send(fw_usart_entity_t entity, fw_usart_xfer_t *xfer){
	return fw_usart_send(entity.ch, xfer);
}

static inline void fw_usart_entity_abortReceive(fw_usart_entity_t entity){
	fw_usart_abortReceive(entity.ch);
}

static inline void fw_usart_entity_abortSend(fw_usart_entity_t entity){
	fw_usart_abortSend(entity.ch);
}

static inline bool fw_usart_entity_setBaudrate(fw_usart_entity_t entity, uint32_t baudrate){
	return fw_usart_setting_setBaudrate(entity.ch, baudrate);
}

/*-----------------------------------------------------------------------------------------
 *    API Link - Entity API
 */
const fw_usart_entity_api_t fw_usart_entity_api = {
	.reciver 				= fw_usart_entity_reciver,
	.send 					= fw_usart_entity_send,
	.abortReceive		= fw_usart_entity_abortReceive,
	.abortSend			= fw_usart_entity_abortSend,
	.setBaudrate		= fw_usart_entity_setBaudrate,
};

/*-----------------------------------------------------------------------------------------
 *    End of file
 */
