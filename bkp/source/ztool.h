//*****************************************************************************
//*			File Name		:ztool.h
//*			Version			:V1.0.1
//*			Create Date	:2019-06-10
//*			Modufy Date	:2019-06-17
//*			Information :
//*****************************************************************************
#ifndef H_ZTOOL
#define H_ZTOOL
#include "stdint.h"
#include "stdbool.h"

#ifndef null
#define null 0x0u
#endif

#ifndef NULL
#define NULL null
#endif

#ifndef high
#define high 1
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef low
#define low 0
#endif

#ifndef LOW
#define LOW 0
#endif

#ifndef output
#define output 1
#endif

//#ifndef OUTPUT
//#define OUTPUT 1
//#endif

#ifndef input
#define input 0
#endif

//#ifndef INPUT
//#define INPUT 0
//#endif

#ifndef EOF
#define EOF -1
#endif 

//**********************************************************
//*				General purpose
//**********************************************************
//-------------------------------------
//						macro
//------------------------------------- 
#ifndef isBitHigh
#define isBitHigh(src , bit) ((src & (1 << bit)) == 0) ? 0 : 1
#endif
/**
* @brief	Check the variable bit is high(1) or not.
* @param	src			: source variable.
* @param	bit			: bit of number.
* @return	1: this bit is high(1) ; 0:this bit is not high,
*/

#ifndef isBitLow
#define isBitLow(src , bit) ((src & (1 << bit)) == 0) ? 1 : 0
#endif
/**
* @brief	Check the variable bit is low(0) or not.
* @param	src			: source variable.
* @param	bit			: bit of number.
* @return	1: this bit is low(0) ; 0:this bit is not low,
*/

#ifndef setBitHigh
#define setBitHigh(src, bit) (src |= (1 << bit))
#endif
/**
* @brief	Set the variable bit to high(1).
* @param	src			: source variable.
* @param	bit			: bit of number.
* @return	none
*/

#ifndef setBitLow
#define setBitLow(src, bit) (src &= ~(1 << bit))
#endif
/**
* @brief	Set the variable bit to low(0).
* @param	src			: source variable.
* @param	bit			: bit of number.
* @return	none
*/

#ifndef isGBitHigh
#define isGBitHigh(src, status) ((src & status) == status) ? 1 : 0
#endif
/**
* @brief	Check the variable bit group all is high(1) or not.
* @param	src			: source variable.
* @param	status	: flag group variable.
* @return	1: all flag are high(1); 0: some flag are not high
*/

#ifndef isGBitLow
#define isGBitLow(src, status) ((src & status) == 0) ? 1 : 0
#endif
/**
* @brief	Check the variable bit group all is low(0) or not.
* @param	src			: source variable.
* @param	status	: flag group variable.
* @return	1: all flag are low(0); 0: some flag are not low
*/

#ifndef setGBitHigh
#define setGBitHigh(src, status) (src |= status)
#endif
/**
* @brief	Set the variable bit group to high(1).
* @param	src			: source variable.
* @param	status	: flag group variable.
* @return	none
*/

#ifndef setGBitLow
#define setGBitLow(src, status) (src &= ~status)
#endif
/**
* @brief	Set the variable bit group to low(0).
* @param	src			: source variable.
* @param	status	: flag group variable.
* @return	none
*/

//-------------------------------------
//						struct
//------------------------------------- 
typedef struct{
	void *ptr;
	/*** @brief	pointer of buffer.*/
	uint32_t size;
	/*** @brief	size of pointer bufer.*/
}Buffer_t;

typedef struct{
	void (*callback)(uint32_t status, void* userData);
	/**
	* @brief	Callback function.
	* @param	status		: Hardware status flag.
	* @param	userData	: User data pointer.
	* @return	none.
	*/
	void *userData;
	/*** @brief	User data pointer.*/
}Callback_t;

//**********************************************************
//*				Version Tool
//**********************************************************
//-------------------------------------
//						macro
//------------------------------------- 
#ifndef versionDef
#define versionDef(major, minor, build) (uint32_t)((major<<24) | (minor<<16) | (build<<0))
#endif

#ifndef objectVersionDef
#define objectVersionDef(name, version) const ObjectBuild_t name##_Version = {version, __DATE__, __TIME__}	
#endif

#ifndef getObjectVersion
#define getObjectVersion(name) name##_Version.build
#endif

#ifndef getObjectDate
#define getObjectDate(name) name##_Version.date
#endif

#ifndef getObjectTime
#define getObjectTime(name) name##_Version.time
#endif

#ifndef getObjectMajor
#define getObjectMajor(name) ((name##_Version.build & 0xFF000000) >> 24)
#endif

#ifndef getObjectMinor
#define getObjectMinor(name) ((name##_Version.build & 0x00FF0000) >> 16)
#endif

#ifndef getObjectBuild
#define getObjectBuild(name) ((name##_Version.build & 0x0000FFFF) >> 0)
#endif


//-------------------------------------
//						struct
//------------------------------------- 
typedef struct{
	uint32_t build;
	const char *date;
	const char *time;
}ObjectBuild_t;



#endif
//*****************************************************************************
//*	End of file
//*****************************************************************************
