/*-----------------------------------------------------------------------------------------
 *    File Name   :fw_chip.c
 *    Version     :V1.0.0
 *    Create Date :2020-10-07
 *    Modufy Date :
 *    Information :
 */
#include <stdint.h>

#include "LPC845.h"
#include "fsl_power.h"
#include "fsl_clock.h"

#include "fw_define_type.h"
#include "fw_io_type.h"
#include "fw_chip_type.h"
/*-----------------------------------------------------------------------------------------
 *    Extern Function/Variable
 */
extern const fw_define_system_t fw_define_system;
/*-----------------------------------------------------------------------------------------
 *    Local Type/Structure
 */ 

/*-----------------------------------------------------------------------------------------
 *    Local Function/Variable
 */
const uint8_t fw_io_ioconMap[FW_DEFINE_IO_MAX_PIN_NUMB] = {
//00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
	17, 11,  6,  5,  4,  3, 16, 15, 14, 13,  8,  7,  2,  1, 18, 10,  9,  0, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 50, 51, 35,
//32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53
	36, 37, 38, 41, 42, 43, 46, 49, 31, 32, 55, 54, 33, 34, 39, 40, 44, 45, 47, 48, 52, 53,
};


/*-----------------------------------------------------------------------------------------
 *    Local Function
 */
void fw_chip_initCoreClock(uint32_t clock){
	/*!< Ensure FRO is on  */
	POWER_DisablePD(kPDRUNCFG_PD_FRO_OUT);      
	
	/*!< Ensure FRO is on  */
	POWER_DisablePD(kPDRUNCFG_PD_FRO);         

	/*!< Set up FRO freq */	
	CLOCK_SetFroOscFreq((clock_fro_osc_freq_t)clock);   
	
	/*!< Set FRO clock source */
	CLOCK_SetFroOutClkSrc(kCLOCK_FroSrcFroOsc);        
	
	/*!< Ensure Main osc is on */
	POWER_DisablePD(kPDRUNCFG_PD_SYSOSC);       
	
	/*!< Set main osc freq */
	CLOCK_InitSysOsc(12000000U);                
	
	/*!< select fro for main clock */
	CLOCK_SetMainClkSrc(kCLOCK_MainClkSrcFro);  
	CLOCK_SetCoreSysClkDiv(1U);
	
	/*!< Set SystemCoreClock variable. */
	SystemCoreClock = clock;	
}



/*-----------------------------------------------------------------------------------------
 *    Public Function
 */
void fw_chip_init(void){
	fw_chip_initCoreClock(fw_define_system.coreClock);
}

/*-----------------------------------------------------------------------------------------
 *    End of file
 */
