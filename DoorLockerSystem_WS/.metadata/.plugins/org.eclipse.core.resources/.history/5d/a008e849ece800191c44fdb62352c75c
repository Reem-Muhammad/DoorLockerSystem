/*
 * I2C.c
 *
 *  Created on: 26 Sep 2019
 *      Author: Home
 */


#include "I2C.h"

void TWI_init(const Twi_ConfigType* config_ptr)
{
	/*
	 * ****************************************
	 * 1- set SCL frequency: TWBR		??	_/
	 *
	 * 2- enable acknowledge bit: TWEA		_/
	 * 3- enable TWI interface: TWEN		_/
	 * 4- enable module interrupt in case you are not using polling: TWIE	XX
	 *
	 * 5- set the prescaler: TWPS		??	_/
	 *
	 * 6- set its slave address: TWA	_/
	 * 7- enable/ disable recognition to general calls: TWGCE	_/
	 *
	 * - clear TWINT
	 * ****************************************
	 */

	/*Enable TWI interface, Acknowledge*/
	TWCR |= (1<<TWEN) | (1<<TWEA);

	/*Set the address*/
	TWAR = (TWAR & 0x01) | (config_ptr->twi_address << 1);

	/*configure the behavior on general calls*/
	TWAR |= config_ptr->e_twi_general_call_recognition;

	/*set the prescaler*/
	TWSR = (TWSR & 0xFC) | (config_ptr->e_twi_prescaler);

	/*set the bit rate register*/
	TWBR = config_ptr->twi_bit_rate;
}


void TWI_start()
{
	/*************************
	 * TWCR should have the following value:
	 * TWINT	TWEA	TWSTA	TWSTO	TWWC	TWEN	-	TWIE
	 * 	1		 X		 1		 0		 X		 1		0	 X
	 *************************/

	/*generate the START condition to claim the bus*/
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	/*TWINT will be set after successful transmission of the START condition*/
	while(BIT_IS_CLEAR(TWCR, TWINT));

	/*TWSTA would be cleared at the beginning of the other functions (it should be cleared after transmission of START condition and it is not cleared automatically),
	 *  so there is no need to clear it here.
	 */
	//TWCR &= ~(1<<TWSTA);
}



void TWI_write(const uint8 byte_to_write) /*made it const since the function shouldn't try to edit the value*/
{
	/*write the data/slave address to the data register TWDR*/
	TWDR = byte_to_write;

	/*******************
	 * TWCR should have the following value:
	 * TWINT	TWEA	TWSTA	TWSTO	TWWC	TWEN	-	TWIE
	 * 	1		 X		 0		 0		 X		 1		0	 X
	 *******************/
	TWCR = (1<<TWINT) | (1<<TWEN);



	/*wait until TWINT indicates the transmission of SLA+W*/
	while( BIT_IS_CLEAR(TWCR, TWINT) );
}

uint8 TWI_readWithACK()
{
	/*******************
	 * TWCR should have the following value:
	 * TWINT	TWEA	TWSTA	TWSTO	TWWC	TWEN	-	TWIE
	 * 	1		 1		 0		 0		 X		 1		0	 X
	 *
	 * 	setting TWINT will clear the flag
	 * 	setting TWEA will generate the ACK pulse when data is received
	 *******************/
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);

	/*wait until the byte is received, then send ACK*/
	while( BIT_IS_CLEAR(TWCR, TWINT) );

	/*return the received byte*/
	return TWDR;
}

uint8 TWI_readWithNACK()
{
	/*******************
	 * TWCR should have the following value:
	 * TWINT	TWEA	TWSTA	TWSTO	TWWC	TWEN	-	TWIE
	 * 	1		 0		 0		 0		 X		 1		0	 X
	 *
	 * 	setting TWINT will clear the flag
	 * 	clearing TWEA will prevent the generation of the ACK pulse when data is received
	 *******************/
	TWCR = (1<<TWINT) | (1<<TWEN);

	/*wait until the byte is received, and no ACK would be sent*/
	while( BIT_IS_CLEAR(TWCR, TWINT) );

	/*return the received byte*/
	return TWDR;
}


void TWI_stop()
{
	/*******************
	 * TWCR should have the following value:
	 * TWINT	TWEA	TWSTA	TWSTO	TWWC	TWEN	-	TWIE
	 * 	1		 X		 0		 1		 X		 1		0	 X
	 *******************/
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);

	/*wait until the STOP condition is transmitted*/
	//while( BIT_IS_CLEAR(TWCR, TWINT) );
}
