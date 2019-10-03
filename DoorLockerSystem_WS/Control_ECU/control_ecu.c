/*
 * control_ecu.c
 *
 *  Created on: 29 Sep 2019
 *      Author: Home
 */


#include "LCD.h"
#include "UART.h"
#include "EEPROM.h"

#define PASSWORD_LENGTH 5
#define CONFIRMATION_FAILED 0
#define CONFIRMATION_PASSED 1


const Uart_ConfigType s_UartConfig = {PARITY_DISABLED, ONE_STOP_BIT, CHAR_8, 9600};
uint8 data_buffer[PASSWORD_LENGTH] = {0};
uint8 confirm_buffer[PASSWORD_LENGTH] = {0};

void saveNewPass()
{
	for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
	{
		data_buffer[i] = UART_receiveByte();
		LCD_displayInt(data_buffer[i]);
	}

	Eep_Write(0, data_buffer, PASSWORD_LENGTH);
	//LCD_displayInt(8888); //debug

}

void confirmPass()
{
	/*receive the confirmation password entered by the user*/

	for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
	{
		confirm_buffer[i] = UART_receiveByte();
	}

	/*compare it to the saved password*/
	Eep_Read(0, data_buffer, PASSWORD_LENGTH);
	for(uint8 i = 0; i<PASSWORD_LENGTH; i++)
	{
		if (confirm_buffer[i] != data_buffer[i])
		{

			UART_sendByte(CONFIRMATION_FAILED);
			LCD_displayInt(22);
			return;
		}
	}
	UART_sendByte(CONFIRMATION_PASSED);
	LCD_displayInt(33);
}

int main()
{
	LCD_init();
	UART_init(&s_UartConfig);
	Eep_init();
	saveNewPass();
	confirmPass();

	while(1)
	{



	}
}
