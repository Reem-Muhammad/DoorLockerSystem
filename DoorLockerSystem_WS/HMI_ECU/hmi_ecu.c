/*
 * hmi_ecu.c
 *
 *  Created on: 29 Sep 2019
 *      Author: Home
 */


#include "keypad.h"
#include "LCD.h"
#include "UART.h"

#define PASSWORD_LENGTH 5
#define CONFIRMATION_FAILED 0
#define CONFIRMATION_PASSED 1

uint8 password_counter = 0;
uint8 key = 0;
const Uart_ConfigType s_UartConfig = {PARITY_DISABLED, ONE_STOP_BIT, CHAR_8, 9600};

void setNewPass()
{
	LCD_goToRowCol(0,0);
	LCD_displayString("Please Enter New Pass:");
	LCD_goToRowCol(1,0);
	LCD_displayString("-----");

	password_counter = 0;
	LCD_goToRowCol(1,0);
	while(password_counter < PASSWORD_LENGTH)
	{
		key = keypad_getPressedKey();
		UART_sendByte(key);
		_delay_ms(400);
		LCD_displayCharacter('*');
		password_counter++;
	}
	LCD_goToRowCol(0,0);
	LCD_displayString("Press 'C' To Enter.         ");
	key = keypad_getPressedKey();
	while(key != 'C')
	{
		key = keypad_getPressedKey();

	}
	_delay_ms(400); //if not introduced, the first digit in the confirmation password would be 'C'
	//LCD_clear();
}


/*Password Confirmation*/
uint8 confirmPass()
{
	LCD_goToRowCol(0,0);
	LCD_displayString("Please Re-enter The Pass:");
	LCD_goToRowCol(1,0);
	LCD_displayString("-----");

	password_counter = 0;
	LCD_goToRowCol(1,0);
	/*send the confirmation password to the control ECU*/
	while(password_counter < PASSWORD_LENGTH)
	{
		key = keypad_getPressedKey();
		UART_sendByte(key);
		_delay_ms(400);
		LCD_displayCharacter('*');
		password_counter++;
	}

	LCD_goToRowCol(0,0);
	LCD_displayString("Press 'C' To Enter.         ");
	key = keypad_getPressedKey();
	while(key != 'C')
	{
		key = keypad_getPressedKey();
	}
	_delay_ms(400);
	LCD_displayCharacter('U');
	uint8 confirm = UART_receiveByte(); //doesn't return --> reason: Eep_Write() wasn't returning due to the polling in TWI_stop()

	return confirm;//UART_receiveByte();
}

int main()
{
	LCD_init();
	UART_init(&s_UartConfig);

	setNewPass();

#if 0
	if ( conf == CONFIRMATION_PASSED)
	{
		LCD_goToRowCol(0,0);
		LCD_displayString("Done              ");
	}
	else
	{
		LCD_goToRowCol(0,0);
		LCD_displayString("Confirmation Failed           ");
		_delay_ms(500);

	}
#else

	while (confirmPass() == CONFIRMATION_FAILED)
	{
		LCD_goToRowCol(0,0);
		LCD_displayString("Confirmation Failed           ");
		_delay_ms(500);

		setNewPass();
		_delay_ms(10);
	}

	LCD_goToRowCol(0,0);
	LCD_displayString("Done              ");
#endif

	while(1)
	{


	}
}
