/*
 * EEPROM.c
 *
 *  Created on: 28 Sep 2019
 *      Author: Home
 */


#include "EEPROM.h"
/**/
#include "I2C.h"
#include "LCD.h"

#define TWI_OWN_ADDRESS 0b00000100

Twi_ConfigType s_Twi_Config = {TWI_PRESCALER_VALUE_1, GENERAL_CALL_RECOGNITION_ENABLED,TWI_OWN_ADDRESS, 2};

/*-----------------------------------------------
 * [Function Name]: Eep_init
 * [Description]: initializes the I2C interface for the EEPROM
 * [Args]: None
 * [Return]: None
 ------------------------------------------------*/
void Eep_init()
{
	TWI_init(&s_Twi_Config);

}

/*-----------------------------------------------
 * [Function Name]: Eep_Read
 * [Description]: reads the specified number of bytes from the EEPROM
 * [Args]:
 * 		EepromAddress:Address in EEPROM (assuming the EEPROM has 0 as base address)
 * 		DataBufferPtr: Pointer to destination data buffer in RAM
 * 		Length: Number of bytes to read
 * [Return]:
 * 		E_OK: success
 * 		E_NOT_OK: failure
 ------------------------------------------------*/
uint8 Eep_Read(Eep_AddressType EepromAddress, uint8 *DataBufferPtr, Eep_LengthType Length)
{
	uint8 DataBufferIndex = 0;

	TWI_start();

	/*
	 * Send the device select code to write in the EEPROM:
	 * b7	b6	b	b4		b3	 b2	 b1	 b0
	 *<--identifier-->   	A10  A9  A8  W
	 *
	 *
	 *  EepromAddress: 0000 0XXX XXXX XXXX
	 *	&	0x700: 	   0000 0111 0000 0000
	 *				=  0000 0XXX 0000 0000  >> 7 = 0000 0000 0000 XXX0
	 *										     +                   0 (WRITE)
	 *										     = 0000 0000 0000 XXX0
	 *										     |           1010 0000 (DEVICE_TYPE_IDENTIFIER<<4)
	 *										     = 0000 0000 1010 XXX0
	 */
	TWI_write( (DEVICE_TYPE_IDENTIFIER<<4) | ( ( (EepromAddress & 0x0700) >>7 ) + WRITE) );

	/*Send a memory location to read from (A7:A0)*/
	TWI_write( (uint8)EepromAddress );

	/*Repeated start*/
	TWI_start();

	/*send the device select code to read from the EEPROM*/
	TWI_write( (DEVICE_TYPE_IDENTIFIER<<4) | ( ( (EepromAddress & 0x0700) >>7 ) + READ) );

	/*Read the specified number of bytes, and store them in the data buffer*/
	for(DataBufferIndex = 0; DataBufferIndex <Length; DataBufferIndex++ )
	{
		DataBufferPtr[DataBufferIndex] = TWI_readWithACK();
	}

	TWI_stop();

	return E_OK;		//<<<<<<<<<<<<<<<<<<<<<<<<<
}


/*-----------------------------------------------
 * [Function Name]: Eep_Write
 * [Description]: writes the specified number of bytes into the EEPROM
 * [Args]:
 * 		EepromAddress:Address in EEPROM (assuming the EEPROM has 0 as base address)
 * 		DataBufferPtr: Pointer to source data
 * 		Length: Number of bytes to write
 * [Return]:
 * 		E_OK: success
 * 		E_NOT_OK: failure
 ------------------------------------------------*/
uint8 Eep_Write(Eep_AddressType EepromAddress, uint8 *DataBufferPtr, Eep_LengthType Length)
{
	uint8 DataBufferIndex = 0;

	TWI_start();
	//LCD_goToRowCol(0,0);		//debug
	//LCD_displayString("Start ");	//debug

	TWI_write( (DEVICE_TYPE_IDENTIFIER<<4) | ( ( (EepromAddress & 0x0700) >>7 ) + WRITE) );
	//LCD_displayString("Sel ");	//debug

	TWI_write( (uint8)EepromAddress );
	//LCD_displayString("Loc");	//debug

	for(DataBufferIndex = 0; DataBufferIndex <Length; DataBufferIndex++ )
	{
		TWI_write( DataBufferPtr[DataBufferIndex] );
		//LCD_displayInt(DataBufferIndex);	//debug
	}
	//LCD_displayInt(TWI_STATUS);		//debug
	TWI_stop();
	//LCD_displayString("stop");	//debug --> not displayed --> reason: polling

	return E_OK;

}
