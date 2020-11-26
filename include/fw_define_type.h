/*-----------------------------------------------------------------------------------------
 *    File Name   :fw_define_type.h
 *    Version     :V1.0.0
 *    Create Date :2020-08-80
 *    Modufy Date :
 *    Information :
 */
#ifndef FW_DEFINE_TYPE_H_
#define FW_DEFINE_TYPE_H_
#include <stdint.h>

/*-----------------------------------------------------------------------------------------
 *    Paraneter
 */ 
#define FW_DEFINE_IO_NUMB 1
#define FW_DEFINE_USART_NUMB 5
#define FW_DEFINE_SPI_NUMB 2

/*-----------------------------------------------------------------------------------------
 *   System
 */ 
 
/* Type/Structure */
typedef struct _fw_define_system_t{
	uint32_t coreClock;
}fw_define_system_t;

/* Macro */
#define FW_DEFINE_SYSTEM_WRITE	\
{ .coreClock = FW_DEFINE_SYSTEM_CLOCK, \
}


/*-----------------------------------------------------------------------------------------
 *    IO
 */ 
/* Type/Structure */
typedef struct _fw_define_io_t{
	void* base;
	uint8_t enable;
}fw_define_io_t;

/* Macro */
#define FW_DEFINE_IO_WRITE(x)	\
{ .base = FW_DEFINE_IO##x##_BASE, \
	.enable = FW_DEFINE_USART##x##_ENABLE \
}
/*-----------------------------------------------------------------------------------------
 *    USART
 */ 
 
/* Type/Structure */
typedef struct _fw_define_usart_t{
	void* base;
	uint32_t baudrate;
	struct{
		uint8_t txd;
		uint8_t rxd;
		uint8_t rts;
		uint8_t cts;
		uint8_t sclk;
	}Pin;
	uint8_t enable;
}fw_define_usart_t;

/* Macro */
#define FW_DEFINE_USART_WRITE(x)	\
{ .base = FW_DEFINE_USART##x##_BASE, \
	.baudrate = FW_DEFINE_USART##x##_BAUDRATE, \
  .Pin = {	\
    .txd = FW_DEFINE_USART##x##_TXD, \
    .rxd = FW_DEFINE_USART##x##_RXD, \
	  .rts = FW_DEFINE_USART##x##_RTS, \
	  .cts = FW_DEFINE_USART##x##_CTS, \
	  .sclk = FW_DEFINE_USART##x##_SCLK \
	}, \
  .enable = FW_DEFINE_USART##x##_ENABLE \
}

/*-----------------------------------------------------------------------------------------
 *    SPI
 */ 

/* Type/Structure */
typedef struct _fw_define_spi_t{
	void* base;
	uint32_t clock;
	struct{
		uint8_t sck;
		uint8_t mosi;
		uint8_t miso;
		uint8_t ssel[8];
	}Pin;
	uint8_t enable;
}fw_define_spi_t;

#define FW_DEFINE_SPI_PIN_SSEL_WRITE(x) \
{ \
	FW_DEFINE_SPI##x##_SSEL0, \
  FW_DEFINE_SPI##x##_SSEL1, \
	FW_DEFINE_SPI##x##_SSEL2, \
	FW_DEFINE_SPI##x##_SSEL3, \
	FW_DEFINE_SPI##x##_SSEL4, \
	FW_DEFINE_SPI##x##_SSEL5, \
	FW_DEFINE_SPI##x##_SSEL6, \
	FW_DEFINE_SPI##x##_SSEL7, \
}

#define FW_DEFINE_SPI_PIN_WRITE(x) \
{	\
  .sck = FW_DEFINE_SPI##x##_SCK, \
  .mosi = FW_DEFINE_SPI##x##_MOSI, \
	.miso = FW_DEFINE_SPI##x##_MISO, \
	.ssel = FW_DEFINE_SPI_PIN_SSEL_WRITE(x) \
} \

/* Macro */
#define FW_DEFINE_SPI_WRITE(x)	\
{ .base = FW_DEFINE_SPI##x##_BASE, \
	.clock = FW_DEFINE_SPI##x##_CLOCK, \
  .enable = FW_DEFINE_USART##x##_ENABLE, \
	.Pin = FW_DEFINE_SPI_PIN_WRITE(x) \
}

#endif //FW_DEFINE_TYPE_H_
/*-----------------------------------------------------------------------------------------
 *    End of file
 */
