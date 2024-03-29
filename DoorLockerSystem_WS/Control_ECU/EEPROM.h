/*
 *
 *	Name		: EEPROM.h
 *	Author		: Reem Muhammad
 *	Description	: Header file for EEPROM driver
 *  Created on: 28 Sep 2019
 *
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "std_types.h"


/*~~~~~~~ Macros ~~~~~~~~*/
#define E_OK 1
#define E_NOT_OK 0
#define READ 0b1
#define WRITE 0b0

#define DEVICE_TYPE_IDENTIFIER 0x0A

/*~~~~~~~~~~~ Type Definitions ~~~~~~~~~~~*/
typedef uint16 Eep_AddressType;
typedef uint16 Eep_LengthType;


/*~~~~~~~~~~ Functions Definitions ~~~~~~~~~~~*/
void Eep_init();
uint8 Eep_Read(Eep_AddressType EepromAddress, uint8 *DataBufferPtr, Eep_LengthType Length);
uint8 Eep_Write(Eep_AddressType EepromAddress, uint8 *DataBufferPtr, Eep_LengthType Length);

#endif /* EEPROM_H_ */
