/*-----------------------------------------------------------------------------------------
 *			File Name		:fw_define.c
 *			Version			:V1.0.0
 *			Create Date	:2020-08-80
 *			Modufy Date	:
 *			Information :
 */
 
#include <stdint.h>

#include "fw_define.h"
#include "fw_define_type.h"

#include "fw_usart_type.h"
#include "fw_spi_type.h"

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
#if FW_DEFINE_USART0_ENABLE
static uint8_t fw_usart0_handleMem[sizeof(fw_usart_local_handleMem_t)];
#define FW_DEFINE_USART0_MEM &fw_usart0_handleMem
#else
#define FW_DEFINE_USART0_MEM (void*)0x00000000
#endif

#if FW_DEFINE_USART1_ENABLE
static uint8_t fw_usart1_handleMem[sizeof(fw_usart_local_handleMem_t)];
#define FW_DEFINE_USART1_MEM &fw_usart1_handleMem
#else
#define FW_DEFINE_USART1_MEM (void*)0x00000000
#endif

#if FW_DEFINE_USART2_ENABLE
static uint8_t fw_usart2_handleMem[sizeof(fw_usart_local_handleMem_t)];
#define FW_DEFINE_USART2_MEM &fw_usart2_handleMem
#else
#define FW_DEFINE_USART2_MEM (void*)0x00000000
#endif

#if FW_DEFINE_USART3_ENABLE
static uint8_t fw_usart3_handleMem[sizeof(fw_usart_local_handleMem_t)];
#define FW_DEFINE_USART3_MEM &fw_usart3_handleMem
#else
#define FW_DEFINE_USART3_MEM (void*)0x00000000
#endif

#if FW_DEFINE_USART4_ENABLE
static uint8_t fw_usart4_handleMem[sizeof(fw_usart_local_handleMem_t)];
#define FW_DEFINE_USART4_MEM &fw_usart4_handleMem
#else
#define FW_DEFINE_USART4_MEM (void*)0x00000000
#endif
 
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
#if FW_DEFINE_SPI0_ENABLE
static uint8_t fw_spi0_handleMem[sizeof(fw_spi_local_handleMem_t)];
#define FW_DEFINE_SPI0_MEM &fw_spi0_handleMem
#else
#define FW_DEFINE_SPI0_MEM (void*)0x00000000
#endif

#if FW_DEFINE_SPI1_ENABLE
static uint8_t fw_spi1_handleMem[sizeof(fw_spi_local_handleMem_t)];
#define FW_DEFINE_SPI1_MEM &fw_spi1_handleMem
#else
#define FW_DEFINE_SPI1_MEM (void*)0x00000000
#endif

const fw_define_spi_t fw_define_spi[FW_DEFINE_SPI_NUMB] = {
	FW_DEFINE_SPI_WRITE(0),
	FW_DEFINE_SPI_WRITE(1),
};

/*-----------------------------------------------------------------------------------------
 *    End of file
 */
