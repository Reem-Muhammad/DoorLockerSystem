/*
 * EEPROM.h
 *
 *  Created on: 28 Sep 2019
 *      Author: Home
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "std_types.h"


/*~~~~~~~ Macros ~~~~~~~~*/
#define E_OK 1
#define E_NOT_OK 0

#define DEVICE_TYPE_IDENTIFIER 0x0A

/*~~~~~~~~~~~ Type Definitions ~~~~~~~~~~~*/
typedef uint16 Eep_AddressType;
typedef uint16 Eep_LengthType;
typedef struct
{
	/*
	 * EEPROM size
	 * maximum block sizes (write, erase) processed within the job processing function
	 * maximum read block sizes for normal and fast EEPROM mode processed within the job processing function
	 * call cycle of cyclic job processing function for read, write, erase
	 */
	Eep_AddressType Eep_Base_Address;
}Eep_ConfigType;


/*~~~~~~~~~~ Functions Definitions ~~~~~~~~~~~*/
void Eep_init();
uint8 Eep_Read(Eep_AddressType EepromAddress, uint8 *DataBufferPtr, Eep_LengthType Length);
uint8 Eep_Write(Eep_AddressType EepromAddress, uint8 *DataBufferPtr, Eep_LengthType Length);

#endif /* EEPROM_H_ */
