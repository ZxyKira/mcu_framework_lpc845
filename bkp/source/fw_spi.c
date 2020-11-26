/*-----------------------------------------------------------------------------------------
 *    File Name   : fw_spi.c
 *    Version     : V1.0.0
 *    Create Date : 2020-08-25
 *    Modufy Date : 2020-10-07
 *    Information :
 */
#include "fw_io.h"
#include "fw_spi.h"

#include "fsl_swm.h"
#include "fsl_swm_connections.h"
#include "fsl_spi.h"
#include "ztool.h"

#include "fw_define_type.h"
#include "fw_spi_type.h"
#include "fw_io_type.h"
#include "fw_chip_type.h"

/*-----------------------------------------------------------------------------------------
 *    Extern Function/Variable
 */
extern const fw_define_spi_t fw_define_spi[FW_DEFINE_SPI_NUMB];

/*-----------------------------------------------------------------------------------------
 *    Local Type/Structure
 */
typedef struct _fw_spim_hwInfo_t{
	uint32_t maxOfHwSselNumb;
	clock_select_t clockSel;
	struct{
		swm_select_movable_t mosi;
		swm_select_movable_t miso;
		swm_select_movable_t sck;
		swm_select_movable_t ssel[4];
	}pinMoveable;
}fw_spi_hwInfo_t;

/*-----------------------------------------------------------------------------------------
 *    Local Variable
 */
static bool fw_spim_isBusy(uint8_t ch);
static fw_spim_entity_t fw_spim_getEntity(uint8_t ch, uint8_t ssel);

static const fw_spi_hwInfo_t fw_spi_hwInfo[2] = {
	{	
		.maxOfHwSselNumb = 4,
		.clockSel = kSPI0_Clk_From_MainClk,
		.pinMoveable = {
			.mosi = kSWM_SPI0_MOSI, 
			.miso = kSWM_SPI0_MISO, 
			.sck = kSWM_SPI0_SCK, 
			.ssel = {kSWM_SPI0_SSEL0, kSWM_SPI0_SSEL1, kSWM_SPI0_SSEL2, kSWM_SPI0_SSEL3}
		},
	},
	{
		.maxOfHwSselNumb = 2,
		.clockSel = kSPI1_Clk_From_MainClk,
		.pinMoveable = {
			.mosi = kSWM_SPI1_MOSI, 
			.miso = kSWM_SPI1_MISO, 
			.sck = kSWM_SPI1_SCK, 
			.ssel = {kSWM_SPI1_SSEL0, kSWM_SPI1_SSEL1, kSWM_MOVABLE_NUM_FUNCS, kSWM_MOVABLE_NUM_FUNCS}
		},
	}
};

/*-----------------------------------------------------------------------------------------
 *    Global Variable
 */
const fw_spim_api_t fw_spim_api;
const fw_spim_entity_api_t fw_spim_entity_api;

/*-----------------------------------------------------------------------------------------
 *    Local Inline Function
 */ 
static inline const fw_define_spi_t* const fw_spi_getHwConf(uint8_t ch){
	assert(ch < FW_DEFINE_SPI_NUMB);
	return &fw_define_spi[ch];
}

static inline fw_spi_local_handleMem_t* fw_spi_getHandleMem(uint8_t ch){
	assert(ch < FW_DEFINE_SPI_NUMB);
	return (fw_spi_local_handleMem_t*)fw_define_spi[ch].handleMem;
}

static inline const fw_spi_hwInfo_t* const fw_spi_getHwInfo(uint8_t ch){
	assert(ch < FW_DEFINE_SPI_NUMB);
	return (fw_spi_hwInfo_t*)&fw_spi_hwInfo[ch];
}

/*-----------------------------------------------------------------------------------------
 *    Local Function
 */
static void fw_spim_setSSEL(uint8_t ch, uint8_t sselNumb){
	const fw_define_spi_t* const pHwConf = fw_spi_getHwConf(ch);
	const fw_spi_hwInfo_t* const pHwInfo = fw_spi_getHwInfo(ch);
	
	SPI_Type* hwBase = pHwConf->base;
	
	/* Unset all hardware ssel */
	hwBase->TXCTL &= 0xFFF0FFFF;

	
	if(sselNumb >= (pHwInfo->maxOfHwSselNumb-1)){
		uint32_t sselMask = ~(1 << (16 + (pHwInfo->maxOfHwSselNumb-1)));
		sselMask &= 0x000F0000;
		
		/* Set hardware ssel */
		hwBase->TXCTL |= sselMask;
		
		CLOCK_EnableClock(kCLOCK_Swm);
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.ssel[3], (swm_port_pin_type_t)(pHwConf->Pin.ssel[sselNumb]));
		CLOCK_DisableClock(kCLOCK_Swm);
		
		
	}else{
		/* Set hardware ssel */
		uint32_t sselMask = ~(1 << (16 + (sselNumb)));
		sselMask &= 0x000F0000;
		
		hwBase->TXCTL |= sselMask;
	}
}

static void fw_spim_setPin(uint8_t ch){
	const fw_define_spi_t* const pHwConf = fw_spi_getHwConf(ch);
	const fw_spi_hwInfo_t* const pHwInfo = fw_spi_getHwInfo(ch);
	int i;
	
	uint32_t pinMod =  IOCON_MODE_PULLUP | IOCON_PIO_HYS_MASK;
	
	CLOCK_EnableClock(kCLOCK_Swm);
	
	if(pHwConf->Pin.sck < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.sck,  (swm_port_pin_type_t)(pHwConf->Pin.sck));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.sck, pinMod);
	}
	
	if(pHwConf->Pin.miso < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.miso, (swm_port_pin_type_t)(pHwConf->Pin.miso));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.miso, pinMod);
	}
	
	if(pHwConf->Pin.miso < FW_DEFINE_IO_MAX_PIN_NUMB){
		SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.mosi, (swm_port_pin_type_t)(pHwConf->Pin.mosi));
		fw_chip_iocon_pinMuxSet(pHwConf->Pin.mosi, pinMod);
	}
	
	for(i=0; i<pHwInfo->maxOfHwSselNumb; i++){
		if(pHwConf->Pin.ssel[i] < FW_DEFINE_IO_MAX_PIN_NUMB){
			SWM_SetMovablePinSelect(SWM0, pHwInfo->pinMoveable.ssel[i], (swm_port_pin_type_t)(pHwConf->Pin.ssel[i]));
			fw_chip_iocon_pinMuxSet(pHwConf->Pin.ssel[i], pinMod);
		}
	}
	
	CLOCK_DisableClock(kCLOCK_Swm);
}

static void fw_spim_callback_handle(SPI_Type *base, spi_master_handle_t *handle, status_t status, void *userData){
	fw_spi_local_handleMem_t *pHandle = userData;
	
	if(pHandle->callback.foo != 0)
		pHandle->callback.foo(pHandle->entity, pHandle->callback.userData);
	
	return;
}

/*-----------------------------------------------------------------------------------------
 *    API Function
 */
static bool fw_spim_init(uint8_t ch){
	const fw_define_spi_t* const pHwConf = fw_spi_getHwConf(ch);
	fw_spi_local_handleMem_t *pHandle = fw_spi_getHandleMem(ch);
	const fw_spi_hwInfo_t* const pHwInfo = fw_spi_getHwInfo(ch);

	spi_master_config_t config = {0};
	

	/* Select SPI source clock. */
	CLOCK_Select(pHwInfo->clockSel);
	
	/* Clear hardware memory */
	memset(pHandle, 0x00, sizeof(fw_spi_local_handleMem_t));
	
	/* Get hardware  */
	SPI_MasterGetDefaultConfig(&config);
	
	config.baudRate_Bps = pHwConf->clock;
	config.sselNumber   = kSPI_Ssel0Assert;
	
	SPI_MasterInit(pHwConf->base, &config, CLOCK_GetFreq(kCLOCK_MainClk));
	
	SPI_MasterTransferCreateHandle(pHwConf->base, &pHandle->handle, fw_spim_callback_handle, pHandle);
	
	fw_spim_setPin(ch);
	pHandle->entity = fw_spim_getEntity(ch, 0);
	
	return true;
}
	
static void fw_spim_deinit(uint8_t ch){
	const fw_define_spi_t* const pHwConf = fw_spi_getHwConf(ch);
	fw_spi_local_handleMem_t *pHandle = fw_spi_getHandleMem(ch);
	
	/* Hardware deinit */
	SPI_Deinit(pHwConf->base);	
	
	/* Clear Handle memory to 0x00 */
	memset(pHandle, 0x00, sizeof(fw_spi_local_handleMem_t));
	
	return;
}
	
static fw_spim_entity_t fw_spim_getEntity(uint8_t ch, uint8_t ssel){
	assert(ssel < FW_DEFINE_SPI_SSEL_MAX_NUMB);
	
	fw_spim_entity_t result = {
		.pAPI = &fw_spim_entity_api,
		.ch = ch,
		.ssel = ssel
	};

	return result;
}

static bool fw_spim_xfer(uint8_t ch, uint8_t ssel, fw_spim_xfer_t *pXfer){
	if(fw_spim_isBusy(ch))
		return false;
	
	const fw_define_spi_t* const pHwConf = fw_spi_getHwConf(ch);
	fw_spi_local_handleMem_t *pHandle = fw_spi_getHandleMem(ch);
	
	SPI_Type* hwBase = pHwConf->base;
	
	uint32_t localResult = 0;
	
	/* No setting baudrate in config check */
	assert(pXfer->config.baudrate != 0);
	
	SPI_SetTransferDelay(hwBase, (const spi_delay_config_t*)&pXfer->config.Delay);
	
	localResult = SPI_MasterSetBaudRate(hwBase, pXfer->config.baudrate, CLOCK_GetFreq(kCLOCK_MainClk));
	
	/* Write hardware config to registor */
	hwBase->CFG = pXfer->config.hwSetting | 0x00000005;
	
	/* Clear Transfer finish flag */
	hwBase->TXCTL &= ~(1<<SPI_TXCTL_EOT_SHIFT);
	
	pHandle->entity.ssel = ssel;
	
	fw_spim_setSSEL(ch, ssel);
	
	spi_transfer_t xfer = {
		.txData = (uint8_t*)pXfer->Data.pTx,
		.rxData = (uint8_t*)pXfer->Data.pRx,
		.dataSize = pXfer->Data.len,
		.configFlags = 	kSPI_EndOfTransfer
	};
	pHandle->callback = pXfer->callback;
	pHandle->entity = fw_spim_getEntity(ch, ssel);
	
	
	/* Create spi master transfer handle  */
	status_t handleResult = SPI_MasterTransferNonBlocking(hwBase, &pHandle->handle, &xfer);
	
	if(handleResult == kStatus_Success)
		return true;
	
	else
		return false;
}

static bool fw_spim_isBusy(uint8_t ch){
	fw_spi_local_handleMem_t *pHandle = fw_spi_getHandleMem(ch);
	
	if(pHandle->handle.state == kStatus_SPI_Busy)
		return true;
	else
		return false;
	
}
 
static fw_spim_config_t fw_spim_getDefaultConfig(uint8_t ch){
	const fw_define_spi_t* const pHwConf = fw_spi_getHwConf(ch);
	
	fw_spim_config_t result = {0};
	result.baudrate = pHwConf->clock;
	result.hwSetting = 0x00000005;
	return result;
}

static void fw_spim_config_cpha(fw_spim_config_t* config, bool enable){
	(enable)?
	(config->hwSetting |=  SPI_CFG_CPHA_MASK):
	(config->hwSetting &= ~SPI_CFG_CPHA_MASK);
}

static void fw_spim_config_cpol(fw_spim_config_t* config, bool enable){
	(enable)?
	(config->hwSetting |=  SPI_CFG_CPOL_MASK):
	(config->hwSetting &= ~SPI_CFG_CPOL_MASK);
}
	
static void fw_spim_config_lsb(fw_spim_config_t* config, bool enable){
	(enable)?
	(config->hwSetting |=  SPI_CFG_LSBF_MASK):
	(config->hwSetting &= ~SPI_CFG_LSBF_MASK);
}
			
static void fw_spim_config_loop(fw_spim_config_t* config, bool enable){
	(enable)?
	(config->hwSetting |=  SPI_CFG_LOOP_MASK):
	(config->hwSetting &= ~SPI_CFG_LOOP_MASK);
}
	
static void fw_spim_config_preDelay(fw_spim_config_t* pConfig, uint8_t val){
	if(val>=8)
		val = 7;
	
	pConfig->Delay.preDelay = val;
}
				
static void fw_spim_config_postDelay(fw_spim_config_t* pConfig, uint8_t val){
	if(val>=8)
		val = 7;
	
	pConfig->Delay.postDelay = val;
}
		
static void fw_spim_config_frameDelay(fw_spim_config_t* pConfig, uint8_t val){
	if(val>=8)
		val = 7;
	
	pConfig->Delay.frameDelay = val;
}
					
static void fw_spim_config_transferDelay(fw_spim_config_t* pConfig, uint8_t val){
	if(val>=8)
		val = 7;
	
	pConfig->Delay.transferDelay = val;
}

/*-----------------------------------------------------------------------------------------
 *    API Link - API
 */
const fw_spim_api_t fw_spim_api = {
  .init             = fw_spim_init,
	.deinit           = fw_spim_deinit,
	.getEntity        = fw_spim_getEntity,
	.xfer             = fw_spim_xfer,
	.isBusy           = fw_spim_isBusy,
	.getDefaultConfig = fw_spim_getDefaultConfig,
	.Config = {
		.cpha          = fw_spim_config_cpha,
		.cpol          = fw_spim_config_cpol,
		.lsb           = fw_spim_config_lsb,
		.loop          = fw_spim_config_loop,
		.preDelay      = fw_spim_config_preDelay,
		.postDelay     = fw_spim_config_postDelay,
		.frameDelay    = fw_spim_config_frameDelay,
		.transferDelay = fw_spim_config_transferDelay
	}
};

/*-----------------------------------------------------------------------------------------
 *    Function - Entity API
 */ 
bool fw_spim_entity_xfer(const fw_spim_entity_t entity, fw_spim_xfer_t *pXfer){
	return fw_spim_xfer(entity.ch, entity.ssel, pXfer);
}
    
bool fw_spim_entity_isBusy(const fw_spim_entity_t entity){
	return fw_spim_isBusy(entity.ch);
}
    
fw_spim_config_t fw_spim_entity_getDefaultConfig(const fw_spim_entity_t entity){
	return fw_spim_getDefaultConfig(entity.ch);
}
  
/*-----------------------------------------------------------------------------------------
 *    API Link - Entity API
 */
const fw_spim_entity_api_t fw_spim_entity_api = {
	.xfer = fw_spim_entity_xfer,
	.isBusy = fw_spim_entity_isBusy,
	.getDefaultConfig = fw_spim_entity_getDefaultConfig,
	.Config = {
		.cpha          = fw_spim_config_cpha,
		.cpol          = fw_spim_config_cpol,
		.lsb           = fw_spim_config_lsb,
		.loop          = fw_spim_config_loop,
		.preDelay      = fw_spim_config_preDelay,
		.postDelay     = fw_spim_config_postDelay,
		.frameDelay    = fw_spim_config_frameDelay,
		.transferDelay = fw_spim_config_transferDelay,
	}
};
/*-----------------------------------------------------------------------------------------
 *    End of File
 */
